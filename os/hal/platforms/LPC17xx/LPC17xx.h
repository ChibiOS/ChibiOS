/******************************************************************************
 * @file:    LPC17xx.h
 * @purpose: CMSIS Cortex-M3 Core Peripheral Access Layer Header File for 
 *           NXP LPC17xx Device Series 
 * @version: V1.10
 * @date:    24. September 2010
 *----------------------------------------------------------------------------
 *
 * @note
 * Copyright (C) 2010 ARM Limited. All rights reserved.
 *
 * @par
 * ARM Limited (ARM) is supplying this software for use with Cortex-M3 
 * processor based microcontrollers.  This file can be freely distributed 
 * within development tools that are supporting such ARM based processors. 
 *
 * @par
 * THIS SOFTWARE IS PROVIDED "AS IS".  NO WARRANTIES, WHETHER EXPRESS, IMPLIED
 * OR STATUTORY, INCLUDING, BUT NOT LIMITED TO, IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE APPLY TO THIS SOFTWARE.
 * ARM SHALL NOT, IN ANY CIRCUMSTANCES, BE LIABLE FOR SPECIAL, INCIDENTAL, OR
 * CONSEQUENTIAL DAMAGES, FOR ANY REASON WHATSOEVER.
 *
 ******************************************************************************/


#ifndef __LPC17xx_H__
#define __LPC17xx_H__

/*
 * ==========================================================================
 * ---------- Interrupt Number Definition -----------------------------------
 * ==========================================================================
 */

/** @addtogroup LPC17xx_System
 * @{
 */

/** @brief IRQ interrupt source definition */
typedef enum IRQn
{
/******  Cortex-M3 Processor Exceptions Numbers ***************************************************/
  NonMaskableInt_IRQn           = -14,      /*!< 2 Non Maskable Interrupt                         */
  MemoryManagement_IRQn         = -12,      /*!< 4 Cortex-M3 Memory Management Interrupt          */
  BusFault_IRQn                 = -11,      /*!< 5 Cortex-M3 Bus Fault Interrupt                  */
  UsageFault_IRQn               = -10,      /*!< 6 Cortex-M3 Usage Fault Interrupt                */
  SVCall_IRQn                   = -5,       /*!< 11 Cortex-M3 SV Call Interrupt                   */
  DebugMonitor_IRQn             = -4,       /*!< 12 Cortex-M3 Debug Monitor Interrupt             */
  PendSV_IRQn                   = -2,       /*!< 14 Cortex-M3 Pend SV Interrupt                   */
  SysTick_IRQn                  = -1,       /*!< 15 Cortex-M3 System Tick Interrupt               */

/******  LPC17xx Specific Interrupt Numbers *******************************************************/
  WDT_IRQn                      = 0,        /*!< Watchdog Timer Interrupt                         */
  TIMER0_IRQn                   = 1,        /*!< Timer0 Interrupt                                 */
  TIMER1_IRQn                   = 2,        /*!< Timer1 Interrupt                                 */
  TIMER2_IRQn                   = 3,        /*!< Timer2 Interrupt                                 */
  TIMER3_IRQn                   = 4,        /*!< Timer3 Interrupt                                 */
  UART0_IRQn                    = 5,        /*!< UART0 Interrupt                                  */
  UART1_IRQn                    = 6,        /*!< UART1 Interrupt                                  */
  UART2_IRQn                    = 7,        /*!< UART2 Interrupt                                  */
  UART3_IRQn                    = 8,        /*!< UART3 Interrupt                                  */
  PWM1_IRQn                     = 9,        /*!< PWM1 Interrupt                                   */
  I2C0_IRQn                     = 10,       /*!< I2C0 Interrupt                                   */
  I2C1_IRQn                     = 11,       /*!< I2C1 Interrupt                                   */
  I2C2_IRQn                     = 12,       /*!< I2C2 Interrupt                                   */
  SPI_IRQn                      = 13,       /*!< SPI Interrupt                                    */
  SSP0_IRQn                     = 14,       /*!< SSP0 Interrupt                                   */
  SSP1_IRQn                     = 15,       /*!< SSP1 Interrupt                                   */
  PLL0_IRQn                     = 16,       /*!< PLL0 Lock (Main PLL) Interrupt                   */
  RTC_IRQn                      = 17,       /*!< Real Time Clock Interrupt                        */
  EINT0_IRQn                    = 18,       /*!< External Interrupt 0 Interrupt                   */
  EINT1_IRQn                    = 19,       /*!< External Interrupt 1 Interrupt                   */
  EINT2_IRQn                    = 20,       /*!< External Interrupt 2 Interrupt                   */
  EINT3_IRQn                    = 21,       /*!< External Interrupt 3 Interrupt                   */
  ADC_IRQn                      = 22,       /*!< A/D Converter Interrupt                          */
  BOD_IRQn                      = 23,       /*!< Brown-Out Detect Interrupt                       */
  USB_IRQn                      = 24,       /*!< USB Interrupt                                    */
  CAN_IRQn                      = 25,       /*!< CAN Interrupt                                    */
  DMA_IRQn                      = 26,       /*!< General Purpose DMA Interrupt                    */
  I2S_IRQn                      = 27,       /*!< I2S Interrupt                                    */
  ENET_IRQn                     = 28,       /*!< Ethernet Interrupt                               */
  RIT_IRQn                      = 29,       /*!< Repetitive Interrupt Timer Interrupt             */
  MCPWM_IRQn                    = 30,       /*!< Motor Control PWM Interrupt                      */
  QEI_IRQn                      = 31,       /*!< Quadrature Encoder Interface Interrupt           */
  PLL1_IRQn                     = 32,       /*!< PLL1 Lock (USB PLL) Interrupt                    */
  USBActivity_IRQn              = 33,       /*!< USB Activity Interrupt(For wakeup only)          */
  CANActivity_IRQn              = 34        /*!< CAN Activity Interrupt(For wakeup only)          */
} IRQn_Type;


/*
 * ==========================================================================
 * ----------- Processor and Core Peripheral Section ------------------------
 * ==========================================================================
 */

/* Configuration of the Cortex-M3 Processor and Core Peripherals */
#define __MPU_PRESENT             1         /*!< MPU present or not                               */
#define __NVIC_PRIO_BITS          5         /*!< Number of Bits used for Priority Levels          */
#define __Vendor_SysTickConfig    0         /*!< Set to 1 if different SysTick Config is used     */


#include "core_cm3.h"                       /* Cortex-M3 processor and core peripherals           */
#include "system_LPC17xx.h"                 /* System Header                                      */


/******************************************************************************/
/*                Device Specific Peripheral registers structures             */
/******************************************************************************/

#if defined ( __CC_ARM   )
#pragma anon_unions
#endif

/*------------- System Control (SC) ------------------------------------------*/
/** @brief System Control (SC) register structure definition */
typedef struct
{
  __IO uint32_t FLASHCFG;                   /*!< Offset: 0x000 (R/W)  Flash Accelerator Configuration Register */
       uint32_t RESERVED0[31];
  __IO uint32_t PLL0CON;                    /*!< Offset: 0x080 (R/W)  PLL0 Control Register */
  __IO uint32_t PLL0CFG;                    /*!< Offset: 0x084 (R/W)  PLL0 Configuration Register */
  __I  uint32_t PLL0STAT;                   /*!< Offset: 0x088 (R/ )  PLL0 Status Register */
  __O  uint32_t PLL0FEED;                   /*!< Offset: 0x08C ( /W)  PLL0 Feed Register */
       uint32_t RESERVED1[4];
  __IO uint32_t PLL1CON;                    /*!< Offset: 0x0A0 (R/W)  PLL1 Control Register */
  __IO uint32_t PLL1CFG;                    /*!< Offset: 0x0A4 (R/W)  PLL1 Configuration Register */
  __I  uint32_t PLL1STAT;                   /*!< Offset: 0x0A8 (R/ )  PLL1 Status Register */
  __O  uint32_t PLL1FEED;                   /*!< Offset: 0x0AC ( /W)  PLL1 Feed Register */
       uint32_t RESERVED2[4];
  __IO uint32_t PCON;                       /*!< Offset: 0x0C0 (R/W)  Power Control Register */
  __IO uint32_t PCONP;                      /*!< Offset: 0x0C4 (R/W)  Power Control for Peripherals Register */
       uint32_t RESERVED3[15];
  __IO uint32_t CCLKCFG;                    /*!< Offset: 0x104 (R/W)  CPU Clock Configure Register  */
  __IO uint32_t USBCLKCFG;                  /*!< Offset: 0x108 (R/W)  USB Clock Configure Register */
  __IO uint32_t CLKSRCSEL;                  /*!< Offset: 0x10C (R/W)  Clock Source Select Register */
  __IO uint32_t CANSLEEPCLR;                /*!< Offset: 0x110 (R/W)  CAN Sleep Clear Register */
  __IO uint32_t CANWAKEFLAGS;               /*!< Offset: 0x114 (R/W)  CAN Wake-up Flags Register */
       uint32_t RESERVED4[10];
  __IO uint32_t EXTINT;                     /*!< Offset: 0x140 (R/W)  External Interrupt Flag Register */
       uint32_t RESERVED5[1];
  __IO uint32_t EXTMODE;                    /*!< Offset: 0x148 (R/W)  External Interrupt Mode Register */
  __IO uint32_t EXTPOLAR;                   /*!< Offset: 0x14C (R/W)  External Interrupt Polarity Register */
       uint32_t RESERVED6[12];
  __IO uint32_t RSID;                       /*!< Offset: 0x180 (R/W)  Reset Source Identification Register */
       uint32_t RESERVED7[7];
  __IO uint32_t SCS;                        /*!< Offset: 0x1A0 (R/W)  System Controls and Status Register */
  __IO uint32_t IRCTRIM;                /* Clock Dividers                     */
  __IO uint32_t PCLKSEL0;                   /*!< Offset: 0x1A8 (R/W)  Peripheral Clock Select 0 Register */
  __IO uint32_t PCLKSEL1;                   /*!< Offset: 0x1AC (R/W)  Peripheral Clock Select 1 Register */
       uint32_t RESERVED8[4];
  __IO uint32_t USBIntSt;                   /*!< Offset: 0x1C0 (R/W)  USB Interrupt Status Register */
  __IO uint32_t DMAREQSEL;                  /*!< Offset: 0x1C4 (R/W)  DMA Request Select Register */
  __IO uint32_t CLKOUTCFG;                  /*!< Offset: 0x1C8 (R/W)  Clock Output Configuration Register */

 } LPC_SC_TypeDef;

