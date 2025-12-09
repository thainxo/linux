/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "energy_meter.h"

EnergyMeter::EnergyMeter(lv_obj_t *parent, int width, int height, int x_ofs, int y_ofs) : val(0), min_val(0), max_val(100), threshold1(50), threshold2(80), increasing(true) {
    meter = lv_meter_create(parent);
    lv_obj_align(meter, LV_ALIGN_CENTER, x_ofs, y_ofs);
    lv_obj_set_size(meter, width, height);

    /*Add a scale first*/
    scale = lv_meter_add_scale(meter);
    lv_meter_set_scale_ticks(meter, scale, 41, 2, 10, lv_palette_main(LV_PALETTE_GREY));
    lv_meter_set_scale_major_ticks(meter, scale, 8, 4, 15, lv_color_black(), 10);

    /*Add a green arc to the start*/
    arc_green = lv_meter_add_arc(meter, scale, 3, lv_palette_main(LV_PALETTE_GREEN), 0);
    lv_meter_set_indicator_start_value(meter, arc_green, min_val);
    lv_meter_set_indicator_end_value(meter, arc_green, threshold1);

    /*Add a orange arc to the middle*/
    arc_orange = lv_meter_add_arc(meter, scale, 3, lv_palette_main(LV_PALETTE_ORANGE), 0);
    lv_meter_set_indicator_start_value(meter, arc_orange, threshold1);
    lv_meter_set_indicator_end_value(meter, arc_orange, threshold2);

    /*Add a red arc to the end*/
    arc_red = lv_meter_add_arc(meter, scale, 3, lv_palette_main(LV_PALETTE_RED), 0);
    lv_meter_set_indicator_start_value(meter, arc_red, threshold2);
    lv_meter_set_indicator_end_value(meter, arc_red, max_val);

    /*Add a needle line indicator*/
    indic = lv_meter_add_needle_line(meter, scale, 4, lv_palette_main(LV_PALETTE_GREY), -10);

    /*Add a label to show the unit*/
    lv_obj_t *label = lv_label_create(meter);
    lv_label_set_text(label, "kW");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 50);

    lv_timer_create(timer_cb, 100, this);
}

void EnergyMeter::timer_cb(lv_timer_t *timer) {
    EnergyMeter *meter = (EnergyMeter *)timer->user_data;
    meter->update();
}

void EnergyMeter::setRange(int32_t min, int32_t max) {
    min_val = min;
    max_val = max;
    lv_meter_set_scale_range(meter, scale, min, max, 270, 135);
    
    // Update outer boundaries of arcs
    lv_meter_set_indicator_start_value(meter, arc_green, min_val);
    lv_meter_set_indicator_end_value(meter, arc_red, max_val);
}

void EnergyMeter::setThresholds(int32_t val1, int32_t val2) {
    threshold1 = val1;
    threshold2 = val2;
    
    lv_meter_set_indicator_end_value(meter, arc_green, threshold1);
    
    lv_meter_set_indicator_start_value(meter, arc_orange, threshold1);
    lv_meter_set_indicator_end_value(meter, arc_orange, threshold2);
    
    lv_meter_set_indicator_start_value(meter, arc_red, threshold2);
}

void EnergyMeter::setMajorTickNth(uint16_t nth) {
    lv_meter_set_scale_major_ticks(meter, scale, nth, 4, 15, lv_color_black(), 10);
}

void EnergyMeter::update() {
    if (increasing) {
        val++;
        if (val >= max_val) {
            increasing = false;
        }
    } else {
        val--;
        if (val <= min_val) {
            increasing = true;
        }
    }
    lv_meter_set_indicator_value(meter, indic, val);
}
