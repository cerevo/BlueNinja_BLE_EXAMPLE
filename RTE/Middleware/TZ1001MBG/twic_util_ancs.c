/**
 * @file twic_util_ancs.c
 * @brief a source file for TZ10xx TWiC for Bluetooth 4.0 Smart
 * @version V1.0.0.FS (Free Sample - The information in this code is
 * subject to change without notice and should not be construed as a
 * commitment by TOSHIBA CORPORATION SEMICONDUCTOR & STORAGE PRODUCTS
 * COMPANY.
 * @note TZ1EM provides the automatic low energy consumption. The low
 * energy consumption of interal BLE Processor and UART2 is managed by
 * the part of TWIC BLE CE (BLE Controller Extension). Please refer to
 * the twicIfLeCe API group. TZ1EM combines the HOST Core low energy
 * consumption with the BLE CE and the other peripheral modules.
 */

/*
 * COPYRIGHT (C) 2015
 * TOSHIBA CORPORATION SEMICONDUCTOR & STORAGE PRODUCTS COMPANY
 * ALL RIGHTS RESERVED.
 *
 * THE SOURCE CODE AND ITS RELATED DOCUMENTATION IS PROVIDED "AS
 * IS". TOSHIBA CORPORATION MAKES NO OTHER WARRANTY OF ANY KIND,
 * WHETHER EXPRESS, IMPLIED OR, STATUTORY AND DISCLAIMS ANY AND ALL
 * IMPLIED WARRANTIES OF MERCHANTABILITY, SATISFACTORY QUALITY, NON
 * INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE.
 * 
 * THE SOURCE CODE AND DOCUMENTATION MAY INCLUDE ERRORS. TOSHIBA
 * CORPORATION RESERVES THE RIGHT TO INCORPORATE MODIFICATIONS TO THE
 * SOURCE CODE IN LATER REVISIONS OF IT, AND TO MAKE IMPROVEMENTS OR
 * CHANGES IN THE DOCUMENTATION OR THE PRODUCTS OR TECHNOLOGIES
 * DESCRIBED THEREIN AT ANY TIME.
 * 
 * TOSHIBA CORPORATION SHALL NOT BE LIABLE FOR ANY DIRECT, INDIRECT OR
 * CONSEQUENTIAL DAMAGE OR LIABILITY ARISING FROM YOUR USE OF THE
 * SOURCE CODE OR ANY DOCUMENTATION, INCLUDING BUT NOT LIMITED TO,
 * LOST REVENUES, DATA OR PROFITS, DAMAGES OF ANY SPECIAL, INCIDENTAL
 * OR CONSEQUENTIAL NATURE, PUNITIVE DAMAGES, LOSS OF PROPERTY OR LOSS
 * OF PROFITS ARISING OUT OF OR IN CONNECTION WITH THIS AGREEMENT, OR
 * BEING UNUSABLE, EVEN IF ADVISED OF THE POSSIBILITY OR PROBABILITY
 * OF SUCH DAMAGES AND WHETHER A CLAIM FOR SUCH DAMAGE IS BASED UPON
 * WARRANTY, CONTRACT, TORT, NEGLIGENCE OR OTHERWISE.
 *
 */

#include "twic_interface.h"
#include "twic_util_service.h"
#include "twic_util_lemng.h"

#if defined(TWIC_UTIL_ANCS)

/* Multiple piconet is not supported in this sample implementation. */
typedef struct twicUtAncsNcState {
  uint8_t start_finding_service : 1;
  uint8_t start_finding_characteristics : 1;
  uint8_t start_finding_notif_source_descriptor : 1;
  uint8_t ready_to_subscribe_notif_source : 1;
  uint8_t subscribe_notif_source : 1;
  uint8_t pad : 3;
  uint16_t start_handle;
  uint16_t end_handle;
  uint16_t notification_source_char_val_handle;
  uint16_t control_point_char_val_handle;
  uint16_t data_source_char_val_handle;
  uint16_t notification_source_char_desp_handle;
  uint8_t notification_source_char_desp_value[2];
} twicUtAncsNcState_t;
static twicUtAncsNcState_t ncState;

void twicUtAncsSetupNc(void)
{
  memset((void*)&ncState, 0, sizeof(twicUtAncsNcState_t));
}

void twicUtAncsStopNc(void)
{
  twicUtAncsSetupNc();
}

void twicUtAncsStartNc(void)
{
  ncState.start_finding_service = true;
}

