/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    chtssi.h
 * @brief   tssi module macros and structures.
 *
 * @addtogroup TSSI
 * @{
 */

#ifndef CHTSSI_H
#define CHTSSI_H

#include "ch.h"
#include "ccportab.h"

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/* TSSI interface version. This code is returned also at run time by
   calling the internal service TS_HND_VERSION.*/
#define TSSI_VERSION          0x01000000    /* 00 major, 000 minor, 000 build.*/

/* Service registry errors as returned by smc.*/
#define SMC_SVC_OK            MSG_OK        /* No error.*/
#define SMC_SVC_INTR          (msg_t)-1     /* Service interrupted ( == MSG_TIMEOUT).*/
#define SMC_SVC_NOENT         (msg_t)-2     /* No existent service.*/
#define SMC_SVC_INVALID       (msg_t)-3     /* Invalid service parameter(s).*/
#define SMC_SVC_BADH          (msg_t)-4     /* Invalid service handle.*/
#define SMC_SVC_EXIST         (msg_t)-5     /* Service already exists.*/
#define SMC_SVC_NHND          (msg_t)-6     /* No more services or
                                               service resources.*/
#define SMC_SVC_BUSY          (msg_t)-7     /* Service busy.*/

/* Special trusted service handles.*/
#define TS_HND_TRAMP          ((ts_state_t *)0)  /* Trampoline service handle.*/
#define TS_HND_DISCOVERY      ((ts_state_t *)1)  /* Discovery service handle.*/
#define TS_HND_STQRY          ((ts_state_t *)2)  /* Query status service handle.*/
#define TS_HND_IDLE           ((ts_state_t *)3)  /* Idle service handle.*/
#define TS_HND_VERSION        ((ts_state_t *)4)  /* Get version service handle.*/

/* Services events event mask.*/
#define EVT_DAEMON_REQ_ATN    EVENT_MASK(0)

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    TSSI module settings.
 * @{
 */

/**
 * @brief   Max number of services.
 */
#define TS_MAX_SVCS           64

/**
 * @brief   Max smc call timeout, in microseconds.
 */
#define TS_MAX_TMO            10000

/**
 * @brief   Secure and non secure memory address spaces.
 */
#if !defined(NSEC_MEMORY_START_ADDR)
#define NSEC_MEMORY_START_ADDR    ((uint8_t *)0x20000000)
#endif
#if !defined(NSEC_MEMORY_EXE_OFFSET)
#define NSEC_MEMORY_EXE_OFFSET    ((uint32_t) 0x00000000)
#endif
#if !defined(NSEC_MEMORY_END_ADDR)
#define NSEC_MEMORY_END_ADDR      ((uint8_t *)0x27000000)
#endif
#if !defined(SEC_MEMORY_START_ADDR)
#define SEC_MEMORY_START_ADDR     ((uint8_t *)0x27000000)
#endif
#if !defined(SEC_MEMORY_SIZE)
#define SEC_MEMORY_SIZE           ((size_t)0x1000000)
#endif

/** @} */

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/
typedef uint8_t * ts_params_area_t;

typedef struct tssi_service_state {
  uint32_t            ts_status;
  thread_reference_t  ts_thdp;
  ts_params_area_t    ts_datap;
  uint32_t            ts_datalen;
} ts_state_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/**
 * @name    Services tables definition macros.
 * @{
 */

/**
 * @brief    Table of the runtime state of the services.
 */
#define TS_STATE_TABLE                                                      \
  ts_state_t ts_state[TS_MAX_SVCS] = {0};

/**
 * @brief    Accessor to the runtime state of service i.
 */
#define TS_STATE(i) (&ts_state[i])


/**
 * @brief   Start of user service table.
 */
#define TS_CONF_TABLE_BEGIN                                                 \
  const thread_descriptor_t ts_configs[TS_MAX_SVCS] = {

/**
 * @brief   Entry of user services table.
 */
#define TS_CONF_TABLE_ENTRY(name, wap, prio, funcp, tsstatep)               \
  {name, wap, ((stkalign_t *)(wap)) + (sizeof (wap) / sizeof(stkalign_t)),  \
   prio, funcp, tsstatep},

/**
 * @brief   End of user services table.
 */
#define TS_CONF_TABLE_END                                                   \
};

/**
 * @brief   Accessor to the service table entry i.
 */
#define TS_CONF_TABLE(i) (&ts_configs[i])

/**
 * @brief   Trusted services base prio.
 */
#define TS_BASE_PRIO    (NORMALPRIO+1)

/**
 * @brief   Set the service status.
 * @note    The service sets the status at a value representing the status
 *          of the completion of the request. This value is
 *          service dependent.
 */
#define TS_SET_STATUS(svcp, newst)  (((ts_state_t *)svcp)->ts_status = newst)

/**
 * @brief   Get the pointer to the client shared memory.
 * @note    The client sets the data field at the start address
 *          of a shared memory allocated from the non secure memory space.
 */
#define TS_GET_DATA(svcp)  ((char *)((ts_state_t *)svcp)->ts_datap)

/**
 * @brief   Get the size of the client shared memory.
 * @note    The client sets the datalen field to the size
 *          of a shared memory allocated from the non secure memory space.
 */
#define TS_GET_DATALEN(svcp)  (((ts_state_t *)svcp)->ts_datalen)

/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  extern event_source_t tsEventSource;
  extern ts_state_t ts_state[];
  extern const thread_descriptor_t ts_configs[];
  extern thread_reference_t _ns_thread;
  CC_NO_RETURN void _ns_trampoline(uint8_t *addr);
  CC_NO_RETURN void tssiInit(void);
  msg_t tssiWaitRequest(ts_state_t *svcp);
  bool tsIsAddrSpaceValid(void *addr, size_t size);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

#endif /* CHTSSI_H */
