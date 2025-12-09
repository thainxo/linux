/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "bluetooth_icon.h"
#include "vehicle_data.h"

BluetoothIcon::BluetoothIcon(lv_obj_t *parent) : connected(false) {
    icon = lv_obj_create(parent);
    lv_obj_set_size(icon, 15, 20);
    lv_obj_align(icon, LV_ALIGN_TOP_RIGHT, -60, 15);
    lv_obj_set_style_bg_opa(icon, LV_OPA_TRANSP, 0);
    lv_obj_set_style_border_width(icon, 0, 0);
    lv_obj_clear_flag(icon, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_add_event_cb(icon, draw_event_cb, LV_EVENT_DRAW_MAIN, this);

    // Toggle connection state for demo
    lv_timer_create(timer_cb, 1000, this);
}

void BluetoothIcon::setConnected(bool connected) {
    this->connected = connected;
    lv_obj_invalidate(icon);
}

void BluetoothIcon::draw_event_cb(lv_event_t * e) {
    BluetoothIcon * bt = (BluetoothIcon *)lv_event_get_user_data(e);
    lv_obj_t * obj = lv_event_get_target(e);
    lv_draw_ctx_t * draw_ctx = lv_event_get_draw_ctx(e);

    lv_area_t coords;
    lv_obj_get_coords(obj, &coords);
    lv_coord_t x = coords.x1;
    lv_coord_t y = coords.y1;
    lv_coord_t w = lv_obj_get_width(obj);
    lv_coord_t h = lv_obj_get_height(obj);

    lv_draw_line_dsc_t line_dsc;
    lv_draw_line_dsc_init(&line_dsc);
    line_dsc.width = 2;
    line_dsc.color = bt->connected ? lv_palette_main(LV_PALETTE_BLUE) : lv_palette_main(LV_PALETTE_GREY);
    line_dsc.round_start = 1;
    line_dsc.round_end = 1;

    lv_point_t points[] = {
        {(lv_coord_t)(x + 2), (lv_coord_t)(y + h/4)},
        {(lv_coord_t)(x + w - 2), (lv_coord_t)(y + 3*h/4)},
        {(lv_coord_t)(x + w/2), (lv_coord_t)(y + h - 2)},
        {(lv_coord_t)(x + w/2), (lv_coord_t)(y + 2)},
        {(lv_coord_t)(x + w - 2), (lv_coord_t)(y + h/4)},
        {(lv_coord_t)(x + 2), (lv_coord_t)(y + 3*h/4)}
    };
    
    lv_draw_line(draw_ctx, &line_dsc, &points[0], &points[1]);
    lv_draw_line(draw_ctx, &line_dsc, &points[1], &points[2]);
    lv_draw_line(draw_ctx, &line_dsc, &points[2], &points[3]);
    lv_draw_line(draw_ctx, &line_dsc, &points[3], &points[4]);
    lv_draw_line(draw_ctx, &line_dsc, &points[4], &points[5]);
}

void BluetoothIcon::timer_cb(lv_timer_t *timer) {
    BluetoothIcon *bt = (BluetoothIcon *)timer->user_data;
    auto& vehicleData = VehicleData::instance();
    bt->setConnected(vehicleData.getBluetoothConnected());
}
