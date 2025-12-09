/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#pragma once

#include "lvgl.h"

class BluetoothIcon {
public:
    BluetoothIcon(lv_obj_t *parent);
    void setConnected(bool connected);

private:
    lv_obj_t *icon;
    bool connected;
    
    static void draw_event_cb(lv_event_t * e);
    static void timer_cb(lv_timer_t *timer);
};
