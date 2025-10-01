/*
 * SPDX-FileCopyrightText: 2025 mhl6829
 * SPDX-License-Identifier: MIT
 * File: [main.c] - Project entry point and system initialization
 */


#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "esp_log.h"

#include "sysex.h"
#include "uart.h"
#include "gt1000.h"
#include "gt1000_param.h"
#include "display.h"
#include "ui_controller.h"
#include "button_controller.h"
#include "led.h"

#define TAG "MAIN"

typedef struct {
    gt1000_param_addr_t btn1;
    gt1000_param_addr_t btn2;
    gt1000_param_addr_t btn3;
} button_mapping_t;

static gt1000_t *device;
static button_mapping_t mapping;

static void update_current() {
    gt1000_update_patch_name();
    gt1000_update_parameter(mapping.btn1);
    gt1000_update_parameter(mapping.btn2);
    gt1000_update_parameter(mapping.btn3);
}

static void toggle_param(gt1000_param_addr_t parameter) {
    gt1000_param_t param;
    gt1000_get_parameter_info(&param, parameter);
    gt1000_set_parameter(parameter, !(bool)param.value);
}

static void gt1000_event_callback(gt1000_event_t event)
{
    switch (event) {
        case PRESET_CHANGE:
            update_current();
            break;
        case PRESET_NAME_UPDATE:
            set_ui_preset_name(device->patch_name);
            break;
        case PARAMETER_UPDATE:
            set_led(LED_1_GPIO, *(bool *)mapping.btn1);
            set_led(LED_2_GPIO, *(bool *)mapping.btn2);
            set_led(LED_3_GPIO, *(bool *)mapping.btn3);
            break;
        default:
            break;
    }
}


static void button_event_callback(button_controller_event_t event) {
    switch (event) {
        case BUTTON_1_PRESSED:
            toggle_param(mapping.btn1);
            break;
        case BUTTON_2_PRESSED:
            toggle_param(mapping.btn2);
            break;
        case BUTTON_3_PRESSED:
            toggle_param(mapping.btn3);
            break;
        case BUTTON_4_PRESSED:
            break;
        default:
            break;
    }
}


void app_main(void)
{

    QueueHandle_t gt1000_msg_queue = gt1000_init();
    QueueHandle_t parser_queue = sysex_init();
    uart_driver_init();    
    uart_register_consumer(parser_queue);
    sysex_register_device_message_queue(gt1000_msg_queue);
    
    start_display();
    init_ui_controller();
    init_button_controller();
    init_led();

    show_screen(UI_LOADING);

    // device inquery
    sysex_identity_reply ir;
    if (!sysex_device_inquiry(&ir, pdMS_TO_TICKS(5000), 5)) {
        return;
    }
    
    // init device id
    gt1000_set_device_id(ir.dev_id);
    device = gt1000_get_device();
    mapping = (button_mapping_t){
        .btn1 = &device->effect.comp.sw,
        .btn2 = &device->effect.dist1.sw,
        .btn3 = &device->effect.mstdelay.sw,
    };
    
    gt1000_register_callback(gt1000_event_callback);
    button_register_callback(button_event_callback);

    sysex_start_parsing();
    
    gt1000_enable_notifications();

    show_screen(UI_MAIN);

    update_current_mapping();
}
