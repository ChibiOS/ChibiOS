/*
    ChibiOS/HAL - Copyright (C) 2006,2007,2008,2009,2010,
                  2011,2012,2013,2014 Giovanni Di Sirio.

    This file is part of ChibiOS/HAL

    ChibiOS/HAL is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
/*
   Concepts and parts of this file have been contributed by Uladzimir Pylinsky
   aka barthess.
 */

/**
 * @file    emcnand.c
 * @brief   EMCNAND Driver code.
 *
 * @addtogroup EMCNAND
 * @{
 */

#include "hal.h"

#if HAL_USE_EMCNAND || defined(__DOXYGEN__)

#include "string.h" /* for memset */

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/**
 * @brief   Check page size.
 *
 * @param[in] page_data_size      size of page data area
 *
 * @notapi
 */
static void pagesize_check(size_t page_data_size){

  /* Page size out of bounds.*/
  osalDbgCheck((page_data_size >= EMCNAND_MIN_PAGE_SIZE) &&
      (page_data_size <= EMCNAND_MAX_PAGE_SIZE));

  /* Page size must be power of 2.*/
  osalDbgCheck(((page_data_size - 1) & page_data_size) == 0);
}

/**
 * @brief   Translate block-page-offset scheme to NAND internal address.
 *
 * @param[in] cfg       pointer to the @p EMCNANDConfig from
 *                      corresponding NAND driver
 * @param[in] block     block number
 * @param[in] page      page number related to begin of block
 * @param[in] offset    data offset related to begin of page
 * @param[out] addr     buffer to store calculated address
 * @param[in] addr_len  length of address buffer
 *
 * @notapi
 */
static void calc_addr(const EMCNANDConfig *cfg,
                      uint32_t block, uint32_t page, uint32_t offset,
                      uint8_t *addr, size_t addr_len){
  size_t i = 0;
  uint32_t row = 0;

  /* Incorrect buffer length.*/
  osalDbgCheck(cfg->rowcycles + cfg->colcycles == addr_len);
  osalDbgCheck((block < cfg->blocks) && (page < cfg->pages_per_block) &&
             (offset < cfg->page_data_size + cfg->page_spare_size));

  /* convert address to NAND specific */
  memset(addr, 0, addr_len);
  row = (block * cfg->pages_per_block) + page;
  for (i=0; i<cfg->colcycles; i++){
    addr[i] = offset & 0xFF;
    offset = offset >> 8;
  }
  for (; i<addr_len; i++){
    addr[i] = row & 0xFF;
    row = row >> 8;
  }
}

/**
 * @brief   Translate block number to NAND internal address.
 * @note    This function designed for erasing purpose.
 *
 * @param[in] cfg       pointer to the @p EMCNANDConfig from
 *                      corresponding NAND driver
 * @param[in] block     block number
 * @param[out] addr     buffer to store calculated address
 * @param[in] addr_len  length of address buffer
 *
 * @notapi
 */
static void calc_blk_addr(const EMCNANDConfig *cfg,
                          uint32_t block, uint8_t *addr, size_t addr_len){
  size_t i = 0;
  uint32_t row = 0;

  /* Incorrect buffer length.*/
  osalDbgCheck(cfg->rowcycles == addr_len);
  osalDbgCheck((block < cfg->blocks));

  /* convert address to NAND specific */
  memset(addr, 0, addr_len);
  row = block * cfg->pages_per_block;
  for (i=0; i<addr_len; i++){
    addr[i] = row & 0xFF;
    row = row >> 8;
  }
}

#if EMCNAND_USE_BAD_MAP
/**
 * @brief   Add new bad block to map.
 *
 * @param[in] emcnandp      pointer to the @p EMCNANDDriver object
 * @param[in] block         block number
 * @param[in] map           pointer to bad block map
 */
