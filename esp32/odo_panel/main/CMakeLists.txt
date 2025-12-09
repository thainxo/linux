idf_component_register(SRCS "main.cpp" "ui/gear_indicator.cpp" "ui/energy_meter.cpp" "ui/speed_meter.cpp" "ui/turn_signal.cpp" "ui/ui_lvgl.cpp" "ui/bluetooth_icon.cpp" "ui/wifi_icon.cpp" "data/vehicle_data.cpp" "controller/data_simulator.cpp"
                       INCLUDE_DIRS "." "ui" "data" "controller"
                       REQUIRES "esp_lcd" "esp_timer")
