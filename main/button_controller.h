#ifndef _BUTTON_CONTROLLER_H
#define _BUTTON_CONTROLLER_H

typedef enum
{
    BUTTON_1_PRESSED,
    BUTTON_2_PRESSED,
    BUTTON_3_PRESSED,
    BUTTON_4_PRESSED,
    BUTTON_UNHANDLED,
} button_controller_event_t;

void init_button_controller(void);

void button_register_callback(void (*cb)(button_controller_event_t t));

#endif