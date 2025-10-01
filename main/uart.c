/*
 * SPDX-FileCopyrightText: 2025 mhl6829
 * SPDX-License-Identifier: MIT
 * File: [uart.c] - UART driver and byte message producer
 */

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"
#include "freertos/semphr.h"
#include "driver/uart.h"
#include "esp_log.h"

#include "uart.h"

#define UART_NUM                    UART_NUM_0
#define UART_BAUDRATE               31250
#define UART_TX_PIN                 21
#define UART_RX_PIN                 20

#define UART_RX_BUF_SIZE            256
#define UART_TX_BUF_SIZE            0
#define UART_MSG_BUF_SIZE           32

#define UART_QUEUE_SIZE             20

#define UART_TASK_STACK_SIZE        2048
#define UART_TASK_PRIORITY          10

#define UART_MAX_CONSUMERS          10

#define TAG "UART"


static QueueHandle_t consumers[UART_MAX_CONSUMERS];
static size_t consumer_count = 0;
static SemaphoreHandle_t consumer_mutex;

static QueueHandle_t uart_queue;
static TaskHandle_t uart_task;


int uart_send(const uint8_t *data, int len)
{
    return uart_write_bytes(UART_NUM, data, len);
}

static void dispatch_byte(uint8_t *bytes, int len)
{
    if (xSemaphoreTake(consumer_mutex, portMAX_DELAY)) {
        for (int i = 0; i < UART_MAX_CONSUMERS; ++i) {
            if (consumers[i] == NULL) {
                continue;
            }
            for (int j = 0; j < len; ++j) {
                BaseType_t result = xQueueSend(consumers[i], &bytes[j], pdMS_TO_TICKS(100));
                if (result != pdPASS) {
                    ESP_LOGW(TAG, "Failed to send to consumer %d queue. Queue full?", i);
                }
            }
        }
        vTaskDelay(1);
        xSemaphoreGive(consumer_mutex);
    }
}

static void uart_receive_task()
{
    ESP_LOGD(TAG, "UART Receive task start");
    uart_event_t event;
    uint8_t msg[UART_MSG_BUF_SIZE];
    for (;;)
    {
        if (xQueueReceive(uart_queue, (void *)&event, portMAX_DELAY)) {
            switch (event.type) {
                case UART_DATA:
                    int len = uart_read_bytes(UART_NUM, (void *)&msg, event.size, pdMS_TO_TICKS(20));
                    if (len > 0)
                    {
                        dispatch_byte(msg, len);
                    }
                    break;
                case UART_BUFFER_FULL:
                case UART_FIFO_OVF:
                    ESP_LOGW(TAG, "Rx buffer overflow");
                    uart_flush_input(UART_NUM);
                    xQueueReset(uart_queue); 
                    break;

                case UART_FRAME_ERR:
                    ESP_LOGW(TAG, "UART Frame error");
                    break;

                case UART_PARITY_ERR:
                    ESP_LOGW(TAG, "Parity error");
                    break;

                default:
                    break;
            }
        }
    }
}

void uart_driver_init()
{
    uart_config_t uart_config = {
        .baud_rate = UART_BAUDRATE,
        .data_bits = UART_DATA_8_BITS,
        .parity    = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .source_clk = UART_SCLK_DEFAULT,
    };

    ESP_ERROR_CHECK(uart_param_config(UART_NUM, &uart_config));

    ESP_ERROR_CHECK(uart_set_pin(UART_NUM,
                                UART_TX_PIN,
                                UART_RX_PIN,
                                UART_PIN_NO_CHANGE,
                                UART_PIN_NO_CHANGE));
    
    ESP_ERROR_CHECK(uart_driver_install(UART_NUM,
                                        UART_RX_BUF_SIZE,
                                        UART_TX_BUF_SIZE,
                                        UART_QUEUE_SIZE,
                                        &uart_queue,
                                        0));

    consumer_mutex = xSemaphoreCreateMutex();
    
    ESP_LOGD(TAG, "UART Initialized");

    xTaskCreate(uart_receive_task,
                "uart_recv",
                UART_TASK_STACK_SIZE,
                NULL,
                UART_TASK_PRIORITY,
                &uart_task);
    return;
}

int uart_register_consumer(QueueHandle_t q)
{
    if (consumer_count >= UART_MAX_CONSUMERS) {
        return -1;
    }

    xSemaphoreTake(consumer_mutex, portMAX_DELAY);    
    for (size_t i = 0; i < UART_MAX_CONSUMERS; ++i) {
        if (!consumers[i]) {
            consumers[i] = q;
            ++consumer_count;
            xSemaphoreGive(consumer_mutex);
            return i;
        }
    }
    xSemaphoreGive(consumer_mutex);
    return -1;
}

void uart_deregister_consumer(int consumer_id)
{
    if (consumer_id < 0 || consumer_id >= UART_MAX_CONSUMERS) {
        return;
    }

    xSemaphoreTake(consumer_mutex, portMAX_DELAY);
    consumers[consumer_id] = NULL;
    --consumer_count;
    xSemaphoreGive(consumer_mutex);
    return;
}

static void cleanup_consumers(void)
{
    xSemaphoreTake(consumer_mutex, portMAX_DELAY);
    for (size_t i = 0; i < UART_MAX_CONSUMERS; i++) {
        consumers[i] = NULL;
    }
    consumer_count = 0;
    xSemaphoreGive(consumer_mutex);
}

void uart_driver_deinit(void)
{
    if (uart_task != NULL) {
        vTaskDelete(uart_task);
        uart_task = NULL;
    }
    
    uart_driver_delete(UART_NUM);
    cleanup_consumers();

    if (consumer_mutex != NULL) {
        vSemaphoreDelete(consumer_mutex);
        consumer_mutex = NULL;
    }
    
    ESP_LOGD(TAG, "UART Deinitialized");
}