static void bad_map_update(EMCNANDDriver *emcnandp, size_t block) {

  uint32_t *map = emcnandp->config->bb_map;
  const size_t BPMC = sizeof(uint32_t) * 8; /* bits per map claster */
  size_t i;
  size_t shift;

  /* Nand device overflow.*/
  osalDbgCheck(emcnandp->config->blocks > block);

  i = block / BPMC;
  shift = block % BPMC;
  /* This block already mapped.*/
  osalDbgCheck(((map[i] >> shift) & 1) != 1);
  map[i] |= (uint32_t)1 << shift;
}

/**
 * @brief   Scan for bad blocks and fill map with their numbers.
 *
 * @param[in] emcnandp      pointer to the @p EMCNANDDriver object
 */
static void scan_bad_blocks(EMCNANDDriver *emcnandp) {

  const size_t blocks = emcnandp->config->blocks;
  const size_t maplen = blocks / 32;

  size_t b;
  uint8_t m0;
  uint8_t m1;

  /* clear map just to be safe */
  for (b=0; b<maplen; b++)
    emcnandp->config->bb_map[b] = 0;

  /* now write numbers of bad block to map */
  for (b=0; b<blocks; b++){
    m0 = emcnandReadBadMark(emcnandp, b, 0);
    m1 = emcnandReadBadMark(emcnandp, b, 1);
    if ((0xFF != m0) || (0xFF != m1)){
      bad_map_update(emcnandp, b);
    }
  }
}
#endif /* EMCNAND_USE_BAD_MAP */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   EMCNAND Driver initialization.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @init
 */
void emcnandInit(void) {

  emcnand_lld_init();
}

/**
 * @brief   Initializes the standard part of a @p EMCNANDDriver structure.
 *
 * @param[out] emcnandp     pointer to the @p EMCNANDDriver object
 *
 * @init
 */
void emcnandObjectInit(EMCNANDDriver *emcnandp) {

#if EMCNAND_USE_MUTUAL_EXCLUSION
#if CH_CFG_USE_MUTEXES
  chMtxObjectInit(&emcnandp->mutex);
#else
  chSemObjectInit(&emcnandp->semaphore, 1);
#endif /* CH_CFG_USE_MUTEXES */
#endif /* EMCNAND_USE_MUTUAL_EXCLUSION */

  emcnandp->state  = EMCNAND_STOP;
  emcnandp->config = NULL;
}

/**
 * @brief   Configures and activates the EMCNAND peripheral.
 *
 * @param[in] emcnandp      pointer to the @p EMCNANDDriver object
 * @param[in] config        pointer to the @p EMCNANDConfig object
 *
 * @api
 */
void emcnandStart(EMCNANDDriver *emcnandp, const EMCNANDConfig *config) {

  osalDbgCheck((emcnandp != NULL) && (config != NULL));
  osalDbgAssert(config->emcp->state == EMC_READY,
      "lower level driver not ready");
  osalDbgAssert((emcnandp->state == EMCNAND_STOP) ||
      (emcnandp->state == EMCNAND_READY),
      "invalid state");

  emcnandp->config = config;
  pagesize_check(emcnandp->config->page_data_size);
  emcnand_lld_start(emcnandp);
  emcnandp->state = EMCNAND_READY;

#if EMCNAND_USE_BAD_MAP
  scan_bad_blocks(emcnandp);
#endif /* EMCNAND_USE_BAD_MAP */
}

/**
 * @brief   Deactivates the EMCNAND peripheral.
 *
 * @param[in] emcnandp      pointer to the @p EMCNANDDriver object
 *
 * @api
 */
void emcnandStop(EMCNANDDriver *emcnandp) {

  osalDbgCheck(emcnandp != NULL);
  osalDbgAssert((emcnandp->state == EMCNAND_STOP) ||
      (emcnandp->state == EMCNAND_READY),
      "invalid state");
  emcnand_lld_stop(emcnandp);
  emcnandp->state = EMCNAND_STOP;
}

/**
 * @brief   Read whole page.
 *
 * @param[in] emcnandp      pointer to the @p EMCNANDDriver object
 * @param[in] block         block number
 * @param[in] page          page number related to begin of block
 * @param[out] data         buffer to store data
 * @param[in] datalen       length of data buffer
 *
 * @api
 */
