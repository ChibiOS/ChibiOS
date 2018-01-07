#include <string.h>
#include "hal.h"
#include "sama_sdmmc_lld.h"
#include "ch_sdmmc_device.h"
#include "ch_sdmmc_cmds.h"
#include "ch_sdmmc_sd.h"
#include "ch_sdmmc_sdio.h"


static uint8_t SdGetTimingFunction(uint8_t mode);
static void SdSelectSlowerTiming(bool high_sig, uint8_t * mode);

#if SAMA_SDMMC_TRACE == 1
struct stringEntry_s
{
	const uint8_t key;
	const char *name;
};
const char sdmmcInvalidCode[] = "!Invalid!";
const struct stringEntry_s sdmmcRCodeNames[] = {
	{ SDMMC_OK,		"OK",			},
	{ SDMMC_LOCKED,		"ERR_LOCKED",		},
	{ SDMMC_BUSY,		"ERR_BUSY",		},
	{ SDMMC_NO_RESPONSE,	"ERR_NO_RESPONSE",	},
	{ SDMMC_CHANGED,		"OK_CHANGED",		},
	{ SDMMC_ERR,			"ERROR",		},
	{ SDMMC_ERR_IO,			"ERR_IO",		},
	{ SDMMC_ERR_RESP,		"ERR_RESP",		},
	{ SDMMC_NOT_INITIALIZED,	"ERR_NOT_INITIALIZED",	},
	{ SDMMC_PARAM,		"ERR_PARAM",		},
	{ SDMMC_STATE,		"ERR_STATE",		},
	{ SDMMC_USER_CANCEL,	"ERR_USER_CANCEL",	},
	{ SDMMC_NOT_SUPPORTED,	"ERR_NO_SUPPORT",	},
};

const struct stringEntry_s sdmmcIOCtrlNames[] = {
	{ SDMMC_IOCTL_BUSY_CHECK,	"BUSY_CHECK",		},
	{ SDMMC_IOCTL_POWER,		"POWER",		},
	{ SDMMC_IOCTL_CANCEL_CMD,	"CANCEL_CMD",		},
	{ SDMMC_IOCTL_RESET,		"RESET",		},
	{ SDMMC_IOCTL_SET_CLOCK,	"SET_CLOCK",		},
	{ SDMMC_IOCTL_SET_BUSMODE,	"SET_BUSMODE",		},
	{ SDMMC_IOCTL_SET_HSMODE,	"SET_HSMODE",		},
	{ SDMMC_IOCTL_SET_BOOTMODE,	"SET_BOOTMODE",		},
	{ SDMMC_IOCTL_SET_LENPREFIX,	"SET_LENPREFIX",	},
	{ SDMMC_IOCTL_GET_CLOCK,	"GET_CLOCK",		},
	{ SDMMC_IOCTL_GET_BUSMODE,	"GET_BUSMODE",		},
	{ SDMMC_IOCTL_GET_HSMODE,	"GET_HSMODE",		},
	{ SDMMC_IOCTL_GET_BOOTMODE,	"GET_BOOTMODE",		},
	{ SDMMC_IOCTL_GET_XFERCOMPL,	"GET_XFERCOMPL",	},
	{ SDMMC_IOCTL_GET_DEVICE,	"GET_DEVICE",		},
};

const char * SD_StringifyRetCode(uint32_t dwRCode)
{
	const uint8_t bound = ARRAY_SIZE(sdmmcRCodeNames);
	uint8_t ix;

	for (ix = 0; ix < bound; ix++) {
		if (dwRCode == (uint32_t)sdmmcRCodeNames[ix].key)
			return sdmmcRCodeNames[ix].name;
	}

	return sdmmcInvalidCode;
}

const char * SD_StringifyIOCtrl(uint32_t dwCtrl)
{
	const uint8_t bound = ARRAY_SIZE(sdmmcIOCtrlNames);
	uint8_t ix;

	for (ix = 0; ix < bound; ix++) {
		if (dwCtrl == (uint32_t)sdmmcIOCtrlNames[ix].key)
			return sdmmcIOCtrlNames[ix].name;
	}

	return sdmmcInvalidCode;
}
#endif

uint8_t SdDecideBuswidth(SdmmcDriver *drv)
{
	uint8_t error, busWidth = 1;
	const uint8_t sd = (drv->card.bCardType & CARD_TYPE_bmSDMMC)  == CARD_TYPE_bmSD;
	const uint8_t io = (drv->card.bCardType & CARD_TYPE_bmSDIO) != 0;

	if (io)
		busWidth = 1;   /* SDIO => 1 bit only. TODO: assign CCCR. */
	else if (sd) {
		busWidth = 4;   /* default to 4-bit mode */
		error = HwSetBusWidth(drv, busWidth);
		if (error)
			busWidth = 1;
	}
	/* Switch to selected bus mode */
	if (sd && busWidth > 1)
		error = Acmd6(drv, busWidth);
	else
		error = HwSetBusWidth(drv, busWidth);
	if (error)
		return error;
	drv->card.bBusMode = busWidth;
	return 0;
}


