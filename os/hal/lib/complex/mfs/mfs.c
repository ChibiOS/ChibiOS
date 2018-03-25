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
 * @file    mfs.c
 * @brief   Managed Flash Storage module code.
 * @details This module manages a flash partition as a generic storage where
 *          arbitrary data records can be created, updated, deleted and
 *          retrieved.<br>
 *          A managed partition is composed of two banks of equal size, a
 *          bank is composed of one or more erasable sectors, a sector is
 *          divided in writable pages.<br>
 *          The module handles flash wear leveling and recovery of damaged
 *          banks (where possible) caused by power loss during operations.
 *          Both operations are transparent to the user.
 *
 * @addtogroup mfs
 * @{
 */

#include <string.h>

#include "hal.h"

#include "mfs.h"

/*===========================================================================*/
/* Driver local definitions.                                                 */
/*===========================================================================*/

#define PAIR(a, b) (((unsigned)(a) << 2U) | (unsigned)(b))

/**
 * @brief   Error check helper.
 */
#define RET_ON_ERROR(err) do {                                              \
  mfs_error_t e = (err);                                                    \
  if (e != MFS_NO_ERROR) {                                                  \
    return e;                                                               \
  }                                                                         \
} while (false)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables and types.                                         */
/*===========================================================================*/

static const uint16_t crc16_table[256] = {
  0x0000, 0x1021, 0x2042, 0x3063, 0x4084, 0x50A5, 0x60C6, 0x70E7,
  0x8108, 0x9129, 0xA14A, 0xB16B, 0xC18C, 0xD1AD, 0xE1CE, 0xF1EF,
  0x1231, 0x0210, 0x3273, 0x2252, 0x52B5, 0x4294, 0x72F7, 0x62D6,
  0x9339, 0x8318, 0xB37B, 0xA35A, 0xD3BD, 0xC39C, 0xF3FF, 0xE3DE,
  0x2462, 0x3443, 0x0420, 0x1401, 0x64E6, 0x74C7, 0x44A4, 0x5485,
  0xA56A, 0xB54B, 0x8528, 0x9509, 0xE5EE, 0xF5CF, 0xC5AC, 0xD58D,
  0x3653, 0x2672, 0x1611, 0x0630, 0x76D7, 0x66F6, 0x5695, 0x46B4,
  0xB75B, 0xA77A, 0x9719, 0x8738, 0xF7DF, 0xE7FE, 0xD79D, 0xC7BC,
  0x48C4, 0x58E5, 0x6886, 0x78A7, 0x0840, 0x1861, 0x2802, 0x3823,
  0xC9CC, 0xD9ED, 0xE98E, 0xF9AF, 0x8948, 0x9969, 0xA90A, 0xB92B,
  0x5AF5, 0x4AD4, 0x7AB7, 0x6A96, 0x1A71, 0x0A50, 0x3A33, 0x2A12,
  0xDBFD, 0xCBDC, 0xFBBF, 0xEB9E, 0x9B79, 0x8B58, 0xBB3B, 0xAB1A,
  0x6CA6, 0x7C87, 0x4CE4, 0x5CC5, 0x2C22, 0x3C03, 0x0C60, 0x1C41,
  0xEDAE, 0xFD8F, 0xCDEC, 0xDDCD, 0xAD2A, 0xBD0B, 0x8D68, 0x9D49,
  0x7E97, 0x6EB6, 0x5ED5, 0x4EF4, 0x3E13, 0x2E32, 0x1E51, 0x0E70,
  0xFF9F, 0xEFBE, 0xDFDD, 0xCFFC, 0xBF1B, 0xAF3A, 0x9F59, 0x8F78,
  0x9188, 0x81A9, 0xB1CA, 0xA1EB, 0xD10C, 0xC12D, 0xF14E, 0xE16F,
  0x1080, 0x00A1, 0x30C2, 0x20E3, 0x5004, 0x4025, 0x7046, 0x6067,
  0x83B9, 0x9398, 0xA3FB, 0xB3DA, 0xC33D, 0xD31C, 0xE37F, 0xF35E,
  0x02B1, 0x1290, 0x22F3, 0x32D2, 0x4235, 0x5214, 0x6277, 0x7256,
  0xB5EA, 0xA5CB, 0x95A8, 0x8589, 0xF56E, 0xE54F, 0xD52C, 0xC50D,
  0x34E2, 0x24C3, 0x14A0, 0x0481, 0x7466, 0x6447, 0x5424, 0x4405,
  0xA7DB, 0xB7FA, 0x8799, 0x97B8, 0xE75F, 0xF77E, 0xC71D, 0xD73C,
  0x26D3, 0x36F2, 0x0691, 0x16B0, 0x6657, 0x7676, 0x4615, 0x5634,
  0xD94C, 0xC96D, 0xF90E, 0xE92F, 0x99C8, 0x89E9, 0xB98A, 0xA9AB,
  0x5844, 0x4865, 0x7806, 0x6827, 0x18C0, 0x08E1, 0x3882, 0x28A3,
  0xCB7D, 0xDB5C, 0xEB3F, 0xFB1E, 0x8BF9, 0x9BD8, 0xABBB, 0xBB9A,
  0x4A75, 0x5A54, 0x6A37, 0x7A16, 0x0AF1, 0x1AD0, 0x2AB3, 0x3A92,
  0xFD2E, 0xED0F, 0xDD6C, 0xCD4D, 0xBDAA, 0xAD8B, 0x9DE8, 0x8DC9,
  0x7C26, 0x6C07, 0x5C64, 0x4C45, 0x3CA2, 0x2C83, 0x1CE0, 0x0CC1,
  0xEF1F, 0xFF3E, 0xCF5D, 0xDF7C, 0xAF9B, 0xBFBA, 0x8FD9, 0x9FF8,
  0x6E17, 0x7E36, 0x4E55, 0x5E74, 0x2E93, 0x3EB2, 0x0ED1, 0x1EF0
};

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

