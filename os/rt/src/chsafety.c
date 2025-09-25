/*
    ChibiOS - Copyright (C) 2006,2007,2008,2009,2010,2011,2012,2013,2014,
              2015,2016,2017,2018,2019,2020,2021 Giovanni Di Sirio.

    This file is part of ChibiOS.

    ChibiOS is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation version 3 of the License.

    ChibiOS is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file    rt/src/chsafety.c
 * @brief   Functional safety code.
 *
 * @addtogroup rt_safety
 * @details This module includes features related to functional safety.
 *          - Safety assertions.
 *          - Runtime integrity checks.
 *          .
 * @{
 */

#include "ch.h"

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   System integrity check.
 * @details Performs an integrity check of the important ChibiOS/RT data
 *          structures.
 * @note    The reaction in case of failure is to invoke the
 *          @p CH_CFG_INTEGRITY_HOOK which, by default, halts the system.
 * @note    If the system is corrupted then one possible outcomes of this
 *          function is an exception caused by @p NULL or corrupted pointers
 *          in list elements. Exception vectors must be monitored as well.
 * @note    This function is not used internally, it is up to the
 *          application to define if and where to perform system
 *          checking.
 * @note    Performing all tests at once can be a slow operation and can
 *          degrade the system response time. It is suggested to execute
 *          one test at time and release the critical zone in between tests.
 *
 * @param[in] testmask  Each bit in this mask is associated to a test to be
 *                      performed.
 * @return              The test result.
 * @retval false        The test succeeded.
 * @retval true         Test failed.
 *
 * @iclass
 */
bool chSftIntegrityCheckI(unsigned testmask) {
  os_instance_t *oip = currcore;

  chDbgCheckClassI();

  /* Ready List integrity check.*/
  if ((testmask & CH_INTEGRITY_RLIST) != 0U) {

    /* Scanning the ready list forward.*/
    ch_priority_queue_t *current = &oip->rlist.pqueue;
    do {
      ch_priority_queue_t *next = current->next;

      /* Checking the backward link.*/
      if (next->prev != current) {
        return true;
      }
      current = next;
    } while (current != &oip->rlist.pqueue);
  }

  /* Timers list integrity check.*/
  if ((testmask & CH_INTEGRITY_VTLIST) != 0U) {

    /* Scanning the timers list forward.*/
    ch_delta_list_t *current = &oip->vtlist.dlist;
    do {
      ch_delta_list_t *next = current->next;

      /* Checking the backward link.*/
      if (next->prev != current) {
        return true;
      }
      current = next;
    } while (current != &oip->vtlist.dlist);
  }

#if CH_CFG_USE_REGISTRY == TRUE
  if ((testmask & CH_INTEGRITY_REGISTRY) != 0U) {

    /* Scanning the registry list forward.*/
    ch_queue_t *current = REG_HEADER(oip);
    do {
      ch_queue_t *next = current->next;

      /* Checking the backward link.*/
      if (next->prev != current) {
        return true;
      }
      current = next;
    } while (current != REG_HEADER(oip));
  }
#endif /* CH_CFG_USE_REGISTRY == TRUE */

#if defined(PORT_INTEGRITY_CHECK)
  if ((testmask & CH_INTEGRITY_PORT) != 0U) {
    PORT_INTEGRITY_CHECK();
  }
#endif

  return false;
}

/** @} */