uint8_t SdEnableHighSpeed(SdmmcDriver *drv)
{
	SdCmd6Arg request = {
		.acc_mode	= 0xf,
		.cmd_sys	= 0xf,
		.drv_strgth	= 0xf,
		.pwr_limit	= 0xf,
		.func_grp5	= 0xf,
		.func_grp6	= 0xf,
		.set		= 0,
	};
	uint32_t status;
	uint16_t mode_mask, val;
	uint8_t mode = drv->card.bCardSigLevel ? SDMMC_TIM_SD_DS : SDMMC_TIM_SD_SDR12;
	uint8_t error, mode_func, pwr_func = SD_SWITCH_ST_MAX_PWR_1_44W;
	const bool has_io = drv->card.bCardType & CARD_TYPE_bmSDIO ? true : false;
	const bool has_mem = drv->card.bCardType & CARD_TYPE_bmSD ? true : false;
	const bool has_switch = SD_CSD_CCC(drv->card.CSD) & 1 << 10 ? true : false;
	bool sfs_v1 = false;

	//assert(sizeof(pSd->sandbox1) >= 512 / 8);

#ifndef SDMMC_TRIM_SDIO
	/* TODO consider the UHS-I timing modes for SDIO devices too */
	if (has_io && !(has_mem && !has_switch)
	    && HwIsTimingSupported(drv, SDMMC_TIM_SD_HS)) {
		/* Check CIA.HS */
		status = 0;
		error = Cmd52(drv, 0, SDIO_CIA, 0, SDIO_HS_REG, &status);
		if (error)
			return SDMMC_ERR;
		if (status & SDIO_SHS) {
			/* Enable High Speed timing mode */
			status = SDIO_EHS;
			error = Cmd52(drv, 1, SDIO_CIA, 1, SDIO_HS_REG,
			    &status);
			if (error || !(status & SDIO_EHS))
				return SDMMC_ERR;
			mode = SDMMC_TIM_SD_HS;
		}
	}
#endif

	if (!has_mem || !has_switch)
		goto Apply;
	/* Search for the fastest supported timing mode */
	error = SdCmd6(drv, &request, drv->card.sandbox1, &status);
	if (error || status & STATUS_SWITCH_ERROR)
		return SDMMC_ERR;
	sfs_v1 = SD_SWITCH_ST_DATA_STRUCT_VER(drv->card.sandbox1) >= 0x01;
	mode_mask = SD_SWITCH_ST_FUN_GRP1_INFO(drv->card.sandbox1);
	TRACE_1("Device timing functions: 0x%04x\n\r", mode_mask);
	if (has_io && mode == SDMMC_TIM_SD_HS
	    && !(mode_mask & 1 << SD_SWITCH_ST_ACC_HS))
		return SDMMC_NOT_SUPPORTED;
	else if (has_io) {
		/* Have SDMEM use the same timing mode as SDIO */
	} else if (mode_mask & 1 << SD_SWITCH_ST_ACC_SDR104
	    && HwIsTimingSupported(drv, SDMMC_TIM_SD_SDR104))
		mode = SDMMC_TIM_SD_SDR104;
	else if (mode_mask & 1 << SD_SWITCH_ST_ACC_DDR50
	    && HwIsTimingSupported(drv, SDMMC_TIM_SD_DDR50))
		mode = SDMMC_TIM_SD_DDR50;
	else if (mode_mask & 1 << SD_SWITCH_ST_ACC_SDR50
	    && HwIsTimingSupported(drv, SDMMC_TIM_SD_SDR50))
		mode = SDMMC_TIM_SD_SDR50;
	else if (mode_mask & 1 << SD_SWITCH_ST_ACC_HS
	    && HwIsTimingSupported(drv, SDMMC_TIM_SD_HS))
		mode = SDMMC_TIM_SD_HS;
	else
		mode = SDMMC_TIM_SD_DS;
	/* Verify current signaling level is the one expected by the device */
	if ((mode >= SDMMC_TIM_SD_SDR50 && drv->card.bCardSigLevel != 0)
	    || (mode < SDMMC_TIM_SD_SDR50 && drv->card.bCardSigLevel == 0))
		return SDMMC_STATE;
	/* Check the electrical power requirements of this device */
	val = SD_SWITCH_ST_FUN_GRP4_INFO(drv->card.sandbox1);
	TRACE_2("Device pwr & strength functions: 0x%04x & 0x%04x\n\r", val,
	    SD_SWITCH_ST_FUN_GRP3_INFO(drv->card.sandbox1));
	if (!(val & 1 << SD_SWITCH_ST_MAX_PWR_1_44W))
		pwr_func = SD_SWITCH_ST_MAX_PWR_0_72W;
	request.acc_mode = mode_func = SdGetTimingFunction(mode);
	request.drv_strgth = SD_SWITCH_ST_OUT_DRV_B;
	request.pwr_limit = SD_SWITCH_ST_MAX_PWR_0_72W;
	error = SdCmd6(drv, &request, drv->card.sandbox1, &status);
	if (error || status & STATUS_SWITCH_ERROR)
		return SDMMC_ERR;
	val = SD_SWITCH_ST_MAX_CURR_CONSUMPTION(drv->card.sandbox1);
	TRACE_1("Device max current: %u mA\n\r", val);
	if (val == 0 || val > (1440 * 10) / 36)
		SdSelectSlowerTiming(drv->card.bCardSigLevel != 0, &mode);
	else if (sfs_v1) {
		val = SD_SWITCH_ST_FUN_GRP4_BUSY(drv->card.sandbox1);
		if (val & 1 << SD_SWITCH_ST_MAX_PWR_1_44W)
			pwr_func = SD_SWITCH_ST_MAX_PWR_0_72W;
		val = SD_SWITCH_ST_FUN_GRP1_BUSY(drv->card.sandbox1);
		if (val & 1 << mode_func)
			SdSelectSlowerTiming(drv->card.bCardSigLevel != 0, &mode);
	}

	/* Select device output Driver Type B, i.e. 50 ohm nominal output
	 * impedance.
	 * FIXME select the optimal device output Driver Type, which depends on
	 * board design. An oscilloscope should be used to observe signal
	 * integrity, then among the driver types that meet rise and fall time
	 * requirements, the weakest should be selected.
	 */
	request.acc_mode = 0xf;
	request.pwr_limit = 0xf;
	request.set = 1;
	error = SdCmd6(drv, &request, drv->card.sandbox1, &status);
	if (error || status & STATUS_SWITCH_ERROR)
		return SDMMC_ERR;
	val = SD_SWITCH_ST_FUN_GRP3_RC(drv->card.sandbox1);
	if (val != request.drv_strgth)
		SdSelectSlowerTiming(drv->card.bCardSigLevel != 0, &mode);

Switch:
	/* Now switch the memory device to the candidating mode */
	request.acc_mode = mode_func = SdGetTimingFunction(mode);
	request.cmd_sys = 0x0;
	request.drv_strgth = 0xf;
	request.pwr_limit = pwr_func;
	error = SdCmd6(drv, &request, drv->card.sandbox1, &status);
	if (error || status & STATUS_SWITCH_ERROR)
		return SDMMC_ERR;
	val = SD_SWITCH_ST_FUN_GRP1_RC(drv->card.sandbox1);
	while (val != mode_func && val != SD_SWITCH_ST_FUN_GRP_RC_ERROR) {
		/* FIXME break upon timeout condition */
		request.acc_mode = 0xf;
		request.cmd_sys = 0xf;
		request.pwr_limit = 0xf;
		request.set = 0;
		error = SdCmd6(drv, &request, drv->card.sandbox1, &status);
		if (error || status & STATUS_SWITCH_ERROR)
			return SDMMC_ERR;
		val = SD_SWITCH_ST_FUN_GRP1_RC(drv->card.sandbox1);
		if (val != mode_func && sfs_v1
		    && !(SD_SWITCH_ST_FUN_GRP1_BUSY(drv->card.sandbox1)
		    & 1 << mode_func))
			break;
	}

	if (val != mode_func && (mode == SDMMC_TIM_SD_DS || mode == SDMMC_TIM_SD_SDR12))
		return SDMMC_ERR;
	else if (val != mode_func) {
		SdSelectSlowerTiming(drv->card.bCardSigLevel != 0, &mode);
		goto Switch;
	}

	val = SD_SWITCH_ST_FUN_GRP4_RC(drv->card.sandbox1);

	if (val != pwr_func) {
		TRACE_1("Device power limit 0x%x\n\r", val);
	}

Apply:
	error = HwSetHsMode(drv, mode);
	if (error == SDMMC_OK)
		drv->card.bSpeedMode = mode;
	else
		return SDMMC_ERR;
	return SDMMC_OK;
}