uint16_t crc16(uint16_t crc, const uint8_t *data, size_t n) {

  while (n > 0U) {
    crc = (crc << 8U) ^ crc16_table[(crc >> 8U) ^ (uint16_t)*data];
    data++;
    n--;
  }

  return crc;
}

static void mfs_state_reset(MFSDriver *mfsp) {
  unsigned i;

  mfsp->current_bank    = MFS_BANK_0;
  mfsp->current_counter = 0U;
  mfsp->next_offset     = 0U;
  mfsp->used_space      = 0U;

  for (i = 0; i < MFS_CFG_MAX_RECORDS; i++) {
    mfsp->descriptors[i].offset = 0U;
    mfsp->descriptors[i].size   = 0U;
  }
}

static flash_offset_t mfs_flash_get_bank_offset(MFSDriver *mfsp,
                                                mfs_bank_t bank) {

  return bank == MFS_BANK_0 ? flashGetSectorOffset(mfsp->config->flashp,
                                                   mfsp->config->bank0_start) :
                              flashGetSectorOffset(mfsp->config->flashp,
                                                   mfsp->config->bank1_start);
}

/**
 * @brief   Flash read.
 *
 * @param[in] mfsp      pointer to the @p MFSDriver object
 * @param[in] offset    flash offset
 * @param[in] n         number of bytes to be read
 * @param[out] rp       pointer to the data buffer
 * @return              The operation status.
 *
 * @notapi
 */
static mfs_error_t mfs_flash_read(MFSDriver *mfsp, flash_offset_t offset,
                                  size_t n, uint8_t *rp) {
  flash_error_t ferr;

  ferr = flashRead(mfsp->config->flashp, offset, n, rp);
  if (ferr != FLASH_NO_ERROR) {
    mfsp->state = MFS_ERROR;
    return MFS_ERR_FLASH_FAILURE;
  }

  return MFS_NO_ERROR;
}

/**
 * @brief   Flash write.
 * @note    If the option @p MFS_CFG_WRITE_VERIFY is enabled then the flash
 *          is also read back for verification.
 *
 * @param[in] mfsp      pointer to the @p MFSDriver object
 * @param[in] offset    flash offset
 * @param[in] n         number of bytes to be written
 * @param[in] wp        pointer to the data buffer
 * @return              The operation status.
 *
 * @notapi
 */
static mfs_error_t mfs_flash_write(MFSDriver *mfsp,
                                   flash_offset_t offset,
                                   size_t n,
                                   const uint8_t *wp) {
  flash_error_t ferr;

  ferr = flashProgram(mfsp->config->flashp, offset, n, wp);
  if (ferr != FLASH_NO_ERROR) {
    mfsp->state = MFS_ERROR;
    return MFS_ERR_FLASH_FAILURE;
  }

#if MFS_CFG_WRITE_VERIFY == TRUE
  /* Verifying the written data by reading it back and comparing.*/
  while (n > 0U) {
    size_t chunk = n <= MFS_CFG_BUFFER_SIZE ? n : MFS_CFG_BUFFER_SIZE;
    RET_ON_ERROR(mfs_flash_read(mfsp, offset, chunk, mfsp->buffer.data8));
    if (memcmp((void *)mfsp->buffer.data8, (void *)wp, chunk)) {
      mfsp->state = MFS_ERROR;
      return MFS_ERR_FLASH_FAILURE;
    }
    n -= chunk;
    offset += (flash_offset_t)chunk;
    wp += chunk;
  }
#endif

  return MFS_NO_ERROR;
}

/**
 * @brief   Flash copy.
 * @note    If the option @p MFS_CFG_WRITE_VERIFY is enabled then the flash
 *          is also read back for verification.
 *
 * @param[in] mfsp      pointer to the @p MFSDriver object
 * @param[in] doffset   destination flash offset
 * @param[in] soffset   source flash offset
 * @param[in] n         number of bytes to be copied
 * @return              The operation status.
 *
 * @notapi
 */
static mfs_error_t mfs_flash_copy(MFSDriver *mfsp,
                                  flash_offset_t doffset,
                                  flash_offset_t soffset,
                                  uint32_t n) {

  /* Splitting the operation in smaller operations because the buffer is
     small.*/
  while (n > 0U) {
    /* Data size that can be written in a single program page operation.*/
    size_t chunk = (size_t)(((doffset | (MFS_CFG_BUFFER_SIZE - 1U)) + 1U) -
                            doffset);
    if (chunk > n) {
      chunk = n;
    }

    RET_ON_ERROR(mfs_flash_read(mfsp, soffset, chunk, mfsp->buffer.data8));
    RET_ON_ERROR(mfs_flash_write(mfsp, doffset, chunk, mfsp->buffer.data8));

    /* Next page.*/
    soffset += chunk;
    doffset += chunk;
    n       -= chunk;
  }

  return MFS_NO_ERROR;
}

/**
 * @brief   Verifies integrity of a record.
 *
 * @param[in] mfsp      pointer to the @p MFSDriver object
 * @param[in] dhdrp     pointer to the header to be checked
 * @param[in] offset    flash offset of the header to be checked
 * @param[in] limit     flash limit offset
 * @param[out] sts      assessed record state
 * @return              The operation status.
 *
 * @notapi
 */
