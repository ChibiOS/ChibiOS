/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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
 * @file    hal_spi_lld.c
 * @brief   SAMA SPI subsystem low level driver source.
 *
 * @addtogroup SPI
 * @{
 */

#include "hal.h"

#if HAL_USE_SPI || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local macros.                                                      */
/*===========================================================================*/

/**
 * @brief   Enable write protection on SPI registers block.
 *
 * @param[in] spip    pointer to a SPI register block
 *
 * @notapi
 */
#define spiEnableWP(spip) {                                                  \
  spip->SPI_WPMR = SPI_WPMR_WPKEY_PASSWD | SPI_WPMR_WPEN;                    \
}

/**
 * @brief   Disable write protection on SPI registers block.
 *
 * @param[in] spip    pointer to a SPI register block
 *
 * @notapi
 */
#define spiDisableWP(spip) {                                                 \
  spip->SPI_WPMR = SPI_WPMR_WPKEY_PASSWD;                                    \
}


/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   SPI0 driver identifier.
 */
#if SAMA_SPI_USE_SPI0 || defined(__DOXYGEN__)
SPIDriver SPID0;
#endif

/**
 * @brief   SPI1 driver identifier.
 */
#if SAMA_SPI_USE_SPI1 || defined(__DOXYGEN__)
SPIDriver SPID1;
#endif

/**
 * @brief   SPI FLEXCOM0 driver identifier.
 */
#if SAMA_SPI_USE_FLEXCOM0 || defined(__DOXYGEN__)
SPIDriver FSPID0;
#endif

/**
 * @brief   SPI FLEXCOM1 driver identifier.
 */
#if SAMA_SPI_USE_FLEXCOM1 || defined(__DOXYGEN__)
SPIDriver FSPID1;
#endif

/**
 * @brief   SPI FLEXCOM2 driver identifier.
 */
#if SAMA_SPI_USE_FLEXCOM2 || defined(__DOXYGEN__)
SPIDriver FSPID2;
#endif

/**
 * @brief   SPI FLEXCOM3 driver identifier.
 */
#if SAMA_SPI_USE_FLEXCOM3 || defined(__DOXYGEN__)
SPIDriver FSPID3;
#endif

/**
 * @brief   SPI FLEXCOM4 driver identifier.
 */
#if SAMA_SPI_USE_FLEXCOM4 || defined(__DOXYGEN__)
SPIDriver FSPID4;
#endif

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

static const uint8_t dummytx = 0xFFU;
static uint8_t dummyrx;

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Shared end-of-rx service routine.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] flags     pre-shifted content of the ISR register
 */
static void spi_lld_serve_rx_interrupt(SPIDriver *spip, uint32_t flags) {

  /* DMA errors handling.*/
#if defined(SAMA_SPI_DMA_ERROR_HOOK)
  if ((flags & (XDMAC_CIS_RBEIS | XDMAC_CIS_ROIS)) != 0) {
    SAMA_SPI_DMA_ERROR_HOOK(spip);
  }
#else
  (void)flags;
#endif

  /* Stop everything.*/
  dmaChannelDisable(spip->dmatx);
  dmaChannelDisable(spip->dmarx);

  /* Portable SPI ISR code defined in the high level driver, note, it is
     a macro.*/
  _spi_isr_code(spip);
}

/**
 * @brief   Shared end-of-tx service routine.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] flags     pre-shifted content of the ISR register
 */
static void spi_lld_serve_tx_interrupt(SPIDriver *spip, uint32_t flags) {

  /* DMA errors handling.*/

#if defined(SAMA_SPI_DMA_ERROR_HOOK)
  (void)spip;
  if ((flags & (XDMAC_CIS_WBEIS | XDMAC_CIS_ROIS)) != 0) {
    SAMA_SPI_DMA_ERROR_HOOK(spip);
  }
#else
  (void)spip;
  (void)flags;
#endif
}
/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level SPI driver initialization.
 *
 * @notapi
 */
