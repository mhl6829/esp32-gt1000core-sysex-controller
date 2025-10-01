#ifndef _LED_H
#define _LED_H

#include "freertos/FreeRTOS.h"

#define LED_1_GPIO      0
#define LED_2_GPIO      1
#define LED_3_GPIO      2
#define LED_4_GPIO      3

void init_led();
void set_led(uint8_t gpio, bool value);

#endif