/*------------- Pin Connect Block (PINCON) -----------------------------------*/
/** @brief Pin Connect Block (PINCON) register structure definition */
typedef struct
{
  __IO uint32_t PINSEL0;				 /* !< Offset: 0x000 PIN Select0 (R/W) */
  __IO uint32_t PINSEL1;				 /* !< Offset: 0x004 PIN Select1 (R/W) */
  __IO uint32_t PINSEL2;				 /* !< Offset: 0x008 PIN Select2 (R/W) */
  __IO uint32_t PINSEL3;				 /* !< Offset: 0x00C PIN Select3 (R/W) */
  __IO uint32_t PINSEL4;				 /* !< Offset: 0x010 PIN Select4 (R/W) */
  __IO uint32_t PINSEL5;				 /* !< Offset: 0x014 PIN Select5 (R/W) */
  __IO uint32_t PINSEL6;				 /* !< Offset: 0x018 PIN Select6 (R/W) */
  __IO uint32_t PINSEL7;				 /* !< Offset: 0x01C PIN Select7 (R/W) */
  __IO uint32_t PINSEL8;				 /* !< Offset: 0x020 PIN Select8 (R/W) */
  __IO uint32_t PINSEL9;				 /* !< Offset: 0x024 PIN Select9 (R/W) */
  __IO uint32_t PINSEL10;				 /* !< Offset: 0x028 PIN Select20 (R/W) */
       uint32_t RESERVED0[5];
  __IO uint32_t PINMODE0;				 /* !< Offset: 0x040 PIN Mode0 (R/W) */
  __IO uint32_t PINMODE1;				 /* !< Offset: 0x044 PIN Mode1 (R/W) */
  __IO uint32_t PINMODE2;				 /* !< Offset: 0x048 PIN Mode2 (R/W) */
  __IO uint32_t PINMODE3;				 /* !< Offset: 0x04C PIN Mode3 (R/W) */
  __IO uint32_t PINMODE4;				 /* !< Offset: 0x050 PIN Mode4 (R/W) */
  __IO uint32_t PINMODE5;				 /* !< Offset: 0x054 PIN Mode5 (R/W) */
  __IO uint32_t PINMODE6;				 /* !< Offset: 0x058 PIN Mode6 (R/W) */
  __IO uint32_t PINMODE7;				 /* !< Offset: 0x05C PIN Mode7 (R/W) */
  __IO uint32_t PINMODE8;				 /* !< Offset: 0x060 PIN Mode8 (R/W) */
  __IO uint32_t PINMODE9;				 /* !< Offset: 0x064 PIN Mode9 (R/W) */
  __IO uint32_t PINMODE_OD0;			 /* !< Offset: 0x068 Open Drain PIN Mode0 (R/W) */
  __IO uint32_t PINMODE_OD1;			 /* !< Offset: 0x06C Open Drain PIN Mode1 (R/W) */
  __IO uint32_t PINMODE_OD2;			 /* !< Offset: 0x070 Open Drain PIN Mode2 (R/W) */
  __IO uint32_t PINMODE_OD3;			 /* !< Offset: 0x074 Open Drain PIN Mode3 (R/W) */
  __IO uint32_t PINMODE_OD4;			 /* !< Offset: 0x078 Open Drain PIN Mode4 (R/W) */
  __IO uint32_t I2CPADCFG;				 /* !< Offset: 0x07C I2C Pad Configure (R/W) */
} LPC_PINCON_TypeDef;

/*------------- General Purpose Input/Output (GPIO) --------------------------*/
/** @brief General Purpose Input/Output (GPIO) register structure definition */
typedef struct
{
  union {
    __IO uint32_t FIODIR;				 /* !< Offset: 0x00 Port direction (R/W) */
    struct {
      __IO uint16_t FIODIRL;
      __IO uint16_t FIODIRH;
    };
    struct {
      __IO uint8_t  FIODIR0;
      __IO uint8_t  FIODIR1;
      __IO uint8_t  FIODIR2;
      __IO uint8_t  FIODIR3;
    };
  };
  uint32_t RESERVED0[3];
  union {
    __IO uint32_t FIOMASK;				 /* !< Offset: 0x10 Port mask (R/W) */
    struct {
      __IO uint16_t FIOMASKL;
      __IO uint16_t FIOMASKH;
    };
    struct {
      __IO uint8_t  FIOMASK0;
      __IO uint8_t  FIOMASK1;
      __IO uint8_t  FIOMASK2;
      __IO uint8_t  FIOMASK3;
    };
  };
  union {
    __IO uint32_t FIOPIN;				 /* !< Offset: 0x14 Port value (R/W) */
    struct {
      __IO uint16_t FIOPINL;
      __IO uint16_t FIOPINH;
    };
    struct {
      __IO uint8_t  FIOPIN0;
      __IO uint8_t  FIOPIN1;
      __IO uint8_t  FIOPIN2;
      __IO uint8_t  FIOPIN3;
    };
  };
  union {
    __IO uint32_t FIOSET;				 /* !< Offset: 0x18 Port output set (R/W) */
    struct {
      __IO uint16_t FIOSETL;
      __IO uint16_t FIOSETH;
    };
    struct {
      __IO uint8_t  FIOSET0;
      __IO uint8_t  FIOSET1;
      __IO uint8_t  FIOSET2;
      __IO uint8_t  FIOSET3;
    };
  };
  union {
    __O  uint32_t FIOCLR;				 /* !< Offset: 0x1C Port output clear (R/W) */
    struct {
      __O  uint16_t FIOCLRL;
      __O  uint16_t FIOCLRH;
    };
    struct {
      __O  uint8_t  FIOCLR0;
      __O  uint8_t  FIOCLR1;
      __O  uint8_t  FIOCLR2;
      __O  uint8_t  FIOCLR3;
    };
  };
} LPC_GPIO_TypeDef;

/** @brief General Purpose Input/Output interrupt (GPIOINT) register structure definition */
typedef struct
{
  __I  uint32_t IntStatus;                  /*!< Offset: 0x000 (R/ )  GPIO overall Interrupt Status Register */
  __I  uint32_t IO0IntStatR;                /*!< Offset: 0x004 (R/ )  GPIO Interrupt Status Register 0 for Rising edge */
  __I  uint32_t IO0IntStatF;                /*!< Offset: 0x008 (R/ )  GPIO Interrupt Status Register 0 for Falling edge */
  __O  uint32_t IO0IntClr;                  /*!< Offset: 0x00C (R/W)  GPIO Interrupt Clear  Register 0 */
  __IO uint32_t IO0IntEnR;                  /*!< Offset: 0x010 ( /W)  GPIO Interrupt Enable Register 0 for Rising edge */
  __IO uint32_t IO0IntEnF;                  /*!< Offset: 0x014 (R/W)  GPIO Interrupt Enable Register 0 for Falling edge */
       uint32_t RESERVED0[3];
  __I  uint32_t IO2IntStatR;                /*!< Offset: 0x000 (R/ )  GPIO Interrupt Status Register 2 for Rising edge */
  __I  uint32_t IO2IntStatF;                /*!< Offset: 0x000 (R/ )  GPIO Interrupt Status Register 2 for Falling edge */
  __O  uint32_t IO2IntClr;                  /*!< Offset: 0x000 ( /W)  GPIO Interrupt Clear  Register 2 */
  __IO uint32_t IO2IntEnR;                  /*!< Offset: 0x000 (R/W)  GPIO Interrupt Enable Register 2 for Rising edge */
  __IO uint32_t IO2IntEnF;                  /*!< Offset: 0x000 (R/W)  GPIO Interrupt Enable Register 2 for Falling edge */
} LPC_GPIOINT_TypeDef;

/*------------- Timer (TIM) --------------------------------------------------*/
/** @brief Timer (TIM) register structure definition */
typedef struct
{
  __IO uint32_t IR;                         /*!< Offset: 0x000 (R/W)  Interrupt Register */
  __IO uint32_t TCR;                        /*!< Offset: 0x004 (R/W)  Timer Control Register */
  __IO uint32_t TC;                         /*!< Offset: 0x008 (R/W)  Timer Counter Register */
  __IO uint32_t PR;                         /*!< Offset: 0x00C (R/W)  Prescale Register */
  __IO uint32_t PC;                         /*!< Offset: 0x010 (R/W)  Prescale Counter Register */
  __IO uint32_t MCR;                        /*!< Offset: 0x014 (R/W)  Match Control Register */
  __IO uint32_t MR0;                        /*!< Offset: 0x018 (R/W)  Match Register 0 */
  __IO uint32_t MR1;                        /*!< Offset: 0x01C (R/W)  Match Register 1 */
  __IO uint32_t MR2;                        /*!< Offset: 0x020 (R/W)  Match Register 2 */
  __IO uint32_t MR3;                        /*!< Offset: 0x024 (R/W)  Match Register 3 */
  __IO uint32_t CCR;                        /*!< Offset: 0x028 (R/W)  Capture Control Register */
  __I  uint32_t CR0;                        /*!< Offset: 0x02C (R/ )  Capture Register 0 */
  __I  uint32_t CR1;                        /*!< Offset: 0x030 (R/ )  Capture Register */
       uint32_t RESERVED0[2];
  __IO uint32_t EMR;                        /*!< Offset: 0x03C (R/W)  External Match Register */
       uint32_t RESERVED1[12];
  __IO uint32_t CTCR;                       /*!< Offset: 0x070 (R/W)  Count Control Register */
} LPC_TIM_TypeDef;

