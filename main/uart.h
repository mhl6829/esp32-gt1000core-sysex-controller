#ifndef _UART_H
#define _UART_H

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

void uart_driver_init(void);
void uart_driver_deinit(void);
int uart_register_consumer(QueueHandle_t q);
void uart_deregister_consumer(int consumer_id);
int uart_send(const uint8_t *data, int len);

#endif
