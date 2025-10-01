/*
 * SPDX-FileCopyrightText: 2025 mhl6829
 * SPDX-License-Identifier: MIT
 * File: [gt1000.c] - GT1000 MIDI SysEx driver
 */

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

#include "gt1000.h"
#include "gt1000_param.h"
#include "sysex.h"

#define MAX_SYSEX_LENGTH                          64

#define PATCH_NUMBER_OFFSET                       0x00000000
#define PATCH_NAME_OFFSET                         0x10000000
#define PATCH_EFFECT_OFFSET                       0x10001200

#define MANUFACTURER_ID                           0x41
#define MODEL_ID_1                                0x00
#define MODEL_ID_2                                0x00
#define MODEL_ID_3                                0x00
#define MODEL_ID_4                                0x4F

#define RQ1_COMMAND                               0x11
#define DT1_COMMAND                               0x12


#define MESSAGE_HANDLER_TASK_STACK_SIZE           2048
#define MESSAGE_HANDLER_TASK_PRIORITY             5

#define TAG "GT1000"

static gt1000_t device = {0};

static QueueHandle_t message_queue;
static TaskHandle_t handler_task;

static uint8_t device_id = 0x7F;

static gt1000_callback_t callback = NULL;

static const uint8_t dt1_header[] = {
    0xF0,
    MANUFACTURER_ID,
    0xFF, // Device ID
    MODEL_ID_1,
    MODEL_ID_2,
    MODEL_ID_3,
    MODEL_ID_4,
    DT1_COMMAND,
};

static const uint8_t rq1_header[] = {
    0xF0,
    MANUFACTURER_ID,
    0xFF, // Device ID
    MODEL_ID_1,
    MODEL_ID_2,
    MODEL_ID_3,
    MODEL_ID_4,
    RQ1_COMMAND,
};

static const uint8_t notification_enable_sequence[] = {
    0xF0,
    MANUFACTURER_ID,
    0xFF, // Device ID
    MODEL_ID_1,
    MODEL_ID_2,
    MODEL_ID_3,
    MODEL_ID_4,
    DT1_COMMAND,
    0x7F,
    0x00,
    0x00,
    0x01,
    0x01,
    0x7F,
    0xF7
};

static const uint8_t notification_disable_sequence[] = {
    0xF0,
    MANUFACTURER_ID,
    0xFF, // Device ID
    MODEL_ID_1,
    MODEL_ID_2,
    MODEL_ID_3,
    MODEL_ID_4,
    DT1_COMMAND,
    0x7F,
    0x00,
    0x00,
    0x01,
    0x00,
    0x00,
    0xF7
};

static inline bool is_valid_param_addr(const gt1000_param_addr_t parameter) {
    const uint8_t *param_addr = (uint8_t *)parameter;
    const uint8_t *base_addr = (uint8_t *)&device.effect;
    const uint8_t *end_addr = base_addr + sizeof(gt1000_effect_t);
    return (param_addr >= base_addr) && (param_addr < end_addr);
}

static inline bool is_valid_dev_addr(const uint32_t address) {
    const uint32_t start_addr = PATCH_EFFECT_OFFSET;
    const uint32_t end_addr = start_addr + sizeof(gt1000_effect_t);
    return (address >= start_addr) && (address < end_addr);
}

static inline uint32_t param_addr_to_dev_addr(const gt1000_param_addr_t parameter) {
    uint32_t param_offset = (uint32_t)parameter - (uint32_t)&device.effect;
    return PATCH_EFFECT_OFFSET + param_offset;
}

static inline gt1000_param_addr_t dev_addr_to_param_addr(uint32_t addr) {
    uint32_t param_offset = addr - PATCH_EFFECT_OFFSET;
    return (uint8_t *)((uint32_t)&device.effect + param_offset);
}

static inline uint8_t calculate_checksum(uint8_t *buffer, int length) {
    uint64_t sum = 0;
    for (int i = 0; i < length; ++i) {
        sum += buffer[i];
    }
    return (128 - (sum % 128)) % 128;
}