void SdGetExtInformation(SdmmcDriver *drv)
{
	uint32_t card_status;
	uint8_t error;

	error = Acmd51(drv, drv->card.SCR, &card_status);

	if (error == SDMMC_OK) {
		card_status &= ~STATUS_READY_FOR_DATA;
		if (card_status != (STATUS_APP_CMD | STATUS_TRAN)) {
			TRACE_1("SCR st %lx\n\r", card_status);
		}
	}

	error = Acmd13(drv, drv->card.SSR, &card_status);

	if (error == SDMMC_OK) {
		card_status &= ~STATUS_READY_FOR_DATA;
		if (card_status != (STATUS_APP_CMD | STATUS_TRAN)) {
			TRACE_1("SSR st %lx\n\r", card_status);
		}
	}
}



/**
 * Reset SD/MMC driver runtime parameters.
 */
void SdParamReset(sSdCard * pSd)
{
	pSd->dwTotalSize = 0;
	pSd->dwNbBlocks = 0;
	pSd->wBlockSize = 0;

	pSd->wCurrBlockLen = 0;
	pSd->dwCurrSpeed = 0;
	pSd->wAddress = 0;

	pSd->bCardType = 0;
	pSd->bCardSigLevel = 2;
	pSd->bSpeedMode = SDMMC_TIM_MMC_BC;
	pSd->bBusMode = 1;
	pSd->bStatus = SDMMC_NOT_INITIALIZED;
	pSd->bSetBlkCnt = 0;
	pSd->bStopMultXfer = 0;


	/* Clear our device register cache */
	memset(pSd->CID, 0, 16);
	memset(pSd->CSD, 0, 16);
	memset(pSd->EXT, 0, EXT_SIZE);
	memset(pSd->SSR, 0, SSR_SIZE);
	memset(pSd->SCR, 0, SCR_SIZE);
}



