/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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

static void eth_init_descriptors(hal_eth_driver_c *ethp) {
  unsigned i;

  /* Descriptor tables are initialized in ring mode.*/
  ethp->rdp = &__eth_private_rd[0];
  ethp->tdp = &__eth_private_td[0];
  for (i = 0; i < STM32_ETH_RECEIVE_BUFFERS; i++) {
    __eth_private_rd[i].rdes0 = (uint32_t)__eth_shared_rb[i];
    __eth_private_rd[i].rdes1  = 0U;
    __eth_private_rd[i].rdes2  = 0U;
    __eth_private_rd[i].rdes3  = STM32_RDES3_OWN | STM32_RDES3_IOC | STM32_RDES3_BUF1V;
    __eth_private_rd[i].offset = 0U;
    __eth_private_rd[i].size   = 0U;
  }
  for (i = 0; i < STM32_ETH_TRANSMIT_BUFFERS; i++) {
    __eth_private_td[i].tdes0  = (uint32_t)__eth_shared_tb[i];
    __eth_private_td[i].tdes1  = 0U;
    __eth_private_td[i].tdes2  = 0U;
    __eth_private_td[i].tdes3  = 0U;
    __eth_private_td[i].offset = 0U;
    __eth_private_td[i].size   = 0U;
  }
}

static void eth_set_address(const uint8_t *p) {

  /* MAC address configuration, only a single address comparator is used,
     hash table not used.*/
  ETH->MACA0HR   = ((uint32_t)p[5] << 8) |
                   ((uint32_t)p[4] << 0);
  ETH->MACA0LR   = ((uint32_t)p[3] << 24) |
                   ((uint32_t)p[2] << 16) |
                   ((uint32_t)p[1] << 8) |
                   ((uint32_t)p[0] << 0);
  ETH->MACA1HR   = 0;
  ETH->MACA1LR   = 0;
  ETH->MACA2HR   = 0;
  ETH->MACA2LR   = 0;
  ETH->MACA3HR   = 0;
  ETH->MACA3LR   = 0;
  ETH->MACHT0R   = 0;
  ETH->MACHT1R   = 0;
}

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

#if STM32_ETH_ETH1_CHANGE_PHY_STATE
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

  /* Resetting the state of all descriptors.*/
  eth_init_descriptors(ethp);

  /* ETH clocks activation and commanded reset procedure.*/
  rccResetETH();
  rccEnableETH(true);

  /* ISR vector enabled.*/
  nvicEnableVector(STM32_ETH_NUMBER, STM32_IRQ_ETH1_PRIORITY);

#if STM32_ETH_ETH1_CHANGE_PHY_STATE
  /* PHY in power up mode.*/
  mii_write(ethp, MII_BMCR, mii_read(ethp, MII_BMCR) & ~BMCR_PDOWN);
#endif

  ETH->DMAMR |= ETH_DMAMR_SWR;
  while (ETH->DMAMR & ETH_DMAMR_SWR) {
    /* Waiting for completion.*/
    /* TODO timeout.*/
  }

  /* MAC configuration.*/
  ETH->MACCR   = ETH_MACCR_DO;
  ETH->MACPFR  = 0U;
  ETH->MACTFCR = 0U;
  ETH->MACRFCR = 0U;
  ETH->MACVTR  = 0U;

  /* MAC address setup.*/
  if (__eth_getfield(ethp, mac_address) == NULL) {
    eth_set_address(default_mac_address);
  }
  else {
    eth_set_address(__eth_getfield(ethp, mac_address));
  }


  /* MMC configuration:
     Disable all interrupts.*/
  ETH->MMCTIMR   = (1<<27) | (1<<26) | (1<<21) | (1<<15) | (1<<14);
  ETH->MMCRIMR   = (1<<27) | (1<<26) | (1<<17) | (1<<6)  | (1<<5);

  /* DMA general settings.*/
  ETH->DMASBMR   = ETH_DMASBMR_AAL;
  ETH->DMACCR    = ETH_DMACCR_DSL_0BIT;
  ETH->DMAMR     = ETH_DMAMR_INTM_0 | ETH_DMAMR_PR_8_1 | ETH_DMAMR_TXPR;  /* TX:RX 8:1 */

  /* DMA configuration:
     Descriptor rings pointers.*/
  ETH->DMACTDLAR = (uint32_t)&__eth_private_td[0];
  ETH->DMACTDRLR = STM32_ETH_TRANSMIT_BUFFERS - 1;
  ETH->DMACRDLAR = (uint32_t)&__eth_private_rd[0];
  ETH->DMACRDRLR = STM32_ETH_RECEIVE_BUFFERS - 1;

  /* Enabling required interrupt sources.*/
  ETH->DMACSR    = ETH_DMACSR_NIS;
  ETH->DMACIER   = ETH_DMACIER_NIE | ETH_DMACIER_RIE | ETH_DMACIER_TIE;

  /* Check because errata on some devices. There should be no need to
     disable flushing because the TXFIFO should be empty on macStart().*/
#if !defined(STM32_ETH_DISABLE_TX_FLUSH)
  /* Transmit FIFO flush.*/
  ETH->MTLTQOMR  = ETH_MTLTQOMR_FTQ;
  while (ETH->MTLTQOMR & ETH_MTLTQOMR_FTQ) {
    /* Waiting for completion.*/
    /* TODO timeout.*/
  }
#endif

  /* DMA final configuration and start.*/
  ETH->MTLRQOMR  = ETH_MTLRQOMR_DISTCPEF | ETH_MTLRQOMR_RSF;
  ETH->MTLTQOMR  = ETH_MTLTQOMR_TSF;
  ETH->DMACTCR   = ETH_DMACTCR_ST | ETH_DMACTCR_TPBL_1PBL;
  ETH->DMACRCR   = ETH_DMACRCR_SR | ETH_DMACRCR_RPBL_1PBL |
                   (STM32_ETH_BUFFERS_SIZE << ETH_DMACRCR_RBSZ_Pos); /* TODO error in HAL */

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

