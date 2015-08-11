/**
 * @file ble_tracker.c
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

#include <stdio.h>
#include "TZ10xx.h"
#include "RTC_TZ10xx.h"
#include "PMU_TZ10xx.h"
#include "RNG_TZ10xx.h"
#include "GPIO_TZ10xx.h"

#include "twic_interface.h"
#include "twic_button.h"
#include "twic_led.h"
#include "blelib.h"

#include "TZ01_system.h"
#include "TZ01_console.h"
#include "TZ01_motion_tracker.h"
#include "TZ01_airpressure.h"

#define APP_TZ1EM_WF_GPIO TZ1EM_WF_G2
#define TZ01_TRACKER_MTU    (40)

extern TZ10XX_DRIVER_RNG Driver_RNG;

union bytes_float {
    uint8_t bytes[4];
    float   value;
};

static uint64_t tz01_tracker_bdaddr  = 0xc0ce00000000;   //

/*--- GATT profile definition ---*/
const uint8_t tz01_tracker_gap_device_name[] = "CDP-TZ01B";
const uint8_t tz01_tracker_gap_appearance[] = {0x00, 0x00};

const uint8_t tz01_tracker_di_manufname[] = "Cerevo";
const uint8_t tz01_tracker_di_fw_version[] = "0.1";
const uint8_t tz01_tracker_di_sw_version[] = "0.1";
const uint8_t tz01_tracker_di_model_string[] = "CDP-TZ01B";

/* BLElib unique id. */
enum {
    BLE_GATT_UNIQUE_ID_GAP_SERVICE = 0,
    BLE_GATT_UNIQUE_ID_GAP_DEVICE_NAME,
    BLE_GATT_UNIQUE_ID_GAP_APPEARANCE,
    BLE_GATT_UNIQUE_ID_DI_SERVICE,
    BLE_GATT_UNIQUE_ID_DI_MANUF_NAME,
    BLE_GATT_UNIQUE_ID_DI_FW_VERSION,
    BLE_GATT_UNIQUE_ID_DI_SW_VERSION,
    BLE_GATT_UNIQUE_ID_DI_MODEL_STRING,
    BLE_GATT_UNIQUE_ID_TZ01_SERVICE,
    BLE_GATT_UNIQUE_ID_TZ01_GYRO,
    BLE_GATT_UNIQUE_ID_TZ01_GYRO_DESC,
    BLE_GATT_UNIQUE_ID_TZ01_ACCEL,
    BLE_GATT_UNIQUE_ID_TZ01_ACCEL_DESC,
    BLE_GATT_UNIQUE_ID_TZ01_MAGNETOMETER,
    BLE_GATT_UNIQUE_ID_TZ01_MAGNETOMETER_DESC,
    BLE_GATT_UNIQUE_ID_TZ01_AXISANGLE,
    BLE_GATT_UNIQUE_ID_TZ01_AXISANGLE_DESC,
    BLE_GATT_UNIQUE_ID_TZ01_TEMPERATURE,
    BLE_GATT_UNIQUE_ID_TZ01_TEMPERATURE_DESC,
    BLE_GATT_UNIQUE_ID_TZ01_AIRPRESSURE,
    BLE_GATT_UNIQUE_ID_TZ01_AIRPRESSURE_DESC,
};

/* GAP */
const BLELib_Characteristics gap_device_name = {
    BLE_GATT_UNIQUE_ID_GAP_DEVICE_NAME, 0x2a00, 0, BLELIB_UUID_16,
    BLELIB_PROPERTY_READ,
    BLELIB_PERMISSION_READ | BLELIB_PERMISSION_WRITE,
    tz01_tracker_gap_device_name, sizeof(tz01_tracker_gap_device_name),
    NULL, 0
};
const BLELib_Characteristics gap_appearance = {
    BLE_GATT_UNIQUE_ID_GAP_APPEARANCE, 0x2a01, 0, BLELIB_UUID_16,
    BLELIB_PROPERTY_READ,
    BLELIB_PERMISSION_READ,
    tz01_tracker_gap_appearance, sizeof(tz01_tracker_gap_appearance),
    NULL, 0
};
const BLELib_Characteristics *const gap_characteristics[] = { &gap_device_name, &gap_appearance };
const BLELib_Service gap_service = {
    BLE_GATT_UNIQUE_ID_GAP_SERVICE, 0x1800, 0, BLELIB_UUID_16,
    true, NULL, 0,
    gap_characteristics, 2
};

