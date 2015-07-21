/**
 * @file ble_tracker.h
 * @breaf Cerevo CDP-TZ01B sample program.
 * BLE Motion Tracker
 *
 * @author Cerevo Inc.
 */

/*
Copyright 2015 Cerevo Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
*/

#ifndef _BLE_TRACKER_H_
#define _BLE_TRACKER_H_

#include <stdbool.h>

/**
 * @brief Initialize BLE tracker application.
 */
int ble_tracker_init(void);

/**
 * @brief BLE tracker application
 *
 * @param[in] en_9axis          Enabled 9-axis sensor.
 * @param[in] en_airpressure    Enabled Airpressur sensor.
 */
int ble_tracker_run(const bool en_9axis, const bool en_airpressure);

/**
 * @brief Stop BLE tracker application.
 */
void ble_tracker_stop(void);

#endif
