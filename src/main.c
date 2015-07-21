/**
 * @file main.c
 * @breaf Cerevo CDP-TZ01B sample program.
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

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>

#include "TZ10xx.h"
#include "RNG_TZ10xx.h"
#include "PMU_TZ10xx.h"

#include "TZ01_system.h"
#include "TZ01_console.h"
#include "TZ01_motion_tracker.h"
#include "TZ01_airpressure.h"
#include "TZ01_battery_charger.h"

#include "utils.h"
#include "ble_tracker.h"

static uint8_t msg[80];

int main(void)
{
    /* Initialize */
    if (ble_tracker_init() != 0) {
        return 1;
    }

    TZ01_system_init();
    TZ01_console_init();

    TZ01_console_puts("Initializing...");

    if (TZ01_motion_tracker_init() == false) {
        TZ01_console_puts("TZ01_motion_tracker_init() failed.\r\n");
        goto term_app;
    }

    if (TZ01_airpressure_init() == false) {
        TZ01_console_puts("TZ_airpressure_init() failed.\r\n");
        goto term_app;
    }

    if (TZ01_battery_charger_init(true) == false) {
        TZ01_console_puts("TZ01_battery_charger_init() failed.\r\n");
        goto term_app;
    }

    TZ01_console_puts("DONE.\r\n");

    for (;;) {
        if (TZ01_system_run() == false) {
            /* Power off operation OR Low voltage detected */
            break;
        }

        ble_tracker_run(true, false);
    }

term_app:
    TZ01_console_puts("Program terminated.\r\n");
    return 0;
}
