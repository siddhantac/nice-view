#include <zephyr/kernel.h>
#include "animation.h"

LV_IMG_DECLARE(arasaka);
LV_IMG_DECLARE(arasaka_glitch_1);
LV_IMG_DECLARE(arasaka_glitch_2);
LV_IMG_DECLARE(arasaka_glitch_3);
LV_IMG_DECLARE(arasaka_glitch_4);
LV_IMG_DECLARE(arasaka_glitch_h1);
LV_IMG_DECLARE(arasaka_glitch_h2);

LV_IMG_DECLARE(corne);

#if IS_ENABLED(CONFIG_NICE_VIEW_GEM_ANIMATION)

// Two burst shapes, picked at random so the corruption is not always the same
// severity. The minor one is a quick horizontal tear; the major one ramps
// through the escalating frames (glitch_1 disturbs ~370px around the middle
// rows, glitch_4 ~1100px spread nearly the full height) before settling back
// through the tighter bands.
static const lv_img_dsc_t *const burst_minor[] = {
    &arasaka_glitch_h1,
    &arasaka_glitch_h2,
    &arasaka_glitch_h1,
};

static const lv_img_dsc_t *const burst_major[] = {
    &arasaka_glitch_1, &arasaka_glitch_2,  &arasaka_glitch_3, &arasaka_glitch_4,
    &arasaka_glitch_h2, &arasaka_glitch_h1, &arasaka_glitch_2,
};

// The peripheral builds exactly one status screen, so a single set of state is
// enough. Everything here runs from the LVGL timer callback, i.e. on the
// display thread, so no locking is needed.
static lv_obj_t *art_img;
static const lv_img_dsc_t *const *burst;
static uint8_t burst_len;
static uint8_t burst_pos;

static uint32_t rng_state = 1;

// xorshift32. Picking animation timings needs nothing better, and rolling our
// own avoids depending on whether this build's libc ships rand() at all
// (minimal libc gates it behind CONFIG_MINIMAL_LIBC_RAND).
static uint32_t next_rand(void) {
    rng_state ^= rng_state << 13;
    rng_state ^= rng_state >> 17;
    rng_state ^= rng_state << 5;
    return rng_state;
}

static uint32_t rand_range(uint32_t lo, uint32_t hi) {
    if (hi <= lo) {
        return lo;
    }
    return lo + (next_rand() % (hi - lo + 1));
}

static void arm_next_burst(lv_timer_t *timer) {
    burst_pos = 0;

    // Roughly one in three bursts is the full cascade.
    if (rand_range(0, 2) == 0) {
        burst = burst_major;
        burst_len = ARRAY_SIZE(burst_major);
    } else {
        burst = burst_minor;
        burst_len = ARRAY_SIZE(burst_minor);
    }

    lv_timer_set_period(timer, rand_range(CONFIG_NICE_VIEW_GLITCH_IDLE_MIN_MS,
                                          CONFIG_NICE_VIEW_GLITCH_IDLE_MAX_MS));
}

static void glitch_tick(lv_timer_t *timer) {
    if (burst_pos < burst_len) {
        lv_img_set_src(art_img, burst[burst_pos++]);
        // Jitter each frame so the tear does not read as a metronome either.
        lv_timer_set_period(timer, rand_range(CONFIG_NICE_VIEW_GLITCH_FRAME_MS / 2,
                                              CONFIG_NICE_VIEW_GLITCH_FRAME_MS * 3 / 2));
        return;
    }

    // Burst finished (or this is the very first tick): back to the clean logo
    // and wait a random while before corrupting it again.
    lv_img_set_src(art_img, &arasaka);
    arm_next_burst(timer);
}

#endif /* IS_ENABLED(CONFIG_NICE_VIEW_GEM_ANIMATION) */

void draw_animation(lv_obj_t *canvas) {
#if IS_ENABLED(CONFIG_NICE_VIEW_GEM_ANIMATION)
    art_img = lv_img_create(canvas);
    lv_img_set_src(art_img, &arasaka);

    // k_cycle_get_32() carries the low-order jitter that uptime alone does not,
    // since display init lands at a fairly repeatable point in the boot. The
    // state must never be zero or xorshift latches there permanently.
    rng_state = (k_uptime_get_32() ^ k_cycle_get_32()) | 1u;

    lv_timer_t *timer = lv_timer_create(glitch_tick, CONFIG_NICE_VIEW_GLITCH_IDLE_MIN_MS, NULL);
    lv_timer_set_repeat_count(timer, -1);
    // Choose the first burst and its lead-in now, so the opening interval is
    // random like every other one rather than a fixed IDLE_MIN.
    arm_next_burst(timer);

    lv_obj_t *art = art_img;
#else
    lv_obj_t *art = lv_img_create(canvas);
    lv_img_set_src(art, &corne);
#endif

    lv_obj_align(art, LV_ALIGN_TOP_LEFT, 0, 0);
}
