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
 *          - Pointers validation.
 *          - Lists and queues integrity checks.
 *          - Runtime full system checks.
 *          .
 *          Purpose of this module is not to detect plain programming errors,
 *          that is handled by the debug module, but to early detect anomalous
 *          conditions triggered by HW failures or unexpected interactions
 *          with other SW elements causing memory corruption.<br>
 *          The concept is to detect corruption early and avoid spreading of
 *          the error causing random actions or stuck execution.<br>
 *          The application can override the reaction to safety checks, the
 *          default is cleanly halting the system.<br>
 *          This is the list of the currently implemented checks:
 *          - Level 0: none.
 *          - Level 1:
 *              - Integrity checking of objects on dispose.
 *              - Zeroing of objects on dispose.
 *              .
 *          - Level 2:
 *              - Ready list backward pointer check on insertion.
 *              .
 *          - Level 3:
 *              - Ready list forward pointer validation on insertion.
 *              .
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

#if (CH_CFG_HARDENING_LEVEL >= 1) || defined(__DOXYGEN__)
/**
 * @brief   Performs an integrity check on a single link list.
 * @details This function performs a quick integrity check, it does not
 *          perform a full list traversal but checks the link to the first
 *          element in the list.
 * @note    This function is only available at hardening level 1, at lower
 *          levels an empty macro replaces it.
 * @note    At hardening level 2 and higher pointers are also verified
 *          before de-referencing them.
 *
 * @param[in] p         a pointer to a @p ch_list_t element
 *
 * @xclass
 */
void chSftCheckListX(const void *p) {
  const ch_list_t *lp = (const ch_list_t *)p;
  ch_list_t *first;

  /* Link to the first element.*/
  first = lp->next;

  /* Checking pointer.*/
  chSftAssert(1, SFT_IS_VALID_DATA_POINTER(first), "invalid link");
}

/**
 * @brief   Performs an integrity check on a double link list.
 * @details This function performs a quick integrity check, it does not
 *          perform a full queue traversal but checks the links to the first
 *          and last elements in the queue.
 * @note    This function is only available at hardening level 1, at lower
 *          levels an empty macro replaces it.
 *
 * @param[in] p         a pointer to a @p ch_queue_t, or @p ch_priority_queue_t
 *                      or @p ch_delta_list_t element
 *
 * @xclass
 */
void chSftCheckQueueX(const void *p) {
  const ch_queue_t *qp = (const ch_queue_t *)p;
  ch_queue_t *first, *last;

  /* Links to the first and last elements.*/
  first = qp->next;
  last = qp->prev;

  /* Checking pointers before de-referencing.*/
  chSftAssert(1, SFT_IS_VALID_DATA_POINTER(first) &&
                 SFT_IS_VALID_DATA_POINTER(last), "invalid links");

  /* Both first and last elements must link back.*/
  chSftAssert(1, (first->prev == qp) && (last->next == qp), "not linking back");
}
#endif

/**
 * @brief   System integrity check.
 * @details Performs an integrity check of the important ChibiOS/RT data
 *          structures.
 * @note    The reaction in case of failure is to invoke the
 *          @p CH_CFG_INTEGRITY_HOOK which, by default, halts the system.
 * @note    This functionality is available at any hardening level.
 * @note    Pointers validation is enabled at hardening level 2 or greater,
 *          at lower levels a corrupted pointer can cause an exception.
 *          Exceptions should be monitored as well as possible outcomes.
 * @note    This function is not used internally, it is up to the
 *          application to define if and where to perform system
 *          checking.
 *
 * @param[in] testmask  Each bit in this mask is associated to a test to be
 *                      performed.
 *
 * @iclass
 */
void chSftIntegrityCheckI(unsigned testmask) {
  os_instance_t *oip = currcore;

  chDbgCheckClassI();

  /* Ready List integrity check.*/
  if ((testmask & CH_INTEGRITY_RLIST) != 0U) {

    /* Scanning the ready list forward.*/
    ch_priority_queue_t *current = &oip->rlist.pqueue;
    do {
      ch_priority_queue_t *next;

      /* Checking the backward link.*/
      next = current->next;
      chSftValidateDataPointerX(2, next);
      chSftAssert(0, next->prev == current, "invalid backward pointer");
      current = next;
    } while (current != &oip->rlist.pqueue);
  }

  /* Timers list integrity check.*/
  if ((testmask & CH_INTEGRITY_VTLIST) != 0U) {

    /* Scanning the timers list forward.*/
    ch_delta_list_t *current = &oip->vtlist.dlist;
    do {
      ch_delta_list_t *next;

      /* Checking the backward link.*/
      next = current->next;
      chSftValidateDataPointerX(2, next);
      chSftAssert(0, next->prev == current, "invalid backward pointer");
      current = next;
    } while (current != &oip->vtlist.dlist);
  }

#if CH_CFG_USE_REGISTRY == TRUE
  if ((testmask & CH_INTEGRITY_REGISTRY) != 0U) {

    /* Scanning the registry list forward.*/
    ch_queue_t *current = REG_HEADER(oip);
    do {
      ch_queue_t *next;

      /* Checking the backward link.*/
      next = current->next;
      chSftValidateDataPointerX(2, next);
      chSftAssert(0, next->prev == current, "invalid backward pointer");
      current = next;
    } while (current != REG_HEADER(oip));
  }
#endif /* CH_CFG_USE_REGISTRY == TRUE */

#if defined(PORT_INTEGRITY_CHECK)
  if ((testmask & CH_INTEGRITY_PORT) != 0U) {
    PORT_INTEGRITY_CHECK();
  }
#endif
}

/** @} */
