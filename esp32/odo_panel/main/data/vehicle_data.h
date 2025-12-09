/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#pragma once

#include <cstdint>
#include <functional>
#include <vector>
#include <string>

/**
 * @brief Vehicle data structure containing all live sensor/state data
 * 
 * This is the central database for all vehicle components.
 * UI components read from this structure to update their display.
 * External systems (CAN, sensors, etc.) write to this structure.
 */
struct VehicleDataState {
    // Energy Meter
    int32_t energy_value = 0;       // Current energy/fuel level (0-100)
    int32_t energy_min = 0;         // Minimum value
    int32_t energy_max = 50;        // Maximum value
    
    // Speed Meter
    int32_t speed_value = 0;        // Current speed in km/h or mph
    int32_t speed_min = 0;          // Minimum speed
    int32_t speed_max = 200;        // Maximum speed
    
    // Turn Signals
    bool turn_signal_left = false;  // Left turn signal active
    bool turn_signal_right = false; // Right turn signal active
    bool hazard_lights = false;     // Hazard lights (both signals)
    
    // Gear Indicator
    int8_t current_gear = 0;        // Current gear (-1=R, 0=N, 1-6=forward gears)
    
    // Bluetooth
    bool bluetooth_connected = false;   // Bluetooth connection status
    std::string bluetooth_device;       // Connected device name
    
    // WiFi
    int8_t wifi_strength = 0;       // WiFi signal strength (0=off, 1=weak, 2=medium, 3=strong)
    bool wifi_connected = false;    // WiFi connection status
    std::string wifi_ssid;          // Connected SSID
    
    // Additional vehicle data
    int32_t odometer = 0;           // Total distance traveled
    int32_t trip_meter = 0;         // Trip distance
    int32_t rpm = 0;                // Engine RPM
    int8_t temperature = 0;         // Engine/battery temperature
    uint8_t battery_soc = 0;        // Battery state of charge (%)
    bool engine_running = false;    // Engine/motor running status
    bool parking_brake = false;     // Parking brake engaged
    bool seatbelt_warning = false;  // Seatbelt warning active
    bool door_open = false;         // Any door open
    bool headlights_on = false;     // Headlights status
    bool high_beam_on = false;      // High beam status
    
    // Timestamp
    uint32_t last_update_ms = 0;    // Last update timestamp in milliseconds
};

/**
 * @brief Callback function type for data change notifications
 */
using VehicleDataCallback = std::function<void(void*)>;

/**
 * @brief VehicleData class - Singleton for managing vehicle state
 * 
 * Thread-safe access to vehicle data with callback notification support.
 */
class VehicleData {
public:
    /**
     * @brief Get the singleton instance
     * @return Reference to the VehicleData instance
     */
    static VehicleData& instance();

    // Delete copy constructor and assignment operator
    VehicleData(const VehicleData&) = delete;
    VehicleData& operator=(const VehicleData&) = delete;

    /**
     * @brief Initialize vehicle data with default values
     */
    void init();

    /**
     * @brief Lock the vehicle data for thread-safe access
     * @param timeout_ms Timeout in milliseconds (-1 for infinite)
     * @return true if lock acquired, false on timeout
     */
    bool lock(int timeout_ms = -1);

    /**
     * @brief Unlock the vehicle data
     */
    void unlock();

    /**
     * @brief Get read-only access to the data state
     * @return Const reference to VehicleDataState
     */
    const VehicleDataState& data() const { return m_data; }

    /**
     * @brief Get mutable access to the data state (use with lock!)
     * @return Reference to VehicleDataState
     */
    VehicleDataState& data() { return m_data; }

    /**
     * @brief Register a callback for data changes
     * @param callback Function to call when data changes
     * @param user_data User data to pass to callback
     */
    void registerCallback(VehicleDataCallback callback, void* user_data = nullptr);

    /**
     * @brief Notify all registered callbacks of data change
     */
    void notify();

    // Thread-safe setter functions
    void setSpeed(int32_t speed);
    void setEnergy(int32_t energy);
    void setGear(int8_t gear);
    void setTurnSignals(bool left, bool right);
    void setBluetooth(bool connected);
    void setWifi(int8_t strength, bool connected);

    // Convenience getters (thread-safe snapshots)
    int32_t getSpeed() const { return m_data.speed_value; }
    int32_t getEnergy() const { return m_data.energy_value; }
    int8_t getGear() const { return m_data.current_gear; }
    bool getTurnSignalLeft() const { return m_data.turn_signal_left; }
    bool getTurnSignalRight() const { return m_data.turn_signal_right; }
    bool getBluetoothConnected() const { return m_data.bluetooth_connected; }
    int8_t getWifiStrength() const { return m_data.wifi_strength; }
    bool getWifiConnected() const { return m_data.wifi_connected; }

private:
    VehicleData() = default;
    ~VehicleData() = default;

    void updateTimestamp();

    VehicleDataState m_data;
    void* m_mutex = nullptr;

    struct CallbackEntry {
        VehicleDataCallback callback;
        void* user_data;
    };
    std::vector<CallbackEntry> m_callbacks;
    static constexpr int MAX_CALLBACKS = 10;
};
