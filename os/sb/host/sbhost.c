/*
    ChibiOS - Copyright (C) 2006..2019 Giovanni Di Sirio.

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
 * @file    sb/host/sbhost.c
 * @brief   ARMv7-M sandbox code.
 *
 * @addtogroup ARMV7M_SANDBOX
 * @{
 */

#include "ch.h"
#include "sbhost.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

uint32_t sb_undef_handler(struct port_extctx *ectxp);

const port_syscall_t sb_syscalls[256] = {
  SB_SVC0_HANDLER,   SB_SVC1_HANDLER,   SB_SVC2_HANDLER,   SB_SVC3_HANDLER,
  SB_SVC4_HANDLER,   SB_SVC5_HANDLER,   SB_SVC6_HANDLER,   SB_SVC7_HANDLER,
  SB_SVC8_HANDLER,   SB_SVC9_HANDLER,   SB_SVC10_HANDLER,  SB_SVC11_HANDLER,
  SB_SVC12_HANDLER,  SB_SVC13_HANDLER,  SB_SVC14_HANDLER,  SB_SVC15_HANDLER,
  SB_SVC16_HANDLER,  SB_SVC17_HANDLER,  SB_SVC18_HANDLER,  SB_SVC19_HANDLER,
  SB_SVC20_HANDLER,  SB_SVC21_HANDLER,  SB_SVC22_HANDLER,  SB_SVC23_HANDLER,
  SB_SVC24_HANDLER,  SB_SVC25_HANDLER,  SB_SVC26_HANDLER,  SB_SVC27_HANDLER,
  SB_SVC28_HANDLER,  SB_SVC29_HANDLER,  SB_SVC30_HANDLER,  SB_SVC31_HANDLER,
  SB_SVC32_HANDLER,  SB_SVC33_HANDLER,  SB_SVC34_HANDLER,  SB_SVC35_HANDLER,
  SB_SVC36_HANDLER,  SB_SVC37_HANDLER,  SB_SVC38_HANDLER,  SB_SVC39_HANDLER,
  SB_SVC40_HANDLER,  SB_SVC41_HANDLER,  SB_SVC42_HANDLER,  SB_SVC43_HANDLER,
  SB_SVC44_HANDLER,  SB_SVC45_HANDLER,  SB_SVC46_HANDLER,  SB_SVC47_HANDLER,
  SB_SVC48_HANDLER,  SB_SVC49_HANDLER,  SB_SVC50_HANDLER,  SB_SVC51_HANDLER,
  SB_SVC52_HANDLER,  SB_SVC53_HANDLER,  SB_SVC54_HANDLER,  SB_SVC55_HANDLER,
  SB_SVC56_HANDLER,  SB_SVC57_HANDLER,  SB_SVC58_HANDLER,  SB_SVC59_HANDLER,
  SB_SVC60_HANDLER,  SB_SVC61_HANDLER,  SB_SVC62_HANDLER,  SB_SVC63_HANDLER,
  SB_SVC64_HANDLER,  SB_SVC65_HANDLER,  SB_SVC66_HANDLER,  SB_SVC67_HANDLER,
  SB_SVC68_HANDLER,  SB_SVC69_HANDLER,  SB_SVC70_HANDLER,  SB_SVC71_HANDLER,
  SB_SVC72_HANDLER,  SB_SVC73_HANDLER,  SB_SVC74_HANDLER,  SB_SVC75_HANDLER,
  SB_SVC76_HANDLER,  SB_SVC77_HANDLER,  SB_SVC78_HANDLER,  SB_SVC79_HANDLER,
  SB_SVC80_HANDLER,  SB_SVC81_HANDLER,  SB_SVC82_HANDLER,  SB_SVC83_HANDLER,
  SB_SVC84_HANDLER,  SB_SVC85_HANDLER,  SB_SVC86_HANDLER,  SB_SVC87_HANDLER,
  SB_SVC88_HANDLER,  SB_SVC89_HANDLER,  SB_SVC90_HANDLER,  SB_SVC91_HANDLER,
  SB_SVC92_HANDLER,  SB_SVC93_HANDLER,  SB_SVC94_HANDLER,  SB_SVC95_HANDLER,
  SB_SVC96_HANDLER,  SB_SVC97_HANDLER,  SB_SVC98_HANDLER,  SB_SVC99_HANDLER,
  SB_SVC100_HANDLER, SB_SVC101_HANDLER, SB_SVC102_HANDLER, SB_SVC103_HANDLER,
  SB_SVC104_HANDLER, SB_SVC105_HANDLER, SB_SVC106_HANDLER, SB_SVC107_HANDLER,
  SB_SVC108_HANDLER, SB_SVC109_HANDLER, SB_SVC110_HANDLER, SB_SVC111_HANDLER,
  SB_SVC112_HANDLER, SB_SVC113_HANDLER, SB_SVC114_HANDLER, SB_SVC115_HANDLER,
  SB_SVC116_HANDLER, SB_SVC117_HANDLER, SB_SVC118_HANDLER, SB_SVC119_HANDLER,
  SB_SVC120_HANDLER, SB_SVC121_HANDLER, SB_SVC122_HANDLER, SB_SVC123_HANDLER,
  SB_SVC124_HANDLER, SB_SVC125_HANDLER, SB_SVC126_HANDLER, SB_SVC127_HANDLER,
  SB_SVC128_HANDLER, SB_SVC129_HANDLER, SB_SVC130_HANDLER, SB_SVC131_HANDLER,
  SB_SVC132_HANDLER, SB_SVC133_HANDLER, SB_SVC134_HANDLER, SB_SVC135_HANDLER,
  SB_SVC136_HANDLER, SB_SVC137_HANDLER, SB_SVC138_HANDLER, SB_SVC139_HANDLER,
  SB_SVC140_HANDLER, SB_SVC141_HANDLER, SB_SVC142_HANDLER, SB_SVC143_HANDLER,
  SB_SVC144_HANDLER, SB_SVC145_HANDLER, SB_SVC146_HANDLER, SB_SVC147_HANDLER,
  SB_SVC148_HANDLER, SB_SVC149_HANDLER, SB_SVC150_HANDLER, SB_SVC151_HANDLER,
  SB_SVC152_HANDLER, SB_SVC153_HANDLER, SB_SVC154_HANDLER, SB_SVC155_HANDLER,
  SB_SVC156_HANDLER, SB_SVC157_HANDLER, SB_SVC158_HANDLER, SB_SVC159_HANDLER,
  SB_SVC160_HANDLER, SB_SVC161_HANDLER, SB_SVC162_HANDLER, SB_SVC163_HANDLER,
  SB_SVC164_HANDLER, SB_SVC165_HANDLER, SB_SVC166_HANDLER, SB_SVC167_HANDLER,
  SB_SVC168_HANDLER, SB_SVC169_HANDLER, SB_SVC170_HANDLER, SB_SVC171_HANDLER,
  SB_SVC172_HANDLER, SB_SVC173_HANDLER, SB_SVC174_HANDLER, SB_SVC175_HANDLER,
  SB_SVC176_HANDLER, SB_SVC177_HANDLER, SB_SVC178_HANDLER, SB_SVC179_HANDLER,
  SB_SVC180_HANDLER, SB_SVC181_HANDLER, SB_SVC182_HANDLER, SB_SVC183_HANDLER,
  SB_SVC184_HANDLER, SB_SVC185_HANDLER, SB_SVC186_HANDLER, SB_SVC187_HANDLER,
  SB_SVC188_HANDLER, SB_SVC189_HANDLER, SB_SVC190_HANDLER, SB_SVC191_HANDLER,
  SB_SVC192_HANDLER, SB_SVC193_HANDLER, SB_SVC194_HANDLER, SB_SVC195_HANDLER,
  SB_SVC196_HANDLER, SB_SVC197_HANDLER, SB_SVC198_HANDLER, SB_SVC199_HANDLER,
  SB_SVC200_HANDLER, SB_SVC201_HANDLER, SB_SVC202_HANDLER, SB_SVC203_HANDLER,
  SB_SVC204_HANDLER, SB_SVC205_HANDLER, SB_SVC206_HANDLER, SB_SVC207_HANDLER,
  SB_SVC208_HANDLER, SB_SVC209_HANDLER, SB_SVC210_HANDLER, SB_SVC211_HANDLER,
  SB_SVC212_HANDLER, SB_SVC213_HANDLER, SB_SVC214_HANDLER, SB_SVC215_HANDLER,
  SB_SVC216_HANDLER, SB_SVC217_HANDLER, SB_SVC218_HANDLER, SB_SVC219_HANDLER,
  SB_SVC220_HANDLER, SB_SVC221_HANDLER, SB_SVC222_HANDLER, SB_SVC223_HANDLER,
  SB_SVC224_HANDLER, SB_SVC225_HANDLER, SB_SVC226_HANDLER, SB_SVC227_HANDLER,
  SB_SVC228_HANDLER, SB_SVC229_HANDLER, SB_SVC230_HANDLER, SB_SVC231_HANDLER,
  SB_SVC232_HANDLER, SB_SVC233_HANDLER, SB_SVC234_HANDLER, SB_SVC235_HANDLER,
  SB_SVC236_HANDLER, SB_SVC237_HANDLER, SB_SVC238_HANDLER, SB_SVC239_HANDLER,
  SB_SVC240_HANDLER, SB_SVC241_HANDLER, SB_SVC242_HANDLER, SB_SVC243_HANDLER,
  SB_SVC244_HANDLER, SB_SVC245_HANDLER, SB_SVC246_HANDLER, SB_SVC247_HANDLER,
  SB_SVC248_HANDLER, SB_SVC249_HANDLER, SB_SVC250_HANDLER, SB_SVC251_HANDLER,
  SB_SVC252_HANDLER, SB_SVC253_HANDLER, SB_SVC254_HANDLER, SB_SVC255_HANDLER
};

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