#if STM32_ETH_ETH1_CHANGE_PHY_STATE
  /* PHY in power down mode until the driver will be restarted.*/
  mii_write(ethp, MII_BMCR, mii_read(ethp, MII_BMCR) | BMCR_PDOWN);
#endif

  /* MAC and DMA stopped.*/
  ETH->MACCR    = 0U;
  ETH->MTLRQOMR = 0U;
  ETH->DMACIER  = 0U;
  ETH->DMACSR   = ETH->DMACSR;

  /* MAC clocks stopped.*/
  rccDisableETH();

  /* ISR vector disabled.*/
  nvicDisableVector(STM32_ETH_NUMBER);
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
eth_receive_handle_t eth_lld_get_receive_handle(hal_eth_driver_c *ethp) {

  /* Iterates through received frames until a valid one is found, invalid
     frames are discarded.*/
  while ((ethp->rdp->rdes3 & STM32_RDES3_OWN) == 0U) {

    /* Is it a valid frame?*/
    if (true &&
#if STM32_ETH_IP_CHECKSUM_OFFLOAD
        ((ethp->rdp->rdes1 & (STM32_RDES1_IPHE | STM32_RDES1_IPCE)) == 0U) &&
#endif
        ((ethp->rdp->rdes2 & STM32_RDES2_DAF) == 0U) &&
        ((ethp->rdp->rdes3 & STM32_RDES3_ES) == 0U) &&
        ((ethp->rdp->rdes3 & STM32_RDES3_FD) != 0U) &&
        ((ethp->rdp->rdes3 & STM32_RDES3_LD)) != 0U) {

      /* Found a valid one.*/
      ethp->rdp->offset = 0U;
      ethp->rdp->size   = (ethp->rdp->rdes3 & STM32_RDES3_PL_MASK) - 2U; /* Lose CRC.*/

      /* Reposition in ring.*/
      ethp->rdp++;
      if (ethp->rdp >= &__eth_private_rd[STM32_ETH_RECEIVE_BUFFERS]) {
        ethp->rdp = &__eth_private_rd[0];
      }

      return (eth_receive_handle_t)ethp->rdp;
    }

    /* Invalid frame found, purging.*/
    ethp->rdp->rdes3 = STM32_RDES3_OWN | STM32_RDES3_IOC | STM32_RDES3_BUF1V;

    /* On next descriptor.*/
    ethp->rdp++;
    if (ethp->rdp >= &__eth_private_rd[STM32_ETH_RECEIVE_BUFFERS]) {
      ethp->rdp = &__eth_private_rd[0];
    }
  }

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
eth_transmit_handle_t eth_lld_get_transmit_handle(hal_eth_driver_c *ethp) {
  stm32_eth_tx_descriptor_t *tdp = ethp->tdp;

  if (!ethp->link_up) {
    return NULL;
  }

  /* Ensure that descriptor isn't owned by the Ethernet DMA or locked by
     another thread.*/
  if (((tdp->tdes3 & STM32_TDES3_OWN) != 0U) ||
      (tdp->tdes1 != 0U)) {
    return NULL;
  }

  /* Marks the current descriptor as locked.*/
  tdp->tdes1 = STM32_TDES1_LOCKED;

  /* Set the buffer size and configuration.*/
  tdp->offset = 0U;
  tdp->size   = STM32_ETH_BUFFERS_SIZE;

  /* Next TX descriptor to use.*/
  ethp->tdp++;
  if (ethp->tdp >= &__eth_private_td[STM32_ETH_TRANSMIT_BUFFERS]) {
    ethp->tdp = &__eth_private_td[0];
  }

  return (eth_transmit_handle_t)tdp;
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
                                    eth_receive_handle_t rxh) {
  stm32_eth_rx_descriptor_t *rdp = (stm32_eth_rx_descriptor_t *)rxh;

  (void)ethp;

  osalDbgAssert((rdp->rdes3 & STM32_RDES3_OWN) == 0U,
                "attempt to release descriptor already owned by DMA");

  /* Give buffer back to the Ethernet DMA.*/
  rdp->rdes3 = STM32_RDES3_OWN | STM32_RDES3_IOC | STM32_RDES3_BUF1V;

  /* Triggers transmission if the DMA was suspended.*/
  ETH->DMACRDTPR = 0U;
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
                                     eth_transmit_handle_t txh) {
  stm32_eth_tx_descriptor_t *tdp = (stm32_eth_tx_descriptor_t *)txh;

  (void)ethp;

  osalDbgAssert((tdp->tdes3 & STM32_TDES3_OWN) == 0U,
              "attempt to release descriptor already owned by DMA");

  /* Unlocks the descriptor and returns it to the DMA engine.*/
  tdp->tdes2 = STM32_TDES2_IOC | tdp->offset;
  tdp->tdes1 = 0U;
#if STM32_ETH_IP_CHECKSUM_OFFLOAD
  tdp->tdes3 = STM32_TDES3_CIC(STM32_MAC_IP_CHECKSUM_OFFLOAD) |
               STM32_TDES3_LD | STM32_TDES3_FD |
               STM32_TDES3_OWN;
#else
  tdp->tdes3 = STM32_TDES3_LD | STM32_TDES3_FD | STM32_TDES3_OWN;
#endif

  ETH->DMACTDTPR = 0U;
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
                                   eth_receive_handle_t rxh,
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
                                     eth_transmit_handle_t txh,
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
                                          eth_receive_handle_t rxh,
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
                                     eth_transmit_handle_t txh,
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