/**
 * From a wide-width device register extract the requested field.
 * \param reg  Contents of the register
 * \param reg_len  Length of the register, in bits
 * \param field_start  Offset (address of the least significant bit) of the
 * requested field, in bits
 * \param field_len  Length of the requested field, in bits
 * \return The value of the field.
 */
uint32_t SD_GetField(const uint8_t *reg, uint16_t reg_len, uint16_t field_start,
            uint8_t field_len)
{
	uint32_t val = 0;
	uint8_t byte, expected_bits = field_len, new_bits;

	//assert(reg);
	//assert(reg_len % 8 == 0);
	//assert(field_len != 0 && field_len <= 32 && field_len <= reg_len);
	//assert(field_start <= reg_len - field_len);

	reg += (reg_len - field_start - field_len) / 8;
	while (expected_bits) {
		byte = *reg;
		new_bits = (field_start + expected_bits) % 8;
		if (new_bits)
			byte &= (1 << new_bits) - 1;
		else
			new_bits = 8;
		if (new_bits <= expected_bits)
			val |= (uint32_t)byte << (expected_bits - new_bits);
		else {
			byte >>= new_bits - expected_bits;
			val |= byte;
			new_bits = expected_bits;
		}
		expected_bits -= new_bits;
		reg++;
	}
	//assert((val & ~0 << field_len) == 0);
	return val;
}

static uint8_t SdGetTimingFunction(uint8_t mode) {
	if (mode == SDMMC_TIM_SD_SDR104)
		return SD_SWITCH_ST_ACC_SDR104;
	else if (mode == SDMMC_TIM_SD_DDR50)
		return SD_SWITCH_ST_ACC_DDR50;
	else if (mode == SDMMC_TIM_SD_SDR50)
		return SD_SWITCH_ST_ACC_SDR50;
	else if (mode == SDMMC_TIM_SD_HS || mode == SDMMC_TIM_SD_SDR25)
		return SD_SWITCH_ST_ACC_HS;
	else
		return SD_SWITCH_ST_ACC_DS;
}

static void SdSelectSlowerTiming(bool high_sig, uint8_t * mode)
{
	if (high_sig)
		*mode = SDMMC_TIM_SD_DS;
	else if (*mode > SDMMC_TIM_SD_SDR50)
		*mode = SDMMC_TIM_SD_SDR50;
	else if (*mode > SDMMC_TIM_SD_SDR25)
		*mode = SDMMC_TIM_SD_SDR25;
	else
		*mode = SDMMC_TIM_SD_SDR12;
}

uint32_t SD_GetTotalSizeKB(const sSdCard * pSd)
{
	//assert(pSd != NULL);

	if (pSd->dwTotalSize == 0xFFFFFFFF)
		return (pSd->dwNbBlocks / 1024) * pSd->wBlockSize;
	else
		return pSd->dwTotalSize / 1024;
}


