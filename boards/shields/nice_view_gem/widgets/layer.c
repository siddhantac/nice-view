#include <zephyr/kernel.h>
#include "layer.h"
#include "../assets/custom_fonts.h"

void draw_layer_status(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_CENTER);

    // char text[10] = "POTKA";
    char text[10] = {};

    if (state->layer_label == NULL) {
        // sprintf(text, "Lyr %i", state->layer_index);
        strncpy(text, "NULL", 4);
    } else {
        // strncpy(text, state->layer_label, 5);
        sprintf(text, "L %i %s", state->layer_index, state->layer_label);
        // strncpy(text, "POTKA", 5);
        // to_uppercase(text);
    }

    lv_canvas_draw_text(canvas, 0, 146 + BUFFER_OFFSET_BOTTOM, 68, &label_dsc, text);
}
