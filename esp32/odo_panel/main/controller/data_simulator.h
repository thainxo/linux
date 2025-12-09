/*
 * SPDX-FileCopyrightText: 2023 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#pragma once

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Start the data simulator
 * 
 * Creates a FreeRTOS timer that periodically updates vehicle_data
 * with simulated values for testing purposes.
 */
void data_simulator_start(void);

/**
 * @brief Stop the data simulator
 */
void data_simulator_stop(void);

/**
 * @brief Set simulation speed multiplier
 * @param multiplier Speed multiplier (1.0 = normal, 2.0 = 2x faster, etc.)
 */
void data_simulator_set_speed(float multiplier);

#ifdef __cplusplus
}
#endif