/* DIS(Device Informatin Service) */
const BLELib_Characteristics di_manuf_name = {
    BLE_GATT_UNIQUE_ID_DI_MANUF_NAME, 0x2a29, 0, BLELIB_UUID_16,
    BLELIB_PROPERTY_READ,
    BLELIB_PERMISSION_READ,
    tz01_tracker_di_manufname, sizeof(tz01_tracker_di_manufname),
    NULL, 0
};
const BLELib_Characteristics di_fw_version = {
    BLE_GATT_UNIQUE_ID_DI_FW_VERSION, 0x2a26, 0, BLELIB_UUID_16,
    BLELIB_PROPERTY_READ,
    BLELIB_PERMISSION_READ,
    tz01_tracker_di_fw_version, sizeof(tz01_tracker_di_fw_version),
    NULL, 0
};
const BLELib_Characteristics di_sw_version = {
    BLE_GATT_UNIQUE_ID_DI_SW_VERSION, 0x2a28, 0, BLELIB_UUID_16,
    BLELIB_PROPERTY_READ,
    BLELIB_PERMISSION_READ,
    tz01_tracker_di_sw_version, sizeof(tz01_tracker_di_sw_version),
    NULL, 0
};
const BLELib_Characteristics di_model_string = {
    BLE_GATT_UNIQUE_ID_DI_MODEL_STRING, 0x2a24, 0, BLELIB_UUID_16,
    BLELIB_PROPERTY_READ,
    BLELIB_PERMISSION_READ,
    tz01_tracker_di_model_string, sizeof(tz01_tracker_di_model_string),
    NULL, 0
};
const BLELib_Characteristics *const di_characteristics[] = {
    &di_manuf_name, &di_fw_version, &di_sw_version, &di_model_string
};
const BLELib_Service di_service = {
    BLE_GATT_UNIQUE_ID_DI_SERVICE, 0x180a, 0, BLELIB_UUID_16,
    true, NULL, 0,
    di_characteristics, 4
};

/* TZ1 motion tracker (GATT base profile) */
static uint8_t tz01_gyro[12];
static uint8_t tz01_gyro_desc[] = { 0x00, 0x00 };
static uint8_t tz01_acel[12];
static uint8_t tz01_acel_desc[] = { 0x00, 0x00 };
static uint8_t tz01_magm[12];
static uint8_t tz01_magm_desc[] = { 0x00, 0x00 };

static uint8_t tz01_axangl[8];
static uint8_t tz01_axangl_desc[] = { 0x00, 0x00};

static uint8_t tz01_temp[4];
static uint8_t tz01_temp_desc[] = { 0x00, 0x00 };
static uint8_t tz01_airp[4];
static uint8_t tz01_airp_desc[] = { 0x00, 0x00 };

/* 9Axis Sensor: GYRO */
const BLELib_Descriptor tz01_tracker_gyro_desc = {
    BLE_GATT_UNIQUE_ID_TZ01_GYRO_DESC, 0x2902, 0, BLELIB_UUID_16,
    BLELIB_PERMISSION_READ | BLELIB_PERMISSION_WRITE,
    tz01_gyro_desc, sizeof(tz01_gyro_desc)
};
const BLELib_Descriptor *const tz01_tracker_gyro_descriptors[] = { &tz01_tracker_gyro_desc };
const BLELib_Characteristics tz01_tracker_gyro = {
    BLE_GATT_UNIQUE_ID_TZ01_GYRO, 0x91825f82ff67e8f8, 0xd43a02010e5f4a80, BLELIB_UUID_128,
    BLELIB_PROPERTY_NOTIFY,
    BLELIB_PERMISSION_READ,
    tz01_gyro, sizeof(tz01_gyro),
    tz01_tracker_gyro_descriptors, 1
};

/* 9Axis Sensor: ACCEL */
const BLELib_Descriptor tz01_tracker_acel_desc = {
    BLE_GATT_UNIQUE_ID_TZ01_ACCEL_DESC, 0x2902, 0, BLELIB_UUID_16,
    BLELIB_PERMISSION_READ | BLELIB_PERMISSION_WRITE,
    tz01_acel_desc, sizeof(tz01_acel_desc)
};
const BLELib_Descriptor *const tz01_tracker_acel_descriptors[] = { &tz01_tracker_acel_desc };
const BLELib_Characteristics tz01_tracker_acel = {
    BLE_GATT_UNIQUE_ID_TZ01_ACCEL, 0x91825f82ff67e8f8, 0xd43a02020e5f4a80, BLELIB_UUID_128,
    BLELIB_PROPERTY_NOTIFY,
    BLELIB_PERMISSION_READ,
    tz01_acel, sizeof(tz01_acel),
    tz01_tracker_acel_descriptors, 1
};