static mfs_error_t mfs_record_check(MFSDriver *mfsp,
                                    mfs_data_header_t *dhdrp,
                                    flash_offset_t offset,
                                    flash_offset_t limit,
                                    mfs_record_state_t *sts) {
  unsigned i;

  for (i = 0; i < 3; i++) {
    if (dhdrp->hdr32[i] != mfsp->config->erased) {
      /* Not erased must verify the header.*/
      if ((dhdrp->fields.magic != MFS_HEADER_MAGIC) ||
          (dhdrp->fields.id < (uint16_t)1) ||
          (dhdrp->fields.id > (uint16_t)MFS_CFG_MAX_RECORDS) ||
          (dhdrp->fields.size + sizeof (mfs_data_header_t) > limit - offset)) {
        *sts = MFS_RECORD_GARBAGE;
        return MFS_NO_ERROR;
      }
#if MFS_CFG_STRONG_CHECKING == TRUE
      {
        /* TODO: Checking the CRC while reading the record data.*/
        (void)mfsp;
      }
#else
      (void)mfsp;
#endif
    }
  }

  /* It is fully erased.*/
  *sts = MFS_RECORD_ERASED;
  return MFS_NO_ERROR;
}

/**
 * @brief   Erases and verifies all sectors belonging to a bank.
 *
 * @param[in] mfsp      pointer to the @p MFSDriver object
 * @param[in] bank      bank to be erased
 * @return              The operation status.
 *
 * @notapi
 */
static mfs_error_t mfs_bank_erase(MFSDriver *mfsp, mfs_bank_t bank) {
  flash_sector_t sector, end;

  if (bank == MFS_BANK_0) {
    sector = mfsp->config->bank0_start;
    end    = mfsp->config->bank0_start + mfsp->config->bank0_sectors;
  }
  else {
    sector = mfsp->config->bank1_start;
    end    = mfsp->config->bank1_start + mfsp->config->bank1_sectors;
  }

  while (sector < end) {
    flash_error_t ferr;

    ferr = flashStartEraseSector(mfsp->config->flashp, sector);
    if (ferr != FLASH_NO_ERROR) {
      mfsp->state = MFS_ERROR;
      return MFS_ERR_FLASH_FAILURE;
    }
    ferr = flashWaitErase(mfsp->config->flashp);
    if (ferr != FLASH_NO_ERROR) {
      mfsp->state = MFS_ERROR;
      return MFS_ERR_FLASH_FAILURE;
    }
    ferr = flashVerifyErase(mfsp->config->flashp, sector);
    if (ferr != FLASH_NO_ERROR) {
      mfsp->state = MFS_ERROR;
      return MFS_ERR_FLASH_FAILURE;
    }

    sector++;
  }

  return MFS_NO_ERROR;
}

/**
 * @brief   Erases and verifies all sectors belonging to a bank.
 *
 * @param[in] mfsp      pointer to the @p MFSDriver object
 * @param[in] bank      bank to be verified
 * @return              The operation status.
 *
 * @notapi
 */
static mfs_error_t mfs_bank_verify_erase(MFSDriver *mfsp, mfs_bank_t bank) {
  flash_sector_t sector, end;

  if (bank == MFS_BANK_0) {
    sector = mfsp->config->bank0_start;
    end    = mfsp->config->bank0_start + mfsp->config->bank0_sectors;
  }
  else {
    sector = mfsp->config->bank1_start;
    end    = mfsp->config->bank1_start + mfsp->config->bank1_sectors;
  }

  while (sector < end) {
    flash_error_t ferr;

    ferr = flashVerifyErase(mfsp->config->flashp, sector);
    if (ferr == FLASH_ERROR_VERIFY) {
      return MFS_ERR_NOT_ERASED;
    }
    if (ferr != FLASH_NO_ERROR) {
      mfsp->state = MFS_ERROR;
      return MFS_ERR_FLASH_FAILURE;
    }

    sector++;
  }

  return MFS_NO_ERROR;
}

/**
 * @brief   Writes the validation header in a bank.
 *
 * @param[in] mfsp      pointer to the @p MFSDriver object
 * @param[in] bank      bank to be validated
 * @param[in] cnt       value for the flash usage counter
 * @return              The operation status.
 * @retval MFS_NO_ERROR if the operation has been successfully completed.
 *
 * @notapi
 */
static mfs_error_t mfs_bank_write_header(MFSDriver *mfsp,
                                         mfs_bank_t bank,
                                         uint32_t cnt) {
  flash_sector_t sector;
  mfs_bank_header_t bhdr;

  if (bank == MFS_BANK_0) {
    sector = mfsp->config->bank0_start;
  }
  else {
    sector = mfsp->config->bank1_start;
  }

  bhdr.fields.magic1    = MFS_BANK_MAGIC_1;
  bhdr.fields.magic2    = MFS_BANK_MAGIC_2;
  bhdr.fields.counter   = cnt;
  bhdr.fields.reserved1 = (uint16_t)mfsp->config->erased;
  bhdr.fields.crc       = crc16(0xFFFFU, bhdr.hdr8,
                                sizeof (mfs_bank_header_t) - sizeof (uint16_t));

  return mfs_flash_write(mfsp,
                         flashGetSectorOffset(mfsp->config->flashp, sector),
                         sizeof (mfs_bank_header_t),
                         bhdr.hdr8);
}

/**
 * @brief   Scans blocks searching for records.
 * @note    The block integrity is strongly checked.
 *
 * @param[in] mfsp      pointer to the @p MFSDriver object
 * @param[in] bank      the bank identifier
 * @param[out] statep   bank state, it can be:
 *                      - MFS_BANK_PARTIAL
 *                      - MFS_BANK_OK
 *                      .
 *
 * @param[in] foundcb   callback to be called for each found record or @p NULL
 * @param[in] endcb     callback to be called after scanning or @p NULL
 *
 * @return              The operation status.
 *
 * @notapi
 */
