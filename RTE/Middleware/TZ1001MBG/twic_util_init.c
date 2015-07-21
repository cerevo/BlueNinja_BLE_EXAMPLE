/**
 * @file twic_util_init.c
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
#include "twic_util_lemng.h"

twicStatus_t twicUtLeCeInit0(twicConnIface_t *cif, uint8_t *aret, uint16_t ext)
{
#if defined(TWIC_API_LECEDBUSWRITE)
  twicStatus_t status;
  uint8_t _ar;
  
  /* API needs to be in the loop to wait NOP. */
  for (;twicUtPeekInApi(cif, TWIC_LECEDBUSWRITE, &_ar) != true;) {
    status = twicIfLeCeDbusWrite(cif, 0xD7, ext);
    if (TWIC_STATUS_OK != status && TWIC_STATUS_UNDER_PROCESSING != status)
      return status;
    twicUtDoEvent();
  }
  if (NULL != aret) *aret = _ar;
  if (_ar) return TWIC_STATUS_ERROR_IO;
#endif
  return TWIC_STATUS_OK;
}


twicStatus_t twicUtLeCeInit1(twicConnIface_t *cif, uint8_t *aret,
                             uint8_t *wait_ms, twicTzbtBr_t br, uint16_t pu)
{
  twicStatus_t status;
  uint8_t _ar;
  
  /* API needs to be in the loop to wait NOP. */
  for (;twicUtPeekInApi(cif, TWIC_LECESETBAUDRATE, &_ar) != true;) {
    status = twicIfLeCeSetBaudrate(cif, br, pu);
    if (TWIC_STATUS_OK != status && TWIC_STATUS_UNDER_PROCESSING != status)
      return status;
    twicUtDoEvent();
  }
  if (NULL != aret) *aret = _ar;

  if (_ar) {
    if (NULL != wait_ms) *wait_ms = 0;
    return TWIC_STATUS_ERROR_IO;
  }
  if (NULL != wait_ms) *wait_ms = 100;

  return TWIC_STATUS_OK;
}

