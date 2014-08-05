/*
    ChibiOS/RT - Copyright (C) 2006-2014 Giovanni Di Sirio

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

/*
 * Hardware notes.
 *
 * Use _external_ pullup on ready/busy pin of NAND IC.
 *
 * Chose MCU with 140 (or more) pins package because 100 pins packages
 * has no dedicated interrupt pins for FSMC.
 *
 * If your hardware already done using 100 pin package than you have to:
 * 1) connect ready/busy pin to GPIOD6 (NWAIT in terms of STM32)
 * 2) set GPIOD6 pin as input with pullup and connect it to alternate
 * function0 (not function12)
 * 3) set up EXTI to catch raising edge on GPIOD6 and call NAND driver's
 * isr_handler() function from an EXTI callback.
 *
 * If you use MLC flash memory do NOT use ECC to detect/correct
 * errors because of its weakness. Use Rid-Solomon on BCH code instead.
 * Yes, you have to realize it in sowftware yourself.
 */

#include "ch.h"
#include "hal.h"

#include "dma_storm.h"
#include "string.h"
#include "stdlib.h"

/*
 ******************************************************************************
 * DEFINES
 ******************************************************************************
 */

#define EMCNAND_TIME_SET          ((uint32_t) 2) //(8nS)
#define EMCNAND_TIME_WAIT         ((uint32_t) 6) //(30nS)
#define EMCNAND_TIME_HOLD         ((uint32_t) 1) //(5nS)
#define EMCNAND_TIME_HIZ          ((uint32_t) 4) //(20nS)

#define NAND_BLOCKS_COUNT         8192
#define NAND_PAGE_DATA_SIZE       2048
#define NAND_PAGE_SPARE_SIZE      64
#define NAND_PAGE_SIZE            (NAND_PAGE_SPARE_SIZE + NAND_PAGE_DATA_SIZE)
#define NAND_PAGES_PER_BLOCK      64
#define NAND_ROW_WRITE_CYCLES     3
#define NAND_COL_WRITE_CYCLES     2

/* statuses returning by NAND IC on 0x70 command */
#define NAND_STATUS_OP_FAILED     ((uint8_t)1 << 0)
#define NAND_STATUS_READY         ((uint8_t)1 << 6)
#define NAND_STATUS_NOT_RPOTECTED ((uint8_t)1 << 7)

#define EMCNAND_USE_KILL_TEST     TRUE

/*
 ******************************************************************************
 * EXTERNS
 ******************************************************************************
 */

/*
 ******************************************************************************
 * PROTOTYPES
 ******************************************************************************
 */
static void ready_isr_enable(void);
static void ready_isr_disable(void);
static void nand_ready_cb(EXTDriver *extp, expchannel_t channel);

/*
 ******************************************************************************
 * GLOBAL VARIABLES
 ******************************************************************************
 */
/*
 *
 */
static uint8_t nand_buf[NAND_PAGE_SIZE];
static uint8_t ref_buf[NAND_PAGE_SIZE];

/*
 *
 */
//static TimeMeasurement tmu_erase;
//static TimeMeasurement tmu_write_data;
//static TimeMeasurement tmu_write_spare;
//static TimeMeasurement tmu_read_data;
//static TimeMeasurement tmu_read_spare;

/*
 *
 */
static const EMCConfig emccfg = {};

#if EMCNAND_USE_BAD_MAP
static uint32_t badblock_map[NAND_BLOCKS_COUNT / 32];
#endif

/*
 *
 */
static const EMCNANDConfig nandcfg = {
    &EMCD1,
    NAND_BLOCKS_COUNT,
    NAND_PAGE_DATA_SIZE,
    NAND_PAGE_SPARE_SIZE,
    NAND_PAGES_PER_BLOCK,
#if EMCNAND_USE_BAD_MAP
    badblock_map,
#endif
    NAND_ROW_WRITE_CYCLES,
    NAND_COL_WRITE_CYCLES,
    /* stm32 specific fields */
    ((EMCNAND_TIME_HIZ << 24) | (EMCNAND_TIME_HOLD << 16) | \
                                  (EMCNAND_TIME_WAIT << 8) | EMCNAND_TIME_SET),
#if !STM32_EMC_EMCNAND_USE_FSMC_INT
    ready_isr_enable,
    ready_isr_disable
#endif
};