void SD_DumpStatus(const sSdCard *pSd)
{
	char text[40] = "";
	char mode[20] = "";
	char vers[7] = { ' ', 'v', '1', '.', '0', '\0', '\0' };

	//assert(pSd != NULL);

	if (pSd->bCardType & CARD_TYPE_bmHC)
		strcat(text, "High-capacity ");
	if (pSd->bCardType & CARD_TYPE_bmSDIO
	    && pSd->bCardType & CARD_TYPE_bmSD)
		strcat(text, "SDIO combo card");
	else if (pSd->bCardType & CARD_TYPE_bmSDIO)
		strcat(text, "SDIO device");
	else if (pSd->bCardType & CARD_TYPE_bmSD)
		strcat(text, "SD card");
#ifndef SDMMC_TRIM_MMC
	else if (pSd->bCardType & CARD_TYPE_bmMMC)
		strcat(text, "MMC device");
#endif
	else
		strcat(text, "unrecognized device");

	if (pSd->bCardType & CARD_TYPE_bmMMC) {
#ifndef SDMMC_TRIM_MMC
		const uint8_t csd = MMC_CSD_SPEC_VERS(pSd->CSD);
		const uint8_t ext = MMC_EXT_EXT_CSD_REV(pSd->EXT);

		if (csd == MMC_CSD_SPEC_VERS_1_4)
			vers[4] = '4';
		else if (csd == MMC_CSD_SPEC_VERS_2_0) {
			vers[2] = '2';
			vers[4] = 'x';
		}
		else if (csd == MMC_CSD_SPEC_VERS_3_1) {
			vers[2] = '3';
			vers[4] = 'x';
		}
		else if (csd == MMC_CSD_SPEC_VERS_4_0) {
			vers[2] = ext <= 6 ? '4' : '5';
			if (ext <= 4)
				vers[4] = '0' + ext;
			else if (ext == 5) {
				vers[4] = '4';
				vers[5] = '1';
			}
			else if (ext == 6) {
				vers[4] = '5';
				vers[5] = 'x';
			}
			else if (ext == 7)
				vers[5] = 'x';
			else if (ext == 8)
				vers[4] = '1';
			else
				vers[4] = 'x';
		}
		else if (csd != MMC_CSD_SPEC_VERS_1_0)
			vers[2] = vers[4] = '?';
		strcat(text, vers);
#endif
	}
	else if (pSd->bCardType & CARD_TYPE_bmSD
	    && SD_SCR_STRUCTURE(pSd->SCR) == SD_SCR_STRUCTURE_1_0) {
		if (SD_SCR_SD_SPEC(pSd->SCR) == SD_SCR_SD_SPEC_1_0)
			vers[5] = 'x';
		else if (SD_SCR_SD_SPEC(pSd->SCR) == SD_SCR_SD_SPEC_1_10) {
			vers[4] = '1';
			vers[5] = '0';
		}
		else if (SD_SCR_SD_SPEC(pSd->SCR) == SD_SCR_SD_SPEC_2_00) {
			if (SD_SCR_SD_SPEC4(pSd->SCR) == SD_SCR_SD_SPEC_4_X) {
				vers[2] = '4';
				vers[4] = vers[5] = 'x';
			}
			else if (SD_SCR_SD_SPEC3(pSd->SCR)
			    == SD_SCR_SD_SPEC_3_0) {
				vers[2] = '3';
				vers[5] = 'x';
			}
			else {
				vers[2] = '2';
				vers[5] = '0';
			}
		}
		else
			vers[2] = vers[4] = '?';
		strcat(text, vers);
	}

	if (pSd->bSpeedMode == SDMMC_TIM_MMC_BC)
		strcat(mode, "Backward-compatible");
#ifndef SDMMC_TRIM_MMC
	else if (pSd->bSpeedMode == SDMMC_TIM_MMC_HS_SDR)
		strcat(mode, "HS SDR");
	else if (pSd->bSpeedMode == SDMMC_TIM_MMC_HS_DDR)
		strcat(mode, "HS DDR");
	else if (pSd->bSpeedMode == SDMMC_TIM_MMC_HS200)
		strcat(mode, "HS200");
#endif
	else if (pSd->bSpeedMode == SDMMC_TIM_SD_DS)
		strcat(mode, "DS");
	else if (pSd->bSpeedMode == SDMMC_TIM_SD_HS)
		strcat(mode, "HS");
	else if (pSd->bSpeedMode >= SDMMC_TIM_SD_SDR12
	    && pSd->bSpeedMode <= SDMMC_TIM_SD_SDR104) {
		char uhs_mode[10] = "UHS-I SDR";

		if (pSd->bSpeedMode == SDMMC_TIM_SD_DDR50)
			uhs_mode[6] = 'D';
		strcat(mode, uhs_mode);
		if (pSd->bSpeedMode == SDMMC_TIM_SD_SDR12)
			strcat(mode, "12");
		else if (pSd->bSpeedMode == SDMMC_TIM_SD_SDR25)
			strcat(mode, "25");
		else if (pSd->bSpeedMode == SDMMC_TIM_SD_SDR50
		    || pSd->bSpeedMode == SDMMC_TIM_SD_DDR50)
			strcat(mode, "50");
		else
			strcat(mode, "104");
	}

	TRACE_4("%s, %u-bit data, in %s mode at %lu kHz\n\r", text, pSd->bBusMode, mode, (pSd->dwCurrSpeed / 1000UL) );

	if (pSd->bCardType & CARD_TYPE_bmSDMMC) {
		TRACE_3("Device memory size: %lu MiB, %lu * %uB\n\r", SD_GetTotalSizeKB(pSd) / 1024ul, pSd->dwNbBlocks,pSd->wBlockSize);

	}

}


/**
 * Display the content of the CID register
 * \param pSd  Pointer to SdCard instance.
 */
void SD_DumpCID(const sSdCard *pSd)
{
	const uint8_t sd_device = (pSd->bCardType & CARD_TYPE_bmSDMMC)  == CARD_TYPE_bmSD;

	/* Function-only SDIO devices have no CID register */
	if ((pSd->bCardType & CARD_TYPE_bmSDMMC) == CARD_TYPE_bmUNKNOWN)
		return;

	TRACE("Card IDentification\r\n");
	TRACE_1("MID 0x%02X\r\n", SD_CID_MID(pSd->CID));

	if (sd_device) {
		TRACE_2("OID %c%c\r\n", (char) SD_CID_OID1(pSd->CID),(char) SD_CID_OID0(pSd->CID));
		TRACE_5("PNM %c%c%c%c%c\r\n", (char) SD_CID_PNM4(pSd->CID),
		    (char) SD_CID_PNM3(pSd->CID), (char) SD_CID_PNM2(pSd->CID),
		    (char) SD_CID_PNM1(pSd->CID), (char) SD_CID_PNM0(pSd->CID));
		TRACE_2("PRV %u.%u\r\n", SD_CID_PRV1(pSd->CID),
		    SD_CID_PRV0(pSd->CID));
		TRACE_4("PSN 0x%02X%02X%02X%02X\r\n", SD_CID_PSN3(pSd->CID),
		    SD_CID_PSN2(pSd->CID), SD_CID_PSN1(pSd->CID),
		    SD_CID_PSN0(pSd->CID));
		TRACE_2("MDT %u/%02u\r\n", 2000 + SD_CID_MDT_Y(pSd->CID),
		    SD_CID_MDT_M(pSd->CID));
	}
#ifndef SDMMC_TRIM_MMC
	else {
		uint16_t year = 1997 + MMC_CID_MDT_Y(pSd->CID);

		if (MMC_EXT_EXT_CSD_REV(pSd->EXT) >= 3) {
			TRACE_1("CBX %u\r\n", eMMC_CID_CBX(pSd->CID));
			TRACE_1("OID 0x%02X\r\n", eMMC_CID_OID(pSd->CID));
		}
		else {
			TRACE_1("OID 0x%04X\r\n", MMC_CID_OID(pSd->CID));
		}
		TRACE_6("PNM %c%c%c%c%c%c\r\n",
		    (char) MMC_CID_PNM5(pSd->CID),
		    (char) MMC_CID_PNM4(pSd->CID),
		    (char) MMC_CID_PNM3(pSd->CID),
		    (char) MMC_CID_PNM2(pSd->CID),
		    (char) MMC_CID_PNM1(pSd->CID),
		    (char) MMC_CID_PNM0(pSd->CID));
		TRACE_2("PRV %u.%u\r\n", MMC_CID_PRV1(pSd->CID),
		    MMC_CID_PRV0(pSd->CID));
		TRACE_4("PSN 0x%02X%02X%02X%02X\r\n", MMC_CID_PSN3(pSd->CID),
		    MMC_CID_PSN2(pSd->CID), MMC_CID_PSN1(pSd->CID),
		    MMC_CID_PSN0(pSd->CID));
		if (MMC_EXT_EXT_CSD_REV(pSd->EXT) > 4 && year < 2010)
			year = year - 1997 + 2013;
		TRACE_2("MDT %u/%02u\r\n", year, MMC_CID_MDT_M(pSd->CID));
	}
#endif

	TRACE_1("CRC 0x%02X\r\n", SD_CID_CRC(pSd->CID));
}