/*------------- Pulse-Width Modulation (PWM) ---------------------------------*/
/** @brief Pulse-Width Modulation (PWM) register structure definition */
typedef struct
{
  __IO uint32_t IR;                         /*!< Offset: 0x000 (R/W)  Interrupt Register */
  __IO uint32_t TCR;                        /*!< Offset: 0x004 (R/W)  Timer Control Register. Register */
  __IO uint32_t TC;                         /*!< Offset: 0x008 (R/W)  Timer Counter Register */
  __IO uint32_t PR;                         /*!< Offset: 0x00C (R/W)  Prescale Register */
  __IO uint32_t PC;                         /*!< Offset: 0x010 (R/W)  Prescale Counter Register */
  __IO uint32_t MCR;                        /*!< Offset: 0x014 (R/W)  Match Control Register */
  __IO uint32_t MR0;                        /*!< Offset: 0x018 (R/W)  Match Register 0 */
  __IO uint32_t MR1;                        /*!< Offset: 0x01C (R/W)  Match Register 1 */
  __IO uint32_t MR2;                        /*!< Offset: 0x020 (R/W)  Match Register 2 */
  __IO uint32_t MR3;                        /*!< Offset: 0x024 (R/W)  Match Register 3 */
  __IO uint32_t CCR;                        /*!< Offset: 0x028 (R/W)  Capture Control Register */
  __I  uint32_t CR0;                        /*!< Offset: 0x02C (R/ )  Capture Register 0 */
  __I  uint32_t CR1;                        /*!< Offset: 0x030 (R/ )  Capture Register 1 */
  __I  uint32_t CR2;                        /*!< Offset: 0x034 (R/ )  Capture Register 2 */
  __I  uint32_t CR3;                        /*!< Offset: 0x038 (R/ )  Capture Register 3 */
       uint32_t RESERVED0;
  __IO uint32_t MR4;                        /*!< Offset: 0x040 (R/W)  Match Register 4 */
  __IO uint32_t MR5;                        /*!< Offset: 0x044 (R/W)  Match Register 5 */
  __IO uint32_t MR6;                        /*!< Offset: 0x048 (R/W)  Match Register 6 */
  __IO uint32_t PCR;                        /*!< Offset: 0x04C (R/W)  PWM Control Register */
  __IO uint32_t LER;                        /*!< Offset: 0x050 (R/W)  Load Enable Register */
       uint32_t RESERVED1[7];
  __IO uint32_t CTCR;                       /*!< Offset: 0x070 (R/W)  Count Control Register */
} LPC_PWM_TypeDef;

/*------------- Universal Asynchronous Receiver Transmitter (UART) -----------*/
/** @brief  Universal Asynchronous Receiver Transmitter (UART) register structure definition */
typedef struct
{
  union {
  __I  uint32_t RBR;                   /*!< Offset: 0x000 Receiver Buffer  Register (R/ ) */
  __O  uint32_t THR;                   /*!< Offset: 0x000 Transmit Holding Register ( /W) */
  __IO uint32_t DLL;                   /*!< Offset: 0x000 Divisor Latch LSB (R/W) */
  };
  union {
  __IO uint32_t DLM;                   /*!< Offset: 0x004 Divisor Latch MSB (R/W) */
  __IO uint32_t IER;                   /*!< Offset: 0x004 Interrupt Enable Register (R/W) */
  };
  union {
  __I  uint32_t IIR;                   /*!< Offset: 0x008 Interrupt ID Register (R/ ) */
  __O  uint32_t FCR;                   /*!< Offset: 0x008 FIFO Control Register ( /W) */
  };
  __IO uint32_t LCR;                   /*!< Offset: 0x00C Line Control Register (R/W) */
       uint32_t RESERVED0;
  __I  uint32_t LSR;                   /*!< Offset: 0x014 Line Status Register (R/ ) */
       uint32_t RESERVED1;
  __IO uint32_t SCR;                   /*!< Offset: 0x01C Scratch Pad Register (R/W) */
  __IO uint32_t ACR;                   /*!< Offset: 0x020 Auto-baud Control Register (R/W) */
  __IO uint32_t ICR;                   /*!< Offset: 0x024 IrDA Control Register (R/W) */
  __IO uint32_t FDR;                   /*!< Offset: 0x028 Fractional Divider Register (R/W) */
       uint32_t RESERVED2;
  __IO uint32_t TER;                   /*!< Offset: 0x030 Transmit Enable Register (R/W) */
} LPC_UART_TypeDef;

/** @brief  Universal Asynchronous Receiver Transmitter 0 (UART0) register structure definition */
typedef struct
{
  union {
  __I  uint32_t  RBR;                   /*!< Offset: 0x000 Receiver Buffer  Register (R/ ) */
  __O  uint32_t  THR;                   /*!< Offset: 0x000 Transmit Holding Register ( /W) */
  __IO uint32_t  DLL;                   /*!< Offset: 0x000 Divisor Latch LSB (R/W) */
  };
  union {
  __IO uint32_t  DLM;                   /*!< Offset: 0x004 Divisor Latch MSB (R/W) */
  __IO uint32_t  IER;                   /*!< Offset: 0x000 Interrupt Enable Register (R/W) */
  };
  union {
  __I  uint32_t  IIR;                   /*!< Offset: 0x008 Interrupt ID Register (R/ ) */
  __O  uint32_t  FCR;                   /*!< Offset: 0x008 FIFO Control Register ( /W) */
  };
  __IO uint32_t  LCR;                   /*!< Offset: 0x00C Line Control Register (R/W) */
  __IO uint32_t  MCR;                   /*!< Offset: 0x010 Modem control Register (R/W) */
  __I  uint32_t  LSR;                   /*!< Offset: 0x014 Line Status Register (R/ ) */
  __I  uint32_t  MSR;                   /*!< Offset: 0x018 Modem status Register (R/ ) */
  __IO uint32_t  SCR;                   /*!< Offset: 0x01C Scratch Pad Register (R/W) */
  __IO uint32_t  ACR;                   /*!< Offset: 0x020 Auto-baud Control Register (R/W) */
       uint32_t  RESERVED0;
  __IO uint32_t  FDR;                   /*!< Offset: 0x028 Fractional Divider Register (R/W) */
       uint32_t  RESERVED1;
  __IO uint32_t  TER;                   /*!< Offset: 0x030 Transmit Enable Register (R/W) */
       uint32_t  RESERVED2[6];
  __IO uint32_t  RS485CTRL;             /*!< Offset: 0x04C RS-485/EIA-485 Control Register (R/W) */
  __IO uint32_t  ADRMATCH;              /*!< Offset: 0x050 RS-485/EIA-485 address match Register (R/W) */
  __IO uint32_t  RS485DLY;              /*!< Offset: 0x054 RS-485/EIA-485 direction control delay Register (R/W) */
} LPC_UART1_TypeDef;

/*------------- Serial Peripheral Interface (SPI) ----------------------------*/
/** @brief  Serial Peripheral Interface (SPI) register structure definition */
typedef struct
{
  __IO uint32_t SPCR;                       /*!< Offset: 0x000 SPI Control Register (R/W) */
  __I  uint32_t SPSR;                       /*!< Offset: 0x004 SPI Status Register (R/) */
  __IO uint32_t SPDR;                       /*!< Offset: 0x008 SPI Data Register (R/W) */
  __IO uint32_t SPCCR;                      /*!< Offset: 0x00C SPI Clock Counter Register (R/W) */
       uint32_t RESERVED0[3];
  __IO uint32_t SPINT;                      /*!< Offset: 0x01C SPI Interrupt Flag Register (R/W) */
} LPC_SPI_TypeDef;

/*------------- Synchronous Serial Communication (SSP) -----------------------*/
/** @brief  Synchronous Serial Communication (SSP) register structure definition */
typedef struct
{
  __IO uint32_t CR0;                        /*!< Offset: 0x000 (R/W)  Control Register 0 */
  __IO uint32_t CR1;                        /*!< Offset: 0x004 (R/W)  Control Register 1 */
  __IO uint32_t DR;                         /*!< Offset: 0x008 (R/W)  Data Register */
  __I  uint32_t SR;                         /*!< Offset: 0x00C (R/ )  Status Register */
  __IO uint32_t CPSR;                       /*!< Offset: 0x010 (R/W)  Clock Prescale Register */
  __IO uint32_t IMSC;                       /*!< Offset: 0x014 (R/W)  Interrupt Mask Set and Clear Register */
  __IO uint32_t RIS;                        /*!< Offset: 0x018 (R/W)  Raw Interrupt Status Register */
  __IO uint32_t MIS;                        /*!< Offset: 0x01C (R/W)  Masked Interrupt Status Register */
  __IO uint32_t ICR;                        /*!< Offset: 0x020 (R/W)  SSPICR Interrupt Clear Register */
  __IO uint32_t DMACR;                      /*!< Offset: 0x024 (R/W)  DMA Control Register */
} LPC_SSP_TypeDef;

/*------------- Inter-Integrated Circuit (I2C) -------------------------------*/
/** @brief  Inter-Integrated Circuit (I2C) register structure definition */
typedef struct
{
  __IO uint32_t CONSET;                     /*!< Offset: 0x000 (R/W)  I2C Control Set Register */
  __I  uint32_t STAT;                       /*!< Offset: 0x004 (R/ )  I2C Status Register */
  __IO uint32_t DAT;                        /*!< Offset: 0x008 (R/W)  I2C Data Register */
  __IO uint32_t ADR0;                       /*!< Offset: 0x00C (R/W)  I2C Slave Address Register 0 */
  __IO uint32_t SCLH;                       /*!< Offset: 0x010 (R/W)  SCH Duty Cycle Register High Half Word */
  __IO uint32_t SCLL;                       /*!< Offset: 0x014 (R/W)  SCL Duty Cycle Register Low Half Word */
  __O  uint32_t CONCLR;                     /*!< Offset: 0x018 (R/W)  I2C Control Clear Register */
  __IO uint32_t MMCTRL;                     /*!< Offset: 0x01C (R/W)  Monitor mode control register */
  __IO uint32_t ADR1;                       /*!< Offset: 0x020 (R/W)  I2C Slave Address Register 1 */
  __IO uint32_t ADR2;                       /*!< Offset: 0x024 (R/W)  I2C Slave Address Register 2 */
  __IO uint32_t ADR3;                       /*!< Offset: 0x028 (R/W)  I2C Slave Address Register 3 */
  __I  uint32_t DATA_BUFFER;                /*!< Offset: 0x02C (R/ )  Data buffer Register */
  __IO uint32_t MASK0;                      /*!< Offset: 0x030 (R/W)  I2C Slave address mask register 0 */
  __IO uint32_t MASK1;                      /*!< Offset: 0x034 (R/W)  I2C Slave address mask register 1 */
  __IO uint32_t MASK2;                      /*!< Offset: 0x038 (R/W)  I2C Slave address mask register 2 */
  __IO uint32_t MASK3;                      /*!< Offset: 0x03C (R/W)  I2C Slave address mask register 3 */
} LPC_I2C_TypeDef;