/* 9Axis Sensor: Magnetometer */
const BLELib_Descriptor tz01_tracker_magm_desc = {
    BLE_GATT_UNIQUE_ID_TZ01_MAGNETOMETER_DESC, 0x2902, 0, BLELIB_UUID_16,
    BLELIB_PERMISSION_READ | BLELIB_PERMISSION_WRITE,
    tz01_magm_desc, sizeof(tz01_magm_desc)
};
const BLELib_Descriptor *const tz01_tracker_magm_descriptors[] = { &tz01_tracker_magm_desc };
const BLELib_Characteristics tz01_tracker_magm = {
    BLE_GATT_UNIQUE_ID_TZ01_MAGNETOMETER, 0x91825f82ff67e8f8, 0xd43a02030e5f4a80, BLELIB_UUID_128,
    BLELIB_PROPERTY_NOTIFY,
    BLELIB_PERMISSION_READ,
    tz01_magm, sizeof(tz01_magm),
    tz01_tracker_magm_descriptors, 1
};

/* Axis angle */
const BLELib_Descriptor tz01_tracker_axangl_desc = {
    BLE_GATT_UNIQUE_ID_TZ01_AXISANGLE_DESC, 0x2902, 0, BLELIB_UUID_16,
    BLELIB_PERMISSION_READ | BLELIB_PERMISSION_WRITE,
    tz01_axangl_desc, sizeof(tz01_axangl_desc)
};
const BLELib_Descriptor *const tz01_tracker_axangl_descriptors[] = { &tz01_tracker_axangl_desc };
const BLELib_Characteristics tz01_tracker_axangl = {
    BLE_GATT_UNIQUE_ID_TZ01_AXISANGLE, 0x91825f82ff67e8f8, 0xd43a02040e5f4a80, BLELIB_UUID_128,
    BLELIB_PROPERTY_NOTIFY,
    BLELIB_PERMISSION_READ,
    tz01_axangl, sizeof(tz01_axangl),
    tz01_tracker_axangl_descriptors, 1
};

/* Temperature */
const BLELib_Descriptor tz01_tracker_temp_desc = {
    BLE_GATT_UNIQUE_ID_TZ01_TEMPERATURE_DESC, 0x2902, 0, BLELIB_UUID_16,
    BLELIB_PERMISSION_READ | BLELIB_PERMISSION_WRITE,
    tz01_temp_desc, sizeof(tz01_temp_desc)
};
const BLELib_Descriptor *const tz01_tracker_temp_descriptors[] = { &tz01_tracker_temp_desc };
const BLELib_Characteristics tz01_tracker_temp = {
    BLE_GATT_UNIQUE_ID_TZ01_TEMPERATURE, 0x91825f82ff67e8f8, 0xd43a02110e5f4a80, BLELIB_UUID_128,
    BLELIB_PROPERTY_NOTIFY,
    BLELIB_PERMISSION_READ,
    tz01_temp, sizeof(tz01_temp),
    tz01_tracker_temp_descriptors, 1
};

/* Airpressure */
const BLELib_Descriptor tz01_tracker_airp_desc = {
    BLE_GATT_UNIQUE_ID_TZ01_AIRPRESSURE_DESC, 0x2902, 0, BLELIB_UUID_16,
    BLELIB_PERMISSION_READ | BLELIB_PERMISSION_WRITE,
    tz01_airp_desc, sizeof(tz01_airp_desc)
};
const BLELib_Descriptor *const tz01_tracker_airp_descriptors[] = { &tz01_tracker_airp_desc };
const BLELib_Characteristics tz01_tracker_airp = {
    BLE_GATT_UNIQUE_ID_TZ01_AIRPRESSURE, 0x91825f82ff67e8f8, 0xd43a02120e5f4a80, BLELIB_UUID_128,
    BLELIB_PROPERTY_NOTIFY,
    BLELIB_PERMISSION_READ,
    tz01_airp, sizeof(tz01_airp),
    tz01_tracker_airp_descriptors, 1
};

