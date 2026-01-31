/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file    RP2350/rp_isr.h
 * @brief   RP2350 ISR handler header.
 *
 * @addtogroup RP2350_ISR
 * @{
 */

#ifndef RP_ISR_H
#define RP_ISR_H

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    ISR names and numbers
 * @{
 */

/* TIMER0 interrupts (4 alarms) */
#define RP_TIMER0_IRQ0_HANDLER              Vector40
#define RP_TIMER0_IRQ1_HANDLER              Vector44
#define RP_TIMER0_IRQ2_HANDLER              Vector48
#define RP_TIMER0_IRQ3_HANDLER              Vector4C

/* TIMER1 interrupts (4 alarms) */
#define RP_TIMER1_IRQ0_HANDLER              Vector50
#define RP_TIMER1_IRQ1_HANDLER              Vector54
#define RP_TIMER1_IRQ2_HANDLER              Vector58
#define RP_TIMER1_IRQ3_HANDLER              Vector5C

/* PWM interrupts */
#define RP_PWM_IRQ_WRAP_0_HANDLER           Vector60
#define RP_PWM_IRQ_WRAP_1_HANDLER           Vector64

/* DMA interrupts */
#define RP_DMA_IRQ_0_HANDLER                Vector68
#define RP_DMA_IRQ_1_HANDLER                Vector6C
#define RP_DMA_IRQ_2_HANDLER                Vector70
#define RP_DMA_IRQ_3_HANDLER                Vector74

/* USB interrupt */
#define RP_USBCTRL_IRQ_HANDLER              Vector78

/* PIO interrupts */
#define RP_PIO0_IRQ_0_HANDLER               Vector7C
#define RP_PIO0_IRQ_1_HANDLER               Vector80
#define RP_PIO1_IRQ_0_HANDLER               Vector84
#define RP_PIO1_IRQ_1_HANDLER               Vector88
#define RP_PIO2_IRQ_0_HANDLER               Vector8C
#define RP_PIO2_IRQ_1_HANDLER               Vector90

/* IO Bank interrupts */
#define RP_IO_IRQ_BANK0_HANDLER             Vector94
#define RP_IO_IRQ_BANK0_NS_HANDLER          Vector98
#define RP_IO_IRQ_QSPI_HANDLER              Vector9C
#define RP_IO_IRQ_QSPI_NS_HANDLER           VectorA0

/* SIO interrupts */
#define RP_SIO_IRQ_FIFO_HANDLER             VectorA4
#define RP_SIO_IRQ_BELL_HANDLER             VectorA8
#define RP_SIO_IRQ_FIFO_NS_HANDLER          VectorAC
#define RP_SIO_IRQ_BELL_NS_HANDLER          VectorB0
#define RP_SIO_IRQ_MTIMECMP_HANDLER         VectorB4

/* Clocks interrupt */
#define RP_CLOCKS_IRQ_HANDLER               VectorB8

/* SPI interrupts */
#define RP_SPI0_IRQ_HANDLER                 VectorBC
#define RP_SPI1_IRQ_HANDLER                 VectorC0

/* UART interrupts */
#define RP_UART0_IRQ_HANDLER                VectorC4
#define RP_UART1_IRQ_HANDLER                VectorC8

/* ADC interrupt */
#define RP_ADC_IRQ_FIFO_HANDLER             VectorCC

/* I2C interrupts */
#define RP_I2C0_IRQ_HANDLER                 VectorD0
#define RP_I2C1_IRQ_HANDLER                 VectorD4

/* OTP interrupt */
#define RP_OTP_IRQ_HANDLER                  VectorD8

/* TRNG interrupt */
#define RP_TRNG_IRQ_HANDLER                 VectorDC

/* CTI interrupts */
#define RP_PROC0_IRQ_CTI_HANDLER            VectorE0
#define RP_PROC1_IRQ_CTI_HANDLER            VectorE4

/* PLL interrupts */
#define RP_PLL_SYS_IRQ_HANDLER              VectorE8
#define RP_PLL_USB_IRQ_HANDLER              VectorEC

/* Power manager interrupts */
#define RP_POWMAN_IRQ_POW_HANDLER           VectorF0
#define RP_POWMAN_IRQ_TIMER_HANDLER         VectorF4

/* Spare interrupts */
#define RP_SPARE_IRQ_0_HANDLER              VectorF8
#define RP_SPARE_IRQ_1_HANDLER              VectorFC
#define RP_SPARE_IRQ_2_HANDLER              Vector100
#define RP_SPARE_IRQ_3_HANDLER              Vector104
#define RP_SPARE_IRQ_4_HANDLER              Vector108
#define RP_SPARE_IRQ_5_HANDLER              Vector10C

/* IRQ numbers */
#define RP_TIMER0_IRQ0_NUMBER               0
#define RP_TIMER0_IRQ1_NUMBER               1
#define RP_TIMER0_IRQ2_NUMBER               2
#define RP_TIMER0_IRQ3_NUMBER               3
#define RP_TIMER1_IRQ0_NUMBER               4
#define RP_TIMER1_IRQ1_NUMBER               5
#define RP_TIMER1_IRQ2_NUMBER               6
#define RP_TIMER1_IRQ3_NUMBER               7
#define RP_PWM_IRQ_WRAP_0_NUMBER            8
#define RP_PWM_IRQ_WRAP_1_NUMBER            9
#define RP_DMA_IRQ_0_NUMBER                 10
#define RP_DMA_IRQ_1_NUMBER                 11
#define RP_DMA_IRQ_2_NUMBER                 12
#define RP_DMA_IRQ_3_NUMBER                 13
#define RP_USBCTRL_IRQ_NUMBER               14
#define RP_PIO0_IRQ_0_NUMBER                15
#define RP_PIO0_IRQ_1_NUMBER                16
#define RP_PIO1_IRQ_0_NUMBER                17
#define RP_PIO1_IRQ_1_NUMBER                18
#define RP_PIO2_IRQ_0_NUMBER                19
#define RP_PIO2_IRQ_1_NUMBER                20
#define RP_IO_IRQ_BANK0_NUMBER              21
#define RP_IO_IRQ_BANK0_NS_NUMBER           22
#define RP_IO_IRQ_QSPI_NUMBER               23
#define RP_IO_IRQ_QSPI_NS_NUMBER            24
#define RP_SIO_IRQ_FIFO_NUMBER              25
#define RP_SIO_IRQ_BELL_NUMBER              26
#define RP_SIO_IRQ_FIFO_NS_NUMBER           27
#define RP_SIO_IRQ_BELL_NS_NUMBER           28
#define RP_SIO_IRQ_MTIMECMP_NUMBER          29
#define RP_CLOCKS_IRQ_NUMBER                30
#define RP_SPI0_IRQ_NUMBER                  31
#define RP_SPI1_IRQ_NUMBER                  32
#define RP_UART0_IRQ_NUMBER                 33
#define RP_UART1_IRQ_NUMBER                 34
#define RP_ADC_IRQ_FIFO_NUMBER              35
#define RP_I2C0_IRQ_NUMBER                  36
#define RP_I2C1_IRQ_NUMBER                  37
#define RP_OTP_IRQ_NUMBER                   38
#define RP_TRNG_IRQ_NUMBER                  39
#define RP_PROC0_IRQ_CTI_NUMBER             40
#define RP_PROC1_IRQ_CTI_NUMBER             41
#define RP_PLL_SYS_IRQ_NUMBER               42
#define RP_PLL_USB_IRQ_NUMBER               43
#define RP_POWMAN_IRQ_POW_NUMBER            44
#define RP_POWMAN_IRQ_TIMER_NUMBER          45
#define RP_SPARE_IRQ_0_NUMBER               46
#define RP_SPARE_IRQ_1_NUMBER               47
#define RP_SPARE_IRQ_2_NUMBER               48
#define RP_SPARE_IRQ_3_NUMBER               49
#define RP_SPARE_IRQ_4_NUMBER               50
#define RP_SPARE_IRQ_5_NUMBER               51
/** @} */

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void irqInit(void);
  void irqDeinit(void);
#ifdef __cplusplus
}
#endif

#endif /* RP_ISR_H */

/** @} */
