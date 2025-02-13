/*
    ChibiOS - Copyright (C) 2006..2025 Giovanni Di Sirio

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

/**
 * @file    ETHv2/hal_eth_lld.c
 * @brief   STM32 low level ETH driver code.
 *
 * @addtogroup HAL_ETH
 * @{
 */

#include <string.h>

#include "hal.h"

#if HAL_USE_ETH || defined(__DOXYGEN__)

#include "hal_mii.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/* Frame buffers size in words.*/
#define BUFFER_SIZE ((((STM32_ETH_BUFFERS_SIZE - 1) | 3) + 1) / 4)

/* Fixing inconsistencies in ST headers.*/
#if !defined(ETH_MACMDIOAR_CR_Div124) && defined(ETH_MACMDIOAR_CR_DIV124)
#define ETH_MACMDIOAR_CR_Div124     ETH_MACMDIOAR_CR_DIV124
#endif
#if !defined(ETH_MACMDIOAR_CR_Div102) && defined(ETH_MACMDIOAR_CR_DIV102)
#define ETH_MACMDIOAR_CR_Div102     ETH_MACMDIOAR_CR_DIV102
#endif
#if !defined(ETH_MACMDIOAR_CR_Div62) && defined(ETH_MACMDIOAR_CR_DIV62)
#define ETH_MACMDIOAR_CR_Div62      ETH_MACMDIOAR_CR_DIV62
#endif
#if !defined(ETH_MACMDIOAR_CR_Div42) && defined(ETH_MACMDIOAR_CR_DIV42)
#define ETH_MACMDIOAR_CR_Div42      ETH_MACMDIOAR_CR_DIV42
#endif
#if !defined(ETH_MACMDIOAR_CR_Div26) && defined(ETH_MACMDIOAR_CR_DIV26)
#define ETH_MACMDIOAR_CR_Div26      ETH_MACMDIOAR_CR_DIV26
#endif
#if !defined(ETH_MACMDIOAR_CR_Div16) && defined(ETH_MACMDIOAR_CR_DIV16)
#define ETH_MACMDIOAR_CR_Div16      ETH_MACMDIOAR_CR_DIV16
#endif

/* MII divider optimal value.*/
#if (STM32_HCLK > 300000000)
#error "STM32_HCLK above maximum frequency for ETH operations (300MHz)"
#elif (STM32_HCLK >= 250000000)
#define MACMDIODR_CR                ETH_MACMDIOAR_CR_Div124
#elif (STM32_HCLK >= 150000000)
#define MACMDIODR_CR                ETH_MACMDIOAR_CR_Div102
#elif (STM32_HCLK >= 100000000)
#define MACMDIODR_CR                ETH_MACMDIOAR_CR_Div62
#elif (STM32_HCLK >= 60000000)
#define MACMDIODR_CR                ETH_MACMDIOAR_CR_Div42
#elif (STM32_HCLK >= 35000000)
#define MACMDIODR_CR                ETH_MACMDIOAR_CR_Div26
#elif (STM32_HCLK >= 20000000)
#define MACMDIODR_CR                ETH_MACMDIOAR_CR_Div16
#else
#error "STM32_HCLK below minimum frequency for ETH operations (20MHz)"
#endif

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   Ethernet driver 1.
 */
hal_eth_driver_c ETHD1;

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

static const uint8_t default_mac_address[] = STM32_ETH_ETH1_DEFAULT_MAC_ADDRESS;

/* Integration note, the linker script must place variables prefixed with
   __eth_private_ into a cache-coherent memory (only accessible by privileged
   threads if required by sandboxes).*/
static stm32_eth_rx_descriptor_t __eth_private_rd[STM32_ETH_RECEIVE_BUFFERS];
static stm32_eth_tx_descriptor_t __eth_private_td[STM32_ETH_TRANSMIT_BUFFERS];

/* Integration note, the linker script must place variables prefixed with
   __eth_shared_ into a cache-coherent memory (also accessible by non-privileged
   threads if required by sandboxes).*/
