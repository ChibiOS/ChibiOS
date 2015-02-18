/*
    ChibiOS - Copyright (C) 2006..2015 Giovanni Di Sirio.

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
 * @file    hal_mmcsd.c
 * @brief   MMC/SD cards common code.
 *
 * @addtogroup MMCSD
 * @{
 */

#include "hal.h"

#if HAL_USE_MMC_SPI || HAL_USE_SDC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Gets a bit field from a words array.
 * @note    The bit zero is the LSb of the first word.
 *
 * @param[in] data      pointer to the words array
 * @param[in] end       bit offset of the last bit of the field, inclusive
 * @param[in] start     bit offset of the first bit of the field, inclusive
 *
 * @return              The bits field value, left aligned.
 *
 * @notapi
 */
uint32_t mmcsdGetSlice(const uint32_t *data, uint32_t end, uint32_t start) {
  unsigned startidx, endidx, startoff;
  uint32_t endmask;

  osalDbgCheck((end >= start) && ((end - start) < 32));

  startidx = start / 32;
  startoff = start % 32;
  endidx   = end / 32;
  endmask  = (1 << ((end % 32) + 1)) - 1;

  /* One or two pieces?*/
  if (startidx < endidx)
    return (data[startidx] >> startoff) |               /* Two pieces case. */
           ((data[endidx] & endmask) << (32 - startoff));
  return (data[startidx] & endmask) >> startoff;        /* One piece case.  */
}

/**
 * @brief   Extract card capacity from a CSD.
 * @details The capacity is returned as number of available blocks.
 *
 * @param[in] csd       the CSD record
 *
 * @return              The card capacity.
 * @retval 0            CSD format error
 */
uint32_t mmcsdGetCapacity(const uint32_t *csd) {
  uint32_t a, b, c;

  osalDbgCheck(NULL != csd);

  switch (mmcsdGetSlice(csd, MMCSD_CSD_10_CSD_STRUCTURE_SLICE)) {
  case 0:
    /* CSD version 1.0 */
    a = mmcsdGetSlice(csd, MMCSD_CSD_10_C_SIZE_SLICE);
    b = mmcsdGetSlice(csd, MMCSD_CSD_10_C_SIZE_MULT_SLICE);
    c = mmcsdGetSlice(csd, MMCSD_CSD_10_READ_BL_LEN_SLICE);
    return (a + 1) << (b + 2) << (c - 9);       /* 2^9 == MMCSD_BLOCK_SIZE. */
  case 1:
    /* CSD version 2.0.*/
    return 1024 * (mmcsdGetSlice(csd, MMCSD_CSD_20_C_SIZE_SLICE) + 1);
  default:
    /* Reserved value detected.*/
    return 0;
  }
}

/**
 * @brief   Extract MMC card capacity from a CSD or EXT_CSD.
 * @details The capacity is returned as number of available blocks.
 *
 * @param[in] csd       the CSD record
 * @param[in] ext_csd   the extended CSD record
 *
 * @return              The card capacity.
 */
uint32_t mmcsdGetCapacityMMC(const uint32_t *csd, const uint8_t *ext_csd) {
  uint32_t a, b, c;

  osalDbgCheck(NULL != csd);

  a = mmcsdGetSlice(csd, MMCSD_CSD_MMC_C_SIZE_SLICE);
  if (0xFFF != a) {
    b = mmcsdGetSlice(csd, MMCSD_CSD_MMC_C_SIZE_MULT_SLICE);
    c = mmcsdGetSlice(csd, MMCSD_CSD_MMC_READ_BL_LEN_SLICE);
    return (a + 1) << (b + 2) << (c - 9);       /* 2^9 == MMCSD_BLOCK_SIZE. */
  }
  else if (NULL != ext_csd) {
    return (ext_csd[215] << 24) +
           (ext_csd[214] << 16) +
           (ext_csd[213] << 8)  +
            ext_csd[212];
  }
  else
    return 0;
}

/**
 * @brief   Unpacks SDC CID array in structure.
 *
 * @param[in] sdcp      pointer to the @p MMCSDBlockDevice object
 * @param[out] cidsdc   pointer to the @p unpacked_sdc_cid_t object
 *
 * @api
 */
