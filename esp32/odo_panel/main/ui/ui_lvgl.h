/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#pragma once

#include "esp_lcd_types.h"
#include "lvgl.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"

class UIApp {
public:
    UIApp(int h_res, int v_res, int bpp);
    ~UIApp();

    lv_disp_t *init();
    void run();

private:
    bool lock(int timeout_ms);
    void unlock();

    static void flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map);
    static void increase_tick(void *arg);
    static void task(void *arg);
    size_t get_buffers(void **buf1, void **buf2);

    esp_lcd_panel_handle_t panel_handle;
    lv_disp_t *disp;
    int h_res;
    int v_res;
    int bpp;
    SemaphoreHandle_t lvgl_mux;
    lv_disp_draw_buf_t disp_buf;
    lv_disp_drv_t disp_drv;
};
