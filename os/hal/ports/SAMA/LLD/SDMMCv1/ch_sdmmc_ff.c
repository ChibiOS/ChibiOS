/* ----------------------------------------------------------------------------
 *         SAM Software Package License
 * ----------------------------------------------------------------------------
 * Copyright (c) 2015, Atmel Corporation
 *
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * - Redistributions of source code must retain the above copyright notice,
 * this list of conditions and the disclaimer below.
 *
 * Atmel's name may not be used to endorse or promote products derived from
 * this software without specific prior written permission.
 *
 * DISCLAIMER: THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA,
 * OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
 * LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
 * NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * ----------------------------------------------------------------------------
 */

/* ----------------------------------------------------------------------------
 * This file is based on the template source file named diskio.c,
 * part of the FatFs Module R0.10b:
 *   Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014
 *   If a working storage control module is available, it should be
 *   attached to the FatFs via a glue function rather than modifying it.
 *   This is an example of glue functions to attach various existing
 *   storage control modules to the FatFs module with a defined API.
 * ----------------------------------------------------------------------------
 */

#include "hal.h"

#if (HAL_USE_SDMMC == TRUE)
#include "sama_sdmmc_lld.h"
#if SDMMC_USE_FF_LIB == 1
#include "ccportab.h"
#include "ffconf.h"
#include "diskio.h"

#include <string.h>
#include "sama_sdmmc_lld.h"
#include "ch_sdmmc_device.h"
#include "ch_sdmmc_sd.h"
#include "ch_sdmmc_sdio.h"
#include "ch_sdmmc_trace.h"
/*----------------------------------------------------------------------------
 *        Definitions
 *----------------------------------------------------------------------------*/

/**
 *  \brief Access the SD/MMC Library instances owned by the application.
 *  Used upon calls from the FatFs Module.
 *
 *  Shall be implemented by the application.
 */
extern  bool CC_WEAK sdmmcGetInstance(uint8_t index, SdmmcDriver **sdmmcp)  ;

/*----------------------------------------------------------------------------
 *        Exported functions
 *----------------------------------------------------------------------------*/

/**
 * \brief Initialize a Drive.
 * \param slot  Physical drive number (0..).
 * \return Drive status flags; STA_NOINIT if the specified drive does not exist.
 */
DSTATUS disk_initialize(BYTE slot)
{
	SdmmcDriver *sdmmcp = NULL;
	uint8_t rc;

	if (!sdmmcGetInstance(slot, &sdmmcp))
		return STA_NOINIT;


	rc = SD_GetStatus(sdmmcp);

	if (rc == SDMMC_NOT_SUPPORTED)
		return STA_NODISK | STA_NOINIT;
#if 0
	if (sdmmcp->state != MCID_IDLE )
		sdmmc_device_deInit(sdmmcp);

	/* FIXME a delay with the bus held off may be required by the device */
	rc = sdmmc_device_start(sdmmcp);

	if (rc == SDMMC_OK) {
		rc = sdmmc_device_identify(sdmmcp);
	}
#endif

	return rc == SDMMC_OK ? 0 : STA_NOINIT;
}

/**
 * \brief Get Drive Status.
 * \param slot  Physical drive number (0..).
 * \return Drive status flags; STA_NODISK if there is currently no device in
 * the specified slot.
 */
DSTATUS disk_status(BYTE slot)
{
	SdmmcDriver *sdmmcp = NULL;
	uint8_t rc;

	if (!sdmmcGetInstance(slot, &sdmmcp))
		return STA_NODISK | STA_NOINIT;

	rc = SD_GetStatus(sdmmcp);

	if (rc == SDMMC_NOT_SUPPORTED)
		return STA_NODISK | STA_NOINIT;
	else if (rc != SDMMC_OK)
		return STA_NOINIT;
	/* Well, no restriction on this drive */
	return 0;
}

/**
 * \brief Read Sector(s).
 * \param slot  Physical drive number (0..).
 * \param buff  Data buffer to store read data.
 * \param sector  Sector address in LBA.
 * \param count  Number of sectors to read.
 * \return Result code; RES_OK if successful.
 */
DRESULT disk_read(BYTE slot, BYTE* buff, DWORD sector, UINT count)
{
	SdmmcDriver *sdmmcp = NULL;
	DRESULT res;
	uint32_t blk_size, addr = sector, len = count;
	uint8_t rc;

	if (!sdmmcGetInstance(slot, &sdmmcp))
		return RES_PARERR;

	blk_size = sdmmcp->card.wBlockSize;

	if (blk_size == 0)
		return RES_NOTRDY;

	if (blk_size < FF_MIN_SS) {
		if (FF_MIN_SS % blk_size)
			return RES_PARERR;
		addr = sector * (FF_MIN_SS / blk_size);
		len  = count * (FF_MIN_SS / blk_size);
	}

	if (count <= 1)
		rc = SD_ReadBlocks(sdmmcp, addr, buff, len);
	else
		rc = SD_Read(sdmmcp, addr, buff, len);

	if (rc == SDMMC_OK || rc == SDMMC_CHANGED)
		res = RES_OK;
	else if (rc == SDMMC_ERR_IO || rc == SDMMC_ERR_RESP || rc == SDMMC_ERR)
		res = RES_ERROR;
	else if (rc == SDMMC_NO_RESPONSE || rc == SDMMC_BUSY
	    || rc == SDMMC_NOT_INITIALIZED || rc == SDMMC_LOCKED
	    || rc == SDMMC_STATE || rc == SDMMC_USER_CANCEL)
		res = RES_NOTRDY;
	else if (rc == SDMMC_PARAM || rc == SDMMC_NOT_SUPPORTED)
		res = RES_PARERR;
	else
		res = RES_ERROR;

	return res;
}

