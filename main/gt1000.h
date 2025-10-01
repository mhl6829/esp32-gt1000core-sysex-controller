#ifndef _GT1000_H
#define _GT1000_H

#include "freertos/FreeRTOS.h"
#include "gt1000_param.h"

typedef enum
{
    UNHANDLED,
    PRESET_CHANGE,
    PRESET_NAME_UPDATE,
    PARAMETER_UPDATE,
} gt1000_event_t;

typedef void (*gt1000_callback_t)(gt1000_event_t);

QueueHandle_t gt1000_init();
void gt1000_set_device_id(uint8_t id);
gt1000_t *gt1000_get_device(void);
gt1000_param_addr_t gt1000_update_state_from_sysex(uint8_t *message, int length);
void gt1000_update_parameter(gt1000_param_addr_t parameter);
void gt1000_update_block(gt1000_param_addr_t block);
void gt1000_set_parameter(gt1000_param_addr_t parameter, uint32_t value);
void gt1000_update_patch_name(void);
void gt1000_register_callback(gt1000_callback_t cbk);
void gt1000_enable_notifications(void);
void gt1000_disable_notifications(void);

#endif