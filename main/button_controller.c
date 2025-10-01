#include "freertos/FreeRTOS.h"
#include "button_gpio.h"
#include "iot_button.h"
#include "esp_log.h"
#include "button_controller.h"

#define TAG "BUTTON"

#define BUTTON_1_GPIO       5
#define BUTTON_2_GPIO       6
#define BUTTON_3_GPIO       7
#define BUTTON_4_GPIO       8

static void (*button_event_callback)(button_controller_event_t);

static void cbk(void *handle, void *parameter) {
    int gpio = (int)parameter;
    button_controller_event_t event = BUTTON_UNHANDLED;
    switch (gpio) {
        case BUTTON_1_GPIO:
            event = BUTTON_1_PRESSED;
            break;
        case BUTTON_2_GPIO:
            event = BUTTON_2_PRESSED;
            break;
        case BUTTON_3_GPIO:
            event = BUTTON_3_PRESSED;
            break;
        case BUTTON_4_GPIO:
            event = BUTTON_4_PRESSED;
            break;
        default:
            break;
    }
    button_event_callback(event);
}

void create_button(int gpio, void(*callback)(void *, void *)) {
    const button_config_t btn_cfg = {0};
    const button_gpio_config_t btn_gpio_cfg = {
        .gpio_num = gpio,
        .active_level = 0,
    };
    button_handle_t gpio_btn = NULL;
    iot_button_new_gpio_device(&btn_cfg, &btn_gpio_cfg, &gpio_btn);

    iot_button_register_cb(gpio_btn, BUTTON_PRESS_DOWN, NULL, cbk, (void *)gpio);
}

void init_button_controller() {
    create_button(BUTTON_1_GPIO, cbk);
    create_button(BUTTON_2_GPIO, cbk);
    create_button(BUTTON_3_GPIO, cbk);
    create_button(BUTTON_4_GPIO, cbk);
}

void button_register_callback(void(*cb)(button_controller_event_t t)) {
    button_event_callback = cb;
}