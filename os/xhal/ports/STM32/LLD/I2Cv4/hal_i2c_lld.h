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
/*
   Concepts and parts of this file have been contributed by Uladzimir Pylinsky
   aka barthess.
 */

/**
 * @file    I2Cv4/hal_i2c_lld.h
 * @brief   STM32 I2C subsystem low level driver header.
 *
 * @addtogroup I2C
 * @{
 */

#ifndef HAL_I2C_LLD_H
#define HAL_I2C_LLD_H

#if HAL_USE_I2C || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @brief   This I2C LLD supports slave mode.
 */
#define I2C_SUPPORTS_SLAVE_MODE             FALSE

/**
 * @name    TIMINGR register definitions
 * @{
 */
#define STM32_TIMINGR_PRESC_MASK        (15U << 28)
#define STM32_TIMINGR_PRESC(n)          ((n) << 28)
#define STM32_TIMINGR_SCLDEL_MASK       (15U << 20)
#define STM32_TIMINGR_SCLDEL(n)         ((n) << 20)
#define STM32_TIMINGR_SDADEL_MASK       (15U << 16)
#define STM32_TIMINGR_SDADEL(n)         ((n) << 16)
#define STM32_TIMINGR_SCLH_MASK         (255U << 8)
#define STM32_TIMINGR_SCLH(n)           ((n) << 8)
#define STM32_TIMINGR_SCLL_MASK         (255U << 0)
#define STM32_TIMINGR_SCLL(n)           ((n) << 0)
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   I2C1 driver enable switch.
 * @details If set to @p TRUE the support for I2C1 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_I2C_USE_I2C1) || defined(__DOXYGEN__)
#define STM32_I2C_USE_I2C1                  FALSE
#endif

/**
 * @brief   I2C2 driver enable switch.
 * @details If set to @p TRUE the support for I2C2 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_I2C_USE_I2C2) || defined(__DOXYGEN__)
#define STM32_I2C_USE_I2C2                  FALSE
#endif

/**
 * @brief   I2C3 driver enable switch.
 * @details If set to @p TRUE the support for I2C3 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_I2C_USE_I2C3) || defined(__DOXYGEN__)
#define STM32_I2C_USE_I2C3                  FALSE
#endif

/**
 * @brief   I2C4 driver enable switch.
 * @details If set to @p TRUE the support for I2C4 is included.
 * @note    The default is @p FALSE.
 */
#if !defined(STM32_I2C_USE_I2C4) || defined(__DOXYGEN__)
#define STM32_I2C_USE_I2C4                  FALSE
#endif

/**
 * @brief   DMA use switch.
 */
#if !defined(STM32_I2C_USE_DMA) || defined(__DOXYGEN__)
#define STM32_I2C_USE_DMA                   TRUE
#endif

/**
 * @brief   I2C1 DMA priority (0..3|lowest..highest).
 */
#if !defined(STM32_I2C_I2C1_DMA_PRIORITY) || defined(__DOXYGEN__)
#define STM32_I2C_I2C1_DMA_PRIORITY         1
#endif

/**
 * @brief   I2C2 DMA priority (0..3|lowest..highest).
 */
#if !defined(STM32_I2C_I2C2_DMA_PRIORITY) || defined(__DOXYGEN__)
#define STM32_I2C_I2C2_DMA_PRIORITY         1
#endif

/**
 * @brief   I2C3 DMA priority (0..3|lowest..highest).
 */
#if !defined(STM32_I2C_I2C3_DMA_PRIORITY) || defined(__DOXYGEN__)
#define STM32_I2C_I2C3_DMA_PRIORITY         1
#endif

/**
 * @brief   I2C4 DMA priority (0..3|lowest..highest).
 */
#if !defined(STM32_I2C_I2C4_DMA_PRIORITY) || defined(__DOXYGEN__)
#define STM32_I2C_I2C4_DMA_PRIORITY         1
#endif

/**
 * @brief   I2C DMA error hook.
 * @note    The default action for DMA errors is a system halt because DMA
 *          error can only happen because programming errors.
 */
#if !defined(STM32_I2C_DMA_ERROR_HOOK) || defined(__DOXYGEN__)
#define STM32_I2C_DMA_ERROR_HOOK(i2cp)      osalSysHalt("DMA failure")
#endif

/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Registry checks.*/
#if !defined(STM32_HAS_I2C1) || !defined(STM32_HAS_I2C2) ||                 \
    !defined(STM32_HAS_I2C3) || !defined(STM32_HAS_I2C4)
#error "STM32_HAS_I2Cx not defined in registry"
#endif

/* IP instances check.*/
#if STM32_I2C_USE_I2C1 && !STM32_HAS_I2C1
#error "I2C1 not present in the selected device"
#endif

#if STM32_I2C_USE_I2C2 && !STM32_HAS_I2C2
#error "I2C2 not present in the selected device"
#endif

