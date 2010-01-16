/*
    ChibiOS/RT - Copyright (C) 2010 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program. If not, see <http://www.gnu.org/licenses/>.

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

/**
 * @file STM32/can_lld.h
 * @brief STM32 CAN subsystem low level driver header.
 * @addtogroup STM32_CAN
 * @{
 */

#ifndef _CAN_LLD_H_
#define _CAN_LLD_H_

#if CH_HAL_USE_CAN || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/*
 * The following macros from the ST header file are replaced with better
 * equivalents.
 */
#undef CAN_BTR_BRP
#undef CAN_BTR_TS1
#undef CAN_BTR_TS2
#undef CAN_BTR_SJW

/**
 * @brief This switch defines whether the driver implementation supports
 *        a low power switch mode with automatic an wakeup feature.
 */
#define CAN_SUPPORTS_SLEEP          TRUE

/**
 * @brief Minimum number of CAN filters.
 */
#if defined(STM32F10X_CL) || defined(__DOXYGEN__)
#define CAN_MAX_FILTERS             28
#else
#define CAN_MAX_FILTERS             14
#endif

#define CAN_BTR_BRP(n)              (n)         /**< @brief BRP field macro.*/
#define CAN_BTR_TS1(n)              ((n) << 16) /**< @brief TS1 field macro.*/
#define CAN_BTR_TS2(n)              ((n) << 20) /**< @brief TS2 field macro.*/
#define CAN_BTR_SJW(n)              ((n) << 24) /**< @brief SJW field macro.*/

#define CAN_IDE_STD                 0           /**< @brief Standard id.    */
#define CAN_IDE_EXT                 1           /**< @brief Extended id.    */

#define CAN_RTR_DATA                0           /**< @brief Data frame.     */
#define CAN_RTR_REMOTE              1           /**< @brief Remote frame.   */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief CAN1 driver enable switch.
 * @details If set to @p TRUE the support for ADC1 is included.
 * @note The default is @p TRUE.
 */
#if !defined(USE_STM32_CAN1) || defined(__DOXYGEN__)
#define USE_STM32_CAN1              TRUE
#endif

/**
 * @brief CAN1 interrupt priority level setting.
 * @note @p BASEPRI_KERNEL >= @p STM32_SPI1_IRQ_PRIORITY > @p PRIORITY_PENDSV.
 */
#if !defined(STM32_CAN1_IRQ_PRIORITY) || defined(__DOXYGEN__)
#define STM32_CAN1_IRQ_PRIORITY     0xB0
#endif

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if CAN_USE_SLEEP_MODE && !CAN_SUPPORTS_SLEEP
#error "CAN sleep mode not supported in this architecture"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief CAN status flags.
 */
typedef uint32_t canstatus_t;

/**
 * @brief CAN transmission frame.
 * @note Accessing the frame data as word16 or word32 is not portable because
 *       machine data endianness, it can be still useful for a quick filling.
 */
typedef struct {
  struct {
    uint8_t                 cf_DLC:4;       /**< @brief Data length.        */
    uint8_t                 cf_RTR:1;       /**< @brief Frame type.         */
    uint8_t                 cf_IDE:1;       /**< @brief Identifier type.    */
  };
  union {
    struct {
      uint32_t              cf_SID:11;      /**< @brief Standard identifier.*/
    };
    struct {
      uint32_t              cf_EID:29;      /**< @brief Extended identifier.*/
    };
  };
  union {
    uint8_t                 cf_data8[8];    /**< @brief Frame data.         */
    uint16_t                cf_data16[4];   /**< @brief Frame data.         */
    uint32_t                cf_data32[2];   /**< @brief Frame data.         */
  };
} CANTxFrame;

/**
 * @brief CAN received frame.
 * @note Accessing the frame data as word16 or word32 is not portable because
 *       machine data endianness, it can be still useful for a quick filling.
 */
typedef struct {
  struct {
    uint8_t                 cf_FMI;         /**< @brief Filter id.          */
    uint16_t                cf_TIME;        /**< @brief Time stamp.         */
  };
  struct {
    uint8_t                 cf_DLC:4;       /**< @brief Data length.        */
    uint8_t                 cf_RTR:1;       /**< @brief Frame type.         */
    uint8_t                 cf_IDE:1;       /**< @brief Identifier type.    */
  };
  union {
    struct {
      uint32_t              cf_SID:11;      /**< @brief Standard identifier.*/
    };
    struct {
      uint32_t              cf_EID:29;      /**< @brief Extended identifier.*/
    };
  };
  union {
    uint8_t                 cf_data8[8];    /**< @brief Frame data.         */
    uint16_t                cf_data16[4];   /**< @brief Frame data.         */
    uint32_t                cf_data32[2];   /**< @brief Frame data.         */
  };
} CANRxFrame;