/*------------- Inter IC Sound (I2S) -----------------------------------------*/
/** @brief  Inter IC Sound (I2S) register structure definition */
typedef struct
{
  __IO uint32_t DAO;                        /*!< Offset: 0x000 (R/W)  Digital Audio Output Register */
  __IO uint32_t DAI;                        /*!< Offset: 0x004 (R/W)  Digital Audio Input Register */
  __O  uint32_t TXFIFO;                     /*!< Offset: 0x008 ( /W)  Transmit FIFO */
  __I  uint32_t RXFIFO;                     /*!< Offset: 0x00C (R/ )  Receive FIFO */
  __I  uint32_t STATE;                      /*!< Offset: 0x010 (R/W)  Status Feedback Register */
  __IO uint32_t DMA1;                       /*!< Offset: 0x014 (R/W)  DMA Configuration Register 1 */
  __IO uint32_t DMA2;                       /*!< Offset: 0x018 (R/W)  DMA Configuration Register 2 */
  __IO uint32_t IRQ;                        /*!< Offset: 0x01C (R/W)  Interrupt Request Control Register */
  __IO uint32_t TXRATE;                     /*!< Offset: 0x020 (R/W)  Transmit reference clock divider Register */
  __IO uint32_t RXRATE;                     /*!< Offset: 0x024 (R/W)  Receive reference clock divider Register */
  __IO uint32_t TXBITRATE;                  /*!< Offset: 0x028 (R/W)  Transmit bit rate divider Register */
  __IO uint32_t RXBITRATE;                  /*!< Offset: 0x02C (R/W)  Receive bit rate divider Register */
  __IO uint32_t TXMODE;                     /*!< Offset: 0x030 (R/W)  Transmit mode control Register */
  __IO uint32_t RXMODE;                     /*!< Offset: 0x034 (R/W)  Receive mode control Register */
} LPC_I2S_TypeDef;

/*------------- Repetitive Interrupt Timer (RIT) -----------------------------*/
/** @brief  Repetitive Interrupt Timer (RIT) register structure definition */
typedef struct
{
  __IO uint32_t RICOMPVAL;
  __IO uint32_t RIMASK;
  __IO uint32_t RICTRL;
  __IO uint32_t RICOUNTER;
} LPC_RIT_TypeDef;

/*------------- Real-Time Clock (RTC) ----------------------------------------*/
/** @brief  Real-Time Clock (RTC) register structure definition */
typedef struct
{
  __IO uint32_t ILR;                        /*!< Offset: 0x000 (R/W)  Interrupt Location Register */
       uint32_t RESERVED0;				    
  __IO uint32_t CCR;                        /*!< Offset: 0x008 (R/W)  Clock Control Register */
  __IO uint32_t CIIR;                       /*!< Offset: 0x00C (R/W)  Counter Increment Interrupt Register */
  __IO uint32_t AMR;                        /*!< Offset: 0x010 (R/W)  Alarm Mask Register */
  __I  uint32_t CTIME0;                     /*!< Offset: 0x014 (R/ )  Consolidated Time Register 0 */
  __I  uint32_t CTIME1;                     /*!< Offset: 0x018 (R/ )  Consolidated Time Register 1 */
  __I  uint32_t CTIME2;                     /*!< Offset: 0x01C (R/ )  Consolidated Time Register 2 */
  __IO uint32_t SEC;                        /*!< Offset: 0x020 (R/W)  Seconds Counter Register */
  __IO uint32_t MIN;                        /*!< Offset: 0x024 (R/W)  Minutes Register */
  __IO uint32_t HOUR;                       /*!< Offset: 0x028 (R/W)  Hours Register */
  __IO uint32_t DOM;                        /*!< Offset: 0x02C (R/W)  Day of Month Register */
  __IO uint32_t DOW;                        /*!< Offset: 0x030 (R/W)  Day of Week Register */
  __IO uint32_t DOY;                        /*!< Offset: 0x034 (R/W)  Day of Year Register */
  __IO uint32_t MONTH;                      /*!< Offset: 0x038 (R/W)  Months Register */
  __IO uint32_t YEAR;                       /*!< Offset: 0x03C (R/W)  Years Register */
  __IO uint32_t CALIBRATION;                /*!< Offset: 0x040 (R/W)  Calibration Value Register */
  __IO uint32_t GPREG0;                     /*!< Offset: 0x044 (R/W)  General Purpose Register 0 */
  __IO uint32_t GPREG1;                     /*!< Offset: 0x048 (R/W)  General Purpose Register 1 */
  __IO uint32_t GPREG2;                     /*!< Offset: 0x04C (R/W)  General Purpose Register 2 */
  __IO uint32_t GPREG3;                     /*!< Offset: 0x050 (R/W)  General Purpose Register 3 */
  __IO uint32_t GPREG4;                     /*!< Offset: 0x054 (R/W)  General Purpose Register 4 */
  __IO uint32_t RTC_AUXEN;                  /*!< Offset: 0x058 (R/W)  RTC Auxiliary Enable Register */
  __IO uint32_t RTC_AUX;                    /*!< Offset: 0x05C (R/W)  RTC Auxiliary Control Register */
  __IO uint32_t ALSEC;                      /*!< Offset: 0x060 (R/W)  Alarm value for Seconds */
  __IO uint32_t ALMIN;                      /*!< Offset: 0x064 (R/W)  Alarm value for Minutes */
  __IO uint32_t ALHOUR;                     /*!< Offset: 0x068 (R/W)  Alarm value for Hours */
  __IO uint32_t ALDOM;                      /*!< Offset: 0x06C (R/W)  Alarm value for Day of Month */
  __IO uint32_t ALDOW;                      /*!< Offset: 0x070 (R/W)  Alarm value for Day of Week */
  __IO uint32_t ALDOY;                      /*!< Offset: 0x074 (R/W)  Alarm value for Day of Year */
  __IO uint32_t ALMON;                      /*!< Offset: 0x078 (R/W)  Alarm value for Months */
  __IO uint32_t ALYEAR;                     /*!< Offset: 0x07C (R/W)  Alarm value for Year */
} LPC_RTC_TypeDef;

/*------------- Watchdog Timer (WDT) -----------------------------------------*/
/** @brief  Watchdog Timer (WDT) register structure definition */
typedef struct
{
  __IO uint32_t MOD;                        /*!< Offset: 0x000 (R/W)  Watchdog mode Register */
  __IO uint32_t TC;                         /*!< Offset: 0x004 (R/W)  Watchdog timer constant Register */
  __O  uint32_t FEED;                       /*!< Offset: 0x008 ( /W)  Watchdog feed sequence Register */
  __I  uint32_t TV;                         /*!< Offset: 0x00C (R/ )  Watchdog timer value Register */
  __IO uint32_t WDCLKSEL;
} LPC_WDT_TypeDef;

/*------------- Analog-to-Digital Converter (ADC) ----------------------------*/
/** @brief  Analog-to-Digital Converter (ADC) register structure definition */
typedef struct
{
  __IO uint32_t CR;                         /*!< Offset: 0x000 (R/W)  A/D Control Register */
  __IO uint32_t GDR;                        /*!< Offset: 0x004 (R/W)  A/D Global Data Register */
       uint32_t RESERVED0;
  __IO uint32_t INTEN;                      /*!< Offset: 0x00C (R/W)  A/D Interrupt Enable Register */
  __I  uint32_t DR[8];                      /*!< Offset: 0x010 (R/ )  A/D Channel # Data Register */
  __I  uint32_t STAT;                       /*!< Offset: 0x030 (R/ )  A/D Status Register */
  __IO uint32_t ADTRM;                      /*!< Offset: 0x034 (R/W)  ADC trim Register */
} LPC_ADC_TypeDef;

/*------------- Digital-to-Analog Converter (DAC) ----------------------------*/
/** @brief  Digital-to-Analog Converter (DAC) register structure definition */
typedef struct
{
  __IO uint32_t CR;                         /*!< Offset: 0x000 (R/W)  D/A Converter Register */
  __IO uint32_t CTRL;                       /*!< Offset: 0x004 (R/W)  DAC Control register */
  __IO uint32_t CNTVAL;                     /*!< Offset: 0x008 (R/W)  DAC Counter Value Register */
} LPC_DAC_TypeDef;

