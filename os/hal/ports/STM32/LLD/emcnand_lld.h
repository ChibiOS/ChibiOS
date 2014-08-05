/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

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
/*
   Concepts and parts of this file have been contributed by Uladzimir Pylinsky
   aka barthess.
 */

/**
 * @file    emcnand_lld.h
 * @brief   EMCNAND Driver subsystem low level driver header template.
 *
 * @addtogroup EMCNAND
 * @{
 */

#ifndef _EMCNAND_LLD_H_
#define _EMCNAND_LLD_H_

#if HAL_USE_EMCNAND || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/
#define EMCNAND_MIN_PAGE_SIZE       256
#define EMCNAND_MAX_PAGE_SIZE       8192
#define EMCNAND_BAD_MAP_END_MARK    ((uint16_t)0xFFFF)

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief   EMD FSMC1 interrupt priority level setting.
 */
#if !defined(STM32_EMC_FSMC1_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_EMC_FSMC1_IRQ_PRIORITY                10
#endif

/**
 * @brief   EMCNAND driver enable switch.
 * @details If set to @p TRUE the support for EMCNAND1 is included.
 */
#if !defined(STM32_EMCNAND_USE_EMCNAND1) || defined(__DOXYGEN__)
#define STM32_EMCNAND_USE_EMCNAND1                  FALSE
#endif

/**
 * @brief   EMCNAND driver enable switch.
 * @details If set to @p TRUE the support for EMCNAND2 is included.
 */
#if !defined(STM32_EMCNAND_USE_EMCNAND2) || defined(__DOXYGEN__)
#define STM32_EMCNAND_USE_EMCNAND2                  FALSE
#endif

/**
 * @brief   EMCNAND DMA error hook.
 * @note    The default action for DMA errors is a system halt because DMA
 *          error can only happen because programming errors.
 */
#if !defined(STM32_EMCNAND_DMA_ERROR_HOOK) || defined(__DOXYGEN__)
#define STM32_EMCNAND_DMA_ERROR_HOOK(emcnandp)      osalSysHalt("DMA failure")
#endif

/**
 * @brief   EMCNAND interrupt enable switch.
 * @details If set to @p TRUE the support for internal FSMC interrupt included.
 */
#if !defined(STM32_EMCNAND_USE_INT) || defined(__DOXYGEN__)
#define STM32_EMCNAND_USE_INT                       FALSE
#endif

/**
* @brief   EMCNAND1 DMA priority (0..3|lowest..highest).
*/
#if !defined(STM32_EMCNAND_EMCNAND1_DMA_PRIORITY) || defined(__DOXYGEN__)
#define STM32_EMCNAND_EMCNAND1_DMA_PRIORITY         0
#endif

/**
* @brief   EMCNAND2 DMA priority (0..3|lowest..highest).
*/
#if !defined(STM32_EMCNAND_EMCNAND2_DMA_PRIORITY) || defined(__DOXYGEN__)
#define STM32_EMCNAND_EMCNAND2_DMA_PRIORITY         0
#endif

/**
 * @brief   DMA stream used for EMCNAND1 operations.
 * @note    This option is only available on platforms with enhanced DMA.
 */
#if !defined(STM32_EMCNAND_EMCNAND1_DMA_STREAM) || defined(__DOXYGEN__)
#define STM32_EMCNAND_EMCNAND1_DMA_STREAM     STM32_DMA_STREAM_ID(2, 6)
#endif

/**
 * @brief   DMA stream used for EMCNAND2 operations.
 * @note    This option is only available on platforms with enhanced DMA.
 */
#if !defined(STM32_EMCNAND_EMCNAND2_DMA_STREAM) || defined(__DOXYGEN__)
#define STM32_EMCNAND_EMCNAND2_DMA_STREAM     STM32_DMA_STREAM_ID(2, 7)
#endif

/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if !STM32_EMCNAND_USE_EMCNAND1 && !STM32_EMCNAND_USE_EMCNAND2
#error "EMCNAND driver activated but no EMCNAND peripheral assigned"
#endif

#if STM32_EMCNAND_USE_EMCNAND1 && !STM32_HAS_EMC
#error "EMC not present in the selected device"
#endif

#if !STM32_EMCNAND_USE_INT && !HAL_USE_EXT
#error "External interrupt controller must be enabled to use this feature"
#endif

#if STM32_EMCNAND_USE_EMCNAND1 &&                                           \
    !STM32_DMA_IS_VALID_ID(STM32_EMCNAND_EMCNAND1_DMA_STREAM,               \
                           STM32_EMC_DMA_MSK)
#error "invalid DMA stream associated to EMCNAND"
#endif

#if STM32_EMCNAND_USE_EMCNAND2 &&                                           \
    !STM32_DMA_IS_VALID_ID(STM32_EMCNAND_EMCNAND2_DMA_STREAM,               \
                           STM32_EMC_DMA_MSK)
#error "invalid DMA stream associated to EMCNAND"
#endif

#if !defined(STM32_DMA_REQUIRED)
#define STM32_DMA_REQUIRED
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   NAND driver condition flags type.
 */
typedef uint32_t emcnandflags_t;

/**
 * @brief   Type of a structure representing an EMCNAND driver.
 */
typedef struct EMCNANDDriver EMCNANDDriver;

#if STM32_EMC_USE_INT
/**
 * @brief   Type of interrupt handler function
 */
typedef void (*emcnandisrhandler_t)
                      (EMCNANDDriver *emcnandp, emcnandflags_t flags);