/**
 * Display the content of the SCR register
 * \param pSCR  Pointer to SCR data.
 */
void SD_DumpSCR(const uint8_t *pSCR)
{
	(void)pSCR;

	_PrintTitle("SD Card Configuration");
	_PrintField("SCR_STRUCT 0x%X\r\n", SD_SCR_STRUCTURE(pSCR));
	_PrintField("SD_SPEC 0x%X\r\n", SD_SCR_SD_SPEC(pSCR));
	_PrintField("SD_SPEC3 %u\r\n", SD_SCR_SD_SPEC3(pSCR));
	_PrintField("SD_SPEC4 %u\r\n", SD_SCR_SD_SPEC4(pSCR));
	_PrintField("DATA_ST_AFTER_ER %u\r\n",
	    SD_SCR_DATA_STAT_AFTER_ERASE(pSCR));
	_PrintField("SD_SEC 0x%X\r\n", SD_SCR_SD_SECURITY(pSCR));
	_PrintField("EX_SEC 0x%X\r\n", SD_SCR_EX_SECURITY(pSCR));
	_PrintField("SD_BUS_WIDTHS 0x%X\r\n", SD_SCR_SD_BUS_WIDTHS(pSCR));
	_PrintField("CMD20 %u\r\n", SD_SCR_CMD20_SUPPORT(pSCR));
	_PrintField("CMD23 %u\r\n", SD_SCR_CMD23_SUPPORT(pSCR));
	_PrintField("CMD48/49 %u\r\n", SD_SCR_CMD48_SUPPORT(pSCR));
	_PrintField("CMD58/59 %u\r\n", SD_SCR_CMD58_SUPPORT(pSCR));
}

/**
 * Display the content of the SD Status Register
 * \param pSSR  Pointer to SSR data.
 */
void SD_DumpSSR(const uint8_t *pSSR)
{
	(void)pSSR;
	_PrintTitle("SD Status");
	_PrintField("DAT_BUS_WIDTH 0x%X\r\n", SD_SSR_DAT_BUS_WIDTH(pSSR));
	_PrintField("SEC_MODE %u\r\n", SD_SSR_SECURED_MODE(pSSR));
	_PrintField("SD_CARD_TYPE 0x%04X\r\n", SD_SSR_CARD_TYPE(pSSR));
	_PrintField("PAREA_SIZE %lu\r\n",
	    SD_SSR_SIZE_OF_PROTECTED_AREA(pSSR));
	_PrintField("SPD_CLASS 0x%02X\r\n", SD_SSR_SPEED_CLASS(pSSR));
	_PrintField("UHS_SPD_GRADE 0x%X\r\n", SD_SSR_UHS_SPEED_GRADE(pSSR));
	_PrintField("PE_MOVE %u MB/sec\r\n", SD_SSR_PERFORMANCE_MOVE(pSSR));
	_PrintField("AU_SIZE 0x%X\r\n", SD_SSR_AU_SIZE(pSSR));
	_PrintField("UHS_AU_SIZE 0x%X\r\n", SD_SSR_UHS_AU_SIZE(pSSR));
	_PrintField("ER_SIZE %u AU\r\n", SD_SSR_ERASE_SIZE(pSSR));
	_PrintField("ER_TIMEOUT %u sec\r\n", SD_SSR_ERASE_TIMEOUT(pSSR));
	_PrintField("ER_OFFS %u sec\r\n", SD_SSR_ERASE_OFFSET(pSSR));
}


/**
 * Display the content of the CSD register
 * \param pSd  Pointer to SdCard instance.
 */