/*------------- Motor Control Pulse-Width Modulation (MCPWM) -----------------*/
/** @brief  Motor Control Pulse-Width Modulation (MCPWM) register structure definition */
typedef struct
{
  __I  uint32_t CON;                        /*!< Offset: 0x000 (R/ )  PWM Control read address Register */
  __O  uint32_t CON_SET;                    /*!< Offset: 0x004 ( /W)  PWM Control set address Register */
  __O  uint32_t CON_CLR;                    /*!< Offset: 0x008 ( /W)  PWM Control clear address Register */
  __I  uint32_t CAPCON;                     /*!< Offset: 0x00C (R/ )  Capture Control read address Register */
  __O  uint32_t CAPCON_SET;                 /*!< Offset: 0x010 ( /W)  Capture Control set address Register */
  __O  uint32_t CAPCON_CLR;                 /*!< Offset: 0x014 ( /W)  Event Control clear address Register */
  __IO uint32_t TC0;                        /*!< Offset: 0x018 (R/W)  Timer Counter Register, channel 0 */
  __IO uint32_t TC1;                        /*!< Offset: 0x01C (R/W)  Timer Counter Register, channel 1 */
  __IO uint32_t TC2;                        /*!< Offset: 0x020 (R/W)  Timer Counter Register, channel 2 */
  __IO uint32_t LIM0;                       /*!< Offset: 0x024 (R/W)  Limit Register, channel 0 */
  __IO uint32_t LIM1;                       /*!< Offset: 0x028 (R/W)  Limit Register, channel 1 */
  __IO uint32_t LIM2;                       /*!< Offset: 0x02C (R/W)  Limit Register, channel 2 */
  __IO uint32_t MAT0;                       /*!< Offset: 0x030 (R/W)  Match Register, channel 0 */
  __IO uint32_t MAT1;                       /*!< Offset: 0x034 (R/W)  Match Register, channel 1 */
  __IO uint32_t MAT2;                       /*!< Offset: 0x038 (R/W)  Match Register, channel 2 */
  __IO uint32_t DT;                         /*!< Offset: 0x03C (R/W)  Dead time Register */
  __IO uint32_t CP;                         /*!< Offset: 0x040 (R/W)  Commutation Pattern Register */
  __IO uint32_t CAP0;                       /*!< Offset: 0x044 (R/W)  Capture Register, channel 0 */
  __IO uint32_t CAP1;                       /*!< Offset: 0x048 (R/W)  Capture Register, channel 1 */
  __IO uint32_t CAP2;                       /*!< Offset: 0x04C (R/W)  Capture Register, channel 2 */
  __I  uint32_t INTEN;                      /*!< Offset: 0x050 (R/ )  Interrupt Enable read Register */
  __O  uint32_t INTEN_SET;                  /*!< Offset: 0x054 ( /W)  Interrupt Enable set address Register */
  __O  uint32_t INTEN_CLR;                  /*!< Offset: 0x058 ( /W)  Interrupt Enable clear address Register */
  __I  uint32_t CNTCON;                     /*!< Offset: 0x05C (R/ )  Count Control read address Register */
  __O  uint32_t CNTCON_SET;                 /*!< Offset: 0x060 ( /W)  Count Control set address Register */
  __O  uint32_t CNTCON_CLR;                 /*!< Offset: 0x064 ( /W)  Count Control clear address Register */
  __I  uint32_t INTF;                       /*!< Offset: 0x068 (R/ )  Interrupt flags read address Register */
  __O  uint32_t INTF_SET;                   /*!< Offset: 0x06C ( /W)  Interrupt flags set address Register */
  __O  uint32_t INTF_CLR;                   /*!< Offset: 0x070 ( /W)  Interrupt flags clear address Register */
  __O  uint32_t CAP_CLR;                    /*!< Offset: 0x074 ( /W)  Capture clear address Register */
} LPC_MCPWM_TypeDef;

/*------------- Quadrature Encoder Interface (QEI) ---------------------------*/
/** @brief  Quadrature Encoder Interface (QEI) register structure definition */
typedef struct
{
  __O  uint32_t CON;                        /*!< Offset: 0x000 ( /W)  Control Register */
  __I  uint32_t STAT;                       /*!< Offset: 0x004 (R/ )  Encoder Status Register */
  __IO uint32_t CONF;                       /*!< Offset: 0x008 (R/W)  Configuration Register */
  __I  uint32_t POS;                        /*!< Offset: 0x00C (R/ )  Position Register */
  __IO uint32_t MAXPOS;                     /*!< Offset: 0x010 (R/W)  Maximum position Register */
  __IO uint32_t CMPOS0;                     /*!< Offset: 0x014 (R/W)  Position compare Register 0 */
  __IO uint32_t CMPOS1;                     /*!< Offset: 0x018 (R/W)  Position compare Register 1 */
  __IO uint32_t CMPOS2;                     /*!< Offset: 0x01C (R/W)  Position compare Register 2 */
  __I  uint32_t INXCNT;                     /*!< Offset: 0x020 (R/ )  Index count Register */
  __IO uint32_t INXCMP0;                    /*!< Offset: 0x024 (R/W)  Index compare Register 0 */
  __IO uint32_t LOAD;                       /*!< Offset: 0x028 (R/W)  Velocity timer reload Register */
  __I  uint32_t TIME;                       /*!< Offset: 0x02C (R/ )  Velocity timer Register */
  __I  uint32_t VEL;                        /*!< Offset: 0x030 (R/ )  Velocity counter Register */
  __I  uint32_t CAP;                        /*!< Offset: 0x034 (R/ )  Velocity capture Register */
  __IO uint32_t VELCOMP;                    /*!< Offset: 0x038 (R/W)  Velocity compare Register */
  __IO uint32_t FILTER;
       uint32_t RESERVED0[998];
  __O  uint32_t IEC;                        /*!< Offset: 0xFD8 ( /W)  Interrupt enable clear Register */
  __O  uint32_t IES;                        /*!< Offset: 0xFDC ( /W)  Interrupt enable set Register */
  __I  uint32_t INTSTAT;                    /*!< Offset: 0xFE0 (R/ )  Interrupt status Register */
  __I  uint32_t IE;                         /*!< Offset: 0xFE4 (R/ )  Interrupt enable Register */
  __O  uint32_t CLR;                        /*!< Offset: 0xFE8 ( /W)  Interrupt status clear Register */
  __O  uint32_t SET;                        /*!< Offset: 0xFEC ( /W)  Interrupt status set Register */
} LPC_QEI_TypeDef;

/*------------- Controller Area Network (CAN) --------------------------------*/
/** @brief  Controller Area Network Acceptance Filter RAM (CANAF_RAM)structure definition */
typedef struct
{
  __IO uint32_t mask[512];                  /*!< Offset: 0x000 (R/W)  Acceptance Filter RAM */
} LPC_CANAF_RAM_TypeDef;

/** @brief  Controller Area Network Acceptance Filter(CANAF) register structure definition */
typedef struct                          /* Acceptance Filter Registers        */
{
  __IO uint32_t AFMR;                       /*!< Offset: 0x000 (R/W)  Acceptance Filter Register */
  __IO uint32_t SFF_sa;                     /*!< Offset: 0x004 (R/W)  Standard Frame Individual Start Address Register */
  __IO uint32_t SFF_GRP_sa;                 /*!< Offset: 0x008 (R/W)  Standard Frame Group Start Address Register */
  __IO uint32_t EFF_sa;                     /*!< Offset: 0x00C (R/W)  Extended Frame Start Address Register */
  __IO uint32_t EFF_GRP_sa;                 /*!< Offset: 0x010 (R/W)  Extended Frame Group Start Address Register */
  __IO uint32_t ENDofTable;                 /*!< Offset: 0x014 (R/W)  End of AF Tables Register */
  __I  uint32_t LUTerrAd;                   /*!< Offset: 0x018 (R/ )  LUT Error Address Register */
  __I  uint32_t LUTerr;                     /*!< Offset: 0x01C (R/ )  LUT Error Register */
  __IO uint32_t FCANIE;                     /*!< Offset: 0x020 (R/W)  Global FullCANInterrupt Enable Register */
  __IO uint32_t FCANIC0;                    /*!< Offset: 0x024 (R/W)  FullCAN Interrupt and Capture Register 0 */
  __IO uint32_t FCANIC1;                    /*!< Offset: 0x028 (R/W)  FullCAN Interrupt and Capture Register 1 */
} LPC_CANAF_TypeDef;

/** @brief  Controller Area Network Central (CANCR) register structure definition */
typedef struct                          /* Central Registers                  */
{
  __I  uint32_t TxSR;                       /*!< Offset: 0x000 (R/ )  CAN Central Transmit Status Register */
  __I  uint32_t RxSR;                       /*!< Offset: 0x004 (R/ )  CAN Central Receive Status Register */
  __I  uint32_t MSR;                        /*!< Offset: 0x008 (R/ )  CAN Central Miscellaneous Register */
} LPC_CANCR_TypeDef;

/** @brief  Controller Area Network Controller (CAN) register structure definition */
typedef struct                              /* Controller Registers               */
{
  __IO uint32_t MOD;                        /*!< Offset: 0x000 (R/W)  CAN Mode Register */
  __O  uint32_t CMR;                        /*!< Offset: 0x004 ( /W)  CAN Command Register */
  __IO uint32_t GSR;                        /*!< Offset: 0x008 (R/W)  CAN Global Status Register */
  __I  uint32_t ICR;                        /*!< Offset: 0x00C (R/ )  CAN Interrupt and Capture Register */
  __IO uint32_t IER;                        /*!< Offset: 0x010 (R/W)  CAN Interrupt Enable Register */
  __IO uint32_t BTR;                        /*!< Offset: 0x014 (R/W)  CAN Bus Timing Register */
  __IO uint32_t EWL;                        /*!< Offset: 0x018 (R/W)  CAN Error Warning Limit Register */
  __I  uint32_t SR;                         /*!< Offset: 0x01C (R/ )  CAN Status Register */
  __IO uint32_t RFS;                        /*!< Offset: 0x020 (R/W)  CAN Receive Frame Status Register */
  __IO uint32_t RID;                        /*!< Offset: 0x024 (R/W)  CAN Receive Identifier Register */
  __IO uint32_t RDA;                        /*!< Offset: 0x028 (R/W)  CAN Receive Data Register A */
  __IO uint32_t RDB;                        /*!< Offset: 0x02C (R/W)  CAN Receive Data Register B */
  __IO uint32_t TFI1;                       /*!< Offset: 0x030 (R/W)  CAN Transmit Frame Information Register 1 */
  __IO uint32_t TID1;                       /*!< Offset: 0x034 (R/W)  CAN Transmit Identifier Register 1 */
  __IO uint32_t TDA1;                       /*!< Offset: 0x038 (R/W)  CAN Transmit Data Register A 1 */
  __IO uint32_t TDB1;                       /*!< Offset: 0x03C (R/W)  CAN Transmit Data Register B 1 */
  __IO uint32_t TFI2;                       /*!< Offset: 0x040 (R/W)  CAN Transmit Frame Information Register 2 */
  __IO uint32_t TID2;                       /*!< Offset: 0x044 (R/W)  CAN Transmit Identifier Register 2 */
  __IO uint32_t TDA2;                       /*!< Offset: 0x048 (R/W)  CAN Transmit Data Register A 2 */
  __IO uint32_t TDB2;                       /*!< Offset: 0x04C (R/W)  CAN Transmit Data Register B 2 */
  __IO uint32_t TFI3;                       /*!< Offset: 0x050 (R/W)  CAN Transmit Frame Information Register 3 */
  __IO uint32_t TID3;                       /*!< Offset: 0x054 (R/W)  CAN Transmit Identifier Register 3 */
  __IO uint32_t TDA3;                       /*!< Offset: 0x058 (R/W)  CAN Transmit Data Register A 3 */
  __IO uint32_t TDB3;                       /*!< Offset: 0x05C (R/W)  CAN Transmit Data Register B 3 */
} LPC_CAN_TypeDef;

