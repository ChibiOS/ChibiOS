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
 * @file        hal_sdc.h
 * @brief       Generated SDC Driver header.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_SDC
 * @brief       SDC Driver macros and structures.
 * @{
 */

#ifndef HAL_SDC_H
#define HAL_SDC_H

#include "hal_cb_driver.h"
#include "hal_block_io.h"

#if (HAL_USE_SDC == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    SD card types
 * @{
 */
/**
 * @brief       Mask of card type bits.
 */
#define SDC_MODE_CARDTYPE_MASK              0x0FU

/**
 * @brief       SD version 1.1 card.
 */
#define SDC_MODE_CARDTYPE_SDV11             0U

/**
 * @brief       SD version 2.0 card.
 */
#define SDC_MODE_CARDTYPE_SDV20             1U

/**
 * @brief       MMC card.
 */
#define SDC_MODE_CARDTYPE_MMC               2U

/**
 * @brief       High-capacity card flag.
 */
#define SDC_MODE_HIGH_CAPACITY              0x10U
/** @} */

/**
 * @name    SDC bus error conditions
 * @{
 */
/**
 * @brief       No error.
 */
#define SDC_NO_ERROR                        0U

/**
 * @brief       Command CRC error.
 */
#define SDC_CMD_CRC_ERROR                   1U

/**
 * @brief       Data CRC error.
 */
#define SDC_DATA_CRC_ERROR                  2U

/**
 * @brief       Data timeout.
 */
#define SDC_DATA_TIMEOUT                    4U

/**
 * @brief       Command timeout.
 */
#define SDC_COMMAND_TIMEOUT                 8U

/**
 * @brief       Transmit underrun.
 */
#define SDC_TX_UNDERRUN                     16U

/**
 * @brief       Receive overrun.
 */
#define SDC_RX_OVERRUN                      32U

/**
 * @brief       Start bit error.
 */
#define SDC_STARTBIT_ERROR                  64U

/**
 * @brief       Address range overflow.
 */
#define SDC_OVERFLOW_ERROR                  128U

/**
 * @brief       Unhandled error condition.
 */
#define SDC_UNHANDLED_ERROR                 0xFFFFFFFFU
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief       Support for blocking SDC transfer APIs.
 */
#if !defined(SDC_USE_SYNCHRONIZATION) || defined(__DOXYGEN__)
#define SDC_USE_SYNCHRONIZATION             TRUE
#endif

/**
 * @brief       Support for user-defined SDC configurations.
 * @note        When enabled the user must provide a variable named @p
 *              sdc_configurations of type @p sdc_configurations_t.
 */
#if !defined(SDC_USE_CONFIGURATIONS) || defined(__DOXYGEN__)
#define SDC_USE_CONFIGURATIONS              FALSE
#endif

/**
 * @brief       Number of initialization attempts before rejecting a card.
 */
#if !defined(SDC_INIT_RETRY) || defined(__DOXYGEN__)
#define SDC_INIT_RETRY                      100
#endif

/**
 * @brief       Include support for MMC cards.
 */
#if !defined(SDC_MMC_SUPPORT) || defined(__DOXYGEN__)
#define SDC_MMC_SUPPORT                     FALSE
#endif

/**
 * @brief       Insert small waits in polling loops.
 */
#if !defined(SDC_NICE_WAITING) || defined(__DOXYGEN__)
#define SDC_NICE_WAITING                    TRUE
#endif

/**
 * @brief       OCR initialization constant for SD V2.0 cards.
 */
#if !defined(SDC_INIT_OCR_V20) || defined(__DOXYGEN__)
#define SDC_INIT_OCR_V20                    0x50FF8000U
#endif

/**
 * @brief       OCR initialization constant for non-V2.0 cards.
 */
#if !defined(SDC_INIT_OCR) || defined(__DOXYGEN__)
#define SDC_INIT_OCR                        0x80100000U
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Checks on SDC_USE_SYNCHRONIZATION configuration.*/
#if (SDC_USE_SYNCHRONIZATION != FALSE) && (SDC_USE_SYNCHRONIZATION != TRUE)
#error "invalid SDC_USE_SYNCHRONIZATION value"
#endif

/* Checks on SDC_USE_CONFIGURATIONS configuration.*/
#if (SDC_USE_CONFIGURATIONS != FALSE) && (SDC_USE_CONFIGURATIONS != TRUE)
#error "invalid SDC_USE_CONFIGURATIONS value"
#endif

/* Checks on SDC_MMC_SUPPORT configuration.*/
#if (SDC_MMC_SUPPORT != FALSE) && (SDC_MMC_SUPPORT != TRUE)
#error "invalid SDC_MMC_SUPPORT value"
#endif

/* Checks on SDC_NICE_WAITING configuration.*/
#if (SDC_NICE_WAITING != FALSE) && (SDC_NICE_WAITING != TRUE)
#error "invalid SDC_NICE_WAITING value"
#endif

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @name    Macro functions
 * @{
 */
/**
 * @brief       Returns the card insertion status.
 *
 * @param[in]     sdcp          Pointer to the SDC driver instance.
 * @return                      The card state.
 * @retval false                Card not inserted.
 * @retval true                 Card inserted.
 *
 * @api
 */
#define sdcIsCardInserted(sdcp)                                             \
  (sdc_lld_is_card_inserted(sdcp))

/**
 * @brief       Returns the card write-protect status.
 *
 * @param[in]     sdcp          Pointer to the SDC driver instance.
 * @return                      The card state.
 * @retval false                Card is writable.
 * @retval true                 Card is write protected.
 *
 * @api
 */
#define sdcIsWriteProtected(sdcp)                                           \
  (sdc_lld_is_write_protected(sdcp))
/** @} */

/**
 * @name    Low level driver helper macros
 * @{
 */
#if (SDC_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
/**
 * @brief       Wakes up the thread waiting for transfer completion.
 *
 * @param[in]     sdcp          Pointer to the SDC driver instance.
 * @param[in]     msg           Wakeup message.
 *
 * @notapi
 */
#define _sdc_wakeup_isr(sdcp, msg)                                          \
  do {                                                                      \
    osalThreadResumeI(&(sdcp)->sync_transfer, msg);                         \
  } while (false)
#endif /* SDC_USE_SYNCHRONIZATION == TRUE */

#if (SDC_USE_SYNCHRONIZATION != TRUE) || defined (__DOXYGEN__)
/**
 * @brief       Wakes up the thread waiting for transfer completion.
 *
 * @param[in]     sdcp          Pointer to the SDC driver instance.
 * @param[in]     msg           Wakeup message.
 *
 * @notapi
 */
#define _sdc_wakeup_isr(sdcp, msg)                                          \
  do {                                                                      \
    (void)(sdcp);                                                           \
    (void)(msg);                                                            \
  } while (false)
#endif /* SDC_USE_SYNCHRONIZATION != TRUE */

/**
 * @brief       Common ISR code for successful transfer completion.
 *
 * @param[in]     sdcp          Pointer to the SDC driver instance.
 *
 * @notapi
 */
#define _sdc_isr_complete_code(sdcp)                                        \
  do {                                                                      \
    __cbdrv_invoke_cb_with_transition(sdcp,                                 \
                                      HAL_DRV_STATE_COMPLETE,               \
                                      HAL_DRV_STATE_READY);                 \
    _sdc_wakeup_isr(sdcp, MSG_OK);                                          \
  } while (false)

/**
 * @brief       Common ISR code for transfer errors.
 *
 * @param[in]     sdcp          Pointer to the SDC driver instance.
 *
 * @notapi
 */
#define _sdc_isr_error_code(sdcp)                                           \
  do {                                                                      \
    __cbdrv_invoke_cb_with_transition(sdcp,                                 \
                                      HAL_DRV_STATE_ERROR,                  \
                                      HAL_DRV_STATE_READY);                 \
    _sdc_wakeup_isr(sdcp, MSG_RESET);                                       \
  } while (false)
/** @} */

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief       Type of an SDC bus width configuration.
 */
typedef enum {
  SDC_MODE_1BIT = 0,
  SDC_MODE_4BIT,
  SDC_MODE_8BIT
} sdcbusmode_t;

/**
 * @brief       Type of an SDC bus clock selection.
 */
typedef enum {
  SDC_CLK_25MHz = 0,
  SDC_CLK_50MHz
} sdcbusclk_t;

/**
 * @brief       Type of card mode flags.
 */
typedef uint32_t sdcmode_t;

/**
 * @brief       Type of SDC error flags.
 */
typedef uint32_t sdcflags_t;

/**
 * @brief       Type of SDC driver state.
 */
typedef driver_state_t sdcstate_t;

/**
 * @brief       Type of structure representing an SDC driver.
 */
typedef struct hal_sdc_driver hal_sdc_driver_c;

/**
 * @brief       Type of structure representing an SDC configuration.
 */
typedef struct hal_sdc_config hal_sdc_config_t;

/**
 * @brief       Type of structure representing an SDC driver (legacy).
 */
typedef struct hal_sdc_driver SDCDriver;

/**
 * @brief       Type of structure representing an SDC configuration (legacy).
 */
typedef struct hal_sdc_config SDCConfig;

#define SDC_CONNECTING                     (HAL_DRV_STATE_ERROR + 1U)
#define SDC_DISCONNECTING                  (HAL_DRV_STATE_ERROR + 2U)
#define SDC_READING                        (HAL_DRV_STATE_ERROR + 3U)
#define SDC_WRITING                        (HAL_DRV_STATE_ERROR + 4U)
#define SDC_SYNCING                        (HAL_DRV_STATE_ERROR + 5U)

/* Inclusion of LLD header.*/
#include "hal_sdc_lld.h"

/**
 * @brief       Driver configuration structure.
 */
struct hal_sdc_config {
  /**
   * @brief       Data bus width to be used after card initialization.
   */
  sdcbusmode_t              bus_width;
  /**
   * @brief       Additional divider value applied to the bus clock.
   */
  uint32_t                  slowdown;
  /* End of the mandatory fields.*/
  sdc_lld_config_fields;
#if (defined(SDC_CONFIG_EXT_FIELDS)) || defined (__DOXYGEN__)
  SDC_CONFIG_EXT_FIELDS
#endif /* defined(SDC_CONFIG_EXT_FIELDS) */
};

/**
 * @brief       Type of user-provided SDC configurations.
 */
typedef struct sdc_configurations sdc_configurations_t;

/**
 * @brief       Structure representing user-provided SDC configurations.
 */
struct sdc_configurations {
  /**
   * @brief       Number of configurations in the open array.
   */
  unsigned                  cfgsnum;
  /**
   * @brief       User SDC configurations.
   */
  hal_sdc_config_t          cfgs[];
};

/**
 * @class       hal_sdc_driver_c
 * @extends     hal_cb_driver_c
 * @implements  block_io_i
 *
 * @brief       Class of an SDC driver.
 *
 * @name        Class @p hal_sdc_driver_c structures
 * @{
 */

/**
 * @brief       Type of a SDC driver class.
 */
typedef struct hal_sdc_driver hal_sdc_driver_c;

/**
 * @brief       Class @p hal_sdc_driver_c virtual methods table.
 */
struct hal_sdc_driver_vmt {
  /* From base_object_c.*/
  void (*dispose)(void *ip);
  /* From hal_base_driver_c.*/
  msg_t (*start)(void *ip, const void *config);
  void (*stop)(void *ip);
  const void * (*setcfg)(void *ip, const void *config);
  const void * (*selcfg)(void *ip, unsigned cfgnum);
  /* From hal_cb_driver_c.*/
  void (*oncbset)(void *ip, drv_cb_t cb);
  /* From hal_sdc_driver_c.*/
};

/**
 * @brief       Structure representing a SDC driver class.
 */
struct hal_sdc_driver {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct hal_sdc_driver_vmt *vmt;
  /**
   * @brief       Driver state.
   */
  driver_state_t            state;
  /**
   * @brief       Associated configuration structure.
   */
  const void                *config;
  /**
   * @brief       Driver argument.
   */
  void                      *arg;
#if (HAL_USE_MUTUAL_EXCLUSION == TRUE) || defined (__DOXYGEN__)
  /**
   * @brief       Driver mutex.
   */
  mutex_t                   mutex;
#endif /* HAL_USE_MUTUAL_EXCLUSION == TRUE */
#if (HAL_USE_REGISTRY == TRUE) || defined (__DOXYGEN__)
  /**
   * @brief       Driver identifier.
   */
  unsigned int              id;
  /**
   * @brief       Driver name.
   */
  const char                *name;
  /**
   * @brief       Registry link structure.
   */
  hal_regent_t              regent;
#endif /* HAL_USE_REGISTRY == TRUE */
  /**
   * @brief       Driver callback.
   * @note        Can be @p NULL.
   */
  drv_cb_t                  cb;
  /**
   * @brief       Implemented interface @p block_io_i.
   */
  block_io_i                blk;
  /**
   * @brief       Cached SDC error flags.
   */
  volatile sdcflags_t       errors;
  /**
   * @brief       Various flags describing the attached card.
   */
  sdcmode_t                 cardmode;
  /**
   * @brief       Card RCA value.
   */
  uint32_t                  rca;
  /**
   * @brief       Card capacity expressed in logical blocks.
   */
  uint32_t                  capacity;
  /**
   * @brief       Card CID register image.
   */
  uint32_t                  cid[4];
  /**
   * @brief       Card CSD register image.
   */
  uint32_t                  csd[4];
#if (SDC_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
  /**
   * @brief       Synchronization point for transfer completion.
   */
  thread_reference_t        sync_transfer;
#endif /* SDC_USE_SYNCHRONIZATION == TRUE */
#if (defined(SDC_DRIVER_EXT_FIELDS)) || defined (__DOXYGEN__)
  SDC_DRIVER_EXT_FIELDS
#endif /* defined(SDC_DRIVER_EXT_FIELDS) */
  /* End of the mandatory fields.*/
  sdc_lld_driver_fields;
};
/** @} */

#if !defined(__DOXYGEN__)
#if STM32_SDC_USE_SDMMC1
extern hal_sdc_driver_c SDCD1;
#endif
#if STM32_SDC_USE_SDMMC2
extern hal_sdc_driver_c SDCD2;
#endif
#endif

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  /* Methods of hal_sdc_driver_c.*/
  void *__sdc_objinit_impl(void *ip, const void *vmt);
  void __sdc_dispose_impl(void *ip);
  msg_t __sdc_start_impl(void *ip, const void *config);
  void __sdc_stop_impl(void *ip);
  const void *__sdc_setcfg_impl(void *ip, const void *config);
  const void *__sdc_selcfg_impl(void *ip, unsigned cfgnum);
  void __sdc_oncbset_impl(void *ip, drv_cb_t cb);
  bool sdcConnect(void *ip);
  bool sdcDisconnect(void *ip);
  msg_t sdcStartRead(void *ip, uint32_t startblk, uint8_t *buf, uint32_t n);
  msg_t sdcStartWrite(void *ip, uint32_t startblk, const uint8_t *buf,
                      uint32_t n);
  msg_t sdcStopTransfer(void *ip);
#if (SDC_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
  msg_t sdcSynchronizeS(void *ip, sysinterval_t timeout);
  msg_t sdcSynchronize(void *ip, sysinterval_t timeout);
  bool sdcRead(void *ip, uint32_t startblk, uint8_t *buf, uint32_t n);
  bool sdcWrite(void *ip, uint32_t startblk, const uint8_t *buf, uint32_t n);
#endif /* SDC_USE_SYNCHRONIZATION == TRUE */
  sdcflags_t sdcGetAndClearErrors(void *ip);
  bool sdcSync(void *ip);
  bool sdcGetInfo(void *ip, hal_blk_info_t *bdip);
  bool sdcErase(void *ip, uint32_t startblk, uint32_t endblk);
  /* Regular functions.*/
  void sdcInit(void);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @name        Default constructor of hal_sdc_driver_c
 * @{
 */
/**
 * @brief       Default initialization function of @p hal_sdc_driver_c.
 *
 * @param[out]    self          Pointer to a @p hal_sdc_driver_c instance to be
 *                              initialized.
 * @return                      Pointer to the initialized object.
 *
 * @objinit
 */
CC_FORCE_INLINE
static inline hal_sdc_driver_c *sdcObjectInit(hal_sdc_driver_c *self) {
  extern const struct hal_sdc_driver_vmt __hal_sdc_driver_vmt;

  return __sdc_objinit_impl(self, &__hal_sdc_driver_vmt);
}
/** @} */

/**
 * @name        Inline methods of hal_sdc_driver_c
 * @{
 */
/**
 * @brief       Returns the cached SDC error flags.
 *
 * @param[in,out] ip            Pointer to a @p hal_sdc_driver_c instance.
 * @return                      The current cached error flags.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline sdcflags_t sdcGetErrorsX(void *ip) {
  hal_sdc_driver_c *self = (hal_sdc_driver_c *)ip;
  return self->errors;
}

/**
 * @brief       Gets and clears cached SDC error flags.
 *
 * @param[in,out] ip            Pointer to a @p hal_sdc_driver_c instance.
 * @param[in]     mask          Mask of error flags to be returned and cleared.
 * @return                      The selected pending error flags.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline sdcflags_t sdcGetAndClearErrorsX(void *ip, sdcflags_t mask) {
  hal_sdc_driver_c *self = (hal_sdc_driver_c *)ip;
  sdcflags_t flags;
  syssts_t sts;

  sts = osalSysGetStatusAndLockX();
  flags = self->errors & mask;
  self->errors &= ~mask;
  osalSysRestoreStatusX(sts);

  return flags;
}
/** @} */

#endif /* HAL_USE_SDC == TRUE */

#endif /* HAL_SDC_H */

/** @} */
