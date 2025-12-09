/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "wifi_icon.h"

WifiIcon::WifiIcon(lv_obj_t *parent) : strength(0) {
    icon = lv_obj_create(parent);
    lv_obj_set_size(icon, 20, 20);
    lv_obj_align(icon, LV_ALIGN_TOP_RIGHT, -80, 15);
    lv_obj_set_style_bg_opa(icon, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(icon, 0, 0);
    lv_obj_clear_flag(icon, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(icon, draw_event_cb, LV_EVENT_DRAW_MAIN, this);

    // Toggle connection state for demo
    lv_timer_create(timer_cb, 1500, this);
}

void WifiIcon::setStrength(int level) {
    this->strength = level;
    lv_obj_invalidate(icon);
}

void WifiIcon::draw_event_cb(lv_event_t * e) {
    WifiIcon * wifi = (WifiIcon *)lv_event_get_user_data(e);
    lv_obj_t * obj = lv_event_get_target(e);
    lv_draw_ctx_t * draw_ctx = lv_event_get_draw_ctx(e);

    lv_area_t coords;
    lv_obj_get_coords(obj, &coords);
    lv_coord_t x = coords.x1;
    lv_coord_t y = coords.y1;
    lv_coord_t w = lv_obj_get_width(obj);
    lv_coord_t h = lv_obj_get_height(obj);

    lv_color_t color_on = lv_palette_main(LV_PALETTE_BLUE);
    lv_color_t color_off = lv_palette_main(LV_PALETTE_GREY);

    lv_draw_arc_dsc_t arc_dsc;
    lv_draw_arc_dsc_init(&arc_dsc);
    arc_dsc.width = 2;
    arc_dsc.rounded = 1;

    // Center point at bottom middle
    lv_coord_t cx = x + w / 2;
    lv_coord_t cy = y + h - 4;
    lv_point_t center = {cx, cy};

    // Draw Outer Arc (Level 3)
    arc_dsc.color = (wifi->strength >= 3) ? color_on : color_off;
    lv_draw_arc(draw_ctx, &arc_dsc, &center, 12, 225, 315);

    // Draw Inner Arc (Level 2)
    arc_dsc.color = (wifi->strength >= 2) ? color_on : color_off;
    lv_draw_arc(draw_ctx, &arc_dsc, &center, 7, 225, 315);
    
    // Draw dot (Level 1)
    lv_draw_rect_dsc_t rect_dsc;
    lv_draw_rect_dsc_init(&rect_dsc);
    rect_dsc.bg_color = (wifi->strength >= 1) ? color_on : color_off;
    rect_dsc.radius = LV_RADIUS_CIRCLE;
    lv_area_t dot_area = { (lv_coord_t)(cx - 2), (lv_coord_t)(cy - 2), (lv_coord_t)(cx + 2), (lv_coord_t)(cy + 2) };
    lv_draw_rect(draw_ctx, &rect_dsc, &dot_area);
}

void WifiIcon::timer_cb(lv_timer_t *timer) {
    WifiIcon *wifi = (WifiIcon *)timer->user_data;
    int next_strength = (wifi->strength + 1) % 4;
    wifi->setStrength(next_strength);
}