void SD_DumpCSD(const sSdCard *pSd)
{
	const uint8_t sd_device = (pSd->bCardType & CARD_TYPE_bmSDMMC)
	    == CARD_TYPE_bmSD;
	const uint8_t sd_csd_v2 = sd_device
	    && SD_CSD_STRUCTURE(pSd->CSD) >= 0x1;

	_PrintTitle("Card-Specific Data");
	_PrintField("CSD_STRUCT 0x%X\r\n", SD_CSD_STRUCTURE(pSd->CSD));
#ifndef SDMMC_TRIM_MMC
	if (!sd_device) {
		_PrintField("SPEC_V 0x%X\r\n", MMC_CSD_SPEC_VERS(pSd->CSD));
	}
#endif
	_PrintField("TAAC 0x%X\r\n", SD_CSD_TAAC(pSd->CSD));
	_PrintField("NSAC 0x%X\r\n", SD_CSD_NSAC(pSd->CSD));
	_PrintField("TRAN_SPD 0x%X\r\n", SD_CSD_TRAN_SPEED(pSd->CSD));
	_PrintField("CCC 0x%X\r\n", SD_CSD_CCC(pSd->CSD));
	_PrintField("RD_BL_LEN 0x%X\r\n", SD_CSD_READ_BL_LEN(pSd->CSD));
	_PrintField("RD_BL_PART %u\r\n", SD_CSD_READ_BL_PARTIAL(pSd->CSD));
	_PrintField("WR_BL_MALIGN %u\r\n", SD_CSD_WRITE_BLK_MISALIGN(pSd->CSD));
	_PrintField("RD_BL_MALIGN %u\r\n", SD_CSD_READ_BLK_MISALIGN(pSd->CSD));
	_PrintField("DSR_IMP %u\r\n", SD_CSD_DSR_IMP(pSd->CSD));
	_PrintField("C_SIZE 0x%lX\r\n", sd_csd_v2 ? SD2_CSD_C_SIZE(pSd->CSD)
	    : SD_CSD_C_SIZE(pSd->CSD));
	if (!sd_csd_v2) {
		_PrintField("RD_CUR_MIN 0x%X\r\n",
		    SD_CSD_VDD_R_CURR_MIN(pSd->CSD));
		_PrintField("RD_CUR_MAX 0x%X\r\n",
		    SD_CSD_VDD_R_CURR_MAX(pSd->CSD));
		_PrintField("WR_CUR_MIN 0x%X\r\n",
		    SD_CSD_VDD_W_CURR_MIN(pSd->CSD));
		_PrintField("WR_CUR_MAX 0x%X\r\n",
		    SD_CSD_VDD_W_CURR_MAX(pSd->CSD));
		_PrintField("C_SIZE_MULT 0x%X\r\n",
		    SD_CSD_C_SIZE_MULT(pSd->CSD));
	}
	if (sd_device) {
		_PrintField("ER_BL_EN %u\r\n", SD_CSD_ERASE_BLK_EN(pSd->CSD));
		_PrintField("SECT_SIZE 0x%X\r\n", SD_CSD_SECTOR_SIZE(pSd->CSD));
	}
#ifndef SDMMC_TRIM_MMC
	else {
		_PrintField("ER_GRP_SIZE 0x%X\r\n",
		    MMC_CSD_ERASE_GRP_SIZE(pSd->CSD));
		_PrintField("ER_GRP_MULT 0x%X\r\n",
		    MMC_CSD_ERASE_GRP_MULT(pSd->CSD));
	}
#endif
#ifdef SDMMC_TRIM_MMC
	_PrintField("WP_GRP_SIZE 0x%X\r\n", SD_CSD_WP_GRP_SIZE(pSd->CSD));
#else
	_PrintField("WP_GRP_SIZE 0x%X\r\n", sd_device ?
	    SD_CSD_WP_GRP_SIZE(pSd->CSD) : MMC_CSD_WP_GRP_SIZE(pSd->CSD));
#endif
	_PrintField("WP_GRP_EN %u\r\n", SD_CSD_WP_GRP_ENABLE(pSd->CSD));
#ifndef SDMMC_TRIM_MMC
	if (!sd_device) {
		_PrintField("DEF_ECC 0x%X\r\n", MMC_CSD_DEFAULT_ECC(pSd->CSD));
	}
#endif
	_PrintField("R2W_FACT 0x%X\r\n", SD_CSD_R2W_FACTOR(pSd->CSD));
	_PrintField("WR_BL_LEN 0x%X\r\n", SD_CSD_WRITE_BL_LEN(pSd->CSD));
	_PrintField("WR_BL_PART %u\r\n", SD_CSD_WRITE_BL_PARTIAL(pSd->CSD));
	_PrintField("FILE_FMT_GRP %u\r\n", SD_CSD_FILE_FORMAT_GRP(pSd->CSD));
	_PrintField("COPY %u\r\n", SD_CSD_COPY(pSd->CSD));
	_PrintField("PERM_WP %u\r\n", SD_CSD_PERM_WRITE_PROTECT(pSd->CSD));
	_PrintField("TMP_WP %u\r\n", SD_CSD_TMP_WRITE_PROTECT(pSd->CSD));
	_PrintField("FILE_FMT 0x%X\r\n", SD_CSD_FILE_FORMAT(pSd->CSD));
#ifndef SDMMC_TRIM_MMC
	if (!sd_device) {
		_PrintField("ECC 0x%X\r\n", MMC_CSD_ECC(pSd->CSD));
	}
#endif
	_PrintField("CRC 0x%X\r\n", SD_CSD_CRC(pSd->CSD));
}

