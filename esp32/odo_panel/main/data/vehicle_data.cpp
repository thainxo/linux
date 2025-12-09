/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "vehicle_data.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "esp_timer.h"

VehicleData& VehicleData::instance() {
    static VehicleData s_instance;
    // Lazy initialization of mutex
    if (s_instance.m_mutex == nullptr) {
        s_instance.m_mutex = xSemaphoreCreateMutex();
    }
    return s_instance;
}

void VehicleData::init() {
    // Create mutex if not already created
    if (m_mutex == nullptr) {
        m_mutex = xSemaphoreCreateMutex();
    }
    
    // Reset to default values
    m_data = VehicleDataState{};
    
    m_callbacks.clear();
    m_callbacks.reserve(MAX_CALLBACKS);
    
    updateTimestamp();
}

bool VehicleData::lock(int timeout_ms) {
    if (m_mutex == nullptr) {
        return false;
    }
    
    TickType_t ticks = (timeout_ms < 0) ? portMAX_DELAY : pdMS_TO_TICKS(timeout_ms);
    return xSemaphoreTake(static_cast<SemaphoreHandle_t>(m_mutex), ticks) == pdTRUE;
}

void VehicleData::unlock() {
    if (m_mutex != nullptr) {
        xSemaphoreGive(static_cast<SemaphoreHandle_t>(m_mutex));
    }
}

void VehicleData::registerCallback(VehicleDataCallback callback, void* user_data) {
    if (m_callbacks.size() < MAX_CALLBACKS && callback) {
        m_callbacks.push_back({callback, user_data});
    }
}

void VehicleData::notify() {
    for (const auto& entry : m_callbacks) {
        if (entry.callback) {
            entry.callback(entry.user_data);
        }
    }
}

void VehicleData::updateTimestamp() {
    m_data.last_update_ms = static_cast<uint32_t>(esp_timer_get_time() / 1000);
}

void VehicleData::setSpeed(int32_t speed) {
    if (lock(100)) {
        m_data.speed_value = speed;
        updateTimestamp();
        unlock();
        notify();
    }
}

void VehicleData::setEnergy(int32_t energy) {
    if (lock(100)) {
        m_data.energy_value = energy;
        updateTimestamp();
        unlock();
        notify();
    }
}

void VehicleData::setGear(int8_t gear) {
    if (lock(100)) {
        m_data.current_gear = gear;
        updateTimestamp();
        unlock();
        notify();
    }
}

void VehicleData::setTurnSignals(bool left, bool right) {
    if (lock(100)) {
        m_data.turn_signal_left = left;
        m_data.turn_signal_right = right;
        updateTimestamp();
        unlock();
        notify();
    }
}

void VehicleData::setBluetooth(bool connected) {
    if (lock(100)) {
        m_data.bluetooth_connected = connected;
        updateTimestamp();
        unlock();
        notify();
    }
}

void VehicleData::setWifi(int8_t strength, bool connected) {
    if (lock(100)) {
        m_data.wifi_strength = strength;
        m_data.wifi_connected = connected;
        updateTimestamp();
        unlock();
        notify();
    }
}