static void handle_dt1(uint32_t dev_addr, uint8_t *data, int length) {
    gt1000_event_t event = UNHANDLED;
    switch (dev_addr)
    {
        case PATCH_NUMBER_OFFSET:
            memcpy(&device.patch_number, data, length);
            event = PRESET_CHANGE;
            break;
        case PATCH_NAME_OFFSET:
            snprintf(device.patch_name, 16, "%.*s", length, (const char*)data);
            event = PRESET_NAME_UPDATE;
            break;
        default:
            if (!is_valid_dev_addr(dev_addr))
            {
                break;
            }
            gt1000_param_addr_t addr = dev_addr_to_param_addr(dev_addr);
            memcpy(addr, data, length);
            event = PARAMETER_UPDATE;
            break;
    }
    if (event != UNHANDLED && callback) {
        callback(event);
    }
    vTaskDelay(1);
}

static void handle_sysex_message(uint8_t *message, int length) {

    int err = 0;
    // Check minimum length
    if (length < sizeof(dt1_header)) {
        err = -1;
        goto handle_invalid_message;
    }
    
    // Check dt1 header
    int idx = 0;
    for (int i = 0; i < sizeof(dt1_header); ++i) {
        if (dt1_header[idx] == 0xFF && message[idx] == device_id) {
            ++idx;
            continue;
        }

        if (dt1_header[idx] != message[idx]) {
            err = -2;
            goto handle_invalid_message;
        }
        ++idx;
    }

    // Validate checksum
    uint8_t checksum = calculate_checksum(message + idx, length - sizeof(dt1_header) - 2);
    if (message[length - 2] != checksum) {
        err = -3;
        goto handle_invalid_message;
    }

    // Assemble device address
    uint32_t dev_addr = 0;
    for (int i = 0; i < 4; ++i) {
        dev_addr = dev_addr << 8;
        dev_addr |= (message[idx++] & 0xFF);
    }

    uint8_t *data_start = message + idx;
    int data_length = length - sizeof(dt1_header) - 6;

    handle_dt1(dev_addr, data_start, data_length);

    return;

handle_invalid_message:
    ESP_LOGD(TAG, "Got invalid SysEx message: %d", err);
    return;
}

static void handle_message_task(void *pvParameter)
{
    sysex_buffer_t *buffer;
    for (;;)
    {
        if(xQueueReceive(message_queue, &buffer, portMAX_DELAY)) {
            handle_sysex_message(buffer->data, buffer->length);
            sysex_free_buffer(buffer);
        }
    }
}

QueueHandle_t gt1000_init() {
    message_queue = xQueueCreate(8, sizeof(sysex_buffer_t *));

    if (!message_queue) {
        ESP_LOGE(TAG, "Failed to create message queue.");
        return NULL;
    }

    xTaskCreate(handle_message_task,
                "handle_message",
                MESSAGE_HANDLER_TASK_STACK_SIZE,
                NULL,
                MESSAGE_HANDLER_TASK_PRIORITY,
                &handler_task);

    return message_queue;
}

void gt1000_set_device_id(uint8_t id) {
    device_id = id;
}

gt1000_t *gt1000_get_device(void) {
    return &device;
}

static void gt1000_send_dt1(uint32_t dev_addr, uint32_t value, size_t size) {
    int err = 0;
    int msg_length = sizeof(dt1_header) + 4 + size + 2;
    if (msg_length > MAX_SYSEX_LENGTH) {
        err = -3;
        goto handle_invalid_parameter;
    }

    if (size > 4) {
        err = -4;
        goto handle_invalid_parameter;
    }

    uint8_t message[MAX_SYSEX_LENGTH];
    
    // Write DT1 header
    memcpy(message, dt1_header, sizeof(dt1_header));
    
    // Overwrite device id
    message[2] = device_id;
    uint8_t *data_start = message + sizeof(dt1_header);
    
    // Write target address
    int data_offset = 0;
    *(data_start + data_offset++) = (dev_addr >> 24) & 0xFF;
    *(data_start + data_offset++) = (dev_addr >> 16) & 0xFF;
    *(data_start + data_offset++) = (dev_addr >> 8) & 0xFF;
    *(data_start + data_offset++) = dev_addr & 0xFF;

    for (int i = 0; i < size; ++i) {
        *(data_start + data_offset++) = (value >> (8 * (size - 1 - i))) & 0xFF;
    }
    
    // Write checksum
    message[msg_length - 2] = calculate_checksum(data_start, size + 4);
    
    // Write EOX
    message[msg_length - 1] = 0xF7;

    sysex_send(message, msg_length);
    return;

handle_invalid_parameter:
    ESP_LOGE(TAG, "Failed to send dt1: %d", err);
    return;
}