static uint32_t __eth_shared_rb[STM32_ETH_RECEIVE_BUFFERS][BUFFER_SIZE];
static uint32_t __eth_shared_tb[STM32_ETH_TRANSMIT_BUFFERS][BUFFER_SIZE];

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void eth_init_descriptors(void) {
  unsigned i;

  /* Descriptor tables are initialized in ring mode.*/
  for (i = 0; i < STM32_ETH_RECEIVE_BUFFERS; i++) {
    __eth_private_rd[i].rdes0 = (uint32_t)__eth_shared_rb[i];
    __eth_private_rd[i].rdes1 = 0U;
    __eth_private_rd[i].rdes2 = 0U;
    __eth_private_rd[i].rdes3 = STM32_RDES3_OWN | STM32_RDES3_IOC | STM32_RDES3_BUF1V;
    __eth_private_rd[i].rdes4 = 0U;
    __eth_private_rd[i].rdes5 = 0U;
  }
  for (i = 0; i < STM32_ETH_TRANSMIT_BUFFERS; i++) {
    __eth_private_td[i].tdes0 = (uint32_t)__eth_shared_tb[i];
    __eth_private_td[i].tdes1 = 0U;
    __eth_private_td[i].tdes2 = 0U;
    __eth_private_td[i].tdes3 = 0U;
    __eth_private_td[i].tdes4 = 0U;
    __eth_private_td[i].tdes5 = 0U;
  }
}

void mii_write(hal_eth_driver_c *ethp, uint32_t reg, uint32_t value) {

  ETH->MACMDIODR = value;
  ETH->MACMDIOAR = ethp->phyaddr | (reg << ETH_MACMDIOAR_RDA_Pos) |
                   MACMDIODR_CR | ETH_MACMDIOAR_MOC_WR | ETH_MACMDIOAR_MB;
  while ((ETH->MACMDIOAR & ETH_MACMDIOAR_MB) != 0U) {
  }
}

uint32_t mii_read(hal_eth_driver_c *ethp, uint32_t reg) {

  ETH->MACMDIOAR = ethp->phyaddr | (reg << ETH_MACMDIOAR_RDA_Pos) |
                   MACMDIODR_CR | ETH_MACMDIOAR_MOC_RD | ETH_MACMDIOAR_MB;
  while ((ETH->MACMDIOAR & ETH_MACMDIOAR_MB) != 0U) {
  }

  return ETH->MACMDIODR;
}

#if !defined(BOARD_PHY_ADDRESS)
static void mii_find_phy(hal_eth_driver_c *ethp) {
  uint32_t i;

#if STM32_ETH_PHY_TIMEOUT > 0
  unsigned n = STM32_ETH_PHY_TIMEOUT;
 do {
#endif
    for (i = 0U; i <= 31U; i++) {
      ethp->phyaddr = i << ETH_MACMDIOAR_PA_Pos;
      ETH->MACMDIOAR = (i << ETH_MACMDIOAR_RDA_Pos) | MACMDIODR_CR;
      ETH->MACMDIODR = (i << ETH_MACMDIODR_RA_Pos) | MACMDIODR_CR;
      if ((mii_read(ethp, MII_PHYSID1) == (BOARD_PHY_ID >> 16U)) &&
          ((mii_read(ethp, MII_PHYSID2) & 0xFFF0U) == (BOARD_PHY_ID & 0xFFF0U))) {
        return;
      }
    }
#if STM32_ETH_PHY_TIMEOUT > 0
    n--;
  } while (n > 0U);
#endif
  /* Wrong or defective board.*/
  osalSysHalt("ETH failure");
}
#endif

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief       ETH Low Level Driver initialization.
 *
 * @init
 */