void emcnandReadPageWhole(EMCNANDDriver *emcnandp, uint32_t block,
                            uint32_t page, uint8_t *data, size_t datalen) {

  const EMCNANDConfig *cfg = emcnandp->config;
  uint8_t addrbuf[8];
  size_t addrlen = cfg->rowcycles + cfg->colcycles;

  osalDbgCheck((emcnandp != NULL) && (data != NULL));
  osalDbgCheck((datalen <= (cfg->page_data_size + cfg->page_spare_size)));
  osalDbgAssert(emcnandp->state == EMCNAND_READY, "invalid state");

  calc_addr(cfg, block, page, 0, addrbuf, addrlen);
  emcnand_lld_read_data(emcnandp, data, datalen, addrbuf, addrlen, NULL);
}

/**
 * @brief   Write whole page.
 *
 * @param[in] emcnandp      pointer to the @p EMCNANDDriver object
 * @param[in] block         block number
 * @param[in] page          page number related to begin of block
 * @param[in] data          buffer with data to be written
 * @param[in] datalen       length of data buffer
 *
 * @return    The operation status reported by NAND IC (0x70 command).
 *
 * @api
 */
uint8_t emcnandWritePageWhole(EMCNANDDriver *emcnandp, uint32_t block,
                    uint32_t page, const uint8_t *data, size_t datalen) {

  uint8_t retval;
  const EMCNANDConfig *cfg = emcnandp->config;
  uint8_t addr[8];
  size_t addrlen = cfg->rowcycles + cfg->colcycles;

  osalDbgCheck((emcnandp != NULL) && (data != NULL));
  osalDbgCheck((datalen <= (cfg->page_data_size + cfg->page_spare_size)));
  osalDbgAssert(emcnandp->state == EMCNAND_READY, "invalid state");

  calc_addr(cfg, block, page, 0, addr, addrlen);
  retval = emcnand_lld_write_data(emcnandp, data, datalen, addr, addrlen, NULL);
  return retval;
}

/**
 * @brief   Read page data without spare area.
 *
 * @param[in] emcnandp      pointer to the @p EMCNANDDriver object
 * @param[in] block         block number
 * @param[in] page          page number related to begin of block
 * @param[out] data         buffer to store data
 * @param[in] datalen       length of data buffer
 * @param[out] ecc          pointer to calculated ECC. Ignored when NULL.
 *
 * @api
 */
void emcnandReadPageData(EMCNANDDriver *emcnandp, uint32_t block, uint32_t page,
                         uint8_t *data, size_t datalen, uint32_t *ecc) {

  const EMCNANDConfig *cfg = emcnandp->config;
  uint8_t addrbuf[8];
  size_t addrlen = cfg->rowcycles + cfg->colcycles;

  osalDbgCheck((emcnandp != NULL) && (data != NULL));
  osalDbgCheck((datalen <= cfg->page_data_size));
  osalDbgAssert(emcnandp->state == EMCNAND_READY, "invalid state");

  calc_addr(cfg, block, page, 0, addrbuf, addrlen);
  emcnand_lld_read_data(emcnandp, data, datalen, addrbuf, addrlen, ecc);
}

/**
 * @brief   Write page data without spare area.
 *
 * @param[in] emcnandp      pointer to the @p EMCNANDDriver object
 * @param[in] block         block number
 * @param[in] page          page number related to begin of block
 * @param[in] data          buffer with data to be written
 * @param[in] datalen       length of data buffer
 * @param[out] ecc          pointer to calculated ECC. Ignored when NULL.
 *
 * @return    The operation status reported by NAND IC (0x70 command).
 *
 * @api
 */
