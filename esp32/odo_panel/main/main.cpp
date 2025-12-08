/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_timer.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_qemu_rgb.h"
#include "driver/gpio.h"
#include "esp_err.h"
#include "esp_log.h"
#include "lvgl.h"
#include "ui_lvgl.h"

static const char *TAG = "example";

/**
 * 32-bit and 16-bit colors are currently supported by QEMU RGB Panel
 */
#if CONFIG_LV_COLOR_DEPTH_32
#define CURRENT_COLOR_DEPTH RGB_QEMU_BPP_32
#elif CONFIG_LV_COLOR_DEPTH_16
#define CURRENT_COLOR_DEPTH RGB_QEMU_BPP_16
#else
#error "QEMU RGB Panel only supports 32-bit and 16-bit colors, please enable LV_COLOR_DEPTH_32 or LV_COLOR_DEPTH_16"
#endif

// The pixel number in horizontal and vertical
#define EXAMPLE_LCD_H_RES              800
#define EXAMPLE_LCD_V_RES              480

extern "C" void app_main(void)
{
    UIApp *ui_app = new UIApp(EXAMPLE_LCD_H_RES, EXAMPLE_LCD_V_RES, CURRENT_COLOR_DEPTH);
    ui_app->init();
    ui_app->run();
}

