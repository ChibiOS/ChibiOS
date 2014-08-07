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
 * @file    nand.c
 * @brief   NAND Driver code.
 *
 * @addtogroup NAND
 * @{
 */

#include "hal.h"

#if HAL_USE_NAND || defined(__DOXYGEN__)

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
  osalDbgCheck((page_data_size >= NAND_MIN_PAGE_SIZE) &&
      (page_data_size <= NAND_MAX_PAGE_SIZE));

  /* Page size must be power of 2.*/
  osalDbgCheck(((page_data_size - 1) & page_data_size) == 0);
}

/**
 * @brief   Translate block-page-offset scheme to NAND internal address.
 *
 * @param[in] cfg       pointer to the @p NANDConfig from
 *                      corresponding NAND driver
 * @param[in] block     block number
 * @param[in] page      page number related to begin of block
 * @param[in] offset    data offset related to begin of page
 * @param[out] addr     buffer to store calculated address
 * @param[in] addr_len  length of address buffer
 *
 * @notapi
 */
static void calc_addr(const NANDConfig *cfg,
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
 * @param[in] cfg       pointer to the @p NANDConfig from
 *                      corresponding NAND driver
 * @param[in] block     block number
 * @param[out] addr     buffer to store calculated address
 * @param[in] addr_len  length of address buffer
 *
 * @notapi
 */
static void calc_blk_addr(const NANDConfig *cfg,
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

#if NAND_USE_BAD_MAP
/**
 * @brief   Add new bad block to map.
 *
 * @param[in] nandp         pointer to the @p NANDDriver object
 * @param[in] block         block number
 * @param[in] map           pointer to bad block map
 */
static void bad_map_update(NANDDriver *nandp, size_t block) {

  uint32_t *map = nandp->config->bb_map;
  const size_t BPMC = sizeof(uint32_t) * 8; /* bits per map claster */
  size_t i;
  size_t shift;

  /* Nand device overflow.*/
  osalDbgCheck(nandp->config->blocks > block);

  i = block / BPMC;
  shift = block % BPMC;
  /* This block already mapped.*/
  osalDbgCheck(((map[i] >> shift) & 1) != 1);
  map[i] |= (uint32_t)1 << shift;
}

/**
 * @brief   Scan for bad blocks and fill map with their numbers.
 *
 * @param[in] nandp         pointer to the @p NANDDriver object
 */
static void scan_bad_blocks(NANDDriver *nandp) {

  const size_t blocks = nandp->config->blocks;
  const size_t maplen = blocks / 32;

  size_t b;
  uint8_t m0;
  uint8_t m1;

  /* clear map just to be safe */
  for (b=0; b<maplen; b++)
    nandp->config->bb_map[b] = 0;

  /* now write numbers of bad block to map */
  for (b=0; b<blocks; b++){
    m0 = nandReadBadMark(nandp, b, 0);
    m1 = nandReadBadMark(nandp, b, 1);
    if ((0xFF != m0) || (0xFF != m1)){
      bad_map_update(nandp, b);
    }
  }
}
#endif /* NAND_USE_BAD_MAP */

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   NAND Driver initialization.
 * @note    This function is implicitly invoked by @p halInit(), there is
 *          no need to explicitly initialize the driver.
 *
 * @init
 */
void nandInit(void) {

  nand_lld_init();
}

/**
 * @brief   Initializes the standard part of a @p NANDDriver structure.
 *
 * @param[out] nandp        pointer to the @p NANDDriver object
 *
 * @init
 */
void nandObjectInit(NANDDriver *nandp) {

#if NAND_USE_MUTUAL_EXCLUSION
#if CH_CFG_USE_MUTEXES
  chMtxObjectInit(&nandp->mutex);
#else
  chSemObjectInit(&nandp->semaphore, 1);
#endif /* CH_CFG_USE_MUTEXES */
#endif /* NAND_USE_MUTUAL_EXCLUSION */

  nandp->state  = NAND_STOP;
  nandp->config = NULL;
}

/**
 * @brief   Configures and activates the NAND peripheral.
 *
 * @param[in] nandp         pointer to the @p NANDDriver object
 * @param[in] config        pointer to the @p NANDConfig object
 *
 * @api
 */
void nandStart(NANDDriver *nandp, const NANDConfig *config) {

  osalDbgCheck((nandp != NULL) && (config != NULL));
  osalDbgAssert((nandp->state == NAND_STOP) ||
      (nandp->state == NAND_READY),
      "invalid state");

  nandp->config = config;
  pagesize_check(nandp->config->page_data_size);
  nand_lld_start(nandp);
  nandp->state = NAND_READY;

#if NAND_USE_BAD_MAP
  scan_bad_blocks(nandp);
#endif /* NAND_USE_BAD_MAP */
}

/**
 * @brief   Deactivates the NAND peripheral.
 *
 * @param[in] nandp         pointer to the @p NANDDriver object
 *
 * @api
 */
void nandStop(NANDDriver *nandp) {

  osalDbgCheck(nandp != NULL);
  osalDbgAssert((nandp->state == NAND_STOP) ||
      (nandp->state == NAND_READY),
      "invalid state");
  nand_lld_stop(nandp);
  nandp->state = NAND_STOP;
}

/**
 * @brief   Read whole page.
 *
 * @param[in] nandp         pointer to the @p NANDDriver object
 * @param[in] block         block number
 * @param[in] page          page number related to begin of block
 * @param[out] data         buffer to store data
 * @param[in] datalen       length of data buffer
 *
 * @api
 */
void nandReadPageWhole(NANDDriver *nandp, uint32_t block,
                            uint32_t page, uint8_t *data, size_t datalen) {

  const NANDConfig *cfg = nandp->config;
  uint8_t addrbuf[8];
  size_t addrlen = cfg->rowcycles + cfg->colcycles;

  osalDbgCheck((nandp != NULL) && (data != NULL));
  osalDbgCheck((datalen <= (cfg->page_data_size + cfg->page_spare_size)));
  osalDbgAssert(nandp->state == NAND_READY, "invalid state");

  calc_addr(cfg, block, page, 0, addrbuf, addrlen);
  nand_lld_read_data(nandp, data, datalen, addrbuf, addrlen, NULL);
}

/**
 * @brief   Write whole page.
 *
 * @param[in] nandp         pointer to the @p NANDDriver object
 * @param[in] block         block number
 * @param[in] page          page number related to begin of block
 * @param[in] data          buffer with data to be written
 * @param[in] datalen       length of data buffer
 *
 * @return    The operation status reported by NAND IC (0x70 command).
 *
 * @api
 */
uint8_t nandWritePageWhole(NANDDriver *nandp, uint32_t block,
                    uint32_t page, const uint8_t *data, size_t datalen) {

  uint8_t retval;
  const NANDConfig *cfg = nandp->config;
  uint8_t addr[8];
  size_t addrlen = cfg->rowcycles + cfg->colcycles;

  osalDbgCheck((nandp != NULL) && (data != NULL));
  osalDbgCheck((datalen <= (cfg->page_data_size + cfg->page_spare_size)));
  osalDbgAssert(nandp->state == NAND_READY, "invalid state");

  calc_addr(cfg, block, page, 0, addr, addrlen);
  retval = nand_lld_write_data(nandp, data, datalen, addr, addrlen, NULL);
  return retval;
}

/**
 * @brief   Read page data without spare area.
 *
 * @param[in] nandp         pointer to the @p NANDDriver object
 * @param[in] block         block number
 * @param[in] page          page number related to begin of block
 * @param[out] data         buffer to store data
 * @param[in] datalen       length of data buffer
 * @param[out] ecc          pointer to calculated ECC. Ignored when NULL.
 *
 * @api
 */
void nandReadPageData(NANDDriver *nandp, uint32_t block, uint32_t page,
                         uint8_t *data, size_t datalen, uint32_t *ecc) {

  const NANDConfig *cfg = nandp->config;
  uint8_t addrbuf[8];
  size_t addrlen = cfg->rowcycles + cfg->colcycles;

  osalDbgCheck((nandp != NULL) && (data != NULL));
  osalDbgCheck((datalen <= cfg->page_data_size));
  osalDbgAssert(nandp->state == NAND_READY, "invalid state");

  calc_addr(cfg, block, page, 0, addrbuf, addrlen);
  nand_lld_read_data(nandp, data, datalen, addrbuf, addrlen, ecc);
}

/**
 * @brief   Write page data without spare area.
 *
 * @param[in] nandp         pointer to the @p NANDDriver object
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
uint8_t nandWritePageData(NANDDriver *nandp, uint32_t block,
        uint32_t page, const uint8_t *data, size_t datalen, uint32_t *ecc) {

  uint8_t retval;
  const NANDConfig *cfg = nandp->config;
  uint8_t addr[8];
  size_t addrlen = cfg->rowcycles + cfg->colcycles;

  osalDbgCheck((nandp != NULL) && (data != NULL));
  osalDbgCheck((datalen <= cfg->page_data_size));
  osalDbgAssert(nandp->state == NAND_READY, "invalid state");

  calc_addr(cfg, block, page, 0, addr, addrlen);
  retval = nand_lld_write_data(nandp, data, datalen, addr, addrlen, ecc);
  return retval;
}

/**
 * @brief   Read page spare area.
 *
 * @param[in] nandp         pointer to the @p NANDDriver object
 * @param[in] block         block number
 * @param[in] page          page number related to begin of block
 * @param[out] spare        buffer to store data
 * @param[in] sparelen      length of data buffer
 *
 * @api
 */
void nandReadPageSpare(NANDDriver *nandp, uint32_t block,
                          uint32_t page, uint8_t *spare, size_t sparelen) {

  const NANDConfig *cfg = nandp->config;
  uint8_t addr[8];
  size_t addrlen = cfg->rowcycles + cfg->colcycles;

  osalDbgCheck((NULL != spare) && (nandp != NULL));
  osalDbgCheck(sparelen <= cfg->page_spare_size);
  osalDbgAssert(nandp->state == NAND_READY, "invalid state");

  calc_addr(cfg, block, page, cfg->page_data_size, addr, addrlen);
  nand_lld_read_data(nandp, spare, sparelen, addr, addrlen, NULL);
}

/**
 * @brief   Write page spare area.
 *
 * @param[in] nandp         pointer to the @p NANDDriver object
 * @param[in] block         block number
 * @param[in] page          page number related to begin of block
 * @param[in] spare         buffer with spare data to be written
 * @param[in] sparelen      length of data buffer
 *
 * @return    The operation status reported by NAND IC (0x70 command).
 *
 * @api
 */
uint8_t nandWritePageSpare(NANDDriver *nandp, uint32_t block,
                      uint32_t page, const uint8_t *spare, size_t sparelen) {

  uint8_t retVal;
  const NANDConfig *cfg = nandp->config;
  uint8_t addr[8];
  size_t addrlen = cfg->rowcycles + cfg->colcycles;

  osalDbgCheck((NULL != spare) && (nandp != NULL));
  osalDbgCheck(sparelen <= cfg->page_spare_size);
  osalDbgAssert(nandp->state == NAND_READY, "invalid state");

  calc_addr(cfg, block, page, cfg->page_data_size, addr, addrlen);
  retVal = nand_lld_write_data(nandp, spare, sparelen, addr, addrlen, NULL);
  return retVal;
}

/**
 * @brief   Mark block as bad.
 *
 * @param[in] nandp         pointer to the @p NANDDriver object
 * @param[in] block         block number
 *
 * @api
 */
void nandMarkBad(NANDDriver *nandp, uint32_t block) {

  uint8_t bb_mark[2] = {0, 0};
  uint8_t op_status;
  op_status = nandWritePageSpare(nandp, block, 0, bb_mark, sizeof(bb_mark));
  osalDbgCheck(0 == (op_status & 1)); /* operation failed*/
  op_status = nandWritePageSpare(nandp, block, 1, bb_mark, sizeof(bb_mark));
  osalDbgCheck(0 == (op_status & 1)); /* operation failed*/

#if NAND_USE_BAD_MAP
  bad_map_update(nandp, block);
#endif
}

/**
 * @brief   Read bad mark out.
 *
 * @param[in] nandp         pointer to the @p NANDDriver object
 * @param[in] block         block number
 * @param[in] page          page number related to begin of block
 *
 * @return                  Bad mark.
 *
 * @api
 */
uint8_t nandReadBadMark(NANDDriver *nandp,
                                  uint32_t block, uint32_t page) {
  uint8_t bb_mark[1];
  nandReadPageSpare(nandp, block, page, bb_mark, sizeof(bb_mark));
  return bb_mark[0];
}

/**
 * @brief   Erase block.
 *
 * @param[in] nandp         pointer to the @p NANDDriver object
 * @param[in] block         block number
 *
 * @return    The operation status reported by NAND IC (0x70 command).
 *
 * @api
 */
uint8_t nandErase(NANDDriver *nandp, uint32_t block){

  uint8_t retVal;
  const NANDConfig *cfg = nandp->config;
  uint8_t addr[4];
  size_t addrlen = cfg->rowcycles;

  osalDbgCheck(nandp != NULL);
  osalDbgAssert(nandp->state == NAND_READY, "invalid state");

  calc_blk_addr(cfg, block, addr, addrlen);
  retVal = nand_lld_erase(nandp, addr, addrlen);
  return retVal;
}

/**
 * @brief   Report block badness.
 *
 * @param[in] nandp         pointer to the @p NANDDriver object
 * @param[in] block         block number
 *
 * @return                  block condition
 * @retval true             if the block is bad.
 * @retval false            if the block is good.
 *
 * @api
 */
bool nandIsBad(NANDDriver *nandp, uint32_t block){

  osalDbgCheck(nandp != NULL);
  osalDbgAssert(nandp->state == NAND_READY, "invalid state");

#if NAND_USE_BAD_MAP
  uint32_t *map = nandp->config->bb_map;
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
  m0 = nandReadBadMark(nandp, block, 0);
  m1 = nandReadBadMark(nandp, block, 1);
  if ((0xFF != m0) || (0xFF != m1))
    return true;
  else
    return false;
#endif /* NAND_USE_BAD_MAP */
}

#if NAND_USE_MUTUAL_EXCLUSION || defined(__DOXYGEN__)
/**
 * @brief   Gains exclusive access to the NAND bus.
 * @details This function tries to gain ownership to the NAND bus, if the bus
 *          is already being used then the invoking thread is queued.
 * @pre     In order to use this function the option
 *          @p NAND_USE_MUTUAL_EXCLUSION must be enabled.
 *
 * @param[in] nandp         pointer to the @p NANDDriver object
 *
 * @api
 */
void nandAcquireBus(NANDDriver *nandp) {

  osalDbgCheck(nandp != NULL);

#if CH_CFG_USE_MUTEXES
  chMtxLock(&nandp->mutex);
#elif CH_CFG_USE_SEMAPHORES
  chSemWait(&nandp->semaphore);
#endif
}

/**
 * @brief   Releases exclusive access to the NAND bus.
 * @pre     In order to use this function the option
 *          @p NAND_USE_MUTUAL_EXCLUSION must be enabled.
 *
 * @param[in] nandp         pointer to the @p NANDDriver object
 *
 * @api
 */
void nandReleaseBus(NANDDriver *nandp) {

  osalDbgCheck(nandp != NULL);

#if CH_CFG_USE_MUTEXES
  chMtxUnlock(&nandp->mutex);
#elif CH_CFG_USE_SEMAPHORES
  chSemSignal(&nandp->semaphore);
#endif
}
#endif /* NAND_USE_MUTUAL_EXCLUSION */

#endif /* HAL_USE_NAND */

/** @} */