void sdcUnpackCID(const MMCSDBlockDevice *sdcp, unpacked_sdc_cid_t *cidsdc) {
  const uint32_t *cid;

  osalDbgCheck((NULL != sdcp) && (NULL != cidsdc));
  cid = sdcp->cid;

  cidsdc->crc    = mmcsdGetSlice(cid, MMCSD_CID_SDC_CRC_SLICE);
  cidsdc->mdt_y  = mmcsdGetSlice(cid, MMCSD_CID_SDC_MDT_Y_SLICE) + 2000;
  cidsdc->mdt_m  = mmcsdGetSlice(cid, MMCSD_CID_SDC_MDT_M_SLICE);
  cidsdc->mid    = mmcsdGetSlice(cid, MMCSD_CID_SDC_MID_SLICE);
  cidsdc->oid    = mmcsdGetSlice(cid, MMCSD_CID_SDC_OID_SLICE);
  cidsdc->pnm[4] = mmcsdGetSlice(cid, MMCSD_CID_SDC_PNM0_SLICE);
  cidsdc->pnm[3] = mmcsdGetSlice(cid, MMCSD_CID_SDC_PNM1_SLICE);
  cidsdc->pnm[2] = mmcsdGetSlice(cid, MMCSD_CID_SDC_PNM2_SLICE);
  cidsdc->pnm[1] = mmcsdGetSlice(cid, MMCSD_CID_SDC_PNM3_SLICE);
  cidsdc->pnm[0] = mmcsdGetSlice(cid, MMCSD_CID_SDC_PNM4_SLICE);
  cidsdc->prv_n  = mmcsdGetSlice(cid, MMCSD_CID_SDC_PRV_N_SLICE);
  cidsdc->prv_m  = mmcsdGetSlice(cid, MMCSD_CID_SDC_PRV_M_SLICE);
  cidsdc->psn    = mmcsdGetSlice(cid, MMCSD_CID_SDC_PSN_SLICE);
}

/**
 * @brief   Unpacks MMC CID array in structure.
 *
 * @param[in] sdcp      pointer to the @p MMCSDBlockDevice object
 * @param[out] cidmmc   pointer to the @p unpacked_mmc_cid_t object
 *
 * @api
 */
void mmcUnpackCID(const MMCSDBlockDevice *sdcp, unpacked_mmc_cid_t *cidmmc) {
  const uint32_t *cid;

  osalDbgCheck((NULL != sdcp) && (NULL != cidmmc));
  cid = sdcp->cid;

  cidmmc->crc    = mmcsdGetSlice(cid, MMCSD_CID_MMC_CRC_SLICE);
  cidmmc->mdt_y  = mmcsdGetSlice(cid, MMCSD_CID_MMC_MDT_Y_SLICE) + 1997;
  cidmmc->mdt_m  = mmcsdGetSlice(cid, MMCSD_CID_MMC_MDT_M_SLICE);
  cidmmc->mid    = mmcsdGetSlice(cid, MMCSD_CID_MMC_MID_SLICE);
  cidmmc->oid    = mmcsdGetSlice(cid, MMCSD_CID_MMC_OID_SLICE);
  cidmmc->pnm[5] = mmcsdGetSlice(cid, MMCSD_CID_MMC_PNM0_SLICE);
  cidmmc->pnm[4] = mmcsdGetSlice(cid, MMCSD_CID_MMC_PNM1_SLICE);
  cidmmc->pnm[3] = mmcsdGetSlice(cid, MMCSD_CID_MMC_PNM2_SLICE);
  cidmmc->pnm[2] = mmcsdGetSlice(cid, MMCSD_CID_MMC_PNM3_SLICE);
  cidmmc->pnm[1] = mmcsdGetSlice(cid, MMCSD_CID_MMC_PNM4_SLICE);
  cidmmc->pnm[0] = mmcsdGetSlice(cid, MMCSD_CID_MMC_PNM5_SLICE);
  cidmmc->prv_n  = mmcsdGetSlice(cid, MMCSD_CID_MMC_PRV_N_SLICE);
  cidmmc->prv_m  = mmcsdGetSlice(cid, MMCSD_CID_MMC_PRV_M_SLICE);
  cidmmc->psn    = mmcsdGetSlice(cid, MMCSD_CID_MMC_PSN_SLICE);
}

/**
 * @brief   Unpacks MMC CSD array in structure.
 *
 * @param[in] sdcp      pointer to the @p MMCSDBlockDevice object
 * @param[out] csdmmc   pointer to the @p unpacked_mmc_csd_t object
 *
 * @api
 */
