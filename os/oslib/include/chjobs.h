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
 * @file    chjobs.h
 * @brief   Jobs Queues structures and macros.
 * @details This module implements queues of generic jobs to be delegated
 *          asynchronously to a pool of dedicated threads.
 *          Operations defined for Jobs Queues
 *          - <b>Get</b>: An job object is taken from the pool of the
 *            available jobs.
 *          - <b>Post</b>: A job is posted to the queue, it will be
 *            returned to the pool after execution.
 *          .
 *
 * @addtogroup oslib_jobs_queues
 * @{
 */

#ifndef CHJOBS_H
#define CHJOBS_H

#if (CH_CFG_USE_JOBS == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

#if CH_CFG_USE_MEMPOOLS == FALSE
#error "CH_CFG_USE_JOBS requires CH_CFG_USE_MEMPOOLS"
#endif

#if CH_CFG_USE_SEMAPHORES == FALSE
#error "CH_CFG_USE_JOBS requires CH_CFG_USE_SEMAPHORES"
#endif

#if CH_CFG_USE_MAILBOXES == FALSE
#error "CH_CFG_USE_JOBS requires CH_CFG_USE_MAILBOXES"
#endif

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a jobs queue.
 */
typedef struct ch_jobs_queue {
  /**
   * @brief   Pool of the free jobs.
   */
  guarded_memory_pool_t     free;
  /**
   * @brief   Mailbox of the sent jobs.
   */
  mailbox_t                 mbx;
} jobs_queue_t;

/**
 * @brief   Type of a job function.
 */
typedef void (*job_function_t)(void *arg);

/**
 * @brief   Type of a job descriptor.
 */
typedef struct ch_job_descriptor {
  /**
   * @brief   Job function.
   */
  job_function_t            jobfunc;
  /**
   * @brief   Argument to be passed to the job function.
   */
  void                      *jobarg;
} job_descriptor_t;

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Initializes a jobs queue object.
 *
 * @param[out] jqp      pointer to a @p jobs_queue_t structure
 * @param[in] jobsn     number of jobs available
 * @param[in] jobsbuf   pointer to the buffer of jobs, it must be able
 *                      to hold @p jobsn @p job_descriptor_t structures
 * @param[in] msgbuf    pointer to the buffer of messages, it must be able
 *                      to hold @p jobsn @p msg_t messages
 *
 * @init
 */
static inline void chJobsObjectInit(jobs_queue_t *jqp,
                                    size_t jobsn,
                                    job_descriptor_t *jobsbuf,
                                    msg_t *msgbuf) {

  chDbgCheck((jobsn > 0U) && (jobsbuf != NULL) && (msgbuf != NULL));

  chGuardedPoolObjectInit(&jqp->free, sizeof (job_descriptor_t));
  chGuardedPoolLoadArray(&jqp->free, (void *)jobsbuf, jobsn);
  chMBObjectInit(&jqp->mbx, msgbuf, jobsn);
}

/**
 * @brief   Allocates a free job object.
 *
 * @param[in] jqp       pointer to a @p jobs_queue_t structure
 * @return              The pointer to the allocated job object.
 * @retval NULL         if a job object is not immediately available.
 *
 * @iclass
 */
static inline job_descriptor_t *chGetTakeI(jobs_queue_t *jqp) {

  return chGuardedPoolAllocI(&jqp->free);
}

/**
 * @brief   Allocates a free job object.
 *
 * @param[in] jqp       pointer to a @p jobs_queue_t structure
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_IMMEDIATE immediate timeout.
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The pointer to the allocated job object.
 * @retval NULL         if a job object is not available within the specified
 *                      timeout.
 *
 * @sclass
 */
static inline job_descriptor_t *chJobsGetTimeoutS(jobs_queue_t *jqp,
                                                  sysinterval_t timeout) {

  return chGuardedPoolAllocTimeoutS(&jqp->free, timeout);
}

/**
 * @brief   Allocates a free job object.
 *
 * @param[in] jqp       pointer to a @p jobs_queue_t structure
 * @param[in] timeout   the number of ticks before the operation timeouts,
 *                      the following special values are allowed:
 *                      - @a TIME_IMMEDIATE immediate timeout.
 *                      - @a TIME_INFINITE no timeout.
 *                      .
 * @return              The pointer to the allocated job object.
 * @retval NULL         if a job object is not available within the specified
 *                      timeout.
 *
 * @api
 */
static inline job_descriptor_t *chJobsGetTimeout(jobs_queue_t *jqp,
                                                 sysinterval_t timeout) {

  return chGuardedPoolAllocTimeout(&jqp->free, timeout);
}

/**
 * @brief   Posts a job object.
 * @note    By design the object can be always immediately posted.
 *
 * @param[in] jqp       pointer to a @p jobs_queue_t structure
 * @param[in] jp        pointer to the job object to be posted
 *
 * @iclass
 */
static inline void chJobsPostI(jobs_queue_t *jqp, job_descriptor_t *jp) {
  msg_t msg;

  msg = chMBPostI(&jqp->mbx, (msg_t)jp);
  chDbgAssert(msg == MSG_OK, "post failed");
}

/**
 * @brief   Posts a job object.
 * @note    By design the object can be always immediately posted.
 *
 * @param[in] jqp       pointer to a @p jobs_queue_t structure
 * @param[in] jp        pointer to the job object to be posted
 *
 * @sclass
 */
static inline void chJobsPostS(jobs_queue_t *jqp, job_descriptor_t *jp) {
  msg_t msg;

  msg = chMBPostTimeoutS(&jqp->mbx, (msg_t)jp, TIME_IMMEDIATE);
  chDbgAssert(msg == MSG_OK, "post failed");
}

/**
 * @brief   Posts a job object.
 * @note    By design the object can be always immediately posted.
 *
 * @param[in] jqp       pointer to a @p jobs_queue_t structure
 * @param[in] jp        pointer to the job object to be posted
 *
 * @api
 */
static inline void chJobsPost(jobs_queue_t *jqp, job_descriptor_t *jp) {
  msg_t msg;

  msg = chMBPostTimeout(&jqp->mbx, (msg_t)jp, TIME_IMMEDIATE);
  chDbgAssert(msg == MSG_OK, "post failed");
}

/**
 * @brief   Posts an high priority job object.
 * @note    By design the object can be always immediately posted.
 *
 * @param[in] jqp       pointer to a @p jobs_queue_t structure
 * @param[in] jp        pointer to the job object to be posted
 *
 * @iclass
 */
static inline void chJobsPostAheadI(jobs_queue_t *jqp, job_descriptor_t *jp) {
  msg_t msg;

  msg = chMBPostAheadI(&jqp->mbx, (msg_t)jp);
  chDbgAssert(msg == MSG_OK, "post failed");
}

/**
 * @brief   Posts an high priority job object.
 * @note    By design the object can be always immediately posted.
 *
 * @param[in] jqp       pointer to a @p jobs_queue_t structure
 * @param[in] jp        pointer to the job object to be posted
 *
 * @sclass
 */
static inline void chJobsPostAheadS(jobs_queue_t *jqp, job_descriptor_t *jp) {
  msg_t msg;

  msg = chMBPostAheadTimeoutS(&jqp->mbx, (msg_t)jp, TIME_IMMEDIATE);
  chDbgAssert(msg == MSG_OK, "post failed");
}

/**
 * @brief   Posts an high priority job object.
 * @note    By design the object can be always immediately posted.
 *
 * @param[in] jqp       pointer to a @p jobs_queue_t structure
 * @param[in] jp        pointer to the job object to be posted
 *
 * @api
 */
static inline void chJobsPostAhead(jobs_queue_t *jqp, job_descriptor_t *jp) {
  msg_t msg;

  msg = chMBPostAheadTimeout(&jqp->mbx, (msg_t)jp, TIME_IMMEDIATE);
  chDbgAssert(msg == MSG_OK, "post failed");
}

/**
 * @brief   Waits for a job then executes it.
 *
 * @param[in] jqp       pointer to a @p jobs_queue_t structure
 * @return              The function outcome.
 * @retval MSG_OK       if a job has been executed.
 * @retval MSG_RESET    if the internal mailbox has been reset.
 */
static inline msg_t chJobsDispatch(jobs_queue_t *jqp) {
  msg_t msg, jmsg;

  msg = chMBFetchTimeout(&jqp->mbx, &jmsg, TIME_INFINITE);
  if (msg == MSG_OK) {
    job_descriptor_t *jp = (job_descriptor_t *)jmsg;

    /* Invoking the job function.*/
    jp->jobfunc(jp->jobarg);

    /* Returning the job descriptor object.*/
    chGuardedPoolFree(&jqp->free, (void *)jp);
  }

  return msg;
}

/**
 * @brief   Waits for a job then executes it.
 *
 * @param[in] jqp       pointer to a @p jobs_queue_t structure
 * @return              The function outcome.
 * @retval MSG_OK       if a job has been executed.
 * @retval MSG_TIMEOUT  if a timeout occurred.
 * @retval MSG_RESET    if the internal mailbox has been reset.
 */
static inline msg_t chJobsDispatchTimeout(jobs_queue_t *jqp,
                                          sysinterval_t timeout) {
  msg_t msg, jmsg;

  msg = chMBFetchTimeout(&jqp->mbx, &jmsg, timeout);
  if (msg == MSG_OK) {
    job_descriptor_t *jp = (job_descriptor_t *)jmsg;

    /* Invoking the job function.*/
    jp->jobfunc(jp->jobarg);

    /* Returning the job descriptor object.*/
    chGuardedPoolFree(&jqp->free, (void *)jp);
  }

  return msg;
}

#endif /* CH_CFG_USE_JOBS == TRUE */

#endif /* CHJOBS_H */

/** @} */