static mfs_error_t mfs_bank_scan_records(MFSDriver *mfsp,
                                         mfs_bank_t bank,
                                         mfs_bank_state_t *statep) {
  flash_offset_t hdr_offset, start_offset, end_offset;
  mfs_record_state_t sts;
  bool warning = false;

  start_offset = mfs_flash_get_bank_offset(mfsp, bank);
  end_offset   = start_offset + mfsp->config->bank_size;

  /* Scanning records.*/
  hdr_offset = start_offset + (flash_offset_t)sizeof(mfs_bank_header_t);
  while (hdr_offset < end_offset) {
    /* Reading the current record header.*/
    RET_ON_ERROR(mfs_flash_read(mfsp, hdr_offset,
                                sizeof (mfs_data_header_t),
                                (void *)&mfsp->buffer.dhdr));

    /* Checking header/data integrity.*/
    RET_ON_ERROR(mfs_record_check(mfsp, &mfsp->buffer.dhdr,
                                  hdr_offset, end_offset, &sts));
    if (sts == MFS_RECORD_ERASED) {
      /* Record area fully erased, stopping scan.*/
      break;
    }
    else if (sts == MFS_RECORD_OK) {
      /* Record OK.*/
      uint32_t size = mfsp->buffer.dhdr.fields.size;

      /* Zero-sized records are erase markers.*/
      if (size == 0U) {
        mfsp->descriptors[mfsp->buffer.dhdr.fields.id - 1U].offset = 0U;
        mfsp->descriptors[mfsp->buffer.dhdr.fields.id - 1U].size   = 0U;
      }
      else {
        mfsp->descriptors[mfsp->buffer.dhdr.fields.id - 1U].offset = hdr_offset;
        mfsp->descriptors[mfsp->buffer.dhdr.fields.id - 1U].size   = size;
      }
    }
    else if (sts == MFS_RECORD_CRC) {
      /* Record payload corrupted, scan can continue because the header
         is OK.*/
      warning = true;
    }
    else {
      /* Unrecognized header, scanning cannot continue.*/
      warning = true;
      break;
    }
  }

  if (hdr_offset > end_offset) {
    return MFS_ERR_INTERNAL;
  }

  /* Final.*/
  mfsp->next_offset = hdr_offset;

  if (warning) {
    *statep = MFS_BANK_PARTIAL;
  }
  else {
    *statep = MFS_BANK_OK;
  }

  return MFS_NO_ERROR;
}

/**
 * @brief   Determines the state of a bank.
 * @note    This function does not test the bank integrity by scanning
 *          the data area, it just checks the header.
 *
 * @param[in] mfsp      pointer to the @p MFSDriver object
 * @param[in] bank      bank to be checked
 * @param[out] statep   bank state, it can be:
 *                      - MFS_BANK_ERASED
 *                      - MFS_BANK_GARBAGE
 *                      - MFS_BANK_OK
 *                      .
 * @param[out] cntp     bank counter
 * @return              The operation status.
 *
 * @notapi
 */
static mfs_error_t mfs_bank_get_state(MFSDriver *mfsp,
                                      mfs_bank_t bank,
                                      mfs_bank_state_t *statep,
                                      uint32_t * cntp) {
  unsigned i;
  mfs_error_t err;
  uint16_t crc;

  /* Worst case is default.*/
  *statep = MFS_BANK_GARBAGE;
  *cntp   = 0U;

  /* Reading the current bank header.*/
  RET_ON_ERROR(mfs_flash_read(mfsp, mfs_flash_get_bank_offset(mfsp, bank),
                              sizeof (mfs_bank_header_t),
                              (void *)&mfsp->buffer.bhdr));

  /* Checking the special case where the header is erased.*/
  for (i = 0; i < 4; i++) {
    if (mfsp->buffer.bhdr.hdr32[i] != mfsp->config->erased) {

      /* Checking header fields integrity.*/
      if ((mfsp->buffer.bhdr.fields.magic1 != MFS_BANK_MAGIC_1) ||
          (mfsp->buffer.bhdr.fields.magic2 != MFS_BANK_MAGIC_2) ||
          (mfsp->buffer.bhdr.fields.counter == mfsp->config->erased) ||
          (mfsp->buffer.bhdr.fields.reserved1 != (uint16_t)mfsp->config->erased)) {
        return MFS_NO_ERROR;
      }

      /* Verifying header CRC.*/
      crc = crc16(0xFFFFU, mfsp->buffer.bhdr.hdr8,
                  sizeof (mfs_bank_header_t) - sizeof (uint16_t));
      if (crc != mfsp->buffer.bhdr.fields.crc) {
        return MFS_NO_ERROR;
      }

      *statep = MFS_BANK_OK;
      *cntp   = mfsp->buffer.bhdr.fields.counter;

      return MFS_NO_ERROR;
    }
  }

  /* If the header is erased then it could be the whole block erased.*/
  err = mfs_bank_verify_erase(mfsp, bank);
  if (err == MFS_NO_ERROR) {
    *statep = MFS_BANK_ERASED;
  }

  return err;
}

/**
 * @brief   Selects a bank as current.
 * @note    The bank header is assumed to be valid.
 *
 * @param[in] mfsp      pointer to the @p MFSDriver object
 * @param[in] bank      bank to be scanned
 * @param[out] statep   bank state, it can be:
 *                      - MFS_BANK_ERASED
 *                      - MFS_BANK_GARBAGE
 *                      - MFS_BANK_PARTIAL
 *                      - MFS_BANK_OK
 *                      .
 * @return              The operation status.
 *
 * @notapi
 */
