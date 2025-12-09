/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "turn_signal.h"

TurnSignal::TurnSignal(lv_obj_t *parent, int width, int height) : left_active(false), right_active(false), blink_state(false) {
    // Left Arrow
    left_arrow = lv_obj_create(parent);
    lv_obj_set_size(left_arrow, 60, 40);
    lv_obj_align(left_arrow, LV_ALIGN_TOP_LEFT, 10, 5);
    lv_obj_set_style_bg_opa(left_arrow, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(left_arrow, 0, 0);
    lv_obj_add_event_cb(left_arrow, draw_event_cb, LV_EVENT_DRAW_MAIN, this);
    lv_obj_clear_flag(left_arrow, LV_OBJ_FLAG_SCROLLABLE);

    // Right Arrow
    right_arrow = lv_obj_create(parent);
    lv_obj_set_size(right_arrow, 60, 40);
    lv_obj_align(right_arrow, LV_ALIGN_TOP_RIGHT, -10, 5);
    lv_obj_set_style_bg_opa(right_arrow, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(right_arrow, 0, 0);
    lv_obj_add_event_cb(right_arrow, draw_event_cb, LV_EVENT_DRAW_MAIN, this);
    lv_obj_clear_flag(right_arrow, LV_OBJ_FLAG_SCROLLABLE);

    lv_timer_create(timer_cb, 500, this);
}

void TurnSignal::draw_event_cb(lv_event_t * e) {
    TurnSignal * ts = (TurnSignal *)lv_event_get_user_data(e);
    lv_obj_t * obj = lv_event_get_target(e);
    lv_draw_ctx_t * draw_ctx = lv_event_get_draw_ctx(e);

    lv_color_t color = lv_palette_main(LV_PALETTE_GREY);
    if (obj == ts->left_arrow) {
        if (ts->left_active && ts->blink_state) color = lv_palette_main(LV_PALETTE_GREEN);
    } else if (obj == ts->right_arrow) {
        if (ts->right_active && ts->blink_state) color = lv_palette_main(LV_PALETTE_GREEN);
    }

    lv_draw_rect_dsc_t rect_dsc;
    lv_draw_rect_dsc_init(&rect_dsc);
    rect_dsc.bg_color = color;

    lv_area_t coords;
    lv_obj_get_coords(obj, &coords);
    lv_coord_t x = coords.x1;
    lv_coord_t y = coords.y1;

    if (obj == ts->left_arrow) {
        // Tail
        lv_area_t tail_area = { (lv_coord_t)(x + 20), (lv_coord_t)(y + 15), (lv_coord_t)(x + 35), (lv_coord_t)(y + 25) };
        lv_draw_rect(draw_ctx, &rect_dsc, &tail_area);
        
        // Triangle
        lv_point_t points[3] = { {(lv_coord_t)x, (lv_coord_t)(y + 20)}, {(lv_coord_t)(x + 20), (lv_coord_t)y}, {(lv_coord_t)(x + 20), (lv_coord_t)(y + 40)} };
        lv_draw_polygon(draw_ctx, &rect_dsc, points, 3);
    } else {
        // Tail
        lv_area_t tail_area = { (lv_coord_t)(x + 15), (lv_coord_t)(y + 15), (lv_coord_t)(x + 30), (lv_coord_t)(y + 25) };
        lv_draw_rect(draw_ctx, &rect_dsc, &tail_area);

        // Triangle
        lv_point_t points[3] = { {(lv_coord_t)(x + 50), (lv_coord_t)(y + 20)}, {(lv_coord_t)(x + 30), (lv_coord_t)y}, {(lv_coord_t)(x + 30), (lv_coord_t)(y + 40)} };
        lv_draw_polygon(draw_ctx, &rect_dsc, points, 3);
    }
}

void TurnSignal::timer_cb(lv_timer_t *timer) {
    TurnSignal *ts = (TurnSignal *)timer->user_data;
    ts->update();
}

void TurnSignal::update() {
    blink_state = !blink_state;
    lv_obj_invalidate(left_arrow);
    lv_obj_invalidate(right_arrow);
}

void TurnSignal::setLeft(bool active) {
    left_active = active;
    lv_obj_invalidate(left_arrow);
}

void TurnSignal::setRight(bool active) {
    right_active = active;
    lv_obj_invalidate(right_arrow);
}
