#include <zephyr/kernel.h>
#include "about.h"
#include "../assets/custom_fonts.h"

// Rows use the same 18px pitch as the top canvas. pixel_operator_mono advances
// 8px per glyph, so each row fits 8 characters; longer strings wrap.
void draw_about_status(lv_obj_t *canvas) {
    lv_draw_label_dsc_t label_left_dsc;
    init_label_dsc(&label_left_dsc, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_LEFT);

    lv_canvas_draw_text(canvas, 0, 0, BUFFER_SIZE, &label_left_dsc, CONFIG_NICE_VIEW_OWNER_NAME);
    lv_canvas_draw_text(canvas, 0, 18, BUFFER_SIZE, &label_left_dsc,
                        CONFIG_NICE_VIEW_FIRMWARE_VERSION);
}
