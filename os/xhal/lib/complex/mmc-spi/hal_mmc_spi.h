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
 * @file    hal_mmc_spi.h
 * @brief   MMC over SPI driver header.
 *
 * @addtogroup HAL_MMC_SPI
 * @{
 */

#ifndef HAL_MMC_SPI_H
#define HAL_MMC_SPI_H

#include "hal.h"

#if !defined(MMC_SPI_USE_MODULE) || defined(__DOXYGEN__)
#define MMC_SPI_USE_MODULE               TRUE
#endif

#if (MMC_SPI_USE_MODULE == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

#define MMC_SPI_CMD0_RETRY               10U
#define MMC_SPI_CMD1_RETRY               100U
#define MMC_SPI_ACMD41_RETRY             100U
#define MMC_SPI_WAIT_DATA                10000U

/**
 * @brief   Size of the driver internal buffer.
 * @note    The buffer is meant to be non-cacheable on platforms with data
 *          cache.
 */
#define MMC_SPI_BUFFER_SIZE              16U

/**
 * @brief   MMC/SD block size in bytes.
 */
#define MMC_SPI_BLOCK_SIZE               512U

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
#if !defined(MMC_SPI_IDLE_TIMEOUT_MS) || defined(__DOXYGEN__)
#define MMC_SPI_IDLE_TIMEOUT_MS          1000
#endif

#if !defined(MMC_SPI_USE_MUTUAL_EXCLUSION) || defined(__DOXYGEN__)
#define MMC_SPI_USE_MUTUAL_EXCLUSION     TRUE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if (HAL_USE_SPI == FALSE) || (SPI_USE_SYNCHRONIZATION == FALSE)
#error "MMC_SPI driver requires HAL_USE_SPI and SPI_USE_SYNCHRONIZATION"
#endif

#if (MMC_SPI_USE_MUTUAL_EXCLUSION != FALSE) &&                         \
    (MMC_SPI_USE_MUTUAL_EXCLUSION != TRUE)
#error "invalid MMC_SPI_USE_MUTUAL_EXCLUSION value"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   MMC-SPI protocol state.
 */
typedef enum {
  MMC_SPI_MEDIA_STOP = 0,
  MMC_SPI_MEDIA_ACTIVE = 1,
  MMC_SPI_MEDIA_CONNECTING = 2,
  MMC_SPI_MEDIA_READY = 3,
  MMC_SPI_MEDIA_READING = 4,
  MMC_SPI_MEDIA_WRITING = 5,
  MMC_SPI_MEDIA_DISCONNECTING = 6,
  MMC_SPI_MEDIA_SYNCING = 7
} mmc_spi_state_t;

/**
 * @brief   Type of a media status callback.
 */
typedef bool (*mmc_spi_status_cb_t)(void *arg);

/**
 * @brief   Type of structure representing an MMC-SPI driver.
 */
typedef struct hal_mmc_spi_driver hal_mmc_spi_driver_c;

/**
 * @brief   Type of structure representing an MMC-SPI configuration.
 */
typedef struct hal_mmc_spi_config hal_mmc_spi_config_t;

/**
 * @brief   Convenience alias for the driver type.
 */
typedef struct hal_mmc_spi_driver MMCSPIDriver;

/**
 * @brief   Convenience alias for the configuration type.
 */
typedef struct hal_mmc_spi_config MMCSPIConfig;

/**
 * @brief   Driver configuration structure.
 */
struct hal_mmc_spi_config {
  /**
   * @brief SPI driver associated to this MMC driver.
   */
  hal_spi_driver_c         *spip;
  /**
   * @brief SPI low speed configuration used during initialization.
   */
  const hal_spi_config_t   *lscfg;
  /**
   * @brief SPI high speed configuration used during transfers.
   */
  const hal_spi_config_t   *hscfg;
  /**
   * @brief Card insertion callback, must not be @p NULL.
   */
  mmc_spi_status_cb_t      is_inserted;
  /**
   * @brief Write-protect callback, can be @p NULL if unsupported.
   */
  mmc_spi_status_cb_t      is_write_protected;
  /**
   * @brief Opaque callback argument.
   */
  void                     *arg;
};

/**
 * @brief   Class @p hal_mmc_spi_driver_c virtual methods table.
 */
struct hal_mmc_spi_driver_vmt {
  /* From base_object_c.*/
  void (*dispose)(void *ip);
  /* From hal_base_driver_c.*/
  msg_t (*start)(void *ip, const void *config);
  void (*stop)(void *ip);
  const void * (*setcfg)(void *ip, const void *config);
  const void * (*selcfg)(void *ip, unsigned cfgnum);
};

/**
 * @brief   Structure representing an MMC-SPI driver.
 */
struct hal_mmc_spi_driver {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct hal_mmc_spi_driver_vmt *vmt;
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
#if (HAL_USE_MUTUAL_EXCLUSION == TRUE) || defined(__DOXYGEN__)
  /**
   * @brief       Driver mutex.
   */
  mutex_t                   mutex;
#endif
#if (HAL_USE_REGISTRY == TRUE) || defined(__DOXYGEN__)
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
#endif
  /**
   * @brief       Implemented interface @p block_io_i.
   */
  block_io_i                blk;
  /**
   * @brief       MMC/SD protocol state.
   */
  mmc_spi_state_t           media_state;
  /**
   * @brief       Card uses block instead of byte addressing.
   */
  bool                      block_addresses;
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
  /**
   * @brief       Pointer to a work buffer of size @p MMC_SPI_BUFFER_SIZE.
   */
  uint8_t                   *buffer;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Returns a pointer to the configuration structure.
 *
 * @param[in] mmcp      pointer to the @p MMCSPIDriver object
 * @return              The current configuration pointer.
 *
 * @notapi
 */
#define mmcSpiGetConfigX(mmcp)                                              \
  ((const hal_mmc_spi_config_t *)((mmcp)->config))

/**
 * @brief   Returns the card insertion status.
 *
 * @param[in] mmcp      pointer to the @p MMCSPIDriver object
 * @return              The card state.
 *
 * @api
 */
#define mmcSpiIsCardInserted(mmcp)                                          \
  (mmcSpiGetConfigX(mmcp)->is_inserted(mmcSpiGetConfigX(mmcp)->arg))

/**
 * @brief   Returns the write protect status.
 *
 * @param[in] mmcp      pointer to the @p MMCSPIDriver object
 * @return              The write protect state.
 *
 * @api
 */
#define mmcSpiIsWriteProtected(mmcp)                                        \
  (((mmcSpiGetConfigX(mmcp)->is_write_protected) != NULL) ?                 \
   mmcSpiGetConfigX(mmcp)->is_write_protected(mmcSpiGetConfigX(mmcp)->arg) :\
   false)

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void *__mmcspi_objinit_impl(void *ip, const void *vmt);
  void __mmcspi_dispose_impl(void *ip);
  msg_t __mmcspi_start_impl(void *ip, const void *config);
  void __mmcspi_stop_impl(void *ip);
  const void *__mmcspi_setcfg_impl(void *ip, const void *config);
  const void *__mmcspi_selcfg_impl(void *ip, unsigned cfgnum);
  bool mmcSpiConnect(MMCSPIDriver *mmcp);
  bool mmcSpiDisconnect(MMCSPIDriver *mmcp);
  bool mmcSpiRead(MMCSPIDriver *mmcp, uint32_t startblk,
                  uint8_t *buffer, uint32_t n);
  bool mmcSpiWrite(MMCSPIDriver *mmcp, uint32_t startblk,
                   const uint8_t *buffer, uint32_t n);
  bool mmcSpiSync(MMCSPIDriver *mmcp);
  bool mmcSpiGetInfo(MMCSPIDriver *mmcp, hal_blk_info_t *bdip);
  bool mmcSpiErase(MMCSPIDriver *mmcp, uint32_t startblk, uint32_t endblk);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Default initialization function of @p hal_mmc_spi_driver_c.
 *
 * @param[out] self      pointer to the instance to initialize.
 * @param[in]  buffer    work buffer.
 * @return               The initialized object.
 *
 * @objinit
 */
CC_FORCE_INLINE
static inline hal_mmc_spi_driver_c *mmcSpiObjectInit(hal_mmc_spi_driver_c *self,
                                                     uint8_t *buffer) {
  extern const struct hal_mmc_spi_driver_vmt __hal_mmc_spi_driver_vmt;

  self = __mmcspi_objinit_impl(self, &__hal_mmc_spi_driver_vmt);
  self->buffer = buffer;

  return self;
}

#endif /* MMC_SPI_USE_MODULE == TRUE */

#endif /* HAL_MMC_SPI_H */

/** @} */