void spi_lld_init(void) {

#if SAMA_SPI_USE_SPI0
#if SAMA_HAL_IS_SECURE
  mtxConfigPeriphSecurity(MATRIX1, ID_SPI0, SECURE_PER);
#endif /* SAMA_HAL_IS_SECURE */
  /* Driver initialization.*/
  spiObjectInit(&SPID0);
  SPID0.spi       = SPI0;
  SPID0.dmarx     = NULL;
  SPID0.dmatx     = NULL;
  SPID0.rxdmamode = XDMAC_CC_TYPE_PER_TRAN |
                    XDMAC_CC_MBSIZE_SIXTEEN |
                    XDMAC_CC_DSYNC_PER2MEM |
                    XDMAC_CC_PROT_SEC |
                    XDMAC_CC_CSIZE_CHK_1 |
                    XDMAC_CC_DWIDTH_BYTE |
                    XDMAC_CC_SIF_AHB_IF1 |
                    XDMAC_CC_DIF_AHB_IF0 |
                    XDMAC_CC_SAM_FIXED_AM |
                    XDMAC_CC_DAM_INCREMENTED_AM |
                    XDMAC_CC_PERID(PERID_SPI0_RX);
  SPID0.txdmamode = XDMAC_CC_TYPE_PER_TRAN |
                    XDMAC_CC_MBSIZE_SIXTEEN |
                    XDMAC_CC_DSYNC_MEM2PER |
                    XDMAC_CC_PROT_SEC |
                    XDMAC_CC_CSIZE_CHK_1 |
                    XDMAC_CC_DWIDTH_BYTE |
                    XDMAC_CC_SIF_AHB_IF0 |
                    XDMAC_CC_DIF_AHB_IF1 |
                    XDMAC_CC_SAM_INCREMENTED_AM |
                    XDMAC_CC_DAM_FIXED_AM |
                    XDMAC_CC_PERID(PERID_SPI0_TX);
#endif /* SAMA_SPI_USE_SPI0 */

#if SAMA_SPI_USE_SPI1
#if SAMA_HAL_IS_SECURE
  mtxConfigPeriphSecurity(MATRIX1, ID_SPI1, SECURE_PER);
#endif /* SAMA_HAL_IS_SECURE */
  /* Driver initialization.*/
  spiObjectInit(&SPID1);
  SPID1.spi       = SPI1;
  SPID1.dmarx     = NULL;
  SPID1.dmatx     = NULL;
  SPID1.rxdmamode = XDMAC_CC_TYPE_PER_TRAN |
                    XDMAC_CC_MBSIZE_SIXTEEN |
                    XDMAC_CC_DSYNC_PER2MEM |
                    XDMAC_CC_PROT_SEC |
                    XDMAC_CC_CSIZE_CHK_1 |
                    XDMAC_CC_DWIDTH_BYTE |
                    XDMAC_CC_SIF_AHB_IF1 |
                    XDMAC_CC_DIF_AHB_IF0 |
                    XDMAC_CC_SAM_FIXED_AM |
                    XDMAC_CC_DAM_INCREMENTED_AM |
                    XDMAC_CC_PERID(PERID_SPI1_RX);
  SPID1.txdmamode = XDMAC_CC_TYPE_PER_TRAN |
                    XDMAC_CC_MBSIZE_SIXTEEN |
                    XDMAC_CC_DSYNC_MEM2PER |
                    XDMAC_CC_PROT_SEC |
                    XDMAC_CC_CSIZE_CHK_1 |
                    XDMAC_CC_DWIDTH_BYTE |
                    XDMAC_CC_SIF_AHB_IF0 |
                    XDMAC_CC_DIF_AHB_IF1 |
                    XDMAC_CC_SAM_INCREMENTED_AM |
                    XDMAC_CC_DAM_FIXED_AM |
                    XDMAC_CC_PERID(PERID_SPI1_TX);
#endif /* SAMA_SPI_USE_SPI1 */

#if SAMA_SPI_USE_FLEXCOM0
#if SAMA_HAL_IS_SECURE
  mtxConfigPeriphSecurity(MATRIX1, ID_FLEXCOM0, SECURE_PER);
#endif /* SAMA_HAL_IS_SECURE */
  /* Driver initialization.*/
  spiObjectInit(&FSPID0);
  FSPID0.spi       = FCOMSPI0;
  FSPID0.flexcom   = FLEXCOM0;
  FSPID0.dmarx     = NULL;
  FSPID0.dmatx     = NULL;
  FSPID0.rxdmamode = XDMAC_CC_TYPE_PER_TRAN |
                     XDMAC_CC_MBSIZE_SIXTEEN |
                     XDMAC_CC_DSYNC_PER2MEM |
                     XDMAC_CC_PROT_SEC |
                     XDMAC_CC_CSIZE_CHK_1 |
                     XDMAC_CC_DWIDTH_BYTE |
                     XDMAC_CC_SIF_AHB_IF1 |
                     XDMAC_CC_DIF_AHB_IF0 |
                     XDMAC_CC_SAM_FIXED_AM |
                     XDMAC_CC_DAM_INCREMENTED_AM |
                     XDMAC_CC_PERID(PERID_FLEXCOM0_RX);
  FSPID0.txdmamode = XDMAC_CC_TYPE_PER_TRAN |
                     XDMAC_CC_MBSIZE_SIXTEEN |
                     XDMAC_CC_DSYNC_MEM2PER |
                     XDMAC_CC_PROT_SEC |
                     XDMAC_CC_CSIZE_CHK_1 |
                     XDMAC_CC_DWIDTH_BYTE |
                     XDMAC_CC_SIF_AHB_IF0 |
                     XDMAC_CC_DIF_AHB_IF1 |
                     XDMAC_CC_SAM_INCREMENTED_AM |
                     XDMAC_CC_DAM_FIXED_AM |
                     XDMAC_CC_PERID(PERID_FLEXCOM0_TX);
#endif /* SAMA_SPI_USE_FLEXCOM0 */

#if SAMA_SPI_USE_FLEXCOM1
#if SAMA_HAL_IS_SECURE
  mtxConfigPeriphSecurity(MATRIX1, ID_FLEXCOM1, SECURE_PER);
#endif /* SAMA_HAL_IS_SECURE */
  /* Driver initialization.*/
  spiObjectInit(&FSPID1);
  FSPID1.spi       = FCOMSPI1;
  FSPID1.flexcom   = FLEXCOM1;
  FSPID1.dmarx     = NULL;
  FSPID1.dmatx     = NULL;
  FSPID1.rxdmamode = XDMAC_CC_TYPE_PER_TRAN |
                     XDMAC_CC_MBSIZE_SIXTEEN |
                     XDMAC_CC_DSYNC_PER2MEM |
                     XDMAC_CC_PROT_SEC |
                     XDMAC_CC_CSIZE_CHK_1 |
                     XDMAC_CC_DWIDTH_BYTE |
                     XDMAC_CC_SIF_AHB_IF1 |
                     XDMAC_CC_DIF_AHB_IF0 |
                     XDMAC_CC_SAM_FIXED_AM |
                     XDMAC_CC_DAM_INCREMENTED_AM |
                     XDMAC_CC_PERID(PERID_FLEXCOM1_RX);
  FSPID1.txdmamode = XDMAC_CC_TYPE_PER_TRAN |
                     XDMAC_CC_MBSIZE_SIXTEEN |
                     XDMAC_CC_DSYNC_MEM2PER |
                     XDMAC_CC_PROT_SEC |
                     XDMAC_CC_CSIZE_CHK_1 |
                     XDMAC_CC_DWIDTH_BYTE |
                     XDMAC_CC_SIF_AHB_IF0 |
                     XDMAC_CC_DIF_AHB_IF1 |
                     XDMAC_CC_SAM_INCREMENTED_AM |
                     XDMAC_CC_DAM_FIXED_AM |
                     XDMAC_CC_PERID(PERID_FLEXCOM1_TX);
#endif /* SAMA_SPI_USE_FLEXCOM1 */

#if SAMA_SPI_USE_FLEXCOM2
#if SAMA_HAL_IS_SECURE
  mtxConfigPeriphSecurity(MATRIX1, ID_FLEXCOM2, SECURE_PER);
#endif /* SAMA_HAL_IS_SECURE */
  /* Driver initialization.*/
  spiObjectInit(&FSPID2);
  FSPID2.spi       = FCOMSPI2;
  FSPID2.flexcom   = FLEXCOM2;
  FSPID2.dmarx     = NULL;
  FSPID2.dmatx     = NULL;
  FSPID2.rxdmamode = XDMAC_CC_TYPE_PER_TRAN |
                     XDMAC_CC_MBSIZE_SIXTEEN |
                     XDMAC_CC_DSYNC_PER2MEM |
                     XDMAC_CC_PROT_SEC |
                     XDMAC_CC_CSIZE_CHK_1 |
                     XDMAC_CC_DWIDTH_BYTE |
                     XDMAC_CC_SIF_AHB_IF1 |
                     XDMAC_CC_DIF_AHB_IF0 |
                     XDMAC_CC_SAM_FIXED_AM |
                     XDMAC_CC_DAM_INCREMENTED_AM |
                     XDMAC_CC_PERID(PERID_FLEXCOM2_RX);
  FSPID2.txdmamode = XDMAC_CC_TYPE_PER_TRAN |
                     XDMAC_CC_MBSIZE_SIXTEEN |
                     XDMAC_CC_DSYNC_MEM2PER |
                     XDMAC_CC_PROT_SEC |
                     XDMAC_CC_CSIZE_CHK_1 |
                     XDMAC_CC_DWIDTH_BYTE |
                     XDMAC_CC_SIF_AHB_IF0 |
                     XDMAC_CC_DIF_AHB_IF1 |
                     XDMAC_CC_SAM_INCREMENTED_AM |
                     XDMAC_CC_DAM_FIXED_AM |
                     XDMAC_CC_PERID(PERID_FLEXCOM2_TX);
#endif /* SAMA_SPI_USE_FLEXCOM2 */

#if SAMA_SPI_USE_FLEXCOM3
#if SAMA_HAL_IS_SECURE
  mtxConfigPeriphSecurity(MATRIX1, ID_FLEXCOM3, SECURE_PER);
#endif /* SAMA_HAL_IS_SECURE */
  /* Driver initialization.*/
  spiObjectInit(&FSPID3);
  FSPID3.spi       = FCOMSPI3;
  FSPID3.flexcom   = FLEXCOM3;
  FSPID3.dmarx     = NULL;
  FSPID3.dmatx     = NULL;
  FSPID3.rxdmamode = XDMAC_CC_TYPE_PER_TRAN |
                     XDMAC_CC_MBSIZE_SIXTEEN |
                     XDMAC_CC_DSYNC_PER2MEM |
                     XDMAC_CC_PROT_SEC |
                     XDMAC_CC_CSIZE_CHK_1 |
                     XDMAC_CC_DWIDTH_BYTE |
                     XDMAC_CC_SIF_AHB_IF1 |
                     XDMAC_CC_DIF_AHB_IF0 |
                     XDMAC_CC_SAM_FIXED_AM |
                     XDMAC_CC_DAM_INCREMENTED_AM |
                     XDMAC_CC_PERID(PERID_FLEXCOM3_RX);
  FSPID3.txdmamode = XDMAC_CC_TYPE_PER_TRAN |
                     XDMAC_CC_MBSIZE_SIXTEEN |
                     XDMAC_CC_DSYNC_MEM2PER |
                     XDMAC_CC_PROT_SEC |
                     XDMAC_CC_CSIZE_CHK_1 |
                     XDMAC_CC_DWIDTH_BYTE |
                     XDMAC_CC_SIF_AHB_IF0 |
                     XDMAC_CC_DIF_AHB_IF1 |
                     XDMAC_CC_SAM_INCREMENTED_AM |
                     XDMAC_CC_DAM_FIXED_AM |
                     XDMAC_CC_PERID(PERID_FLEXCOM3_TX);
#endif /* SAMA_SPI_USE_FLEXCOM3 */

#if SAMA_SPI_USE_FLEXCOM4
#if SAMA_HAL_IS_SECURE
  mtxConfigPeriphSecurity(MATRIX1, ID_FLEXCOM4, SECURE_PER);
#endif /* SAMA_HAL_IS_SECURE */
  /* Driver initialization.*/
  spiObjectInit(&FSPID4);
  FSPID4.spi       = FCOMSPI4;
  FSPID4.flexcom   = FLEXCOM4;
  FSPID4.dmarx     = NULL;
  FSPID4.dmatx     = NULL;
  FSPID4.rxdmamode = XDMAC_CC_TYPE_PER_TRAN |
                     XDMAC_CC_MBSIZE_SIXTEEN |
                     XDMAC_CC_DSYNC_PER2MEM |
                     XDMAC_CC_PROT_SEC |
                     XDMAC_CC_CSIZE_CHK_1 |
                     XDMAC_CC_DWIDTH_BYTE |
                     XDMAC_CC_SIF_AHB_IF1 |
                     XDMAC_CC_DIF_AHB_IF0 |
                     XDMAC_CC_SAM_FIXED_AM |
                     XDMAC_CC_DAM_INCREMENTED_AM |
                     XDMAC_CC_PERID(PERID_FLEXCOM4_RX);
  FSPID4.txdmamode = XDMAC_CC_TYPE_PER_TRAN |
                     XDMAC_CC_MBSIZE_SIXTEEN |
                     XDMAC_CC_DSYNC_MEM2PER |
                     XDMAC_CC_PROT_SEC |
                     XDMAC_CC_CSIZE_CHK_1 |
                     XDMAC_CC_DWIDTH_BYTE |
                     XDMAC_CC_SIF_AHB_IF0 |
                     XDMAC_CC_DIF_AHB_IF1 |
                     XDMAC_CC_SAM_INCREMENTED_AM |
                     XDMAC_CC_DAM_FIXED_AM |
                     XDMAC_CC_PERID(PERID_FLEXCOM4_TX);
#endif /* SAMA_SPI_USE_FLEXCOM4 */
}