void gt1000_set_parameter(gt1000_param_addr_t parameter, uint32_t value) {
    int err = 0;
    
    if (!is_valid_param_addr(parameter))
    {
        err = -1;
        goto handle_invalid_parameter;
    }

    gt1000_param_t param;
    if (!gt1000_get_parameter_info(&param, parameter)) {
        err = -2;
        goto handle_invalid_parameter;
    }

    size_t size = param.size;

    uint32_t dev_addr = param_addr_to_dev_addr(parameter);

    gt1000_send_dt1(dev_addr, value, size);
    return;

handle_invalid_parameter:
    ESP_LOGE(TAG, "Invalid parameter: %d", err);
    return;
}

static void gt1000_send_rq1(uint32_t dev_addr, size_t size) {
    int msg_length = sizeof(rq1_header) + 4 + 4 + 2;
    uint8_t message[msg_length];
    
    // Write RQ1 header
    memcpy(message, rq1_header, sizeof(rq1_header));
    
    // Overwrite device id
    message[2] = device_id;
    uint8_t *data_start = message + sizeof(rq1_header);
    
    // Write target address
    int data_offset = 0;
    *(data_start + data_offset++) = (dev_addr >> 24) & 0xFF;
    *(data_start + data_offset++) = (dev_addr >> 16) & 0xFF;
    *(data_start + data_offset++) = (dev_addr >> 8) & 0xFF;
    *(data_start + data_offset++) = dev_addr & 0xFF;
    
    // Write size
    *(data_start + data_offset++) = (size >> 24) & 0xFF;
    *(data_start + data_offset++) = (size >> 16) & 0xFF;
    *(data_start + data_offset++) = (size >> 8) & 0xFF;
    *(data_start + data_offset++) = size & 0xFF;
    
    // Write checksum
    message[msg_length - 2] = calculate_checksum(data_start, 8);
    
    // Write EOX
    message[msg_length - 1] = 0xF7;

    sysex_send(message, msg_length);
    return;
}

void gt1000_update_block(gt1000_param_addr_t parameter) {
    int err = 0;
    if (!is_valid_param_addr(parameter))
    {
        err = -1;
        goto handle_invalid_parameter;
    }

    uint32_t dev_addr = param_addr_to_dev_addr(parameter);
    
    if ((dev_addr & 0xFF) != 0x00) {
        err = -2;
        goto handle_invalid_parameter;
    }

    gt1000_send_rq1(dev_addr, EFFECT_BLOCK_SIZE);
    return;

handle_invalid_parameter:
    ESP_LOGE(TAG, "Invalid parameter: %d", err);
    return;
}

void gt1000_update_parameter(gt1000_param_addr_t parameter) {
    int err = 0;
    if (!is_valid_param_addr(parameter))
    {
        err = -1;
        goto handle_invalid_parameter;
    }

    gt1000_param_t param;
    if (!gt1000_get_parameter_info(&param, parameter)) {
        err = -2;
        goto handle_invalid_parameter;
    }

    uint32_t dev_addr = param_addr_to_dev_addr(parameter);
    
    size_t size = param.size;

    gt1000_send_rq1(dev_addr, size);
    return;

handle_invalid_parameter:
    ESP_LOGE(TAG, "Invalid parameter: %d", err);
    return;
}

void gt1000_update_patch_name() {
    gt1000_send_rq1(PATCH_NAME_OFFSET, 16);
    return;
}

void gt1000_register_callback(gt1000_callback_t cbk) {
    callback = cbk;
    return;
}

void gt1000_enable_notifications() {
    int msg_length = sizeof(notification_enable_sequence);
    uint8_t message[msg_length];
    memcpy(message, notification_enable_sequence, msg_length);
    message[2] = device_id;
    sysex_send(message, msg_length);
    ESP_LOGI(TAG, "Parameter change notification enabled");
    return;
}

void gt1000_disable_notifications() {
    int msg_length = sizeof(notification_disable_sequence);
    uint8_t message[msg_length];
    memcpy(message, notification_disable_sequence, msg_length);
    message[2] = device_id;
    sysex_send(message, msg_length);
    ESP_LOGI(TAG, "Parameter change notification disabled");
}