/**
 *
 */
static const EXTConfig extcfg = {
  {
    {EXT_CH_MODE_DISABLED, NULL},     //0
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},     //4
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_RISING_EDGE | EXT_MODE_GPIOD, nand_ready_cb},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},     //8
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},     //12
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},     //16
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},     //20
    {EXT_CH_MODE_DISABLED, NULL},
    {EXT_CH_MODE_DISABLED, NULL},
  }
};

/*
 *
 */
volatile uint32_t IdleCnt = 0;
volatile systime_t T = 0;

#if EMCNAND_USE_KILL_TEST
volatile uint32_t KillCycle = 0;
#endif

/*
 ******************************************************************************
 ******************************************************************************
 * LOCAL FUNCTIONS
 ******************************************************************************
 ******************************************************************************
 */

static void nand_ready_cb(EXTDriver *extp, expchannel_t channel){
  (void)extp;
  (void)channel;
#if !STM32_EMC_EMCNAND_USE_FSMC_INT
  EMCNANDD1.isr_handler(&EMCNANDD1);
#endif
}

static void ready_isr_enable(void) {
  extChannelEnable(&EXTD1, GPIOD_NAND_RB);
}

static void ready_isr_disable(void) {
  extChannelDisable(&EXTD1, GPIOD_NAND_RB);
}

static void nand_wp_assert(void) {
  palClearPad(GPIOB, GPIOB_NAND_WP);
}

static void nand_wp_release(void) {
  palSetPad(GPIOB, GPIOB_NAND_WP);
}

static void red_led_on(void) {
  palSetPad(GPIOE, GPIOE_LED_R);
}

static void red_led_off(void) {
  palClearPad(GPIOE, GPIOE_LED_R);
}

static THD_WORKING_AREA(fsmcIdleThreadWA, 128);
static THD_FUNCTION(fsmcIdleThread, arg) {
  (void)arg;

  while(true){
    IdleCnt++;
  }
  return 0;
}

/*
 *
 */
static bool isErased(EMCNANDDriver *dp, size_t block){
  uint32_t page = 0;
  size_t i = 0;

  for (page=0; page<EMCNANDD1.config->pages_per_block; page++){
    emcnandReadPageData(dp, block, page, nand_buf, EMCNANDD1.config->page_data_size, NULL);
    emcnandReadPageSpare(dp, block, page, &nand_buf[2048], EMCNANDD1.config->page_spare_size);
    for (i=0; i<sizeof(nand_buf); i++) {
      if (nand_buf[i] != 0xFF)
        return false;
    }
  }

  return true;
}

static void pattern_fill(void) {

  size_t i;




  ///////////////////////// FIXME //////////////////////////////////
  //srand(hal_lld_get_counter_value());
  srand(0);


  for(i=0; i<NAND_PAGE_SIZE; i++){
    ref_buf[i] = rand() & 0xFF;
  }

  /* protect bad mark */
  ref_buf[NAND_PAGE_DATA_SIZE]     = 0xFF;
  ref_buf[NAND_PAGE_DATA_SIZE + 1] = 0xFF;
  memcpy(nand_buf, ref_buf, NAND_PAGE_SIZE);

  /* paranoid mode ON */
  osalDbgCheck(0 == memcmp(ref_buf, nand_buf, NAND_PAGE_SIZE));
}