static mfs_error_t mfs_bank_mount(MFSDriver *mfsp,
                                  mfs_bank_t bank,
                                  mfs_bank_state_t *statep) {
  unsigned i;

  /* Resetting the bank state, then reading the required header data.*/
  mfs_state_reset(mfsp);
  RET_ON_ERROR(mfs_bank_get_state(mfsp, bank, statep, &mfsp->current_counter));
  mfsp->current_bank = bank;

  /* Scanning for the most recent instance of all records.*/
  RET_ON_ERROR(mfs_bank_scan_records(mfsp, bank, statep));

  /* Calculating the effective used size.*/
  mfsp->used_space = sizeof (mfs_bank_header_t);
  for (i = 0; i < MFS_CFG_MAX_RECORDS; i++) {
    if (mfsp->descriptors[i].offset != 0U) {
      mfsp->used_space += mfsp->descriptors[i].size + sizeof (mfs_data_header_t);
    }
  }

  return MFS_NO_ERROR;
}

/**
 * @brief   Enforces a garbage collection.
 * @details Storage data is compacted into a single bank.
 *
 * @param[out] mfsp     pointer to the @p MFSDriver object
 * @return              The operation status.
 *
 * @notapi
 */
static mfs_error_t mfs_garbage_collect(MFSDriver *mfsp) {
  unsigned i;
  mfs_bank_t sbank, dbank;
  flash_offset_t dest_offset;

  sbank = mfsp->current_bank;
  if (sbank == MFS_BANK_0) {
    dbank = MFS_BANK_1;
  }
  else {
    dbank = MFS_BANK_0;
  }

  /* Write address.*/
  dest_offset = mfs_flash_get_bank_offset(mfsp, dbank) +
                sizeof (mfs_bank_header_t);

  /* Copying the most recent record instances only.*/
  for (i = 0; i < MFS_CFG_MAX_RECORDS; i++) {
    uint32_t totsize = mfsp->descriptors[i].size + sizeof (mfs_data_header_t);
    if (mfsp->descriptors[i].offset != 0) {
      RET_ON_ERROR(mfs_flash_copy(mfsp, dest_offset,
                                  mfsp->descriptors[i].offset,
                                  totsize));
      mfsp->descriptors[i].offset = dest_offset;
      dest_offset += totsize;
    }
  }

  /* New current bank.*/
  mfsp->current_bank = dbank;
  mfsp->current_counter += 1U;
  mfsp->next_offset = dest_offset;

  /* The header is written after the data.*/
  RET_ON_ERROR(mfs_bank_write_header(mfsp, dbank, mfsp->current_counter));

  /* The source bank is erased last.*/
  RET_ON_ERROR(mfs_bank_erase(mfsp, sbank));

  return MFS_NO_ERROR;
}

/**
 * @brief   Performs a flash partition mount attempt.
 *
 * @param[in] mfsp      pointer to the @p MFSDriver object
 * @return              The operation status.
 *
 * @api
 */
static mfs_error_t mfs_try_mount(MFSDriver *mfsp) {
  mfs_bank_state_t sts, sts0, sts1;
  mfs_bank_t bank;
  uint32_t cnt0 = 0, cnt1 = 0;
  bool warning = false;

  /* Assessing the state of the two banks.*/
  RET_ON_ERROR(mfs_bank_get_state(mfsp, MFS_BANK_0, &sts0, &cnt0));
  RET_ON_ERROR(mfs_bank_get_state(mfsp, MFS_BANK_1, &sts1, &cnt1));

  /* Handling all possible scenarios, each one requires its own recovery
     strategy.*/
  switch (PAIR(sts0, sts1)) {

  case PAIR(MFS_BANK_ERASED, MFS_BANK_ERASED):
    /* Both banks erased, first initialization.*/
    RET_ON_ERROR(mfs_bank_write_header(mfsp, MFS_BANK_0, 1));
    bank = MFS_BANK_0;
    break;

  case PAIR(MFS_BANK_OK, MFS_BANK_OK):
    /* Both banks appear to be valid but one must be newer, erasing the
       older one.*/
    if (cnt0 > cnt1) {
      /* Bank 0 is newer.*/
      RET_ON_ERROR(mfs_bank_erase(mfsp, MFS_BANK_1));
      bank = MFS_BANK_0;
    }
    else {
      /* Bank 1 is newer.*/
      RET_ON_ERROR(mfs_bank_erase(mfsp, MFS_BANK_0));
      bank = MFS_BANK_1;
    }
    warning = true;
    break;

  case PAIR(MFS_BANK_GARBAGE, MFS_BANK_GARBAGE):
    /* Both banks are unreadable, reinitializing.*/
    RET_ON_ERROR(mfs_bank_erase(mfsp, MFS_BANK_0));
    RET_ON_ERROR(mfs_bank_erase(mfsp, MFS_BANK_1));
    RET_ON_ERROR(mfs_bank_write_header(mfsp, MFS_BANK_0, 1));
    bank = MFS_BANK_0;
    warning = true;
    break;

  case PAIR(MFS_BANK_ERASED, MFS_BANK_OK):
    /* Normal situation, bank one is used.*/
    bank = MFS_BANK_1;
    break;

  case PAIR(MFS_BANK_OK, MFS_BANK_ERASED):
    /* Normal situation, bank zero is used.*/
    bank = MFS_BANK_0;
    break;

  case PAIR(MFS_BANK_ERASED, MFS_BANK_GARBAGE):
    /* Bank zero is erased, bank one is not readable.*/
    RET_ON_ERROR(mfs_bank_erase(mfsp, MFS_BANK_1));
    RET_ON_ERROR(mfs_bank_write_header(mfsp, MFS_BANK_0, 1));
    bank = MFS_BANK_0;
    warning = true;
    break;

  case PAIR(MFS_BANK_GARBAGE, MFS_BANK_ERASED):
    /* Bank zero is not readable, bank one is erased.*/
    RET_ON_ERROR(mfs_bank_erase(mfsp, MFS_BANK_0));
    RET_ON_ERROR(mfs_bank_write_header(mfsp, MFS_BANK_1, 1));
    bank = MFS_BANK_1;
    warning = true;
    break;

  case PAIR(MFS_BANK_OK, MFS_BANK_GARBAGE):
    /* Bank zero is normal, bank one is unreadable.*/
    RET_ON_ERROR(mfs_bank_erase(mfsp, MFS_BANK_1));
    bank = MFS_BANK_0;
    warning = true;
    break;

  case PAIR(MFS_BANK_GARBAGE, MFS_BANK_OK):
    /* Bank zero is unreadable, bank one is normal.*/
    RET_ON_ERROR(mfs_bank_erase(mfsp, MFS_BANK_0));
    bank = MFS_BANK_1;
    warning = true;
    break;

  default:
    return MFS_ERR_INTERNAL;
  }

  /* Mounting the bank.*/
  RET_ON_ERROR(mfs_bank_mount(mfsp, bank, &sts));

  /* This condition should not occur, the bank has just been repaired.*/
  if ((sts == MFS_BANK_ERASED) || (sts == MFS_BANK_GARBAGE)) {
    return MFS_ERR_INTERNAL;
  }

  /* In case of detected problems then a garbage collection is performed in
     order to repair/remove anomalies.*/
  if (sts == MFS_BANK_PARTIAL) {
    RET_ON_ERROR(mfs_garbage_collect(mfsp));
    warning = true;
  }

  return warning ? MFS_WARN_REPAIR : MFS_NO_ERROR;
}

