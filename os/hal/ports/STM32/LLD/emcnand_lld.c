/*
    ChibiOS/HAL - Copyright (C) 2006-2014 Giovanni Di Sirio

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
/*
   Concepts and parts of this file have been contributed by Uladzimir Pylinsky
   aka barthess.
 */

/**
 * @file    emcnand_lld.c
 * @brief   EMCNAND Driver subsystem low level driver source template.
 *
 * @addtogroup EMCNAND
 * @{
 */

#include "hal.h"

#if HAL_USE_EMCNAND || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/
#define EMCNAND_DMA_CHANNEL                                                 \
  STM32_DMA_GETCHANNEL(STM32_EMCNAND_EMCNAND1_DMA_STREAM,                   \
                       STM32_EMC_DMA_CHN)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/**
 * @brief   EMCNAND1 driver identifier.
 */
#if STM32_EMCNAND_USE_EMCNAND1 || defined(__DOXYGEN__)
EMCNANDDriver EMCNANDD1;
#endif

/**
 * @brief   EMCNAND2 driver identifier.
 */
#if STM32_EMCNAND_USE_EMCNAND2 || defined(__DOXYGEN__)
EMCNANDDriver EMCNANDD2;
#endif

/*===========================================================================*/
/* Driver local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/
/**
 * @brief   Wakes up the waiting thread.
 *
 * @param[in] emcnandp  pointer to the @p EMCNANDDriver object
 * @param[in] msg       wakeup message
 *
 * @notapi
 */
static void wakeup_isr(EMCNANDDriver *emcnandp){

  osalDbgCheck(emcnandp->thread != NULL);
  osalThreadResumeI(&emcnandp->thread, MSG_OK);
}

/**
 * @brief   Put calling thread in suspend and switch driver state
 *
 * @param[in] emcnandp    pointer to the @p EMCNANDDriver object
 */
static void emcnand_lld_suspend_thread(EMCNANDDriver *emcnandp) {

  //emcnandp->thread = chThdGetSelfX();
  osalThreadSuspendS(&emcnandp->thread);
}

/**
 * @brief   Caclulate ECCPS register value
 *
 * @param[in] emcnandp    pointer to the @p EMCNANDDriver object
 */
