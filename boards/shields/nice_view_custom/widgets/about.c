#include <zephyr/kernel.h>
#include "about.h"
#include "../assets/custom_fonts.h"

// Positions are in whole-screen coordinates, offset into this canvas the same
// way profile.c and layer.c do it. The profile dots sit at 129, so the version
// lands one 13px line above them; the canvas reaches 128 and no further.
void draw_about_status(lv_obj_t *canvas) {
    lv_draw_label_dsc_t label_left_dsc;
    init_label_dsc(&label_left_dsc, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_LEFT);
    lv_canvas_draw_text(canvas, 0, 61 + BUFFER_OFFSET_MIDDLE, BUFFER_SIZE, &label_left_dsc,
                        CONFIG_NICE_VIEW_OWNER_NAME);

    lv_draw_label_dsc_t label_center_dsc;
    init_label_dsc(&label_center_dsc, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_CENTER);
    lv_canvas_draw_text(canvas, 0, 112 + BUFFER_OFFSET_MIDDLE, BUFFER_SIZE, &label_center_dsc,
                        CONFIG_NICE_VIEW_FIRMWARE_VERSION);
}