#if STM32_I2C_USE_I2C3 && !STM32_HAS_I2C3
#error "I2C3 not present in the selected device"
#endif

#if STM32_I2C_USE_I2C4 && !STM32_HAS_I2C4
#error "I2C4 not present in the selected device"
#endif

#if !STM32_I2C_USE_I2C1 && !STM32_I2C_USE_I2C2 && !STM32_I2C_USE_I2C3 &&    \
    !STM32_I2C_USE_I2C4
#error "I2C driver activated but no I2C peripheral assigned"
#endif

#if STM32_I2C_USE_I2C1
#if defined(STM32_I2C1_IS_USED)
#error "I2CD1 requires I2C1 but it is already used"
#else
#define STM32_I2C1_IS_USED
#endif
#endif

#if STM32_I2C_USE_I2C2
#if defined(STM32_I2C2_IS_USED)
#error "I2CD2 requires I2C2 but it is already used"
#else
#define STM32_I2C2_IS_USED
#endif
#endif

#if STM32_I2C_USE_I2C3
#if defined(STM32_I2C3_IS_USED)
#error "I2CD3 requires I2C3 but it is already used"
#else
#define STM32_I2C3_IS_USED
#endif
#endif

#if STM32_I2C_USE_I2C4
#if defined(STM32_I2C4_IS_USED)
#error "I2CD4 requires I2C4 but it is already used"
#else
#define STM32_I2C4_IS_USED
#endif
#endif

/* IRQ-related checks.*/
#if !defined(STM32_I2C_SINGLE_IRQ)
#error "STM32_I2C_SINGLE_IRQ not defined in registry"
#endif

#if (STM32_I2C_SINGLE_IRQ != FALSE) && (STM32_I2C_SINGLE_IRQ != TRUE)
#error "invalid STM32_I2C_SINGLE_IRQ value"
#endif

#if defined(I2C_ENABLE_SLAVE_MODE) && (I2C_ENABLE_SLAVE_MODE == TRUE)
#error "I2C slave mode is not supported by this XHAL LLD"
#endif

/* DMA-related checks.*/
#if STM32_I2C_USE_DMA == TRUE
#if defined(STM32_DMA3_PRESENT)

/* Check on the presence of the GPDMA channels settings in mcuconf.h.*/
#if STM32_I2C_USE_I2C1 && (!defined(STM32_I2C_I2C1_DMA3_CHANNEL))
#error "I2C1 GPDMA channels not defined"
#endif

#if STM32_I2C_USE_I2C2 && (!defined(STM32_I2C_I2C2_DMA3_CHANNEL))
#error "I2C2 GPDMA channels not defined"
#endif

#if STM32_I2C_USE_I2C3 && (!defined(STM32_I2C_I2C3_DMA3_CHANNEL))
#error "I2C3 GPDMA channels not defined"
#endif

#if STM32_I2C_USE_I2C4 && (!defined(STM32_I2C_I2C4_DMA3_CHANNEL))
#error "I2C4 GPDMA channels not defined"
#endif

/* Check on DMA channels assignment.*/
#if STM32_I2C_USE_I2C1 &&                                                   \
    !STM32_DMA3_ARE_VALID_CHANNELS(STM32_I2C_I2C1_DMA3_CHANNEL)
#error "Invalid GPDMA channel assigned to I2C1"
#endif

#if STM32_I2C_USE_I2C2 &&                                                   \
    !STM32_DMA3_ARE_VALID_CHANNELS(STM32_I2C_I2C2_DMA3_CHANNEL)
#error "Invalid GPDMA channel assigned to I2C2"
#endif

#if STM32_I2C_USE_I2C3 &&                                                   \
    !STM32_DMA3_ARE_VALID_CHANNELS(STM32_I2C_I2C3_DMA3_CHANNEL)
#error "Invalid GPDMA channel assigned to I2C3"
#endif

#if STM32_I2C_USE_I2C4 &&                                                   \
    !STM32_DMA3_ARE_VALID_CHANNELS(STM32_I2C_I2C4_DMA3_CHANNEL)
#error "Invalid GPDMA channel assigned to I2C4"
#endif

#if STM32_I2C_USE_I2C1 &&                                                   \
    !STM32_DMA3_IS_VALID_PRIORITY(STM32_I2C_I2C1_DMA_PRIORITY)
#error "Invalid GPDMA priority assigned to I2C1"
#endif

#if STM32_I2C_USE_I2C2 &&                                                   \
    !STM32_DMA3_IS_VALID_PRIORITY(STM32_I2C_I2C2_DMA_PRIORITY)
#error "Invalid GPDMA priority assigned to I2C2"
#endif

#if STM32_I2C_USE_I2C3 &&                                                   \
    !STM32_DMA3_IS_VALID_PRIORITY(STM32_I2C_I2C3_DMA_PRIORITY)