#if EMCNAND_USE_KILL_TEST
static void kill_block(EMCNANDDriver *emcnandp, uint32_t block){

  size_t i = 0;
  size_t page = 0;
  uint8_t op_status;

  /* This test require good block.*/
  osalDbgCheck(!emcnandIsBad(emcnandp, block));

  while(true){
    op_status = emcnandErase(&EMCNANDD1, block);
    if (0 != (op_status & 1)){
      if(!isErased(emcnandp, block))
        osalSysHalt("Block successfully killed");
    }
    if(!isErased(emcnandp, block))
      osalSysHalt("Block block not erased, but erase operation report success");

    for (page=0; page<emcnandp->config->pages_per_block; page++){
      memset(nand_buf, 0, NAND_PAGE_SIZE);
      op_status = emcnandWritePageWhole(emcnandp, block, page, nand_buf, NAND_PAGE_SIZE);
      if (0 != (op_status & 1)){
        emcnandReadPageWhole(emcnandp, block, page, nand_buf, NAND_PAGE_SIZE);
        for (i=0; i<NAND_PAGE_SIZE; i++){
          if (nand_buf[i] != 0)
            osalSysHalt("Block successfully killed");
        }
      }

      emcnandReadPageWhole(emcnandp, block, page, nand_buf, NAND_PAGE_SIZE);
      for (i=0; i<NAND_PAGE_SIZE; i++){
        if (nand_buf[i] != 0)
          osalSysHalt("Page write failed, but write operation report success");
      }
    }
    KillCycle++;
  }
}
#endif /* EMCNAND_USE_KILL_TEST */

typedef enum {
  ECC_NO_ERROR = 0,
  ECC_CORRECTABLE_ERROR = 1,
  ECC_UNCORRECTABLE_ERROR = 2,
  ECC_CORRUPTED = 3,
} ecc_result_t;

static ecc_result_t parse_ecc(uint32_t ecclen, uint32_t ecc1, uint32_t ecc2,
                                                          uint32_t *corrupted){

  size_t i = 0;
  uint32_t corr = 0;
  uint32_t e = 0;
  uint32_t shift = (32 - ecclen);
  uint32_t b0, b1;

  ecc1 <<= shift;
  ecc1 >>= shift;
  ecc2 <<= shift;
  ecc2 >>= shift;
  e = ecc1 ^ ecc2;

  if (0 == e){
    return ECC_NO_ERROR;
  }
  else if (((e - 1) & e) == 0){
    return ECC_CORRUPTED;
  }
  else {
    for (i=0; i<ecclen/2; i++){
      b0 = e & 1;
      e >>= 1;
      b1 = e & 1;
      e >>= 1;
      if ((b0 + b1) != 1)
        return ECC_UNCORRECTABLE_ERROR;
      corr |= b1 << i;
    }
    *corrupted = corr;
    return ECC_CORRECTABLE_ERROR;
  }
}

static void invert_bit(uint8_t *buf, uint32_t byte, uint32_t bit){
  osalDbgCheck((byte < NAND_PAGE_DATA_SIZE) && (bit < 8));
  buf[byte] ^= ((uint8_t)1) << bit;
}

/*
 *
 */