twicStatus_t twicUtLeCeInit2(twicConnIface_t *cif, uint8_t *aret,
                             uint8_t *wait_ms, uint64_t *bd_addr,
                             bool low_power)
{
  twicStatus_t status;
  uint8_t _ar;

  status = twicIfLeReadBdaddr(cif);
  if (TWIC_STATUS_OK != status) return status;
  for (;twicUtPeekInApi(cif, TWIC_LEREADBDADDR, &_ar) != true;)
    twicUtDoEvent();
  if (NULL != aret) *aret = _ar;
  if (_ar) return TWIC_STATUS_ERROR_IO;

  status = twicIfLeCeFlowControl(cif, true);
  if (TWIC_STATUS_OK != status) return status;
  for (;twicUtPeekInApi(cif, TWIC_LECEFLOWCONTROL, &_ar) != true;)
    twicUtDoEvent();
  if (NULL != aret) *aret = _ar;
  if (_ar) return TWIC_STATUS_ERROR_IO;

  if (false == low_power) status = twicIfLeCeHostDelay(cif, 0x0000);
  else status = twicIfLeCeHostDelay(cif, 0x000A);
  if (TWIC_STATUS_OK != status) return status;
  for (;twicUtPeekInApi(cif, TWIC_LECEHOSTDELAY, &_ar) != true;)
    twicUtDoEvent();
  if (NULL != aret) *aret = _ar;
  if (_ar) return TWIC_STATUS_ERROR_IO;
    
  status = twicIfLeWriteBdaddr(cif, bd_addr);
  if (TWIC_STATUS_OK != status) return status;
  for (;twicUtPeekInApi(cif, TWIC_LEWRITEBDADDR, &_ar) != true;)
    twicUtDoEvent();
  if (NULL != aret) *aret = _ar;
  if (_ar) return TWIC_STATUS_ERROR_IO;

  if (false == low_power) goto change_to_tcu;
#if !defined(TWIC_LECE_LOWPOWER)
  if (true == low_power) return TWIC_STATUS_ERROR_PARAMETER;
#endif /* TWIC_LECE_LOWPOWER */

  status = twicIfLeCeLowPowerClockSetup(cif, NULL);
  if (TWIC_STATUS_OK != status) return status;
  for (;twicUtPeekInApi(cif, TWIC_LECELOWPOWERCLOCKSETUP, &_ar) != true;)
    twicUtDoEvent();
  if (NULL != aret) *aret = _ar;
  if (_ar) return TWIC_STATUS_ERROR_IO;

  status = twicIfLeCeLowPowerPrimarySetup(cif, 0x00FA, 0xA);
  if (TWIC_STATUS_OK != status) return status;
  for (;twicUtPeekInApi(cif, TWIC_LECELOWPOWERPRIMARYSETUP, &_ar) != true;)
    twicUtDoEvent();
  if (NULL != aret) *aret = _ar;
  if (_ar) return TWIC_STATUS_ERROR_IO;

  status = twicIfLeCeLowPowerControlPinSetup(cif, true);
  if (TWIC_STATUS_OK != status) return status;
  for (;twicUtPeekInApi(cif, TWIC_LECELOWPOWERCONTROLPINSETUP, &_ar) != true;)
    twicUtDoEvent();
  if (NULL != aret) *aret = _ar;
  if (_ar) return TWIC_STATUS_ERROR_IO;
  
  status = twicIfLeCeLowPowerDiscoverableSetup(cif);
  if (TWIC_STATUS_OK != status) return status;
  for (;twicUtPeekInApi(cif, TWIC_LECELOWPOWERDISCOVERABLESETUP, &_ar) != true;)
    twicUtDoEvent();
  if (NULL != aret) *aret = _ar;
  if (_ar) return TWIC_STATUS_ERROR_IO;

  change_to_tcu:
  
  status = twicIfLeCeChangeToCm(cif);
  if (TWIC_STATUS_OK != status) return status;
  for (;twicUtPeekInApi(cif, TWIC_LECECHANGETOCM, &_ar) != true;)
    twicUtDoEvent();
  if (NULL != aret) *aret = _ar;
  if (_ar) {
    if (NULL != wait_ms) *wait_ms = 0;
    return TWIC_STATUS_ERROR_IO;
  }
  if (NULL != wait_ms) *wait_ms = 30;

  return TWIC_STATUS_OK;
}

twicStatus_t twicUtLeCeInit3(twicConnIface_t * const cif, uint8_t *aret,
                             bool client)
{
  twicStatus_t status;
  uint8_t _ar;

  status = twicIfLeInitializeDevice(cif);
  if (TWIC_STATUS_OK != status) return status;
  for (;twicUtPeekInApi(cif, TWIC_LEINITIALIZEDEVICE, &_ar) != true;)
    twicUtDoEvent();
  if (NULL != aret) *aret = _ar;
  if (_ar) return TWIC_STATUS_ERROR_IO;

  status = twicIfLeGattServerStart(cif);
  if (TWIC_STATUS_OK != status) return status;
  for (;twicUtPeekInApi(cif, TWIC_LEGATTSERVERSTART, &_ar) != true;)
    twicUtDoEvent();
  if (NULL != aret) *aret = _ar;
  if (_ar) return TWIC_STATUS_ERROR_IO;

  if (false == client) return TWIC_STATUS_OK;
  
  status = twicIfLeGattClientStart(cif);
  if (TWIC_STATUS_OK != status) return status;
  for (;twicUtPeekInApi(cif, TWIC_LEGATTCLIENTSTART, &_ar) != true;)
    twicUtDoEvent();
  if (NULL != aret) *aret = _ar;
  if (_ar) return TWIC_STATUS_ERROR_IO;

  return TWIC_STATUS_OK;
}
