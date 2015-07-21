/**
 * @file   tz01_system_conf.h
 * @brief  Cerevo CDP-TZ01B system configuration.
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

#ifndef _TZ01_SYSTEM_CONF_H_
#define _TZ01_SYSTEM_CONF_H_

/*** TICK TIMER ***/
#define TZ01_SYSTEM_TICK_TMR_NO      (0) /* TMR device no. */
/* TICK TIMER NUMBERS */
typedef enum {
    /* SYSTEM DEFINITION TIMERS */
    SYSTICK_NO_LED_BLINK,       /* Hartbeat LED interval. */
    SYSTICK_NO_PWSW_CHECK,      /* Check PowerSW interval. */
    /* USER DEFINITION TIMERS */
    USRTICK_NO_GPIO_INTERVAL,   /* Check GPIO interval. */
    USRTICK_NO_BLE_MAIN,
    /* Timer count. !DO NOT EDIT! */
    _TICK_NO_COUNT
} TZ01_SYSTEM_TICK_NO;

/*** POWER SWITCH ***/
#define TZ01_SYSTEM_PWSW_PORT_LED    (10)    /* OUT: Power LED */
#define TZ01_SYSTEM_PWSW_PORT_HLD    (3)     /* OUT: Power Hold   */
#define TZ01_SYSTEM_PWSW_PORT_SW     (1)     /* IN:  Power Switch */
#define TZ01_SYSTEM_PWSW_PORT_UVD    (4)     /* IN:  UVdetect */

/*
 * CONSOLE
 */
#define TZ01_CONSOLE_ENABLE  (1)
#if TZ01_CONSOLE_ENABLE
    #define TZ01_CONSOLE_UART_CH (1)
    #define TZ01_CONSOLE_BAUD    (115200)
#endif

#endif //_TZ01_SYSTEM_CONF_H_