static void ecc_test(EMCNANDDriver *emcnandp, uint32_t block){

  uint32_t corrupted;
  uint32_t byte, bit;
  const uint32_t ecclen = 28;
  uint32_t ecc_ref, ecc_broken;
  uint8_t op_status;
  ecc_result_t ecc_result = ECC_NO_ERROR;

  /* This test requires good block.*/
  osalDbgCheck(!emcnandIsBad(emcnandp, block));
  if (!isErased(emcnandp, block))
    emcnandErase(&EMCNANDD1, block);

  pattern_fill();

  /*** Correctable errors ***/
  op_status = emcnandWritePageData(emcnandp, block, 0,
                nand_buf, emcnandp->config->page_data_size, &ecc_ref);
  osalDbgCheck(0 == (op_status & 1)); /* operation failed */
  emcnandReadPageData(emcnandp, block, 0,
                  nand_buf, emcnandp->config->page_data_size, &ecc_broken);
  ecc_result = parse_ecc(ecclen, ecc_ref, ecc_broken, &corrupted);
  osalDbgCheck(ECC_NO_ERROR == ecc_result); /* unexpected error */

  /**/
  byte = 0;
  bit = 7;
  invert_bit(nand_buf, byte, bit);
  op_status = emcnandWritePageData(emcnandp, block, 1,
                nand_buf, emcnandp->config->page_data_size, &ecc_broken);
  osalDbgCheck(0 == (op_status & 1)); /* operation failed */
  invert_bit(nand_buf, byte, bit);
  ecc_result = parse_ecc(ecclen, ecc_ref, ecc_broken, &corrupted);
  osalDbgCheck(ECC_CORRECTABLE_ERROR == ecc_result); /* this error must be correctable */
  osalDbgCheck(corrupted == (byte * 8 + bit)); /* wrong correction code */

  /**/
  byte = 2047;
  bit = 0;
  invert_bit(nand_buf, byte, bit);
  op_status = emcnandWritePageData(emcnandp, block, 2,
                nand_buf, emcnandp->config->page_data_size, &ecc_broken);
  osalDbgCheck(0 == (op_status & 1)); /* operation failed */
  invert_bit(nand_buf, byte, bit);
  ecc_result = parse_ecc(ecclen, ecc_ref, ecc_broken, &corrupted);
  osalDbgCheck(ECC_CORRECTABLE_ERROR == ecc_result); /* this error must be correctable */
  osalDbgCheck(corrupted == (byte * 8 + bit)); /* wrong correction code */

  /**/
  byte = 1027;
  bit = 3;
  invert_bit(nand_buf, byte, bit);
  op_status = emcnandWritePageData(emcnandp, block, 3,
                nand_buf, emcnandp->config->page_data_size, &ecc_broken);
  osalDbgCheck(0 == (op_status & 1)); /* operation failed */
  invert_bit(nand_buf, byte, bit);
  ecc_result = parse_ecc(ecclen, ecc_ref, ecc_broken, &corrupted);
  osalDbgCheck(ECC_CORRECTABLE_ERROR == ecc_result); /* this error must be correctable */
  osalDbgCheck(corrupted == (byte * 8 + bit)); /* wrong correction code */

  /*** Uncorrectable error ***/
  byte = 1027;
  invert_bit(nand_buf, byte, 3);
  invert_bit(nand_buf, byte, 4);
  op_status = emcnandWritePageData(emcnandp, block, 4,
                nand_buf, emcnandp->config->page_data_size, &ecc_broken);
  osalDbgCheck(0 == (op_status & 1)); /* operation failed */
  invert_bit(nand_buf, byte, 3);
  invert_bit(nand_buf, byte, 4);
  ecc_result = parse_ecc(28, ecc_ref, ecc_broken, &corrupted);
  osalDbgCheck(ECC_UNCORRECTABLE_ERROR == ecc_result); /* This error must be NOT correctable */

  /*** make clean ***/
  emcnandErase(&EMCNANDD1, block);
}

/*
 *
 */
static void general_test (EMCNANDDriver *emcnandp, size_t first,
                                        size_t last, size_t read_rounds){

  size_t block, page, round;
  bool status;
  uint8_t op_status;
  uint32_t recc, wecc;

  red_led_on();

  /* initialize time measurement units */
  ////////////////////////////// FIXME //////////////////////////////
//  tmObjectInit(&tmu_erase);
//  tmObjectInit(&tmu_write_data);
//  tmObjectInit(&tmu_write_spare);
//  tmObjectInit(&tmu_read_data);
//  tmObjectInit(&tmu_read_spare);

  /* perform basic checks */
  for (block=first; block<last; block++){
    if (!emcnandIsBad(emcnandp, block)){
      if (!isErased(emcnandp, block)){
        op_status = emcnandErase(emcnandp, block);
        osalDbgCheck(0 == (op_status & 1)); /* operation failed */
      }
    }
  }

  /* write block with pattern, read it back and compare */
  for (block=first; block<last; block++){
    if (!emcnandIsBad(emcnandp, block)){
      for (page=0; page<emcnandp->config->pages_per_block; page++){
        pattern_fill();

        //tmStartMeasurement(&tmu_write_data);
        op_status = emcnandWritePageData(emcnandp, block, page,
                      nand_buf, emcnandp->config->page_data_size, &wecc);
        //tmStopMeasurement(&tmu_write_data);
        osalDbgCheck(0 == (op_status & 1)); /* operation failed */

        //tmStartMeasurement(&tmu_write_spare);
        op_status = emcnandWritePageSpare(emcnandp, block, page,
                      nand_buf + emcnandp->config->page_data_size,
                      emcnandp->config->page_spare_size);
        //tmStopMeasurement(&tmu_write_spare);
        osalDbgCheck(0 == (op_status & 1)); /* operation failed */

        /* read back and compare */
        for (round=0; round<read_rounds; round++){
          memset(nand_buf, 0, NAND_PAGE_SIZE);

          //tmStartMeasurement(&tmu_read_data);
          emcnandReadPageData(emcnandp, block, page,
                      nand_buf, emcnandp->config->page_data_size, &recc);
          //tmStopMeasurement(&tmu_read_data);
          osalDbgCheck(0 == (recc ^ wecc)); /* ECC error detected */

          //tmStartMeasurement(&tmu_read_spare);
          emcnandReadPageSpare(emcnandp, block, page,
                      nand_buf + emcnandp->config->page_data_size,
                      emcnandp->config->page_spare_size);
          //tmStopMeasurement(&tmu_read_spare);

          osalDbgCheck(0 == memcmp(ref_buf, nand_buf, NAND_PAGE_SIZE)); /* Read back failed */
        }
      }

      /* make clean */
      //tmStartMeasurement(&tmu_erase);
      op_status = emcnandErase(emcnandp, block);
      //tmStopMeasurement(&tmu_erase);
      osalDbgCheck(0 == (op_status & 1)); /* operation failed */

      status = isErased(emcnandp, block);
      osalDbgCheck(true == status); /* blocks was not erased successfully */
    }/* if (!emcnandIsBad(emcnandp, block)){ */
  }
  red_led_off();
}


