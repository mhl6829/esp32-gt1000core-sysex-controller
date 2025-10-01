/*
 * SPDX-FileCopyrightText: 2025 mhl6829
 * SPDX-License-Identifier: MIT
 * File: [sysex.c] - SysEx Parser and Sync/Async message router
 */

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

#include "sysex.h"
#include "uart.h"

#define SYSEX_MESSAGE_BUFFER_SIZE         8
#define SYSEX_QUEUE_SIZE                  256
#define SYSEX_BUFFER_SIZE                 512
#define SYSEX_TASK_STACK_SIZE             4096
#define SYSEX_TASK_PRIORITY               5
#define SYSEX_IDENTITY_REQUEST_LEN        6
#define SYSEX_IDENTITY_REPLY_LEN          15

#define TAG "SYSEX"

typedef struct {
    uint8_t * buffer;
    int length;
    SemaphoreHandle_t completion;
} sync_request_t;

static sysex_buffer_t sysex_buffer_pool[SYSEX_MESSAGE_BUFFER_SIZE];

static QueueHandle_t device_message_queue;

static QueueHandle_t parser_queue;
static TaskHandle_t parser_task;

static parser_callback_t parser_cbk;

static sync_request_t *g_sync_request;
static SemaphoreHandle_t g_sync_request_mutex;

static bool is_callback_ready = false;

static const uint8_t identity_request[] = {
    0xF0,                       // Status
    0x7E,                       // ID (Universal Non-realtime)
    0x7F,                       // Device ID (Broadcast)
    0x06, 0x01,                 // Sub ID #1, #2
    0xF7,
};

static const uint8_t identity_reply_pattern[] = {
    0xF0,                       // Status
    0x7E,                       // ID (Universal Non-realtime)
    0x00,                       // Device ID
    0x06, 0x02,                 // Sub ID #1, #2
    0x00,                       // Manufacturer ID
    0x00, 0x00, 0x00, 0x00,     // Family code #1, #2
    0x00, 0x00, 0x00, 0x00,     // Software version
    0xF7,
};

void sysex_free_buffer(sysex_buffer_t *buffer) {
    buffer->in_use = false;
}

static bool is_identity_reply(uint8_t *buf, int length) {
    if (length != SYSEX_IDENTITY_REPLY_LEN) {
        return false;
    }

    for (int i = 0; i < length; ++i) {
        if (identity_reply_pattern[i] == 0x00) {
            continue;
        }

        if (buf[i] != identity_reply_pattern[i]) {
            return false;
        }
    }

    return true;
}

static void handle_sysex_message(uint8_t *buf, int length) {
    bool handled_as_sync = false;
    // Handle synchronous message
    if (xSemaphoreTake(g_sync_request_mutex, 10))
    {
        if (g_sync_request != NULL) {
            sync_request_t *request = g_sync_request;

            if (is_identity_reply(buf, length))
            {
                memcpy(request->buffer, buf, length);
                request->length = length;
                
                g_sync_request = NULL;
                xSemaphoreGive(request->completion);
                
                handled_as_sync = true;
            }
        }
        xSemaphoreGive(g_sync_request_mutex);
    }

    // Handle asynchronous message
    if (!handled_as_sync) {
        // Find free buffer
        bool buffer_available = false;
        for (int i = 0; i < SYSEX_MESSAGE_BUFFER_SIZE; ++i)
        {
            sysex_buffer_t *pool_entry = &sysex_buffer_pool[i];
            if (!pool_entry->in_use)
            {
                memcpy(pool_entry->data, buf, length);
                pool_entry->length = length;
                pool_entry->in_use = true;
                
                BaseType_t result = xQueueSend(device_message_queue, &pool_entry, pdMS_TO_TICKS(100));
                vTaskDelay(1);
                if (result != pdPASS) {
                    ESP_LOGW(TAG, "Failed to send to device message queue. Queue full?");
                }

                buffer_available = true;
                break;
            }
        }
        if (!buffer_available) {
            ESP_LOGW(TAG, "Buffer pool not available.");
        }
    }
}

static void sysex_parse_task(void *pvParameter)
{
    uint8_t buffer[SYSEX_BUFFER_SIZE];
    bool in_sysex = false;
    int length = 0;
    uint8_t byte;
    for (;;) {
        if(xQueueReceive(parser_queue, &byte, portMAX_DELAY)) {
            switch (byte) {
                case 0xFE:
                    // active sensing
                    break;
                case 0xF7:
                    // EOX
                    if (in_sysex) {
                        buffer[length++] = byte;
                        
                        handle_sysex_message(buffer, length);

                        length = 0;
                        in_sysex = false;
                    } else {
                        ESP_LOGD(TAG, "Unexpected EOX byte.");
                    }
                    break;
                case 0xF0:
                    // system excusive start
                    if (!in_sysex) {
                        in_sysex = true;
                        buffer[length++] = byte;
                    } else {
                        ESP_LOGD(TAG, "Unexpected SysEx start byte. Message discarded.");
                        length = 0;
                        buffer[length++] = byte;
                    }
                    break;
                default:
                    if (in_sysex) {
                        buffer[length++] = byte;
                    }
                    break;
            }
            if (length >= SYSEX_BUFFER_SIZE) {
                ESP_LOGE(TAG, "SysEx buffer overflow");
                in_sysex = false;
                length = 0;
            }
        }
    }
}

