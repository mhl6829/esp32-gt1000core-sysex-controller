#ifndef _SYSEX_H
#define _SYSEX_H

#include "freertos/FreeRTOS.h"
#include "freertos/queue.h"

#define SYSEX_MAX_MESSAGE_SIZE            256

typedef void(*parser_callback_t)(uint8_t*, int);

typedef struct {
    uint8_t dev_id;
    uint8_t manufacturer_id;
    uint8_t dev_family_code_1;
    uint8_t dev_family_code_2;
    uint8_t dev_family_num_1;
    uint8_t dev_family_num_2;
    uint8_t sw_rev_lv_1;
    uint8_t sw_rev_lv_2;
    uint8_t sw_rev_lv_3;
    uint8_t sw_rev_lv_4;
} sysex_identity_reply;

typedef struct {
    uint8_t data[SYSEX_MAX_MESSAGE_SIZE];
    int length;
    bool in_use;
} sysex_buffer_t;

QueueHandle_t sysex_init();
void sysex_register_device_message_queue(QueueHandle_t queue);
void sysex_start_parsing();
void sysex_free_buffer(sysex_buffer_t *buffer);
bool sysex_device_inquiry(sysex_identity_reply *identity, TickType_t timeout, int retry);
int sysex_send(const uint8_t *message, int length);
void sysex_deinit();

#endif