void eth_lld_init(void) {
  hal_eth_driver_c *ethp = &ETHD1;

  ethObjectInit(ethp);
  ethp->link_up = false;

  /* Selection of the RMII or MII mode based on info exported by board.h.*/
#if defined(STM32H7XX)
  {
    uint32_t pmcr = SYSCFG->PMCR & ~SBS_PMCR_ETH_SEL_PHY_Msk;
#if defined(BOARD_PHY_RMII)
  pmcr |= SYSCFG_PMCR_EPIS_SEL_2;
#endif
  SYSCFG->PMCR = pmcr;
}

#elif defined(STM32H5XX)
  {
    uint32_t pmcr = SBS->PMCR & ~SBS_PMCR_ETH_SEL_PHY_Msk;
#if defined(BOARD_PHY_RMII)
    pmcr |= SBS_PMCR_ETH_SEL_PHY_2;
#endif
    SBS->PMCR = pmcr;
  }

#else
#error "unsupported STM32 platform for ETHv2 driver"
#endif

  /* Reset of the MAC core then enabling clocks.*/
  rccResetETH();
  rccEnableETH(true);

  /* PHY address setup.*/
#if defined(BOARD_PHY_ADDRESS)
  ethp->phyaddr = BOARD_PHY_ADDRESS << 11;
#else
  mii_find_phy(ethp);
#endif

#if defined(BOARD_PHY_RESET)
  /* PHY board-specific reset procedure.*/
  BOARD_PHY_RESET();
#else
  /* PHY soft reset procedure.*/
  mii_write(ethp, MII_BMCR, BMCR_RESET);
#if defined(BOARD_PHY_RESET_DELAY)
  osalSysPolledDelayX(BOARD_PHY_RESET_DELAY);
#endif
  while (mii_read(ethp, MII_BMCR) & BMCR_RESET)
    ;
#endif

#if STM32_MAC_ETH1_CHANGE_PHY_STATE
  /* PHY in power down mode until the driver will be started.*/
  mii_write(ethp, MII_BMCR, mii_read(ethp, MII_BMCR) | BMCR_PDOWN);
#endif

  /* MAC clocks stopped again.*/
  rccDisableETH();
}

/**
 * @brief   Activates the ETH peripheral using the default configuration.
 *
 * @param[in,out] ethp          Pointer to a @p hal_eth_driver_c instance.
 *
 * @notapi
 */
msg_t eth_lld_start(hal_eth_driver_c *ethp) {

  (void)ethp;

  return HAL_RET_SUCCESS;
}

/**
 * @brief   Stops the ETH peripheral.
 *
 * @param[in,out] ethp          Pointer to a @p hal_eth_driver_c instance.
 *
 * @notapi
 */
void eth_lld_stop(hal_eth_driver_c *ethp) {

  (void)ethp;
}

/**
 * @brief   ETH Low Level Driver direct configuration.
 *
 * @param[in,out] ethp          Pointer to a @p hal_eth_driver_c instance.
 * @param[in] config            Pointer to the @p hal_eth_config_t structure.
 * @return                      A pointer to the current configuration
 *                              structure.
 * @retval NULL                 If the configuration failed.
 *
 * @notapi
 */
const hal_eth_config_t *eth_lld_setcfg(hal_eth_driver_c *ethp,
                                       const hal_eth_config_t *config) {

  (void)ethp;
  (void)config;

  return NULL;
}

/**
 * @brief   ETH Low Level Driver configuration selection.
 *
 * @param[in,out] ethp          Pointer to a @p hal_eth_driver_c instance.
 * @param[in] config            Pointer to the @p hal_eth_config_t structure.
 * @return                      A pointer to the current configuration
 *                              structure.
 * @retval NULL                 If the configuration failed.
 *
 * @notapi
 */
const hal_eth_config_t *eth_lld_selcfg(hal_eth_driver_c *ethp,
                                       unsigned cfgnum) {

  (void)ethp;
  (void)cfgnum;

  return NULL;
}

/**
 * @brief       Queries for a received frame handle.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @return                      The receive handle.
 * @retval NULL                 If a received frame is not available.
 *
 * @notapi
 */
etc_receive_handle_t eth_lld_get_receive_handle(hal_eth_driver_c *ethp) {

  (void)ethp;

  return NULL;
}

