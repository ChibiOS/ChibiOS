/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

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

/**
 * @file    RX62N/rx62n_mii.c
 * @brief   RX62N low level MII driver code.
 *
 * @addtogroup RX62N_MII
 * @{
 */

#include "ch.h"
#include "hal.h"
#include "rx62n_mii.h"

#if HAL_USE_MAC || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local variables.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Driver local functions.                                                   */
/*===========================================================================*/

#if (RX62N_SYSCLK == 96000000)
__attribute__((always_inline))
static inline void delay200ns(void) {

  asm volatile ("mov.l   #5,r2                                      \n\t"
                "1:                                                 \n\t"
                "sub     #1,r2                                      \n\t"
                "bne.b   1b                                         \n\t");
}

__attribute__((always_inline))
static inline void delay1ms(void) {

  asm volatile ("mov.l   #24000,r2                                  \n\t"
                "1:                                                 \n\t"
                "sub     #1,r2                                      \n\t"
                "bne.b   1b                                         \n\t");
}
#else
#error Adjust delay200ns and delay1ms for RX62N_SYSCLK other than 96MHz.
#endif

/**
 * @brief   Write 1 bit to SMI bus.
 *
 * @param[in] b         the bit value
 */
static void smi_bit_w(uint32_t b) {
  uint32_t pir = PIR_MMD;

  if (b)
    pir |= PIR_MDO;
  ETHERC.PIR.LONG = pir;
  /* wait 10ns */
  asm volatile("nop");
  ETHERC.PIR.LONG = pir | PIR_MDC;
  delay200ns();
  ETHERC.PIR.LONG = pir;
  delay200ns();
}

/**
 * @brief   Read 1 bit from SMI bus.
 *
 * @return              The bit value.
 */
static uint32_t smi_bit_r(void) {
  uint32_t pir = 0;

  ETHERC.PIR.LONG = PIR_MDC;
  delay200ns();
  ETHERC.PIR.LONG = 0;
  delay200ns();
  if (ETHERC.PIR.LONG & PIR_MDI)
    pir = 1;
  return pir;
}

/**
 * @brief   Release SMI bus.
 */
static void smi_z0(void) {

  ETHERC.PIR.LONG = PIR_MDC;
  delay200ns();
  ETHERC.PIR.LONG = 0;
  delay200ns();
}

/**
 * @brief   Write up to 32 bits to SMI bus (MSB first).
 *
 * @note    Writing starts from len-1 bit.
 * @param[in] value     value to write
 * @param[in] len       number of bits
 */
static void smi_write(uint32_t value, uint8_t len) {

  while (len--)
    smi_bit_w(value & (1<<len) );
}

/**
 * @brief   Read up to 16 bits from SMI bus (MSB first).
 *
 * @param[in] len       number of bits
 * @return              The bits value.
 */
static phyreg_t smi_read(uint8_t len) {
  phyreg_t value = 0;

  while (len--) {
    value <<= 1;
    if (smi_bit_r())
      value |= 1;
  }
  return value;
}

static phyreg_t mii_read_reg(uint8_t phyaddr, phyaddr_t regaddr) {
  phyreg_t value;

  /* preamble */
  smi_write(0xFFFFFFFF, 32);
  /* start of frame */
  smi_write(1, 2);
  /* opcode (read) */
  smi_write(2, 2);
  /* phy address */
  smi_write(phyaddr, 5);
  /* phy register */
  smi_write(regaddr, 5);
  /* turn around */
  smi_z0();
  /* phy register */
  value = smi_read(16);
  /* idle */
  smi_z0();
  return value;
}

static void mii_write_reg(uint8_t phyaddr, phyaddr_t regaddr, phyreg_t value) {

  /* preamble */
  smi_write(0xFFFFFFFF, 32);
  /* start of frame */
  smi_write(1, 2);
  /* opcode (write) */
  smi_write(1, 2);
  /* phy address */
  smi_write(phyaddr, 5);
  /* phy register */
  smi_write(regaddr, 5);
  /* turn around */
  smi_write(1, 2);
  /* phy register */
  smi_write(value, 16);
  /* idle */
  ETHERC.PIR.LONG = 0;
}

/*===========================================================================*/
/* Driver interrupt handlers.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Driver exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Low level MII driver initialization.
 *
 * @notapi
 */
void miiInit(void) {

}

/**
 * @brief   Resets a PHY device.
 *
 * @param[in] macp      pointer to the @p MACDriver object
 *
 * @notapi
 */
void miiReset(MACDriver *macp) {

  (void)macp;

  /*
   * PHY reset by pulsing the RESET_OUT pin.
   */
  ETHERC.PIR.LONG = 0;
  /* Release PHY pins */
  IOPORT.PFENET.BYTE = 0;
  /* Pulse RESET_OUT pin */
  palClearPad(GPIO10, PORTA_ETH_RESETOUT);
  delay1ms();
  palSetPad(GPIO10, PORTA_ETH_RESETOUT);
  delay1ms();
  /* Enable Ethernet pins, set RMII mode */
  IOPORT.PFENET.BYTE = (1<<7)|(0<<4)|(1<<1);
}

/**
 * @brief   Reads a PHY register through the MII interface.
 *
 * @param[in] macp      pointer to the @p MACDriver object
 * @param[in] addr      the register address
 * @return              The register value.
 *
 * @notapi
 */
phyreg_t miiGet(MACDriver *macp, phyaddr_t regaddr) {

  (void)macp;
  return mii_read_reg(macp->phyaddr, regaddr);
}

/**
 * @brief   Writes a PHY register through the MII interface.
 *
 * @param[in] macp      pointer to the @p MACDriver object
 * @param[in] addr      the register address
 * @param[in] value     the new register value
 *
 * @notapi
 */
void miiPut(MACDriver *macp, phyaddr_t regaddr, phyreg_t value) {

  (void)macp;
  mii_write_reg(macp->phyaddr, regaddr, value);
}

#endif /* HAL_USE_MAC */

/** @} */
