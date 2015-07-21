/**
 * @file twic_util_lemng.c
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
#include "twic_util_macro.h"
#include "twic_util_service.h"
#include "twic_util_advertise.h"
#include "twic_util_lemng.h"

void twicUtPrintVal(const char *name, const uint8_t len, const uint8_t * value)
{
  uint8_t i;
  twicPrintf("%s : 0x ", name);
  for (i = 0; i < len; i++) twicPrintf("%02x ", value[i]);
  twicPrintf("\r\n");
}

void twicUtPrintBdAddr(const uint8_t * const addr)
{
  twicPrintf("bdaddr:%02x:%02x:%02x:%02x:%02x:%02x\r\n",
             addr[5], addr[4], addr[3], addr[2], addr[1], addr[0]);
}

void twicUtDisplayTwicErrorList(void)
{
  twicPrintf("TWIC_STATUS_OK = 0x%x.\r\n", TWIC_STATUS_OK);
  twicPrintf("TWIC_STATUS_EVENT_SIGNAL = 0x%x.\r\n",
             TWIC_STATUS_EVENT_SIGNAL);
  twicPrintf("TWIC_STATUS_EVENT_MESSAGE = 0x%x.\r\n",
             TWIC_STATUS_EVENT_MESSAGE);
  twicPrintf("TWIC_STATUS_EVENT_MAIL = 0x%x.\r\n",
             TWIC_STATUS_EVENT_MAIL);
  twicPrintf("TWIC_STATUS_EVENT_TIMEOUT = 0x%x.\r\n",
             TWIC_STATUS_EVENT_TIMEOUT);
  twicPrintf("TWIC_STATUS_ERROR_PARAMETER = 0x%x.\r\n",
             TWIC_STATUS_ERROR_PARAMETER);
  twicPrintf("TWIC_STATUS_ERROR_RESOURCE = 0x%x.\r\n",
             TWIC_STATUS_ERROR_RESOURCE);
  twicPrintf("TWIC_STATUS_ERROR_TIMEOUT_RESOURCE = 0x%x.\r\n",
             TWIC_STATUS_ERROR_TIMEOUT_RESOURCE);
  twicPrintf("TWIC_STATUS_ERROR_ISR = 0x%x.\r\n",
             TWIC_STATUS_ERROR_ISR);
  twicPrintf("TWIC_STATUS_ERROR_ISR_RECURSIVE = 0x%x.\r\n",
             TWIC_STATUS_ERROR_ISR_RECURSIVE);
  twicPrintf("TWIC_STATUS_ERROR_PRIORITY = 0x%x.\r\n",
             TWIC_STATUS_ERROR_PRIORITY);
  twicPrintf("TWIC_STATUS_ERROR_NOMEMORY = 0x%x.\r\n",
             TWIC_STATUS_ERROR_NOMEMORY);
  twicPrintf("TWIC_STATUS_ERROR_VALUE = 0x%x.\r\n",
             TWIC_STATUS_ERROR_VALUE);
  twicPrintf("TWIC_STATUS_ERROR_OS = 0x%x.\r\n",
             TWIC_STATUS_ERROR_OS);
  twicPrintf("TWIC_STATUS_SOURCE_STOPPED = 0x%x.\r\n",
             TWIC_STATUS_SOURCE_STOPPED);
  twicPrintf("TWIC_STATUS_SOURCE_PREPARING = 0x%x.\r\n",
             TWIC_STATUS_SOURCE_PREPARING);
  twicPrintf("TWIC_STATUS_SOURCE_RUNNING = 0x%x.\r\n",
             TWIC_STATUS_SOURCE_RUNNING);
  twicPrintf("TWIC_STATUS_ERROR_IO = 0x%x.\r\n",
             TWIC_STATUS_ERROR_IO);
  twicPrintf("TWIC_STATUS_ERROR_DATA = 0x%x.\r\n",
             TWIC_STATUS_ERROR_DATA);
  twicPrintf("TWIC_STATUS_REQUEST_HANDLE = 0x%x.\r\n",
             TWIC_STATUS_REQUEST_HANDLE);
  twicPrintf("TWIC_STATUS_UNDER_PROCESSING = 0x%x.\r\n",
             TWIC_STATUS_UNDER_PROCESSING);
  twicPrintf("TWIC_STATUS_COMPLETION = 0x%x.\r\n",
             TWIC_STATUS_COMPLETION);
  twicPrintf("TWIC_STATUS_OPERATION_NOT_PERMITTED = 0x%x.\r\n",
             TWIC_STATUS_OPERATION_NOT_PERMITTED);
  twicPrintf("TWIC_STATUS_DISCONNECTED = 0x%x.\r\n",
             TWIC_STATUS_DISCONNECTED);
  twicPrintf("TWIC_STATUS_COMMAND_ELIMINATION = 0x%x.\r\n",
             TWIC_STATUS_COMMAND_ELIMINATION);
  twicPrintf("TWIC_STATUS_WAITING_FOR_ACTIVATION = 0x%x.\r\n",
             TWIC_STATUS_WAITING_FOR_ACTIVATION);
  twicPrintf("TWIC_STATUS_ERROR_HAL = 0x%x.\r\n",
             TWIC_STATUS_ERROR_HAL);
  twicPrintf("TWIC_STATUS_ERROR_TZ1EM = 0x%x.\r\n",
             TWIC_STATUS_ERROR_TZ1EM);
  twicPrintf("TWIC_STATUS_RESERVED = 0x%x.\r\n",
             TWIC_STATUS_RESERVED);
}

void twicUtDoEvent(void)
{
  if (TWIC_STATUS_EVENT_MESSAGE == twicIfPeekEvent()) twicIfDoEvents();
  tz1smHalOsYeild();
}

bool twicUtPeekInApi(twicConnIface_t *cif, uint8_t fidx, uint8_t *aret)
{
  uint8_t _fidx;
  twicStatus_t status;
  
  *aret = 0;
  twicIfLeCeHkTimeout(6000, fidx);
  status = twicIfIsDone(cif, &_fidx);
  if ((TWIC_STATUS_EVENT_MESSAGE == status) && (_fidx == fidx)) {
    if (TWIC_STATUS_OK == twicIfAcceptance(cif, aret, NULL)) return true;
  }
  return false;
}

bool twicUtDbPeekInApi(twicConnIface_t *cif, uint8_t fidx, uint8_t eidx,
                       uint8_t *aret)
{
  uint8_t _fidx, _eidx;
  twicStatus_t status;
  
  *aret = 0;
  twicIfLeCeHkTimeout(3000, fidx);
  status = twicIfDbIsDone(cif, &_fidx, &_eidx, 0, 0);
  if ((TWIC_STATUS_EVENT_MESSAGE == status) && 
      (_fidx == fidx) && (_eidx == eidx)) {
    if (TWIC_STATUS_OK == twicIfDbAcceptance(cif, eidx, aret)) return true;
  }
  return false;
}

bool twicUtCheckAndDoEvent(twicStatus_t status)
{
  if (TWIC_STATUS_OK != status && TWIC_STATUS_UNDER_PROCESSING != status &&
      TWIC_STATUS_WAITING_FOR_ACTIVATION != status) return false;
  twicUtDoEvent();
  return true;
}

#if defined(TWIC_UTIL_GA_SERVICE) || defined(TWIC_UTIL_DI_SERVICE) || \
  defined(TWIC_UTIL_HR_SERVICE) || defined(TWIC_UTIL_BP_SERVICE) ||   \
  defined(TWIC_UTIL_UD_SERVICE) || defined(TWIC_UTIL_CT_SERVICE) ||   \
  defined(TWIC_UTIL_NC_SERVICE) || defined(TWIC_UTIL_RU_SERVICE) ||   \
  defined(TWIC_UTIL_IA_SERVICE) || defined(TWIC_UTIL_HT_SERVICE) ||   \
  defined(TWIC_UTIL_UU_SERVICE)
twicStatus_t twicUtLeDiscoverable(
  twicConnIface_t * const cif,
  uint16_t min_interval, uint16_t max_interval,
  uint8_t advertising_type, uint8_t own_address_type,
  uint8_t direct_address_type, uint64_t direct_address,
  uint8_t advertising_channel_map, uint8_t advertising_filter_policy)
{
  twicStatus_t status = TWIC_STATUS_OK;
  uint8_t _ar;

  for (;twicUtPeekInApi(cif, TWIC_LEDISCOVERABLE, &_ar) != true;) {
    status = twicIfLeDiscoverable(cif, min_interval, max_interval,
                                  advertising_type, own_address_type,
                                  direct_address_type, direct_address,
                                  advertising_channel_map,
                                  advertising_filter_policy,
                                  sizeof(twic_util_advertising_data),
                                  twic_util_advertising_data,
                                  sizeof(twic_util_scan_resp_data),
                                  twic_util_scan_resp_data);
    if (false == twicUtCheckAndDoEvent(status)) return status;
  }
  if (_ar) return TWIC_STATUS_ERROR_IO;
  return TWIC_STATUS_OK;
}
#endif

twicStatus_t twicUtLeConnectionUpdate(twicConnIface_t * const cif,
                                      uint16_t conn_int_min,
                                      uint16_t conn_int_max,
                                      uint16_t slave_latency,
                                      uint16_t supervison_timeout)
{
  twicStatus_t status = TWIC_STATUS_OK;
  uint8_t _ar;

  for (;twicUtPeekInApi(cif, TWIC_LECONNECTIONUPDATE, &_ar) != true;) {
    status = twicIfLeConnectionUpdate(cif, conn_int_min, conn_int_max,
                                      slave_latency, supervison_timeout,
                                      0x20, 0x30);
    if (false == twicUtCheckAndDoEvent(status)) return status;
  }
  if (_ar) return TWIC_STATUS_ERROR_IO;
  return TWIC_STATUS_OK;
}

twicStatus_t twicUtGattServerWriteCharacteristics(
  twicConnIface_t * const cif,
  const uint8_t cha, const uint16_t length, const uint8_t * const value)
{
  twicStatus_t status = TWIC_STATUS_OK;
  uint8_t _ar;

  for (;twicUtPeekInApi(
         cif, TWIC_GATTSERVERWRITECHARACTERISTICS, &_ar) != true;) {
    status = twicIfGattServerWriteCharacteristics(cif, cha, length, value);
    if (false == twicUtCheckAndDoEvent(status)) return status;
  }
  if (_ar) return TWIC_STATUS_ERROR_IO;
  return TWIC_STATUS_OK;
}

twicStatus_t twicUtGattServerExgMtuResponse(
  twicConnIface_t * const cif, const uint16_t rx_mtu_size)
{
  twicStatus_t status = TWIC_STATUS_OK;
  uint8_t _ar;

  for (;twicUtPeekInApi(cif, TWIC_GATTSERVEREXGMTURESPONSE, &_ar) != true;) {
    status = twicIfGattServerExgMtuResponse(cif, 0, rx_mtu_size);
    if (false == twicUtCheckAndDoEvent(status)) return status;
  }
  if (_ar) return TWIC_STATUS_ERROR_IO;
  return TWIC_STATUS_OK;
}

twicStatus_t twicUtGattServerCharValMultiReadOutResponse(
  twicConnIface_t * const cif, uint8_t eidx, uint8_t code)
{
  twicStatus_t status = TWIC_STATUS_OK;
  uint8_t _ar;

  for (;twicUtPeekInApi(cif, TWIC_GATTSERVERCHARVALMULTIREADOUTRESPONSE,
                        &_ar) != true;) {
    status = twicIfGattServerCharValMultiReadOutResponse(cif, code, eidx);
    if (false == twicUtCheckAndDoEvent(status)) return status;
  }
  if (_ar) return TWIC_STATUS_ERROR_IO;
  return TWIC_STATUS_OK;
}

twicStatus_t twicUtGattServerCharValReadOutResponse(
  twicConnIface_t * const cif, uint8_t eidx, uint8_t code)
{
  twicStatus_t status = TWIC_STATUS_OK;
  uint8_t _ar;

  for (;twicUtPeekInApi(
         cif, TWIC_GATTSERVERCHARVALREADOUTRESPONSE, &_ar) != true;) {
    status = twicIfGattServerCharValReadOutResponse(cif, eidx, code);
    if (false == twicUtCheckAndDoEvent(status)) return status;
  }
  if (_ar) return TWIC_STATUS_ERROR_IO;
  return TWIC_STATUS_OK;
}

twicStatus_t twicUtGattServerCharDespReadOutResponse(
  twicConnIface_t * const cif, uint8_t eidx)
{
  twicStatus_t status = TWIC_STATUS_OK;
  uint8_t _ar;

  for (;twicUtPeekInApi(
         cif, TWIC_GATTSERVERCHARDESPREADOUTRESPONSE, &_ar) != true;) {
    status = twicIfGattServerCharDespReadOutResponse(cif, eidx, 0);
    if (false == twicUtCheckAndDoEvent(status)) return status;
  }
  if (_ar) return TWIC_STATUS_ERROR_IO;
  return TWIC_STATUS_OK;
}

twicStatus_t twicUtGattServerCharValWriteInResponse(
  twicConnIface_t * const cif, uint8_t eidx, uint8_t code)
{
  twicStatus_t status = TWIC_STATUS_OK;
  uint8_t _ar;

  for (;twicUtPeekInApi(
         cif, TWIC_GATTSERVERCHARVALWRITEINRESPONSE, &_ar) != true;) {
    status = twicIfGattServerCharValWriteInResponse(cif, eidx, code);
    if (false == twicUtCheckAndDoEvent(status)) return status;
  }
  if (_ar) return TWIC_STATUS_ERROR_IO;
  return TWIC_STATUS_OK;
}

twicStatus_t twicUtGattServerCharDespWriteInResponse(
  twicConnIface_t * const cif, uint8_t eidx)
{
  twicStatus_t status = TWIC_STATUS_OK;
  uint8_t _ar;

  for (;twicUtPeekInApi(
         cif, TWIC_GATTSERVERCHARDESPWRITEINRESPONSE, &_ar) != true;) {
    status = twicIfGattServerCharDespWriteInResponse(cif, eidx, 0);
    if (false == twicUtCheckAndDoEvent(status)) return status;
  }
  if (_ar) return TWIC_STATUS_ERROR_IO;
  return TWIC_STATUS_OK;
}  

uint8_t twicUtGattNotification(
  twicConnIface_t * const cif, uint8_t eidx, uint8_t *data, uint8_t len)
{
  twicStatus_t status = TWIC_STATUS_OK;
  uint8_t _ar;

  for (;twicUtPeekInApi(cif, TWIC_GATTNOTIFICATION, &_ar) != true;) {
    status = twicIfGattNotification(cif, eidx, len, data);
    if (false == twicUtCheckAndDoEvent(status)) return status;
  }
  return _ar;
}

uint8_t twicUtGattIndication(
  twicConnIface_t * const cif, uint8_t eidx, uint8_t *data, uint8_t len)
{
  twicStatus_t status = TWIC_STATUS_OK;
  uint8_t _ar;

  for (;twicUtPeekInApi(cif, TWIC_GATTINDICATION, &_ar) != true;) {
    status = twicIfGattIndication(cif, eidx, len, data);
    if (false == twicUtCheckAndDoEvent(status)) return status;
  }
  return _ar;
}

twicStatus_t twicUtLeStopAdvertising(twicConnIface_t * const cif)
{
  twicStatus_t status = TWIC_STATUS_OK;
  uint8_t _ar;

  for (;twicUtPeekInApi(cif, TWIC_LESTOPADVERTISING, &_ar) != true;) {
    status = twicIfLeStopAdvertising(cif);
    if (false == twicUtCheckAndDoEvent(status)) return status;
  }
  if (_ar) return TWIC_STATUS_ERROR_IO;
  return TWIC_STATUS_OK;
}

twicStatus_t
twicUtLeDisconnect(twicConnIface_t * const cif, const twicBdaddr_t * const bd)
{
  twicStatus_t status = TWIC_STATUS_OK;
  uint8_t _ar;

  for (;twicUtPeekInApi(cif, TWIC_LEDISCONNECT, &_ar) != true;) {
    status = twicIfLeDisconnect(cif, bd);
    if (false == twicUtCheckAndDoEvent(status)) return status;
  }
  if (_ar) return TWIC_STATUS_ERROR_IO;
  return TWIC_STATUS_OK;
}

twicStatus_t
twicUtLeCeLowPowerMode(twicConnIface_t * const cif, bool conn_scan, bool deep)
{
  twicStatus_t status = TWIC_STATUS_OK;
  uint8_t _ar;

  for (;twicUtPeekInApi(cif, TWIC_LECELOWPOWERMODE, &_ar) != true;) {
    status = twicIfLeCeLowPowerMode(cif, true, true, conn_scan, deep);
    if (false == twicUtCheckAndDoEvent(status)) return status;
  }
  if (_ar) return TWIC_STATUS_ERROR_IO;
  return TWIC_STATUS_OK;
}

twicStatus_t twicUtLeSetScanEnable(twicConnIface_t * const cif,
                                   uint16_t interval, uint16_t window,
                                   bool active, bool random, bool whitelist,
                                   bool duplicates)
{
#if defined(TWIC_CONFIG_ENABLE_SCAN)
  twicStatus_t status = TWIC_STATUS_OK;
  uint8_t _ar;

  for (;twicUtPeekInApi(cif, TWIC_LESETSCANENABLE, &_ar) != true;) {
    status = twicIfLeSetScanEnable(cif, interval, window, active, random,
                                   whitelist, duplicates);
    if (false == twicUtCheckAndDoEvent(status)) return status;
  }
  if (_ar) return TWIC_STATUS_ERROR_IO;
  return TWIC_STATUS_OK;
#else
  (void)cif;
  (void)interval;
  (void)window;
  (void)active;
  (void)random;
  (void)whitelist;
  (void)duplicates;
  return TWIC_STATUS_OPERATION_NOT_PERMITTED;
#endif  
}

twicStatus_t twicUtLeSetScanDisable(twicConnIface_t * const cif)
{
#if defined(TWIC_CONFIG_ENABLE_SCAN)
  twicStatus_t status = TWIC_STATUS_OK;
  uint8_t _ar;

  for (;twicUtPeekInApi(cif, TWIC_LESETSCANDISABLE, &_ar) != true;) {
    status = twicIfLeSetScanDisable(cif);
    if (false == twicUtCheckAndDoEvent(status)) return status;
  }
  if (_ar) return TWIC_STATUS_ERROR_IO;
  return TWIC_STATUS_OK;
#else
  (void)cif;
  return TWIC_STATUS_OPERATION_NOT_PERMITTED;
#endif  
}

twicStatus_t twicUtLeCreateConnection(twicConnIface_t * const cif,
                                      uint16_t interval, uint16_t window,
                                      bool whitelist, bool peer,
                                      twicBdaddr_t * addr, uint16_t min,
                                      uint16_t max, uint16_t latency,
                                      uint16_t timeout, bool own)
{
  twicStatus_t status = TWIC_STATUS_OK;
  uint8_t _ar;
  
  for (;twicUtPeekInApi(cif, TWIC_LECREATECONNECTION, &_ar) != true;) {
    status = twicIfLeCreateConnection(cif, interval, window, whitelist, peer,
                                      addr, min, max, latency, timeout, 0, 0,
                                      own);
    if (false == twicUtCheckAndDoEvent(status)) return status;
  }
  if (_ar) return TWIC_STATUS_ERROR_IO;
  return TWIC_STATUS_OK;
}

twicStatus_t twicUtLeCreateConnectionCancel(twicConnIface_t * const cif)
{
  twicStatus_t status = TWIC_STATUS_OK;
  uint8_t _ar;
  
  for (;twicUtPeekInApi(cif, TWIC_LECREATECONNECTIONCANCEL, &_ar) != true;) {
    status = twicIfLeCreateConnectionCancel(cif);
    if (false == twicUtCheckAndDoEvent(status)) return status;
  }
  if (_ar) return TWIC_STATUS_ERROR_IO;
  return TWIC_STATUS_OK;
}

twicStatus_t
twicUtLeCeLowPowerControlPinSetup(twicConnIface_t * const cif, bool pin)
{
  twicStatus_t status = TWIC_STATUS_OK;
  uint8_t _ar;

  for (;twicUtPeekInApi(cif, TWIC_LECELOWPOWERCONTROLPINSETUP, &_ar) != true;) {
    status = twicIfLeCeLowPowerControlPinSetup(cif, pin);
    if (false == twicUtCheckAndDoEvent(status)) return status;
  }
  if (_ar) return TWIC_STATUS_ERROR_IO;
  return TWIC_STATUS_OK;
}
                                               
twicStatus_t twicUtLeCeLowPower(twicConnIface_t * const cif, bool control,
                                bool conn, bool shutdown)
{
  twicStatus_t status;

  if (false == control) {
    status = twicUtLeCeLowPowerMode(cif, false, false);
    if (TWIC_STATUS_OK == status)
      status = twicUtLeCeLowPowerControlPinSetup(cif, false);
  } else {
    status = twicUtLeCeLowPowerControlPinSetup(cif, true);
    if (TWIC_STATUS_OK == status)
      status = twicUtLeCeLowPowerMode(cif, conn, shutdown);
  }
  
  return status;
}

#if defined(TWIC_API_GATTCLIENTDISCOVERPRIMARYSERVICE) ||            \
  defined(TWIC_API_GATTCLIENTDISCOVERPRIMARYSERVICEBYSERVICEUUID) || \
  defined(TWIC_API_GATTCLIENTFINDINCLUDEDSERVICE) ||                 \
  defined(TWIC_API_GATTCLIENTDISCOVERALLCHARACTERISTICS) ||          \
  defined(TWIC_API_GATTCLIENTDISCOVERCHARACTERISTICSBYUUID) ||       \
  defined(TWIC_API_GATTCLIENTDISCOVERALLDESCRIPTORS) ||              \
  defined(TWIC_API_GATTCLIENTREADCHARACTERISTICVALUE) ||             \
  defined(TWIC_API_GATTCLIENTREADUSINGCHARACTERISTICUUID) ||         \
  defined(TWIC_API_GATTCLIENTWRITECHARACTERISTICVALUE) ||            \
  defined(TWIC_API_GATTCLIENTRELIABLEWRITE) ||                       \
  defined(TWIC_API_GATTCLIENTWRITEWITHOUTRESPONSE) ||                \
  defined(TWIC_API_GATTCLIENTSIGNEDWRITEWITHOUTRESPONSE) ||          \
  defined(TWIC_API_GATTCLIENTINDICATIONCONFIRMATIONRESPONSE) ||      \
  defined(TWIC_API_GATTCLIENTREADCHARACTERISTICDESCRIPTOR) ||        \
  defined(TWIC_API_GATTCLIENTREADMULTIPLECHARVALUES) ||              \
  defined(TWIC_API_GATTCLIENTWRITECHARACTERISTICDESCRIPTOR) ||       \
  defined(TWIC_API_GATTCLIENTREADLONGCHARACTERISTICVALUE) ||         \
  defined(TWIC_API_GATTCLIENTREADLONGCHARACTERISTICDESCRIPTOR) ||    \
  defined(TWIC_API_GATTCLIENTWRITELONGCHARACTERISTICVALUE) ||        \
  defined(TWIC_API_GATTCLIENTWRITELONGCHARACTERISTICDESCRIPTOR)
twicIfLeClientCb_t twicUtCcb;
void twicUtGattClientCleanup(void)
{
  memset((void*)&twicUtCcb, 0, sizeof(twicIfLeClientCb_t));
}
#endif
#if defined(TWIC_API_GATTCLIENTDISCOVERPRIMARYSERVICEBYSERVICEUUID)
twicStatus_t
twicUtGattDiscoverPrimaryServiceByServiceUuid(twicConnIface_t * const cif,
                                              uint16_t start_handle,
                                              uint16_t end_handle,
                                              uint64_t uuid_lsb,
                                              uint64_t uuid_msb,
                                              uint8_t uuid_len)
{
  twicStatus_t status = TWIC_STATUS_OK;
  uint8_t _ar;

  for (;twicUtPeekInApi(cif, TWIC_GATTCLIENTDISCOVERPRIMARYSERVICEBYSERVICEUUID,
                        &_ar) != true;) {
    status = twicIfLeGattClientDiscoverPrimaryServiceByServiceUuid(
      cif, start_handle, end_handle, uuid_lsb, uuid_msb, uuid_len);
    if (false == twicUtCheckAndDoEvent(status)) return status;
  }
  if (_ar) return TWIC_STATUS_ERROR_IO;
  return TWIC_STATUS_OK;
}
#endif
#if defined(TWIC_API_GATTCLIENTDISCOVERALLCHARACTERISTICS)
twicStatus_t
twicUtGattClientDiscoverAllCharacteristics(twicConnIface_t * const cif,
                                           uint16_t start_handle,
                                           uint16_t end_handle)
{
  twicStatus_t status = TWIC_STATUS_OK;
  uint8_t _ar;

  for (;twicUtPeekInApi(
         cif, TWIC_GATTCLIENTDISCOVERALLCHARACTERISTICS, &_ar) != true;) {
    status = twicIfLeGattClientDiscoverAllCharacteristics(cif, start_handle,
                                                          end_handle);
    if (false == twicUtCheckAndDoEvent(status)) return status;
  }
  if (_ar) return TWIC_STATUS_ERROR_IO;
  return TWIC_STATUS_OK;
}
#endif
#if defined(TWIC_API_GATTCLIENTDISCOVERALLDESCRIPTORS)
twicStatus_t twicUtGattClientDiscoverAllDescriptors(twicConnIface_t * const cif,
                                                    uint16_t start_handle,
                                                    uint16_t end_handle)
{
  twicStatus_t status = TWIC_STATUS_OK;
  uint8_t _ar;

  for (;twicUtPeekInApi(
         cif, TWIC_GATTCLIENTDISCOVERALLDESCRIPTORS, &_ar) != true;) {
    status = twicIfLeGattClientDiscoverAllDescriptors(cif, start_handle,
                                                      end_handle);
    if (false == twicUtCheckAndDoEvent(status)) return status;
  }
  if (_ar) return TWIC_STATUS_ERROR_IO;
  return TWIC_STATUS_OK;
}
#endif
#if defined(TWIC_API_GATTCLIENTWRITECHARACTERISTICDESCRIPTOR)
twicStatus_t
twicUtGattClientWriteCharacteristicDescriptor(twicConnIface_t * const cif,
                                              uint16_t handle, uint8_t length,
                                              uint8_t * const value)
{
  twicStatus_t status = TWIC_STATUS_OK;
  uint8_t _ar;

  for (;twicUtPeekInApi(
         cif, TWIC_GATTCLIENTWRITECHARACTERISTICDESCRIPTOR, &_ar) != true;) {
    status = twicIfLeGattClientWriteCharacteristicDescriptor(cif, handle,
                                                             length, value);
    if (false == twicUtCheckAndDoEvent(status)) return status;
  }
  if (_ar) return TWIC_STATUS_ERROR_IO;
  return TWIC_STATUS_OK;
}  
#endif