/**
 * @brief   Configures and activates a MFS driver.
 *
 * @param[in] mfsp      pointer to the @p MFSDriver object
 * @param[in] config    pointer to the configuration
 * @return              The operation status.
 * @retval MFS_NO_ERROR if the operation has been successfully completed.
 * @retval MFS_WARN_GC  if the operation triggered a garbage collection.
 * @retval MFS_ERR_FLASH_FAILURE if the flash memory is unusable because HW
 *                      failures. Makes the driver enter the @p MFS_ERROR state.
 * @retval MFS_ERR_INTERNAL if an internal logic failure is detected.
 *
 * @api
 */
mfs_error_t mfs_mount(MFSDriver *mfsp) {
  unsigned i;

  /* Resetting previous state.*/
  mfs_state_reset(mfsp);

  /* Attempting to mount the managed partition.*/
  for (i = 0; i < MFS_CFG_MAX_REPAIR_ATTEMPTS; i++) {
    mfs_error_t err;

    err = mfs_try_mount(mfsp);
    if (err == MFS_ERR_INTERNAL) {
      /* Special case, do not retry on internal errors but report
         immediately.*/
      mfsp->state = MFS_ERROR;
      return err;
    }
    if (!MFS_IS_ERROR(err)) {
      mfsp->state  = MFS_READY;
      return err;
    }
  }

  /* Driver start failed.*/
  mfsp->state = MFS_ERROR;
  return MFS_ERR_FLASH_FAILURE;
}

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Initializes an instance.
 *
 * @param[out] mfsp     pointer to the @p MFSDriver object
 *
 * @init
 */
void mfsObjectInit(MFSDriver *mfsp) {

  osalDbgCheck(mfsp != NULL);

  mfsp->state = MFS_STOP;
  mfsp->config = NULL;
}

/**
 * @brief   Configures and activates a MFS driver.
 *
 * @param[in] mfsp      pointer to the @p MFSDriver object
 * @param[in] config    pointer to the configuration
 * @return              The operation status.
 * @retval MFS_NO_ERROR if the operation has been successfully completed.
 * @retval MFS_WARN_GC  if the operation triggered a garbage collection.
 * @retval MFS_ERR_FLASH_FAILURE if the flash memory is unusable because HW
 *                      failures. Makes the driver enter the @p MFS_ERROR state.
 * @retval MFS_ERR_INTERNAL if an internal logic failure is detected.
 *
 * @api
 */
mfs_error_t mfsStart(MFSDriver *mfsp, const MFSConfig *config) {

  osalDbgCheck((mfsp != NULL) && (config != NULL));
  osalDbgAssert((mfsp->state == MFS_STOP) || (mfsp->state == MFS_READY) ||
                (mfsp->state == MFS_ERROR), "invalid state");

  /* Storing configuration.*/
  mfsp->config = config;

  return mfs_mount(mfsp);
} 

/**
 * @brief   Deactivates a MFS driver.
 *
 * @param[in] mfsp      pointer to the @p MFSDriver object
 *
 * @api
 */
void mfsStop(MFSDriver *mfsp) {

  osalDbgCheck(mfsp != NULL);
  osalDbgAssert((mfsp->state == MFS_STOP) || (mfsp->state == MFS_READY) ||
                (mfsp->state == MFS_ERROR), "invalid state");

  mfsp->config = NULL;
  mfsp->state = MFS_STOP;
}

/**
 * @brief   Destroys the state of the managed storage by erasing the flash.
 *
 * @param[in] mfsp      pointer to the @p MFSDriver object
 * @return              The operation status.
 * @retval MFS_ERR_INV_STATE if the driver is in not in @p MSG_READY state.
 * @retval MFS_NO_ERROR if the operation has been successfully completed.
 * @retval MFS_ERR_FLASH_FAILURE if the flash memory is unusable because HW
 *                      failures. Makes the driver enter the @p MFS_ERROR state.
 * @retval MFS_ERR_INTERNAL if an internal logic failure is detected.
 *
 * @api
 */