/**
 * @brief   Configures and activates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_start(SPIDriver *spip) {

  /* Configures the peripheral.*/

  if (spip->state == SPI_STOP) {

#if SAMA_SPI_USE_SPI0
    if (&SPID0 == spip) {
      spip->dmarx = dmaChannelAllocate(SAMA_SPI_SPI0_DMA_IRQ_PRIORITY,
                                       (sama_dmaisr_t)spi_lld_serve_rx_interrupt,
                                       (void *)spip);
      osalDbgAssert(spip->dmarx != NULL, "no channel allocated");

      spip->dmatx = dmaChannelAllocate(SAMA_SPI_SPI0_DMA_IRQ_PRIORITY,
                                       (sama_dmaisr_t)spi_lld_serve_tx_interrupt,
                                       (void *)spip);
      osalDbgAssert(spip->dmatx != NULL, "no channel allocated");
    /* Enable SPI0 clock */
      pmcEnableSPI0();
    }
#endif /* SAMA_SPI_USE_SPI0 */
#if SAMA_SPI_USE_SPI1
    if (&SPID1 == spip) {
      spip->dmarx = dmaChannelAllocate(SAMA_SPI_SPI1_DMA_IRQ_PRIORITY,
                                       (sama_dmaisr_t)spi_lld_serve_rx_interrupt,
                                       (void *)spip);
      osalDbgAssert(spip->dmarx != NULL, "no channel allocated");

      spip->dmatx = dmaChannelAllocate(SAMA_SPI_SPI1_DMA_IRQ_PRIORITY,
                                       (sama_dmaisr_t)spi_lld_serve_tx_interrupt,
                                       (void *)spip);
      osalDbgAssert(spip->dmatx != NULL, "no channel allocated");
    /* Enable SPI1 clock */
      pmcEnableSPI1();
    }
#endif /* SAMA_SPI_USE_SPI1 */
#if SAMA_SPI_USE_FLEXCOM0
    if (&FSPID0 == spip) {
      spip->dmarx = dmaChannelAllocate(SAMA_SPI_FLEXCOM0_DMA_IRQ_PRIORITY,
                                       (sama_dmaisr_t)spi_lld_serve_rx_interrupt,
                                       (void *)spip);
      osalDbgAssert(spip->dmarx != NULL, "no channel allocated");

      spip->dmatx = dmaChannelAllocate(SAMA_SPI_FLEXCOM0_DMA_IRQ_PRIORITY,
                                       (sama_dmaisr_t)spi_lld_serve_tx_interrupt,
                                       (void *)spip);
      osalDbgAssert(spip->dmatx != NULL, "no channel allocated");
    /* Enabling SPI on FLEXCOM */
      spip->flexcom->FLEX_MR = FLEX_MR_OPMODE_SPI;
    /* Enable FLEXCOM0 clock */
      pmcEnableFLEXCOM0();
    }
#endif /* SAMA_SPI_USE_FLEXCOM0 */
#if SAMA_SPI_USE_FLEXCOM1
    if (&FSPID1 == spip) {
      spip->dmarx = dmaChannelAllocate(SAMA_SPI_FLEXCOM1_DMA_IRQ_PRIORITY,
                                       (sama_dmaisr_t)spi_lld_serve_rx_interrupt,
                                       (void *)spip);
      osalDbgAssert(spip->dmarx != NULL, "no channel allocated");

      spip->dmatx = dmaChannelAllocate(SAMA_SPI_FLEXCOM1_DMA_IRQ_PRIORITY,
                                       (sama_dmaisr_t)spi_lld_serve_tx_interrupt,
                                       (void *)spip);
      osalDbgAssert(spip->dmatx != NULL, "no channel allocated");
    /* Enabling SPI on FLEXCOM */
      spip->flexcom->FLEX_MR = FLEX_MR_OPMODE_SPI;
    /* Enable FLEXCOM1 clock */
      pmcEnableFLEXCOM1();
    }
#endif /* SAMA_SPI_USE_FLEXCOM1 */
#if SAMA_SPI_USE_FLEXCOM2
    if (&FSPID2 == spip) {
      spip->dmarx = dmaChannelAllocate(SAMA_SPI_FLEXCOM2_DMA_IRQ_PRIORITY,
                                       (sama_dmaisr_t)spi_lld_serve_rx_interrupt,
                                       (void *)spip);
      osalDbgAssert(spip->dmarx != NULL, "no channel allocated");

      spip->dmatx = dmaChannelAllocate(SAMA_SPI_FLEXCOM2_DMA_IRQ_PRIORITY,
                                       (sama_dmaisr_t)spi_lld_serve_tx_interrupt,
                                       (void *)spip);
      osalDbgAssert(spip->dmatx != NULL, "no channel allocated");
    /* Enabling SPI on FLEXCOM */
      spip->flexcom->FLEX_MR = FLEX_MR_OPMODE_SPI;
    /* Enable FLEXCOM2 clock */
      pmcEnableFLEXCOM2();
    }
#endif /* SAMA_SPI_USE_FLEXCOM2 */
#if SAMA_SPI_USE_FLEXCOM3
    if (&FSPID3 == spip) {
      spip->dmarx = dmaChannelAllocate(SAMA_SPI_FLEXCOM3_DMA_IRQ_PRIORITY,
                                       (sama_dmaisr_t)spi_lld_serve_rx_interrupt,
                                       (void *)spip);
      osalDbgAssert(spip->dmarx != NULL, "no channel allocated");

      spip->dmatx = dmaChannelAllocate(SAMA_SPI_FLEXCOM3_DMA_IRQ_PRIORITY,
                                       (sama_dmaisr_t)spi_lld_serve_tx_interrupt,
                                       (void *)spip);
      osalDbgAssert(spip->dmatx != NULL, "no channel allocated");
    /* Enabling SPI on FLEXCOM */
      spip->flexcom->FLEX_MR = FLEX_MR_OPMODE_SPI;
    /* Enable FLEXCOM3 clock */
      pmcEnableFLEXCOM3();
    }
#endif /* SAMA_SPI_USE_FLEXCOM3 */
#if SAMA_SPI_USE_FLEXCOM4
    if (&FSPID4 == spip) {
      spip->dmarx = dmaChannelAllocate(SAMA_SPI_FLEXCOM4_DMA_IRQ_PRIORITY,
                                       (sama_dmaisr_t)spi_lld_serve_rx_interrupt,
                                       (void *)spip);
      osalDbgAssert(spip->dmarx != NULL, "no channel allocated");

      spip->dmatx = dmaChannelAllocate(SAMA_SPI_FLEXCOM4_DMA_IRQ_PRIORITY,
                                       (sama_dmaisr_t)spi_lld_serve_tx_interrupt,
                                       (void *)spip);
      osalDbgAssert(spip->dmatx != NULL, "no channel allocated");
    /* Enabling SPI on FLEXCOM */
      spip->flexcom->FLEX_MR = FLEX_MR_OPMODE_SPI;
    /* Enable FLEXCOM4 clock */
      pmcEnableFLEXCOM4();
    }
#endif /* SAMA_SPI_USE_FLEXCOM4 */
  }

  /* Set mode */
  dmaChannelSetMode(spip->dmarx, spip->rxdmamode);
  dmaChannelSetMode(spip->dmatx, spip->txdmamode);

  /* Disable write protection */
  spiDisableWP(spip->spi);

  /* Execute a software reset of the SPI twice */
  spip->spi->SPI_CR = SPI_CR_SWRST;
  spip->spi->SPI_CR = SPI_CR_SWRST;

  /* SPI configuration */
  spip->spi->SPI_MR = SPI_MR_MSTR | SPI_MR_WDRBT | spip->config->mr;
  spip->spi->SPI_MR &= ~SPI_MR_PCS_Msk;
  spip->spi->SPI_MR |=  SPI_PCS(spip->config->npcs);
  spip->spi->SPI_CSR[spip->config->npcs] = spip->config->csr;

  /* if SPI_CSRx_BITS > 8, dma is set to 16 bits  */
  if (((spip->spi->SPI_CSR[spip->config->npcs] >> 4) & 0xF) > 0) {
    dmaChannelSetMode(spip->dmatx, spip->txdmamode | XDMAC_CC_DWIDTH_HALFWORD);
    dmaChannelSetMode(spip->dmarx, spip->rxdmamode | XDMAC_CC_DWIDTH_HALFWORD);
  }

  /* Enable SPI */
  spip->spi->SPI_CR |= SPI_CR_SPIEN;
  /* Enable write protection.  */
  spiEnableWP(spip->spi);
}

