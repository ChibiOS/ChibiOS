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
 * @file    PIOv1/rp_pio.c
 * @brief   PIO helper driver code.
 *
 * @addtogroup RP_PIO
 * @details PIO state machine allocation driver.
 * @{
 */

#include "hal.h"

#if defined(RP_PIO_REQUIRED) || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   PIO block descriptors.
 */
const rp_pio_block_t __rp_pio_blocks[RP_PIO_NUM_BLOCKS] = {
  {PIO0, 0U, RESETS_ALLREG_PIO0},
  {PIO1, 1U, RESETS_ALLREG_PIO1},
#if RP_HAS_PIO2 == TRUE
  {PIO2, 2U, RESETS_ALLREG_PIO2},
#endif
};

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/**
 * @brief   PIO state machine descriptors.
 */
static rp_pio_sm_t pio_sms[RP_PIO_NUM_BLOCKS][RP_PIO_NUM_STATE_MACHINES];

/**
 * @brief   Global PIO-related data structures.
 */
static struct {
  struct {
    /**
     * @brief   Mask of the allocated state machines for core 0.
     */
    uint32_t        c0_allocated_mask;
    /**
     * @brief   Mask of the allocated state machines for core 1.
     */
    uint32_t        c1_allocated_mask;
    /**
     * @brief   Instruction memory allocation bitmap.
     */
    uint32_t        imem_allocated;
    /**
     * @brief   PIO state machine IRQ redirectors.
     */
    struct {
      /**
       * @brief   PIO state Machine callback function.
       */
      rp_pioisr_t   func;
      /**
       * @brief   PIO state machine callback parameter.
       */
      void          *param;
    } sm[RP_PIO_NUM_STATE_MACHINES];
  } blocks[RP_PIO_NUM_BLOCKS];
} pio;

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

static void serve_pio_irq(uint32_t blockidx, __I uint32_t *ints_reg) {
  uint32_t ints;

  ints = *ints_reg;

  if (ints != 0U) {
    unsigned i;

    for (i = 0U; i < RP_PIO_NUM_STATE_MACHINES; i++) {
      if (pio.blocks[blockidx].sm[i].func != NULL) {
        pio.blocks[blockidx].sm[i].func(pio.blocks[blockidx].sm[i].param,ints);
      }
    }
  }
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/**
 * @brief   PIO0 IRQ0 handler for core 0.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(RP_PIO0_IRQ_0_HANDLER) {

  OSAL_IRQ_PROLOGUE();
  serve_pio_irq(0U, &PIO0->IRQ0_INTS);
  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   PIO0 IRQ1 handler for core 1.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(RP_PIO0_IRQ_1_HANDLER) {

  OSAL_IRQ_PROLOGUE();
  serve_pio_irq(0U, &PIO0->IRQ1_INTS);
  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   PIO1 IRQ0 handler for core 0.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(RP_PIO1_IRQ_0_HANDLER) {

  OSAL_IRQ_PROLOGUE();
  serve_pio_irq(1U, &PIO1->IRQ0_INTS);
  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   PIO1 IRQ1 handler for core 1.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(RP_PIO1_IRQ_1_HANDLER) {

  OSAL_IRQ_PROLOGUE();
  serve_pio_irq(1U, &PIO1->IRQ1_INTS);
  OSAL_IRQ_EPILOGUE();
}

#if RP_HAS_PIO2 == TRUE
/**
 * @brief   PIO2 IRQ0 handler for core 0.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(RP_PIO2_IRQ_0_HANDLER) {

  OSAL_IRQ_PROLOGUE();
  serve_pio_irq(2U, &PIO2->IRQ0_INTS);
  OSAL_IRQ_EPILOGUE();
}

/**
 * @brief   PIO2 IRQ1 handler for core 1.
 *
 * @isr
 */
OSAL_IRQ_HANDLER(RP_PIO2_IRQ_1_HANDLER) {

  OSAL_IRQ_PROLOGUE();
  serve_pio_irq(2U, &PIO2->IRQ1_INTS);
  OSAL_IRQ_EPILOGUE();
}
#endif /* RP_HAS_PIO2 */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   PIO helper initialization.
 *
 * @init
 */
void pioInit(void) {
  unsigned b, s;

  for (b = 0U; b < RP_PIO_NUM_BLOCKS; b++) {
    pio.blocks[b].c0_allocated_mask = 0U;
    pio.blocks[b].c1_allocated_mask = 0U;
    pio.blocks[b].imem_allocated    = 0U;
    for (s = 0U; s < RP_PIO_NUM_STATE_MACHINES; s++) {
      pio.blocks[b].sm[s].func = NULL;

      /* Initialize state machine descriptors.*/
      pio_sms[b][s].block  = &__rp_pio_blocks[b];
      pio_sms[b][s].smidx  = s;
      pio_sms[b][s].smmask = 1U << s;
    }
  }
}

/**
 * @brief   Allocates a PIO state machine.
 *
 * @param[in] block     pointer to the PIO block descriptor
 * @param[in] smid      numeric identifier of a specific state machine or:
 *                      - @p RP_PIO_SM_ID_ANY for any state machine.
 *                      .
 * @param[in] irq_priority IRQ priority for the PIO state machine
 * @param[in] func      handling function pointer, can be @p NULL
 * @param[in] param     a parameter to be passed to the handling function
 * @return              Pointer to the allocated @p rp_pio_sm_t structure.
 * @retval NULL         if state machine is not available.
 *
 * @iclass
 */
const rp_pio_sm_t *pioSmAllocI(const rp_pio_block_t *block,
                                uint32_t smid,
                                uint32_t irq_priority,
                                rp_pioisr_t func, void *param) {
  uint32_t b, i, startid, endid;

  osalDbgCheckClassI();
  osalDbgCheck(block != NULL);

  b = block->pioidx;

  if (smid < RP_PIO_SM_ID_ANY) {
    startid = smid;
    endid   = smid;
  }
  else if (smid == RP_PIO_SM_ID_ANY) {
    startid = 0U;
    endid   = RP_PIO_SM_ID_ANY - 1U;
  }
  else {
    osalDbgCheck(false);
    return NULL;
  }

  for (i = startid; i <= endid; i++) {
    uint32_t prevmask = pio.blocks[b].c0_allocated_mask |
                        pio.blocks[b].c1_allocated_mask;
    uint32_t smmask = 1U << i;

    if ((prevmask & smmask) == 0U) {

      /* Installs the PIO handler.*/
      pio.blocks[b].sm[i].func  = func;
      pio.blocks[b].sm[i].param = param;

      /* Releasing PIO reset if this is the first state machine taken.*/
      if (prevmask == 0U) {
        rp_peripheral_unreset(block->resets_mask);
      }

      if (SIO->CPUID == 0U) {
        /* state machine taken by core 0.*/
        if (pio.blocks[b].c0_allocated_mask == 0U) {
          switch (b) {
          case 0U:
            nvicEnableVector(RP_PIO0_IRQ_0_NUMBER, irq_priority);
            break;
          case 1U:
            nvicEnableVector(RP_PIO1_IRQ_0_NUMBER, irq_priority);
            break;
#if RP_HAS_PIO2 == TRUE
          case 2U:
            nvicEnableVector(RP_PIO2_IRQ_0_NUMBER, irq_priority);
            break;
#endif
          default:
            break;
          }
        }
        pio.blocks[b].c0_allocated_mask |= smmask;
      }
      else {
        /* state machine taken by core 1.*/
        if (pio.blocks[b].c1_allocated_mask == 0U) {
          switch (b) {
          case 0U:
            nvicEnableVector(RP_PIO0_IRQ_1_NUMBER, irq_priority);
            break;
          case 1U:
            nvicEnableVector(RP_PIO1_IRQ_1_NUMBER, irq_priority);
            break;
#if RP_HAS_PIO2 == TRUE
          case 2U:
            nvicEnableVector(RP_PIO2_IRQ_1_NUMBER, irq_priority);
            break;
#endif
          default:
            break;
          }
        }
        pio.blocks[b].c1_allocated_mask |= smmask;
      }

      return &pio_sms[b][i];
    }
  }

  return NULL;
}

/**
 * @brief   Allocates a PIO state machine.
 *
 * @param[in] block     pointer to the PIO block descriptor
 * @param[in] smid      numeric identifier of a specific state machine or:
 *                      - @p RP_PIO_SM_ID_ANY for any state machine.
 *                      .
 * @param[in] irq_priority IRQ priority for the PIO state machine
 * @param[in] func      handling function pointer, can be @p NULL
 * @param[in] param     a parameter to be passed to the handling function
 * @return              Pointer to the allocated @p rp_pio_sm_t structure.
 * @retval NULL         if state machine is not available.
 *
 * @api
 */
const rp_pio_sm_t *pioSmAlloc(const rp_pio_block_t *block,
                               uint32_t smid,
                               uint32_t irq_priority,
                               rp_pioisr_t func, void *param) {
  const rp_pio_sm_t *smp;

  osalSysLock();
  smp = pioSmAllocI(block, smid, irq_priority, func, param);
  osalSysUnlock();

  return smp;
}

/**
 * @brief   Releases a PIO state machine.
 *
 * @param[in] smp       pointer to a rp_pio_sm_t structure
 *
 * @iclass
 */
void pioSmFreeI(const rp_pio_sm_t *smp) {
  uint32_t b;

  osalDbgCheck(smp != NULL);

  b = smp->block->pioidx;

  /* Check if the state machine is allocated.*/
  osalDbgAssert(((pio.blocks[b].c0_allocated_mask |
                  pio.blocks[b].c1_allocated_mask) & smp->smmask) != 0U,
                "not allocated");

  /* Disable the state machine.*/
  pioSmDisableX(smp);

  /* Disabling state machine interrupts for both cores.*/
  pioSmDisableInterruptX(smp, PIO_IRQ_RXNEMPTY(smp->smidx) |
                               PIO_IRQ_TXNFULL(smp->smidx)  |
                               PIO_IRQ_SM(smp->smidx));

  if (SIO->CPUID == 0U) {
    /* state machine released by core 0.*/
    pio.blocks[b].c0_allocated_mask &= ~smp->smmask;
    if (pio.blocks[b].c0_allocated_mask == 0U) {
      switch (b) {
      case 0U:
        nvicDisableVector(RP_PIO0_IRQ_0_NUMBER);
        break;
      case 1U:
        nvicDisableVector(RP_PIO1_IRQ_0_NUMBER);
        break;
#if RP_HAS_PIO2 == TRUE
      case 2U:
        nvicDisableVector(RP_PIO2_IRQ_0_NUMBER);
        break;
#endif
      default:
        break;
      }
    }
  }
  else {
    /* state machine released by core 1.*/
    pio.blocks[b].c1_allocated_mask &= ~smp->smmask;
    if (pio.blocks[b].c1_allocated_mask == 0U) {
      switch (b) {
      case 0U:
        nvicDisableVector(RP_PIO0_IRQ_1_NUMBER);
        break;
      case 1U:
        nvicDisableVector(RP_PIO1_IRQ_1_NUMBER);
        break;
#if RP_HAS_PIO2 == TRUE
      case 2U:
        nvicDisableVector(RP_PIO2_IRQ_1_NUMBER);
        break;
#endif
      default:
        break;
      }
    }
  }

  /* Remove the PIO handler.*/
  pio.blocks[b].sm[smp->smidx].func  = NULL;
  pio.blocks[b].sm[smp->smidx].param = NULL;

  /* Reset PIO block if no state machines remain.*/
  if ((pio.blocks[b].c0_allocated_mask |
       pio.blocks[b].c1_allocated_mask) == 0U) {
    rp_peripheral_reset(smp->block->resets_mask);
  }
}

/**
 * @brief   Releases a PIO state machine.
 *
 * @param[in] smp       pointer to a rp_pio_sm_t structure
 *
 * @api
 */
void pioSmFree(const rp_pio_sm_t *smp) {

  osalSysLock();
  pioSmFreeI(smp);
  osalSysUnlock();
}

/**
 * @brief   Loads a PIO program into instruction memory.
 *
 * @param[in] block     pointer to the PIO block descriptor
 * @param[in] program   pointer to the program descriptor
 * @return              The offset at which the program was loaded.
 * @retval -1           if the program could not be loaded.
 *
 * @iclass
 */
int32_t pioProgramLoadI(const rp_pio_block_t *block,
                         const rp_pio_program_t *program) {
  uint32_t b, i, offset, length, mask;

  osalDbgCheckClassI();
  osalDbgCheck(block != NULL);
  osalDbgCheck(program != NULL);
  osalDbgCheck(program->length > 0U);
  osalDbgCheck(program->length <= RP_PIO_NUM_INSTR_MEM);

  b = block->pioidx;
  length = program->length;

  if (program->origin >= 0) {
    /* Check that the requested slots are free.*/
    offset = (uint32_t)program->origin;
    osalDbgCheck(offset + length <= RP_PIO_NUM_INSTR_MEM);

    mask = ((1U << length) - 1U) << offset;
    if ((pio.blocks[b].imem_allocated & mask) != 0U) {
      return -1;
    }
  }
  else {
    /* Find first contiguous run of free slots.*/
    uint32_t found = 0U;

    for (offset = 0U; offset <= RP_PIO_NUM_INSTR_MEM - length; offset++) {
      mask = ((1U << length) - 1U) << offset;
      if ((pio.blocks[b].imem_allocated & mask) == 0U) {
        found = 1U;
        break;
      }
    }
    if (found == 0U) {
      return -1;
    }
  }

  /* Write the instructions to memory.*/
  for (i = 0U; i < length; i++) {
    block->pio->INSTR_MEM[offset + i] = program->instructions[i];
  }

  /* Mark slots as used.*/
  pio.blocks[b].imem_allocated |= ((1U << length) - 1U) << offset;

  return (int32_t)offset;
}

/**
 * @brief   Unloads a PIO program from instruction memory.
 *
 * @param[in] block     pointer to the PIO block descriptor
 * @param[in] offset    the offset at which the program was loaded
 * @param[in] length    number of instructions to free
 *
 * @iclass
 */
void pioProgramUnloadI(const rp_pio_block_t *block,
                        int32_t offset, uint32_t length) {
  uint32_t b, i, mask;

  osalDbgCheckClassI();
  osalDbgCheck(block != NULL);
  osalDbgCheck(offset >= 0);
  osalDbgCheck(((uint32_t)offset + length) <= RP_PIO_NUM_INSTR_MEM);

  b = block->pioidx;
  mask = ((1U << length) - 1U) << (uint32_t)offset;

  osalDbgAssert((pio.blocks[b].imem_allocated & mask) == mask,
                "not allocated");

  /* Zero out memory.*/
  for (i = 0U; i < length; i++) {
    block->pio->INSTR_MEM[(uint32_t)offset + i] = 0U;
  }

  /* Free slots.*/
  pio.blocks[b].imem_allocated &= ~mask;
}

/**
 * @brief   Loads a PIO program into instruction memory.
 *
 * @param[in] block     pointer to the PIO block descriptor
 * @param[in] program   pointer to the program descriptor
 * @return              The offset at which the program was loaded.
 * @retval -1           if the program could not be loaded.
 *
 * @api
 */
int32_t pioProgramLoad(const rp_pio_block_t *block,
                        const rp_pio_program_t *program) {
  int32_t offset;

  osalSysLock();
  offset = pioProgramLoadI(block, program);
  osalSysUnlock();

  return offset;
}

/**
 * @brief   Unloads a PIO program from instruction memory.
 *
 * @param[in] block     pointer to the PIO block descriptor
 * @param[in] offset    the offset at which the program was loaded
 * @param[in] length    number of instructions to free
 *
 * @api
 */
void pioProgramUnload(const rp_pio_block_t *block,
                       int32_t offset, uint32_t length) {

  osalSysLock();
  pioProgramUnloadI(block, offset, length);
  osalSysUnlock();
}

#endif /* RP_PIO_REQUIRED */

/** @} */