/**
 * Display the content of the EXT_CSD register
 * \param pExtCSD Pointer to extended CSD data.
 */
void SD_DumpExtCSD(const uint8_t *pExtCSD)
{
	(void)pExtCSD;
	_PrintTitle("Extended Device Specific Data");
	_PrintField("S_CMD_SET 0x%X\r\n", MMC_EXT_S_CMD_SET(pExtCSD));
	_PrintField("BOOT_INFO 0x%X\r\n", MMC_EXT_BOOT_INFO(pExtCSD));
	_PrintField("BOOT_SIZE_MULTI 0x%X\r\n",
	    MMC_EXT_BOOT_SIZE_MULTI(pExtCSD));
	_PrintField("ACC_SIZE 0x%X\r\n", MMC_EXT_ACC_SIZE(pExtCSD));
	_PrintField("HC_ER_GRP_SIZE 0x%X\r\n",
	    MMC_EXT_HC_ERASE_GRP_SIZE(pExtCSD));
	_PrintField("ER_TIMEOUT_MULT 0x%X\r\n",
	    MMC_EXT_ERASE_TIMEOUT_MULT(pExtCSD));
	_PrintField("REL_WR_SEC_C 0x%X\r\n", MMC_EXT_REL_WR_SEC_C(pExtCSD));
	_PrintField("HC_WP_GRP_SIZE 0x%X\r\n", MMC_EXT_HC_WP_GRP_SIZE(pExtCSD));
	_PrintField("S_C_VCC 0x%X\r\n", MMC_EXT_S_C_VCC(pExtCSD));
	_PrintField("S_C_VCCQ 0x%X\r\n", MMC_EXT_S_C_VCCQ(pExtCSD));
	_PrintField("S_A_TIMEOUT 0x%X\r\n", MMC_EXT_S_A_TIMEOUT(pExtCSD));
	_PrintField("SEC_CNT 0x%lX\r\n", MMC_EXT_SEC_COUNT(pExtCSD));
	_PrintField("MIN_PE_W_8_52 0x%X\r\n", MMC_EXT_MIN_PERF_W_8_52(pExtCSD));
	_PrintField("MIN_PE_R_8_52 0x%X\r\n", MMC_EXT_MIN_PERF_R_8_52(pExtCSD));
	_PrintField("MIN_PE_W_8_26_4_52 0x%X\r\n",
	    MMC_EXT_MIN_PERF_W_8_26_4_52(pExtCSD));
	_PrintField("MIN_PE_R_8_26_4_52 0x%X\r\n",
	    MMC_EXT_MIN_PERF_R_8_26_4_52(pExtCSD));
	_PrintField("MIN_PE_W_4_26 0x%X\r\n", MMC_EXT_MIN_PERF_W_4_26(pExtCSD));
	_PrintField("MIN_PE_R_4_26 0x%X\r\n", MMC_EXT_MIN_PERF_R_4_26(pExtCSD));
	_PrintField("PWR_CL_26_360 0x%X\r\n", MMC_EXT_PWR_CL_26_360(pExtCSD));
	_PrintField("PWR_CL_52_360 0x%X\r\n", MMC_EXT_PWR_CL_52_360(pExtCSD));
	_PrintField("PWR_CL_26_195 0x%X\r\n", MMC_EXT_PWR_CL_26_195(pExtCSD));
	_PrintField("PWR_CL_52_195 0x%X\r\n", MMC_EXT_PWR_CL_52_195(pExtCSD));
	_PrintField("DRV_STR 0x%X\r\n", MMC_EXT_DRV_STRENGTH(pExtCSD));
	_PrintField("CARD_TYPE 0x%X\r\n", MMC_EXT_CARD_TYPE(pExtCSD));
	_PrintField("CSD_STRUCT 0x%X\r\n", MMC_EXT_CSD_STRUCTURE(pExtCSD));
	_PrintField("EXT_CSD_REV 0x%X\r\n", MMC_EXT_EXT_CSD_REV(pExtCSD));
	_PrintField("CMD_SET 0x%X\r\n", MMC_EXT_CMD_SET(pExtCSD));
	_PrintField("CMD_SET_REV 0x%X\r\n", MMC_EXT_CMD_SET_REV(pExtCSD));
	_PrintField("PWR_CLASS 0x%X\r\n", MMC_EXT_POWER_CLASS(pExtCSD));
	_PrintField("HS_TIM 0x%X\r\n", MMC_EXT_HS_TIMING(pExtCSD));
	_PrintField("BUS_WIDTH 0x%X\r\n", MMC_EXT_BUS_WIDTH(pExtCSD));
	_PrintField("ER_MEM_CONT 0x%X\r\n", MMC_EXT_ERASED_MEM_CONT(pExtCSD));
	_PrintField("BOOT_CFG 0x%X\r\n", MMC_EXT_BOOT_CONFIG(pExtCSD));
	_PrintField("BOOT_BUS_WIDTH 0x%X\r\n", MMC_EXT_BOOT_BUS_WIDTH(pExtCSD));
	_PrintField("ER_GRP_DEF 0x%X\r\n", MMC_EXT_ERASE_GROUP_DEF(pExtCSD));
}