mfs_error_t mfsErase(MFSDriver *mfsp) {

  osalDbgCheck(mfsp != NULL);

  if (mfsp->state != MFS_READY) {
    return MFS_ERR_INV_STATE;
  }

  RET_ON_ERROR(mfs_bank_erase(mfsp, MFS_BANK_0));
  RET_ON_ERROR(mfs_bank_erase(mfsp, MFS_BANK_1));

  return mfs_mount(mfsp);
}

/**
 * @brief   Retrieves and reads a data record.
 *
 * @param[in] mfsp      pointer to the @p MFSDriver object
 * @param[in] id        record numeric identifier, the valid range is between
 *                      @p 1 and @p MFS_CFG_MAX_RECORDS
 * @param[in,out] np    on input is the maximum buffer size, on return it is
 *                      the size of the data copied into the buffer
 * @param[out] buffer   pointer to a buffer for record data
 * @return              The operation status.
 * @retval MFS_NO_ERROR if the operation has been successfully completed.
 * @retval MFS_ERR_INV_STATE if the driver is in not in @p MSG_READY state.
 * @retval MFS_ERR_INV_SIZE if the passed buffer is not large enough to
 *                      contain the record data.
 * @retval MFS_ERR_NOT_FOUND if the specified id does not exists.
 * @retval MFS_ERR_FLASH_FAILURE if the flash memory is unusable because HW
 *                      failures. Makes the driver enter the @p MFS_ERROR state.
 * @retval MFS_ERR_INTERNAL if an internal logic failure is detected.
 *
 * @api
 */
mfs_error_t mfsReadRecord(MFSDriver *mfsp, mfs_id_t id,
                          size_t *np, uint8_t *buffer) {
  uint16_t crc;

  osalDbgCheck((mfsp != NULL) &&
               (id >= 1) && (id <= (mfs_id_t)MFS_CFG_MAX_RECORDS) &&
               (np != NULL) && (buffer != NULL));

  if (mfsp->state != MFS_READY) {
    return MFS_ERR_INV_STATE;
  }

  /* Checking if the requested record actually exists.*/
  if (mfsp->descriptors[id - 1U].offset == 0U) {
    return MFS_ERR_NOT_FOUND;
  }

  /* Making sure to not overflow the buffer.*/
  if (*np < mfsp->descriptors[id - 1U].size) {
    return MFS_ERR_INV_SIZE;
  }

  /* Header read from flash.*/
  RET_ON_ERROR(mfs_flash_read(mfsp,
                              mfsp->descriptors[id - 1U].offset,
                              sizeof (mfs_data_header_t),
                              mfsp->buffer.data8));

  /* Data read from flash.*/
  *np = mfsp->descriptors[id - 1U].size;
  RET_ON_ERROR(mfs_flash_read(mfsp,
                              mfsp->descriptors[id - 1U].offset + sizeof (mfs_data_header_t),
                              *np,
                              buffer));

  /* Checking CRC.*/
  crc = crc16(0xFFFFU, buffer, *np);
  if (crc != mfsp->buffer.dhdr.fields.crc) {
    mfsp->state = MFS_ERROR;
    return MFS_ERR_FLASH_FAILURE;
  }

  return MFS_NO_ERROR;
}

/**
 * @brief   Creates or updates a data record.
 *
 * @param[in] mfsp      pointer to the @p MFSDriver object
 * @param[in] id        record numeric identifier, the valid range is between
 *                      @p 1 and @p MFS_CFG_MAX_RECORDS
 * @param[in] n         size of data to be written, it cannot be zero
 * @param[in] buffer    pointer to a buffer for record data
 * @return              The operation status.
 * @retval MFS_NO_ERROR if the operation has been successfully completed.
 * @retval MFS_WARN_GC  if the operation triggered a garbage collection.
 * @retval MFS_ERR_INV_STATE if the driver is in not in @p MSG_READY state.
 * @retval MFS_ERR_OUT_OF_MEM if there is not enough flash space for the
 *                      operation.
 * @retval MFS_ERR_FLASH_FAILURE if the flash memory is unusable because HW
 *                      failures. Makes the driver enter the @p MFS_ERROR state.
 * @retval MFS_ERR_INTERNAL if an internal logic failure is detected.
 *
 * @api
 */
mfs_error_t mfsWriteRecord(MFSDriver *mfsp, mfs_id_t id,
                           size_t n, const uint8_t *buffer) {
  flash_offset_t free, required;
  bool warning = false;

  osalDbgCheck((mfsp != NULL) &&
               (id >= 1) && (id <= (mfs_id_t)MFS_CFG_MAX_RECORDS) &&
               (n > 0U) && (buffer != NULL));

  if (mfsp->state != MFS_READY) {
    return MFS_ERR_INV_STATE;
  }

  /* If the required space is beyond the available (compacted) block
     size then an error is returned.
     NOTE: The space for one extra header is reserved in order to allow
     for an erase operation after the space has been fully allocated.*/
  required = ((flash_offset_t)sizeof (mfs_data_header_t) * 2U) +
             (flash_offset_t)n;
  if (required > mfsp->config->bank_size - mfsp->used_space) {
    return MFS_ERR_OUT_OF_MEM;
  }

  /* Checking for immediately (not compacted) available space.*/
  free = (mfs_flash_get_bank_offset(mfsp, mfsp->current_bank) +
          mfsp->config->bank_size) - mfsp->next_offset;
  if (required > free) {
    /* We need to perform a garbage collection, there is enough space
       but it has to be freed.*/
    warning = true;
    RET_ON_ERROR(mfs_garbage_collect(mfsp));
  }

  /* Writing the data header without the magic, it will be written last.*/
  mfsp->buffer.dhdr.fields.magic = (uint32_t)mfsp->config->erased;
  mfsp->buffer.dhdr.fields.id    = (uint16_t)id;
  mfsp->buffer.dhdr.fields.size  = (uint32_t)n;
  mfsp->buffer.dhdr.fields.crc   = crc16(0xFFFFU, buffer, n);
  RET_ON_ERROR(mfs_flash_write(mfsp,
                               mfsp->next_offset,
                               sizeof (mfs_data_header_t),
                               mfsp->buffer.data8));

  /* Writing the data part.*/
  RET_ON_ERROR(mfs_flash_write(mfsp,
                               mfsp->next_offset + sizeof (mfs_data_header_t),
                               n,
                               buffer));

  /* Finally writing the magic number, it seals the transaction.*/
  mfsp->buffer.dhdr.fields.magic = (uint32_t)MFS_HEADER_MAGIC;
  RET_ON_ERROR(mfs_flash_write(mfsp,
                               mfsp->next_offset,
                               sizeof (uint32_t),
                               mfsp->buffer.data8));

  /* The size of the old record instance, if present, must be subtracted
     to the total used size.*/
  if (mfsp->descriptors[id - 1U].offset != 0U) {
    mfsp->used_space -= sizeof (mfs_data_header_t) +
                        mfsp->descriptors[id - 1U].size;
  }

  /* Adjusting bank-related metadata.*/
  mfsp->descriptors[id - 1U].offset = mfsp->next_offset;
  mfsp->descriptors[id - 1U].size   = (uint32_t)n;
  mfsp->next_offset += sizeof (mfs_data_header_t) + n;
  mfsp->used_space  += sizeof (mfs_data_header_t) + n;

  return warning ? MFS_WARN_GC : MFS_NO_ERROR;
}

