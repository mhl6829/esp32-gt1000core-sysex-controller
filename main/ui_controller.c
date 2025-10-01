#include "freertos/FreeRTOS.h"
#include "display.h"
#include "ui_controller.h"
#include "lvgl.h"

static lv_obj_t *loading_ui = NULL;
static lv_obj_t *main_ui = NULL;
static lv_obj_t *current_ui = NULL;

typedef struct
{
    lv_obj_t *preset_name;
} main_ui_component_t;

static main_ui_component_t main_ui_component;

static void create_loading_ui() {
    lv_api_lock();
    lv_obj_t *scr = lv_obj_create(NULL);
    lv_obj_t *label = lv_label_create(scr);
    lv_label_set_long_mode(label, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(label, "Connecting. . .");
    lv_obj_set_width(label, lv_obj_get_width(scr));
    lv_obj_center(label);
    loading_ui = scr;
    lv_api_release();
}

static void create_main_ui() {
    lv_api_lock();
    lv_obj_t *scr = lv_obj_create(NULL);
    main_ui = scr;

    lv_obj_t *preset_name = lv_label_create(scr);
    lv_label_set_long_mode(preset_name, LV_LABEL_LONG_SCROLL_CIRCULAR);
    lv_label_set_text(preset_name, "");
    lv_obj_set_style_text_font(preset_name, &lv_font_montserrat_14, 0);
    lv_obj_set_width(preset_name, lv_obj_get_width(scr));
    lv_obj_center(preset_name);
    main_ui_component.preset_name = preset_name;
    lv_api_release();
}

void set_ui_preset_name(char *name) {
    lv_api_lock();
    lv_label_set_text(main_ui_component.preset_name, name);
    lv_api_release();
}

void init_ui_controller(void) {
    create_loading_ui();
    create_main_ui();
    current_ui = loading_ui;
}

void show_screen(ui_t screen) {
    switch (screen) {
        case UI_LOADING:
            current_ui = loading_ui;
            break;
        case UI_MAIN:
            current_ui = main_ui;
            break;
        default:
            break;
    }
    lv_api_lock();
    lv_scr_load(current_ui);
    lv_api_release();
}