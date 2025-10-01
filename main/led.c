#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "led.h"

static void register_led(uint8_t gpio) {
    gpio_config_t io_conf = {
        .pin_bit_mask = 1ULL << gpio,
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = 0,
        .pull_down_en = 0,
        .intr_type = GPIO_INTR_DISABLE
    };
    gpio_config(&io_conf);
}

void init_led() {
    register_led(LED_1_GPIO);
    register_led(LED_2_GPIO);
    register_led(LED_3_GPIO);
    register_led(LED_4_GPIO);
}

void set_led(uint8_t gpio, bool value) {
    gpio_set_level(gpio, value);
}