/* Characteristics list */
const BLELib_Characteristics *const tz01_tracker_characteristics[] = {
    &tz01_tracker_gyro, &tz01_tracker_acel, &tz01_tracker_magm, &tz01_tracker_axangl,
    &tz01_tracker_temp, &tz01_tracker_airp,
};
/* TZ1 Tracker service */
const BLELib_Service tz01_tracker_service = {
    BLE_GATT_UNIQUE_ID_TZ01_SERVICE, 0x91825f82ff67e8f8, 0xd43a02000e5f4a80, BLELIB_UUID_128,
    true, NULL, 0,
    tz01_tracker_characteristics, 6
};

/* Service list */
const BLELib_Service *const tz01_tracker_service_list[] = {
    &gap_service, &di_service, &tz01_tracker_service
};

/*- INDICATION data -*/
uint8_t tz01_tracker_advertising_data[] = {
    0x02, /* length of this data */
    0x01, /* AD type = Flags */
    0x06, /* LE General Discoverable Mode = 0x02 */
    /* BR/EDR Not Supported (i.e. bit 37
     * of LMP Extended Feature bits Page 0) = 0x04 */

    0x03, /* length of this data */
    0x08, /* AD type = Short local name */
    'T',  /* (T) */
    'Z',  /* (Z) */

    0x05, /* length of this data */
    0x03, /* AD type = Complete list of 16-bit UUIDs available */
    0x00, /* Generic Access Profile Service 1800 */
    0x18,
    0x0A, /* Device Information Service 180A */
    0x18,
};

uint8_t tz01_tracker_scan_resp_data[] = {
    0x02, /* length of this data */
    0x01, /* AD type = Flags */
    0x06, /* LE General Discoverable Mode = 0x02 */
    /* BR/EDR Not Supported (i.e. bit 37
     * of LMP Extended Feature bits Page 0) = 0x04 */

    0x02, /* length of this data */
    0x0A, /* AD type = TX Power Level (1 byte) */
    0x00, /* 0dB (-127...127 = 0x81...0x7F) */

    0x0a, /* length of this data */
    0x09, /* AD type = Complete local name */
    'C', 'D', 'P', '-', 'T', 'Z', '0', '1', 'B' /* CDP-TZ01B */
};

/*=== TZ1 motion tracker application ===*/
static union bytes_float maesure_gyro[3];
static union bytes_float maesure_acel[3];
static union bytes_float maesure_magm[3];
static union bytes_float maesure_axangl[2];
static union bytes_float maesure_temp;
static union bytes_float maesure_airp;

static bool led_blink = false;

/*= Timer =*/
static bool tz01_tracker_notif_gyro_enable         = false;
static bool tz01_tracker_notif_accel_enable        = false;
static bool tz01_tracker_notif_magnetometer_enable = false;
static bool tz01_tracker_notif_axis_angle_enable   = false;
static bool tz01_tracker_notif_temperature_enable  = false;
static bool tz01_tracker_notif_airpressure_enable  = false;

/* NOTIFICATION */
static uint8_t notify_dat[12];
/* Eliminate chatter */
void tz01_tracker_enable_button(void const *arg)
{
    twicButtonEnable();
}

/* TimerFakeAlarm */
tz1smHalTimerId tz01_tracker_timer_id_enable_button;
TZ1SM_HAL_TIMER_INIT(TZ01_TRACKER_ENABLE_BUTTON, tz01_tracker_enable_button);

static uint64_t central_bdaddr;

/*= BLElib callback functions =*/
void connectionCompleteCb(const uint8_t status, const bool master, const uint64_t bdaddr, const uint16_t conn_interval)
{
    central_bdaddr = bdaddr;

    //Notification disabled.
    tz01_tracker_notif_gyro_enable = false;
    tz01_tracker_notif_accel_enable = false;
    tz01_tracker_notif_magnetometer_enable = false;
    tz01_tracker_notif_axis_angle_enable = false;
    tz01_tracker_notif_temperature_enable = false;
    tz01_tracker_notif_airpressure_enable = false;

    BLELib_requestMtuExchange(TZ01_TRACKER_MTU);

    //tz1smHalTimerStart(tz01_tracker_timer_id_notif, 500);
    TZ01_system_tick_start(USRTICK_NO_BLE_MAIN, 100);
}