/*------------- General Purpose Direct Memory Access (GPDMA) -----------------*/
/** @brief  General Purpose Direct Memory Access (GPDMA) register structure definition */
typedef struct                              /* Common Registers                   */
{
  __I  uint32_t IntStat;                    /*!< Offset: 0x000 (R/ )  DMA Interrupt Status Register */
  __I  uint32_t IntTCStat;                  /*!< Offset: 0x004 (R/ )  DMA Interrupt Terminal Count Request Status Register */
  __O  uint32_t IntTCClear;                 /*!< Offset: 0x008 ( /W)  DMA Interrupt Terminal Count Request Clear Register */
  __I  uint32_t IntErrStat;                 /*!< Offset: 0x00C (R/ )  DMA Interrupt Error Status Register */
  __O  uint32_t IntErrClr;                  /*!< Offset: 0x010 ( /W)  DMA Interrupt Error Clear Register */
  __I  uint32_t RawIntTCStat;               /*!< Offset: 0x014 (R/ )  DMA Raw Interrupt Terminal Count Status Register */
  __I  uint32_t RawIntErrStat;              /*!< Offset: 0x018 (R/ )  DMA Raw Error Interrupt Status Register */
  __I  uint32_t EnbldChns;                  /*!< Offset: 0x01C (R/ )  DMA Enabled Channel Register */
  __IO uint32_t SoftBReq;                   /*!< Offset: 0x020 (R/W)  DMA Software Burst Request Register */
  __IO uint32_t SoftSReq;                   /*!< Offset: 0x024 (R/W)  DMA Software Single Request Register */
  __IO uint32_t SoftLBReq;                  /*!< Offset: 0x028 (R/W)  DMA Software Last Burst Request Register */
  __IO uint32_t SoftLSReq;                  /*!< Offset: 0x02C (R/W)  DMA Software Last Single Request Register */
  __IO uint32_t Config;                     /*!< Offset: 0x030 (R/W)  DMA Configuration Register */
  __IO uint32_t Sync;                       /*!< Offset: 0x034 (R/W)  DMA Synchronization Register */
} LPC_GPDMA_TypeDef;

/** @brief  General Purpose Direct Memory Access Channel (GPDMACH) register structure definition */
typedef struct                              /* Channel Registers                  */
{
  __IO uint32_t CSrcAddr;                   /*!< Offset: 0x000 (R/W)  DMA Channel # Source Address Register */
  __IO uint32_t CDestAddr;                  /*!< Offset: 0x004 (R/W)  DMA Channel # Destination Address Register */
  __IO uint32_t CLLI;                       /*!< Offset: 0x008 (R/W)  DMA Channel # Linked List Item Register */
  __IO uint32_t CControl;                   /*!< Offset: 0x00C (R/W)  DMA Channel # Control Register */
  __IO uint32_t CConfig;                    /*!< Offset: 0x010 (R/W)  DMA Channel # Configuration Register */
} LPC_GPDMACH_TypeDef;

/*------------- Universal Serial Bus (USB) -----------------------------------*/
/** @brief  Universal Serial Bus (USB) register structure definition */
typedef struct
{
  __I  uint32_t Revision;                 /*!< Offset: 0x000 (R/ )  Revision Register */
  __IO uint32_t Control;                  /*!< Offset: 0x004 (R/W)  Control Register */
  __IO uint32_t CommandStatus;            /*!< Offset: 0x008 (R/W)  Command / Status Register */
  __IO uint32_t InterruptStatus;          /*!< Offset: 0x00C (R/W)  Interrupt Status Register */
  __IO uint32_t InterruptEnable;          /*!< Offset: 0x010 (R/W)  Interrupt Enable Register */
  __IO uint32_t InterruptDisable;         /*!< Offset: 0x014 (R/W)  Interrupt Disable Register */
  __IO uint32_t HCCA;                     /*!< Offset: 0x018 (R/W)  Host Controller communication Area Register */
  __I  uint32_t PeriodCurrentED;          /*!< Offset: 0x01C (R/ )   Register */
  __IO uint32_t ControlHeadED;            /*!< Offset: 0x020 (R/W)   Register */
  __IO uint32_t ControlCurrentED;         /*!< Offset: 0x024 (R/W)   Register */
  __IO uint32_t BulkHeadED;               /*!< Offset: 0x028 (R/W)   Register */
  __IO uint32_t BulkCurrentED;            /*!< Offset: 0x02C (R/W)   Register */
  __I  uint32_t DoneHead;                 /*!< Offset: 0x030 (R/ )   Register */
  __IO uint32_t FmInterval;               /*!< Offset: 0x034 (R/W)   Register */
  __I  uint32_t FmRemaining;              /*!< Offset: 0x038 (R/ )   Register */
  __I  uint32_t FmNumber;                 /*!< Offset: 0x03C (R/ )   Register */
  __IO uint32_t PeriodicStart;            /*!< Offset: 0x040 (R/W)   Register */
  __IO uint32_t LSTreshold;               /*!< Offset: 0x044 (R/W)   Register */
  __IO uint32_t RhDescriptorA;            /*!< Offset: 0x048 (R/W)   Register */
  __IO uint32_t RhDescriptorB;            /*!< Offset: 0x04C (R/W)   Register */
  __IO uint32_t RhStatus;                 /*!< Offset: 0x050 (R/W)   Register */
  __IO uint32_t RhPortStatus1;            /*!< Offset: 0x054 (R/W)   Register */
  __IO uint32_t RhPortStatus2;            /*!< Offset: 0x05C (R/W)   Register */
       uint32_t RESERVED0[40];
  __I  uint32_t Module_ID;                /*!< Offset: 0x0FC (R/ )  Module ID / Version Reverence ID Register */
                                          /* USB On-The-Go Registers            */
  __I  uint32_t IntSt;                    /*!< Offset: 0x100 (R/ )  OTG Interrupt Status Register */
  __IO uint32_t IntEn;                    /*!< Offset: 0x104 (R/W)  OTG Interrupt Enable Register */
  __O  uint32_t IntSet;                   /*!< Offset: 0x108 ( /W)  OTG Interrupt Set Register */
  __O  uint32_t IntClr;                   /*!< Offset: 0x10C ( /W)  OTG Interrupt Clear Register */
  __IO uint32_t StCtrl;                   /*!< Offset: 0x110 (R/W)  OTG Status and Control Register */
  __IO uint32_t Tmr;                      /*!< Offset: 0x114 (R/W)  OTG Timer Register */
       uint32_t RESERVED1[58];
                                          /* USB Device Interrupt Registers     */
  __I  uint32_t DevIntSt;                 /*!< Offset: 0x200 (R/ )  USB Device Interrupt Status Register */
  __IO uint32_t DevIntEn;                 /*!< Offset: 0x204 (R/W)  USB Device Interrupt Enable Register */
  __O  uint32_t DevIntClr;                /*!< Offset: 0x208 ( /W)  USB Device Interrupt Clear Register */
  __O  uint32_t DevIntSet;                /*!< Offset: 0x20C ( /W)  USB Device Interrupt Set Register */
                                          /* USB Device SIE Command Registers   */
  __O  uint32_t CmdCode;                  /*!< Offset: 0x210 (R/W)  USB Command Code Register */
  __I  uint32_t CmdData;                  /*!< Offset: 0x214 (R/W)  USB Command Data Register */
                                          /* USB Device Transfer Registers      */
  __I  uint32_t RxData;                   /*!< Offset: 0x218 (R/ )  USB Receive Data Register */
  __O  uint32_t TxData;                   /*!< Offset: 0x21C ( /W)  USB Transmit Data Register */
  __I  uint32_t RxPLen;                   /*!< Offset: 0x220 (R/ )  USB Receive Packet Length Register */
  __O  uint32_t TxPLen;                   /*!< Offset: 0x224 ( /W)  USB Transmit Packet Length Register */
  __IO uint32_t Ctrl;                     /*!< Offset: 0x228 (R/W)  USB Control Register */
  __O  uint32_t DevIntPri;                /*!< Offset: 0x22C (R/W)  USB Device Interrupt Priority Register */
                                          /* USB Device Endpoint Interrupt Regs */
  __I  uint32_t EpIntSt;                  /*!< Offset: 0x230 (R/ )  USB Endpoint Interrupt Status Register */
  __IO uint32_t EpIntEn;                  /*!< Offset: 0x234 (R/W)  USB Endpoint Interrupt Enable Register */
  __O  uint32_t EpIntClr;                 /*!< Offset: 0x238 ( /W)  USB Endpoint Interrupt Clear Register */
  __O  uint32_t EpIntSet;                 /*!< Offset: 0x23C ( /W)  USB Endpoint Interrupt Set Register */
  __O  uint32_t EpIntPri;                 /*!< Offset: 0x240 ( /W)  USB Endpoint Interrupt Priority Register */
                                          /* USB Device Endpoint Realization Reg*/
  __IO uint32_t ReEp;                     /*!< Offset: 0x244 (R/W)  USB Realize Endpoint Register */
  __O  uint32_t EpInd;                    /*!< Offset: 0x248 ( /W)  USB Endpoint Index Register */
  __IO uint32_t MaxPSize;                 /*!< Offset: 0x24C (R/W)  USB MaxPacketSize Register */
                                          /* USB Device DMA Registers           */
  __I  uint32_t DMARSt;                   /*!< Offset: 0x250 (R/ )  USB DMA Request Status Register */
  __O  uint32_t DMARClr;                  /*!< Offset: 0x254 ( /W)  USB DMA Request Clear Register */
  __O  uint32_t DMARSet;                  /*!< Offset: 0x258 ( /W)  USB DMA Request Set Register */
       uint32_t RESERVED2[9];
  __IO uint32_t UDCAH;                    /*!< Offset: 0x280 (R/W)  USB UDCA Head Register */
  __I  uint32_t EpDMASt;                  /*!< Offset: 0x284 (R/ )  USB EP DMA Status Register */
  __O  uint32_t EpDMAEn;                  /*!< Offset: 0x288 ( /W)  USB EP DMA Enable Register */
  __O  uint32_t EpDMADis;                 /*!< Offset: 0x28C ( /W)  USB EP DMA Disable Register */
  __I  uint32_t DMAIntSt;                 /*!< Offset: 0x290 (R/ )  USB DMA Interrupt Status Register */
  __IO uint32_t DMAIntEn;                 /*!< Offset: 0x294 (R/W)  USB DMA Interrupt Enable Register */
       uint32_t RESERVED3[2];
  __I  uint32_t EoTIntSt;                 /*!< Offset: 0x2A0 (R/ )  USB End of Transfer Interrupt Status Register */
  __O  uint32_t EoTIntClr;                /*!< Offset: 0x2A4 ( /W)  USB End of Transfer Interrupt Clear Register */
  __O  uint32_t EoTIntSet;                /*!< Offset: 0x2A8 ( /W)  USB End of Transfer Interrupt Set Register */
  __I  uint32_t NDDRIntSt;                /*!< Offset: 0x2AC (R/ )  USB New DD Request Interrupt Status Register */
  __O  uint32_t NDDRIntClr;               /*!< Offset: 0x2B0 ( /W)  USB New DD Request Interrupt Clear Register */
  __O  uint32_t NDDRIntSet;               /*!< Offset: 0x2B4 ( /W)  USB New DD Request Interrupt Set Register */
  __I  uint32_t SysErrIntSt;              /*!< Offset: 0x2B8 (R/ )  USB System Error Interrupt Status Register */
  __O  uint32_t SysErrIntClr;             /*!< Offset: 0x2BC ( /W)  USB System Error Interrupt Clear Register */
  __O  uint32_t SysErrIntSet;             /*!< Offset: 0x2C0 ( /W)  USB System Error Interrupt Set Register */
       uint32_t RESERVED4[15];
                                          /* USB OTG I2C Registers              */
  union {
  __I  uint32_t I2C_RX;                   /*!< Offset: 0x300 (R/ )  OTG I2C Receive Register */
  __O  uint32_t I2C_TX;                   /*!< Offset: 0x300 ( /W)  OTG I2C Transmit Register */
  };
  __I  uint32_t I2C_STS;                  /*!< Offset: 0x304 (R/ )  OTG I2C Status Register */
  __IO uint32_t I2C_CTL;                  /*!< Offset: 0x308 (R/W)  OTG I2C Control Register */
  __IO uint32_t I2C_CLKHI;                /*!< Offset: 0x30C (R/W)  OTG I2C Clock High Register */
  __O  uint32_t I2C_CLKLO;                /*!< Offset: 0x310 ( /W)  OTG I2C Clock Low Register */
       uint32_t RESERVED5[824];
                                          /* USB Clock Control Registers        */
  union {
  __IO uint32_t USBClkCtrl;               /*!< Offset: 0xFF4 (R/W)  OTG clock controller Register */
  __IO uint32_t OTGClkCtrl;               /*!< Offset: 0xFF4 (R/W)  USB clock controller Register */
  };
  union {
  __I  uint32_t USBClkSt;                 /*!< Offset: 0xFF8 (R/ )  OTG clock status Register */
  __I  uint32_t OTGClkSt;                 /*!< Offset: 0xFF8 (R/ )  USB clock status Register */
  };
} LPC_USB_TypeDef;