static void twicUtAncsNcPrimaryServiceByUuid(
  const void * const connif, const uint8_t status, const bool next,
  const uint16_t error_handle, const uint16_t attribute_handle,
  const uint16_t end_group_handle)
{
  twicTrace();
  twicPrintf("status:0x%02x\r\n", status);
  twicPrintf("next:%d\r\n", next);
  twicPrintf("error_handle:0x%04x\r\n", error_handle);
  twicPrintf("attribute_handle:0x%04x\r\n", attribute_handle);
  twicPrintf("end_group_handle:0x%04x\r\n", end_group_handle);

  if (0 == status && 0 == error_handle) {
    ncState.start_finding_characteristics = true;
  } else {
    ncState.start_finding_characteristics = false;
  }
  ncState.start_handle = attribute_handle;
  ncState.end_handle = end_group_handle;
  ncState.notification_source_char_val_handle = 0;
  ncState.control_point_char_val_handle = 0;
  ncState.data_source_char_val_handle = 0;
  ncState.notification_source_char_desp_handle = 0;
}

static void twicUtAncsNcAllCharOfService(
  const void * const connif, const uint8_t status, const bool next,
  const uint16_t error_handle, const uint16_t attribute_handle,
  const uint8_t char_properties, const uint16_t char_value_handle,
  const uint64_t uuid_lsb, const uint64_t uuid_msb, const uint8_t uuid_len)
{
  twicUuid_t ancs_uuid;

  twicTrace();
  twicPrintf("status:0x%02x\r\n", status);
  twicPrintf("next:%d\r\n", next);
  twicPrintf("error_handle:0x%04x\r\n", error_handle);
  twicPrintf("attribute_handle:0x%04x\r\n", attribute_handle);
  twicPrintf("char_properties:0x%02x\r\n", char_properties);
  twicPrintf("char_value_handle:0x%04x\r\n", char_value_handle);
  twicPrintf("uuid_msb:0x%llx\r\n", uuid_msb);
  twicPrintf("uuid_lsb:0x%llx\r\n", uuid_lsb);
  twicPrintf("uuid_len:0x%02x\r\n", uuid_len);

  twicSetUuid((uint8_t *)&ancs_uuid.uu, uuid_lsb, uuid_msb);
  twicUtPrintVal("UUID", uuid_len, (uint8_t *)&ancs_uuid.uu);

  /* Notification Source: UUID 9FBF120D-6301-42D9-8C58-25E699A21DBD
   * (notifiable) */
  if (0x9FBF120D630142D9 == uuid_msb && 0x8C5825E699A21DBD == uuid_lsb) {
    twicPrintf("FOUND ANCS Notification Source characteristics value.\r\n");
    ncState.notification_source_char_val_handle = char_value_handle;
    ncState.notification_source_char_desp_handle = 0;
    ncState.start_finding_notif_source_descriptor = true;
  }
  /* Control Point: UUID 69D1D8F3-45E1-49A8-9821-9BBDFDAAD9D9
   * (writeable with response) */
  if (0x69D1D8F345E149A8 == uuid_msb && 0x98219BBDFDAAD9D9 == uuid_lsb) {
    twicPrintf("FOUND ANCS Control Point characteristics value.\r\n");
    ncState.control_point_char_val_handle = char_value_handle;
  }
  /* Data Source: UUID 22EAC6E9-24D6-4BB5-BE44-B36ACE7C7BFB (notifiable) */
  if (0x22EAC6E924D64BB5 == uuid_msb && 0xBE44B36ACE7C7BFB == uuid_lsb) {
    twicPrintf("FOUND ANCS Data Source characteristics value.\r\n");
    ncState.data_source_char_val_handle = char_value_handle;
  }
}

static void twicUtAncsNcAllCharDescriptors(
  const void * const connif, const uint8_t status, const bool next,
  const uint16_t error_handle, const uint16_t attribute_handle,
  const uint64_t uuid_lsb, const uint64_t uuid_msb, const uint8_t uuid_len)
{
  twicUuid_t ancs_uuid;

  twicTrace();
  twicPrintf("status:0x%02x\r\n", status);
  twicPrintf("next:%d\r\n", next);
  twicPrintf("error_handle:0x%04x\r\n", error_handle);
  twicPrintf("attribute_handle:0x%04x\r\n", attribute_handle);
  twicPrintf("uuid_msb:0x%llx\r\n", uuid_msb);
  twicPrintf("uuid_lsb:0x%llx\r\n", uuid_lsb);
  twicPrintf("uuid_len:0x%02x\r\n", uuid_len);
  twicSetUuid((uint8_t *)&ancs_uuid.uu, uuid_lsb, uuid_msb);
  twicUtPrintVal("UUID", uuid_len, (uint8_t *)&ancs_uuid.uu);

  if (0x2902 == uuid_lsb) {
    twicPrintf("FOUND CHAR DESP of ANCS Notification.\r\n");
    ncState.notification_source_char_desp_handle = attribute_handle;
    ncState.notification_source_char_desp_value[0] = 0x01;
    ncState.notification_source_char_desp_value[1] = 0x00;
    ncState.ready_to_subscribe_notif_source = true;
    /* This sample just subscribe Notification Source soon to explain
     * ANCS NC briefly. */
    ncState.subscribe_notif_source = true;    
  }
}