void mmcUnpackCSD(const MMCSDBlockDevice *sdcp, unpacked_mmc_csd_t *csdmmc) {
  const uint32_t *csd;

  osalDbgCheck((NULL != sdcp) && (NULL != csdmmc));
  csd = sdcp->csd;

  csdmmc->c_size             = mmcsdGetSlice(csd, MMCSD_CSD_MMC_C_SIZE_SLICE);
  csdmmc->c_size_mult        = mmcsdGetSlice(csd, MMCSD_CSD_MMC_C_SIZE_MULT_SLICE);
  csdmmc->ccc                = mmcsdGetSlice(csd, MMCSD_CSD_MMC_CCC_SLICE);
  csdmmc->copy               = mmcsdGetSlice(csd, MMCSD_CSD_MMC_COPY_SLICE);
  csdmmc->crc                = mmcsdGetSlice(csd, MMCSD_CSD_MMC_CRC_SLICE);
  csdmmc->csd_structure      = mmcsdGetSlice(csd, MMCSD_CSD_MMC_CSD_STRUCTURE_SLICE);
  csdmmc->dsr_imp            = mmcsdGetSlice(csd, MMCSD_CSD_MMC_DSR_IMP_SLICE);
  csdmmc->ecc                = mmcsdGetSlice(csd, MMCSD_CSD_MMC_ECC_SLICE);
  csdmmc->erase_grp_mult     = mmcsdGetSlice(csd, MMCSD_CSD_MMC_ERASE_GRP_MULT_SLICE);
  csdmmc->erase_grp_size     = mmcsdGetSlice(csd, MMCSD_CSD_MMC_ERASE_GRP_SIZE_SLICE);
  csdmmc->file_format        = mmcsdGetSlice(csd, MMCSD_CSD_MMC_FILE_FORMAT_SLICE);
  csdmmc->file_format_grp    = mmcsdGetSlice(csd, MMCSD_CSD_MMC_FILE_FORMAT_GRP_SLICE);
  csdmmc->nsac               = mmcsdGetSlice(csd, MMCSD_CSD_MMC_NSAC_SLICE);
  csdmmc->perm_write_protect = mmcsdGetSlice(csd, MMCSD_CSD_MMC_PERM_WRITE_PROTECT_SLICE);
  csdmmc->r2w_factor         = mmcsdGetSlice(csd, MMCSD_CSD_MMC_R2W_FACTOR_SLICE);
  csdmmc->read_bl_len        = mmcsdGetSlice(csd, MMCSD_CSD_MMC_READ_BL_LEN_SLICE);
  csdmmc->read_bl_partial    = mmcsdGetSlice(csd, MMCSD_CSD_MMC_READ_BL_PARTIAL_SLICE);
  csdmmc->read_blk_misalign  = mmcsdGetSlice(csd, MMCSD_CSD_MMC_READ_BLK_MISALIGN_SLICE);
  csdmmc->spec_vers          = mmcsdGetSlice(csd, MMCSD_CSD_MMC_SPEC_VERS_SLICE);
  csdmmc->taac               = mmcsdGetSlice(csd, MMCSD_CSD_MMC_TAAC_SLICE);
  csdmmc->tmp_write_protect  = mmcsdGetSlice(csd, MMCSD_CSD_MMC_TMP_WRITE_PROTECT_SLICE);
  csdmmc->tran_speed         = mmcsdGetSlice(csd, MMCSD_CSD_MMC_TRAN_SPEED_SLICE);
  csdmmc->vdd_r_curr_max     = mmcsdGetSlice(csd, MMCSD_CSD_MMC_VDD_R_CURR_MAX_SLICE);
  csdmmc->vdd_r_curr_min     = mmcsdGetSlice(csd, MMCSD_CSD_MMC_VDD_R_CURR_MIN_SLICE);
  csdmmc->vdd_w_curr_max     = mmcsdGetSlice(csd, MMCSD_CSD_MMC_VDD_W_CURR_MAX_SLICE);
  csdmmc->vdd_w_curr_min     = mmcsdGetSlice(csd, MMCSD_CSD_MMC_VDD_W_CURR_MIN_SLICE);
  csdmmc->wp_grp_enable      = mmcsdGetSlice(csd, MMCSD_CSD_MMC_WP_GRP_ENABLE_SLICE);
  csdmmc->wp_grp_size        = mmcsdGetSlice(csd, MMCSD_CSD_MMC_WP_GRP_SIZE_SLICE);
  csdmmc->write_bl_len       = mmcsdGetSlice(csd, MMCSD_CSD_MMC_WRITE_BL_LEN_SLICE);
  csdmmc->write_bl_partial   = mmcsdGetSlice(csd, MMCSD_CSD_MMC_WRITE_BL_PARTIAL_SLICE);
  csdmmc->write_blk_misalign = mmcsdGetSlice(csd, MMCSD_CSD_MMC_WRITE_BLK_MISALIGN_SLICE);
}

