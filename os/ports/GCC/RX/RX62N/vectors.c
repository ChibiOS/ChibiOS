/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011,2012,2013 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
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

/**
 * @file    GCC/RX/RX62N/vectors.c
 * @brief   Interrupt vectors for the RX family.
 *
 * @defgroup RX_RX62N_VECTORS RX62N Interrupt Vectors
 * @ingroup RX_SPECIFIC
 * @details Interrupt vectors for the RX family.
 * @{
 */
#include "ch.h"
#include "hal.h"


/**
 * @brief   Unhandled exceptions handler.
 * @details Any undefined exception vector points to this function by default.
 *          This function simply stops the system into an infinite loop.
 *
 * @notapi
 */
#if !defined(__DOXYGEN__)
__attribute__ ((naked))
#endif
void _unhandled_exception(void) {

  while (1);
}

#if !defined(__DOXYGEN__)
/* hardware vectors */
extern void ResetHandler(void);
void NMIHandler(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void UndefHandler(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void FPUHandler(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void SVHandler(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
/* relocatable vectors */
void Excep_BRK(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_BUSERR(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_FCU_FCUERR(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_FCU_FRDYI(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_ICU_SWINT(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_CMTU0_CMT0(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_CMTU0_CMT1(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_CMTU1_CMT2(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_CMTU1_CMT3(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_ETHER_EINT(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_USB0_D0FIFO0(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_USB0_D1FIFO0(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_USB0_USBI0(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_USB1_D0FIFO1(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_USB1_D1FIFO1(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_USB1_USBI1(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_RSPI0_SPEI0(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_RSPI0_SPRI0(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_RSPI0_SPTI0(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_RSPI0_SPII0(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_RSPI1_SPEI1(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_RSPI1_SPRI1(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_RSPI1_SPTI1(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_RSPI1_SPII1(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_CAN0_ERS0(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_CAN0_RXF0(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_CAN0_TXF0(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_CAN0_RXM0(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_CAN0_TXM0(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_RTC_PRD(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_RTC_CUP(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_IRQ0(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_IRQ1(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_IRQ2(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_IRQ3(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_IRQ4(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_IRQ5(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_IRQ6(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_IRQ7(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_IRQ8(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_IRQ9(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_IRQ10(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_IRQ11(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_IRQ12(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_IRQ13(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_IRQ14(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_IRQ15(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_USB_USBR0(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_USB_USBR1(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_RTC_ALM(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_WDT_WOVI(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_AD0_ADI0(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_AD1_ADI1(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_S12AD_ADI12(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU0_TGIA0(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU0_TGIB0(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU0_TGIC0(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU0_TGID0(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU0_TCIV0(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU0_TGIE0(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU0_TGIF0(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU1_TGIA1(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU1_TGIB1(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU1_TCIV1(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU1_TCIU1(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU2_TGIA2(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU2_TGIB2(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU2_TCIV2(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU2_TCIU2(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU3_TGIA3(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU3_TGIB3(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU3_TGIC3(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU3_TGID3(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU3_TCIV3(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU4_TGIA4(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU4_TGIB4(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU4_TGIC4(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU4_TGID4(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU4_TCIV4(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU5_TCIU5(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU5_TCIV5(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU5_TCIW5(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU6_TGIA6(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU6_TGIB6(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU6_TGIC6(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU6_TGID6(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU6_TCIV6(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU6_TGIE6(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU6_TGIF6(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU7_TGIA7(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU7_TGIB7(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU7_TCIV7(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU7_TCIU7(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU8_TGIA8(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU8_TGIB8(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU8_TCIV8(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU8_TCIU8(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU9_TGIA9(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU9_TGIB9(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU9_TGIC9(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU9_TGID9(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU9_TCIV9(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU10_TGIA10(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU10_TGIB10(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU10_TGIC10(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU10_TGID10(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU10_TCIV10(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU11_TCIU11(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU11_TCIV11(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_MTU11_TCIW11(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_POE_OEI1(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_POE_OEI2(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_POE_OEI3(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_POE_OEI4(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_TMR0_CMI0A(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_TMR0_CMI0B(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_TMR0_OV0I(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_TMR1_CMI1A(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_TMR1_CMI1B(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_TMR1_OV1I(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_TMR2_CMI2A(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_TMR2_CMI2B(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_TMR2_OV2I(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_TMR3_CMI3A(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_TMR3_CMI3B(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_TMR3_OV3I(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_DMACA_DMAC0(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_DMACA_DMAC1(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_DMACA_DMAC2(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_DMACA_DMAC3(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_EXDMAC_DMAC0(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_EXDMAC_DMAC1(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_SCI0_ERI0(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_SCI0_RXI0(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_SCI0_TXI0(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_SCI0_TEI0(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_SCI1_ERI1(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_SCI1_RXI1(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_SCI1_TXI1(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_SCI1_TEI1(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_SCI2_ERI2(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_SCI2_RXI2(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_SCI2_TXI2(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_SCI2_TEI2(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_SCI3_ERI3(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_SCI3_RXI3(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_SCI3_TXI3(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_SCI3_TEI3(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_SCI5_ERI5(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_SCI5_RXI5(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_SCI5_TXI5(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_SCI5_TEI5(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_SCI6_ERI6(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_SCI6_RXI6(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_SCI6_TXI6(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_SCI6_TEI6(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_RIIC0_EEI0(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_RIIC0_RXI0(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_RIIC0_TXI0(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_RIIC0_TEI0(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_RIIC1_EEI1(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_RIIC1_RXI1(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_RIIC1_TXI1(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
void Excep_RIIC1_TEI1(void)__attribute__((interrupt, weak, alias("_unhandled_exception")));
#endif

#if defined(RAMRUN)
__attribute__((naked,section(".ramjumps")))
void ramjumps(void)
{
  asm volatile ("bra.a _ResetHandler\n\t\
                 bra.a _NMIHandler\n\t\
                 bra.a _UndefHandler\n\t\
                 bra.a _FPUHandler\n\t\
                 bra.a _SVHandler\n\t");
}
#endif

const unsigned long _hardwareVectors[] __attribute__ ((section (".fvectors"))) = {
/* 0xFFFFFF80 - reserved */
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
    0, 0, 0, 0,
/* 0xFFFFFFD0 - Supervisor Instruction */
    16,
/* 0xFFFFFFD4 - reserved */
    0, 0,
/* 0xFFFFFFDC  Undefined Instruction */
    8,
/* 0xFFFFFFE0 - reserved */
    0,
/* 0xFFFFFFE4 - Floating Point */
    12,
/* 0xFFFFFFE8 - reserved */
    0, 0, 0, 0,
/* 0xFFFFFFF8 - NMI */
    4,
/* 0xFFFFFFFC - RESET */
    0
};

void (*_relocatableVectors[256])(void) __attribute__ ((section (".rvectors"))) = {
  Excep_BRK,                   // 0
  _unhandled_exception,        // 1
  _unhandled_exception,        // 2
  _unhandled_exception,        // 3
  _unhandled_exception,        // 4
  _unhandled_exception,        // 5
  _unhandled_exception,        // 6
  _unhandled_exception,        // 7
  _unhandled_exception,        // 8
  _unhandled_exception,        // 9
  _unhandled_exception,        // 10
  _unhandled_exception,        // 11
  _unhandled_exception,        // 12
  _unhandled_exception,        // 13
  _unhandled_exception,        // 14
  _unhandled_exception,        // 15
  Excep_BUSERR,                // 16
  _unhandled_exception,        // 17
  _unhandled_exception,        // 18
  _unhandled_exception,        // 19
  _unhandled_exception,        // 20
  Excep_FCU_FCUERR,            // 21
  _unhandled_exception,        // 22
  Excep_FCU_FRDYI,             // 23
  _unhandled_exception,        // 24
  _unhandled_exception,        // 25
  _unhandled_exception,        // 26
  Excep_ICU_SWINT,             // 27
  Excep_CMTU0_CMT0,            // 28
  Excep_CMTU0_CMT1,            // 29
  Excep_CMTU1_CMT2,            // 30
  Excep_CMTU1_CMT3,            // 31
  Excep_ETHER_EINT,            // 32
  _unhandled_exception,        // 33
  _unhandled_exception,        // 34
  _unhandled_exception,        // 35
  Excep_USB0_D0FIFO0,          // 36
  Excep_USB0_D1FIFO0,          // 37
  Excep_USB0_USBI0,            // 38
  _unhandled_exception,        // 39
  Excep_USB1_D0FIFO1,          // 40
  Excep_USB1_D1FIFO1,          // 41
  Excep_USB1_USBI1,            // 42
  _unhandled_exception,        // 43
  Excep_RSPI0_SPEI0,           // 44
  Excep_RSPI0_SPRI0,           // 45
  Excep_RSPI0_SPTI0,           // 46
  Excep_RSPI0_SPII0,           // 47
  Excep_RSPI1_SPEI1,           // 48
  Excep_RSPI1_SPRI1,           // 49
  Excep_RSPI1_SPTI1,           // 50
  Excep_RSPI1_SPII1,           // 51
  _unhandled_exception,        // 52
  _unhandled_exception,        // 53
  _unhandled_exception,        // 54
  _unhandled_exception,        // 55
  Excep_CAN0_ERS0,             // 56
  Excep_CAN0_RXF0,             // 57
  Excep_CAN0_TXF0,             // 58
  Excep_CAN0_RXM0,             // 59
  Excep_CAN0_TXM0,             // 60
  _unhandled_exception,        // 61
  Excep_RTC_PRD,               // 62
  Excep_RTC_CUP,               // 63
  Excep_IRQ0,                  // 64
  Excep_IRQ1,                  // 65
  Excep_IRQ2,                  // 66
  Excep_IRQ3,                  // 67
  Excep_IRQ4,                  // 68
  Excep_IRQ5,                  // 69
  Excep_IRQ6,                  // 70
  Excep_IRQ7,                  // 71
  Excep_IRQ8,                  // 72
  Excep_IRQ9,                  // 73
  Excep_IRQ10,                 // 74
  Excep_IRQ11,                 // 75
  Excep_IRQ12,                 // 76
  Excep_IRQ13,                 // 77
  Excep_IRQ14,                 // 78
  Excep_IRQ15,                 // 79
  _unhandled_exception,        // 80
  _unhandled_exception,        // 81
  _unhandled_exception,        // 82
  _unhandled_exception,        // 83
  _unhandled_exception,        // 84
  _unhandled_exception,        // 85
  _unhandled_exception,        // 86
  _unhandled_exception,        // 87
  _unhandled_exception,        // 88
  _unhandled_exception,        // 89
  Excep_USB_USBR0,             // 90
  Excep_USB_USBR1,             // 91
  Excep_RTC_ALM,               // 92
  _unhandled_exception,        // 93
  _unhandled_exception,        // 94
  _unhandled_exception,        // 95
  Excep_WDT_WOVI,              // 96
  _unhandled_exception,        // 97
  Excep_AD0_ADI0,              // 98
  Excep_AD1_ADI1,              // 99
  _unhandled_exception,        // 100
  _unhandled_exception,        // 101
  Excep_S12AD_ADI12,           // 102
  _unhandled_exception,        // 103
  _unhandled_exception,        // 104
  _unhandled_exception,        // 105
  _unhandled_exception,        // 106
  _unhandled_exception,        // 107
  _unhandled_exception,        // 108
  _unhandled_exception,        // 109
  _unhandled_exception,        // 110
  _unhandled_exception,        // 111
  _unhandled_exception,        // 112
  _unhandled_exception,        // 113
  Excep_MTU0_TGIA0,            // 114
  Excep_MTU0_TGIB0,            // 115
  Excep_MTU0_TGIC0,            // 116
  Excep_MTU0_TGID0,            // 117
  Excep_MTU0_TCIV0,            // 118
  Excep_MTU0_TGIE0,            // 119
  Excep_MTU0_TGIF0,            // 120
  Excep_MTU1_TGIA1,            // 121
  Excep_MTU1_TGIB1,            // 122
  Excep_MTU1_TCIV1,            // 123
  Excep_MTU1_TCIU1,            // 124
  Excep_MTU2_TGIA2,            // 125
  Excep_MTU2_TGIB2,            // 126
  Excep_MTU2_TCIV2,            // 127
  Excep_MTU2_TCIU2,            // 128
  Excep_MTU3_TGIA3,            // 129
  Excep_MTU3_TGIB3,            // 130
  Excep_MTU3_TGIC3,            // 131
  Excep_MTU3_TGID3,            // 132
  Excep_MTU3_TCIV3,            // 133
  Excep_MTU4_TGIA4,            // 134
  Excep_MTU4_TGIB4,            // 135
  Excep_MTU4_TGIC4,            // 136
  Excep_MTU4_TGID4,            // 137
  Excep_MTU4_TCIV4,            // 138
  Excep_MTU5_TCIU5,            // 139
  Excep_MTU5_TCIV5,            // 140
  Excep_MTU5_TCIW5,            // 141
  Excep_MTU6_TGIA6,            // 142
  Excep_MTU6_TGIB6,            // 143
  Excep_MTU6_TGIC6,            // 144
  Excep_MTU6_TGID6,            // 145
  Excep_MTU6_TCIV6,            // 146
  Excep_MTU6_TGIE6,            // 147
  Excep_MTU6_TGIF6,            // 148
  Excep_MTU7_TGIA7,            // 149
  Excep_MTU7_TGIB7,            // 150
  Excep_MTU7_TCIV7,            // 151
  Excep_MTU7_TCIU7,            // 152
  Excep_MTU8_TGIA8,            // 153
  Excep_MTU8_TGIB8,            // 154
  Excep_MTU8_TCIV8,            // 155
  Excep_MTU8_TCIU8,            // 156
  Excep_MTU9_TGIA9,            // 157
  Excep_MTU9_TGIB9,            // 158
  Excep_MTU9_TGIC9,            // 159
  Excep_MTU9_TGID9,            // 160
  Excep_MTU9_TCIV9,            // 161
  Excep_MTU10_TGIA10,          // 162
  Excep_MTU10_TGIB10,          // 163
  Excep_MTU10_TGIC10,          // 164
  Excep_MTU10_TGID10,          // 165
  Excep_MTU10_TCIV10,          // 166
  Excep_MTU11_TCIU11,          // 167
  Excep_MTU11_TCIV11,          // 168
  Excep_MTU11_TCIW11,          // 169
  Excep_POE_OEI1,              // 170
  Excep_POE_OEI2,              // 171
  Excep_POE_OEI3,              // 172
  Excep_POE_OEI4,              // 173
  Excep_TMR0_CMI0A,            // 174
  Excep_TMR0_CMI0B,            // 175
  Excep_TMR0_OV0I,             // 176
  Excep_TMR1_CMI1A,            // 177
  Excep_TMR1_CMI1B,            // 178
  Excep_TMR1_OV1I,             // 179
  Excep_TMR2_CMI2A,            // 180
  Excep_TMR2_CMI2B,            // 181
  Excep_TMR2_OV2I,             // 182
  Excep_TMR3_CMI3A,            // 183
  Excep_TMR3_CMI3B,            // 184
  Excep_TMR3_OV3I,             // 185
  _unhandled_exception,        // 186
  _unhandled_exception,        // 187
  _unhandled_exception,        // 188
  _unhandled_exception,        // 189
  _unhandled_exception,        // 190
  _unhandled_exception,        // 191
  _unhandled_exception,        // 192
  _unhandled_exception,        // 193
  _unhandled_exception,        // 194
  _unhandled_exception,        // 195
  _unhandled_exception,        // 196
  _unhandled_exception,        // 197
  Excep_DMACA_DMAC0,           // 198
  Excep_DMACA_DMAC1,           // 199
  Excep_DMACA_DMAC2,           // 200
  Excep_DMACA_DMAC3,           // 201
  Excep_EXDMAC_DMAC0,          // 202
  Excep_EXDMAC_DMAC1,          // 203
  _unhandled_exception,        // 204
  _unhandled_exception,        // 205
  _unhandled_exception,        // 206
  _unhandled_exception,        // 207
  _unhandled_exception,        // 208
  _unhandled_exception,        // 209
  _unhandled_exception,        // 210
  _unhandled_exception,        // 211
  _unhandled_exception,        // 212
  _unhandled_exception,        // 213
  Excep_SCI0_ERI0,             // 214
  Excep_SCI0_RXI0,             // 215
  Excep_SCI0_TXI0,             // 216
  Excep_SCI0_TEI0,             // 217
  Excep_SCI1_ERI1,             // 218
  Excep_SCI1_RXI1,             // 219
  Excep_SCI1_TXI1,             // 220
  Excep_SCI1_TEI1,             // 221
  Excep_SCI2_ERI2,             // 222
  Excep_SCI2_RXI2,             // 223
  Excep_SCI2_TXI2,             // 224
  Excep_SCI2_TEI2,             // 225
  Excep_SCI3_ERI3,             // 226
  Excep_SCI3_RXI3,             // 227
  Excep_SCI3_TXI3,             // 228
  Excep_SCI3_TEI3,             // 229
  _unhandled_exception,        // 230
  _unhandled_exception,        // 231
  _unhandled_exception,        // 232
  _unhandled_exception,        // 233
  Excep_SCI5_ERI5,             // 234
  Excep_SCI5_RXI5,             // 235
  Excep_SCI5_TXI5,             // 236
  Excep_SCI5_TEI5,             // 237
  Excep_SCI6_ERI6,             // 238
  Excep_SCI6_RXI6,             // 239
  Excep_SCI6_TXI6,             // 240
  Excep_SCI6_TEI6,             // 241
  _unhandled_exception,        // 242
  _unhandled_exception,        // 243
  _unhandled_exception,        // 244
  _unhandled_exception,        // 245
  Excep_RIIC0_EEI0,            // 246
  Excep_RIIC0_RXI0,            // 247
  Excep_RIIC0_TXI0,            // 248
  Excep_RIIC0_TEI0,            // 249
  Excep_RIIC1_EEI1,            // 250
  Excep_RIIC1_RXI1,            // 251
  Excep_RIIC1_TXI1,            // 252
  Excep_RIIC1_TEI1,            // 253
  _unhandled_exception,        // 254
  _unhandled_exception,        // 255
};

/** @} */