static uint32_t calc_eccps(EMCNANDDriver *emcnandp){

  uint32_t i = 0;
  uint32_t eccps = emcnandp->config->page_data_size;

  eccps = eccps >> 9;
  while (eccps > 0){
    i++;
    eccps >>= 1;
  }

  return i << 17;
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

#if STM32_EMC_USE_INT
/**
 * @brief   Enable interrupts from FSMC
 *
 * @param[in] emcnandp    pointer to the @p EMCNANDDriver object
 *
 * @notapi
 */
static void emcnand_ready_isr_enable(EMCNANDDriver *emcnandp) {
  emcnandp->nand->SR |= FSMC_SR_IREN;
  osalSysHalt("Function untested");
}

/**
 * @brief   Disable interrupts from FSMC
 *
 * @param[in] emcnandp    pointer to the @p EMCNANDDriver object
 *
 * @notapi
 */
static void emcnand_ready_isr_disable(EMCNANDDriver *emcnandp) {
  emcnandp->nand->SR &= ~FSMC_SR_IREN;
  osalSysHalt("Function untested");
}

/**
 * @brief   Ready interrupt handler
 *
 * @param[in] emcnandp    pointer to the @p EMCNANDDriver object
 * @param[in] flags       flags passed from FSMC intrrupt handler
 *
 * @notapi
 */
static void emcnand_isr_handler (EMCNANDDriver *emcnandp,
                                                emcnandflags_t flags){
  (void)emcnandp;
  (void)flags;

  osalSysHalt("Unrealized");
}
#else /* STM32_EMC_USE_INT */
/**
 * @brief   Disable interrupts from EXTI
 *
 * @param[in] emcnandp    pointer to the @p EMCNANDDriver object
 *
 * @notapi
 */
static void emcnand_ready_isr_enable(EMCNANDDriver *emcnandp) {
  emcnandp->config->ext_isr_enable();
}

/**
 * @brief   Enable interrupts from EXTI
 *
 * @param[in] emcnandp    pointer to the @p EMCNANDDriver object
 *
 * @notapi
 */
static void emcnand_ready_isr_disable(EMCNANDDriver *emcnandp) {
  emcnandp->config->ext_isr_disable();
}

/**
 * @brief   Ready pin interrupt handler.
 *
 * @param[in] emcnandp    pointer to the @p EMCNANDDriver object
 *
 * @notapi
 */
static void emcnand_isr_handler(EMCNANDDriver *emcnandp){

  osalSysLockFromISR();

  switch (emcnandp->state){
  case EMCNAND_READ:
    emcnandp->state = EMCNAND_DMA_RX;
    dmaStartMemCopy(emcnandp->dma, emcnandp->dmamode,
                   emcnandp->map_data, emcnandp->rxdata, emcnandp->datalen);
    /* thread will be woked up from DMA ISR */
    break;

  case EMCNAND_ERASE:
    /* NAND reports about erase finish */
    emcnandp->state = EMCNAND_READY;
    wakeup_isr(emcnandp);
    break;

  case EMCNAND_PROGRAM:
    /* NAND reports about page programming finish */
    emcnandp->state = EMCNAND_READY;
    wakeup_isr(emcnandp);
    break;

  default:
    osalSysHalt("Unhandled case");
    break;
  }

  osalSysUnlockFromISR();
}
#endif /* STM32_EMC_USE_INT */

/**
 * @brief   DMA RX end IRQ handler.
 *
 * @param[in] emcnandp    pointer to the @p EMCNANDDriver object
 * @param[in] flags       pre-shifted content of the ISR register
 *
 * @notapi
 */
static void emcnand_lld_serve_transfer_end_irq(EMCNANDDriver *emcnandp,
                                               uint32_t flags) {
  /* DMA errors handling.*/
#if defined(STM32_EMCNAND_DMA_ERROR_HOOK)
  if ((flags & (STM32_DMA_ISR_TEIF | STM32_DMA_ISR_DMEIF)) != 0) {
    STM32_EMCNAND_DMA_ERROR_HOOK(emcnandp);
  }
#else
  (void)flags;
#endif

  osalSysLockFromISR();

  dmaStreamDisable(emcnandp->dma);

  switch (emcnandp->state){
  case EMCNAND_DMA_TX:
    emcnandp->state = EMCNAND_PROGRAM;
    emcnandp->map_cmd[0] = NAND_CMD_PAGEPROG;
    /* thread will be woken from ready_isr() */
    break;

  case EMCNAND_DMA_RX:
    emcnandp->state = EMCNAND_READY;
    emcnandp->rxdata = NULL;
    emcnandp->datalen = 0;
    wakeup_isr(emcnandp);
    break;

  default:
    osalSysHalt("Unhandled case");
    break;
  }

  osalSysUnlockFromISR();
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level EMCNAND driver initialization.
 *
 * @notapi
 */
void emcnand_lld_init(void) {

#if STM32_EMCNAND_USE_EMCNAND1
  /* Driver initialization.*/
  emcnandObjectInit(&EMCNANDD1);
  EMCNANDD1.rxdata   = NULL;
  EMCNANDD1.datalen  = 0;
  EMCNANDD1.thread   = NULL;
  EMCNANDD1.dma      = STM32_DMA_STREAM(STM32_EMCNAND_EMCNAND1_DMA_STREAM);
  EMCNANDD1.nand     = (FSMC_NAND_TypeDef *)FSMC_Bank2_R_BASE;
  EMCNANDD1.map_data = (uint8_t*)FSMC_Bank2_MAP_COMMON_DATA;
  EMCNANDD1.map_cmd  = (uint8_t*)FSMC_Bank2_MAP_COMMON_CMD;
  EMCNANDD1.map_addr = (uint8_t*)FSMC_Bank2_MAP_COMMON_ADDR;
#endif /* STM32_EMCNAND_USE_EMCNAND1 */

#if STM32_EMCNAND_USE_EMCNAND2
  /* Driver initialization.*/
  #warning "Untested"
  emcnandObjectInit(&EMCNANDD1);
  EMCNANDD2.rxdata   = NULL;
  EMCNANDD2.datalen  = 0;
  EMCNANDD2.thread   = NULL;
  EMCNANDD2.dma      = STM32_DMA_STREAM(STM32_EMCNAND_EMCNAND2_DMA_STREAM);
  EMCNANDD2.nand     = (FSMC_NAND_TypeDef *)FSMC_Bank3_R_BASE;
  EMCNANDD2.map_data = (uint8_t*)FSMC_Bank3_MAP_COMMON_DATA;
  EMCNANDD2.map_cmd  = (uint8_t*)FSMC_Bank3_MAP_COMMON_CMD;
  EMCNANDD2.map_addr = (uint8_t*)FSMC_Bank3_MAP_COMMON_ADDR;
#endif /* STM32_EMCNAND_USE_EMCNAND2 */
}

/**
 * @brief   Configures and activates the EMCNAND peripheral.
 *
 * @param[in] emcnandp      pointer to the @p EMCNANDDriver object
 *
 * @notapi
 */
void emcnand_lld_start(EMCNANDDriver *emcnandp) {

  bool_t b;

  if (emcnandp->state == EMCNAND_STOP) {
    b = dmaStreamAllocate(emcnandp->dma,
                          STM32_EMC_FSMC1_IRQ_PRIORITY,
                          (stm32_dmaisr_t)emcnand_lld_serve_transfer_end_irq,
                          (void *)emcnandp);
    osalDbgAssert(!b, "stream already allocated");
    emcnandp->dmamode = STM32_DMA_CR_CHSEL(EMCNAND_DMA_CHANNEL) |
                        STM32_DMA_CR_PL(STM32_EMCNAND_EMCNAND1_DMA_PRIORITY) |
                        STM32_DMA_CR_PSIZE_BYTE     | STM32_DMA_CR_MSIZE_BYTE |
                        STM32_DMA_CR_DMEIE          | STM32_DMA_CR_TEIE |
                        STM32_DMA_CR_TCIE;
    /* dmaStreamSetFIFO(emcnandp->dma,
                    STM32_DMA_FCR_DMDIS | EMCNAND_STM32_DMA_FCR_FTH_LVL); */
    emcnandp->nand->PCR = calc_eccps(emcnandp) | FSMC_PCR_PTYP | FSMC_PCR_PBKEN;
    emcnandp->nand->PMEM = emcnandp->config->pmem;
    emcnandp->nand->PATT = emcnandp->config->pmem;
    emcnandp->isr_handler = emcnand_isr_handler;
    emcnand_ready_isr_enable(emcnandp);
  }
}

/**
 * @brief   Deactivates the EMCNAND peripheral.
 *
 * @param[in] emcnandp      pointer to the @p EMCNANDDriver object
 *
 * @notapi
 */
void emcnand_lld_stop(EMCNANDDriver *emcnandp) {

  if (emcnandp->state == EMCNAND_READY) {
    dmaStreamRelease(emcnandp->dma);
    emcnandp->nand->PCR &= ~FSMC_PCR_PBKEN;
    emcnand_ready_isr_disable(emcnandp);
    emcnandp->isr_handler = NULL;
  }
}

/**
 * @brief   Read data from NAND.
 *
 * @param[in] emcnandp      pointer to the @p EMCNANDDriver object
 * @param[out] data         pointer to data buffer
 * @param[in] datalen       size of data buffer
 * @param[in] addr          pointer to address buffer
 * @param[in] addrlen       length of address
 * @param[out] ecc          pointer to store computed ECC. Ignored when NULL.
 *
 * @notapi
 */
void emcnand_lld_read_data(EMCNANDDriver *emcnandp, uint8_t *data,
                size_t datalen, uint8_t *addr, size_t addrlen, uint32_t *ecc){

  emcnandp->state = EMCNAND_READ;
  emcnandp->rxdata = data;
  emcnandp->datalen = datalen;

  emcnand_lld_write_cmd (emcnandp, NAND_CMD_READ0);
  emcnand_lld_write_addr(emcnandp, addr, addrlen);
  osalSysLock();
  emcnand_lld_write_cmd (emcnandp, NAND_CMD_READ0_CONFIRM);

  /* Here NAND asserts busy signal and starts transferring from memory
     array to page buffer. After the end of transmission ready_isr functions
     starts DMA transfer from page buffer to MCU's RAM.*/
  osalDbgAssert((emcnandp->nand->PCR & FSMC_PCR_ECCEN) == 0,
          "State machine broken. ECCEN must be previously disabled.");

  if (NULL != ecc){
    emcnandp->nand->PCR |= FSMC_PCR_ECCEN;
  }

  emcnand_lld_suspend_thread(emcnandp);
  osalSysUnlock();

  /* thread was woken up from DMA ISR */
  if (NULL != ecc){
    while (! (emcnandp->nand->SR & FSMC_SR_FEMPT))
      ;
    *ecc = emcnandp->nand->ECCR;
    emcnandp->nand->PCR &= ~FSMC_PCR_ECCEN;
  }
}

/**
 * @brief   Write data to NAND.
 *
 * @param[in] emcnandp      pointer to the @p EMCNANDDriver object
 * @param[in] data          buffer with data to be written
 * @param[in] datalen       size of data buffer
 * @param[in] addr          pointer to address buffer
 * @param[in] addrlen       length of address
 * @param[out] ecc          pointer to store computed ECC. Ignored when NULL.
 *
 * @return    The operation status reported by NAND IC (0x70 command).
 *
 * @notapi
 */
uint8_t emcnand_lld_write_data(EMCNANDDriver *emcnandp, const uint8_t *data,
                size_t datalen, uint8_t *addr, size_t addrlen, uint32_t *ecc){

  emcnandp->state = EMCNAND_WRITE;

  emcnand_lld_write_cmd (emcnandp, NAND_CMD_WRITE);
  osalSysLock();
  emcnand_lld_write_addr(emcnandp, addr, addrlen);

  /* Now start DMA transfer to NAND buffer and put thread in sleep state.
     Tread will we woken up from ready ISR. */
  emcnandp->state = EMCNAND_DMA_TX;
  osalDbgAssert((emcnandp->nand->PCR & FSMC_PCR_ECCEN) == 0,
          "State machine broken. ECCEN must be previously disabled.");

  if (NULL != ecc){
    emcnandp->nand->PCR |= FSMC_PCR_ECCEN;
  }

  dmaStartMemCopy(emcnandp->dma, emcnandp->dmamode,
                           data, emcnandp->map_data, datalen);

  emcnand_lld_suspend_thread(emcnandp);
  osalSysUnlock();

  if (NULL != ecc){
    while (! (emcnandp->nand->SR & FSMC_SR_FEMPT))
      ;
    *ecc = emcnandp->nand->ECCR;
    emcnandp->nand->PCR &= ~FSMC_PCR_ECCEN;
  }

  return emcnand_lld_read_status(emcnandp);
}

/**
 * @brief   Erase block.
 *
 * @param[in] emcnandp      pointer to the @p EMCNANDDriver object
 * @param[in] addr          pointer to address buffer
 * @param[in] addrlen       length of address
 *
 * @return    The operation status reported by NAND IC (0x70 command).
 *
 * @notapi
 */
uint8_t emcnand_lld_erase(EMCNANDDriver *emcnandp,
                                        uint8_t *addr, size_t addrlen){

  emcnandp->state = EMCNAND_ERASE;

  emcnand_lld_write_cmd (emcnandp, NAND_CMD_ERASE);
  emcnand_lld_write_addr(emcnandp, addr, addrlen);
  osalSysLock();
  emcnand_lld_write_cmd (emcnandp, NAND_CMD_ERASE_CONFIRM);
  emcnand_lld_suspend_thread(emcnandp);
  osalSysUnlock();

  return emcnand_lld_read_status(emcnandp);
}

/**
 * @brief     Read data from NAND using polling approach.
 *
 * @detatils  Use this function to read data when no waiting expected. For
 *            Example read status word after 0x70 command
 *
 * @param[in] emcnandp      pointer to the @p EMCNANDDriver object
 * @param[out] data         pointer to output buffer
 * @param[in] len           length of data to be read
 *
 * @notapi
 */
void emcnand_lld_polled_read_data(EMCNANDDriver *emcnandp,
                                                  uint8_t *data, size_t len){
  size_t i = 0;

  for (i=0; i<len; i++)
    data[i] = emcnandp->map_data[i];
}

/**
 * @brief   Send addres to NAND.
 *
 * @param[in] emcnandp      pointer to the @p EMCNANDDriver object
 * @param[in] len           length of address array
 * @param[in] addr          pointer to address array
 *
 * @notapi
 */
void emcnand_lld_write_addr(EMCNANDDriver *emcnandp,
                                            const uint8_t *addr, size_t len){
  size_t i = 0;

  for (i=0; i<len; i++)
    emcnandp->map_addr[i] = addr[i];
}

/**
 * @brief   Send command to NAND.
 *
 * @param[in] emcnandp      pointer to the @p EMCNANDDriver object
 * @param[in] cmd           command value
 *
 * @notapi
 */
void emcnand_lld_write_cmd(EMCNANDDriver *emcnandp, uint8_t cmd){
  emcnandp->map_cmd[0] = cmd;
}

/**
 * @brief   Read status byte from NAND.
 *
 * @param[in] emcnandp      pointer to the @p EMCNANDDriver object
 *
 * @return    Status byte.
 *
 * @notapi
 */
uint8_t emcnand_lld_read_status(EMCNANDDriver *emcnandp) {

  uint8_t status[1] = {0x01}; /* presume worse */

  emcnand_lld_write_cmd(emcnandp, NAND_CMD_STATUS);
  emcnand_lld_polled_read_data(emcnandp, status, 1);

  return status[0];
}

#endif /* HAL_USE_EMCNAND */

/** @} */