/**
 * @brief   Unpacks SDC CSD v1.0 array in structure.
 *
 * @param[in] sdcp      pointer to the @p MMCSDBlockDevice object
 * @param[out] csd10    pointer to the @p unpacked_sdc_csd_10_t object
 *
 * @api
 */
void sdcUnpackCSDv10(const MMCSDBlockDevice *sdcp,
                     unpacked_sdc_csd_10_t *csd10) {
  const uint32_t *csd;

  osalDbgCheck(NULL != sdcp);
  csd = sdcp->csd;

  csd10->c_size              = mmcsdGetSlice(csd, MMCSD_CSD_10_C_SIZE_SLICE);
  csd10->c_size_mult         = mmcsdGetSlice(csd, MMCSD_CSD_10_C_SIZE_MULT_SLICE);
  csd10->ccc                 = mmcsdGetSlice(csd, MMCSD_CSD_10_CCC_SLICE);
  csd10->copy                = mmcsdGetSlice(csd, MMCSD_CSD_10_COPY_SLICE);
  csd10->crc                 = mmcsdGetSlice(csd, MMCSD_CSD_10_CRC_SLICE);
  csd10->csd_structure       = mmcsdGetSlice(csd, MMCSD_CSD_10_CSD_STRUCTURE_SLICE);
  csd10->dsr_imp             = mmcsdGetSlice(csd, MMCSD_CSD_10_DSR_IMP_SLICE);
  csd10->erase_blk_en        = mmcsdGetSlice(csd, MMCSD_CSD_10_ERASE_BLK_EN_SLICE);
  csd10->erase_sector_size   = mmcsdGetSlice(csd, MMCSD_CSD_10_ERASE_SECTOR_SIZE_SLICE);
  csd10->file_format         = mmcsdGetSlice(csd, MMCSD_CSD_10_FILE_FORMAT_SLICE);
  csd10->file_format_grp     = mmcsdGetSlice(csd, MMCSD_CSD_10_FILE_FORMAT_GRP_SLICE);
  csd10->nsac                = mmcsdGetSlice(csd, MMCSD_CSD_10_NSAC_SLICE);
  csd10->perm_write_protect  = mmcsdGetSlice(csd, MMCSD_CSD_10_PERM_WRITE_PROTECT_SLICE);
  csd10->r2w_factor          = mmcsdGetSlice(csd, MMCSD_CSD_10_R2W_FACTOR_SLICE);
  csd10->read_bl_len         = mmcsdGetSlice(csd, MMCSD_CSD_10_READ_BL_LEN_SLICE);
  csd10->read_bl_partial     = mmcsdGetSlice(csd, MMCSD_CSD_10_READ_BL_PARTIAL_SLICE);
  csd10->read_blk_misalign   = mmcsdGetSlice(csd, MMCSD_CSD_10_READ_BLK_MISALIGN_SLICE);
  csd10->taac                = mmcsdGetSlice(csd, MMCSD_CSD_10_TAAC_SLICE);
  csd10->tmp_write_protect   = mmcsdGetSlice(csd, MMCSD_CSD_10_TMP_WRITE_PROTECT_SLICE);
  csd10->tran_speed          = mmcsdGetSlice(csd, MMCSD_CSD_10_TRANS_SPEED_SLICE);
  csd10->wp_grp_enable       = mmcsdGetSlice(csd, MMCSD_CSD_10_WP_GRP_ENABLE_SLICE);
  csd10->wp_grp_size         = mmcsdGetSlice(csd, MMCSD_CSD_10_WP_GRP_SIZE_SLICE);
  csd10->write_bl_len        = mmcsdGetSlice(csd, MMCSD_CSD_10_WRITE_BL_LEN_SLICE);
  csd10->write_bl_partial    = mmcsdGetSlice(csd, MMCSD_CSD_10_WRITE_BL_PARTIAL_SLICE);
  csd10->write_blk_misalign  = mmcsdGetSlice(csd, MMCSD_CSD_10_WRITE_BLK_MISALIGN_SLICE);
}