/*------------- Ethernet Media Access Controller (EMAC) ----------------------*/
/** @brief  Ethernet Media Access Controller (EMAC) register structure definition */
typedef struct
{
  __IO uint32_t MAC1;                       /*!< Offset: 0x000 (R/W)  MAC Configuration Register 1 */
  __IO uint32_t MAC2;                       /*!< Offset: 0x004 (R/W)  MAC Configuration Register 2 */
  __IO uint32_t IPGT;                       /*!< Offset: 0x008 (R/W)  Back-to-Back Inter-Packet-Gap Register */
  __IO uint32_t IPGR;                       /*!< Offset: 0x00C (R/W)  Non Back-to-Back Inter-Packet-Gap Register */
  __IO uint32_t CLRT;                       /*!< Offset: 0x010 (R/W)  Collision Window / Retry Register */
  __IO uint32_t MAXF;                       /*!< Offset: 0x014 (R/W)  Maximum Frame Register */
  __IO uint32_t SUPP;                       /*!< Offset: 0x018 (R/W)  PHY Support Register */
  __IO uint32_t TEST;                       /*!< Offset: 0x01C (R/W)  Test Register */
  __IO uint32_t MCFG;                       /*!< Offset: 0x020 (R/W)  MII Mgmt Configuration Register */
  __IO uint32_t MCMD;                       /*!< Offset: 0x024 (R/W)  MII Mgmt Command Register */
  __IO uint32_t MADR;                       /*!< Offset: 0x028 (R/W)  MII Mgmt Address Register */
  __O  uint32_t MWTD;                       /*!< Offset: 0x02C ( /W)  MII Mgmt Write Data Register */
  __I  uint32_t MRDD;                       /*!< Offset: 0x030 (R/ )  MII Mgmt Read Data Register */
  __I  uint32_t MIND;                       /*!< Offset: 0x034 (R/ )  MII Mgmt Indicators Register */
       uint32_t RESERVED0[2];
  __IO uint32_t SA0;                        /*!< Offset: 0x040 (R/W)  Station Address 0 Register */
  __IO uint32_t SA1;                        /*!< Offset: 0x044 (R/W)  Station Address 1 Register */
  __IO uint32_t SA2;                        /*!< Offset: 0x048 (R/W)  Station Address 2 Register */
       uint32_t RESERVED1[45];
  __IO uint32_t Command;                    /*!< Offset: 0x100 (R/W)  Command Register */
  __I  uint32_t Status;                     /*!< Offset: 0x104 (R/ )  Status Register */
  __IO uint32_t RxDescriptor;               /*!< Offset: 0x108 (R/W)  Receive Descriptor Base Address Register */
  __IO uint32_t RxStatus;                   /*!< Offset: 0x10C (R/W)  Receive Status Base Address Register */
  __IO uint32_t RxDescriptorNumber;         /*!< Offset: 0x110 (R/W)  Receive Number of Descriptors Register */
  __I  uint32_t RxProduceIndex;             /*!< Offset: 0x114 (R/ )  Receive Produce Index Register */
  __IO uint32_t RxConsumeIndex;             /*!< Offset: 0x118 (R/W)  Receive Consume Index Register */
  __IO uint32_t TxDescriptor;               /*!< Offset: 0x11C (R/W)  Transmit Descriptor Base Address Register */
  __IO uint32_t TxStatus;                   /*!< Offset: 0x120 (R/W)  Transmit Status Base Address Register */
  __IO uint32_t TxDescriptorNumber;         /*!< Offset: 0x124 (R/W)  Transmit Number of Descriptors Register */
  __IO uint32_t TxProduceIndex;             /*!< Offset: 0x128 (R/W)  Transmit Produce Index Register */
  __I  uint32_t TxConsumeIndex;             /*!< Offset: 0x12C (R/ )  Transmit Consume Index Register */
       uint32_t RESERVED2[10];
  __I  uint32_t TSV0;                       /*!< Offset: 0x158 (R/ )  Transmit Status Vector 0 Register */
  __I  uint32_t TSV1;                       /*!< Offset: 0x15C (R/ )  Transmit Status Vector 1 Register */
  __I  uint32_t RSV;                        /*!< Offset: 0x160 (R/ )  Receive Status Vector Register */
       uint32_t RESERVED3[3];
  __IO uint32_t FlowControlCounter;         /*!< Offset: 0x170 (R/W)  Flow Control Counter Register */
  __I  uint32_t FlowControlStatus;          /*!< Offset: 0x174 (R/ )  Flow Control Status egister */
       uint32_t RESERVED4[34];
  __IO uint32_t RxFilterCtrl;               /*!< Offset: 0x200 (R/W)  Receive Filter Control Register */
  __I  uint32_t RxFilterWoLStatus;          /*!< Offset: 0x204 (R/ )  Receive Filter WoL Status Register */
  __O  uint32_t RxFilterWoLClear;           /*!< Offset: 0x208 ( /W)  Receive Filter WoL Clear Register */
       uint32_t RESERVED5;
  __IO uint32_t HashFilterL;                /*!< Offset: 0x210 (R/W)  Hash Filter Table LSBs Register */
  __IO uint32_t HashFilterH;                /*!< Offset: 0x214 (R/W)  Hash Filter Table MSBs Register */
       uint32_t RESERVED6[882];
  __I  uint32_t IntStatus;                  /*!< Offset: 0xFE0 (R/ )  Interrupt Status Register */
  __IO uint32_t IntEnable;                  /*!< Offset: 0xFE4 (R/W)  Interrupt Enable Register */
  __O  uint32_t IntClear;                   /*!< Offset: 0xFE8 ( /W)  Interrupt Clear Register */
  __O  uint32_t IntSet;                     /*!< Offset: 0xFEC ( /W)  Interrupt Set Register */
       uint32_t RESERVED7;
  __IO uint32_t PowerDown;                  /*!< Offset: 0xFF4 (R/W)  Power-Down Register */
} LPC_EMAC_TypeDef;

#if defined ( __CC_ARM   )
#pragma no_anon_unions
#endif


/******************************************************************************/
/*                         Peripheral memory map                              */
/******************************************************************************/
/* Base addresses                                                             */
#define LPC_FLASH_BASE        (0x00000000UL)
#define LPC_RAM_BASE          (0x10000000UL)
#ifdef __LPC17XX_REV00
#define LPC_AHBRAM0_BASE      (0x20000000UL)
#define LPC_AHBRAM1_BASE      (0x20004000UL)
#else
#define LPC_AHBRAM0_BASE      (0x2007C000UL)
#define LPC_AHBRAM1_BASE      (0x20080000UL)
#endif
#define LPC_GPIO_BASE         (0x2009C000UL)
#define LPC_APB0_BASE         (0x40000000UL)
#define LPC_APB1_BASE         (0x40080000UL)
#define LPC_AHB_BASE          (0x50000000UL)
#define LPC_CM3_BASE          (0xE0000000UL)

