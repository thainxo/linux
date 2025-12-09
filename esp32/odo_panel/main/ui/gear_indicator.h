/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#pragma once

#include "lvgl.h"

class GearIndicator {
public:
    GearIndicator(lv_obj_t *parent);
    void setGear(int gear);

private:
    lv_obj_t *bars[3];
    int current_gear;
    
    static void timer_cb(lv_timer_t *timer);
    void update();
};