/**
 * @brief   Unpacks SDC CSD v2.0 array in structure.
 *
 * @param[in] sdcp      pointer to the @p MMCSDBlockDevice object
 * @param[out] csd20    pointer to the @p unpacked_sdc_csd_20_t object
 *
 * @api
 */
void sdcUnpackCSDv20(const MMCSDBlockDevice *sdcp,
                     unmacked_sdc_csd_20_t *csd20) {
  const uint32_t *csd;

  osalDbgCheck(NULL != sdcp);
  csd = sdcp->csd;

  csd20->c_size              = mmcsdGetSlice(csd, MMCSD_CSD_20_C_SIZE_SLICE);
  csd20->crc                 = mmcsdGetSlice(csd, MMCSD_CSD_20_CRC_SLICE);
  csd20->ccc                 = mmcsdGetSlice(csd, MMCSD_CSD_20_CCC_SLICE);
  csd20->copy                = mmcsdGetSlice(csd, MMCSD_CSD_20_COPY_SLICE);
  csd20->csd_structure       = mmcsdGetSlice(csd, MMCSD_CSD_20_CSD_STRUCTURE_SLICE);
  csd20->dsr_imp             = mmcsdGetSlice(csd, MMCSD_CSD_20_DSR_IMP_SLICE);
  csd20->erase_blk_en        = mmcsdGetSlice(csd, MMCSD_CSD_20_ERASE_BLK_EN_SLICE);
  csd20->file_format         = mmcsdGetSlice(csd, MMCSD_CSD_20_FILE_FORMAT_SLICE);
  csd20->file_format_grp     = mmcsdGetSlice(csd, MMCSD_CSD_20_FILE_FORMAT_GRP_SLICE);
  csd20->nsac                = mmcsdGetSlice(csd, MMCSD_CSD_20_NSAC_SLICE);
  csd20->perm_write_protect  = mmcsdGetSlice(csd, MMCSD_CSD_20_PERM_WRITE_PROTECT_SLICE);
  csd20->r2w_factor          = mmcsdGetSlice(csd, MMCSD_CSD_20_R2W_FACTOR_SLICE);
  csd20->read_bl_len         = mmcsdGetSlice(csd, MMCSD_CSD_20_READ_BL_LEN_SLICE);
  csd20->read_bl_partial     = mmcsdGetSlice(csd, MMCSD_CSD_20_READ_BL_PARTIAL_SLICE);
  csd20->read_blk_misalign   = mmcsdGetSlice(csd, MMCSD_CSD_20_READ_BLK_MISALIGN_SLICE);
  csd20->erase_sector_size   = mmcsdGetSlice(csd, MMCSD_CSD_20_ERASE_SECTOR_SIZE_SLICE);
  csd20->taac                = mmcsdGetSlice(csd, MMCSD_CSD_20_TAAC_SLICE);
  csd20->tmp_write_protect   = mmcsdGetSlice(csd, MMCSD_CSD_20_TMP_WRITE_PROTECT_SLICE);
  csd20->tran_speed          = mmcsdGetSlice(csd, MMCSD_CSD_20_TRANS_SPEED_SLICE);
  csd20->wp_grp_enable       = mmcsdGetSlice(csd, MMCSD_CSD_20_WP_GRP_ENABLE_SLICE);
  csd20->wp_grp_size         = mmcsdGetSlice(csd, MMCSD_CSD_20_WP_GRP_SIZE_SLICE);
  csd20->write_bl_len        = mmcsdGetSlice(csd, MMCSD_CSD_20_WRITE_BL_LEN_SLICE);
  csd20->write_bl_partial    = mmcsdGetSlice(csd, MMCSD_CSD_20_WRITE_BL_PARTIAL_SLICE);
  csd20->write_blk_misalign  = mmcsdGetSlice(csd, MMCSD_CSD_20_WRITE_BLK_MISALIGN_SLICE);
}

#endif /* HAL_USE_MMC_SPI || HAL_USE_SDC */

/** @} */
