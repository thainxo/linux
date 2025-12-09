/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "ui_lvgl.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
#include "esp_timer.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_qemu_rgb.h"
#include "esp_log.h"
#include "sdkconfig.h"
#include "energy_meter.h"
#include "speed_meter.h"
#include "turn_signal.h"
#include "gear_indicator.h"
#include "bluetooth_icon.h"
#include "wifi_icon.h"
#include "vehicle_data.h"

static const char *TAG = "ui_lvgl";

#define EXAMPLE_LVGL_TICK_PERIOD_MS    2
#define EXAMPLE_LVGL_TASK_MAX_DELAY_MS 500
#define EXAMPLE_LVGL_TASK_MIN_DELAY_MS 1
#define EXAMPLE_LVGL_TASK_STACK_SIZE   (4 * 1024)
#define EXAMPLE_LVGL_TASK_PRIORITY     2

UIApp::UIApp(int h_res, int v_res, int bpp)
    : panel_handle(NULL), h_res(h_res), v_res(v_res), bpp(bpp), lvgl_mux(NULL)
{
}

UIApp::~UIApp()
{
    if (lvgl_mux) {
        vSemaphoreDelete(lvgl_mux);
    }
}

void UIApp::flush_cb(lv_disp_drv_t *drv, const lv_area_t *area, lv_color_t *color_map)
{
    UIApp *app = (UIApp *)drv->user_data;
    int offsetx1 = area->x1;
    int offsetx2 = area->x2;
    int offsety1 = area->y1;
    int offsety2 = area->y2;
    // pass the draw buffer to the driver
    esp_lcd_panel_draw_bitmap(app->panel_handle, offsetx1, offsety1, offsetx2 + 1, offsety2 + 1, color_map);
    lv_disp_flush_ready(drv);
}

void UIApp::increase_tick(void *arg)
{
    /* Tell LVGL how many milliseconds has elapsed */
    lv_tick_inc(EXAMPLE_LVGL_TICK_PERIOD_MS);
}

bool UIApp::lock(int timeout_ms)
{
    // Convert timeout in milliseconds to FreeRTOS ticks
    // If `timeout_ms` is set to -1, the program will block until the condition is met
    const TickType_t timeout_ticks = (timeout_ms == -1) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
    return xSemaphoreTakeRecursive(lvgl_mux, timeout_ticks) == pdTRUE;
}

void UIApp::unlock()
{
    xSemaphoreGiveRecursive(lvgl_mux);
}

void UIApp::task(void *arg)
{
    UIApp *app = (UIApp *)arg;
    ESP_LOGI(TAG, "Starting LVGL task");
    uint32_t task_delay_ms = EXAMPLE_LVGL_TASK_MAX_DELAY_MS;
    while (1) {
        // Lock the mutex due to the LVGL APIs are not thread-safe
        if (app->lock(-1)) {
            task_delay_ms = lv_timer_handler();
            // Release the mutex
            app->unlock();
        }
        if (task_delay_ms > EXAMPLE_LVGL_TASK_MAX_DELAY_MS) {
            task_delay_ms = EXAMPLE_LVGL_TASK_MAX_DELAY_MS;
        } else if (task_delay_ms < EXAMPLE_LVGL_TASK_MIN_DELAY_MS) {
            task_delay_ms = EXAMPLE_LVGL_TASK_MIN_DELAY_MS;
        }
        vTaskDelay(pdMS_TO_TICKS(task_delay_ms));
    }
}

size_t UIApp::get_buffers(void **buf1, void **buf2)
{
    if (buf2) {
        *buf2 = NULL;
    }
#if CONFIG_EXAMPLE_QEMU_RGB_PANEL_DEDIC_FB
    ESP_LOGI(TAG, "Use QEMU dedicated frame buffer as LVGL draw buffer");
    ESP_ERROR_CHECK(esp_lcd_rgb_qemu_get_frame_buffer(panel_handle, buf1));
    return h_res * v_res;
#else
    ESP_LOGI(TAG, "Allocate separate LVGL draw buffer");
    /* Allocate 10 horizontal lines as the frame buffer */
    *buf1 = malloc(h_res * 10 * sizeof(lv_color_t));
    assert(*buf1);
    return h_res * 10;
#endif // CONFIG_EXAMPLE_DOUBLE_FB
}

