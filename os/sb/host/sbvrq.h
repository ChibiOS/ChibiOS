/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file    sb/host/sbvrq.h
 * @brief   ARM SandBox host Virtual IRQs macros and structures.
 *
 * @addtogroup ARM_SANDBOX_HOSTAPI
 * @{
 */

#ifndef SBVRQ_H
#define SBVRQ_H

#if (SB_CFG_ENABLE_VRQ == TRUE) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/**
 * @name    Virtual ISR register bit definitions
 * @{
 */
#define SB_VRQ_ISR_DISABLED     1U
/** @} */

/**
 * @name    VRQ pseudo-instructions handlers
 * @{
 */
#define SB_SVC119_HANDLER       sb_fastc_vrq_gcsts
#define SB_SVC120_HANDLER       sb_fastc_vrq_setwt
#define SB_SVC121_HANDLER       sb_fastc_vrq_clrwt
#define SB_SVC122_HANDLER       sb_fastc_vrq_seten
#define SB_SVC123_HANDLER       sb_fastc_vrq_clren
#define SB_SVC124_HANDLER       sb_fastc_vrq_disable
#define SB_SVC125_HANDLER       sb_fastc_vrq_enable
#define SB_SVC126_HANDLER       sb_fastc_vrq_getisr
#define SB_SVC127_HANDLER       sb_fastc_vrq_return

#define SB_SVC253_HANDLER       sb_sysc_vrq_set_alarm
#define SB_SVC254_HANDLER       sb_sysc_vrq_reset_alarm
#define SB_SVC255_HANDLER       sb_sysc_vrq_wait
/** @} */

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a mask of Virtual IRQs.
 */
typedef uint32_t sb_vrqmask_t;

/**
 * @brief   Type of a Virtual IRQs.
 */
typedef uint32_t sb_vrqnum_t;

/**
 * @brief   Type of a structure representing a VRQ handling block.
 */
typedef struct sb_vrqblock sb_vrqblock_t;

/**
 * @brief   Structure representing a VRQ handling block.
 */
struct sb_vrqblock {
  /**
   * @brief   Mask of enabled virtual IRQ flags.
   */
  sb_vrqmask_t                  enmask;
  /**
   * @brief   Mask of pending virtual IRQ flags.
   */
  sb_vrqmask_t                  wtmask;
  /**
   * @brief   Global virtual IRQ status register.
   */
  uint32_t                      isr;
  /**
   * @brief   Reference to sh SB thread while waiting for VRQs.
   */
  thread_reference_t            trp;
  /**
   * @brief   Virtual timer used for alarms.
   */
  virtual_timer_t               alarm_vt;
  /**
   * @brief   Status flags associated to each VRQ.
   */
  uint32_t                      flags[32];
};

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void sbVRQSetFlagsI(sb_class_t *sbp, sb_vrqnum_t nvrq, uint32_t flags);
  void sbVRQTriggerS(sb_class_t *sbp, sb_vrqnum_t nvrq);
  void sbVRQTriggerI(sb_class_t *sbp, sb_vrqnum_t nvrq);
  void sb_sysc_vrq_set_alarm(sb_class_t *sbp, struct port_extctx *ectxp);
  void sb_sysc_vrq_reset_alarm(sb_class_t *sbp, struct port_extctx *ectxp);
  void sb_sysc_vrq_wait(sb_class_t *sbp, struct port_extctx *ectxp);
  void sb_fastc_vrq_gcsts(sb_class_t *sbp, struct port_extctx *ectxp);
  void sb_fastc_vrq_setwt(sb_class_t *sbp, struct port_extctx *ectxp);
  void sb_fastc_vrq_clrwt(sb_class_t *sbp, struct port_extctx *ectxp);
  void sb_fastc_vrq_seten(sb_class_t *sbp, struct port_extctx *ectxp);
  void sb_fastc_vrq_clren(sb_class_t *sbp, struct port_extctx *ectxp);
  void sb_fastc_vrq_disable(sb_class_t *sbp, struct port_extctx *ectxp);
  void sb_fastc_vrq_enable(sb_class_t *sbp, struct port_extctx *ectxp);
  void sb_fastc_vrq_getisr(sb_class_t *sbp, struct port_extctx *ectxp);
  void sb_fastc_vrq_return(sb_class_t *sbp, struct port_extctx *ectxp);
  void __sb_vrq_check_pending(sb_class_t *sbp, struct port_extctx *ectxp);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Triggers VRQs on the specified sandbox.
 * @note    This function must be called from IRQ context because
 *          it manipulates exception stack frames.
 *
 * @param[in] sbp       pointer to a @p sb_class_t structure
 * @param[in] nvrq      number of VRQ to be activated
 *
 * @special
 */
static inline void sbVRQTriggerFromISR(sb_class_t *sbp, sb_vrqnum_t nvrq) {

  chSysLockFromISR();

  sbVRQTriggerI(sbp, nvrq);

  chSysUnlockFromISR();
}

#endif /* SB_CFG_ENABLE_VRQ == TRUE */

#endif /* SBVRQ_H */

/** @} */
