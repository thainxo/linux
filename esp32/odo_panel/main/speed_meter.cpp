/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "speed_meter.h"

SpeedMeter::SpeedMeter(lv_obj_t *parent) : val(0), min_val(0), max_val(100), threshold1(20), threshold2(80), increasing(true) {
    meter = lv_meter_create(parent);
    lv_obj_align(meter, LV_ALIGN_CENTER, 110, 0);
    lv_obj_set_size(meter, 200, 200);

    /*Add a scale first*/
    scale = lv_meter_add_scale(meter);
    lv_meter_set_scale_ticks(meter, scale, 41, 2, 10, lv_palette_main(LV_PALETTE_GREY));
    lv_meter_set_scale_major_ticks(meter, scale, 10, 4, 15, lv_color_black(), 10);

    /*Add a green arc to the start*/
    arc_green = lv_meter_add_arc(meter, scale, 3, lv_palette_main(LV_PALETTE_GREEN), 0);
    lv_meter_set_indicator_start_value(meter, arc_green, min_val);
    lv_meter_set_indicator_end_value(meter, arc_green, threshold1);

    /*Make the tick lines green at the start of the scale*/
    lines_green = lv_meter_add_scale_lines(meter, scale, lv_palette_main(LV_PALETTE_GREEN), lv_palette_main(LV_PALETTE_GREEN), false, 0);
    lv_meter_set_indicator_start_value(meter, lines_green, min_val);
    lv_meter_set_indicator_end_value(meter, lines_green, threshold1);

    /*Add a red arc to the end*/
    arc_red = lv_meter_add_arc(meter, scale, 3, lv_palette_main(LV_PALETTE_RED), 0);
    lv_meter_set_indicator_start_value(meter, arc_red, threshold2);
    lv_meter_set_indicator_end_value(meter, arc_red, max_val);

    /*Make the tick lines red at the end of the scale*/
    lines_red = lv_meter_add_scale_lines(meter, scale, lv_palette_main(LV_PALETTE_RED), lv_palette_main(LV_PALETTE_RED), false, 0);
    lv_meter_set_indicator_start_value(meter, lines_red, threshold2);
    lv_meter_set_indicator_end_value(meter, lines_red, max_val);

    /*Add a needle line indicator*/
    this->indic = lv_meter_add_needle_line(meter, scale, 4, lv_palette_main(LV_PALETTE_GREY), -10);

    /*Add a label to show the unit*/
    lv_obj_t *label = lv_label_create(meter);
    lv_label_set_text(label, "km/h");
    lv_obj_align(label, LV_ALIGN_CENTER, 0, 50);

    lv_timer_create(timer_cb, 100, this);
}

void SpeedMeter::timer_cb(lv_timer_t *timer) {
    SpeedMeter *speed = (SpeedMeter *)timer->user_data;
    speed->update();
}

void SpeedMeter::setRange(int32_t min, int32_t max) {
    min_val = min;
    max_val = max;
    lv_meter_set_scale_range(meter, scale, min, max, 270, 135);
    
    // Update outer boundaries of arcs
    lv_meter_set_indicator_start_value(meter, arc_green, min_val);
    lv_meter_set_indicator_start_value(meter, lines_green, min_val);
    
    lv_meter_set_indicator_end_value(meter, arc_red, max_val);
    lv_meter_set_indicator_end_value(meter, lines_red, max_val);
}

void SpeedMeter::setThresholds(int32_t val1, int32_t val2) {
    threshold1 = val1;
    threshold2 = val2;
    
    lv_meter_set_indicator_end_value(meter, arc_green, threshold1);
    lv_meter_set_indicator_end_value(meter, lines_green, threshold1);
    
    lv_meter_set_indicator_start_value(meter, arc_red, threshold2);
    lv_meter_set_indicator_start_value(meter, lines_red, threshold2);
}

void SpeedMeter::setMajorTickNth(uint16_t nth) {
    lv_meter_set_scale_major_ticks(meter, scale, nth, 4, 15, lv_color_black(), 10);
}

void SpeedMeter::update() {
    if (increasing) {
        val += 1;
        if (val >= max_val) {
            increasing = false;
        }
    } else {
        val -= 1;
        if (val <= min_val) {
            increasing = true;
        }
    }

    lv_meter_set_indicator_value(meter, indic, val);
}
