#include <zephyr/kernel.h>
#include "version.h"
#include "../assets/custom_fonts.h"

// pixel_operator_mono advances 8px per glyph, so the canvas fits 8 characters
// per line. Longer strings wrap onto the next line.
void draw_version_status(lv_obj_t *canvas) {
    lv_draw_label_dsc_t label_left_dsc;
    init_label_dsc(&label_left_dsc, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_LEFT);

    lv_canvas_draw_text(canvas, 0, 0, BUFFER_SIZE, &label_left_dsc,
                        CONFIG_NICE_VIEW_FIRMWARE_VERSION);
}