/* APB0 peripherals                                                           */
#define LPC_WDT_BASE          (LPC_APB0_BASE + 0x00000)
#define LPC_TIM0_BASE         (LPC_APB0_BASE + 0x04000)
#define LPC_TIM1_BASE         (LPC_APB0_BASE + 0x08000)
#define LPC_UART0_BASE        (LPC_APB0_BASE + 0x0C000)
#define LPC_UART1_BASE        (LPC_APB0_BASE + 0x10000)
#define LPC_PWM1_BASE         (LPC_APB0_BASE + 0x18000)
#define LPC_I2C0_BASE         (LPC_APB0_BASE + 0x1C000)
#define LPC_SPI_BASE          (LPC_APB0_BASE + 0x20000)
#define LPC_RTC_BASE          (LPC_APB0_BASE + 0x24000)
#define LPC_GPIOINT_BASE      (LPC_APB0_BASE + 0x28080)
#define LPC_PINCON_BASE       (LPC_APB0_BASE + 0x2C000)
#define LPC_SSP1_BASE         (LPC_APB0_BASE + 0x30000)
#define LPC_ADC_BASE          (LPC_APB0_BASE + 0x34000)
#define LPC_CANAF_RAM_BASE    (LPC_APB0_BASE + 0x38000)
#define LPC_CANAF_BASE        (LPC_APB0_BASE + 0x3C000)
#define LPC_CANCR_BASE        (LPC_APB0_BASE + 0x40000)
#define LPC_CAN1_BASE         (LPC_APB0_BASE + 0x44000)
#define LPC_CAN2_BASE         (LPC_APB0_BASE + 0x48000)
#define LPC_I2C1_BASE         (LPC_APB0_BASE + 0x5C000)

/* APB1 peripherals                                                           */
#define LPC_SSP0_BASE         (LPC_APB1_BASE + 0x08000)
#define LPC_DAC_BASE          (LPC_APB1_BASE + 0x0C000)
#define LPC_TIM2_BASE         (LPC_APB1_BASE + 0x10000)
#define LPC_TIM3_BASE         (LPC_APB1_BASE + 0x14000)
#define LPC_UART2_BASE        (LPC_APB1_BASE + 0x18000)
#define LPC_UART3_BASE        (LPC_APB1_BASE + 0x1C000)
#define LPC_I2C2_BASE         (LPC_APB1_BASE + 0x20000)
#define LPC_I2S_BASE          (LPC_APB1_BASE + 0x28000)
#define LPC_RIT_BASE          (LPC_APB1_BASE + 0x30000)
#define LPC_MCPWM_BASE        (LPC_APB1_BASE + 0x38000)
#define LPC_QEI_BASE          (LPC_APB1_BASE + 0x3C000)
#define LPC_SC_BASE           (LPC_APB1_BASE + 0x7C000)

/* AHB peripherals                                                            */
#define LPC_EMAC_BASE         (LPC_AHB_BASE  + 0x00000)
#define LPC_GPDMA_BASE        (LPC_AHB_BASE  + 0x04000)
#define LPC_GPDMACH0_BASE     (LPC_AHB_BASE  + 0x04100)
#define LPC_GPDMACH1_BASE     (LPC_AHB_BASE  + 0x04120)
#define LPC_GPDMACH2_BASE     (LPC_AHB_BASE  + 0x04140)
#define LPC_GPDMACH3_BASE     (LPC_AHB_BASE  + 0x04160)
#define LPC_GPDMACH4_BASE     (LPC_AHB_BASE  + 0x04180)
#define LPC_GPDMACH5_BASE     (LPC_AHB_BASE  + 0x041A0)
#define LPC_GPDMACH6_BASE     (LPC_AHB_BASE  + 0x041C0)
#define LPC_GPDMACH7_BASE     (LPC_AHB_BASE  + 0x041E0)
#define LPC_USB_BASE          (LPC_AHB_BASE  + 0x0C000)

/* GPIOs                                                                      */
#define LPC_GPIO0_BASE        (LPC_GPIO_BASE + 0x00000)
#define LPC_GPIO1_BASE        (LPC_GPIO_BASE + 0x00020)
#define LPC_GPIO2_BASE        (LPC_GPIO_BASE + 0x00040)
#define LPC_GPIO3_BASE        (LPC_GPIO_BASE + 0x00060)
#define LPC_GPIO4_BASE        (LPC_GPIO_BASE + 0x00080)


/******************************************************************************/
/*                         Peripheral declaration                             */
/******************************************************************************/
#define LPC_SC                ((LPC_SC_TypeDef        *) LPC_SC_BASE       )
#define LPC_GPIO0             ((LPC_GPIO_TypeDef      *) LPC_GPIO0_BASE    )
#define LPC_GPIO1             ((LPC_GPIO_TypeDef      *) LPC_GPIO1_BASE    )
#define LPC_GPIO2             ((LPC_GPIO_TypeDef      *) LPC_GPIO2_BASE    )
#define LPC_GPIO3             ((LPC_GPIO_TypeDef      *) LPC_GPIO3_BASE    )
#define LPC_GPIO4             ((LPC_GPIO_TypeDef      *) LPC_GPIO4_BASE    )
#define LPC_WDT               ((LPC_WDT_TypeDef       *) LPC_WDT_BASE      )
#define LPC_TIM0              ((LPC_TIM_TypeDef       *) LPC_TIM0_BASE     )
#define LPC_TIM1              ((LPC_TIM_TypeDef       *) LPC_TIM1_BASE     )
#define LPC_TIM2              ((LPC_TIM_TypeDef       *) LPC_TIM2_BASE     )
#define LPC_TIM3              ((LPC_TIM_TypeDef       *) LPC_TIM3_BASE     )
#define LPC_RIT               ((LPC_RIT_TypeDef       *) LPC_RIT_BASE      )
#define LPC_UART0             ((LPC_UART_TypeDef      *) LPC_UART0_BASE    )
#define LPC_UART1             ((LPC_UART1_TypeDef     *) LPC_UART1_BASE    )
#define LPC_UART2             ((LPC_UART_TypeDef      *) LPC_UART2_BASE    )
#define LPC_UART3             ((LPC_UART_TypeDef      *) LPC_UART3_BASE    )
#define LPC_PWM1              ((LPC_PWM_TypeDef       *) LPC_PWM1_BASE     )
#define LPC_I2C0              ((LPC_I2C_TypeDef       *) LPC_I2C0_BASE     )
#define LPC_I2C1              ((LPC_I2C_TypeDef       *) LPC_I2C1_BASE     )
#define LPC_I2C2              ((LPC_I2C_TypeDef       *) LPC_I2C2_BASE     )
#define LPC_I2S               ((LPC_I2S_TypeDef       *) LPC_I2S_BASE      )
#define LPC_SPI               ((LPC_SPI_TypeDef       *) LPC_SPI_BASE      )
#define LPC_RTC               ((LPC_RTC_TypeDef       *) LPC_RTC_BASE      )
#define LPC_GPIOINT           ((LPC_GPIOINT_TypeDef   *) LPC_GPIOINT_BASE  )
#define LPC_PINCON            ((LPC_PINCON_TypeDef    *) LPC_PINCON_BASE   )
#define LPC_SSP0              ((LPC_SSP_TypeDef       *) LPC_SSP0_BASE     )
#define LPC_SSP1              ((LPC_SSP_TypeDef       *) LPC_SSP1_BASE     )
#define LPC_ADC               ((LPC_ADC_TypeDef       *) LPC_ADC_BASE      )
#define LPC_DAC               ((LPC_DAC_TypeDef       *) LPC_DAC_BASE      )
#define LPC_CANAF_RAM         ((LPC_CANAF_RAM_TypeDef *) LPC_CANAF_RAM_BASE)
#define LPC_CANAF             ((LPC_CANAF_TypeDef     *) LPC_CANAF_BASE    )
#define LPC_CANCR             ((LPC_CANCR_TypeDef     *) LPC_CANCR_BASE    )
#define LPC_CAN1              ((LPC_CAN_TypeDef       *) LPC_CAN1_BASE     )
#define LPC_CAN2              ((LPC_CAN_TypeDef       *) LPC_CAN2_BASE     )
#define LPC_MCPWM             ((LPC_MCPWM_TypeDef     *) LPC_MCPWM_BASE    )
#define LPC_QEI               ((LPC_QEI_TypeDef       *) LPC_QEI_BASE      )
#define LPC_EMAC              ((LPC_EMAC_TypeDef      *) LPC_EMAC_BASE     )
#define LPC_GPDMA             ((LPC_GPDMA_TypeDef     *) LPC_GPDMA_BASE    )
#define LPC_GPDMACH0          ((LPC_GPDMACH_TypeDef   *) LPC_GPDMACH0_BASE )
#define LPC_GPDMACH1          ((LPC_GPDMACH_TypeDef   *) LPC_GPDMACH1_BASE )
#define LPC_GPDMACH2          ((LPC_GPDMACH_TypeDef   *) LPC_GPDMACH2_BASE )
#define LPC_GPDMACH3          ((LPC_GPDMACH_TypeDef   *) LPC_GPDMACH3_BASE )
#define LPC_GPDMACH4          ((LPC_GPDMACH_TypeDef   *) LPC_GPDMACH4_BASE )
#define LPC_GPDMACH5          ((LPC_GPDMACH_TypeDef   *) LPC_GPDMACH5_BASE )
#define LPC_GPDMACH6          ((LPC_GPDMACH_TypeDef   *) LPC_GPDMACH6_BASE )
#define LPC_GPDMACH7          ((LPC_GPDMACH_TypeDef   *) LPC_GPDMACH7_BASE )
#define LPC_USB               ((LPC_USB_TypeDef       *) LPC_USB_BASE      )

/**
 * @}
 */

#endif  // __LPC17xx_H__
