#ifndef _UI_CONTROLLER_H
#define _UI_CONTROLLER_H

typedef enum
{
    UI_LOADING,
    UI_MAIN,
} ui_t;

void init_ui_controller(void);
void show_screen(ui_t screen);
void set_ui_preset_name(char *name);

#endif