/*
 ******************************************************************************
 * EXPORTED FUNCTIONS
 ******************************************************************************
 */

/*
 * Application entry point.
 */
int main(void) {

  size_t start = 1100;
  size_t end = 1150;
  volatile int32_t adc_its = 0;
  volatile int32_t spi_its = 0;
  volatile int32_t uart_its = 0;
  volatile int32_t adc_its_idle = 0;
  volatile int32_t spi_its_idle = 0;
  volatile int32_t uart_its_idle = 0;
  volatile uint32_t idle_thread_cnt = 0;

  #if EMCNAND_USE_KILL_TEST
  size_t kill = 8000;
  #endif

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   */
  halInit();
  chSysInit();

  emcStart(&EMCD1, &emccfg);
  extStart(&EXTD1, &extcfg);
  emcnandStart(&EMCNANDD1, &nandcfg);

  chThdSleepMilliseconds(4000);

  chThdCreateStatic(fsmcIdleThreadWA,
        sizeof(fsmcIdleThreadWA),
        NORMALPRIO - 20,
        fsmcIdleThread,
        NULL);

  nand_wp_release();

  dma_storm_adc_start();
  dma_storm_uart_start();
  dma_storm_spi_start();
  T = chVTGetSystemTimeX();
  general_test(&EMCNANDD1, start, end, 1);
  T = chVTGetSystemTimeX() - T;
  adc_its = dma_storm_adc_stop();
  uart_its = dma_storm_uart_stop();
  spi_its = dma_storm_spi_stop();
  chSysLock();
  idle_thread_cnt = IdleCnt;
  IdleCnt = 0;
  chSysUnlock();

  dma_storm_adc_start();
  dma_storm_uart_start();
  dma_storm_spi_start();
  chThdSleep(T);
  adc_its_idle = dma_storm_adc_stop();
  uart_its_idle = dma_storm_uart_stop();
  spi_its_idle = dma_storm_spi_stop();

  osalDbgCheck(idle_thread_cnt > (IdleCnt / 4));
  osalDbgCheck(abs(adc_its - adc_its_idle)   < (adc_its_idle  / 20));
  osalDbgCheck(abs(uart_its - uart_its_idle) < (uart_its_idle / 20));
  osalDbgCheck(abs(spi_its - spi_its_idle)   < (spi_its_idle  / 10));

  ecc_test(&EMCNANDD1, end);

#if EMCNAND_USE_KILL_TEST
  kill_block(&EMCNANDD1, kill);
#endif

  nand_wp_assert();

  /*
   * Normal main() thread activity, in this demo it does nothing.
   */
  while (TRUE) {
    chThdSleepMilliseconds(500);
  }

  /*warning suppressor */
#if STM32_EMC_EMCNAND_USE_FSMC_INT
  (void)ready_isr_enable;
  (void)ready_isr_disable;
#endif
}