/**
 * @brief   Erases a data record.
 *
 * @param[in] mfsp      pointer to the @p MFSDriver object
 * @param[in] id        record numeric identifier, the valid range is between
 *                      @p 1 and @p MFS_CFG_MAX_RECORDS
 * @return              The operation status.
 * @retval MFS_NO_ERROR if the operation has been successfully completed.
 * @retval MFS_WARN_GC  if the operation triggered a garbage collection.
 * @retval MFS_ERR_INV_STATE if the driver is in not in @p MSG_READY state.
 * @retval MFS_ERR_FLASH_FAILURE if the flash memory is unusable because HW
 *                      failures. Makes the driver enter the @p MFS_ERROR state.
 * @retval MFS_ERR_INTERNAL if an internal logic failure is detected.
 *
 * @api
 */
mfs_error_t mfsEraseRecord(MFSDriver *mfsp, mfs_id_t id) {
  flash_offset_t free, required;
  bool warning = false;

  osalDbgCheck((mfsp != NULL) &&
               (id >= 1U) && (id <= (mfs_id_t)MFS_CFG_MAX_RECORDS));

  if (mfsp->state != MFS_READY) {
    return MFS_ERR_INV_STATE;
  }

  /* Checking if the requested record actually exists.*/
  if (mfsp->descriptors[id - 1U].offset == 0U) {
    return MFS_ERR_NOT_FOUND;
  }

  /* If the required space is beyond the available (compacted) block
     size then an internal error is returned, it should never happen.*/
  required = (flash_offset_t)sizeof (mfs_data_header_t);
  if (required > mfsp->config->bank_size - mfsp->used_space) {
    return MFS_ERR_INTERNAL;
  }

  /* Checking for immediately (not compacted) available space.*/
  free = (mfs_flash_get_bank_offset(mfsp, mfsp->current_bank) +
          mfsp->config->bank_size) - mfsp->next_offset;
  if (required > free) {
    /* We need to perform a garbage collection, there is enough space
       but it has to be freed.*/
    warning = true;
    RET_ON_ERROR(mfs_garbage_collect(mfsp));
  }

  /* Writing the data header with size set to zero, it means that the
     record is logically erased.*/
  mfsp->buffer.dhdr.fields.magic = (uint32_t)MFS_HEADER_MAGIC;
  mfsp->buffer.dhdr.fields.id    = (uint16_t)id;
  mfsp->buffer.dhdr.fields.size  = (uint32_t)0;
  mfsp->buffer.dhdr.fields.crc   = (uint16_t)0;
  RET_ON_ERROR(mfs_flash_write(mfsp,
                               mfsp->next_offset,
                               sizeof (mfs_data_header_t),
                               mfsp->buffer.data8));

  /* Adjusting bank-related metadata.*/
  mfsp->used_space  -= sizeof (mfs_data_header_t) +
                       mfsp->descriptors[id - 1U].size;
  mfsp->next_offset += sizeof (mfs_data_header_t);
  mfsp->descriptors[id - 1U].offset = 0U;
  mfsp->descriptors[id - 1U].size   = 0U;

  return warning ? MFS_WARN_GC : MFS_NO_ERROR;
}

/**
 * @brief   Enforces a garbage collection operation.
 * @details Garbage collection involves: integrity check, optionally repairs,
 *          obsolete data removal, data compaction and a flash bank swap.
 *
 * @param[in] mfsp      pointer to the @p MFSDriver object
 * @return              The operation status.
 * @retval MFS_NO_ERROR if the operation has been successfully completed.
 * @retval MFS_ERR_INV_STATE if the driver is in not in @p MSG_READY state.
 * @retval MFS_ERR_FLASH_FAILURE if the flash memory is unusable because HW
 *                      failures. Makes the driver enter the @p MFS_ERROR state.
 * @retval MFS_ERR_INTERNAL if an internal logic failure is detected.
 *
 * @api
 */
mfs_error_t mfsPerformGarbageCollection(MFSDriver *mfsp) {

  osalDbgCheck(mfsp != NULL);

  if (mfsp->state != MFS_READY) {
    return MFS_ERR_INV_STATE;
  }

  return mfs_garbage_collect(mfsp);
}

/** @} */