QueueHandle_t sysex_init()
{
    g_sync_request_mutex = xSemaphoreCreateMutex();
    if (!g_sync_request_mutex) {
        ESP_LOGE(TAG, "Failed to create internal synchronous request mutex.");
        return NULL;
    }

    parser_queue = xQueueCreate(256, sizeof(uint8_t));
    if (!parser_queue) {
        ESP_LOGE(TAG, "Failed to create parser queue.");
        goto cleanup;
    }

    xTaskCreate(sysex_parse_task,
                "sysex_parser",
                SYSEX_TASK_STACK_SIZE,
                NULL,
                SYSEX_TASK_PRIORITY,
                &parser_task);

    return parser_queue;

cleanup:
    vSemaphoreDelete(g_sync_request_mutex);
    return NULL;
}

void sysex_register_device_message_queue(QueueHandle_t queue) {
    device_message_queue = queue;
}

void sysex_start_parsing()
{
    is_callback_ready = true;
}

void sysex_stop_parsing()
{
    is_callback_ready = false;
}

int sysex_send(const uint8_t *message, int length)
{
    return uart_send(message, length);
}

static bool _sysex_device_inquiry(sysex_identity_reply *identity, TickType_t timeout) {
    if (g_sync_request_mutex == NULL) {
        ESP_LOGE(TAG, "Not initialized yet");
        return false;
    }

    sync_request_t *request;
    uint8_t *buffer;

    if (xSemaphoreTake(g_sync_request_mutex, pdMS_TO_TICKS(100)))
    {
        if (g_sync_request != NULL)
        {
            ESP_LOGE(TAG, "Another sync request is already in progress");
            xSemaphoreGive(g_sync_request_mutex);
            return false;
        }

        request = malloc(sizeof(sync_request_t));
        buffer = malloc(SYSEX_IDENTITY_REPLY_LEN);
    
        if (!buffer || !request) {
            ESP_LOGE(TAG, "Out of memory for request");
            xSemaphoreGive(g_sync_request_mutex);
            return false;
        }

        request->buffer = buffer;
        request->completion = xSemaphoreCreateBinary();

        g_sync_request = request;
        xSemaphoreGive(g_sync_request_mutex);
    }
    else
    {
        ESP_LOGE(TAG, "Failed to aquire sync request mutex");
        return false;
    }

    int res = sysex_send(identity_request, SYSEX_IDENTITY_REQUEST_LEN);
    if (res < 0) {
        ESP_LOGE(TAG, "Failed to send identity request message");
        goto cleanup;
    }

    if (xSemaphoreTake(request->completion, timeout)) {
        *identity = (sysex_identity_reply) {
            .dev_id = buffer[2],
            .manufacturer_id = buffer[5],
            .dev_family_code_1 = buffer[6],
            .dev_family_code_2 = buffer[7],
            .dev_family_num_1 = buffer[8],
            .dev_family_num_2 = buffer[9],
            .sw_rev_lv_1 = buffer[10],
            .sw_rev_lv_2 = buffer[11],
            .sw_rev_lv_3 = buffer[12],
            .sw_rev_lv_4 = buffer[13],
        };

        vSemaphoreDelete(request->completion);
        free(buffer);
        request->buffer = NULL;
        free(request);

        return true;
    } else {
        ESP_LOGD(TAG, "Synchronous request timed out");
    }

cleanup:
    if (xSemaphoreTake(g_sync_request_mutex, portMAX_DELAY)) {
        g_sync_request = NULL;
        xSemaphoreGive(g_sync_request_mutex);
    }
    
    vSemaphoreDelete(request->completion);
    free(buffer);
    request->buffer = NULL;
    free(request);

    return false;
}

bool sysex_device_inquiry(sysex_identity_reply *identity, TickType_t timeout, int retry) {
    if (!_sysex_device_inquiry(identity, timeout))
    {
        for (int i = 0; i < retry; ++i)
        {
            ESP_LOGE(TAG, "Failed to send identity request. Retrying...");
            if (_sysex_device_inquiry(identity, timeout))
            {
                return true;
            }
        }
    }
    ESP_LOGE(TAG, "Failed to connect");
    return false;
}

void sysex_deinit()
{
    parser_cbk = NULL;
    vTaskDelete(parser_task);
    vSemaphoreDelete(g_sync_request_mutex);
    return;
}