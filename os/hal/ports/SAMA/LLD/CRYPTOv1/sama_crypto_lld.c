/*
    ChibiOS - Copyright (C) 2006..2016 Giovanni Di Sirio

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
#include "hal.h"

#if HAL_USE_CRY || defined(__DOXYGEN__)

#include "sama_crypto_lld.h"

#define KEY0_BUFFER_SIZE_W	HAL_CRY_MAX_KEY_SIZE/4

uint32_t key0_buffer[KEY0_BUFFER_SIZE_W];

#if defined(SAMA_DMA_REQUIRED)
static void crypto_lld_serve_read_interrupt(CRYDriver *cryp, uint32_t flags);
static void crypto_lld_serve_write_interrupt(CRYDriver *cryp, uint32_t flags);
#endif

/**
 * @brief   Wakes up the waiting thread.
 *
 * @param[in] cryp      pointer to the @p CRYDRiver object
 *
 * @notapi
 */
#define _cry_wakeup_isr(cryp) {                                             \
  osalSysLockFromISR();                                                     \
  osalThreadResumeI(&cryp->thread, MSG_OK);                               	\
  osalSysUnlockFromISR();                                                   \
}


/**
 * @brief   Common ISR code.
 * @details This code handles the portable part of the ISR code:
 *          - Callback invocation.
 *          - Waiting thread wakeup, if any.
 *          - Driver state transitions.
 *          .
 * @note    This macro is meant to be used in the low level drivers
 *          implementation only.
 *
 * @param[in] spip      pointer to the @p SPIDriver object
 *
 * @notapi
 */
#define _cry_isr_code(cryp) {                                               \
  _cry_wakeup_isr(cryp);                                                    \
}

void samaCryptoDriverInit(CRYDriver *cryp) {
	cryp->enabledPer = 0;
	cryp->thread = NULL;
}

void samaCryptoDriverStart(CRYDriver *cryp) {


	samaClearKeyBuffer();

	if (cryp->config->transfer_mode == TRANSFER_DMA)
	{
#if defined(SAMA_DMA_REQUIRED)
		cryp->dmarx = dmaChannelAllocate(SAMA_CRY_CRYD1_DMA_IRQ_PRIORITY,
				(sama_dmaisr_t) crypto_lld_serve_read_interrupt, (void *) cryp);
		osalDbgAssert(cryp->dmarx != NULL, "no channel allocated");

		cryp->dmatx = dmaChannelAllocate(SAMA_CRY_CRYD1_DMA_IRQ_PRIORITY,
				(sama_dmaisr_t) crypto_lld_serve_write_interrupt, (void *) cryp);
		osalDbgAssert(cryp->dmatx != NULL, "no channel allocated");
#endif
	}

}

void samaCryptoDriverStop(CRYDriver *cryp) {
	samaCryptoDriverDisable(cryp);
}


/**
 * write key into internal buffer
 */
cryerror_t samaCryptoDriverWriteTransientKey(const uint8_t *keyp,size_t size)
{
	uint8_t *p = (uint8_t *)key0_buffer;

	if (size <= HAL_CRY_MAX_KEY_SIZE)
	{
	  samaClearKeyBuffer();

	  for (size_t i=0;i<size;i++)
	  {
		  p[i] = keyp[i];
	  }
	}
	else
	{
	  return CRY_ERR_INV_KEY_SIZE;
	}

	  return CRY_NOERROR;
}


void samaClearKeyBuffer(void)
{
	for (size_t i=0;i<KEY0_BUFFER_SIZE_W;i++)
  {
	  key0_buffer[i] = 0;
  }
}

void samaCryptoDriverDisable(CRYDriver *cryp)
{
		if ((cryp->enabledPer & AES_PER)) {
			cryp->enabledPer &= ~AES_PER;
			pmcDisableAES();
		}
		if ((cryp->enabledPer & TDES_PER)) {
			cryp->enabledPer &= ~TDES_PER;
			pmcDisableDES();
		}
		if ((cryp->enabledPer & SHA_PER)) {
			cryp->enabledPer &= ~SHA_PER;
			pmcDisableSHA();
		}
		if ((cryp->enabledPer & TRNG_PER)) {
			cryp->enabledPer &= ~TRNG_PER;
			pmcDisableTRNG();
		}
}

#if defined(SAMA_DMA_REQUIRED)
/**
 * @brief   Shared end-of-rx service routine.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] flags     pre-shifted content of the ISR register
 */
static void crypto_lld_serve_read_interrupt(CRYDriver *cryp, uint32_t flags) {

  /* DMA errors handling.*/
  #if defined(SAMA_CRY_DMA_ERROR_HOOK)
  if ((flags & (XDMAC_CIS_RBEIS | XDMAC_CIS_ROIS)) != 0) {
	  SAMA_CRY_DMA_ERROR_HOOK(cryp);
  }
#else
  (void)flags;
#endif

  /* Stop everything.*/

  dmaChannelDisable(cryp->dmatx);
  dmaChannelDisable(cryp->dmarx);
  /* Portable CRY ISR code defined in the high level driver, note, it is
     a macro.*/
  _cry_isr_code(cryp);
}

/**
 * @brief   Shared end-of-tx service routine.
 *
 * @param[in] cryp      pointer to the @p CRYDriver object
 * @param[in] flags     pre-shifted content of the ISR register
 */
static void crypto_lld_serve_write_interrupt(CRYDriver *cryp, uint32_t flags) {

  /* DMA errors handling.*/

#if defined(SAMA_CRY_DMA_ERROR_HOOK)
  (void)cryp;
  if ((flags & (XDMAC_CIS_WBEIS | XDMAC_CIS_ROIS)) != 0) {
	  SAMA_CRY_DMA_ERROR_HOOK(cryp);
  }
#else
  (void)cryp;
  (void)flags;
#endif


}

#endif

#endif /* HAL_USE_CRY */

/** @} */