uint8_t emcnandWritePageData(EMCNANDDriver *emcnandp, uint32_t block,
        uint32_t page, const uint8_t *data, size_t datalen, uint32_t *ecc) {

  uint8_t retval;
  const EMCNANDConfig *cfg = emcnandp->config;
  uint8_t addr[8];
  size_t addrlen = cfg->rowcycles + cfg->colcycles;

  osalDbgCheck((emcnandp != NULL) && (data != NULL));
  osalDbgCheck((datalen <= cfg->page_data_size));
  osalDbgAssert(emcnandp->state == EMCNAND_READY, "invalid state");

  calc_addr(cfg, block, page, 0, addr, addrlen);
  retval = emcnand_lld_write_data(emcnandp, data, datalen, addr, addrlen, ecc);
  return retval;
}

/**
 * @brief   Read page spare area.
 *
 * @param[in] emcnandp      pointer to the @p EMCNANDDriver object
 * @param[in] block         block number
 * @param[in] page          page number related to begin of block
 * @param[out] spare        buffer to store data
 * @param[in] sparelen      length of data buffer
 *
 * @api
 */
void emcnandReadPageSpare(EMCNANDDriver *emcnandp, uint32_t block,
                          uint32_t page, uint8_t *spare, size_t sparelen) {

  const EMCNANDConfig *cfg = emcnandp->config;
  uint8_t addr[8];
  size_t addrlen = cfg->rowcycles + cfg->colcycles;

  osalDbgCheck((NULL != spare) && (emcnandp != NULL));
  osalDbgCheck(sparelen <= cfg->page_spare_size);
  osalDbgAssert(emcnandp->state == EMCNAND_READY, "invalid state");

  calc_addr(cfg, block, page, cfg->page_data_size, addr, addrlen);
  emcnand_lld_read_data(emcnandp, spare, sparelen, addr, addrlen, NULL);
}

/**
 * @brief   Write page spare area.
 *
 * @param[in] emcnandp      pointer to the @p EMCNANDDriver object
 * @param[in] block         block number
 * @param[in] page          page number related to begin of block
 * @param[in] spare         buffer with spare data to be written
 * @param[in] sparelen      length of data buffer
 *
 * @return    The operation status reported by NAND IC (0x70 command).
 *
 * @api
 */
uint8_t emcnandWritePageSpare(EMCNANDDriver *emcnandp, uint32_t block,
                      uint32_t page, const uint8_t *spare, size_t sparelen) {

  uint8_t retVal;
  const EMCNANDConfig *cfg = emcnandp->config;
  uint8_t addr[8];
  size_t addrlen = cfg->rowcycles + cfg->colcycles;

  osalDbgCheck((NULL != spare) && (emcnandp != NULL));
  osalDbgCheck(sparelen <= cfg->page_spare_size);
  osalDbgAssert(emcnandp->state == EMCNAND_READY, "invalid state");

  calc_addr(cfg, block, page, cfg->page_data_size, addr, addrlen);
  retVal = emcnand_lld_write_data(emcnandp, spare, sparelen, addr, addrlen, NULL);
  return retVal;
}

/**
 * @brief   Mark block as bad.
 *
 * @param[in] emcnandp      pointer to the @p EMCNANDDriver object
 * @param[in] block         block number
 *
 * @api
 */
void emcnandMarkBad(EMCNANDDriver *emcnandp, uint32_t block) {

  uint8_t bb_mark[2] = {0, 0};
  uint8_t op_status;
  op_status = emcnandWritePageSpare(emcnandp, block, 0, bb_mark, sizeof(bb_mark));
  osalDbgCheck(0 == (op_status & 1)); /* operation failed*/
  op_status = emcnandWritePageSpare(emcnandp, block, 1, bb_mark, sizeof(bb_mark));
  osalDbgCheck(0 == (op_status & 1)); /* operation failed*/

#if EMCNAND_USE_BAD_MAP
  bad_map_update(emcnandp, block);
#endif
}

/**
 * @brief   Read bad mark out.
 *
 * @param[in] emcnandp      pointer to the @p EMCNANDDriver object
 * @param[in] block         block number
 * @param[in] page          page number related to begin of block
 *
 * @return                  Bad mark.
 *
 * @api
 */
