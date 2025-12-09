/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "gear_indicator.h"
#include "vehicle_data.h"
#include <stdio.h>

GearIndicator::GearIndicator(lv_obj_t *parent) : current_gear(1) {
    // Create a container for the bars to keep them centered
    lv_obj_t * cont = lv_obj_create(parent);
    lv_obj_set_size(cont, 65, 30);
    lv_obj_align(cont, LV_ALIGN_TOP_LEFT, 50, 10);
    lv_obj_set_style_bg_opa(cont, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(cont, 0, 0);
    lv_obj_set_style_pad_all(cont, 0, 0);
    lv_obj_clear_flag(cont, LV_OBJ_FLAG_SCROLLABLE);

    for (int i = 0; i < 3; i++) {
        bars[i] = lv_obj_create(cont);
        lv_obj_set_size(bars[i], 15, 20);
        lv_obj_align(bars[i], LV_ALIGN_LEFT_MID, i * 20 + 5, 0);
        lv_obj_set_style_border_width(bars[i], 0, 0);
        lv_obj_set_style_bg_color(bars[i], lv_palette_main(LV_PALETTE_ORANGE), 0);
        lv_obj_clear_flag(bars[i], LV_OBJ_FLAG_SCROLLABLE);
        lv_obj_set_style_shadow_width(bars[i], 0, 0);
        lv_obj_set_style_pad_all(bars[i], 0, 0);

        lv_obj_t * label = lv_label_create(bars[i]);
        lv_label_set_text_fmt(label, "%d", i + 1);
        lv_obj_center(label);
        lv_obj_set_style_text_color(label, lv_color_hex(0x000000), 0);
    }
    
    update();

    // Cycle gears for demo
    lv_timer_create(timer_cb, 2000, this);
}

void GearIndicator::setGear(int gear) {
    if (gear < 0) gear = 0;
    if (gear > 3) gear = 3;
    current_gear = gear;
    update();
}

void GearIndicator::update() {
    for (int i = 0; i < 3; i++) {
        if (current_gear > 0 && i == (current_gear - 1)) {
            lv_obj_set_style_bg_opa(bars[i], LV_OPA_COVER, 0);
        } else {
            lv_obj_set_style_bg_opa(bars[i], LV_OPA_30, 0);
        }
    }
}

void GearIndicator::timer_cb(lv_timer_t *timer) {
    GearIndicator *gi = (GearIndicator *)timer->user_data;
    auto& vehicleData = VehicleData::instance();
    gi->current_gear = vehicleData.getGear();
    gi->update();
}
