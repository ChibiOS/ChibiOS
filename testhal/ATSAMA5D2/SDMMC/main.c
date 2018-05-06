/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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
#include <string.h>
#include "ch.h"
#include "hal.h"
#include "sama_sdmmc_lld.h"
#include "chprintf.h"

#if SDMMC_USE_FF_LIB == 1
#include "fat32test.h"
#endif

#if SDMMC_USE_RELEDGE_LIB == 1
#include "reledgetest.h"
#endif

//-----------	DEMO CONFIGURATION	------------------------
//0 (SLOT0) or 1 (SLOT1)
#define DEMO_SLOT					0
//1 for FFLib, 0 for Reliance
#define DEMO_FAT					1
//----------------------------------------------------------

#define BLOCK_CNT_MAX               32u
#define DMADL_CNT_MAX               64u
#define BLOCK_CNT                   3u



CACHE_ALIGNED uint8_t data_buf[BLOCK_CNT_MAX * 512ul];

CACHE_ALIGNED static uint32_t dma_table[DMADL_CNT_MAX * SDMMC_DMADL_SIZE];

CACHE_ALIGNED uint8_t sdmmcbuffer[ROUND_UP_MULT(SDMMC_BUFFER_SIZE, L1_CACHE_BYTES)];



BaseSequentialStream * ts;

static const SerialConfig sdcfg = { 115200, 0,UART_MR_PAR_NO };

/*
 * LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 512);
static THD_FUNCTION(Thread1, arg) {

	(void) arg;
	chRegSetThreadName("blinker");

	while (true) {
		palToggleLine(LINE_LED_BLUE);
		chThdSleepMilliseconds(500);
	}
}

#if DEMO_SLOT == 0
static const SamaSDMMCConfig sdmmc_cfg = {
		SDMMC_SLOT0,
		sdmmcbuffer,
		data_buf,
		sizeof(data_buf),
		dma_table,
		DMADL_CNT_MAX
};
#else
static const SamaSDMMCConfig sdmmc_cfg = {
		SDMMC_SLOT1,
		sdmmcbuffer,
		data_buf,
		sizeof(data_buf),
		dma_table,
		DMADL_CNT_MAX
};
#endif

/*
 * Application entry point.
 */
int main(void) {

	/*
	 * System initializations.
	 * - HAL initialization, this also initializes the configured device drivers
	 *   and performs the board-specific initializations.
	 * - Kernel initialization, the main() function becomes a thread and the
	 *   RTOS is active.
	 */
	halInit();
	chSysInit();

	/*
	 * Activates the serial driver 0 using the driver default configuration.
	 */
	sdStart(&SD1, &sdcfg);

	/* Redirecting  UART0 RX on PD2 and UART0 TX on PD3. */
	palSetGroupMode(PIOD, PAL_PORT_BIT(2) | PAL_PORT_BIT(3), 0U,
			PAL_SAMA_FUNC_PERIPH_A | PAL_MODE_SECURE);

	ts = (BaseSequentialStream *) &SD1;

	sdmmcInit();

	sdmmcStart(&SDMMCD1, &sdmmc_cfg);

	if (SDMMCD1.state != MCID_INIT_ERROR) {

		chprintf(ts,"starting MMC Demo with slot %d\n\r",SDMMCD1.config->slot_id);

		if (sdmmcOpenDevice(&SDMMCD1)) {

#if DEMO_FAT	== 1
#if SDMMC_USE_FF_LIB == 1
			fat32test_demo();
#endif
#else
#if SDMMC_USE_RELEDGE_LIB == 1
			relianceedge_demo();
#endif
#endif

			if (SDMMCD1.config->slot_id == SDMMC_SLOT1)
			{
				chprintf(ts,"remove card to finish demo\n\r");
				sdmmcCloseDevice(&SDMMCD1);
			}

		}


	} else {
		chprintf(ts,"error,stopping MMC\n\r");
		sdmmcStop(&SDMMCD1);
	}

	/*
	 * Creates the blinker thread.
	 */
	chThdCreateStatic(waThread1, sizeof(waThread1), NORMALPRIO, Thread1, NULL);

	/*
	 * Normal main() thread activity, in this demo it does nothing except
	 * sleeping in a loop and check the button state.
	 */
	while (true) {
		chThdSleepMilliseconds(500);
	}
}


bool sdmmcGetInstance(uint8_t index, SdmmcDriver **sdmmcp)
{
	(void)index;

	*sdmmcp = &SDMMCD1;

	//read();

	return true;
}