#error "Invalid GPDMA priority assigned to I2C3"
#endif

#if STM32_I2C_USE_I2C4 &&                                                   \
    !STM32_DMA3_IS_VALID_PRIORITY(STM32_I2C_I2C4_DMA_PRIORITY)
#error "Invalid GPDMA priority assigned to I2C4"
#endif

#if STM32_I2C_USE_I2C1 || STM32_I2C_USE_I2C2 || STM32_I2C_USE_I2C3 ||       \
    STM32_I2C_USE_I2C4
#if !defined(STM32_DMA3_REQUIRED)
#define STM32_DMA3_REQUIRED
#endif
#endif

#else

/* Check on the presence of the DMA channels settings in mcuconf.h.*/
#if STM32_I2C_USE_I2C1 && !defined(STM32_I2C_I2C1_DMA_CHANNEL)
#error "I2C1 DMA channel not defined"
#endif

#if STM32_I2C_USE_I2C2 && !defined(STM32_I2C_I2C2_DMA_CHANNEL)
#error "I2C2 DMA channel not defined"
#endif

#if STM32_I2C_USE_I2C3 && !defined(STM32_I2C_I2C3_DMA_CHANNEL)
#error "I2C3 DMA channel not defined"
#endif

#if STM32_I2C_USE_I2C4 && !defined(STM32_I2C_I2C4_DMA_CHANNEL)
#error "I2C4 DMA channel not defined"
#endif

/* Check on DMA channel assignment.*/
#if STM32_I2C_USE_I2C1 &&                                                   \
    !STM32_DMA_IS_VALID_STREAM(STM32_I2C_I2C1_DMA_CHANNEL)
#error "Invalid DMA channel assigned to I2C1"
#endif

/* Check on DMA channel assignment.*/
#if STM32_I2C_USE_I2C2 &&                                                   \
    !STM32_DMA_IS_VALID_STREAM(STM32_I2C_I2C2_DMA_CHANNEL)
#error "Invalid DMA channel assigned to I2C2"
#endif

/* Check on DMA channel assignment.*/
#if STM32_I2C_USE_I2C3 &&                                                   \
    !STM32_DMA_IS_VALID_STREAM(STM32_I2C_I2C3_DMA_CHANNEL)
#error "Invalid DMA channel assigned to I2C3"
#endif

/* Check on DMA channel assignment.*/
#if STM32_I2C_USE_I2C4 &&                                                   \
    !STM32_DMA_IS_VALID_STREAM(STM32_I2C_I2C4_DMA_CHANNEL)
#error "Invalid DMA channel assigned to I2C4"
#endif

#if STM32_I2C_USE_I2C1 &&                                                   \
    !STM32_DMA_IS_VALID_PRIORITY(STM32_I2C_I2C1_DMA_PRIORITY)
#error "Invalid DMA priority assigned to I2C1"
#endif

#if STM32_I2C_USE_I2C2 &&                                                   \
    !STM32_DMA_IS_VALID_PRIORITY(STM32_I2C_I2C2_DMA_PRIORITY)
#error "Invalid DMA priority assigned to I2C2"
#endif

#if STM32_I2C_USE_I2C3 &&                                                   \
    !STM32_DMA_IS_VALID_PRIORITY(STM32_I2C_I2C3_DMA_PRIORITY)
#error "Invalid DMA priority assigned to I2C3"
#endif

#if STM32_I2C_USE_I2C4 &&                                                   \
    !STM32_DMA_IS_VALID_PRIORITY(STM32_I2C_I2C4_DMA_PRIORITY)
#error "Invalid DMA priority assigned to I2C4"
#endif

#if STM32_I2C_USE_I2C1 || STM32_I2C_USE_I2C2 || STM32_I2C_USE_I2C3 ||       \
    STM32_I2C_USE_I2C4
#if !defined(STM32_DMA_REQUIRED)
#define STM32_DMA_REQUIRED
#endif
#endif

#endif /* STM32_DMA3_PRESENT */
#endif /* STM32_I2C_USE_DMA == TRUE */

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Platform-dependent I2C configuration fields.
 */
#if (STM32_I2C_USE_DMA == TRUE) && defined(STM32_DMA3_PRESENT)
#define i2c_lld_config_fields                                               \
  /* TIMINGR register initialization.*/                                     \
  uint32_t                          timingr;                                \
  /* CR1 register initialization.*/                                         \
  uint32_t                          cr1;                                    \
  /* CR2 register initialization.*/                                         \
  uint32_t                          cr2;                                    \
  /* DMA RX CTR1 register settings.*/                                       \
  uint32_t                          dtr1rx;                                 \
  /* DMA TX CTR1 register settings.*/                                       \
  uint32_t                          dtr1tx;                                 \
  /* DMA RX CTR2 register settings.*/                                       \
  uint32_t                          dtr2rx;                                 \
  /* DMA TX CTR2 register settings.*/                                       \
  uint32_t                          dtr2tx