__attribute__((weak))
uint32_t sb_undef_handler(struct port_extctx *ectxp) {

  (void)ectxp;

  return SB_ERR_NOT_IMPLEMENTED;
}

/**
 * @brief   Sandbox object initialization.
 *
 * @param[out] sbcp     pointer to the sandbox object
 *
 * @init
 */
void sbObjectInit(sb_class_t *sbcp) {

  sbcp->config = NULL;
  sbcp->tp     = NULL;
#if CH_CFG_USE_MESSAGES == TRUE
  sbcp->msg_tp = NULL;
#endif
#if CH_CFG_USE_EVENTS == TRUE
  chEvtObjectInit(&sbcp->es);
#endif
}

/**
 * @brief   Starts a sandboxed thread.
 *
 * @param[in] sbcp      pointer to the sandbox object
 * @return              The function returns only if the operation failed.
 *
 * @api
 */
void sbStart(sb_class_t *sbcp, const sb_config_t *config) {
  uint32_t pc, psp;
  const sb_header_t *sbhp;

  /* The header is conventionally placed at base of region zero.*/
  sbhp = (const sb_header_t *)config->r0_base;

  /* Checking header magic numbers.*/
  if ((sbhp->hdr_magic1 != SB_MAGIC1) || (sbhp->hdr_magic2 != SB_MAGIC2)) {
    return;
  }

  /* Checking header size and alignment.*/
  if (sbhp->hdr_size != sizeof (sb_header_t)) {
    return;
  }

  /* Checking regions, applet regions and sandbox regions must match.*/
  if ((sbhp->r0_base != config->r0_base) || (sbhp->r0_end != config->r0_end) ||
      (sbhp->r1_base != config->r1_base) || (sbhp->r1_end != config->r1_end)) {
    return;
  }

  /* PC initial address, by convention it is immediately after the header.*/
  pc = (sbhp->r0_base + sbhp->hdr_size) | 1U;

  /* PSP initial address, it is placed at end of the last region.*/
  if (config->r1_base == 0U) {
    /* Must be in region 1.*/
    psp = config->r0_end;
  }
  else {
    /* Must be in region 2.*/
    psp = config->r1_end;
  }

  /* Additional context information.*/
  sbcp->config = config;
  chThdGetSelfX()->ctx.syscall.p    = (const void *)sbcp;
  chThdGetSelfX()->ctx.syscall.psp  = (regarm_t)__get_PSP();

  /* Jumping to the unprivileged code.*/
  port_unprivileged_jump((regarm_t)pc, (regarm_t)psp);

  /* Must never happen condition.*/
  chSysHalt("returned");
}

/** @} */