/**
 * @brief   Deactivates the SPI peripheral.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_stop(SPIDriver *spip) {

  if (spip->state == SPI_READY) {
  /* Disable write protection */
    spiDisableWP(spip->spi);
  /* Reset SPI */
    spip->spi->SPI_MR = 0;
    spip->spi->SPI_CSR[spip->config->npcs] = 0;
  /* Disable SPI */
    spip->spi->SPI_CR |= SPI_CR_SPIDIS;
  /* Enable write protection */
    spiEnableWP(spip->spi);
	
    dmaChannelRelease(spip->dmarx);
    dmaChannelRelease(spip->dmatx);

#if SAMA_SPI_USE_SPI0
    if (&SPID0 == spip)
    /* Disable SPI0 clock */
      pmcDisableSPI0();

#endif /* SAMA_SPI_USE_SPI1 */
#if SAMA_SPI_USE_SPI1
    if (&SPID1 == spip)
    /* Disable SPI1 clock */
      pmcDisableSPI1();

#endif /* SAMA_SPI_USE_FLEXCOM0 */

#if SAMA_SPI_USE_FLEXCOM0
    if (&FSPID0 == spip)
    /* Disable FLEXCOM0 clock */
      pmcDisableFLEXCOM0();

#endif /* SAMA_SPI_USE_FLEXCOM0 */

