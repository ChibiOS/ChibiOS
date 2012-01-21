/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    STM32/mac_lld.c
 * @brief   STM32 low level MAC driver code.
 *
 * @addtogroup MAC
 * @{
 */

#include "ch.h"
#include "hal.h"
#include "mii.h"

#if HAL_USE_MAC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define BUFFER_SLICE ((((MAC_BUFFERS_SIZE - 1) | 3) + 1) / 4)

/* MII divider optimal value.*/
#if (STM32_HCLK >= 60000000)
#define MACMIIDR_CR     ETH_MACMIIAR_CR_Div42
#elif (STM32_HCLK >= 35000000)
#define MACMIIDR_CR     ETH_MACMIIAR_CR_Div26
#elif (STM32_HCLK >= 20000000)
#define MACMIIDR_CR     ETH_MACMIIAR_CR_Div16
#else
#error "STM32_HCLK below minimum frequency for ETH operations (20MHz)"
#endif

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   Ethernet driver 1.
 */
MACDriver ETHD1;

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

static uint32_t phyaddr;

static stm32_eth_rx_descriptor_t *rxptr;
static stm32_eth_tx_descriptor_t *txptr;

static stm32_eth_rx_descriptor_t rd[MAC_RECEIVE_BUFFERS];
static stm32_eth_tx_descriptor_t td[MAC_TRANSMIT_BUFFERS];

static uint32_t rb[MAC_RECEIVE_BUFFERS * BUFFER_SLICE];
static uint32_t tb[MAC_TRANSMIT_BUFFERS * BUFFER_SLICE];

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Writes a PHY register.
 *
 * @param[in] reg       register number
 * @param[in] value     new register value
 */
static void mii_write_phy(uint32_t reg, uint32_t value) {

  ETH->MACMIIDR = value;
  ETH->MACMIIAR = phyaddr | (reg << 6) | MACMIIDR_CR |
                  ETH_MACMIIAR_MW | ETH_MACMIIAR_MB;
  while ((ETH->MACMIIAR & ETH_MACMIIAR_MB) != 0)
    ;
}

/**
 * @brief   Reads a PHY register.
 *
 * @param[in] reg       register number
 */
static uint32_t mii_read_phy(uint32_t reg) {

  ETH->MACMIIAR = phyaddr | (reg << 6) | MACMIIDR_CR |
                  ETH_MACMIIAR_MB;
  while ((ETH->MACMIIAR & ETH_MACMIIAR_MB) != 0)
    ;
  return ETH->MACMIIDR;
}


/**
 * @brief   PHY address detection.
 */
static void mii_find_phy(void) {
  uint32_t i;

  for (i = 0; i < 31; i++) {
    ETH->MACMIIDR = (i << 6) | MACMIIDR_CR;
    if ((mii_read_phy(MII_PHYSID1) == (BOARD_PHY_ID >> 16)) &&
        (mii_read_phy(MII_PHYSID2) == (BOARD_PHY_ID & 0xFFF0))) {
      phyaddr = i << 11;
      return;
    }
  }
  /* Wrong or defective board.*/
  chSysHalt();
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level MAC initialization.
 *
 * @notapi
 */
void mac_lld_init(void) {
  unsigned i;

  macObjectInit(&ETHD1);

  /* Descriptor tables are initialized in linked mode, note that the first
     word is not initialized here but in mac_lld_start().*/
  for (i = 0; i < MAC_RECEIVE_BUFFERS; i++) {
    rd[i].rdes1 = STM32_RDES1_RCH | MAC_BUFFERS_SIZE;
    rd[i].rdes2 = (uint32_t)&rb[i * BUFFER_SLICE];
    rd[i].rdes3 = (uint32_t)&rb[((i + 1) % MAC_RECEIVE_BUFFERS) *
                                BUFFER_SLICE];
  }
  for (i = 0; i < MAC_TRANSMIT_BUFFERS; i++) {
    td[i].tdes1 = 0;
    td[i].tdes2 = (uint32_t)&tb[i * BUFFER_SLICE];
    td[i].tdes3 = (uint32_t)&tb[((i + 1) % MAC_TRANSMIT_BUFFERS) *
                                BUFFER_SLICE];
  }

  /* MAC clocks activation.*/
  rccEnableETH(FALSE);

  /* Reset of the MAC core.*/
  rccResetETH();

  /* Find PHY address.*/
  mii_find_phy();

#if defined(BOARD_PHY_RESET)
  /* PHY board-specific reset procedure.*/
  BOARD_PHY_RESET();
#else
  /* PHY soft reset procedure.*/
  mii_write_phy(MII_BMCR, BMCR_RESET);
  while (mii_read_phy(MII_BMCR) & BMCR_RESET)
    ;
#endif

  /* PHY in power down mode until the driver will be started.*/
  mii_write_phy(MII_BMCR, BMCR_PDOWN);

  /* MAC clocks stopped again.*/
  rccDisableETH(FALSE);
}

/**
 * @brief   Configures and activates the MAC peripheral.
 *
 * @param[in] macp      pointer to the @p MACDriver object
 *
 * @notapi
 */
void mac_lld_start(MACDriver *macp) {
  unsigned i;

  /* Resets the state of all descriptors.*/
  for (i = 0; i < MAC_RECEIVE_BUFFERS; i++)
    rd[i].rdes0 = STM32_RDES0_OWN;
  rxptr = (stm32_eth_rx_descriptor_t *)rd;
  for (i = 0; i < MAC_TRANSMIT_BUFFERS; i++)
    td[i].tdes0 = STM32_TDES0_TCH;
  txptr = (stm32_eth_tx_descriptor_t *)td;

  /* MAC clocks activation.*/
  rccEnableETH(FALSE);

  /* Descriptor chains pointers.*/
  ETH->DMARDLAR = (uint32_t)rd;
  ETH->DMATDLAR = (uint32_t)rd;

  /* MAC configuration:
       ETH_MACCR_TE     - Transmitter enable.
       ETH_MACCR_RE     - Receiver enable.
     Note that the complete setup of the MAC is performed when the link
     status is detected.*/
  ETH->MACCR = ETH_MACCR_TE | ETH_MACCR_TE;

  ETH->MACFFR    = 0;
  ETH->MACHTHR   = 0;
  ETH->MACHTLR   = 0;
  ETH->MACHTLR   = 0;
  ETH->MACFCR    = 0;
  ETH->MACVLANTR = 0;
}

/**
 * @brief   Deactivates the MAC peripheral.
 *
 * @param[in] macp      pointer to the @p MACDriver object
 *
 * @notapi
 */
void mac_lld_stop(MACDriver *macp) {

  /* MAC clocks stopped.*/
  rccDisableETH(FALSE);
}

/**
 * @brief   Returns a transmission descriptor.
 * @details One of the available transmission descriptors is locked and
 *          returned.
 *
 * @param[in] macp      pointer to the @p MACDriver object
 * @param[out] tdp      pointer to a @p MACTransmitDescriptor structure
 * @return              The operation status.
 * @retval RDY_OK       the descriptor has been obtained.
 * @retval RDY_TIMEOUT  descriptor not available.
 *
 * @notapi
 */
msg_t max_lld_get_transmit_descriptor(MACDriver *macp,
                                      MACTransmitDescriptor *tdp) {

  return RDY_OK;
}

/**
 * @brief   Writes to a transmit descriptor's stream.
 *
 * @param[in] tdp       pointer to a @p MACTransmitDescriptor structure
 * @param[in] buf       pointer to the buffer cointaining the data to be
 *                      written
 * @param[in] size      number of bytes to be written
 * @return              The number of bytes written into the descriptor's
 *                      stream, this value can be less than the amount
 *                      specified in the parameter @p size if the maximum
 *                      frame size is reached.
 *
 * @notapi
 */
size_t mac_lld_write_transmit_descriptor(MACTransmitDescriptor *tdp,
                                         uint8_t *buf,
                                         size_t size) {

  return 0;
}

/**
 * @brief   Releases a transmit descriptor and starts the transmission of the
 *          enqueued data as a single frame.
 *
 * @param[in] tdp       the pointer to the @p MACTransmitDescriptor structure
 *
 * @notapi
 */
void mac_lld_release_transmit_descriptor(MACTransmitDescriptor *tdp) {

}

/**
 * @brief   Returns a receive descriptor.
 *
 * @param[in] macp      pointer to the @p MACDriver object
 * @param[out] rdp      pointer to a @p MACReceiveDescriptor structure
 * @return              The operation status.
 * @retval RDY_OK       the descriptor has been obtained.
 * @retval RDY_TIMEOUT  descriptor not available.
 *
 * @notapi
 */
msg_t max_lld_get_receive_descriptor(MACDriver *macp,
                                     MACReceiveDescriptor *rdp) {

  return RDY_TIMEOUT;
}

/**
 * @brief   Reads from a receive descriptor's stream.
 *
 * @param[in] rdp       pointer to a @p MACReceiveDescriptor structure
 * @param[in] buf       pointer to the buffer that will receive the read data
 * @param[in] size      number of bytes to be read
 * @return              The number of bytes read from the descriptor's
 *                      stream, this value can be less than the amount
 *                      specified in the parameter @p size if there are
 *                      no more bytes to read.
 *
 * @notapi
 */
size_t mac_lld_read_receive_descriptor(MACReceiveDescriptor *rdp,
                                         uint8_t *buf,
                                         size_t size) {

  return 0;
}

/**
 * @brief   Releases a receive descriptor.
 * @details The descriptor and its buffer are made available for more incoming
 *          frames.
 *
 * @param[in] rdp       the pointer to the @p MACReceiveDescriptor structure
 *
 * @notapi
 */
void mac_lld_release_receive_descriptor(MACReceiveDescriptor *rdp) {

}

/**
 * @brief   Updates and returns the link status.
 *
 * @param[in] macp      pointer to the @p MACDriver object
 * @return              The link status.
 * @retval TRUE         if the link is active.
 * @retval FALSE        if the link is down.
 *
 * @notapi
 */
bool_t mac_lld_poll_link_status(MACDriver *macp) {

}

#endif /* HAL_USE_MAC */

/** @} */