void connectionUpdateCb(const uint8_t status, const uint16_t conn_interval, const uint16_t conn_latency)
{
}

void disconnectCb(const uint8_t status, const uint8_t reason)
{
    //Notification disabled.
    tz01_tracker_notif_gyro_enable = false;
    tz01_tracker_notif_accel_enable = false;
    tz01_tracker_notif_magnetometer_enable = false;
    tz01_tracker_notif_axis_angle_enable = false;
    tz01_tracker_notif_temperature_enable = false;
    tz01_tracker_notif_airpressure_enable = false;

    TZ01_system_tick_stop(USRTICK_NO_BLE_MAIN);
}

BLELib_RespForDemand mtuExchangeDemandCb(const uint16_t client_rx_mtu_size, uint16_t *resp_mtu_size)
{
    uint8_t msg[64];
    *resp_mtu_size = TZ01_TRACKER_MTU;
    sprintf(msg, "client_rx_mtu_size=%d, resp_mtu_size=%d\r\n", client_rx_mtu_size, *resp_mtu_size);
    TZ01_console_puts(msg);
    return BLELIB_DEMAND_ACCEPT;
}

void mtuExchangeResultCb(const uint8_t status, const uint16_t negotiated_mtu_size)
{
    uint8_t msg[32];
    sprintf(msg, "negotiated_mtu_size=%d\r\n", negotiated_mtu_size);
    TZ01_console_puts(msg);
}

void notificationSentCb(const uint8_t unique_id)
{
}

void indicationConfirmCb(const uint8_t unique_id)
{
}

void updateCompleteCb(const uint8_t unique_id)
{
}

void queuedWriteCompleteCb(const uint8_t status)
{
}

BLELib_RespForDemand readoutDemandCb(const uint8_t *const unique_id_array, const uint8_t unique_id_num)
{
    return BLELIB_DEMAND_ACCEPT;
}

BLELib_RespForDemand writeinDemandCb(const uint8_t unique_id, const uint8_t *const value, const uint8_t value_len)
{

    /* Notification Enable/Disable */
    switch (unique_id) {
        case BLE_GATT_UNIQUE_ID_TZ01_GYRO_DESC:
            tz01_tracker_notif_gyro_enable  = (value[0] != 0);
            break;
        case BLE_GATT_UNIQUE_ID_TZ01_ACCEL_DESC:
            tz01_tracker_notif_accel_enable = (value[0] != 0);
            break;
        case BLE_GATT_UNIQUE_ID_TZ01_MAGNETOMETER_DESC:
            tz01_tracker_notif_magnetometer_enable = (value[0] != 0);
            break;
        case BLE_GATT_UNIQUE_ID_TZ01_AXISANGLE_DESC:
            tz01_tracker_notif_axis_angle_enable = (value[0] != 0);
            break;
        case BLE_GATT_UNIQUE_ID_TZ01_TEMPERATURE_DESC:
            tz01_tracker_notif_temperature_enable = (value[0] != 0);
            break;
        case BLE_GATT_UNIQUE_ID_TZ01_AIRPRESSURE_DESC:
            tz01_tracker_notif_airpressure_enable = (value[0] != 0);
            break;
    }
    return BLELIB_DEMAND_ACCEPT;
}

void writeinPostCb(const uint8_t unique_id, const uint8_t *const value, const uint8_t value_len)
{
}

void isrNewEventCb(void)
{
    /* this sample always call BLELib_run() */
}

void isrWakeupCb(void)
{
    /* this callback is not used currently */
}

BLELib_CommonCallbacks tz01_common_callbacks = {
    connectionCompleteCb,
    connectionUpdateCb,
    mtuExchangeResultCb,
    disconnectCb,
    isrNewEventCb,
    isrWakeupCb
  };

BLELib_ServerCallbacks tz01_server_callbacks = {
    mtuExchangeDemandCb,
    notificationSentCb,
    indicationConfirmCb,
    updateCompleteCb,
    queuedWriteCompleteCb,
    readoutDemandCb,
    writeinDemandCb,
    writeinPostCb,
  };


void rtc_periodic_handler(RTC_EVENT e)
{
    tz1smHalTimerFakeAlarmTrigger(128 /* Hz */);
}