lv_disp_t *UIApp::init()
{
    ESP_LOGI(TAG, "Install RGB LCD panel driver");
    esp_lcd_rgb_qemu_config_t panel_config = {
        .width = (uint32_t)h_res,
        .height = (uint32_t)v_res,
        .bpp = (esp_lcd_rgb_qemu_bpp_t)bpp,
    };
    ESP_ERROR_CHECK(esp_lcd_new_rgb_qemu(&panel_config, &panel_handle));

    ESP_LOGI(TAG, "Initialize RGB LCD panel");
    ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
    ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));

    ESP_LOGI(TAG, "Initialize LVGL library");
    lv_init();
    void *buf1 = NULL;
    void *buf2 = NULL;
    const size_t buf_pixels = get_buffers(&buf1, &buf2);
    lv_disp_draw_buf_init(&disp_buf, buf1, buf2, buf_pixels);

    ESP_LOGI(TAG, "Register display driver to LVGL");
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = h_res;
    disp_drv.ver_res = v_res;
    disp_drv.flush_cb = flush_cb;
    disp_drv.draw_buf = &disp_buf;
    disp_drv.user_data = this; // Pass this instance to the callback
#if CONFIG_EXAMPLE_QEMU_RGB_PANEL_DEDIC_FB
    disp_drv.full_refresh = true; // the full_refresh mode can maintain the synchronization between the two frame buffers
#endif
    lv_disp_t *disp = lv_disp_drv_register(&disp_drv);
    this->disp = disp;

    ESP_LOGI(TAG, "Install LVGL tick timer");
    // Tick interface for LVGL (using esp_timer to generate 2ms periodic event)
    const esp_timer_create_args_t lvgl_tick_timer_args = {
        .callback = &increase_tick,
        .name = "lvgl_tick"
    };
    esp_timer_handle_t lvgl_tick_timer = NULL;
    ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer));
    ESP_ERROR_CHECK(esp_timer_start_periodic(lvgl_tick_timer, EXAMPLE_LVGL_TICK_PERIOD_MS * 1000));

    lvgl_mux = xSemaphoreCreateRecursiveMutex();
    assert(lvgl_mux);
    ESP_LOGI(TAG, "Create LVGL task");
    xTaskCreate(task, "LVGL", EXAMPLE_LVGL_TASK_STACK_SIZE, this, EXAMPLE_LVGL_TASK_PRIORITY, NULL);

    return disp;
}

void UIApp::run()
{
    ESP_LOGI(TAG, "Display LVGL Scatter Chart");
    // Lock the mutex due to the LVGL APIs are not thread-safe
    if (lock(-1)) {
        lv_obj_t *scr = lv_disp_get_scr_act(disp);
        
        // Create a container to hold both widgets side-by-side or top-bottom
        // For simplicity, let's just add them to the screen. 
        // They might overlap if not positioned correctly, but OdoMeter uses ALIGN_CENTER.
        // Let's move OdoMeter to top and SpeedMeter to bottom or similar.
        
        // Actually, let's just instantiate both. The user can refine layout later.
        // OdoMeter is centered. SpeedMeter is centered. They will overlap.
        // Let's try to position them.
        
        // Modify OdoMeter to accept position? Or just let them overlap for now as requested "add class".
        // I'll just add it.
        
        int meter_size = (h_res / 2) - 10;
        int offset = h_res / 4;

        auto& vehicleData = VehicleData::instance();
        const auto& data = vehicleData.data();

        EnergyMeter *energy = new EnergyMeter(scr, meter_size, meter_size, -offset, 0);
        energy->setRange(data.energy_min, data.energy_max);
        energy->setThresholds(data.energy_max / 2, data.energy_max * 4 / 5);

        SpeedMeter *speed = new SpeedMeter(scr, meter_size, meter_size, offset, 0);
        speed->setRange(data.speed_min, data.speed_max);
        speed->setThresholds(data.speed_max / 5, data.speed_max * 4 / 5);

        TurnSignal *turn = new TurnSignal(scr, h_res, v_res);
        turn->setLeft(true);
        turn->setRight(true);

        GearIndicator *gear = new GearIndicator(scr);
        
        BluetoothIcon *bt = new BluetoothIcon(scr);
        
        WifiIcon *wifi = new WifiIcon(scr);

        // Release the mutex
        unlock();
    }
}