/**
 * @brief       Queries for a transmit frame handle.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @return                      The transmit handle.
 * @retval NULL                 If an empty transmit frame is not available.
 *
 * @notapi
 */
etc_transmit_handle_t eth_lld_get_transmit_handle(hal_eth_driver_c *ethp) {

  (void)ethp;

  return NULL;
}

/**
 * @brief       Releases a received frame.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @param[in]     rxh           Receive handle.
 *
 * @notapi
 */
void eth_lld_release_receive_handle(hal_eth_driver_c *ethp,
                                    etc_receive_handle_t rxh) {

  (void)ethp;
  (void)rxh;
}

/**
 * @brief       Releases and transmits a frame.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @param[in]     txh           Transmi] handle.
 *
 * @notapi
 */
void eth_lld_release_transmit_handle(hal_eth_driver_c *ethp,
                                     etc_transmit_handle_t txh) {

  (void)ethp;
  (void)txh;
}

/**
 * @brief       Reads data sequentially from a received frame.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @param[in]     rxh           Receive handle.
 * @param[out]    bp            Received data buffer pointer.
 * @param[in]     n             Number of bytes to read.
 * @return                      The number of bytes read from the handle
 *                              buffer, this value can be less than the amount
 *                              specified in the parameter @p size if there are
 *                              no more bytes to read.
 *
 * @notapi
 */
size_t eth_lld_read_receive_handle(hal_eth_driver_c *ethp,
                                   etc_receive_handle_t rxh,
                                   uint8_t *bp, size_t n) {

  (void)ethp;
  (void)rxh;
  (void)bp;
  (void)n;

  return 0;
}

/**
 * @brief       Reads data sequentially from a received frame.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @param[in]     txh           Transmit handle.
 * @param[in]     bp            Transmit data buffer pointer.
 * @param[in]     n             Number of bytes to write.
 * @return                      The number of bytes written into the handle
 *                              buffer this value can be less than the amount
 *                              specified in the parameter @p size if the
 *                              maximum frame size is reached.
 *
 * @notapi
 */
size_t eth_lld_write_transmit_handle(hal_eth_driver_c *ethp,
                                     etc_transmit_handle_t txh,
                                     const uint8_t *bp, size_t n) {

  (void)ethp;
  (void)txh;
  (void)bp;
  (void)n;

  return 0;
}

/**
 * @brief       Direct access to the receive handle buffer.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @param[in]     rxh           Receive handle.
 * @param[out]    sizep         Size of the received frame.
 * @return                      Pointer to the received frame buffer or @p NULL
 *                              if the driver does not support memory-mapped
 *                              direct access.
 *
 * @notapi
 */
const uint8_t *eth_lld_get_receive_buffer(hal_eth_driver_c *ethp,
                                          etc_receive_handle_t rxh,
                                          size_t *sizep) {

  (void)ethp;
  (void)rxh;
  (void)sizep;

  return 0;
}

/**
 * @brief       Direct access to the transmit handle buffer.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @param[in]     txh           Transmit handle.
 * @param[out]    sizep         Maximum size of the transmit buffer.
 * @return                      Pointer to the transmit frame buffer or @p NULL
 *                              if the driver does not support memory-mapped
 *                              direct access.
 *
 * @notapi
 */
uint8_t *eth_lld_get_transmit_buffer(hal_eth_driver_c *ethp,
                                     etc_transmit_handle_t txh,
                                     size_t *sizep) {

  (void)ethp;
  (void)txh;
  (void)sizep;

  return NULL;
}

/**
 * @brief       Returns the link status.
 *
 * @param[in,out] ip            Pointer to a @p hal_eth_driver_c instance.
 * @return                      The link status,
 * @retval false                If the link is active.
 * @retval true                 If the link is down.
 *
 * @notapi
 */
bool eth_lld_poll_link_status(hal_eth_driver_c *ethp) {

  (void)ethp;

  return true;
}

#endif /* HAL_USE_ETH */

/** @} */