int ble_tracker_init(void)
{
    if (TZ1EM_STATUS_OK != tz1emInitializeSystem())
      return 1; /* Must not use UART for LOG before twicIfLeIoInitialize. */

    /*
     * TimerFakeAlarm
     *
     */
    tz01_tracker_timer_id_enable_button = tz1smHalTimerCreate(
        TZ1SM_HAL_TIMER(TZ01_TRACKER_ENABLE_BUTTON), TZ1SM_HAL_TIMER_ONCE, NULL);

    {
        extern TZ10XX_DRIVER_RTC Driver_RTC;
        const RTC_TIME now = {
            55, 59, 11, 8, 6, 15, 1 /* 15-06-08(Mon) 11:59:55 */
        };
        Driver_RTC.Initialize();
        Driver_RTC.SetTime(&now); /* Set current date and time */
        Driver_RTC.SetPeriodicInterrupt(
            RTC_PERIOD_EVERY_1_128_SECOND, rtc_periodic_handler
        );
    }

    /* create random bdaddr */
    uint32_t randval;
    Driver_PMU.SetPowerDomainState(PMU_PD_ENCRYPT, PMU_PD_MODE_ON);
    Driver_RNG.Initialize();
    Driver_RNG.PowerControl(ARM_POWER_FULL);
    Driver_RNG.Read(&randval);
    Driver_RNG.Uninitialize();
    tz01_tracker_bdaddr |= (uint64_t)randval;

    /* initialize BLELib */
    int ret;
    BLELib_initialize(tz01_tracker_bdaddr, BLELIB_BAUDRATE_2304, &tz01_common_callbacks, &tz01_server_callbacks, NULL);
    ret = BLELib_registerService(tz01_tracker_service_list, 3);
    BLELib_setLowPowerMode(BLELIB_LOWPOWER_ON);

    twicButtonInit();
    twicLedInit();
    twicButtonEnable();

    return ret;
}