#if SAMA_SPI_USE_FLEXCOM1
    if (&FSPID1 == spip)
    /* Disable FLEXCOM1 clock */
      pmcDisableFLEXCOM1();

#endif /* SAMA_SPI_USE_FLEXCOM1 */

#if SAMA_SPI_USE_FLEXCOM2
    if (&FSPID2 == spip)
    /* Disable FLEXCOM2 clock */
      pmcDisableFLEXCOM2();

#endif /* SAMA_SPI_USE_FLEXCOM2 */

#if SAMA_SPI_USE_FLEXCOM3
    if (&FSPID3 == spip)
    /* Disable FLEXCOM3 clock */
      pmcDisableFLEXCOM3();

#endif /* SAMA_SPI_USE_FLEXCOM3 */

#if SAMA_SPI_USE_FLEXCOM4
    if (&FSPID4 == spip)
    /* Disable FLEXCOM4 clock */
      pmcDisableFLEXCOM4();

#endif /* SAMA_SPI_USE_FLEXCOM4 */
  }
}

#if (SPI_SELECT_MODE == (SPI_SELECT_MODE_LLD || SPI_SELECT_MODE_PAD ||        \
                         SPI_SELECT_MODE_PORT || SPI_SELECT_MODE_LINE)) || defined(__DOXYGEN__)
