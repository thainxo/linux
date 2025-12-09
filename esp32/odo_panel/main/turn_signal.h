/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#pragma once

#include "lvgl.h"

class TurnSignal {
public:
    TurnSignal(lv_obj_t *parent, int width, int height);
    void setLeft(bool active);
    void setRight(bool active);

private:
    lv_obj_t *left_arrow;
    lv_obj_t *right_arrow;
    bool left_active;
    bool right_active;
    bool blink_state;

    static void timer_cb(lv_timer_t *timer);
    static void draw_event_cb(lv_event_t * e);
    void update();
};