#else
/**
 * @brief   Type of interrupt handler function
 */
typedef void (*emcnandisrhandler_t)(EMCNANDDriver *emcnandp);

/**
 * @brief   Type of function switching external interrupts on and off.
 */
typedef void (*emcnandisrswitch_t)(void);
#endif /* STM32_EMC_USE_INT */

/**
 * @brief   Driver configuration structure.
 * @note    It could be empty on some architectures.
 */
typedef struct {
  /**
   * @brief   Pointer to lower level driver.
   */
  EMCDriver                 *emcp;
  /**
   * @brief   Number of erase blocks in NAND device.
   */
  uint32_t                  blocks;
  /**
   * @brief   Number of data bytes in page.
   */
  uint32_t                  page_data_size;
  /**
   * @brief   Number of spare bytes in page.
   */
  uint32_t                  page_spare_size;
  /**
   * @brief   Number of pages in block.
   */
  uint32_t                  pages_per_block;
#if EMCNAND_USE_BAD_MAP
  /**
   * @brief   Pointer to bad block map.
   * @details One bit per block. Memory for map must be allocated by user.
   */
  uint32_t                  *bb_map;
#endif /* EMCNAND_USE_BAD_MAP */
  /**
   * @brief   Number of write cycles for row addressing.
   */
  uint8_t                   rowcycles;
  /**
   * @brief   Number of write cycles for column addressing.
   */
  uint8_t                   colcycles;

  /* End of the mandatory fields.*/
  /**
   * @brief   Number of wait cycles. This value will be used both for
   *          PMEM and PATTR registers
   *
   * @note    For proper calculation procedure please look at AN2784 document
   *          from STMicroelectronics.
   */
  uint32_t                  pmem;
#if !STM32_EMC_USE_INT
  /**
   * @brief   Function enabling interrupts from EXTI
   */
  emcnandisrswitch_t        ext_isr_enable;
  /**
   * @brief   Function disabling interrupts from EXTI
   */
  emcnandisrswitch_t        ext_isr_disable;
#endif /* !STM32_EMC_USE_INT */
} EMCNANDConfig;

/**
 * @brief   Structure representing an EMCNAND driver.
 */
struct EMCNANDDriver {
  /**
   * @brief   Driver state.
   */
  emcnandstate_t            state;
  /**
   * @brief   Current configuration data.
   */
  const EMCNANDConfig       *config;
  /**
   * @brief   Array to store bad block map.
   */
#if EMCNAND_USE_MUTUAL_EXCLUSION || defined(__DOXYGEN__)
#if CH_CFG_USE_MUTEXES || defined(__DOXYGEN__)
  /**
   * @brief   Mutex protecting the bus.
   */
  mutex_t                   mutex;
#elif CH_CFG_USE_SEMAPHORES
  semaphore_t               semaphore;
#endif
#endif /* EMCNAND_USE_MUTUAL_EXCLUSION */
  /* End of the mandatory fields.*/
  /**
   * @brief   Function enabling interrupts from FSMC
   */
  emcnandisrhandler_t       isr_handler;
  /**
   * @brief   Pointer to current transaction buffer
   */
  uint8_t                   *rxdata;
  /**
   * @brief   Current transaction length
   */
  size_t                    datalen;
  /**
   * @brief DMA mode bit mask.
   */
  uint32_t                  dmamode;
  /**
   * @brief     DMA channel.
   */
  const stm32_dma_stream_t  *dma;
  /**
   * @brief     Thread waiting for I/O completion.
   */
  thread_t                  *thread;
  /**
   * @brief     Pointer to the FSMC NAND registers block.
   */
  FSMC_NAND_TypeDef         *nand;
  /**
   * @brief     Memory mapping for data.
   */
  uint8_t                   *map_data;
  /**
   * @brief     Memory mapping for commands.
   */
  uint8_t                   *map_cmd;
  /**
   * @brief     Memory mapping for addresses.
   */
  uint8_t                   *map_addr;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if STM32_EMCNAND_USE_EMCNAND1 && !defined(__DOXYGEN__)
extern EMCNANDDriver EMCNANDD1;
#endif

#if STM32_EMCNAND_USE_EMCNAND2 && !defined(__DOXYGEN__)
extern EMCNANDDriver EMCNANDD2;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void emcnand_lld_init(void);
  void emcnand_lld_start(EMCNANDDriver *emcnandp);
  void emcnand_lld_stop(EMCNANDDriver *emcnandp);
  uint8_t emcnand_lld_write_data(EMCNANDDriver *emcnandp, const uint8_t *data,
          size_t datalen, uint8_t *addr, size_t addrlen, uint32_t *ecc);
  void emcnand_lld_read_data(EMCNANDDriver *emcnandp, uint8_t *data,
          size_t datalen, uint8_t *addr, size_t addrlen, uint32_t *ecc);
  void emcnand_lld_polled_read_data(EMCNANDDriver *emcnandp, uint8_t *data,
          size_t len);
  uint8_t emcnand_lld_erase(EMCNANDDriver *emcnandp, uint8_t *addr,
          size_t addrlen);
  void emcnand_lld_write_addr(EMCNANDDriver *emcnandp,
          const uint8_t *addr, size_t len);
  void emcnand_lld_write_cmd(EMCNANDDriver *emcnandp, uint8_t cmd);
  uint8_t emcnand_lld_read_status(EMCNANDDriver *emcnandp);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_EMCNAND */

#endif /* _EMCNAND_LLD_H_ */

/** @} */