/**
 * @brief   Asserts the slave select signal and prepares for transfers.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_select(SPIDriver *spip) {
  /* No implementation on SAMA.*/
}

/**
 * @brief   Deasserts the slave select signal.
 * @details The previously selected peripheral is unselected.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
void spi_lld_unselect(SPIDriver *spip) {
  /* No implementation on SAMA.*/
}
#endif

/**
 * @brief   Exchanges data on the SPI bus.
 * @details This asynchronous function starts a simultaneous transmit/receive
 *          operation.
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below or
 *          equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to be exchanged
 * @param[in] txbuf    the pointer to the transmit buffer
 * @param[out]rxbuf    the pointer to the receive buffer
 *
 * @notapi
 */
void spi_lld_exchange(SPIDriver *spip, size_t n,
                      const void *txbuf, void *rxbuf) {

  /* Writing channel */
  dmaChannelSetSource(spip->dmatx, txbuf);
  dmaChannelSetDestination(spip->dmatx, &spip->spi->SPI_TDR);
  dmaChannelSetTransactionSize(spip->dmatx, n);

  /* Reading channel */
  dmaChannelSetSource(spip->dmarx, &spip->spi->SPI_RDR);
  dmaChannelSetDestination(spip->dmarx, rxbuf);
  dmaChannelSetTransactionSize(spip->dmarx, n);

  dmaChannelEnable(spip->dmarx);
  dmaChannelEnable(spip->dmatx);
}