#if !FF_FS_READONLY
/**
 * \brief Write Sector(s).
 *
 * \param slot  Physical drive number (0..).
 * \param buff  Data to be written.
 * \param sector  Sector address in LBA.
 * \param count  Number of sectors to write.
 * \return Result code; RES_OK if successful.
 *
 * \note The FatFs module will issue multiple sector transfer request
 * (count > 1) to the disk I/O layer. The disk function should process
 * the multiple sector transfer properly. Do not translate it into
 * multiple single sector transfers to the media, or the data read/write
 * performance may be drastically decreased.
 */
DRESULT disk_write(BYTE slot, const BYTE* buff, DWORD sector, UINT count)
{
	SdmmcDriver *sdmmcp = NULL;
	DRESULT res;
	uint32_t blk_size, addr = sector, len = count;
	uint8_t rc;

	if (!sdmmcGetInstance(slot, &sdmmcp))
		return RES_PARERR;

	blk_size = sdmmcp->card.wBlockSize;

	if (blk_size < FF_MIN_SS) {
		if (FF_MIN_SS % blk_size)
			return RES_PARERR;
		addr = sector * (FF_MIN_SS / blk_size);
		len  = count * (FF_MIN_SS / blk_size);
	}
	if (count <= 1)
		rc = SD_WriteBlocks(sdmmcp, addr, buff, len);
	else
		rc = SD_Write(sdmmcp, addr, buff, len);

	if (rc == SDMMC_OK || rc == SDMMC_CHANGED)
		res = RES_OK;
	else if (rc == SDMMC_ERR_IO || rc == SDMMC_ERR_RESP || rc == SDMMC_ERR)
		res = RES_ERROR;
	else if (rc == SDMMC_NO_RESPONSE || rc == SDMMC_BUSY
	    || rc == SDMMC_NOT_INITIALIZED || rc == SDMMC_LOCKED
	    || rc == SDMMC_STATE || rc == SDMMC_USER_CANCEL)
		res = RES_NOTRDY;
	else if (rc == SDMMC_PARAM || rc == SDMMC_NOT_SUPPORTED)
		res = RES_PARERR;
	else
		res = RES_ERROR;
	return res;
}
#endif /* _FS_READONLY */

/**
 * \brief Miscellaneous Functions.
 * \param slot  Physical drive number (0..).
 * \param cmd  Control code.
 * \param buff  Buffer to send/receive control data.
 * \return Result code; RES_OK if successful.
 */
DRESULT disk_ioctl(BYTE slot, BYTE cmd, void* buff)
{
	SdmmcDriver *sdmmcp = NULL;
	DRESULT res;
	DWORD *param_u32 = (DWORD *)buff;
	WORD *param_u16 = (WORD *)buff;
	uint32_t blk_size, blk_count;

	if (!sdmmcGetInstance(slot, &sdmmcp))
		return RES_PARERR;

	switch (cmd)
	{
	case CTRL_SYNC:
		/* SD/MMC devices do not seem to cache data beyond completion
		 * of the write commands. Note that if _FS_READONLY is enabled,
		 * this command is not needed. */
		res = RES_OK;
		break;

	case GET_SECTOR_COUNT:
		if (!buff)
			return RES_PARERR;
		blk_size = sdmmcp->card.wBlockSize;
		blk_count = sdmmcp->card.dwNbBlocks;

		if (blk_size < FF_MIN_SS)
		{
			if (FF_MIN_SS % blk_size)
				return RES_PARERR;
			*param_u32 = blk_count / (FF_MIN_SS / blk_size);
		}
		else
			*param_u32 = blk_count;
		res = RES_OK;
		break;

	case GET_SECTOR_SIZE:
		/* Note that if _MAX_SS equals _MIN_SS i.e. the drive does not
		 * have to support several sector sizes, this command is not
		 * needed. */
		if (!buff)
			return RES_PARERR;
		blk_size = sdmmcp->card.wBlockSize;
		*param_u16 = blk_size >= FF_MIN_SS ? blk_size : FF_MIN_SS;
		res = RES_OK;
		break;

	case GET_BLOCK_SIZE:
		if (!buff)
			return RES_PARERR;
		/* On SD/MMC devices, erase block size is the same as write
		 * block size.
		 * Hence, erasing as little as one sector is allowed. */
		*param_u32 = 1;
		res = RES_OK;
		break;

	case CTRL_TRIM:
		/* This TRIM-like command is not implemented.
		 * It would be required if _USE_TRIM was enabled. */
		res = RES_PARERR;
		break;

	default:
		res = RES_PARERR;
		break;
	}
	return res;
}
#endif
#endif