static void twicUtAncsNcCharDespWriteinResponse(const void * const connif,
                                                const uint8_t status)
{
  twicTrace();
  twicPrintf("status:0x%02x\r\n", status);
  /* iOS7/iOS8 iPodTouch and iPhone6 OK. */
  if (0 == status) {
    twicPrintf("Subscription Success.\r\n");
  } else if (0x05 == status) {
    twicPrintf("Subscription failed due to Insufficient Authentication.\r\n");
    ncState.subscribe_notif_source = true;
  } else if (0x0F == status) {
    twicPrintf("Subscription failed due to Insufficient Encryption.\r\n");
    ncState.subscribe_notif_source = true;
  } else {
    twicPrintf("Subscription failed due to unexpected reason.\r\n");
    ncState.subscribe_notif_source = true;
  }
}

static void twicUtAncsPrepareNc(twicConnIface_t * const cif)
{
  twicStatus_t status;
  
  if (true == ncState.start_finding_service) {
    if (NULL != twicUtCcb.primary_service_by_uuid) return;
    twicUtCcb.primary_service_by_uuid = &twicUtAncsNcPrimaryServiceByUuid;
    /* The Apple Notification Center Service is a primary service
     * whose service UUID is 7905F431-B5CE-4E99-A40F-4B1E122D00D0. */
    twicLog("'Discover Primary Service By ServiceUuid'.\r\n");
    status = twicUtGattDiscoverPrimaryServiceByServiceUuid(
      cif, 0x0001, /* start handle */ 0xFFFF, /* end_handle */
      0xA40F4B1E122D00D0, /* uuid_lso */ 0x7905F431B5CE4E99, /* mso */ 16);
    ncState.start_finding_service = false;
    if (TWIC_STATUS_OK != status) {
      twicLog("Failure in DiscoverPrimaryServiceByServiceUuid.\r\n");
    }
  }
  if (true == ncState.start_finding_characteristics) {
    if (NULL != twicUtCcb.all_char_of_service) return;
    twicUtCcb.all_char_of_service = &twicUtAncsNcAllCharOfService;
    twicLog("'Discover All Characteristics'.\r\n");
    status = twicUtGattClientDiscoverAllCharacteristics(
      cif, ncState.start_handle, ncState.end_handle);
    ncState.start_finding_characteristics = false;
    if (TWIC_STATUS_OK != status) {
      twicLog("Failure in DiscoverAllCharacteristics.\r\n");
    }
  }
  if (true == ncState.start_finding_notif_source_descriptor) {
    if (NULL != twicUtCcb.all_char_descriptors) return;
    twicUtCcb.all_char_descriptors = &twicUtAncsNcAllCharDescriptors;
    twicLog("'Discover All Descriptors'.\r\n");
    status = twicUtGattClientDiscoverAllDescriptors(
      cif, ncState.notification_source_char_val_handle + 1,
      ncState.notification_source_char_val_handle + 1);
    ncState.start_finding_notif_source_descriptor = false;
    if (TWIC_STATUS_OK != status) {
      twicLog("Failure in DiscoverAllDescriptors.\r\n");
    }
  }
}

static void twicUtAncsNcSubscribeNotificationSource(twicConnIface_t * const cif)
{
  twicStatus_t status;
  
  if (false == ncState.ready_to_subscribe_notif_source) return;
  if (true == ncState.subscribe_notif_source) {
    ncState.subscribe_notif_source = false;
    twicUtCcb.char_desp_writein_response = &twicUtAncsNcCharDespWriteinResponse;
    twicLog("'Subscribe Notification Source'.\r\n");
    status = twicUtGattClientWriteCharacteristicDescriptor(
      cif, ncState.notification_source_char_desp_handle,
      2, ncState.notification_source_char_desp_value);
    if (TWIC_STATUS_OK != status) {
      twicLog("Failure in WriteCharacteristicDescriptor (0x%x).\r\n", status);
    }
  }
}

void twicUtAncsRunNc(twicConnIface_t * const cif)
{
  twicUtAncsPrepareNc(cif);
  twicUtAncsNcSubscribeNotificationSource(cif);
}

void twicUtAncsNcNotificationReceived(const void * const cif,
                                      const uint8_t status,
                                      const uint16_t char_value_handle,
                                      const twicAttValue_t *const resp)
{
  twicTrace();
  twicPrintf("status:0x%02x\r\n", status);
  twicPrintf("char_value_handle:0x%04x\r\n", char_value_handle);
  twicUtPrintVal("VALUE", resp->parameter_length, resp->value);
}

#else

void twicUtAncsSetupNc(void) {return;}
void twicUtAncsStopNc(void) {return;}
void twicUtAncsStartNc(void) {return;}
void twicUtAncsRunNc(twicConnIface_t * const cif)
{
  (void)cif;
}
void twicUtAncsNcNotificationReceived(const void * const cif,
                                      const uint8_t status,
                                      const uint16_t char_value_handle,
                                      const twicAttValue_t *const resp)
{
  (void)cif;
  (void)status;
  (void)char_value_handle;
  (void)resp;
}

#endif