static uint8_t msg[80];
int ble_tracker_run(const bool en_9axis, const bool en_airpressure)
{
    int ret;
    BLELib_State state;
    bool has_event;
    uint16_t button_state;

    button_state = twicButton();
    state = BLELib_getState();
    has_event = BLELib_hasEvent();

    switch (state) {
        case BLELIB_STATE_UNINITIALIZED:
        case BLELIB_STATE_INITIALIZED:
            break;

        case BLELIB_STATE_ADVERTISE_READY:
            twicSetLed(TWIC_LED_GPIO_LED3, false);
            if (button_state == TWIC_BUTTON_PUSH) {
                ret = BLELib_startAdvertising(tz01_tracker_advertising_data, sizeof(tz01_tracker_advertising_data), tz01_tracker_scan_resp_data, sizeof(tz01_tracker_scan_resp_data));
                sprintf(msg, "BLELib_startAdvertising(): %d state=%d\r\n", ret, state);
                TZ01_console_puts(msg);
            }
            break;
        case BLELIB_STATE_ADVERTISING:
            twicSetLed(TWIC_LED_GPIO_LED3, true);
            if (button_state == TWIC_BUTTON_PUSH) {
                ret = BLELib_stopAdvertising();
                sprintf(msg, "BLELib_stopAdvertising(): %d\r\n", ret);
                TZ01_console_puts(msg);
            }
            break;

        case BLELIB_STATE_ONLINE:
            if (button_state == TWIC_BUTTON_PUSH) {
                ret = BLELib_disconnect(central_bdaddr);
                sprintf(msg, "BLELib_disconnect(): %d\r\n", ret);
                TZ01_console_puts(msg);
                break;
            }

            if (TZ01_system_tick_check_timeout(USRTICK_NO_BLE_MAIN)) {
                TZ01_system_tick_start(USRTICK_NO_BLE_MAIN, 100);
                if (en_9axis) {
                    TZ01_motion_tracker_gyro_read(
                        NULL, NULL, NULL, &maesure_gyro[0].value, &maesure_gyro[1].value, &maesure_gyro[2].value
                    );
                    TZ01_motion_tracker_accel_read(
                        NULL, NULL, NULL, &maesure_acel[0].value, &maesure_acel[1].value, &maesure_acel[2].value
                    );
                    TZ01_motion_tracker_magnetometer_read(
                        NULL, NULL, NULL, &maesure_magm[0].value, &maesure_magm[1].value, &maesure_magm[2].value
                    );

                    TZ01_motion_tracker_compute_axis_angle(
                        maesure_acel[0].value, maesure_acel[1].value, maesure_acel[2].value,
                        &maesure_axangl[0].value, &maesure_axangl[1].value
                    );
                }
                if (en_airpressure) {
                    maesure_temp.value = TZ01_airpressure_temp_read();
                    maesure_airp.value = TZ01_airpressure_press_read();
                }

                int len = 0;
                if (tz01_tracker_notif_gyro_enable) {
                    len = sprintf(
                        msg, "{gx:%0.1f,gy:%0.1f,gz:%0.1f}",
                        maesure_gyro[0].value, maesure_gyro[1].value, maesure_gyro[2].value
                    );
                    ret = BLELib_notifyValue(BLE_GATT_UNIQUE_ID_TZ01_GYRO, msg, len);
                    if (ret != 0) {
                        sprintf(msg, "%d BLELib_notifyValue(): %d\r\n", __LINE__, ret);
                        TZ01_console_puts(msg);
                    }
                }

                if (tz01_tracker_notif_accel_enable) {
                    len = sprintf(
                        msg, "{ax:%0.1f,ay:%0.1f,az:%0.1f}",
                        maesure_acel[0].value, maesure_acel[1].value, maesure_acel[2].value
                    );
                    ret = BLELib_notifyValue(BLE_GATT_UNIQUE_ID_TZ01_ACCEL, msg, len);
                    if (ret != 0) {
                        sprintf(msg, "%d BLELib_notifyValue(): %d\r\n", __LINE__, ret);
                        TZ01_console_puts(msg);
                    }
                }

                if (tz01_tracker_notif_magnetometer_enable) {
                    len = sprintf(
                        msg, "{mx:%0.1f,my:%0.1f,mz:%0.1f}",
                        maesure_magm[0].value, maesure_magm[1].value, maesure_magm[2].value
                    );
                    ret = BLELib_notifyValue(BLE_GATT_UNIQUE_ID_TZ01_MAGNETOMETER, msg, len);
                    if (ret != 0) {
                        sprintf(msg, "%d BLELib_notifyValue(): %d\r\n", __LINE__, ret);
                        TZ01_console_puts(msg);
                    }
                }

                if (tz01_tracker_notif_axis_angle_enable) {
                    len = sprintf(
                        msg, "{p:%0.4f,r:%0.4f}",
                        maesure_axangl[0].value, maesure_axangl[1].value
                    );
                    ret = BLELib_notifyValue(BLE_GATT_UNIQUE_ID_TZ01_AXISANGLE, msg, len);
                    if (ret != 0) {
                        sprintf(msg, "%d BLELib_notifyValue(): %d\r\n", __LINE__, ret);
                        TZ01_console_puts(msg);
                    }
                }

                if (tz01_tracker_notif_temperature_enable) {
                    len = sprintf(msg, "{tp:%0.1f}", maesure_temp.value);
                    ret = BLELib_notifyValue(BLE_GATT_UNIQUE_ID_TZ01_TEMPERATURE, msg, len);
                    if (ret != 0) {
                        sprintf(msg, "%d BLELib_notifyValue(): %d\r\n", __LINE__, ret);
                        TZ01_console_puts(msg);
                    }
                }

                if (tz01_tracker_notif_airpressure_enable) {
                    len = sprintf(msg, "{ap:%0.1f}", maesure_airp.value);
                    ret = BLELib_notifyValue(BLE_GATT_UNIQUE_ID_TZ01_AIRPRESSURE, msg, len);
                    if (ret != 0) {
                        sprintf(msg, "%d BLELib_notifyValue(): %d\r\n", __LINE__, ret);
                        TZ01_console_puts(msg);
                    }
                }
                /* BLE Connection heart beat. */
                twicSetLed(TWIC_LED_GPIO_LED3, led_blink);
                led_blink = led_blink ? false : true;
            }
            break;

        default:
            break;
    }

    if (has_event) {
        ret = BLELib_run();
        if (ret != 0) {
            sprintf(msg, "BLELib_run(): %d\r\n", ret);
            TZ01_console_puts(msg);
        }
    }

    return 0;
}

void ble_tracker_stop(void)
{
    switch (BLELib_getState()) {
        case BLELIB_STATE_ADVERTISING:
            BLELib_stopAdvertising();
            twicSetLed(TWIC_LED_GPIO_LED3, false);
            break;

        case BLELIB_STATE_ONLINE:
            BLELib_disconnect(central_bdaddr);
            break;
        default:
            break;
    }
}