uint8_t emcnandReadBadMark(EMCNANDDriver *emcnandp,
                                  uint32_t block, uint32_t page) {
  uint8_t bb_mark[1];
  emcnandReadPageSpare(emcnandp, block, page, bb_mark, sizeof(bb_mark));
  return bb_mark[0];
}

/**
 * @brief   Erase block.
 *
 * @param[in] emcnandp      pointer to the @p EMCNANDDriver object
 * @param[in] block         block number
 *
 * @return    The operation status reported by NAND IC (0x70 command).
 *
 * @api
 */
uint8_t emcnandErase(EMCNANDDriver *emcnandp, uint32_t block){

  uint8_t retVal;
  const EMCNANDConfig *cfg = emcnandp->config;
  uint8_t addr[4];
  size_t addrlen = cfg->rowcycles;

  osalDbgCheck(emcnandp != NULL);
  osalDbgAssert(emcnandp->state == EMCNAND_READY, "invalid state");

  calc_blk_addr(cfg, block, addr, addrlen);
  retVal = emcnand_lld_erase(emcnandp, addr, addrlen);
  return retVal;
}

/**
 * @brief   Report block badness.
 *
 * @param[in] emcnandp      pointer to the @p EMCNANDDriver object
 * @param[in] block         block number
 *
 * @return                  block condition
 * @retval true             if the block is bad.
 * @retval false            if the block is good.
 *
 * @api
 */
bool emcnandIsBad(EMCNANDDriver *emcnandp, uint32_t block){

  osalDbgCheck(emcnandp != NULL);
  osalDbgAssert(emcnandp->state == EMCNAND_READY, "invalid state");

#if EMCNAND_USE_BAD_MAP
  uint32_t *map = emcnandp->config->bb_map;
  const size_t BPMC = sizeof(uint32_t) * 8; /* bits per map claster */
  size_t i;
  size_t shift;

  i = block / BPMC;
  shift = block % BPMC;
  if (((map[i] >> shift) & 1) == 1)
    return true;
  else
    return false;
#else
  uint8_t m0, m1;
  m0 = emcnandReadBadMark(emcnandp, block, 0);
  m1 = emcnandReadBadMark(emcnandp, block, 1);
  if ((0xFF != m0) || (0xFF != m1))
    return true;
  else
    return false;
#endif /* EMCNAND_USE_BAD_MAP */
}

#if EMCNAND_USE_MUTUAL_EXCLUSION || defined(__DOXYGEN__)
/**
 * @brief   Gains exclusive access to the EMCNAND bus.
 * @details This function tries to gain ownership to the EMCNAND bus, if the bus
 *          is already being used then the invoking thread is queued.
 * @pre     In order to use this function the option
 *          @p EMCNAND_USE_MUTUAL_EXCLUSION must be enabled.
 *
 * @param[in] emcnandp      pointer to the @p EMCNANDDriver object
 *
 * @api
 */
void emcnandAcquireBus(EMCNANDDriver *emcnandp) {

  osalDbgCheck(emcnandp != NULL);

#if CH_CFG_USE_MUTEXES
  chMtxLock(&emcnandp->mutex);
#elif CH_CFG_USE_SEMAPHORES
  chSemWait(&emcnandp->semaphore);
#endif
}

/**
 * @brief   Releases exclusive access to the EMCNAND bus.
 * @pre     In order to use this function the option
 *          @p EMCNAND_USE_MUTUAL_EXCLUSION must be enabled.
 *
 * @param[in] emcnandp      pointer to the @p EMCNANDDriver object
 *
 * @api
 */
void emcnandReleaseBus(EMCNANDDriver *emcnandp) {

  osalDbgCheck(emcnandp != NULL);

#if CH_CFG_USE_MUTEXES
  chMtxUnlock(&emcnandp->mutex);
#elif CH_CFG_USE_SEMAPHORES
  chSemSignal(&emcnandp->semaphore);
#endif
}
#endif /* EMCNAND_USE_MUTUAL_EXCLUSION */

#endif /* HAL_USE_EMCNAND */

/** @} */