/**
 * @brief CAN filter.
 * @note Refer to the STM32 reference manual for info about filters.
 */
typedef struct {
  /**
   * @brief Filter mode.
   * @note This bit represent the CAN_FM1R register bit associated to this
   *       filter (0=mask mode, 1=list mode).
   */
  uint32_t                  cf_mode:1;
  /**
   * @brief Filter sclae.
   * @note This bit represent the CAN_FS1R register bit associated to this
   *       filter (0=16 bits mode, 1=32 bits mode).
   */
  uint32_t                  cf_scale:1;
  /**
   * @brief Filter mode.
   * @note This bit represent the CAN_FFA1R register bit associated to this
   *       filter, must be set to zero in this version of the driver.
   */
  uint32_t                  cf_assignment:1;
  /**
   * @brief Filter register 1 (identifier).
   */
  uint32_t                  cf_register1;
  /**
   * @brief Filter register 2 (mask/identifier depending on cf_mode=0/1).
   */
  uint32_t                  cf_register2;
} CANFilter;

/**
 * @brief Driver configuration structure.
 */
typedef struct {
  /**
   * @brief CAN MCR register initialization data.
   * @note Some bits in this register are enforced by the driver regardless
   *       their status in this field.
   */
  uint32_t                  cc_mcr;
  /**
   * @brief CAN BTR register initialization data.
   * @note Some bits in this register are enforced by the driver regardless
   *       their status in this field.
   */
  uint32_t                  cc_btr;
  /**
   * @brief Number of elements into the filters array.
   * @note By setting this field to zero a default filter is enabled that
   *       allows all frames, this should be adequate  for simple applications.
   */
  uint32_t                  cc_num;
  /**
   * @brief Pointer to an array of @p CANFilter structures.
   * @note This field can be set to @p NULL if the field @p cc_num is set to
   *       zero.
   */
  const CANFilter           *cc_filters;
} CANConfig;

/**
 * @brief Structure representing an CAN driver.
 */
typedef struct {
  /**
   * @brief Driver state.
   */
  canstate_t                cd_state;
  /**
   * @brief Current configuration data.
   */
  const CANConfig           *cd_config;
  /**
   * @brief Transmission queue semaphore.
   */
  Semaphore                 cd_txsem;
  /**
   * @brief Receive queue semaphore.
   */
  Semaphore                 cd_rxsem;
  /**
   * @brief One or more frames become available.
   * @note After broadcasting this event it will not be broadcasted again
   *       until the received frames queue has been completely emptied. It
   *       is <b>not</b> broadcasted for each received frame. It is
   *       responsibility of the application to empty the queue by repeatedly
   *       invoking @p chReceive() when listening to this event. This behavior
   *       minimizes the interrupt served by the system because CAN traffic.
   */
  EventSource               cd_rxfull_event;
  /**
   * @brief One or more transmission slots become available.
   */
  EventSource               cd_txempty_event;
  /**
   * @brief A CAN bus error happened.
   */
  EventSource               cd_error_event;
  /**
   * @brief Error flags set when an error event is broadcasted.
   */
  canstatus_t               cd_status;
#if CAN_USE_SLEEP_MODE || defined (__DOXYGEN__)
  /**
   * @brief Entering sleep state event.
   */
  EventSource               cd_sleep_event;
  /**
   * @brief Exiting sleep state event.
   */
  EventSource               cd_wakeup_event;
#endif /* CAN_USE_SLEEP_MODE */
  /* End of the mandatory fields.*/
  /**
   * @brief Pointer to the CAN registers.
   */
  CAN_TypeDef               *cd_can;
} CANDriver;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

/** @cond never*/
#if USE_STM32_CAN1
extern CANDriver CAND1;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void can_lld_init(void);
  void can_lld_start(CANDriver *canp);
  void can_lld_stop(CANDriver *canp);
  bool_t can_lld_can_transmit(CANDriver *canp);
  void can_lld_transmit(CANDriver *canp, const CANTxFrame *crfp);
  bool_t can_lld_can_receive(CANDriver *canp);
  void can_lld_receive(CANDriver *canp, CANRxFrame *ctfp);
#if CAN_USE_SLEEP_MODE
  void can_lld_sleep(CANDriver *canp);
  void can_lld_wakeup(CANDriver *canp);
#endif /* CAN_USE_SLEEP_MODE */
#ifdef __cplusplus
}
#endif
/** @endcond*/

#endif /* CH_HAL_USE_CAN */

#endif /* _CAN_LLD_H_ */

/** @} */
