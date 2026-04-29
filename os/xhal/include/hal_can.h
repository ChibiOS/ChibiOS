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
 * @file        hal_can.h
 * @brief       Generated CAN Driver header.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  HAL_CAN
 * @brief       CAN Driver macros and structures.
 * @{
 */

#ifndef HAL_CAN_H
#define HAL_CAN_H

#include "hal_cb_driver.h"

#if (HAL_USE_CAN == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    CAN status flags
 * @{
 */
/**
 * @brief       Error rate warning.
 */
#define CAN_LIMIT_WARNING                   1U

/**
 * @brief       Error rate limit reached.
 */
#define CAN_LIMIT_ERROR                     2U

/**
 * @brief       Bus-off condition reached.
 */
#define CAN_BUS_OFF_ERROR                   4U

/**
 * @brief       Framing error on the CAN bus.
 */
#define CAN_FRAMING_ERROR                   8U

/**
 * @brief       Receive queue overflow.
 */
#define CAN_OVERFLOW_ERROR                  16U
/** @} */

/**
 * @name    CAN mailbox constants
 * @{
 */
/**
 * @brief       Special mailbox identifier.
 */
#define CAN_ANY_MAILBOX                     0U
/** @} */

/**
 * @name    CAN event flags
 * @{
 */
/**
 * @brief       One or more frames became available.
 */
#define CAN_EVENT_RX                        (1U << 0)

/**
 * @brief       One or more transmit mailboxes became available.
 */
#define CAN_EVENT_TX                        (1U << 1)

/**
 * @brief       A CAN error condition occurred.
 */
#define CAN_EVENT_ERROR                     (1U << 2)

/**
 * @brief       The driver entered sleep state.
 */
#define CAN_EVENT_SLEEP                     (1U << 3)

/**
 * @brief       The driver exited sleep state.
 */
#define CAN_EVENT_WAKEUP                    (1U << 4)

/**
 * @brief       All CAN event flags.
 */
#define CAN_EVENT_ALL                       (CAN_EVENT_RX | CAN_EVENT_TX | CAN_EVENT_ERROR | CAN_EVENT_SLEEP | CAN_EVENT_WAKEUP)
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
/**
 * @brief       Support for synchronous CAN transmit and receive APIs.
 */
#if !defined(CAN_USE_SYNCHRONIZATION) || defined(__DOXYGEN__)
#define CAN_USE_SYNCHRONIZATION             TRUE
#endif

/**
 * @brief       Support for CAN sleep mode APIs.
 */
#if !defined(CAN_USE_SLEEP_MODE) || defined(__DOXYGEN__)
#define CAN_USE_SLEEP_MODE                  TRUE
#endif

/**
 * @brief       Support for CAN user configurations.
 * @note        When enabled the user must provide a variable named @p
 *              can_configurations of type @p can_configurations_t.
 */
#if !defined(CAN_USE_CONFIGURATIONS) || defined(__DOXYGEN__)
#define CAN_USE_CONFIGURATIONS              FALSE
#endif
/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/* Checks on CAN_USE_SYNCHRONIZATION configuration.*/
#if (CAN_USE_SYNCHRONIZATION != FALSE) && (CAN_USE_SYNCHRONIZATION != TRUE)
#error "invalid CAN_USE_SYNCHRONIZATION value"
#endif

/* Checks on CAN_USE_SLEEP_MODE configuration.*/
#if (CAN_USE_SLEEP_MODE != FALSE) && (CAN_USE_SLEEP_MODE != TRUE)
#error "invalid CAN_USE_SLEEP_MODE value"
#endif

/* Checks on CAN_USE_CONFIGURATIONS configuration.*/
#if (CAN_USE_CONFIGURATIONS != FALSE) && (CAN_USE_CONFIGURATIONS != TRUE)
#error "invalid CAN_USE_CONFIGURATIONS value"
#endif

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @name    Macro functions
 * @{
 */
/**
 * @brief       Converts a mailbox index to a bit mask.
 *
 * @param[in]     mbx           Mailbox index.
 * @return                      The mailbox mask.
 *
 * @api
 */
#define CAN_MAILBOX_TO_MASK(mbx)                                            \
  ((canmbxmask_t)1U << ((mbx) - 1U))

/**
 * @brief       Legacy name for @p canTransmitTimeout().
 *
 * @param[in]     canp          Pointer to the CAN driver instance.
 * @param[in]     mailbox       Mailbox number.
 * @param[in]     ctfp          Frame to transmit.
 * @param[in]     timeout       Timeout interval.
 *
 * @api
 */
#define canTransmit(canp, mailbox, ctfp, timeout)                           \
  canTransmitTimeout(canp, mailbox, ctfp, timeout)

/**
 * @brief       Legacy name for @p canReceiveTimeout().
 *
 * @param[in]     canp          Pointer to the CAN driver instance.
 * @param[in]     mailbox       Mailbox number.
 * @param[out]    crfp          Frame receive buffer.
 * @param[in]     timeout       Timeout interval.
 *
 * @api
 */
#define canReceive(canp, mailbox, crfp, timeout)                            \
  canReceiveTimeout(canp, mailbox, crfp, timeout)
/** @} */

/**
 * @name    Low level driver helper macros
 * @{
 */
#if (CAN_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
/**
 * @brief       Common ISR code, TX mailbox empty event.
 *
 * @param[in,out] canp          Pointer to the CAN driver instance.
 * @param[in]     flags         Mailbox and transmit error flags.
 *
 * @notapi
 */
#define _can_tx_empty_isr(canp, flags)                                      \
  do {                                                                      \
    (canp)->tx_mailbox_mask |= (canmbxmask_t)((flags) & 0xFFFFU);           \
    (canp)->tx_error_mask   |= (canmbxmask_t)(((flags) >> 16U) & 0xFFFFU);  \
    (canp)->events          |= CAN_EVENT_TX;                                \
    osalSysLockFromISR();                                                   \
    osalThreadDequeueAllI(&(canp)->txqueue, MSG_OK);                        \
    osalSysUnlockFromISR();                                                 \
    __cbdrv_invoke_cb(canp);                                                \
  } while (false)

/**
 * @brief       Common ISR code, RX mailbox full event.
 *
 * @param[in,out] canp          Pointer to the CAN driver instance.
 * @param[in]     flags         Mailbox flags.
 *
 * @notapi
 */
#define _can_rx_full_isr(canp, flags)                                       \
  do {                                                                      \
    (canp)->rx_mailbox_mask |= (canmbxmask_t)(flags);                       \
    (canp)->events          |= CAN_EVENT_RX;                                \
    osalSysLockFromISR();                                                   \
    osalThreadDequeueAllI(&(canp)->rxqueue, MSG_OK);                        \
    osalSysUnlockFromISR();                                                 \
    __cbdrv_invoke_cb(canp);                                                \
  } while (false)

#else

/**
 * @brief       Common ISR code, TX mailbox empty event.
 *
 * @param[in,out] canp          Pointer to the CAN driver instance.
 * @param[in]     flags         Mailbox and transmit error flags.
 *
 * @notapi
 */
#define _can_tx_empty_isr(canp, flags)                                      \
  do {                                                                      \
    (canp)->tx_mailbox_mask |= (canmbxmask_t)((flags) & 0xFFFFU);           \
    (canp)->tx_error_mask   |= (canmbxmask_t)(((flags) >> 16U) & 0xFFFFU);  \
    (canp)->events          |= CAN_EVENT_TX;                                \
    __cbdrv_invoke_cb(canp);                                                \
  } while (false)

/**
 * @brief       Common ISR code, RX mailbox full event.
 *
 * @param[in,out] canp          Pointer to the CAN driver instance.
 * @param[in]     flags         Mailbox flags.
 *
 * @notapi
 */
#define _can_rx_full_isr(canp, flags)                                       \
  do {                                                                      \
    (canp)->rx_mailbox_mask |= (canmbxmask_t)(flags);                       \
    (canp)->events          |= CAN_EVENT_RX;                                \
    __cbdrv_invoke_cb(canp);                                                \
  } while (false)
#endif /* CAN_USE_SYNCHRONIZATION == TRUE */

/**
 * @brief       Common ISR code, CAN wakeup event.
 *
 * @param[in,out] canp          Pointer to the CAN driver instance.
 *
 * @notapi
 */
#define _can_wakeup_isr(canp)                                               \
  do {                                                                      \
    (canp)->state  = HAL_DRV_STATE_READY;                                   \
    (canp)->events |= CAN_EVENT_WAKEUP;                                     \
    __cbdrv_invoke_cb(canp);                                                \
  } while (false)

/**
 * @brief       Common ISR code, CAN error event.
 *
 * @param[in,out] canp          Pointer to the CAN driver instance.
 * @param[in]     flags         Error flags.
 *
 * @notapi
 */
#define _can_error_isr(canp, flags)                                         \
  do {                                                                      \
    (canp)->errors |= (canerror_t)(flags);                                  \
    (canp)->events |= CAN_EVENT_ERROR;                                      \
    __cbdrv_invoke_cb(canp);                                                \
  } while (false)
/** @} */

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief       Type of a CAN mailbox index.
 */
typedef uint32_t canmbx_t;

/**
 * @brief       Type of CAN mailbox masks.
 */
typedef uint32_t canmbxmask_t;

/**
 * @brief       Type of CAN event flags.
 */
typedef uint32_t can_events_t;

/**
 * @brief       Type of CAN error flags.
 */
typedef uint32_t canerror_t;

/**
 * @brief       Type of CAN driver state.
 */
typedef driver_state_t canstate_t;

/**
 * @brief       Type of structure representing a CAN driver.
 */
typedef struct hal_can_driver hal_can_driver_c;

/**
 * @brief       Type of structure representing a CAN configuration.
 */
typedef struct hal_can_config hal_can_config_t;

/**
 * @brief       Type of structure representing a CAN driver (legacy).
 */
typedef struct hal_can_driver CANDriver;

/**
 * @brief       Type of structure representing a CAN configuration (legacy).
 */
typedef struct hal_can_config CANConfig;

#define CAN_SLEEP                          (HAL_DRV_STATE_ERROR + 1U)

/* Inclusion of LLD header.*/
#include "hal_can_lld.h"

/**
 * @brief       Driver configuration structure.
 */
struct hal_can_config {
  /* End of the mandatory fields.*/
  can_lld_config_fields;
#if (defined(CAN_CONFIG_EXT_FIELDS)) || defined (__DOXYGEN__)
  CAN_CONFIG_EXT_FIELDS
#endif /* defined(CAN_CONFIG_EXT_FIELDS) */
};

/**
 * @brief       Type of user-provided CAN configurations.
 */
typedef struct can_configurations can_configurations_t;

/**
 * @brief       Structure representing user-provided CAN configurations.
 */
struct can_configurations {
  /**
   * @brief       Number of configurations in the open array.
   */
  unsigned                  cfgsnum;
  /**
   * @brief       User CAN configurations.
   */
  hal_can_config_t          cfgs[];
};

/**
 * @class       hal_can_driver_c
 * @extends     hal_cb_driver_c
 *
 * @brief       Class of a CAN driver.
 *
 * @name        Class @p hal_can_driver_c structures
 * @{
 */

/**
 * @brief       Type of a CAN driver class.
 */
typedef struct hal_can_driver hal_can_driver_c;

/**
 * @brief       Class @p hal_can_driver_c virtual methods table.
 */
struct hal_can_driver_vmt {
  /* From base_object_c.*/
  void (*dispose)(void *ip);
  /* From hal_base_driver_c.*/
  msg_t (*start)(void *ip, const void *config);
  void (*stop)(void *ip);
  const void * (*setcfg)(void *ip, const void *config);
  const void * (*selcfg)(void *ip, unsigned cfgnum);
  /* From hal_cb_driver_c.*/
  void (*oncbset)(void *ip, drv_cb_t cb);
  /* From hal_can_driver_c.*/
};

/**
 * @brief       Structure representing a CAN driver class.
 */
struct hal_can_driver {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct hal_can_driver_vmt *vmt;
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
   * @brief       Cached CAN event flags.
   */
  volatile can_events_t     events;
  /**
   * @brief       Cached RX mailbox flags.
   */
  volatile canmbxmask_t     rx_mailbox_mask;
  /**
   * @brief       Cached TX mailbox flags.
   */
  volatile canmbxmask_t     tx_mailbox_mask;
  /**
   * @brief       Cached TX error mailbox flags.
   */
  volatile canmbxmask_t     tx_error_mask;
  /**
   * @brief       Cached CAN error flags.
   */
  volatile canerror_t       errors;
#if (CAN_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
  /**
   * @brief       Transmission threads queue.
   */
  threads_queue_t           txqueue;
  /**
   * @brief       Receive threads queue.
   */
  threads_queue_t           rxqueue;
#endif /* CAN_USE_SYNCHRONIZATION == TRUE */
#if (defined(CAN_DRIVER_EXT_FIELDS)) || defined (__DOXYGEN__)
  CAN_DRIVER_EXT_FIELDS
#endif /* defined(CAN_DRIVER_EXT_FIELDS) */
  /* End of the mandatory fields.*/
  can_lld_driver_fields;
};
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  /* Methods of hal_can_driver_c.*/
  void *__can_objinit_impl(void *ip, const void *vmt);
  void __can_dispose_impl(void *ip);
  msg_t __can_start_impl(void *ip, const void *config);
  void __can_stop_impl(void *ip);
  const void *__can_setcfg_impl(void *ip, const void *config);
  const void *__can_selcfg_impl(void *ip, unsigned cfgnum);
  void __can_oncbset_impl(void *ip, drv_cb_t cb);
  bool canTryTransmitI(void *ip, canmbx_t mailbox, const CANTxFrame *ctfp);
  bool canTryReceiveI(void *ip, canmbx_t mailbox, CANRxFrame *crfp);
  void canTryAbortX(void *ip, canmbx_t mailbox);
#if (CAN_USE_SYNCHRONIZATION == TRUE) || defined (__DOXYGEN__)
  msg_t canTransmitTimeout(void *ip, canmbx_t mailbox, const CANTxFrame *ctfp,
                           sysinterval_t timeout);
  msg_t canReceiveTimeout(void *ip, canmbx_t mailbox, CANRxFrame *crfp,
                          sysinterval_t timeout);
#endif /* CAN_USE_SYNCHRONIZATION == TRUE */
#if (CAN_USE_SLEEP_MODE == TRUE) || defined (__DOXYGEN__)
  void canSleep(void *ip);
  void canWakeup(void *ip);
#endif /* CAN_USE_SLEEP_MODE == TRUE */
  /* Regular functions.*/
  void canInit(void);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @name        Default constructor of hal_can_driver_c
 * @{
 */
/**
 * @brief       Default initialization function of @p hal_can_driver_c.
 *
 * @param[out]    self          Pointer to a @p hal_can_driver_c instance to be
 *                              initialized.
 * @return                      Pointer to the initialized object.
 *
 * @objinit
 */
CC_FORCE_INLINE
static inline hal_can_driver_c *canObjectInit(hal_can_driver_c *self) {
  extern const struct hal_can_driver_vmt __hal_can_driver_vmt;

  return __can_objinit_impl(self, &__hal_can_driver_vmt);
}
/** @} */

/**
 * @name        Inline methods of hal_can_driver_c
 * @{
 */
/**
 * @brief       Returns the cached CAN event flags.
 *
 * @param[in,out] ip            Pointer to a @p hal_can_driver_c instance.
 * @return                      The current cached event flags.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline can_events_t canGetEventsX(void *ip) {
  hal_can_driver_c *self = (hal_can_driver_c *)ip;
  return self->events;
}

/**
 * @brief       Gets and clears cached CAN event flags.
 *
 * @param[in,out] ip            Pointer to a @p hal_can_driver_c instance.
 * @param[in]     mask          Event flags to get and clear.
 * @return                      The selected pending event flags.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline can_events_t canGetAndClearEventsX(void *ip, can_events_t mask) {
  hal_can_driver_c *self = (hal_can_driver_c *)ip;
  can_events_t events;
  syssts_t sts;

  sts = osalSysGetStatusAndLockX();
  events = self->events & mask;
  self->events &= ~mask;
  osalSysRestoreStatusX(sts);

  return events;
}

/**
 * @brief       Gets and clears cached RX mailbox flags.
 *
 * @param[in,out] ip            Pointer to a @p hal_can_driver_c instance.
 * @return                      The pending RX mailbox flags.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline canmbxmask_t canGetAndClearRxMailboxMaskX(void *ip) {
  hal_can_driver_c *self = (hal_can_driver_c *)ip;
  canmbxmask_t mask;
  syssts_t sts;

  sts = osalSysGetStatusAndLockX();
  mask = self->rx_mailbox_mask;
  self->rx_mailbox_mask = 0U;
  osalSysRestoreStatusX(sts);

  return mask;
}

/**
 * @brief       Gets and clears cached TX mailbox flags.
 *
 * @param[in,out] ip            Pointer to a @p hal_can_driver_c instance.
 * @return                      The pending TX mailbox flags.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline canmbxmask_t canGetAndClearTxMailboxMaskX(void *ip) {
  hal_can_driver_c *self = (hal_can_driver_c *)ip;
  canmbxmask_t mask;
  syssts_t sts;

  sts = osalSysGetStatusAndLockX();
  mask = self->tx_mailbox_mask;
  self->tx_mailbox_mask = 0U;
  osalSysRestoreStatusX(sts);

  return mask;
}

/**
 * @brief       Gets and clears cached TX error mailbox flags.
 *
 * @param[in,out] ip            Pointer to a @p hal_can_driver_c instance.
 * @return                      The pending TX error mailbox flags.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline canmbxmask_t canGetAndClearTxErrorMaskX(void *ip) {
  hal_can_driver_c *self = (hal_can_driver_c *)ip;
  canmbxmask_t mask;
  syssts_t sts;

  sts = osalSysGetStatusAndLockX();
  mask = self->tx_error_mask;
  self->tx_error_mask = 0U;
  osalSysRestoreStatusX(sts);

  return mask;
}

/**
 * @brief       Gets and clears cached CAN error flags.
 *
 * @param[in,out] ip            Pointer to a @p hal_can_driver_c instance.
 * @return                      The pending CAN error flags.
 *
 * @xclass
 */
CC_FORCE_INLINE
static inline canerror_t canGetAndClearErrorsX(void *ip) {
  hal_can_driver_c *self = (hal_can_driver_c *)ip;
  canerror_t errors;
  syssts_t sts;

  sts = osalSysGetStatusAndLockX();
  errors = self->errors;
  self->errors = 0U;
  osalSysRestoreStatusX(sts);

  return errors;
}
/** @} */

#endif /* HAL_USE_CAN == TRUE */

#endif /* HAL_CAN_H */

/** @} */
