/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "data_simulator.h"
#include "vehicle_data.h"
#include "freertos/FreeRTOS.h"
#include "freertos/timers.h"
#include "esp_random.h"
#include <cstdlib>

namespace {

TimerHandle_t s_simulator_timer = nullptr;
float s_speed_multiplier = 1.0f;
bool s_running = false;

// Simulation state
struct SimState {
    int32_t speed = 0;
    int32_t energy = 75;
    int8_t gear = 0;
    bool speed_increasing = true;
    bool energy_decreasing = false;
    int gear_timer = 0;
    int turn_signal_timer = 0;
    int wifi_timer = 0;
    int bt_timer = 0;
};

SimState s_sim_state;

void simulator_timer_callback(TimerHandle_t xTimer) {
    (void)xTimer; // Unused parameter
    
    if (!s_running) return;
    
    auto& vehicleData = VehicleData::instance();
    const auto& data = vehicleData.data();
    
    // Simulate speed changes (0-200 km/h)
    if (s_sim_state.speed_increasing) {
        s_sim_state.speed += static_cast<int32_t>((esp_random() % 3) + 1);
        if (s_sim_state.speed >= data.speed_max) {
            s_sim_state.speed_increasing = false;
        }
    } else {
        s_sim_state.speed -= static_cast<int32_t>((esp_random() % 3) + 1);
        if (s_sim_state.speed <= 0) {
            s_sim_state.speed = 0;
            s_sim_state.speed_increasing = true;
        }
    }
    vehicleData.setSpeed(s_sim_state.speed);
    
    // Simulate energy/fuel consumption
    s_sim_state.energy_decreasing = (s_sim_state.speed > 0);
    if (s_sim_state.energy_decreasing && (esp_random() % 10) == 0) {
        s_sim_state.energy--;
        if (s_sim_state.energy < 0) {
            s_sim_state.energy = 100; // Refuel!
        }
        vehicleData.setEnergy(s_sim_state.energy);
    }
    
    // Simulate gear changes based on speed
    s_sim_state.gear_timer++;
    if (s_sim_state.gear_timer >= 20) {
        s_sim_state.gear_timer = 0;
        
        int8_t new_gear;
        if (s_sim_state.speed == 0) {
            new_gear = 0; // Neutral
        } else if (s_sim_state.speed < 20) {
            new_gear = 1;
        } else if (s_sim_state.speed < 40) {
            new_gear = 2;
        } else if (s_sim_state.speed < 60) {
            new_gear = 3;
        } else if (s_sim_state.speed < 80) {
            new_gear = 4;
        } else if (s_sim_state.speed < 100) {
            new_gear = 5;
        } else {
            new_gear = 6;
        }
        
        if (new_gear != s_sim_state.gear) {
            s_sim_state.gear = new_gear;
            vehicleData.setGear(s_sim_state.gear);
        }
    }
    
    // Simulate turn signals (random activation)
    s_sim_state.turn_signal_timer++;
    if (s_sim_state.turn_signal_timer >= 100) {
        s_sim_state.turn_signal_timer = 0;
        
        bool left = (esp_random() % 4) == 0;
        bool right = !left && ((esp_random() % 4) == 0);
        vehicleData.setTurnSignals(left, right);
    }
    
    // Simulate WiFi strength changes
    s_sim_state.wifi_timer++;
    if (s_sim_state.wifi_timer >= 50) {
        s_sim_state.wifi_timer = 0;
        int8_t strength = static_cast<int8_t>(esp_random() % 4); // 0-3
        vehicleData.setWifi(strength, strength > 0);
    }
    
    // Simulate Bluetooth connection
    s_sim_state.bt_timer++;
    if (s_sim_state.bt_timer >= 80) {
        s_sim_state.bt_timer = 0;
        bool connected = (esp_random() % 3) != 0; // 66% chance of being connected
        vehicleData.setBluetooth(connected);
    }
}

} // anonymous namespace

void data_simulator_start(void) {
    if (s_simulator_timer != nullptr) {
        return; // Already running
    }
    
    // Initialize simulation state
    s_sim_state = SimState{}; // Reset to defaults
    s_sim_state.energy = 75;
    s_sim_state.speed_increasing = true;
    
    // Create timer (100ms interval = 10 updates per second)
    s_simulator_timer = xTimerCreate(
        "DataSim",
        pdMS_TO_TICKS(static_cast<int>(100 / s_speed_multiplier)),
        pdTRUE, // Auto-reload
        nullptr,
        simulator_timer_callback
    );
    
    if (s_simulator_timer != nullptr) {
        s_running = true;
        xTimerStart(s_simulator_timer, 0);
    }
}

void data_simulator_stop(void) {
    s_running = false;
    if (s_simulator_timer != nullptr) {
        xTimerStop(s_simulator_timer, 0);
        xTimerDelete(s_simulator_timer, 0);
        s_simulator_timer = nullptr;
    }
}

void data_simulator_set_speed(float multiplier) {
    if (multiplier > 0) {
        s_speed_multiplier = multiplier;
        
        // Update timer period if running
        if (s_simulator_timer != nullptr && s_running) {
            xTimerChangePeriod(s_simulator_timer, 
                               pdMS_TO_TICKS(static_cast<int>(100 / s_speed_multiplier)), 
                               0);
        }
    }
}
