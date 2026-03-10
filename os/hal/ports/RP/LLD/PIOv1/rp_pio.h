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
 * @file    PIOv1/rp_pio.h
 * @brief   PIO helper driver header.
 *
 * @addtogroup RP_PIO
 * @{
 */

#ifndef RP_PIO_H
#define RP_PIO_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    PIO GPIO function select values
 * @{
 */
#define RP_PIO_FUNCSEL_PIO0             6U
#define RP_PIO_FUNCSEL_PIO1             7U
#if RP_HAS_PIO2 == TRUE
#define RP_PIO_FUNCSEL_PIO2             8U
#endif
#define RP_PIO_FUNCSEL_NULL             31U
/** @} */

/**
 * @name    PIO resource constants
 * @{
 */
#define RP_PIO_NUM_STATE_MACHINES       4U
#define RP_PIO_NUM_INSTR_MEM            32U

/**
 * @brief   Any state machine selector.
 */
#define RP_PIO_SM_ID_ANY                RP_PIO_NUM_STATE_MACHINES
/** @} */

/**
 * @name    PIO CTRL register bits
 * @{
 */
#define PIO_CTRL_SM_ENABLE(n)           (1U << (n))
#define PIO_CTRL_SM_RESTART(n)          (1U << ((n) + 4U))
#define PIO_CTRL_CLKDIV_RESTART(n)      (1U << ((n) + 8U))
/** @} */

/**
 * @name    PIO FSTAT register bits
 * @{
 */
#define PIO_FSTAT_RXFULL(n)             (1U << ((n) + 0U))
#define PIO_FSTAT_RXEMPTY(n)            (1U << ((n) + 8U))
#define PIO_FSTAT_TXFULL(n)             (1U << ((n) + 16U))
#define PIO_FSTAT_TXEMPTY(n)            (1U << ((n) + 24U))
/** @} */

/**
 * @name    PIO FDEBUG register bits (W1C)
 * @{
 */
#define PIO_FDEBUG_RXSTALL(n)           (1U << ((n) + 0U))
#define PIO_FDEBUG_RXUNDER(n)           (1U << ((n) + 8U))
#define PIO_FDEBUG_TXOVER(n)            (1U << ((n) + 16U))
#define PIO_FDEBUG_TXSTALL(n)           (1U << ((n) + 24U))
/** @} */

/**
 * @name    PIO state machine CLKDIV register bits
 * @{
 */
#define PIO_SM_CLKDIV_FRAC_Pos          8U
#define PIO_SM_CLKDIV_FRAC_Msk          (0xFFU << PIO_SM_CLKDIV_FRAC_Pos)
#define PIO_SM_CLKDIV_INT_Pos           16U
#define PIO_SM_CLKDIV_INT_Msk           (0xFFFFU << PIO_SM_CLKDIV_INT_Pos)

/**
 * @brief   Builds a CLKDIV register value from integer and fractional parts.
 */
#define PIO_SM_CLKDIV(intdiv, frac)     (((uint32_t)(intdiv) << PIO_SM_CLKDIV_INT_Pos) | \
                                         ((uint32_t)(frac) << PIO_SM_CLKDIV_FRAC_Pos))
/** @} */

/**
 * @name    PIO state machine EXECCTRL register bits
 * @{
 */
#define PIO_SM_EXECCTRL_STATUS_N_Pos    0U
#define PIO_SM_EXECCTRL_STATUS_N_Msk    (0xFU << PIO_SM_EXECCTRL_STATUS_N_Pos)
#define PIO_SM_EXECCTRL_STATUS_SEL      (1U << 4U)
#define PIO_SM_EXECCTRL_WRAP_BOTTOM_Pos 7U
#define PIO_SM_EXECCTRL_WRAP_BOTTOM_Msk (0x1FU << PIO_SM_EXECCTRL_WRAP_BOTTOM_Pos)
#define PIO_SM_EXECCTRL_WRAP_TOP_Pos    12U
#define PIO_SM_EXECCTRL_WRAP_TOP_Msk    (0x1FU << PIO_SM_EXECCTRL_WRAP_TOP_Pos)
#define PIO_SM_EXECCTRL_OUT_STICKY      (1U << 17U)
#define PIO_SM_EXECCTRL_INLINE_OUT_EN   (1U << 18U)
#define PIO_SM_EXECCTRL_OUT_EN_SEL_Pos  19U
#define PIO_SM_EXECCTRL_OUT_EN_SEL_Msk  (0x1FU << PIO_SM_EXECCTRL_OUT_EN_SEL_Pos)
#define PIO_SM_EXECCTRL_JMP_PIN_Pos     24U
#define PIO_SM_EXECCTRL_JMP_PIN_Msk     (0x1FU << PIO_SM_EXECCTRL_JMP_PIN_Pos)
#define PIO_SM_EXECCTRL_SIDE_PINDIR     (1U << 29U)
#define PIO_SM_EXECCTRL_SIDE_EN         (1U << 30U)
#define PIO_SM_EXECCTRL_EXEC_STALLED    (1U << 31U)

/**
 * @brief   Helper to build wrap range for EXECCTRL.
 */
#define PIO_SM_EXECCTRL_WRAP(bottom, top)                                    \
  (((uint32_t)(bottom) << PIO_SM_EXECCTRL_WRAP_BOTTOM_Pos) |                \
   ((uint32_t)(top) << PIO_SM_EXECCTRL_WRAP_TOP_Pos))
/** @} */

/**
 * @name    PIO state machine SHIFTCTRL register bits
 * @{
 */
#define PIO_SM_SHIFTCTRL_AUTOPUSH       (1U << 16U)
#define PIO_SM_SHIFTCTRL_AUTOPULL       (1U << 17U)
#define PIO_SM_SHIFTCTRL_IN_SHIFTDIR    (1U << 18U)
#define PIO_SM_SHIFTCTRL_OUT_SHIFTDIR   (1U << 19U)
#define PIO_SM_SHIFTCTRL_PUSH_THRESH_Pos  20U
#define PIO_SM_SHIFTCTRL_PUSH_THRESH_Msk  (0x1FU << PIO_SM_SHIFTCTRL_PUSH_THRESH_Pos)
#define PIO_SM_SHIFTCTRL_PULL_THRESH_Pos  25U
#define PIO_SM_SHIFTCTRL_PULL_THRESH_Msk  (0x1FU << PIO_SM_SHIFTCTRL_PULL_THRESH_Pos)
#define PIO_SM_SHIFTCTRL_FJOIN_TX       (1U << 30U)
#define PIO_SM_SHIFTCTRL_FJOIN_RX       (1U << 31U)
/** @} */

/**
 * @name    PIO state machine PINCTRL register bits
 * @{
 */
#define PIO_SM_PINCTRL_OUT_BASE_Pos     0U
#define PIO_SM_PINCTRL_OUT_BASE_Msk     (0x1FU << PIO_SM_PINCTRL_OUT_BASE_Pos)
#define PIO_SM_PINCTRL_SET_BASE_Pos     5U
#define PIO_SM_PINCTRL_SET_BASE_Msk     (0x1FU << PIO_SM_PINCTRL_SET_BASE_Pos)
#define PIO_SM_PINCTRL_SIDESET_BASE_Pos 10U
#define PIO_SM_PINCTRL_SIDESET_BASE_Msk (0x1FU << PIO_SM_PINCTRL_SIDESET_BASE_Pos)
#define PIO_SM_PINCTRL_IN_BASE_Pos      15U
#define PIO_SM_PINCTRL_IN_BASE_Msk      (0x1FU << PIO_SM_PINCTRL_IN_BASE_Pos)
#define PIO_SM_PINCTRL_OUT_COUNT_Pos    20U
#define PIO_SM_PINCTRL_OUT_COUNT_Msk    (0x3FU << PIO_SM_PINCTRL_OUT_COUNT_Pos)
#define PIO_SM_PINCTRL_SET_COUNT_Pos    26U
#define PIO_SM_PINCTRL_SET_COUNT_Msk    (0x7U << PIO_SM_PINCTRL_SET_COUNT_Pos)
#define PIO_SM_PINCTRL_SIDESET_COUNT_Pos 29U
#define PIO_SM_PINCTRL_SIDESET_COUNT_Msk (0x7U << PIO_SM_PINCTRL_SIDESET_COUNT_Pos)
/** @} */

/**
 * @name    PIO interrupt bits (for IRQ0_INTE/IRQ1_INTE/IRQ0_INTS/IRQ1_INTS)
 * @{
 */
#define PIO_IRQ_RXNEMPTY(n)             (1U << (n))
#define PIO_IRQ_TXNFULL(n)              (1U << ((n) + 4U))
#define PIO_IRQ_SM(n)                   (1U << ((n) + 8U))
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/**
 * @brief   Type of a PIO ISR callback.
 *
 * @param[in] p         parameter for the registered function
 * @param[in] ints      content of the IRQn_INTS register
 */
typedef void (*rp_pioisr_t)(void *p, uint32_t ints);

/**
 * @brief   RP PIO block descriptor structure.
 */
typedef struct {
  PIO_TypeDef           *pio;           /**< @brief Associated PIO block.   */
  uint32_t              pioidx;         /**< @brief Block index (0..2).     */
  uint32_t              resets_mask;    /**< @brief RESETS bit for this PIO.*/
} rp_pio_block_t;

/**
 * @brief   RP PIO state machine descriptor structure.
 */
typedef struct {
  const rp_pio_block_t  *block;         /**< @brief Owning PIO block.                 */
  uint32_t              smidx;          /**< @brief state machine index within block. */
  uint32_t              smmask;         /**< @brief state machine bit mask (1 << idx).*/
} rp_pio_sm_t;

/**
 * @brief   PIO program descriptor.
 */
typedef struct {
  const uint16_t        *instructions;  /**< @brief Instruction array.     */
  uint32_t              length;         /**< @brief Number of instructions.*/
  int32_t               origin;         /**< @brief Load offset, -1 = any. */
} rp_pio_program_t;

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/**
 * @brief   Returns a pointer to a PIO block descriptor.
 *
 * @param[in] n         the PIO block index (0, 1, or 2)
 */
#define RP_PIO_BLOCK(n)                 (&__rp_pio_blocks[n])

#define RP_PIO0_BLOCK                   RP_PIO_BLOCK(0)
#define RP_PIO1_BLOCK                   RP_PIO_BLOCK(1)
#if RP_HAS_PIO2 == TRUE
#define RP_PIO2_BLOCK                   RP_PIO_BLOCK(2)
#endif

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if !defined(__DOXYGEN__)
extern const rp_pio_block_t __rp_pio_blocks[RP_PIO_NUM_BLOCKS];
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void pioInit(void);
  const rp_pio_sm_t *pioSmAllocI(const rp_pio_block_t *block,
                                  uint32_t smid,
                                  uint32_t irq_priority,
                                  rp_pioisr_t func, void *param);
  const rp_pio_sm_t *pioSmAlloc(const rp_pio_block_t *block,
                                 uint32_t smid,
                                 uint32_t irq_priority,
                                 rp_pioisr_t func, void *param);
  void pioSmFreeI(const rp_pio_sm_t *smp);
  void pioSmFree(const rp_pio_sm_t *smp);
  int32_t pioProgramLoadI(const rp_pio_block_t *block,
                           const rp_pio_program_t *program);
  void pioProgramUnloadI(const rp_pio_block_t *block,
                          int32_t offset, uint32_t length);
  int32_t pioProgramLoad(const rp_pio_block_t *block,
                          const rp_pio_program_t *program);
  void pioProgramUnload(const rp_pio_block_t *block,
                         int32_t offset, uint32_t length);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Driver inline functions.                                                  */
/*===========================================================================*/

/**
 * @brief   Enables a state machine.
 *
 * @param[in] smp       pointer to a rp_pio_sm_t structure
 *
 * @special
 */
__STATIC_INLINE void pioSmEnableX(const rp_pio_sm_t *smp) {

  smp->block->pio->SET.CTRL = PIO_CTRL_SM_ENABLE(smp->smidx);
}

/**
 * @brief   Disables a state machine.
 *
 * @param[in] smp       pointer to a rp_pio_sm_t structure
 *
 * @special
 */
__STATIC_INLINE void pioSmDisableX(const rp_pio_sm_t *smp) {

  smp->block->pio->CLR.CTRL = PIO_CTRL_SM_ENABLE(smp->smidx);
}

/**
 * @brief   Restarts a state machine.
 *
 * @param[in] smp       pointer to a rp_pio_sm_t structure
 *
 * @special
 */
__STATIC_INLINE void pioSmRestartX(const rp_pio_sm_t *smp) {

  smp->block->pio->SET.CTRL = PIO_CTRL_SM_RESTART(smp->smidx);
}

/**
 * @brief   Restarts the clock divider of a state machine.
 *
 * @param[in] smp       pointer to a rp_pio_sm_t structure
 *
 * @special
 */
__STATIC_INLINE void pioSmClkdivRestartX(const rp_pio_sm_t *smp) {

  smp->block->pio->SET.CTRL = PIO_CTRL_CLKDIV_RESTART(smp->smidx);
}

/**
 * @brief   Sets the clock divider of a state machine.
 *
 * @param[in] smp       pointer to a rp_pio_sm_t structure
 * @param[in] clkdiv    CLKDIV register value
 *
 * @special
 */
__STATIC_INLINE void pioSmSetClkdivX(const rp_pio_sm_t *smp,
                                      uint32_t clkdiv) {

  smp->block->pio->SM[smp->smidx].CLKDIV = clkdiv;
}

/**
 * @brief   Sets the execution control of a state machine.
 *
 * @param[in] smp       pointer to a rp_pio_sm_t structure
 * @param[in] execctrl  EXECCTRL register value
 *
 * @special
 */
__STATIC_INLINE void pioSmSetExecctrlX(const rp_pio_sm_t *smp,
                                        uint32_t execctrl) {

  smp->block->pio->SM[smp->smidx].EXECCTRL = execctrl;
}

/**
 * @brief   Sets the shift control of a state machine.
 *
 * @param[in] smp       pointer to a rp_pio_sm_t structure
 * @param[in] shiftctrl SHIFTCTRL register value
 *
 * @special
 */
__STATIC_INLINE void pioSmSetShiftctrlX(const rp_pio_sm_t *smp,
                                         uint32_t shiftctrl) {

  smp->block->pio->SM[smp->smidx].SHIFTCTRL = shiftctrl;
}

/**
 * @brief   Sets the pin control of a state machine.
 *
 * @param[in] smp       pointer to a rp_pio_sm_t structure
 * @param[in] pinctrl   PINCTRL register value
 *
 * @special
 */
__STATIC_INLINE void pioSmSetPinctrlX(const rp_pio_sm_t *smp,
                                       uint32_t pinctrl) {

  smp->block->pio->SM[smp->smidx].PINCTRL = pinctrl;
}

/**
 * @brief   Executes an instruction immediately on a state machine.
 *
 * @param[in] smp       pointer to a rp_pio_sm_t structure
 * @param[in] instr     16-bit PIO instruction
 *
 * @special
 */
__STATIC_INLINE void pioSmExecX(const rp_pio_sm_t *smp,
                                 uint16_t instr) {

  smp->block->pio->SM[smp->smidx].INSTR = instr;
}

/**
 * @brief   Returns the current instruction address of a state machine.
 *
 * @param[in] smp       pointer to a rp_pio_sm_t structure
 * @return              The current program counter value.
 *
 * @special
 */
__STATIC_INLINE uint32_t pioSmGetAddrX(const rp_pio_sm_t *smp) {

  return smp->block->pio->SM[smp->smidx].ADDR;
}

/**
 * @brief   Writes a word to a state machine's TX FIFO.
 * @pre     The TX FIFO must not be full.
 *
 * @param[in] smp       pointer to a rp_pio_sm_t structure
 * @param[in] data      word to write
 *
 * @special
 */
__STATIC_INLINE void pioSmPutX(const rp_pio_sm_t *smp, uint32_t data) {

  smp->block->pio->TXF[smp->smidx] = data;
}

/**
 * @brief   Reads a word from a state machine's RX FIFO.
 * @pre     The RX FIFO must not be empty.
 *
 * @param[in] smp       pointer to a rp_pio_sm_t structure
 * @return              The word read from the FIFO.
 *
 * @special
 */
__STATIC_INLINE uint32_t pioSmGetX(const rp_pio_sm_t *smp) {

  return smp->block->pio->RXF[smp->smidx];
}

/**
 * @brief   Checks if the TX FIFO of a state machine is full.
 *
 * @param[in] smp       pointer to a rp_pio_sm_t structure
 * @return              The FIFO full state.
 * @retval false        if the FIFO is not full.
 * @retval true         if the FIFO is full.
 *
 * @special
 */
__STATIC_INLINE bool pioSmIsTxFullX(const rp_pio_sm_t *smp) {

  return (bool)((smp->block->pio->FSTAT & PIO_FSTAT_TXFULL(smp->smidx)) != 0U);
}

/**
 * @brief   Checks if the TX FIFO of a state machine is empty.
 *
 * @param[in] smp       pointer to a rp_pio_sm_t structure
 * @return              The FIFO empty state.
 * @retval false        if the FIFO is not empty.
 * @retval true         if the FIFO is empty.
 *
 * @special
 */
__STATIC_INLINE bool pioSmIsTxEmptyX(const rp_pio_sm_t *smp) {

  return (bool)((smp->block->pio->FSTAT & PIO_FSTAT_TXEMPTY(smp->smidx)) != 0U);
}

/**
 * @brief   Checks if the RX FIFO of a state machine is full.
 *
 * @param[in] smp       pointer to a rp_pio_sm_t structure
 * @return              The FIFO full state.
 * @retval false        if the FIFO is not full.
 * @retval true         if the FIFO is full.
 *
 * @special
 */
__STATIC_INLINE bool pioSmIsRxFullX(const rp_pio_sm_t *smp) {

  return (bool)((smp->block->pio->FSTAT & PIO_FSTAT_RXFULL(smp->smidx)) != 0U);
}

/**
 * @brief   Checks if the RX FIFO of a state machine is empty.
 *
 * @param[in] smp       pointer to a rp_pio_sm_t structure
 * @return              The FIFO empty state.
 * @retval false        if the FIFO is not empty.
 * @retval true         if the FIFO is empty.
 *
 * @special
 */
__STATIC_INLINE bool pioSmIsRxEmptyX(const rp_pio_sm_t *smp) {

  return (bool)((smp->block->pio->FSTAT & PIO_FSTAT_RXEMPTY(smp->smidx)) != 0U);
}

/**
 * @brief   Clears the TX and RX FIFOs of a state machine.
 * @note    Toggling FJOIN_TX clears both FIFOs (per datasheet).
 *
 * @param[in] smp       pointer to a rp_pio_sm_t structure
 *
 * @special
 */
__STATIC_INLINE void pioSmClearFifosX(const rp_pio_sm_t *smp) {
  uint32_t shiftctrl = smp->block->pio->SM[smp->smidx].SHIFTCTRL;

  /* Toggle FJOIN_TX to flush, then restore.*/
  smp->block->pio->SM[smp->smidx].SHIFTCTRL = shiftctrl ^ PIO_SM_SHIFTCTRL_FJOIN_TX;
  smp->block->pio->SM[smp->smidx].SHIFTCTRL = shiftctrl;
}

/**
 * @brief   Clears FDEBUG flags for a state machine.
 *
 * @param[in] smp       pointer to a rp_pio_sm_t structure
 *
 * @special
 */
__STATIC_INLINE void pioClearDebugX(const rp_pio_sm_t *smp) {

  smp->block->pio->FDEBUG = PIO_FDEBUG_RXSTALL(smp->smidx) |
                             PIO_FDEBUG_RXUNDER(smp->smidx) |
                             PIO_FDEBUG_TXOVER(smp->smidx)  |
                             PIO_FDEBUG_TXSTALL(smp->smidx);
}

/**
 * @brief   Enables interrupts for a state machine on the current core.
 *
 * @param[in] smp       pointer to a rp_pio_sm_t structure
 * @param[in] mask      interrupt mask (combination of PIO_IRQ_* bits)
 *
 * @special
 */
__STATIC_INLINE void pioSmEnableInterruptX(const rp_pio_sm_t *smp,
                                            uint32_t mask) {

  if (SIO->CPUID == 0U) {
    smp->block->pio->SET.IRQ0_INTE = mask;
  }
  else {
    smp->block->pio->SET.IRQ1_INTE = mask;
  }
}

/**
 * @brief   Disables interrupts for a state machine.
 * @note    Clears both cores' INTE unconditionally (avoids CPUID check).
 *
 * @param[in] smp       pointer to a rp_pio_sm_t structure
 * @param[in] mask      interrupt mask (combination of PIO_IRQ_* bits)
 *
 * @special
 */
__STATIC_INLINE void pioSmDisableInterruptX(const rp_pio_sm_t *smp,
                                             uint32_t mask) {

  smp->block->pio->CLR.IRQ0_INTE = mask;
  smp->block->pio->CLR.IRQ1_INTE = mask;
}

/**
 * @brief   Writes a word to the TX FIFO, blocking while full.
 *
 * @param[in] smp       pointer to a rp_pio_sm_t structure
 * @param[in] data      word to write
 *
 * @special
 */
__STATIC_INLINE void pioSmPut(const rp_pio_sm_t *smp, uint32_t data) {

  while (pioSmIsTxFullX(smp))
    ;
  pioSmPutX(smp, data);
}

/**
 * @brief   Reads a word from the RX FIFO, blocking while empty.
 *
 * @param[in] smp       pointer to a rp_pio_sm_t structure
 * @return              The word read from the FIFO.
 *
 * @special
 */
__STATIC_INLINE uint32_t pioSmGet(const rp_pio_sm_t *smp) {

  while (pioSmIsRxEmptyX(smp))
    ;
  return pioSmGetX(smp);
}

/**
 * @brief   Routes a GPIO pin to the PIO block that owns this state machine.
 * @details Sets IO_BANK0 FUNCSEL for the given pin to PIO0, PIO1, or PIO2
 *          based on the block index of the state machine.
 *
 * @param[in] smp       pointer to a rp_pio_sm_t structure
 * @param[in] gpio      GPIO pin number
 *
 * @special
 */
__STATIC_INLINE void pioSmSetPinFunctionX(const rp_pio_sm_t *smp,
                                           uint32_t gpio) {
  static const uint32_t funcsel[] = {
    RP_PIO_FUNCSEL_PIO0,
    RP_PIO_FUNCSEL_PIO1,
#if RP_HAS_PIO2 == TRUE
    RP_PIO_FUNCSEL_PIO2,
#endif
  };

  IO_BANK0->GPIO[gpio].CTRL = funcsel[smp->block->pioidx];
}

/**
 * @brief   Sets the program counter of a state machine.
 * @details Executes a JMP instruction to the given address.
 *
 * @param[in] smp       pointer to a rp_pio_sm_t structure
 * @param[in] addr      target instruction address (0..31)
 *
 * @special
 */
__STATIC_INLINE void pioSmSetPCX(const rp_pio_sm_t *smp, uint32_t addr) {
  pioSmExecX(smp, (uint16_t)(addr & 0x1FU));
}

/**
 * @brief   Sets the clock divider from a target frequency.
 * @details Computes the integer and fractional divider from the system
 *          clock frequency and the desired PIO clock rate.
 *
 * @param[in] smp       pointer to a rp_pio_sm_t structure
 * @param[in] freq_hz   desired PIO clock frequency in Hz
 *
 * @special
 */
__STATIC_INLINE void pioSmSetFrequencyX(const rp_pio_sm_t *smp, uint32_t freq_hz) {
  uint32_t div_fp8 = ((uint64_t)RP_CLK_SYS_FREQ << 8) / freq_hz;
  uint32_t int_part = div_fp8 >> 8;
  uint32_t frac_part = div_fp8 & 0xFFU;

  pioSmSetClkdivX(smp, PIO_SM_CLKDIV(int_part, frac_part));
}

#endif /* RP_PIO_H */

/** @} */
