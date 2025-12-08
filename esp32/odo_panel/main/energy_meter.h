/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#pragma once

#include "lvgl.h"

class EnegyMeter {
public:
    EnegyMeter(lv_obj_t *parent);
    void setRange(int32_t min, int32_t max);
    void setThresholds(int32_t val1, int32_t val2);
    void setMajorTickNth(uint16_t nth);

private:
    lv_obj_t *meter;
    lv_meter_scale_t *scale;
    lv_meter_indicator_t *indic;
    lv_meter_indicator_t *arc_green;
    lv_meter_indicator_t *arc_orange;
    lv_meter_indicator_t *arc_red;
    int32_t val;
    int32_t min_val;
    int32_t max_val;
    int32_t threshold1;
    int32_t threshold2;
    bool increasing;

    static void timer_cb(lv_timer_t *timer);
    void update();
};