#else
#define i2c_lld_config_fields                                               \
  /* TIMINGR register initialization.*/                                     \
  uint32_t                          timingr;                                \
  /* CR1 register initialization.*/                                         \
  uint32_t                          cr1;                                    \
  /* CR2 register initialization.*/                                         \
  uint32_t                          cr2
#endif

/**
 * @brief   Platform-dependent I2C driver fields.
 */
#if STM32_I2C_USE_DMA == TRUE
#if defined(STM32_DMA3_PRESENT)
#define i2c_lld_driver_dma_fields                                           \
  /* DMA channel.*/                                                         \
  const stm32_dma3_channel_t        *dma
#else
#define i2c_lld_driver_dma_fields                                           \
  /* DMA stream.*/                                                          \
  const stm32_dma_stream_t          *dma
#endif

#define i2c_lld_driver_fields                                               \
  i2c_lld_driver_dma_fields;                                                \
  /* DMA channel priority.*/                                                \
  uint8_t                           dprio;                                  \
  /* DMA request line for TX.*/                                             \
  uint8_t                           dreqtx;                                 \
  /* DMA request line for RX.*/                                             \
  uint8_t                           dreqrx;                                 \
  /* Pointer to the next TX buffer location.*/                              \
  const uint8_t                     *txptr;                                 \
  /* Number of bytes in TX phase.*/                                         \
  size_t                            txbytes;                                \
  /* Pointer to the next RX buffer location.*/                              \
  uint8_t                           *rxptr;                                 \
  /* Number of bytes in RX phase.*/                                         \
  size_t                            rxbytes;                                \
  /* Pointer to the I2Cx registers block.*/                                 \
  I2C_TypeDef                       *i2c
#else
#define i2c_lld_driver_fields                                               \
  /* Pointer to the next TX buffer location.*/                              \
  const uint8_t                     *txptr;                                 \
  /* Number of bytes in TX phase.*/                                         \
  size_t                            txbytes;                                \
  /* Pointer to the next RX buffer location.*/                              \
  uint8_t                           *rxptr;                                 \
  /* Number of bytes in RX phase.*/                                         \
  size_t                            rxbytes;                                \
  /* Pointer to the I2Cx registers block.*/                                 \
  I2C_TypeDef                       *i2c
#endif

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Returns the active configuration.
 *
 * @param[in] i2cp      pointer to the @p hal_i2c_driver_c object
 *
 * @notapi
 */
#define i2c_lld_getcfg(i2cp) ((const hal_i2c_config_t *)((i2cp)->config))

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
#if STM32_I2C_USE_I2C1
extern hal_i2c_driver_c I2CD1;
#endif

#if STM32_I2C_USE_I2C2
extern hal_i2c_driver_c I2CD2;
#endif

#if STM32_I2C_USE_I2C3
extern hal_i2c_driver_c I2CD3;
#endif

#if STM32_I2C_USE_I2C4
extern hal_i2c_driver_c I2CD4;
#endif

#endif /* !defined(__DOXYGEN__) */

#ifdef __cplusplus
extern "C" {
#endif
  void i2c_lld_init(void);
  msg_t i2c_lld_start(hal_i2c_driver_c *i2cp);
  void i2c_lld_stop(hal_i2c_driver_c *i2cp);
  const hal_i2c_config_t *i2c_lld_setcfg(hal_i2c_driver_c *i2cp,
                                         const hal_i2c_config_t *config);
  const hal_i2c_config_t *i2c_lld_selcfg(hal_i2c_driver_c *i2cp,
                                         unsigned cfgnum);
  void i2c_lld_set_callback(hal_i2c_driver_c *i2cp, drv_cb_t cb);
  msg_t i2c_lld_start_master_transmit(hal_i2c_driver_c *i2cp, i2caddr_t addr,
                                      const uint8_t *txbuf, size_t txbytes,
                                      uint8_t *rxbuf, size_t rxbytes);
  msg_t i2c_lld_start_master_receive(hal_i2c_driver_c *i2cp, i2caddr_t addr,
                                     uint8_t *rxbuf, size_t rxbytes);
  msg_t i2c_lld_stop_transfer(hal_i2c_driver_c *i2cp);
#if STM32_I2C_SINGLE_IRQ == TRUE
  void i2c_lld_serve_global_interrupt(hal_i2c_driver_c *i2cp);
#else
  void i2c_lld_serve_ev_interrupt(hal_i2c_driver_c *i2cp);
  void i2c_lld_serve_er_interrupt(hal_i2c_driver_c *i2cp);
#endif
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_I2C  */

#endif /* HAL_I2C_LLD_H */

/** @} */