/**
 * @brief   Sends data over the SPI bus.
 * @details This asynchronous function starts a transmit operation.
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below or
 *          equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to send
 * @param[in] txbuf     the pointer to the transmit buffer
 *
 * @notapi
 */
void spi_lld_send(SPIDriver *spip, size_t n, const void *txbuf) {

  /* Writing channel */
  dmaChannelSetSource(spip->dmatx, txbuf);
  dmaChannelSetDestination(spip->dmatx, &spip->spi->SPI_TDR);
  dmaChannelSetTransactionSize(spip->dmatx, n);

  /* Reading channel */
  dmaChannelSetSource(spip->dmarx, &spip->spi->SPI_RDR);
  dmaChannelSetDestination(spip->dmarx, &dummyrx);
  dmaChannelSetTransactionSize(spip->dmarx, n);
  /* Enable write protection.  */

  dmaChannelEnable(spip->dmarx);
  dmaChannelEnable(spip->dmatx);

  /* Waiting TXEMPTY flag */
  while (!(spip->spi->SPI_SR & SPI_SR_TXEMPTY)) {
    ;
  }
}

/**
 * @brief   Receives data from the SPI bus.
 * @details This asynchronous function starts a receive operation.
 * @post    At the end of the operation the configured callback is invoked.
 * @note    The buffers are organized as uint8_t arrays for data sizes below or
 *          equal to 8 bits else it is organized as uint16_t arrays.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 * @param[in] n         number of words to receive
 * @param[out]rxbuf    the pointer to the receive buffer
 *
 * @notapi
 */
void spi_lld_receive(SPIDriver *spip, size_t n, void *rxbuf) {

  /* Writing channel */
  dmaChannelSetSource(spip->dmatx, &dummytx);
  dmaChannelSetDestination(spip->dmatx, &spip->spi->SPI_TDR);
  dmaChannelSetTransactionSize(spip->dmatx, n);

  /* Reading channel */
  dmaChannelSetSource(spip->dmarx, &spip->spi->SPI_RDR);
  dmaChannelSetDestination(spip->dmarx, rxbuf);
  dmaChannelSetTransactionSize(spip->dmarx, n);

  dmaChannelEnable(spip->dmarx);
  dmaChannelEnable(spip->dmatx);
}

#endif /* HAL_USE_SPI */

/** @} */

