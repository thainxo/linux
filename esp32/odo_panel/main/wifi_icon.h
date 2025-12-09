/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#pragma once

#include "lvgl.h"

class WifiIcon {
public:
    WifiIcon(lv_obj_t *parent);
    void setStrength(int level); // 0: off, 1: low, 2: high

private:
    lv_obj_t *icon;
    int strength;
    
    static void draw_event_cb(lv_event_t * e);
    static void timer_cb(lv_timer_t *timer);
};
