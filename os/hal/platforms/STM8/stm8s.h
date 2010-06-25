/**
  ******************************************************************************
  * @file stm8s.h
  * @brief This file contains all HW registers definitions and memory mapping.
  * @author STMicroelectronics - MCD Application Team
  * @version V1.1.1
  * @date 06/05/2009
  ******************************************************************************
  *
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2009 STMicroelectronics</center></h2>
  * @image html logo.bmp
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __STM8S_H
#define __STM8S_H

/******************************************************************************/
/*                   Library configuration section                            */
/******************************************************************************/
/* Check the used compiler */
#if defined(__CSMC__)
 #undef _RAISONANCE_
 #define _COSMIC_
#elif defined(__RCST7__)
 #undef _COSMIC_
 #define _RAISONANCE_
#else
 #error "Unsupported Compiler!"          /* Compiler defines not found */
#endif

/* Uncomment the line below according to the target STM8S device used in your
   application. 
   Tip: To avoid modifying this file each time you need to switch between these
        devices, you can define the device in your toolchain compiler preprocessor. */
#if !defined (STM8S208) && !defined (STM8S207) && !defined (STM8S105) && !defined (STM8S103) && !defined (STM8S903)
 #define STM8S208
 /* #define STM8S207 */
 /* #define STM8S105 */  
 /* #define STM8S103 */
 /* #define STM8S903 */
#endif


#if !defined  USE_STDPERIPH_DRIVER
/* Comment the line below if you will not use the peripherals drivers.
   In this case, these drivers will not be included and the application code will be
   based on direct access to peripherals registers */
/*  #define USE_STDPERIPH_DRIVER*/
#endif

/* For FLASH routines, select whether pointer will be declared as near (2 bytes, handle
   code smaller than 64KB) or far (3 bytes, handle code larger than 64K) */
/*#define PointerAttr_Near 1  */  /*!< Used with memory Models for code smaller than 64K */ 
#define PointerAttr_Far 2 /*!< Used with memory Models for code larger than 64K */ 

#ifdef _COSMIC_
 #define FAR  @far
 #define NEAR @near
 #define TINY @tiny
 #define __CONST  const
#else /* __RCST7__ */
 #define FAR  far
 #define NEAR data
 #define TINY page0
 #define __CONST  code
#endif /* __CSMC__ */

#ifdef PointerAttr_Far
 #define PointerAttr FAR 
#else /* PointerAttr_Near */
 #define PointerAttr NEAR
#endif /* PointerAttr_Far */


/* Uncomment the line below to use the cosmic section */
#if defined(_COSMIC_)
/* #define USE_COSMIC_SECTIONS  (1)*/
#endif

/******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm8s_type.h"

/* Exported types and constants-----------------------------------------------*/
/** @addtogroup MAP_FILE_Exported_Types_and_Constants
  * @{
  */

/******************************************************************************/
/*                          IP registers structures                           */
/******************************************************************************/
/*----------------------------------------------------------------------------*/
/**
  * @brief General Purpose I/Os (GPIO)
  */

typedef struct GPIO_struct
{
  vu8 ODR; /*!< Output Data Register */
  vu8 IDR; /*!< Input Data Register */
  vu8 DDR; /*!< Data Direction Register */
  vu8 CR1; /*!< Configuration Register 1 */
  vu8 CR2; /*!< Configuration Register 2 */
}
GPIO_TypeDef;

/** @addtogroup GPIO_Registers_Reset_Value
  * @{
  */

#define GPIO_ODR_RESET_VALUE ((u8)0x00)
#define GPIO_DDR_RESET_VALUE ((u8)0x00)
#define GPIO_CR1_RESET_VALUE ((u8)0x00)
#define GPIO_CR2_RESET_VALUE ((u8)0x00)

/**
  * @}
  */

/*----------------------------------------------------------------------------*/
#if defined(STM8S105) || defined(STM8S103) || defined(STM8S903)
/**
  * @brief Analog to Digital Converter (ADC1)
  */
typedef struct ADC1_struct
{
  vu8 DB0RH;     /*!< ADC1 Data Buffer Register (MSB)  */
  vu8 DB0RL;      /*!< ADC1 Data Buffer Register (LSB)  */
  vu8 DB1RH;      /*!< ADC1 Data Buffer Register (MSB)  */
  vu8 DB1RL;      /*!< ADC1 Data Buffer Register (LSB)  */
  vu8 DB2RH;      /*!< ADC1 Data Buffer Register (MSB)  */
  vu8 DB2RL;      /*!< ADC1 Data Buffer Register (LSB)  */
  vu8 DB3RH;      /*!< ADC1 Data Buffer Register (MSB)  */
  vu8 DB3RL;      /*!< ADC1 Data Buffer Register (LSB)  */
  vu8 DB4RH;      /*!< ADC1 Data Buffer Register (MSB)  */
  vu8 DB4RL;      /*!< ADC1 Data Buffer Register (LSB)  */
  vu8 DB5RH;      /*!< ADC1 Data Buffer Register (MSB)  */
  vu8 DB5RL;      /*!< ADC1 Data Buffer Register (LSB)  */
  vu8 DB6RH;      /*!< ADC1 Data Buffer Register (MSB)  */
  vu8 DB6RL;      /*!< ADC1 Data Buffer Register (LSB)  */
  vu8 DB7RH;      /*!< ADC1 Data Buffer Register (MSB)  */
  vu8 DB7RL;      /*!< ADC1 Data Buffer Register (LSB)  */
  vu8 DB8RH;     /*!< ADC1 Data Buffer Register (MSB)  */
  vu8 DB8RL;      /*!< ADC1 Data Buffer Register (LSB)  */
  vu8 DB9RH;      /*!< ADC1 Data Buffer Register (MSB)  */
  vu8 DB9RL;      /*!< ADC1 Data Buffer Register (LSB)  */
  vu8 RESERVED[12]; /*!< Reserved byte */
  vu8 CSR;        /*!< ADC1 control status register */
  vu8 CR1;        /*!< ADC1 configuration register 1 */
  vu8 CR2;        /*!< ADC1 configuration register 2 */
  vu8 CR3;    /*!< ADC1 configuration register 3  */
  vu8 DRH;        /*!< ADC1 Data high */
  vu8 DRL;        /*!< ADC1 Data low */
  vu8 TDRH;       /*!< ADC1 Schmitt trigger disable register high  */
  vu8 TDRL;       /*!< ADC1 Schmitt trigger disable register low */
  vu8 HTRH;   /*!< ADC1 high threshold register High*/
  vu8 HTRL;   /*!< ADC1 high threshold register Low*/
  vu8 LTRH;   /*!< ADC1 low threshold register high */
  vu8 LTRL;   /*!< ADC1 low threshold register low */
  vu8 AWSRH;   /*!< ADC1 watchdog status register high */
  vu8 AWSRL;   /*!< ADC1 watchdog status register low */
  vu8 AWCRH;   /*!< ADC1 watchdog control register high */
  vu8 AWCRL;   /*!< ADC1 watchdog control register low */
}
ADC1_TypeDef;

/** @addtogroup ADC1_Registers_Reset_Value
  * @{
  */

#define  ADC1_CSR_RESET_VALUE  ((u8)0x00)
#define  ADC1_CR1_RESET_VALUE  ((u8)0x00)
#define  ADC1_CR2_RESET_VALUE  ((u8)0x00)
#define  ADC1_CR3_RESET_VALUE  ((u8)0x00)
#define  ADC1_TDRL_RESET_VALUE ((u8)0x00)
#define  ADC1_TDRH_RESET_VALUE ((u8)0x00)
#define  ADC1_HTRL_RESET_VALUE ((u8)0x03)
#define  ADC1_HTRH_RESET_VALUE ((u8)0xFF)
#define  ADC1_LTRH_RESET_VALUE ((u8)0x00)
#define  ADC1_LTRL_RESET_VALUE ((u8)0x00)
#define  ADC1_AWCRH_RESET_VALUE ((u8)0x00)
#define  ADC1_AWCRL_RESET_VALUE ((u8)0x00)

/**
  * @}
  */

/** @addtogroup ADC1_Registers_Bits_Definition
  * @{
  */

#define ADC1_CSR_EOC  ((u8)0x80) /*!< End of Conversion mask */
#define ADC1_CSR_AWD  ((u8)0x40) /*!< Analog Watch Dog Status mask */
#define ADC1_CSR_EOCIE ((u8)0x20) /*!< Interrupt Enable for EOC mask */
#define ADC1_CSR_AWDIE ((u8)0x10) /*!< Analog Watchdog interrupt enable mask */
#define ADC1_CSR_CH   ((u8)0x0F) /*!< Channel selection bits mask */

#define ADC1_CR1_SPSEL ((u8)0x70) /*!< Prescaler selectiont mask */
#define ADC1_CR1_CONT  ((u8)0x02) /*!< Continuous conversion mask */
#define ADC1_CR1_ADON  ((u8)0x01) /*!< A/D Converter on/off mask */

#define ADC1_CR2_EXTTRIG ((u8)0x40) /*!< External trigger enable mask */
#define ADC1_CR2_EXTSEL  ((u8)0x30) /*!< External event selection mask */
#define ADC1_CR2_ALIGN   ((u8)0x08) /*!< Data Alignment mask */
#define ADC1_CR2_SCAN   ((u8)0x02) /*!< Scan mode mask */

#define ADC1_CR3_DBUF   ((u8)0x80) /*!< Data Buffer Enable mask */
#define ADC1_CR3_OVR    ((u8)0x40) /*!< Overrun Status Flag mask */

#endif /* (STM8S105) ||(STM8S103) || (STM8S903) */
/**
  * @}
  */

/*----------------------------------------------------------------------------*/
/**
  * @brief Analog to Digital Converter (ADC2)
  */
#if defined(STM8S208) || defined(STM8S207)
typedef struct ADC2_struct
{
  vu8 CSR;        /*!< ADC2 control status register */
  vu8 CR1;        /*!< ADC2 configuration register 1 */
  vu8 CR2;        /*!< ADC2 configuration register 2 */
  vu8 RESERVED;   /*!< Reserved byte */
  vu8 DRH;        /*!< ADC2 Data high */
  vu8 DRL;        /*!< ADC2 Data low */
  vu8 TDRH;       /*!< ADC2 Schmitt trigger disable register high  */
  vu8 TDRL;       /*!< ADC2 Schmitt trigger disable register low */
}
ADC2_TypeDef;

/** @addtogroup ADC2_Registers_Reset_Value
  * @{
  */

#define  ADC2_CSR_RESET_VALUE  ((u8)0x00)
#define  ADC2_CR1_RESET_VALUE  ((u8)0x00)
#define  ADC2_CR2_RESET_VALUE  ((u8)0x00)
#define  ADC2_TDRL_RESET_VALUE ((u8)0x00)
#define  ADC2_TDRH_RESET_VALUE ((u8)0x00)

/**
  * @}
  */

/** @addtogroup ADC2_Registers_Bits_Definition
  * @{
  */

#define ADC2_CSR_EOC  ((u8)0x80) /*!< End of Conversion mask */
#define ADC2_CSR_EOCIE ((u8)0x20) /*!< Interrupt Enable for EOC mask */
#define ADC2_CSR_CH   ((u8)0x0F) /*!< Channel selection bits mask */

#define ADC2_CR1_SPSEL ((u8)0x70) /*!< Prescaler selectiont mask */
#define ADC2_CR1_CONT  ((u8)0x02) /*!< Continuous conversion mask */
#define ADC2_CR1_ADON  ((u8)0x01) /*!< A/D Converter on/off mask */

#define ADC2_CR2_EXTTRIG ((u8)0x40) /*!< External trigger enable mask */
#define ADC2_CR2_EXTSEL  ((u8)0x30) /*!< External event selection mask */
#define ADC2_CR2_ALIGN   ((u8)0x08) /*!< Data Alignment mask */

#endif /* (STM8S208) ||(STM8S207)  */
/**
  * @}
  */
/*----------------------------------------------------------------------------*/
/**
  * @brief Auto Wake Up (AWU) peripheral registers.
  */

typedef struct AWU_struct
{
  vu8 CSR; /*!< AWU Control status register */
  vu8 APR; /*!< AWU Asynchronous prescalar buffer */
  vu8 TBR; /*!< AWU Time base selection register */
}
AWU_TypeDef;

/** @addtogroup AWU_Registers_Reset_Value
  * @{
  */

#define AWU_CSR_RESET_VALUE ((u8)0x00)
#define AWU_APR_RESET_VALUE ((u8)0x3F)
#define AWU_TBR_RESET_VALUE ((u8)0x00)

/**
  * @}
  */

/** @addtogroup AWU_Registers_Bits_Definition
  * @{
  */

#define AWU_CSR_AWUF  ((u8)0x20) /*!< Interrupt flag mask */
#define AWU_CSR_AWUEN ((u8)0x10) /*!< Auto Wake-up enable mask */
#define AWU_CSR_MR    ((u8)0x02) /*!< Master Reset mask */
#define AWU_CSR_MSR   ((u8)0x01) /*!< Measurement enable mask */

#define AWU_APR_APR ((u8)0x3F) /*!< Asynchronous Prescaler divider mask */

#define AWU_TBR_AWUTB ((u8)0x0F) /*!< Timebase selection mask */

/**
  * @}
  */

/*----------------------------------------------------------------------------*/
/**
  * @brief Beeper (BEEP) peripheral registers.
  */

typedef struct BEEP_struct
{
  vu8 CSR; /*!< BEEP Control status register */
}
BEEP_TypeDef;

/** @addtogroup BEEP_Registers_Reset_Value
  * @{
  */

#define BEEP_CSR_RESET_VALUE ((u8)0x1F)

/**
  * @}
  */

/** @addtogroup BEEP_Registers_Bits_Definition
  * @{
  */

#define BEEP_CSR_BEEPSEL ((u8)0xC0) /*!< Beeper frequency selection mask */
#define BEEP_CSR_BEEPEN  ((u8)0x20) /*!< Beeper enable mask */
#define BEEP_CSR_BEEPDIV ((u8)0x1F) /*!< Beeper Divider prescalar mask */

/**
  * @}
  */

/*----------------------------------------------------------------------------*/
/**
  * @brief Clock Controller (CLK)
  */

typedef struct CLK_struct
{
  vu8 ICKR;     /*!< Internal Clocks Control Register */
  vu8 ECKR;     /*!< External Clocks Control Register */
  u8 RESERVED; /*!< Reserved byte */
  vu8 CMSR;     /*!< Clock Master Status Register */
  vu8 SWR;      /*!< Clock Master Switch Register */
  vu8 SWCR;     /*!< Switch Control Register */
  vu8 CKDIVR;   /*!< Clock Divider Register */
  vu8 PCKENR1;  /*!< Peripheral Clock Gating Register 1 */
  vu8 CSSR;     /*!< Clock Security Sytem Register */
  vu8 CCOR;     /*!< Configurable Clock Output Register */
  vu8 PCKENR2;  /*!< Peripheral Clock Gating Register 2 */
  vu8 CANCCR;   /*!< CAN external clock control Register (exist only in STM8S208 otherwise it is reserved) */
  vu8 HSITRIMR; /*!< HSI Calibration Trimmer Register */
  vu8 SWIMCCR;  /*!< SWIM clock control register */
}
CLK_TypeDef;

/** @addtogroup CLK_Registers_Reset_Value
  * @{
  */

#define CLK_ICKR_RESET_VALUE     ((u8)0x01)
#define CLK_ECKR_RESET_VALUE     ((u8)0x00)
#define CLK_CMSR_RESET_VALUE     ((u8)0xE1)
#define CLK_SWR_RESET_VALUE      ((u8)0xE1)
#define CLK_SWCR_RESET_VALUE     ((u8)0x00)
#define CLK_CKDIVR_RESET_VALUE   ((u8)0x18)
#define CLK_PCKENR1_RESET_VALUE  ((u8)0xFF)
#define CLK_PCKENR2_RESET_VALUE  ((u8)0xFF)
#define CLK_CSSR_RESET_VALUE     ((u8)0x00)
#define CLK_CCOR_RESET_VALUE     ((u8)0x00)
#define CLK_CANCCR_RESET_VALUE   ((u8)0x00)
#define CLK_HSITRIMR_RESET_VALUE ((u8)0x00)
#define CLK_SWIMCCR_RESET_VALUE  ((u8)0x00)

/**
  * @}
  */

/** @addtogroup CLK_Registers_Bits_Definition
  * @{
  */

#define CLK_ICKR_SWUAH    ((u8)0x20) /*!< Slow Wake-up from Active Halt/Halt modes */
#define CLK_ICKR_LSIRDY ((u8)0x10) /*!< Low speed internal oscillator ready */
#define CLK_ICKR_LSIEN  ((u8)0x08) /*!< Low speed internal RC oscillator enable */
#define CLK_ICKR_FHWU    ((u8)0x04) /*!< Fast Wake-up from Active Halt/Halt mode */
#define CLK_ICKR_HSIRDY ((u8)0x02) /*!< High speed internal RC oscillator ready */
#define CLK_ICKR_HSIEN  ((u8)0x01) /*!< High speed internal RC oscillator enable */

#define CLK_ECKR_HSERDY ((u8)0x02) /*!< High speed external crystal oscillator ready */
#define CLK_ECKR_HSEEN  ((u8)0x01) /*!< High speed external crystal oscillator enable */

#define CLK_CMSR_CKM    ((u8)0xFF) /*!< Clock master status bits */

#define CLK_SWR_SWI     ((u8)0xFF) /*!< Clock master selection bits */

#define CLK_SWCR_SWIF   ((u8)0x08) /*!< Clock switch interrupt flag */
#define CLK_SWCR_SWIEN  ((u8)0x04) /*!< Clock switch interrupt enable */
#define CLK_SWCR_SWEN   ((u8)0x02) /*!< Switch start/stop */
#define CLK_SWCR_SWBSY  ((u8)0x01) /*!< Switch busy */

#define CLK_CKDIVR_HSIDIV ((u8)0x18) /*!< High speed internal clock prescaler */
#define CLK_CKDIVR_CPUDIV ((u8)0x07) /*!< CPU clock prescaler */

#define CLK_PCKENR1_TIM1    ((u8)0x80) /*!< Timer 1 clock enable */ 
#define CLK_PCKENR1_TIM3    ((u8)0x40) /*!< Timer 3 clock enable */
#define CLK_PCKENR1_TIM2    ((u8)0x20) /*!< Timer 2 clock enable */
#define CLK_PCKENR1_TIM5    ((u8)0x20) /*!< Timer 5 clock enable */
#define CLK_PCKENR1_TIM4    ((u8)0x10) /*!< Timer 4 clock enable */
#define CLK_PCKENR1_TIM6    ((u8)0x10) /*!< Timer 6 clock enable */
#define CLK_PCKENR1_UART3 ((u8)0x08) /*!< UART3 clock enable */
#define CLK_PCKENR1_UART2 ((u8)0x08) /*!< UART2 clock enable */
#define CLK_PCKENR1_UART1   ((u8)0x04) /*!< UART1 clock enable */
#define CLK_PCKENR1_SPI     ((u8)0x02) /*!< SPI clock enable */
#define CLK_PCKENR1_I2C     ((u8)0x01) /*!< I2C clock enable */

#define CLK_PCKENR2_CAN ((u8)0x80) /*!< CAN clock enable */
#define CLK_PCKENR2_ADC ((u8)0x08) /*!< ADC clock enable */
#define CLK_PCKENR2_AWU ((u8)0x04) /*!< AWU clock enable */

#define CLK_CSSR_CSSD   ((u8)0x08) /*!< Clock security sytem detection */
#define CLK_CSSR_CSSDIE ((u8)0x04) /*!< Clock security system detection interrupt enable */
#define CLK_CSSR_AUX    ((u8)0x02) /*!< Auxiliary oscillator connected to master clock */
#define CLK_CSSR_CSSEN  ((u8)0x01) /*!< Clock security system enable */

#define CLK_CCOR_CCOBSY ((u8)0x40) /*!< Configurable clock output busy */
#define CLK_CCOR_CCORDY ((u8)0x20) /*!< Configurable clock output ready */
#define CLK_CCOR_CCOSEL ((u8)0x1E) /*!< Configurable clock output selection */
#define CLK_CCOR_CCOEN  ((u8)0x01) /*!< Configurable clock output enable */

#define CLK_CANCCR_CANDIV ((u8)0x07) /*!< External CAN clock divider */

#define CLK_HSITRIMR_HSITRIM ((u8)0x07) /*!< High speed internal oscillator trimmer */

#define CLK_SWIMCCR_SWIMDIV ((u8)0x01) /*!< SWIM Clock Dividing Factor */

/**
  * @}
  */

/*----------------------------------------------------------------------------*/
/**
  * @brief 16-bit timer with complementary PWM outputs (TIM1)
  */

typedef struct TIM1_struct
{
  vu8 CR1;   /*!< control register 1 */
  vu8 CR2;   /*!< control register 2 */
  vu8 SMCR;  /*!< Synchro mode control register */
  vu8 ETR;   /*!< external trigger register */
  vu8 IER;   /*!< interrupt enable register*/
  vu8 SR1;   /*!< status register 1 */
  vu8 SR2;   /*!< status register 2 */
  vu8 EGR;   /*!< event generation register */
  vu8 CCMR1; /*!< CC mode register 1 */
  vu8 CCMR2; /*!< CC mode register 2 */
  vu8 CCMR3; /*!< CC mode register 3 */
  vu8 CCMR4; /*!< CC mode register 4 */
  vu8 CCER1; /*!< CC enable register 1 */
  vu8 CCER2; /*!< CC enable register 2 */
  vu8 CNTRH; /*!< counter high */
  vu8 CNTRL; /*!< counter low */
  vu8 PSCRH; /*!< prescaler high */
  vu8 PSCRL; /*!< prescaler low */
  vu8 ARRH;  /*!< auto-reload register high */
  vu8 ARRL;  /*!< auto-reload register low */
  vu8 RCR;   /*!< Repetition Counter register */
  vu8 CCR1H; /*!< capture/compare register 1 high */
  vu8 CCR1L; /*!< capture/compare register 1 low */
  vu8 CCR2H; /*!< capture/compare register 2 high */
  vu8 CCR2L; /*!< capture/compare register 2 low */
  vu8 CCR3H; /*!< capture/compare register 3 high */
  vu8 CCR3L; /*!< capture/compare register 3 low */
  vu8 CCR4H; /*!< capture/compare register 3 high */
  vu8 CCR4L; /*!< capture/compare register 3 low */
  vu8 BKR;   /*!< Break Register */
  vu8 DTR;   /*!< dead-time register */
  vu8 OISR;  /*!< Output idle register */
}
TIM1_TypeDef;

/** @addtogroup TIM1_Registers_Reset_Value
  * @{
  */

#define TIM1_CR1_RESET_VALUE   ((u8)0x00)
#define TIM1_CR2_RESET_VALUE   ((u8)0x00)
#define TIM1_SMCR_RESET_VALUE  ((u8)0x00)
#define TIM1_ETR_RESET_VALUE   ((u8)0x00)
#define TIM1_IER_RESET_VALUE   ((u8)0x00)
#define TIM1_SR1_RESET_VALUE   ((u8)0x00)
#define TIM1_SR2_RESET_VALUE   ((u8)0x00)
#define TIM1_EGR_RESET_VALUE   ((u8)0x00)
#define TIM1_CCMR1_RESET_VALUE ((u8)0x00)
#define TIM1_CCMR2_RESET_VALUE ((u8)0x00)
#define TIM1_CCMR3_RESET_VALUE ((u8)0x00)
#define TIM1_CCMR4_RESET_VALUE ((u8)0x00)
#define TIM1_CCER1_RESET_VALUE ((u8)0x00)
#define TIM1_CCER2_RESET_VALUE ((u8)0x00)
#define TIM1_CNTRH_RESET_VALUE ((u8)0x00)
#define TIM1_CNTRL_RESET_VALUE ((u8)0x00)
#define TIM1_PSCRH_RESET_VALUE ((u8)0x00)
#define TIM1_PSCRL_RESET_VALUE ((u8)0x00)
#define TIM1_ARRH_RESET_VALUE  ((u8)0xFF)
#define TIM1_ARRL_RESET_VALUE  ((u8)0xFF)
#define TIM1_RCR_RESET_VALUE   ((u8)0x00)
#define TIM1_CCR1H_RESET_VALUE ((u8)0x00)
#define TIM1_CCR1L_RESET_VALUE ((u8)0x00)
#define TIM1_CCR2H_RESET_VALUE ((u8)0x00)
#define TIM1_CCR2L_RESET_VALUE ((u8)0x00)
#define TIM1_CCR3H_RESET_VALUE ((u8)0x00)
#define TIM1_CCR3L_RESET_VALUE ((u8)0x00)
#define TIM1_CCR4H_RESET_VALUE ((u8)0x00)
#define TIM1_CCR4L_RESET_VALUE ((u8)0x00)
#define TIM1_BKR_RESET_VALUE   ((u8)0x00)
#define TIM1_DTR_RESET_VALUE   ((u8)0x00)
#define TIM1_OISR_RESET_VALUE  ((u8)0x00)

/**
  * @}
  */

/** @addtogroup TIM1_Registers_Bits_Definition
  * @{
  */
/* CR1*/
#define TIM1_CR1_ARPE ((u8)0x80) /*!< Auto-Reload Preload Enable mask. */
#define TIM1_CR1_CMS  ((u8)0x60) /*!< Center-aligned Mode Selection mask. */
#define TIM1_CR1_DIR  ((u8)0x10) /*!< Direction mask. */
#define TIM1_CR1_OPM  ((u8)0x08) /*!< One Pulse Mode mask. */
#define TIM1_CR1_URS  ((u8)0x04) /*!< Update Request Source mask. */
#define TIM1_CR1_UDIS ((u8)0x02) /*!< Update DIsable mask. */
#define TIM1_CR1_CEN  ((u8)0x01) /*!< Counter Enable mask. */
/* CR2*/
#define TIM1_CR2_TI1S ((u8)0x80) /*!< TI1S Selection mask. */
#define TIM1_CR2_MMS  ((u8)0x70) /*!< MMS Selection mask. */
#define TIM1_CR2_COMS ((u8)0x04) /*!< Capture/Compare Control Update Selection mask. */
#define TIM1_CR2_CCPC ((u8)0x01) /*!< Capture/Compare Preloaded Control mask. */
/* SMCR*/
#define TIM1_SMCR_MSM ((u8)0x80) /*!< Master/Slave Mode mask. */
#define TIM1_SMCR_TS  ((u8)0x70) /*!< Trigger Selection mask. */
#define TIM1_SMCR_SMS ((u8)0x07) /*!< Slave Mode Selection mask. */
/*ETR*/
#define TIM1_ETR_ETP  ((u8)0x80) /*!< External Trigger Polarity mask. */
#define TIM1_ETR_ECE  ((u8)0x40)/*!< External Clock mask. */
#define TIM1_ETR_ETPS ((u8)0x30) /*!< External Trigger Prescaler mask. */
#define TIM1_ETR_ETF  ((u8)0x0F) /*!< External Trigger Filter mask. */
/*IER*/
#define TIM1_IER_BIE   ((u8)0x80) /*!< Break Interrupt Enable mask. */
#define TIM1_IER_TIE   ((u8)0x40) /*!< Trigger Interrupt Enable mask. */
#define TIM1_IER_COMIE ((u8)0x20) /*!<  Commutation Interrupt Enable mask.*/
#define TIM1_IER_CC4IE ((u8)0x10) /*!< Capture/Compare 4 Interrupt Enable mask. */
#define TIM1_IER_CC3IE ((u8)0x08) /*!< Capture/Compare 3 Interrupt Enable mask. */
#define TIM1_IER_CC2IE ((u8)0x04) /*!< Capture/Compare 2 Interrupt Enable mask. */
#define TIM1_IER_CC1IE ((u8)0x02) /*!< Capture/Compare 1 Interrupt Enable mask. */
#define TIM1_IER_UIE   ((u8)0x01) /*!< Update Interrupt Enable mask. */
/*SR1*/
#define TIM1_SR1_BIF   ((u8)0x80) /*!< Break Interrupt Flag mask. */
#define TIM1_SR1_TIF   ((u8)0x40) /*!< Trigger Interrupt Flag mask. */
#define TIM1_SR1_COMIF ((u8)0x20) /*!< Commutation Interrupt Flag mask. */
#define TIM1_SR1_CC4IF ((u8)0x10) /*!< Capture/Compare 4 Interrupt Flag mask. */
#define TIM1_SR1_CC3IF ((u8)0x08) /*!< Capture/Compare 3 Interrupt Flag mask. */
#define TIM1_SR1_CC2IF ((u8)0x04) /*!< Capture/Compare 2 Interrupt Flag mask. */
#define TIM1_SR1_CC1IF ((u8)0x02) /*!< Capture/Compare 1 Interrupt Flag mask. */
#define TIM1_SR1_UIF   ((u8)0x01) /*!< Update Interrupt Flag mask. */
/*SR2*/
#define TIM1_SR2_CC4OF ((u8)0x10) /*!< Capture/Compare 4 Overcapture Flag mask. */
#define TIM1_SR2_CC3OF ((u8)0x08) /*!< Capture/Compare 3 Overcapture Flag mask. */
#define TIM1_SR2_CC2OF ((u8)0x04) /*!< Capture/Compare 2 Overcapture Flag mask. */
#define TIM1_SR2_CC1OF ((u8)0x02) /*!< Capture/Compare 1 Overcapture Flag mask. */
/*EGR*/
#define TIM1_EGR_BG   ((u8)0x80) /*!< Break Generation mask. */
#define TIM1_EGR_TG   ((u8)0x40) /*!< Trigger Generation mask. */
#define TIM1_EGR_COMG ((u8)0x20) /*!< Capture/Compare Control Update Generation mask. */
#define TIM1_EGR_CC4G ((u8)0x10) /*!< Capture/Compare 4 Generation mask. */
#define TIM1_EGR_CC3G ((u8)0x08) /*!< Capture/Compare 3 Generation mask. */
#define TIM1_EGR_CC2G ((u8)0x04) /*!< Capture/Compare 2 Generation mask. */
#define TIM1_EGR_CC1G ((u8)0x02) /*!< Capture/Compare 1 Generation mask. */
#define TIM1_EGR_UG   ((u8)0x01) /*!< Update Generation mask. */
/*CCMR*/
#define TIM1_CCMR_ICxPSC ((u8)0x0C) /*!< Input Capture x Prescaler mask. */
#define TIM1_CCMR_ICxF   ((u8)0xF0) /*!< Input Capture x Filter mask. */
#define TIM1_CCMR_OCM    ((u8)0x70) /*!< Output Compare x Mode mask. */
#define TIM1_CCMR_OCxPE  ((u8)0x08) /*!< Output Compare x Preload Enable mask. */
#define TIM1_CCMR_OCxFE  ((u8)0x04) /*!< Output Compare x Fast Enable mask. */
#define TIM1_CCMR_CCxS   ((u8)0x03) /*!< Capture/Compare x Selection mask. */

#define CCMR_TIxDirect_Set ((u8)0x01)
/*CCER1*/
#define TIM1_CCER1_CC2NP ((u8)0x80) /*!< Capture/Compare 2 Complementary output Polarity mask. */
#define TIM1_CCER1_CC2NE ((u8)0x40) /*!< Capture/Compare 2 Complementary output enable mask. */
#define TIM1_CCER1_CC2P  ((u8)0x20) /*!< Capture/Compare 2 output Polarity mask. */
#define TIM1_CCER1_CC2E  ((u8)0x10) /*!< Capture/Compare 2 output enable mask. */
#define TIM1_CCER1_CC1NP ((u8)0x08) /*!< Capture/Compare 1 Complementary output Polarity mask. */
#define TIM1_CCER1_CC1NE ((u8)0x04) /*!< Capture/Compare 1 Complementary output enable mask. */
#define TIM1_CCER1_CC1P  ((u8)0x02) /*!< Capture/Compare 1 output Polarity mask. */
#define TIM1_CCER1_CC1E  ((u8)0x01) /*!< Capture/Compare 1 output enable mask. */
/*CCER2*/
#define TIM1_CCER2_CC4P  ((u8)0x20) /*!< Capture/Compare 4 output Polarity mask. */
#define TIM1_CCER2_CC4E  ((u8)0x10) /*!< Capture/Compare 4 output enable mask. */
#define TIM1_CCER2_CC3NP ((u8)0x08) /*!< Capture/Compare 3 Complementary output Polarity mask. */
#define TIM1_CCER2_CC3NE ((u8)0x04) /*!< Capture/Compare 3 Complementary output enable mask. */
#define TIM1_CCER2_CC3P  ((u8)0x02) /*!< Capture/Compare 3 output Polarity mask. */
#define TIM1_CCER2_CC3E  ((u8)0x01) /*!< Capture/Compare 3 output enable mask. */
/*CNTRH*/
#define TIM1_CNTRH_CNT ((u8)0xFF) /*!< Counter Value (MSB) mask. */
/*CNTRL*/
#define TIM1_CNTRL_CNT ((u8)0xFF) /*!< Counter Value (LSB) mask. */
/*PSCH*/
#define TIM1_PSCH_PSC ((u8)0xFF) /*!< Prescaler Value (MSB) mask. */
/*PSCL*/
#define TIM1_PSCL_PSC ((u8)0xFF) /*!< Prescaler Value (LSB) mask. */
/*ARR*/
#define TIM1_ARRH_ARR ((u8)0xFF) /*!< Autoreload Value (MSB) mask. */
#define TIM1_ARRL_ARR ((u8)0xFF) /*!< Autoreload Value (LSB) mask. */
/*RCR*/
#define TIM1_RCR_REP ((u8)0xFF) /*!< Repetition Counter Value mask. */
/*CCR1*/
#define TIM1_CCR1H_CCR1 ((u8)0xFF) /*!< Capture/Compare 1 Value (MSB) mask. */
#define TIM1_CCR1L_CCR1 ((u8)0xFF) /*!< Capture/Compare 1 Value (LSB) mask. */
/*CCR2*/
#define TIM1_CCR2H_CCR2 ((u8)0xFF) /*!< Capture/Compare 2 Value (MSB) mask. */
#define TIM1_CCR2L_CCR2 ((u8)0xFF) /*!< Capture/Compare 2 Value (LSB) mask. */
/*CCR3*/
#define TIM1_CCR3H_CCR3 ((u8)0xFF) /*!< Capture/Compare 3 Value (MSB) mask. */
#define TIM1_CCR3L_CCR3 ((u8)0xFF) /*!< Capture/Compare 3 Value (LSB) mask. */
/*CCR4*/
#define TIM1_CCR4H_CCR4 ((u8)0xFF) /*!< Capture/Compare 4 Value (MSB) mask. */
#define TIM1_CCR4L_CCR4 ((u8)0xFF) /*!< Capture/Compare 4 Value (LSB) mask. */
/*BKR*/
#define TIM1_BKR_MOE  ((u8)0x80) /*!< Main Output Enable mask. */
#define TIM1_BKR_AOE  ((u8)0x40) /*!< Automatic Output Enable mask. */
#define TIM1_BKR_BKP  ((u8)0x20) /*!< Break Polarity mask. */
#define TIM1_BKR_BKE  ((u8)0x10) /*!< Break Enable mask. */
#define TIM1_BKR_OSSR ((u8)0x08) /*!< Off-State Selection for Run mode mask. */
#define TIM1_BKR_OSSI ((u8)0x04) /*!< Off-State Selection for Idle mode mask. */
#define TIM1_BKR_LOCK ((u8)0x03) /*!< Lock Configuration mask. */
/*DTR*/
#define TIM1_DTR_DTG ((u8)0xFF) /*!< Dead-Time Generator set-up mask. */
/*OISR*/
#define TIM1_OISR_OIS4  ((u8)0x40) /*!< Output Idle state 4 (OC4 output) mask. */
#define TIM1_OISR_OIS3N ((u8)0x20) /*!< Output Idle state 3 (OC3N output) mask. */
#define TIM1_OISR_OIS3  ((u8)0x10) /*!< Output Idle state 3 (OC3 output) mask. */
#define TIM1_OISR_OIS2N ((u8)0x08) /*!< Output Idle state 2 (OC2N output) mask. */
#define TIM1_OISR_OIS2  ((u8)0x04) /*!< Output Idle state 2 (OC2 output) mask. */
#define TIM1_OISR_OIS1N ((u8)0x02) /*!< Output Idle state 1 (OC1N output) mask. */
#define TIM1_OISR_OIS1  ((u8)0x01) /*!< Output Idle state 1 (OC1 output) mask. */

/**
  * @}
  */

/*----------------------------------------------------------------------------*/
/**
  * @brief 16-bit timer (TIM2)
  */

typedef struct TIM2_struct
{
  vu8 CR1;   /*!< control register 1 */
#if defined STM8S103
	vu8 RESERVED1; /*!< Reserved register */
	vu8 RESERVED2; /*!< Reserved register */
#endif
  vu8 IER;   /*!< interrupt enable register */
  vu8 SR1;   /*!< status register 1 */
  vu8 SR2;   /*!< status register 2 */
  vu8 EGR;   /*!< event generation register */
  vu8 CCMR1; /*!< CC mode register 1 */
  vu8 CCMR2; /*!< CC mode register 2 */
  vu8 CCMR3; /*!< CC mode register 3 */
  vu8 CCER1; /*!< CC enable register 1 */
  vu8 CCER2; /*!< CC enable register 2 */
  vu8 CNTRH; /*!< counter high */
  vu8 CNTRL; /*!< counter low */
  vu8 PSCR;  /*!< prescaler register */
  vu8 ARRH;  /*!< auto-reload register high */
  vu8 ARRL;  /*!< auto-reload register low */
  vu8 CCR1H; /*!< capture/compare register 1 high */
  vu8 CCR1L; /*!< capture/compare register 1 low */
  vu8 CCR2H; /*!< capture/compare register 2 high */
  vu8 CCR2L; /*!< capture/compare register 2 low */
  vu8 CCR3H; /*!< capture/compare register 3 high */
  vu8 CCR3L; /*!< capture/compare register 3 low */
}
TIM2_TypeDef;

/** @addtogroup TIM2_Registers_Reset_Value
  * @{
  */

#define TIM2_CR1_RESET_VALUE   ((u8)0x00)
#define TIM2_IER_RESET_VALUE   ((u8)0x00)
#define TIM2_SR1_RESET_VALUE   ((u8)0x00)
#define TIM2_SR2_RESET_VALUE   ((u8)0x00)
#define TIM2_EGR_RESET_VALUE   ((u8)0x00)
#define TIM2_CCMR1_RESET_VALUE ((u8)0x00)
#define TIM2_CCMR2_RESET_VALUE ((u8)0x00)
#define TIM2_CCMR3_RESET_VALUE ((u8)0x00)
#define TIM2_CCER1_RESET_VALUE ((u8)0x00)
#define TIM2_CCER2_RESET_VALUE ((u8)0x00)
#define TIM2_CNTRH_RESET_VALUE ((u8)0x00)
#define TIM2_CNTRL_RESET_VALUE ((u8)0x00)
#define TIM2_PSCR_RESET_VALUE  ((u8)0x00)
#define TIM2_ARRH_RESET_VALUE  ((u8)0xFF)
#define TIM2_ARRL_RESET_VALUE  ((u8)0xFF)
#define TIM2_CCR1H_RESET_VALUE ((u8)0x00)
#define TIM2_CCR1L_RESET_VALUE ((u8)0x00)
#define TIM2_CCR2H_RESET_VALUE ((u8)0x00)
#define TIM2_CCR2L_RESET_VALUE ((u8)0x00)
#define TIM2_CCR3H_RESET_VALUE ((u8)0x00)
#define TIM2_CCR3L_RESET_VALUE ((u8)0x00)

/**
  * @}
  */

/** @addtogroup TIM2_Registers_Bits_Definition
  * @{
  */
/*CR1*/
#define TIM2_CR1_ARPE ((u8)0x80) /*!< Auto-Reload Preload Enable mask. */
#define TIM2_CR1_OPM  ((u8)0x08) /*!< One Pulse Mode mask. */
#define TIM2_CR1_URS  ((u8)0x04) /*!< Update Request Source mask. */
#define TIM2_CR1_UDIS ((u8)0x02) /*!< Update DIsable mask. */
#define TIM2_CR1_CEN  ((u8)0x01) /*!< Counter Enable mask. */
/*IER*/
#define TIM2_IER_CC3IE ((u8)0x08) /*!< Capture/Compare 3 Interrupt Enable mask. */
#define TIM2_IER_CC2IE ((u8)0x04) /*!< Capture/Compare 2 Interrupt Enable mask. */
#define TIM2_IER_CC1IE ((u8)0x02) /*!< Capture/Compare 1 Interrupt Enable mask. */
#define TIM2_IER_UIE   ((u8)0x01) /*!< Update Interrupt Enable mask. */
/*SR1*/
#define TIM2_SR1_CC3IF ((u8)0x08) /*!< Capture/Compare 3 Interrupt Flag mask. */
#define TIM2_SR1_CC2IF ((u8)0x04) /*!< Capture/Compare 2 Interrupt Flag mask. */
#define TIM2_SR1_CC1IF ((u8)0x02) /*!< Capture/Compare 1 Interrupt Flag mask. */
#define TIM2_SR1_UIF   ((u8)0x01) /*!< Update Interrupt Flag mask. */
/*SR2*/
#define TIM2_SR2_CC3OF ((u8)0x08) /*!< Capture/Compare 3 Overcapture Flag mask. */
#define TIM2_SR2_CC2OF ((u8)0x04) /*!< Capture/Compare 2 Overcapture Flag mask. */
#define TIM2_SR2_CC1OF ((u8)0x02) /*!< Capture/Compare 1 Overcapture Flag mask. */
/*EGR*/
#define TIM2_EGR_CC3G  ((u8)0x08) /*!< Capture/Compare 3 Generation mask. */
#define TIM2_EGR_CC2G  ((u8)0x04) /*!< Capture/Compare 2 Generation mask. */
#define TIM2_EGR_CC1G  ((u8)0x02) /*!< Capture/Compare 1 Generation mask. */
#define TIM2_EGR_UG    ((u8)0x01) /*!< Update Generation mask. */
/*CCMR*/
#define TIM2_CCMR_ICxPSC ((u8)0x0C) /*!< Input Capture x Prescaler mask. */
#define TIM2_CCMR_ICxF   ((u8)0xF0) /*!< Input Capture x Filter mask. */
#define TIM2_CCMR_OCM    ((u8)0x70) /*!< Output Compare x Mode mask. */
#define TIM2_CCMR_OCxPE  ((u8)0x08) /*!< Output Compare x Preload Enable mask. */
#define TIM2_CCMR_CCxS   ((u8)0x03) /*!< Capture/Compare x Selection mask. */
/*CCER1*/
#define TIM2_CCER1_CC2P ((u8)0x20) /*!< Capture/Compare 2 output Polarity mask. */
#define TIM2_CCER1_CC2E ((u8)0x10) /*!< Capture/Compare 2 output enable mask. */
#define TIM2_CCER1_CC1P ((u8)0x02) /*!< Capture/Compare 1 output Polarity mask. */
#define TIM2_CCER1_CC1E ((u8)0x01) /*!< Capture/Compare 1 output enable mask. */
/*CCER2*/
#define TIM2_CCER2_CC3P ((u8)0x02) /*!< Capture/Compare 3 output Polarity mask. */
#define TIM2_CCER2_CC3E ((u8)0x01) /*!< Capture/Compare 3 output enable mask. */
/*CNTR*/
#define TIM2_CNTRH_CNT ((u8)0xFF) /*!< Counter Value (MSB) mask. */
#define TIM2_CNTRL_CNT ((u8)0xFF) /*!< Counter Value (LSB) mask. */
/*PSCR*/
#define TIM2_PSCR_PSC ((u8)0xFF) /*!< Prescaler Value (MSB) mask. */
/*ARR*/
#define TIM2_ARRH_ARR ((u8)0xFF) /*!< Autoreload Value (MSB) mask. */
#define TIM2_ARRL_ARR ((u8)0xFF) /*!< Autoreload Value (LSB) mask. */
/*CCR1*/
#define TIM2_CCR1H_CCR1 ((u8)0xFF) /*!< Capture/Compare 1 Value (MSB) mask. */
#define TIM2_CCR1L_CCR1 ((u8)0xFF) /*!< Capture/Compare 1 Value (LSB) mask. */
/*CCR2*/
#define TIM2_CCR2H_CCR2 ((u8)0xFF) /*!< Capture/Compare 2 Value (MSB) mask. */
#define TIM2_CCR2L_CCR2 ((u8)0xFF) /*!< Capture/Compare 2 Value (LSB) mask. */
/*CCR3*/
#define TIM2_CCR3H_CCR3 ((u8)0xFF) /*!< Capture/Compare 3 Value (MSB) mask. */
#define TIM2_CCR3L_CCR3 ((u8)0xFF) /*!< Capture/Compare 3 Value (LSB) mask. */

/**
  * @}
  */

/*----------------------------------------------------------------------------*/
/**
  * @brief 16-bit timer (TIM3)
  */
typedef struct TIM3_struct
{
  vu8 CR1;   /*!< control register 1 */
  vu8 IER;   /*!< interrupt enable register */
  vu8 SR1;   /*!< status register 1 */
  vu8 SR2;   /*!< status register 2 */
  vu8 EGR;   /*!< event generation register */
  vu8 CCMR1; /*!< CC mode register 1 */
  vu8 CCMR2; /*!< CC mode register 2 */
  vu8 CCER1; /*!< CC enable register 1 */
  vu8 CNTRH; /*!< counter high */
  vu8 CNTRL; /*!< counter low */
  vu8 PSCR;  /*!< prescaler register */
  vu8 ARRH;  /*!< auto-reload register high */
  vu8 ARRL;  /*!< auto-reload register low */
  vu8 CCR1H; /*!< capture/compare register 1 high */
  vu8 CCR1L; /*!< capture/compare register 1 low */
  vu8 CCR2H; /*!< capture/compare register 2 high */
  vu8 CCR2L; /*!< capture/compare register 2 low */
}
TIM3_TypeDef;

/** @addtogroup TIM3_Registers_Reset_Value
  * @{
  */

#define TIM3_CR1_RESET_VALUE   ((u8)0x00)
#define TIM3_IER_RESET_VALUE   ((u8)0x00)
#define TIM3_SR1_RESET_VALUE   ((u8)0x00)
#define TIM3_SR2_RESET_VALUE   ((u8)0x00)
#define TIM3_EGR_RESET_VALUE   ((u8)0x00)
#define TIM3_CCMR1_RESET_VALUE ((u8)0x00)
#define TIM3_CCMR2_RESET_VALUE ((u8)0x00)
#define TIM3_CCER1_RESET_VALUE ((u8)0x00)
#define TIM3_CNTRH_RESET_VALUE ((u8)0x00)
#define TIM3_CNTRL_RESET_VALUE ((u8)0x00)
#define TIM3_PSCR_RESET_VALUE  ((u8)0x00)
#define TIM3_ARRH_RESET_VALUE  ((u8)0xFF)
#define TIM3_ARRL_RESET_VALUE  ((u8)0xFF)
#define TIM3_CCR1H_RESET_VALUE ((u8)0x00)
#define TIM3_CCR1L_RESET_VALUE ((u8)0x00)
#define TIM3_CCR2H_RESET_VALUE ((u8)0x00)
#define TIM3_CCR2L_RESET_VALUE ((u8)0x00)

/**
  * @}
  */

/** @addtogroup TIM3_Registers_Bits_Definition
  * @{
  */
/*CR1*/
#define TIM3_CR1_ARPE ((u8)0x80) /*!< Auto-Reload Preload Enable mask. */
#define TIM3_CR1_OPM  ((u8)0x08) /*!< One Pulse Mode mask. */
#define TIM3_CR1_URS  ((u8)0x04) /*!< Update Request Source mask. */
#define TIM3_CR1_UDIS ((u8)0x02) /*!< Update DIsable mask. */
#define TIM3_CR1_CEN  ((u8)0x01) /*!< Counter Enable mask. */
/*IER*/
#define TIM3_IER_CC2IE ((u8)0x04) /*!< Capture/Compare 2 Interrupt Enable mask. */
#define TIM3_IER_CC1IE ((u8)0x02) /*!< Capture/Compare 1 Interrupt Enable mask. */
#define TIM3_IER_UIE   ((u8)0x01) /*!< Update Interrupt Enable mask. */
/*SR1*/
#define TIM3_SR1_CC2IF ((u8)0x04) /*!< Capture/Compare 2 Interrupt Flag mask. */
#define TIM3_SR1_CC1IF ((u8)0x02) /*!< Capture/Compare 1 Interrupt Flag mask. */
#define TIM3_SR1_UIF   ((u8)0x01) /*!< Update Interrupt Flag mask. */
/*SR2*/
#define TIM3_SR2_CC2OF ((u8)0x04) /*!< Capture/Compare 2 Overcapture Flag mask. */
#define TIM3_SR2_CC1OF ((u8)0x02) /*!< Capture/Compare 1 Overcapture Flag mask. */
/*EGR*/
#define TIM3_EGR_CC2G ((u8)0x04) /*!< Capture/Compare 2 Generation mask. */
#define TIM3_EGR_CC1G ((u8)0x02) /*!< Capture/Compare 1 Generation mask. */
#define TIM3_EGR_UG   ((u8)0x01) /*!< Update Generation mask. */
/*CCMR*/
#define TIM3_CCMR_ICxPSC ((u8)0x0C) /*!< Input Capture x Prescaler mask. */
#define TIM3_CCMR_ICxF   ((u8)0xF0) /*!< Input Capture x Filter mask. */
#define TIM3_CCMR_OCM    ((u8)0x70) /*!< Output Compare x Mode mask. */
#define TIM3_CCMR_OCxPE  ((u8)0x08) /*!< Output Compare x Preload Enable mask. */
#define TIM3_CCMR_CCxS   ((u8)0x03) /*!< Capture/Compare x Selection mask. */
/*CCER1*/
#define TIM3_CCER1_CC2P ((u8)0x20) /*!< Capture/Compare 2 output Polarity mask. */
#define TIM3_CCER1_CC2E ((u8)0x10) /*!< Capture/Compare 2 output enable mask. */
#define TIM3_CCER1_CC1P ((u8)0x02) /*!< Capture/Compare 1 output Polarity mask. */
#define TIM3_CCER1_CC1E ((u8)0x01) /*!< Capture/Compare 1 output enable mask. */
/*CNTR*/
#define TIM3_CNTRH_CNT ((u8)0xFF) /*!< Counter Value (MSB) mask. */
#define TIM3_CNTRL_CNT ((u8)0xFF) /*!< Counter Value (LSB) mask. */
/*PSCR*/
#define TIM3_PSCR_PSC ((u8)0xFF) /*!< Prescaler Value (MSB) mask. */
/*ARR*/
#define TIM3_ARRH_ARR ((u8)0xFF) /*!< Autoreload Value (MSB) mask. */
#define TIM3_ARRL_ARR ((u8)0xFF) /*!< Autoreload Value (LSB) mask. */
/*CCR1*/
#define TIM3_CCR1H_CCR1 ((u8)0xFF) /*!< Capture/Compare 1 Value (MSB) mask. */
#define TIM3_CCR1L_CCR1 ((u8)0xFF) /*!< Capture/Compare 1 Value (LSB) mask. */
/*CCR2*/
#define TIM3_CCR2H_CCR2 ((u8)0xFF) /*!< Capture/Compare 2 Value (MSB) mask. */
#define TIM3_CCR2L_CCR2 ((u8)0xFF) /*!< Capture/Compare 2 Value (LSB) mask. */

/**
  * @}
  */

/*----------------------------------------------------------------------------*/
/**
  * @brief 8-bit system timer (TIM4)
  */

typedef struct TIM4_struct
{
  vu8 CR1;  /*!< control register 1 */
#if defined STM8S103
	vu8 RESERVED1; /*!< Reserved register */
	vu8 RESERVED2; /*!< Reserved register */
#endif
  vu8 IER;  /*!< interrupt enable register */
  vu8 SR1;  /*!< status register 1 */
  vu8 EGR;  /*!< event generation register */
  vu8 CNTR; /*!< counter register */
  vu8 PSCR; /*!< prescaler register */
  vu8 ARR;  /*!< auto-reload register */
}
TIM4_TypeDef;

/** @addtogroup TIM4_Registers_Reset_Value
  * @{
  */

#define TIM4_CR1_RESET_VALUE  ((u8)0x00)
#define TIM4_IER_RESET_VALUE  ((u8)0x00)
#define TIM4_SR1_RESET_VALUE  ((u8)0x00)
#define TIM4_EGR_RESET_VALUE  ((u8)0x00)
#define TIM4_CNTR_RESET_VALUE ((u8)0x00)
#define TIM4_PSCR_RESET_VALUE ((u8)0x00)
#define TIM4_ARR_RESET_VALUE  ((u8)0xFF)

/**
  * @}
  */

/** @addtogroup TIM4_Registers_Bits_Definition
  * @{
  */
/*CR1*/
#define TIM4_CR1_ARPE ((u8)0x80) /*!< Auto-Reload Preload Enable mask. */
#define TIM4_CR1_OPM  ((u8)0x08) /*!< One Pulse Mode mask. */
#define TIM4_CR1_URS  ((u8)0x04) /*!< Update Request Source mask. */
#define TIM4_CR1_UDIS ((u8)0x02) /*!< Update DIsable mask. */
#define TIM4_CR1_CEN  ((u8)0x01) /*!< Counter Enable mask. */
/*IER*/
#define TIM4_IER_UIE ((u8)0x01) /*!< Update Interrupt Enable mask. */
/*SR1*/
#define TIM4_SR1_UIF ((u8)0x01) /*!< Update Interrupt Flag mask. */
/*EGR*/
#define TIM4_EGR_UG ((u8)0x01) /*!< Update Generation mask. */
/*CNTR*/
#define TIM4_CNTR_CNT ((u8)0xFF) /*!< Counter Value (LSB) mask. */
/*PSCR*/
#define TIM4_PSCR_PSC ((u8)0x07) /*!< Prescaler Value  mask. */
/*ARR*/
#define TIM4_ARR_ARR ((u8)0xFF) /*!< Autoreload Value mask. */

/**
  * @}
  */

/*----------------------------------------------------------------------------*/
/**
  * @brief 16-bit timer with synchro module (TIM5)
  */

typedef struct TIM5_struct
{
  vu8 CR1;       /*!<TIM5 Control Register 1                */
  vu8 CR2;       /*!<TIM5 Control Register 2                */
  vu8 SMCR;      /*!<TIM5 Slave Mode Control Register       */
  vu8 IER;       /*!<TIM5 Interrupt Enable Register         */
  vu8 SR1;       /*!<TIM5 Status Register 1                 */
  vu8 SR2;       /*!<TIM5 Status Register 2                 */
  vu8 EGR;       /*!<TIM5 Event Generation Register         */
  vu8 CCMR1;     /*!<TIM5 Capture/Compare Mode Register 1   */
  vu8 CCMR2;     /*!<TIM5 Capture/Compare Mode Register 2   */
  vu8 CCMR3;     /*!<TIM5 Capture/Compare Mode Register 3   */
  vu8 CCER1;     /*!<TIM5 Capture/Compare Enable Register 1 */
  vu8 CCER2;     /*!<TIM5 Capture/Compare Enable Register 2 */
  vu8 CNTRH;     /*!<TIM5 Counter High                      */
  vu8 CNTRL;     /*!<TIM5 Counter Low                       */
  vu8 PSCR;      /*!<TIM5 Prescaler Register                */
  vu8 ARRH;      /*!<TIM5 Auto-Reload Register High         */
  vu8 ARRL;      /*!<TIM5 Auto-Reload Register Low          */
  vu8 CCR1H;     /*!<TIM5 Capture/Compare Register 1 High   */
  vu8 CCR1L;     /*!<TIM5 Capture/Compare Register 1 Low    */
  vu8 CCR2H;     /*!<TIM5 Capture/Compare Register 2 High   */
  vu8 CCR2L;     /*!<TIM5 Capture/Compare Register 2 Low    */
  vu8 CCR3H;     /*!<TIM5 Capture/Compare Register 3 High   */
  vu8 CCR3L;     /*!<TIM5 Capture/Compare Register 3 Low    */
}TIM5_TypeDef;

/** @addtogroup TIM5_Registers_Reset_Value
  * @{
  */

#define TIM5_CR1_RESET_VALUE   ((u8)0x00)
#define TIM5_CR2_RESET_VALUE 	((u8)0x00)
#define TIM5_SMCR_RESET_VALUE	((u8)0x00)
#define TIM5_IER_RESET_VALUE   ((u8)0x00)
#define TIM5_SR1_RESET_VALUE   ((u8)0x00)
#define TIM5_SR2_RESET_VALUE   ((u8)0x00)
#define TIM5_EGR_RESET_VALUE   ((u8)0x00)
#define TIM5_CCMR1_RESET_VALUE ((u8)0x00)
#define TIM5_CCMR2_RESET_VALUE ((u8)0x00)
#define TIM5_CCMR3_RESET_VALUE ((u8)0x00)
#define TIM5_CCER1_RESET_VALUE ((u8)0x00)
#define TIM5_CCER2_RESET_VALUE ((u8)0x00)
#define TIM5_CNTRH_RESET_VALUE ((u8)0x00)
#define TIM5_CNTRL_RESET_VALUE ((u8)0x00)
#define TIM5_PSCR_RESET_VALUE  ((u8)0x00)
#define TIM5_ARRH_RESET_VALUE  ((u8)0xFF)
#define TIM5_ARRL_RESET_VALUE  ((u8)0xFF)
#define TIM5_CCR1H_RESET_VALUE ((u8)0x00)
#define TIM5_CCR1L_RESET_VALUE ((u8)0x00)
#define TIM5_CCR2H_RESET_VALUE ((u8)0x00)
#define TIM5_CCR2L_RESET_VALUE ((u8)0x00)
#define TIM5_CCR3H_RESET_VALUE ((u8)0x00)
#define TIM5_CCR3L_RESET_VALUE ((u8)0x00)

/**
  * @}
  */

/** @addtogroup TIM5_Registers_Bits_Definition
  * @{
  */
/* CR1*/
#define TIM5_CR1_ARPE ((u8)0x80) /*!< Auto-Reload Preload Enable mask. */
#define TIM5_CR1_OPM  ((u8)0x08) /*!< One Pulse Mode mask. */
#define TIM5_CR1_URS  ((u8)0x04) /*!< Update Request Source mask. */
#define TIM5_CR1_UDIS ((u8)0x02) /*!< Update DIsable mask. */
#define TIM5_CR1_CEN  ((u8)0x01) /*!< Counter Enable mask. */

/* CR2*/
#define TIM5_CR2_TI1S 	  ((u8)0x80) /*!< TI1S Selection Mask. */
#define	TIM5_CR2_MMS	  ((u8)0x70) /*!< MMS Selection Mask. */

/* SMCR*/
#define TIM5_SMCR_MSM      ((u8)0x80) /*!< Master/Slave Mode Mask. */
#define TIM5_SMCR_TS       ((u8)0x70) /*!< Trigger Selection Mask. */
#define TIM5_SMCR_SMS      ((u8)0x07) /*!< Slave Mode Selection Mask. */


/*IER*/
#define TIM5_IER_TIE   ((u8)0x40) /*!< Trigger Interrupt Enable mask. */
#define TIM5_IER_CC3IE ((u8)0x08) /*!< Capture/Compare 3 Interrupt Enable mask. */
#define TIM5_IER_CC2IE ((u8)0x04) /*!< Capture/Compare 2 Interrupt Enable mask. */
#define TIM5_IER_CC1IE ((u8)0x02) /*!< Capture/Compare 1 Interrupt Enable mask. */
#define TIM5_IER_UIE   ((u8)0x01) /*!< Update Interrupt Enable mask. */
/*SR1*/
#define TIM5_SR1_TIF   ((u8)0x40) /*!< Trigger Interrupt Flag mask. */
#define TIM5_SR1_CC3IF ((u8)0x08) /*!< Capture/Compare 3 Interrupt Flag mask. */
#define TIM5_SR1_CC2IF ((u8)0x04) /*!< Capture/Compare 2 Interrupt Flag mask. */
#define TIM5_SR1_CC1IF ((u8)0x02) /*!< Capture/Compare 1 Interrupt Flag mask. */
#define TIM5_SR1_UIF   ((u8)0x01) /*!< Update Interrupt Flag mask. */
/*SR2*/
#define TIM5_SR2_CC3OF ((u8)0x08) /*!< Capture/Compare 3 Overcapture Flag mask. */
#define TIM5_SR2_CC2OF ((u8)0x04) /*!< Capture/Compare 2 Overcapture Flag mask. */
#define TIM5_SR2_CC1OF ((u8)0x02) /*!< Capture/Compare 1 Overcapture Flag mask. */
/*EGR*/
#define TIM5_EGR_TG    ((u8)0x40) /*!< Trigger Generation mask. */
#define TIM5_EGR_CC3G  ((u8)0x08) /*!< Capture/Compare 3 Generation mask. */
#define TIM5_EGR_CC2G  ((u8)0x04) /*!< Capture/Compare 2 Generation mask. */
#define TIM5_EGR_CC1G  ((u8)0x02) /*!< Capture/Compare 1 Generation mask. */
#define TIM5_EGR_UG    ((u8)0x01) /*!< Update Generation mask. */
/*CCMR*/
#define TIM5_CCMR_ICxPSC ((u8)0x0C) /*!< Input Capture x Prescaler mask. */
#define TIM5_CCMR_ICxF   ((u8)0xF0) /*!< Input Capture x Filter mask. */
#define TIM5_CCMR_OCM    ((u8)0x70) /*!< Output Compare x Mode mask. */
#define	TIM5_CCMR_OCxPE  ((u8)0x08) /*!< Output Compare x Preload Enable mask. */
#define TIM5_CCMR_CCxS   ((u8)0x03) /*!< Capture/Compare x Selection mask. */
/*CCER1*/
#define TIM5_CCER1_CC2P ((u8)0x20) /*!< Capture/Compare 2 output Polarity mask. */
#define TIM5_CCER1_CC2E ((u8)0x10) /*!< Capture/Compare 2 output enable mask. */
#define TIM5_CCER1_CC1P ((u8)0x02) /*!< Capture/Compare 1 output Polarity mask. */
#define TIM5_CCER1_CC1E ((u8)0x01) /*!< Capture/Compare 1 output enable mask. */
/*CCER2*/
#define TIM5_CCER2_CC3P ((u8)0x02) /*!< Capture/Compare 3 output Polarity mask. */
#define TIM5_CCER2_CC3E ((u8)0x01) /*!< Capture/Compare 3 output enable mask. */
/*CNTR*/
#define TIM5_CNTRH_CNT ((u8)0xFF) /*!< Counter Value (MSB) mask. */
#define TIM5_CNTRL_CNT ((u8)0xFF) /*!< Counter Value (LSB) mask. */
/*PSCR*/
#define TIM5_PSCR_PSC ((u8)0xFF) /*!< Prescaler Value (MSB) mask. */
/*ARR*/
#define TIM5_ARRH_ARR ((u8)0xFF) /*!< Autoreload Value (MSB) mask. */
#define TIM5_ARRL_ARR ((u8)0xFF) /*!< Autoreload Value (LSB) mask. */
/*CCR1*/
#define TIM5_CCR1H_CCR1 ((u8)0xFF) /*!< Capture/Compare 1 Value (MSB) mask. */
#define TIM5_CCR1L_CCR1 ((u8)0xFF) /*!< Capture/Compare 1 Value (LSB) mask. */
/*CCR2*/
#define TIM5_CCR2H_CCR2 ((u8)0xFF) /*!< Capture/Compare 2 Value (MSB) mask. */
#define TIM5_CCR2L_CCR2 ((u8)0xFF) /*!< Capture/Compare 2 Value (LSB) mask. */
/*CCR3*/
#define TIM5_CCR3H_CCR3 ((u8)0xFF) /*!< Capture/Compare 3 Value (MSB) mask. */
#define TIM5_CCR3L_CCR3 ((u8)0xFF) /*!< Capture/Compare 3 Value (LSB) mask. */
/*CCMR*/
#define TIM5_CCMR_TIxDirect_Set   ((u8)0x01)
/**
  * @}
  */
	
/*----------------------------------------------------------------------------*/
/**
  * @brief 8-bit system timer  with synchro module(TIM6)
  */

typedef struct TIM6_struct
{
    vu8 CR1; 	/*!< control register 1 */
    vu8 CR2; 	/*!< control register 2 */
    vu8 SMCR; 	/*!< Synchro mode control register */
    vu8 IER; 	/*!< interrupt enable register  */
    vu8 SR1; 	/*!< status register 1    */
    vu8 EGR; 	/*!< event generation register */
    vu8 CNTR; 	/*!< counter register  */
    vu8 PSCR; 	/*!< prescaler register */
    vu8 ARR; 	/*!< auto-reload register */
}
TIM6_TypeDef;
/** @addtogroup TIM6_Registers_Reset_Value
  * @{
  */
#define TIM6_CR1_RESET_VALUE    ((u8)0x00)
#define TIM6_CR2_RESET_VALUE    ((u8)0x00)
#define TIM6_SMCR_RESET_VALUE   ((u8)0x00)
#define TIM6_IER_RESET_VALUE    ((u8)0x00)
#define TIM6_SR1_RESET_VALUE    ((u8)0x00)
#define TIM6_EGR_RESET_VALUE    ((u8)0x00)
#define TIM6_CNTR_RESET_VALUE   ((u8)0x00)
#define TIM6_PSCR_RESET_VALUE   ((u8)0x00)
#define TIM6_ARR_RESET_VALUE    ((u8)0xFF)

/**
* @}
*/

/** @addtogroup TIM6_Registers_Bits_Definition
  * @{
  */
/* CR1*/
#define TIM6_CR1_ARPE     ((u8)0x80) /*!< Auto-Reload Preload Enable Mask. */
#define TIM6_CR1_OPM      ((u8)0x08) /*!< One Pulse Mode Mask. */
#define TIM6_CR1_URS      ((u8)0x04) /*!< Update Request Source Mask. */
#define TIM6_CR1_UDIS     ((u8)0x02) /*!< Update DIsable Mask. */
#define TIM6_CR1_CEN      ((u8)0x01) /*!< Counter Enable Mask. */

/* CR2*/

#define	TIM6_CR2_MMS	  ((u8)0x70) /*!< MMS Selection Mask. */

/* SMCR*/
#define TIM6_SMCR_MSM      ((u8)0x80) /*!< Master/Slave Mode Mask. */
#define TIM6_SMCR_TS       ((u8)0x70) /*!< Trigger Selection Mask. */
#define TIM6_SMCR_SMS      ((u8)0x07) /*!< Slave Mode Selection Mask. */

/* IER*/
#define TIM6_IER_TIE       ((u8)0x40) /*!< Trigger Interrupt Enable Mask. */
#define TIM6_IER_UIE       ((u8)0x01) /*!< Update Interrupt Enable Mask. */
/* SR1*/
#define TIM6_SR1_TIF       ((u8)0x40) /*!< Trigger Interrupt Flag mask. */
#define TIM6_SR1_UIF       ((u8)0x01) /*!< Update Interrupt Flag Mask. */
/* EGR*/
#define TIM6_EGR_TG   ((u8)0x40) /*!< Trigger Generation mask. */
#define TIM6_EGR_UG        ((u8)0x01) /*!< Update Generation Mask. */
/* CNTR*/
#define TIM6_CNTR_CNT      ((u8)0xFF) /*!<Counter Value (LSB) Mask. */
/* PSCR*/
#define TIM6_PSCR_PSC      ((u8)0x07) /*!<Prescaler Value  Mask. */

#define TIM6_ARR_ARR 	   ((u8)0xFF) /*!<Autoreload Value Mask. */
/**
  * @}
  */
/*----------------------------------------------------------------------------*/
/**
  * @brief Inter-Integrated Circuit (I2C)
  */

typedef struct I2C_struct
{
  vu8 CR1;       /*!< I2C control register 1 */
  vu8 CR2;       /*!< I2C control register 2 */
  vu8 FREQR;     /*!< I2C frequency register */
  vu8 OARL;      /*!< I2C own address register LSB */
  vu8 OARH;      /*!< I2C own address register MSB */
  vu8 RESERVED1; /*!< Reserved byte */
  vu8 DR;        /*!< I2C data register */
  vu8 SR1;       /*!< I2C status register 1 */
  vu8 SR2;       /*!< I2C status register 2 */
  vu8 SR3;       /*!< I2C status register 3 */
  vu8 ITR;       /*!< I2C interrupt register */
  vu8 CCRL;      /*!< I2C clock control register low */
  vu8 CCRH;      /*!< I2C clock control register high */
  vu8 TRISER;    /*!< I2C maximum rise time register */
  vu8 RESERVED2; /*!< Reserved byte */
}
I2C_TypeDef;

/** @addtogroup I2C_Registers_Reset_Value
  * @{
  */

#define I2C_CR1_RESET_VALUE    ((u8)0x00)
#define I2C_CR2_RESET_VALUE    ((u8)0x00)
#define I2C_FREQR_RESET_VALUE  ((u8)0x00)
#define I2C_OARL_RESET_VALUE   ((u8)0x00)
#define I2C_OARH_RESET_VALUE   ((u8)0x00)
#define I2C_DR_RESET_VALUE     ((u8)0x00)
#define I2C_SR1_RESET_VALUE    ((u8)0x00)
#define I2C_SR2_RESET_VALUE    ((u8)0x00)
#define I2C_SR3_RESET_VALUE    ((u8)0x00)
#define I2C_ITR_RESET_VALUE    ((u8)0x00)
#define I2C_CCRL_RESET_VALUE   ((u8)0x00)
#define I2C_CCRH_RESET_VALUE   ((u8)0x00)
#define I2C_TRISER_RESET_VALUE ((u8)0x02)

/**
  * @}
  */

/** @addtogroup I2C_Registers_Bits_Definition
  * @{
  */

#define I2C_CR1_NOSTRETCH ((u8)0x80) /*!< Clock Stretching Disable (Slave mode) */
#define I2C_CR1_ENGC      ((u8)0x40) /*!< General Call Enable */
#define I2C_CR1_PE        ((u8)0x01) /*!< Peripheral Enable */

#define I2C_CR2_SWRST ((u8)0x80) /*!< Software Reset */
#define I2C_CR2_POS   ((u8)0x08) /*!< Acknowledge */
#define I2C_CR2_ACK   ((u8)0x04) /*!< Acknowledge Enable */
#define I2C_CR2_STOP  ((u8)0x02) /*!< Stop Generation */
#define I2C_CR2_START ((u8)0x01) /*!< Start Generation */

#define I2C_FREQR_FREQ ((u8)0x3F) /*!< Peripheral Clock Frequency */

#define I2C_OARL_ADD  ((u8)0xFE) /*!< Interface Address bits [7..1] */
#define I2C_OARL_ADD0 ((u8)0x01) /*!< Interface Address bit0 */

#define I2C_OARH_ADDMODE ((u8)0x80) /*!< Addressing Mode (Slave mode) */
#define I2C_OARH_ADDCONF ((u8)0x40) /*!< Address Mode Configuration */
#define I2C_OARH_ADD     ((u8)0x06) /*!< Interface Address bits [9..8] */

#define I2C_DR_DR ((u8)0xFF) /*!< Data Register */

#define I2C_SR1_TXE   ((u8)0x80) /*!< Data Register Empty (transmitters) */
#define I2C_SR1_RXNE  ((u8)0x40) /*!< Data Register not Empty (receivers) */
#define I2C_SR1_STOPF ((u8)0x10) /*!< Stop detection (Slave mode) */
#define I2C_SR1_ADD10 ((u8)0x08) /*!< 10-bit header sent (Master mode) */
#define I2C_SR1_BTF   ((u8)0x04) /*!< Byte Transfer Finished */
#define I2C_SR1_ADDR  ((u8)0x02) /*!< Address sent (master mode)/matched (slave mode) */
#define I2C_SR1_SB    ((u8)0x01) /*!< Start Bit (Master mode) */

#define I2C_SR2_WUFH    ((u8)0x20) /*!< Wake-up from Halt */
#define I2C_SR2_OVR     ((u8)0x08) /*!< Overrun/Underrun */
#define I2C_SR2_AF      ((u8)0x04) /*!< Acknowledge Failure */
#define I2C_SR2_ARLO    ((u8)0x02) /*!< Arbitration Lost (master mode) */
#define I2C_SR2_BERR    ((u8)0x01) /*!< Bus Error */

#define I2C_SR3_GENCALL ((u8)0x10) /*!< General Call Header (Slave mode) */
#define I2C_SR3_TRA     ((u8)0x04) /*!< Transmitter/Receiver */
#define I2C_SR3_BUSY    ((u8)0x02) /*!< Bus Busy */
#define I2C_SR3_MSL     ((u8)0x01) /*!< Master/Slave */

#define I2C_ITR_ITBUFEN ((u8)0x04) /*!< Buffer Interrupt Enable */
#define I2C_ITR_ITEVTEN ((u8)0x02) /*!< Event Interrupt Enable */
#define I2C_ITR_ITERREN ((u8)0x01) /*!< Error Interrupt Enable */

#define I2C_CCRL_CCR ((u8)0xFF) /*!< Clock Control Register (Master mode) */

#define I2C_CCRH_FS   ((u8)0x80) /*!< Master Mode Selection */
#define I2C_CCRH_DUTY ((u8)0x40) /*!< Fast Mode Duty Cycle */
#define I2C_CCRH_CCR  ((u8)0x0F) /*!< Clock Control Register in Fast/Standard mode (Master mode) bits [11..8] */

#define I2C_TRISER_TRISE ((u8)0x3F) /*!< Maximum Rise Time in Fast/Standard mode (Master mode) */

/**
  * @}
  */

/*----------------------------------------------------------------------------*/
/**
  * @brief Interrupt Controller (ITC)
  */

typedef struct ITC_struct
{
  vu8 ISPR1; /*!< Interrupt Software Priority register 1 */
  vu8 ISPR2; /*!< Interrupt Software Priority register 2 */
  vu8 ISPR3; /*!< Interrupt Software Priority register 3 */
  vu8 ISPR4; /*!< Interrupt Software Priority register 4 */
  vu8 ISPR5; /*!< Interrupt Software Priority register 5 */
  vu8 ISPR6; /*!< Interrupt Software Priority register 6 */
  vu8 ISPR7; /*!< Interrupt Software Priority register 7 */
  vu8 ISPR8; /*!< Interrupt Software Priority register 8 */
}
ITC_TypeDef;

/** @addtogroup ITC_Registers_Reset_Value
  * @{
  */

#define ITC_SPRX_RESET_VALUE ((u8)0xFF) /*!< Reset value of Software Priority registers */

/**
  * @}
  */

/** @addtogroup CPU_Registers_Bits_Definition
  * @{
  */

#define CPU_CC_I1I0 ((u8)0x28) /*!< Condition Code register, I1 and I0 bits mask */

/**
  * @}
  */

/*----------------------------------------------------------------------------*/
/**
  * @brief External Interrupt Controller (EXTI)
  */

typedef struct EXTI_struct
{
  vu8 CR1; /*!< External Interrupt Control Register for PORTA to PORTD */
  vu8 CR2; /*!< External Interrupt Control Register for PORTE and TLI */
}
EXTI_TypeDef;

/** @addtogroup EXTI_Registers_Reset_Value
  * @{
  */

#define EXTI_CR1_RESET_VALUE ((u8)0x00)
#define EXTI_CR2_RESET_VALUE ((u8)0x00)

/**
  * @}
  */

/** @addtogroup EXTI_Registers_Bits_Definition
  * @{
  */

#define EXTI_CR1_PDIS ((u8)0xC0) /*!< PORTD external interrupt sensitivity bits mask */
#define EXTI_CR1_PCIS ((u8)0x30) /*!< PORTC external interrupt sensitivity bits mask */
#define EXTI_CR1_PBIS ((u8)0x0C) /*!< PORTB external interrupt sensitivity bits mask */
#define EXTI_CR1_PAIS ((u8)0x03) /*!< PORTA external interrupt sensitivity bits mask */

#define EXTI_CR2_TLIS ((u8)0x04) /*!< Top level interrupt sensitivity bit mask */
#define EXTI_CR2_PEIS ((u8)0x03) /*!< PORTE external interrupt sensitivity bits mask */

/**
  * @}
  */



/*----------------------------------------------------------------------------*/
/**
  * @brief FLASH program and Data memory (FLASH)
  */

typedef struct FLASH_struct
{
  vu8 CR1;       /*!< Flash control register 1 */
  vu8 CR2;       /*!< Flash control register 2 */
  vu8 NCR2;      /*!< Flash complementary control register 2 */
  vu8 FPR;       /*!< Flash protection register */
  vu8 NFPR;      /*!< Flash complementary protection register */
  vu8 IAPSR;     /*!< Flash in-application programming status register */
  vu8 RESERVED1; /*!< Reserved byte */
  vu8 RESERVED2; /*!< Reserved byte */
  vu8 PUKR;      /*!< Flash program memory unprotection register */
  vu8 RESERVED3; /*!< Reserved byte */
  vu8 DUKR;      /*!< Data EEPROM unprotection register */
}
FLASH_TypeDef;

/** @addtogroup FLASH_Registers_Reset_Value
  * @{
  */

#define FLASH_CR1_RESET_VALUE   ((u8)0x00)
#define FLASH_CR2_RESET_VALUE   ((u8)0x00)
#define FLASH_NCR2_RESET_VALUE  ((u8)0xFF)
#define FLASH_IAPSR_RESET_VALUE ((u8)0x40)
#define FLASH_PUKR_RESET_VALUE  ((u8)0x00)
#define FLASH_DUKR_RESET_VALUE  ((u8)0x00)

/**
  * @}
  */

/** @addtogroup FLASH_Registers_Bits_Definition
  * @{
  */

#define FLASH_CR1_HALT  ((u8)0x08) /*!< Standby in Halt mode mask */
#define FLASH_CR1_AHALT ((u8)0x04) /*!< Standby in Active Halt mode mask */
#define FLASH_CR1_IE    ((u8)0x02) /*!< Flash Interrupt enable mask */
#define FLASH_CR1_FIX   ((u8)0x01) /*!< Fix programming time mask */

#define FLASH_CR2_OPT   ((u8)0x80) /*!< Select option byte mask */
#define FLASH_CR2_WPRG  ((u8)0x40) /*!< Word Programming mask */
#define FLASH_CR2_ERASE ((u8)0x20) /*!< Erase block mask */
#define FLASH_CR2_FPRG  ((u8)0x10) /*!< Fast programming mode mask */
#define FLASH_CR2_PRG   ((u8)0x01) /*!< Program block mask */

#define FLASH_NCR2_NOPT   ((u8)0x80) /*!< Select option byte mask */
#define FLASH_NCR2_NWPRG  ((u8)0x40) /*!< Word Programming mask */
#define FLASH_NCR2_NERASE ((u8)0x20) /*!< Erase block mask */
#define FLASH_NCR2_NFPRG  ((u8)0x10) /*!< Fast programming mode mask */
#define FLASH_NCR2_NPRG   ((u8)0x01) /*!< Program block mask */

#define FLASH_IAPSR_HVOFF     ((u8)0x40) /*!< End of high voltage flag mask */
#define FLASH_IAPSR_DUL       ((u8)0x08) /*!< Data EEPROM unlocked flag mask */
#define FLASH_IAPSR_EOP       ((u8)0x04) /*!< End of operation flag mask */
#define FLASH_IAPSR_PUL       ((u8)0x02) /*!< Flash Program memory unlocked flag mask */
#define FLASH_IAPSR_WR_PG_DIS ((u8)0x01) /*!< Write attempted to protected page mask */

#define FLASH_PUKR_PUK ((u8)0xFF) /*!< Flash Program memory unprotection mask */

#define FLASH_DUKR_DUK ((u8)0xFF) /*!< Data EEPROM unprotection mask */

/**
  * @}
  */

/*----------------------------------------------------------------------------*/
/**
  * @brief Option Bytes (OPT)
  */
typedef struct OPT_struct
{
  vu8 OPT0;  /*!< Option byte 0: Read-out protection (not accessible in IAP mode) */
  vu8 OPT1;  /*!< Option byte 1: User boot code */
  vu8 NOPT1; /*!< Complementary Option byte 1 */
  vu8 OPT2;  /*!< Option byte 2: Alternate function remapping */
  vu8 NOPT2; /*!< Complementary Option byte 2 */
  vu8 OPT3;  /*!< Option byte 3: Watchdog option */
  vu8 NOPT3; /*!< Complementary Option byte 3 */
  vu8 OPT4;  /*!< Option byte 4: Clock option */
  vu8 NOPT4; /*!< Complementary Option byte 4 */
  vu8 OPT5;  /*!< Option byte 5: HSE clock startup */
  vu8 NOPT5; /*!< Complementary Option byte 5 */
  vu8 RESERVED1;  /*!< Reserved Option byte*/
  vu8 RESERVED2; /*!< Reserved Option byte*/
  vu8 OPT7;  /*!< Option byte 7: flash wait states */
  vu8 NOPT7; /*!< Complementary Option byte 7 */
}
OPT_TypeDef;

/*----------------------------------------------------------------------------*/
/**
  * @brief Independent Watchdog (IWDG)
  */

typedef struct IWDG_struct
{
  vu8 KR;  /*!< Key Register */
  vu8 PR;  /*!< Prescaler Register */
  vu8 RLR; /*!< Reload Register */
}
IWDG_TypeDef;

/** @addtogroup IWDG_Registers_Reset_Value
  * @{
  */

#define IWDG_PR_RESET_VALUE  ((u8)0x00)
#define IWDG_RLR_RESET_VALUE ((u8)0xFF)

/**
  * @}
  */

/*----------------------------------------------------------------------------*/
/**
  * @brief Window Watchdog (WWDG)
  */

typedef struct WWDG_struct
{
  vu8 CR; /*!< Control Register */
  vu8 WR; /*!< Window Register */
}
WWDG_TypeDef;

/** @addtogroup WWDG_Registers_Reset_Value
  * @{
  */

#define WWDG_CR_RESET_VALUE ((u8)0x7F)
#define WWDG_WR_RESET_VALUE ((u8)0x7F)

/**
  * @}
  */

/** @addtogroup WWDG_Registers_Bits_Definition
  * @{
  */

#define WWDG_CR_WDGA ((u8)0x80) /*!< WDGA bit mask */
#define WWDG_CR_T6   ((u8)0x40) /*!< T6 bit mask */
#define WWDG_CR_T    ((u8)0x7F) /*!< T bits mask */

#define WWDG_WR_MSB  ((u8)0x80) /*!< MSB bit mask */
#define WWDG_WR_W    ((u8)0x7F) /*!< W bits mask */

/**
  * @}
  */

/*----------------------------------------------------------------------------*/
/**
  * @brief Reset Controller (RST)
  */

typedef struct RST_struct
{
  vu8 SR; /*!< Reset status register */
}
RST_TypeDef;

/** @addtogroup RST_Registers_Bits_Definition
  * @{
  */

#define RST_SR_EMCF   ((u8)0x10) /*!< EMC reset flag bit mask */
#define RST_SR_SWIMF  ((u8)0x08) /*!< SWIM reset flag bit mask */
#define RST_SR_ILLOPF ((u8)0x04) /*!< Illegal opcode reset flag bit mask */
#define RST_SR_IWDGF  ((u8)0x02) /*!< IWDG reset flag bit mask */
#define RST_SR_WWDGF  ((u8)0x01) /*!< WWDG reset flag bit mask */

/**
  * @}
  */

/*----------------------------------------------------------------------------*/
/**
  * @brief Serial Peripheral Interface (SPI)
  */

typedef struct SPI_struct
{
  vu8 CR1;    /*!< SPI control register 1 */
  vu8 CR2;    /*!< SPI control register 2 */
  vu8 ICR;    /*!< SPI interrupt control register */
  vu8 SR;     /*!< SPI status register */
  vu8 DR;     /*!< SPI data I/O register */
  vu8 CRCPR;  /*!< SPI CRC polynomial register */
  vu8 RXCRCR; /*!< SPI Rx CRC register */
  vu8 TXCRCR; /*!< SPI Tx CRC register */
}
SPI_TypeDef;

/** @addtogroup SPI_Registers_Reset_Value
  * @{
  */

#define SPI_CR1_RESET_VALUE    ((u8)0x00) /*!< Control Register 1 reset value */
#define SPI_CR2_RESET_VALUE    ((u8)0x00) /*!< Control Register 2 reset value */
#define SPI_ICR_RESET_VALUE    ((u8)0x00) /*!< Interrupt Control Register reset value */
#define SPI_SR_RESET_VALUE     ((u8)0x02) /*!< Status Register reset value */
#define SPI_DR_RESET_VALUE     ((u8)0x00) /*!< Data Register reset value */
#define SPI_CRCPR_RESET_VALUE  ((u8)0x07) /*!< Polynomial Register reset value */
#define SPI_RXCRCR_RESET_VALUE ((u8)0x00) /*!< RX CRC Register reset value */
#define SPI_TXCRCR_RESET_VALUE ((u8)0x00) /*!< TX CRC Register reset value */

/**
  * @}
  */

/** @addtogroup SPI_Registers_Bits_Definition
  * @{
  */

#define SPI_CR1_LSBFIRST ((u8)0x80) /*!< Frame format mask */
#define SPI_CR1_SPE      ((u8)0x40) /*!< Enable bits mask */
#define SPI_CR1_BR       ((u8)0x38) /*!< Baud rate control mask */
#define SPI_CR1_MSTR     ((u8)0x04) /*!< Master Selection mask */
#define SPI_CR1_CPOL     ((u8)0x02) /*!< Clock Polarity mask */
#define SPI_CR1_CPHA     ((u8)0x01) /*!< Clock Phase mask */

#define SPI_CR2_BDM     ((u8)0x80) /*!< Bi-directional data mode enable mask */
#define SPI_CR2_BDOE       ((u8)0x40) /*!< Output enable in bi-directional mode mask */
#define SPI_CR2_CRCEN        ((u8)0x20) /*!< Hardware CRC calculation enable mask */
#define SPI_CR2_CRCNEXT      ((u8)0x10) /*!< Transmit CRC next mask */
#define SPI_CR2_RXONLY       ((u8)0x04) /*!< Receive only mask */
#define SPI_CR2_SSM          ((u8)0x02) /*!< Software slave management mask */
#define SPI_CR2_SSI          ((u8)0x01) /*!< Internal slave select mask */

#define SPI_ICR_TXEI     ((u8)0x80) /*!< Tx buffer empty interrupt enable mask */
#define SPI_ICR_RXEI     ((u8)0x40) /*!< Rx buffer empty interrupt enable mask */
#define SPI_ICR_ERRIE     ((u8)0x20) /*!< Error interrupt enable mask */
#define SPI_ICR_WKIE    ((u8)0x10) /*!< Wake-up interrupt enable mask */

#define SPI_SR_BSY    ((u8)0x80) /*!< Busy flag */
#define SPI_SR_OVR    ((u8)0x40) /*!< Overrun flag */
#define SPI_SR_MODF   ((u8)0x20) /*!< Mode fault */
#define SPI_SR_CRCERR ((u8)0x10) /*!< CRC error flag */
#define SPI_SR_WKUP   ((u8)0x08) /*!< Wake-Up flag */
#define SPI_SR_TXE    ((u8)0x02) /*!< Transmit buffer empty */
#define SPI_SR_RXNE   ((u8)0x01) /*!< Receive buffer not empty */

/**
  * @}
  */

/*----------------------------------------------------------------------------*/
/**
  * @brief Single Wire Interface Module (SWIM)
  */
typedef struct SWIM_struct
{
  vu8 CSR; /*!< Control/Status register */
  vu8 DR;  /*!< Data register */
}
SWIM_TypeDef;

/*----------------------------------------------------------------------------*/
/**
  * @brief Universal Synchronous Asynchronous Receiver Transmitter (UART1)
  */

typedef struct UART1_struct
{
  vu8 SR;   /*!< UART1 status register */
  vu8 DR;   /*!< UART1 data register */
  vu8 BRR1; /*!< UART1 baud rate register */
  vu8 BRR2; /*!< UART1 DIV mantissa[11:8] SCIDIV fraction */
  vu8 CR1;  /*!< UART1 control register 1 */
  vu8 CR2;  /*!< UART1 control register 2 */
  vu8 CR3;  /*!< UART1 control register 3 */
  vu8 CR4;  /*!< UART1 control register 4 */
  vu8 CR5;  /*!< UART1 control register 5 */
  vu8 GTR;  /*!< UART1 guard time register */
  vu8 PSCR; /*!< UART1 prescaler register */
}
UART1_TypeDef;

/** @addtogroup UART1_Registers_Reset_Value
  * @{
  */

#define UART1_SR_RESET_VALUE   ((u8)0xC0)
#define UART1_BRR1_RESET_VALUE ((u8)0x00)
#define UART1_BRR2_RESET_VALUE ((u8)0x00)
#define UART1_CR1_RESET_VALUE  ((u8)0x00)
#define UART1_CR2_RESET_VALUE  ((u8)0x00)
#define UART1_CR3_RESET_VALUE  ((u8)0x00)
#define UART1_CR4_RESET_VALUE  ((u8)0x00)
#define UART1_CR5_RESET_VALUE  ((u8)0x00)
#define UART1_GTR_RESET_VALUE  ((u8)0x00)
#define UART1_PSCR_RESET_VALUE ((u8)0x00)

/**
  * @}
  */

/** @addtogroup UART1_Registers_Bits_Definition
  * @{
  */

#define UART1_SR_TXE   ((u8)0x80) /*!< Transmit Data Register Empty mask */
#define UART1_SR_TC    ((u8)0x40) /*!< Transmission Complete mask */
#define UART1_SR_RXNE  ((u8)0x20) /*!< Read Data Register Not Empty mask */
#define UART1_SR_IDLE  ((u8)0x10) /*!< IDLE line detected mask */
#define UART1_SR_OR   ((u8)0x08) /*!< OverRun error mask */
#define UART1_SR_NF    ((u8)0x04) /*!< Noise Flag mask */
#define UART1_SR_FE    ((u8)0x02) /*!< Framing Error mask */
#define UART1_SR_PE    ((u8)0x01) /*!< Parity Error mask */

#define UART1_BRR1_DIVM  ((u8)0xFF) /*!< LSB mantissa of UART1DIV [7:0] mask */

#define UART1_BRR2_DIVM  ((u8)0xF0) /*!< MSB mantissa of UART1DIV [11:8] mask */
#define UART1_BRR2_DIVF  ((u8)0x0F) /*!< Fraction bits of UART1DIV [3:0] mask */

#define UART1_CR1_R8      ((u8)0x80) /*!< Receive Data bit 8 */
#define UART1_CR1_T8      ((u8)0x40) /*!< Transmit data bit 8 */
#define UART1_CR1_UARTD   ((u8)0x20) /*!< UART1 Disable (for low power consumption) */
#define UART1_CR1_M       ((u8)0x10) /*!< Word length mask */
#define UART1_CR1_WAKE    ((u8)0x08) /*!< Wake-up method mask */
#define UART1_CR1_PCEN    ((u8)0x04) /*!< Parity Control Enable mask */
#define UART1_CR1_PS      ((u8)0x02) /*!< UART1 Parity Selection */
#define UART1_CR1_PIEN    ((u8)0x01) /*!< UART1 Parity Interrupt Enable mask */

#define UART1_CR2_TIEN    ((u8)0x80) /*!< Transmitter Interrupt Enable mask */
#define UART1_CR2_TCIEN   ((u8)0x40) /*!< TransmissionComplete Interrupt Enable mask */
#define UART1_CR2_RIEN    ((u8)0x20) /*!< Receiver Interrupt Enable mask */
#define UART1_CR2_ILIEN   ((u8)0x10) /*!< IDLE Line Interrupt Enable mask */
#define UART1_CR2_TEN     ((u8)0x08) /*!< Transmitter Enable mask */
#define UART1_CR2_REN     ((u8)0x04) /*!< Receiver Enable mask */
#define UART1_CR2_RWU     ((u8)0x02) /*!< Receiver Wake-Up mask */
#define UART1_CR2_SBK     ((u8)0x01) /*!< Send Break mask */

#define UART1_CR3_LINEN   ((u8)0x40) /*!< Alternate Function outpu mask */
#define UART1_CR3_STOP    ((u8)0x30) /*!< STOP bits [1:0] mask */
#define UART1_CR3_CKEN    ((u8)0x08) /*!< Clock Enable mask */
#define UART1_CR3_CPOL    ((u8)0x04) /*!< Clock Polarity mask */
#define UART1_CR3_CPHA    ((u8)0x02) /*!< Clock Phase mask */
#define UART1_CR3_LBCL    ((u8)0x01) /*!< Last Bit Clock pulse mask */

#define UART1_CR4_LBDIEN  ((u8)0x40) /*!< LIN Break Detection Interrupt Enable mask */
#define UART1_CR4_LBDL    ((u8)0x20) /*!< LIN Break Detection Length mask */
#define UART1_CR4_LBDF    ((u8)0x10) /*!< LIN Break Detection Flag mask */
#define UART1_CR4_ADD     ((u8)0x0F) /*!< Address of the UART1 node mask */

#define UART1_CR5_SCEN    ((u8)0x20) /*!< Smart Card Enable mask */
#define UART1_CR5_NACK    ((u8)0x10) /*!< Smart Card Nack Enable mask */
#define UART1_CR5_HDSEL   ((u8)0x08) /*!< Half-Duplex Selection mask */
#define UART1_CR5_IRLP    ((u8)0x04) /*!< Irda Low Power Selection mask */
#define UART1_CR5_IREN    ((u8)0x02) /*!< Irda Enable mask */

/**
  * @}
  */

/*----------------------------------------------------------------------------*/
/**
  * @brief Universal Synchronous Asynchronous Receiver Transmitter (UART2)
  */

typedef struct UART2_struct
{
  vu8 SR;   /*!< UART1 status register */
  vu8 DR;   /*!< UART1 data register */
  vu8 BRR1; /*!< UART1 baud rate register */
  vu8 BRR2; /*!< UART1 DIV mantissa[11:8] SCIDIV fraction */
  vu8 CR1;  /*!< UART1 control register 1 */
  vu8 CR2;  /*!< UART1 control register 2 */
  vu8 CR3;  /*!< UART1 control register 3 */
  vu8 CR4;  /*!< UART1 control register 4 */
  vu8 CR5;  /*!< UART1 control register 5 */
	vu8 CR6;  /*!< UART1 control register 6 */
  vu8 GTR;  /*!< UART1 guard time register */
  vu8 PSCR; /*!< UART1 prescaler register */
}
UART2_TypeDef;

/** @addtogroup UART2_Registers_Reset_Value
  * @{
  */

#define UART2_SR_RESET_VALUE   ((u8)0xC0)
#define UART2_BRR1_RESET_VALUE ((u8)0x00)
#define UART2_BRR2_RESET_VALUE ((u8)0x00)
#define UART2_CR1_RESET_VALUE  ((u8)0x00)
#define UART2_CR2_RESET_VALUE  ((u8)0x00)
#define UART2_CR3_RESET_VALUE  ((u8)0x00)
#define UART2_CR4_RESET_VALUE  ((u8)0x00)
#define UART2_CR5_RESET_VALUE  ((u8)0x00)
#define UART2_CR6_RESET_VALUE  ((u8)0x00)
#define UART2_GTR_RESET_VALUE  ((u8)0x00)
#define UART2_PSCR_RESET_VALUE ((u8)0x00)

/**
  * @}
  */

/** @addtogroup UART2_Registers_Bits_Definition
  * @{
  */

#define UART2_SR_TXE   ((u8)0x80) /*!< Transmit Data Register Empty mask */
#define UART2_SR_TC    ((u8)0x40) /*!< Transmission Complete mask */
#define UART2_SR_RXNE  ((u8)0x20) /*!< Read Data Register Not Empty mask */
#define UART2_SR_IDLE  ((u8)0x10) /*!< IDLE line detected mask */
#define UART2_SR_OR   ((u8)0x08) /*!< OverRun error mask */
#define UART2_SR_NF    ((u8)0x04) /*!< Noise Flag mask */
#define UART2_SR_FE    ((u8)0x02) /*!< Framing Error mask */
#define UART2_SR_PE    ((u8)0x01) /*!< Parity Error mask */

#define UART2_BRR1_DIVM  ((u8)0xFF) /*!< LSB mantissa of UART2DIV [7:0] mask */

#define UART2_BRR2_DIVM  ((u8)0xF0) /*!< MSB mantissa of UART2DIV [11:8] mask */
#define UART2_BRR2_DIVF  ((u8)0x0F) /*!< Fraction bits of UART2DIV [3:0] mask */

#define UART2_CR1_R8      ((u8)0x80) /*!< Receive Data bit 8 */
#define UART2_CR1_T8      ((u8)0x40) /*!< Transmit data bit 8 */
#define UART2_CR1_UARTD   ((u8)0x20) /*!< UART2 Disable (for low power consumption) */
#define UART2_CR1_M       ((u8)0x10) /*!< Word length mask */
#define UART2_CR1_WAKE    ((u8)0x08) /*!< Wake-up method mask */
#define UART2_CR1_PCEN    ((u8)0x04) /*!< Parity Control Enable mask */
#define UART2_CR1_PS      ((u8)0x02) /*!< UART2 Parity Selection */
#define UART2_CR1_PIEN    ((u8)0x01) /*!< UART2 Parity Interrupt Enable mask */

#define UART2_CR2_TIEN    ((u8)0x80) /*!< Transmitter Interrupt Enable mask */
#define UART2_CR2_TCIEN   ((u8)0x40) /*!< TransmissionComplete Interrupt Enable mask */
#define UART2_CR2_RIEN    ((u8)0x20) /*!< Receiver Interrupt Enable mask */
#define UART2_CR2_ILIEN   ((u8)0x10) /*!< IDLE Line Interrupt Enable mask */
#define UART2_CR2_TEN     ((u8)0x08) /*!< Transmitter Enable mask */
#define UART2_CR2_REN     ((u8)0x04) /*!< Receiver Enable mask */
#define UART2_CR2_RWU     ((u8)0x02) /*!< Receiver Wake-Up mask */
#define UART2_CR2_SBK     ((u8)0x01) /*!< Send Break mask */

#define UART2_CR3_LINEN   ((u8)0x40) /*!< Alternate Function outpu mask */
#define UART2_CR3_STOP    ((u8)0x30) /*!< STOP bits [1:0] mask */
#define UART2_CR3_CKEN    ((u8)0x08) /*!< Clock Enable mask */
#define UART2_CR3_CPOL    ((u8)0x04) /*!< Clock Polarity mask */
#define UART2_CR3_CPHA    ((u8)0x02) /*!< Clock Phase mask */
#define UART2_CR3_LBCL    ((u8)0x01) /*!< Last Bit Clock pulse mask */

#define UART2_CR4_LBDIEN  ((u8)0x40) /*!< LIN Break Detection Interrupt Enable mask */
#define UART2_CR4_LBDL    ((u8)0x20) /*!< LIN Break Detection Length mask */
#define UART2_CR4_LBDF    ((u8)0x10) /*!< LIN Break Detection Flag mask */
#define UART2_CR4_ADD     ((u8)0x0F) /*!< Address of the UART2 node mask */

#define UART2_CR5_SCEN    ((u8)0x20) /*!< Smart Card Enable mask */
#define UART2_CR5_NACK    ((u8)0x10) /*!< Smart Card Nack Enable mask */
#define UART2_CR5_HDSEL   ((u8)0x08) /*!< Half-Duplex Selection mask */
#define UART2_CR5_IRLP    ((u8)0x04) /*!< Irda Low Power Selection mask */
#define UART2_CR5_IREN    ((u8)0x02) /*!< Irda Enable mask */

#define UART2_CR6_LDUM    ((u8)0x80) /*!< LIN Divider Update Method */
#define UART2_CR6_LSLV    ((u8)0x20) /*!< LIN Slave Enable */
#define UART2_CR6_LASE    ((u8)0x10) /*!< LIN Autosynchronization Enable */
#define UART2_CR6_LHDIEN  ((u8)0x04) /*!< LIN Header Detection Interrupt Enable */
#define UART2_CR6_LHDF    ((u8)0x02) /*!< LIN Header Detection Flag */
#define UART2_CR6_LSF     ((u8)0x01) /*!< LIN Synch Field */

/**
  * @}
  */


/*----------------------------------------------------------------------------*/
/**
  * @brief LIN Universal Asynchronous Receiver Transmitter (UART3)
  */

typedef struct UART3_struct
{
  vu8 SR;       /*!< status register */
  vu8 DR;       /*!< data register */
  vu8 BRR1;     /*!< baud rate register */
  vu8 BRR2;     /*!< DIV mantissa[11:8] SCIDIV fraction */
  vu8 CR1;      /*!< control register 1 */
  vu8 CR2;      /*!< control register 2 */
  vu8 CR3;      /*!< control register 3 */
  vu8 CR4;      /*!< control register 4 */
  vu8 RESERVED; /*!< Reserved byte */
  vu8 CR6;      /*!< control register 5 */
}
UART3_TypeDef;

/** @addtogroup UART3_Registers_Reset_Value
  * @{
  */

#define UART3_SR_RESET_VALUE   ((u8)0xC0)
#define UART3_BRR1_RESET_VALUE ((u8)0x00)
#define UART3_BRR2_RESET_VALUE ((u8)0x00)
#define UART3_CR1_RESET_VALUE  ((u8)0x00)
#define UART3_CR2_RESET_VALUE  ((u8)0x00)
#define UART3_CR3_RESET_VALUE  ((u8)0x00)
#define UART3_CR4_RESET_VALUE  ((u8)0x00)
#define UART3_CR6_RESET_VALUE  ((u8)0x00)

/**
  * @}
  */

/** @addtogroup UART3_Registers_Bits_Definition
  * @{
  */

#define UART3_SR_TXE      ((u8)0x80) /*!< Transmit Data Register Empty mask */
#define UART3_SR_TC       ((u8)0x40) /*!< Transmission Complete mask */
#define UART3_SR_RXNE     ((u8)0x20) /*!< Read Data Register Not Empty mask */
#define UART3_SR_IDLE     ((u8)0x10) /*!< IDLE line detected mask */
#define UART3_SR_OR       ((u8)0x08) /*!< OverRun error mask */
#define UART3_SR_NF       ((u8)0x04) /*!< Noise Flag mask */
#define UART3_SR_FE       ((u8)0x02) /*!< Framing Error mask */
#define UART3_SR_PE       ((u8)0x01) /*!< Parity Error mask */

#define UART3_BRR1_DIVM   ((u8)0xFF) /*!< LSB mantissa of UARTDIV [7:0] mask */

#define UART3_BRR2_DIVM   ((u8)0xF0) /*!< MSB mantissa of UARTDIV [11:8] mask */
#define UART3_BRR2_DIVF   ((u8)0x0F) /*!< Fraction bits of UARTDIV [3:0] mask */

#define UART3_CR1_R8      ((u8)0x80) /*!< Receive Data bit 8 */
#define UART3_CR1_T8      ((u8)0x40) /*!< Transmit data bit 8 */
#define UART3_CR1_UARTD   ((u8)0x20) /*!< UART Disable (for low power consumption) */
#define UART3_CR1_M       ((u8)0x10) /*!< Word length mask */
#define UART3_CR1_WAKE    ((u8)0x08) /*!< Wake-up method mask */
#define UART3_CR1_PCEN    ((u8)0x04) /*!< Parity control enable mask */
#define UART3_CR1_PS      ((u8)0x02) /*!< Parity selection bit mask */
#define UART3_CR1_PIEN    ((u8)0x01) /*!< Parity interrupt enable bit mask */

#define UART3_CR2_TIEN    ((u8)0x80) /*!< Transmitter Interrupt Enable mask */
#define UART3_CR2_TCIEN   ((u8)0x40) /*!< TransmissionComplete Interrupt Enable mask */
#define UART3_CR2_RIEN    ((u8)0x20) /*!< Receiver Interrupt Enable mask */
#define UART3_CR2_ILIEN   ((u8)0x10) /*!< IDLE Line Interrupt Enable mask */
#define UART3_CR2_TEN     ((u8)0x08) /*!< Transmitter Enable mask */
#define UART3_CR2_REN     ((u8)0x04) /*!< Receiver Enable mask */
#define UART3_CR2_RWU     ((u8)0x02) /*!< Receiver Wake-Up mask */
#define UART3_CR2_SBK     ((u8)0x01) /*!< Send Break mask */

#define UART3_CR3_LINEN   ((u8)0x40) /*!< Alternate Function outpu mask */
#define UART3_CR3_STOP    ((u8)0x30) /*!< STOP bits [1:0] mask */

#define UART3_CR4_LBDIEN  ((u8)0x40) /*!< LIN Break Detection Interrupt Enable mask */
#define UART3_CR4_LBDL    ((u8)0x20) /*!< LIN Break Detection Length mask */
#define UART3_CR4_LBDF    ((u8)0x10) /*!< LIN Break Detection Flag mask */
#define UART3_CR4_ADD     ((u8)0x0F) /*!< Address of the UART3 node mask */

#define UART3_CR6_LDUM    ((u8)0x80) /*!< LIN Divider Update Method */
#define UART3_CR6_LSLV    ((u8)0x20) /*!< LIN Slave Enable */
#define UART3_CR6_LASE    ((u8)0x10) /*!< LIN Autosynchronization Enable */
#define UART3_CR6_LHDIEN  ((u8)0x04) /*!< LIN Header Detection Interrupt Enable */
#define UART3_CR6_LHDF    ((u8)0x02) /*!< LIN Header Detection Flag */
#define UART3_CR6_LSF     ((u8)0x01) /*!< LIN Synch Field */

/**
  * @}
  */


/*----------------------------------------------------------------------------*/
/**
  * @brief Controller Area Network  (CAN)
  */

typedef struct
{
  vu8 MCR;    /*!< CAN master control register */
  vu8 MSR;    /*!< CAN master status register */
  vu8 TSR;    /*!< CAN transmit status register */
  vu8 TPR;    /*!< CAN transmit priority register */
  vu8 RFR;    /*!< CAN receive FIFO register */
  vu8 IER;    /*!< CAN interrupt enable register */
  vu8 DGR;    /*!< CAN diagnosis register */
  vu8 PSR;    /*!< CAN page selection register */

  union
  {
    struct
    {
      vu8 MCSR;
      vu8 MDLCR;
      vu8 MIDR1;
      vu8 MIDR2;
      vu8 MIDR3;
      vu8 MIDR4;
      vu8 MDAR1;
      vu8 MDAR2;
      vu8 MDAR3;
      vu8 MDAR4;
      vu8 MDAR5;
      vu8 MDAR6;
      vu8 MDAR7;
      vu8 MDAR8;
      vu8 MTSRL;
      vu8 MTSRH;
    }
	  TxMailbox;

	struct
    {
      vu8 FR01;
      vu8 FR02;
      vu8 FR03;
      vu8 FR04;
      vu8 FR05;
      vu8 FR06;
      vu8 FR07;
      vu8 FR08;

      vu8 FR09;
      vu8 FR10;
      vu8 FR11;
      vu8 FR12;
      vu8 FR13;
      vu8 FR14;
      vu8 FR15;
      vu8 FR16;
    }
	Filter;
	  

    struct
    {
      vu8 F0R1;
      vu8 F0R2;
      vu8 F0R3;
      vu8 F0R4;
      vu8 F0R5;
      vu8 F0R6;
      vu8 F0R7;
      vu8 F0R8;

      vu8 F1R1;
      vu8 F1R2;
      vu8 F1R3;
      vu8 F1R4;
      vu8 F1R5;
      vu8 F1R6;
      vu8 F1R7;
	  vu8 F1R8;
    }
	  Filter01;
    
    struct
    {
      vu8 F2R1;
      vu8 F2R2;
      vu8 F2R3;
      vu8 F2R4;
      vu8 F2R5;
      vu8 F2R6;
      vu8 F2R7;
      vu8 F2R8;
	
      vu8 F3R1;
      vu8 F3R2;
      vu8 F3R3;
      vu8 F3R4;
      vu8 F3R5;
      vu8 F3R6;
      vu8 F3R7;
      vu8 F3R8;
    }
	  Filter23;
    
    struct
    {
      vu8 F4R1;
      vu8 F4R2;
      vu8 F4R3;
      vu8 F4R4;
      vu8 F4R5;
      vu8 F4R6;
      vu8 F4R7;
      vu8 F4R8;
			
      vu8 F5R1;
      vu8 F5R2;
      vu8 F5R3;
      vu8 F5R4;
      vu8 F5R5;
      vu8 F5R6;
      vu8 F5R7;
      vu8 F5R8;
    }
	  Filter45;
    
    struct
    {
      vu8 ESR;
      vu8 EIER;
      vu8 TECR;
      vu8 RECR;
      vu8 BTR1;
      vu8 BTR2;
      u8 Reserved1[2];
      vu8 FMR1;
      vu8 FMR2;
      vu8 FCR1;
      vu8 FCR2;
      vu8 FCR3;
      u8 Reserved2[3];
    }
		Config;
    
    struct
    {
      vu8 MFMI;
      vu8 MDLCR;
      vu8 MIDR1;
      vu8 MIDR2;
      vu8 MIDR3;
      vu8 MIDR4;
      vu8 MDAR1;
      vu8 MDAR2;
      vu8 MDAR3;
      vu8 MDAR4;
      vu8 MDAR5;
      vu8 MDAR6;
      vu8 MDAR7;
      vu8 MDAR8;
      vu8 MTSRL;
      vu8 MTSRH;
    }
	  RxFIFO;
  }Page; 
} CAN_TypeDef;
/** @addtogroup CAN_Registers_Bits_Definition
  * @{
  */
/*******************************Commun****************************************/
/* CAN Master Control Register bits */
#define CAN_MCR_INRQ     ((u8)0x01)
#define CAN_MCR_SLEEP    ((u8)0x02)
#define CAN_MCR_TXFP     ((u8)0x04)
#define CAN_MCR_RFLM     ((u8)0x08)
#define CAN_MCR_NART     ((u8)0x10)
#define CAN_MCR_AWUM     ((u8)0x20)
#define CAN_MCR_ABOM     ((u8)0x40)
#define CAN_MCR_TTCM     ((u8)0x80)

/* CAN Master Status Register bits */
#define CAN_MSR_INAK     ((u8)0x01)
#define CAN_MSR_SLAK     ((u8)0x02)
#define CAN_MSR_ERRI     ((u8)0x04)
#define CAN_MSR_WKUI     ((u8)0x08)
#define CAN_MSR_TX       ((u8)0x10)
#define CAN_MSR_RX       ((u8)0x20)

/* CAN Transmit Status Register bits */
#define CAN_TSR_RQCP0    ((u8)0x01)
#define CAN_TSR_RQCP1    ((u8)0x02)
#define CAN_TSR_RQCP2    ((u8)0x04)
#define CAN_TSR_RQCP012  ((u8)0x07)
#define CAN_TSR_TXOK0    ((u8)0x10)
#define CAN_TSR_TXOK1    ((u8)0x20)
#define CAN_TSR_TXOK2    ((u8)0x40)

#define CAN_TPR_CODE0    ((u8)0x01)
#define CAN_TPR_TME0     ((u8)0x04)
#define CAN_TPR_TME1     ((u8)0x08)
#define CAN_TPR_TME2     ((u8)0x10)
#define CAN_TPR_LOW0     ((u8)0x20)
#define CAN_TPR_LOW1     ((u8)0x40)
#define CAN_TPR_LOW2     ((u8)0x80)
/* CAN Receive FIFO Register bits */
#define CAN_RFR_FMP01  ((u8)0x03)
#define CAN_RFR_FULL   ((u8)0x08)
#define CAN_RFR_FOVR   ((u8)0x10)
#define CAN_RFR_RFOM   ((u8)0x20)

/* CAN Interrupt Register bits */
#define CAN_IER_TMEIE  ((u8)0x01)
#define CAN_IER_FMPIE  ((u8)0x02)
#define CAN_IER_FFIE   ((u8)0x04)
#define CAN_IER_FOVIE  ((u8)0x08)
#define CAN_IER_WKUIE  ((u8)0x80)


/* CAN diagnostic Register bits */
#define CAN_DGR_LBKM  ((u8)0x01)
#define CAN_DGR_SLIM  ((u8)0x02)
#define CAN_DGR_SAMP  ((u8)0x04)
#define CAN_DGR_RX    ((u8)0x08)
#define CAN_DGR_TXM2E ((u8)0x10)


/* CAN page select Register bits */
#define CAN_PSR_PS0  ((u8)0x01)
#define CAN_PSR_PS1  ((u8)0x02)
#define CAN_PSR_PS2  ((u8)0x04)

/**************************Tx MailBox & Fifo Page cummun bits*****************************************/
#define CAN_MCSR_TXRQ    ((u8)0x01)
#define CAN_MCSR_ABRQ    ((u8)0x02)
#define CAN_MCSR_RQCP    ((u8)0x04)
#define CAN_MCSR_TXOK    ((u8)0x08)
#define CAN_MCSR_ALST    ((u8)0x10)
#define CAN_MCSR_TERR    ((u8)0x20)

#define CAN_MDLCR_DLC    ((u8)0x0F)
#define CAN_MDLCR_TGT    ((u8)0x80)

#define CAN_MIDR1_RTR    ((u8)0x20)
#define CAN_MIDR1_IDE    ((u8)0x40)


/*************************Filter Page**********************************************/

/* CAN Error Status Register bits */
#define CAN_ESR_EWGF     ((u8)0x01)
#define CAN_ESR_EPVF     ((u8)0x02)
#define CAN_ESR_BOFF     ((u8)0x04)
#define CAN_ESR_LEC0     ((u8)0x10)
#define CAN_ESR_LEC1     ((u8)0x20)
#define CAN_ESR_LEC2     ((u8)0x40)
#define CAN_ESR_LEC      ((u8)0x70)

/* CAN Error Status Register bits */
#define CAN_EIER_EWGIE    ((u8)0x01)
#define CAN_EIER_EPVIE    ((u8)0x02)
#define CAN_EIER_BOFIE    ((u8)0x04)
#define CAN_EIER_LECIE    ((u8)0x10)
#define CAN_EIER_ERRIE    ((u8)0x80)    

/* CAN transmit error counter Register bits(CAN_TECR) */
#define CAN_TECR_TEC0     ((u8)0x01)    
#define CAN_TECR_TEC1     ((u8)0x02)    
#define CAN_TECR_TEC2     ((u8)0x04)    
#define CAN_TECR_TEC3     ((u8)0x08)    
#define CAN_TECR_TEC4     ((u8)0x10)    
#define CAN_TECR_TEC5     ((u8)0x20)    
#define CAN_TECR_TEC6     ((u8)0x40)    
#define CAN_TECR_TEC7     ((u8)0x80)    

/* CAN RECEIVE error counter Register bits(CAN_TECR) */
#define CAN_RECR_REC0     ((u8)0x01)    
#define CAN_RECR_REC1     ((u8)0x02)    
#define CAN_RECR_REC2     ((u8)0x04)    
#define CAN_RECR_REC3     ((u8)0x08)    
#define CAN_RECR_REC4     ((u8)0x10)    
#define CAN_RECR_REC5     ((u8)0x20)    
#define CAN_RECR_REC6     ((u8)0x40)    
#define CAN_RECR_REC7     ((u8)0x80)    

/* CAN Bit Timing Register 2 bits(CAN_BTR2) */
#define CAN_BTR2_CLKS     ((u8)0x80)    
 
/* CAN filter mode register bits (CAN_FMR) */
#define CAN_FMR1_FML0     ((u8)0x01)    
#define CAN_FMR1_FMH0     ((u8)0x02)    
#define CAN_FMR1_FML1     ((u8)0x04)    
#define CAN_FMR1_FMH1     ((u8)0x08)    
#define CAN_FMR1_FML2     ((u8)0x10)    
#define CAN_FMR1_FMH2     ((u8)0x20)    
#define CAN_FMR1_FML3     ((u8)0x40)    
#define CAN_FMR1_FMH3     ((u8)0x80)    

#define CAN_FMR2_FML4     ((u8)0x01)    
#define CAN_FMR2_FMH4     ((u8)0x02)    
#define CAN_FMR2_FML5     ((u8)0x04)    
#define CAN_FMR2_FMH5     ((u8)0x08)    

/* CAN filter Config register bits (CAN_FCR) */
#define CAN_FCR1_FACT0     ((u8)0x01)    
#define CAN_FCR1_FACT1     ((u8)0x10)    
#define CAN_FCR2_FACT2     ((u8)0x01)    
#define CAN_FCR2_FACT3     ((u8)0x10)    
#define CAN_FCR3_FACT4     ((u8)0x01)    
#define CAN_FCR3_FACT5     ((u8)0x10)    

#define CAN_FCR1_FSC00    ((u8)0x02)    
#define CAN_FCR1_FSC01    ((u8)0x04)    
#define CAN_FCR1_FSC10    ((u8)0x20)    
#define CAN_FCR1_FSC11    ((u8)0x40)    
#define CAN_FCR2_FSC20    ((u8)0x02)    
#define CAN_FCR2_FSC21    ((u8)0x04)    
#define CAN_FCR2_FSC30    ((u8)0x20)    
#define CAN_FCR2_FSC31    ((u8)0x40)    
#define CAN_FCR3_FSC40    ((u8)0x02)    
#define CAN_FCR3_FSC41    ((u8)0x04)    
#define CAN_FCR3_FSC50    ((u8)0x20)    
#define CAN_FCR3_FSC51    ((u8)0x40)



/**
  * @}
  */

/** @addtogroup CAN_Registers_Reset_Value
  * @{
  */

#define  	CAN_MCR_RESET_VALUE			((u8)0x02)
#define  	CAN_MSR_RESET_VALUE			((u8)0x02)
#define  	CAN_TSR_RESET_VALUE			((u8)0x00)
#define  	CAN_TPR_RESET_VALUE			((u8)0x0C)
#define  	CAN_RFR_RESET_VALUE			((u8)0x00)
#define  	CAN_IER_RESET_VALUE			((u8)0x00)
#define  	CAN_DGR_RESET_VALUE			((u8)0x0C)
#define  	CAN_PSR_RESET_VALUE			((u8)0x00)

#define  	CAN_ESR_RESET_VALUE			((u8)0x00)
#define  	CAN_EIER_RESET_VALUE		((u8)0x00)
#define  	CAN_TECR_RESET_VALUE		((u8)0x00)
#define  	CAN_RECR_RESET_VALUE		((u8)0x00)
#define  	CAN_BTR1_RESET_VALUE		((u8)0x40)
#define  	CAN_BTR2_RESET_VALUE		((u8)0x23)
#define  	CAN_FMR1_RESET_VALUE		((u8)0x00)
#define  	CAN_FMR2_RESET_VALUE		((u8)0x00)
#define  	CAN_FCR_RESET_VALUE			((u8)0x00)

#define  	CAN_MFMI_RESET_VALUE			((u8)0x00)
#define  	CAN_MDLC_RESET_VALUE			((u8)0x00)
#define  	CAN_MCSR_RESET_VALUE			((u8)0x00)

/**
  * @}
  */

/**
  * @brief Configuration Registers (CFG)
  */

typedef struct CFG_struct
{
  vu8 GCR; /*!< Global Configuration register */
}
CFG_TypeDef;

/** @addtogroup CFG_Registers_Reset_Value
  * @{
  */

#define CFG_GCR_RESET_VALUE ((u8)0x00)

/**
  * @}
  */

/** @addtogroup CFG_Registers_Bits_Definition
  * @{
  */

#define CFG_GCR_SWD ((u8)0x01) /*!< Swim disable bit mask */
#define CFG_GCR_AL  ((u8)0x02) /*!< Activation Level bit mask */

/**
  * @}
  */

/**
  * @}
  */

/******************************************************************************/
/*                          Peripherals Base Address                          */
/******************************************************************************/

/** @addtogroup MAP_FILE_Base_Addresses
  * @{
  */
#define OPT_BaseAddress         0x4800
#define GPIOA_BaseAddress       0x5000
#define GPIOB_BaseAddress       0x5005
#define GPIOC_BaseAddress       0x500A
#define GPIOD_BaseAddress       0x500F
#define GPIOE_BaseAddress       0x5014
#define GPIOF_BaseAddress       0x5019
#define GPIOG_BaseAddress       0x501E
#define GPIOH_BaseAddress       0x5023
#define GPIOI_BaseAddress       0x5028

#define FLASH_BaseAddress       0x505A

#define EXTI_BaseAddress        0x50A0
#define RST_BaseAddress         0x50B3
#define CLK_BaseAddress         0x50C0
#define WWDG_BaseAddress        0x50D1
#define IWDG_BaseAddress        0x50E0
#define AWU_BaseAddress         0x50F0
#define BEEP_BaseAddress        0x50F3
#define SPI_BaseAddress         0x5200
#define I2C_BaseAddress         0x5210
#define UART1_BaseAddress       0x5230
#define UART2_BaseAddress       0x5240
#define UART3_BaseAddress       0x5240
#define TIM1_BaseAddress        0x5250
#define TIM2_BaseAddress        0x5300
#define TIM3_BaseAddress        0x5320
#define TIM4_BaseAddress        0x5340
#define TIM5_BaseAddress        0x5300
#define TIM6_BaseAddress        0x5340

#define ADC1_BaseAddress        0x53E0
#define ADC2_BaseAddress        0x5400
#define CAN_BaseAddress         0x5420

#define CFG_BaseAddress         0x7F60
#define ITC_BaseAddress         0x7F70
#define SWIM_BaseAddress        0x7F80
#define DM_BaseAddress          0x7F90

/**
  * @}
  */

/******************************************************************************/
/*                          Peripherals declarations                          */
/******************************************************************************/

#if defined(STM8S105) || defined(STM8S103) || defined(STM8S903)
 #define ADC1 ((ADC1_TypeDef *) ADC1_BaseAddress)
#endif /* (STM8S105) ||(STM8S103) || (STM8S903) */

#if defined(STM8S208) || defined(STM8S207)
#define ADC2 ((ADC2_TypeDef *) ADC2_BaseAddress)
#endif /* (STM8S208) ||(STM8S207)  */

#define AWU ((AWU_TypeDef *) AWU_BaseAddress)

#define BEEP ((BEEP_TypeDef *) BEEP_BaseAddress)

#ifdef STM8S208
 #define CAN ((CAN_TypeDef *) CAN_BaseAddress)
#endif /* (STM8S208) */

#define CLK ((CLK_TypeDef *) CLK_BaseAddress)

#define EXTI ((EXTI_TypeDef *) EXTI_BaseAddress)

#define FLASH ((FLASH_TypeDef *) FLASH_BaseAddress)

#define OPT ((OPT_TypeDef *) OPT_BaseAddress)

#define GPIOA ((GPIO_TypeDef *) GPIOA_BaseAddress)

#define GPIOB ((GPIO_TypeDef *) GPIOB_BaseAddress)

#define GPIOC ((GPIO_TypeDef *) GPIOC_BaseAddress)

#define GPIOD ((GPIO_TypeDef *) GPIOD_BaseAddress)

#define GPIOE ((GPIO_TypeDef *) GPIOE_BaseAddress)

#define GPIOF ((GPIO_TypeDef *) GPIOF_BaseAddress)

#if defined(STM8S207) || defined(STM8S208) || defined(STM8S105)
 #define GPIOG ((GPIO_TypeDef *) GPIOG_BaseAddress)
#endif /* (STM8S208) ||(STM8S207)  || (STM8S105) */

#if defined(STM8S207) || defined(STM8S208)
 #define GPIOH ((GPIO_TypeDef *) GPIOH_BaseAddress)
 #define GPIOI ((GPIO_TypeDef *) GPIOI_BaseAddress)
#endif /* (STM8S208) ||(STM8S207) */

#define RST ((RST_TypeDef *) RST_BaseAddress)

#define WWDG ((WWDG_TypeDef *) WWDG_BaseAddress)
#define IWDG ((IWDG_TypeDef *) IWDG_BaseAddress)

#define SPI ((SPI_TypeDef *) SPI_BaseAddress)
#define I2C ((I2C_TypeDef *) I2C_BaseAddress)

#if defined(STM8S208) ||defined(STM8S207) ||defined(STM8S103) ||defined(STM8S903)
 #define UART1 ((UART1_TypeDef *) UART1_BaseAddress)
#endif /* (STM8S208) ||(STM8S207)  || (STM8S103) || (STM8S903)  */

#ifdef STM8S105
 #define UART2 ((UART2_TypeDef *) UART2_BaseAddress)
#endif /* STM8S105 */

#if defined(STM8S208) ||defined(STM8S207)
 #define UART3 ((UART3_TypeDef *) UART3_BaseAddress)
#endif /* (STM8S208) ||(STM8S207) */

#define TIM1 ((TIM1_TypeDef *) TIM1_BaseAddress)

#if defined(STM8S208) ||defined(STM8S207) ||defined(STM8S103) ||defined(STM8S105)
 #define TIM2 ((TIM2_TypeDef *) TIM2_BaseAddress)
#endif /* (STM8S208) ||(STM8S207)  || (STM8S103) || (STM8S105)  */

#if defined(STM8S208) ||defined(STM8S207) ||defined(STM8S105)
 #define TIM3 ((TIM3_TypeDef *) TIM3_BaseAddress)
#endif /* (STM8S208) ||(STM8S207)  || (STM8S105) */

#if defined(STM8S208) ||defined(STM8S207) ||defined(STM8S103) ||defined(STM8S105)
 #define TIM4 ((TIM4_TypeDef *) TIM4_BaseAddress)
#endif /* (STM8S208) ||(STM8S207)  || (STM8S103) || (STM8S105)  */

#ifdef STM8S903
 #define TIM5 ((TIM5_TypeDef *) TIM5_BaseAddress)
 #define TIM6 ((TIM6_TypeDef *) TIM6_BaseAddress)
#endif /* STM8S903 */ 

#define ITC ((ITC_TypeDef *) ITC_BaseAddress)

#define CFG ((CFG_TypeDef *) CFG_BaseAddress)

#define SWIM ((SWIM_TypeDef *) SWIM_BaseAddress)

#define DM ((DM_TypeDef *) DM_BaseAddress)


#ifdef USE_STDPERIPH_DRIVER
 #include "stm8s_conf.h"
#endif

/* Exported macro --------------------------------------------------------------*/

/*============================== Interrupts ====================================*/
#ifdef _RAISONANCE_
 #include <intrist7.h>
 #define enableInterrupts()  _rim_()  /* enable interrupts */
 #define disableInterrupts() _sim_()  /* disable interrupts */
 #define rim()               _rim_()  /* enable interrupts */
 #define sim()               _sim_()  /* disable interrupts */
 #define nop()               _nop_()  /* No Operation */
 #define trap()              _trap_() /* Trap (soft IT) */
 #define wfi()               _wfi_()  /* Wait For Interrupt */
 #define halt()              _halt_() /* Halt */
#else /* COSMIC */
 #define enableInterrupts() {_asm("rim\n");} /* enable interrupts */
 #define disableInterrupts() {_asm("sim\n");} /* disable interrupts */
 #define rim() {_asm("rim\n");} /* enable interrupts */
 #define sim() {_asm("sim\n");} /* disable interrupts */
 #define nop() {_asm("nop\n");} /* No Operation */
 #define trap() {_asm("trap\n");} /* Trap (soft IT) */
 #define wfi() {_asm("wfi\n");} /* Wait For Interrupt */
 #define halt() {_asm("halt\n");} /* Halt */
#endif

/*============================== Handling bits ====================================*/
/*-----------------------------------------------------------------------------
Method : I
Description : Handle the bit from the character variables.
Comments :    The different parameters of commands are
              - VAR : Name of the character variable where the bit is located.
              - Place : Bit position in the variable (7 6 5 4 3 2 1 0)
              - Value : Can be 0 (reset bit) or not 0 (set bit)
              The "MskBit" command allows to select some bits in a source
              variables and copy it in a destination var (return the value).
              The "ValBit" command returns the value of a bit in a char
              variable: the bit is reseted if it returns 0 else the bit is set.
              This method generates not an optimised code yet.
-----------------------------------------------------------------------------*/
#define SetBit(VAR,Place)         ( (VAR) |= (u8)((u8)1<<(u8)(Place)) )
#define ClrBit(VAR,Place)         ( (VAR) &= (u8)((u8)((u8)1<<(u8)(Place))^(u8)255) )

#define ChgBit(VAR,Place)         ( (VAR) ^= (u8)((u8)1<<(u8)(Place)) )
#define AffBit(VAR,Place,Value)   ((Value) ? \
                                   ((VAR) |= ((u8)1<<(Place))) : \
                                   ((VAR) &= (((u8)1<<(Place))^(u8)255)))
#define MskBit(Dest,Msk,Src)      ( (Dest) = ((Msk) & (Src)) | ((~(Msk)) & (Dest)) )

#define ValBit(VAR,Place)         ((u8)(VAR) & (u8)((u8)1<<(u8)(Place)))

#define BYTE_0(n)                 ((u8)((n) & (u8)0xFF))        /*!< Returns the low byte of the 32-bit value */
#define BYTE_1(n)                 ((u8)(BYTE_0((n) >> (u8)8)))  /*!< Returns the second byte of the 32-bit value */
#define BYTE_2(n)                 ((u8)(BYTE_0((n) >> (u8)16))) /*!< Returns the third byte of the 32-bit value */
#define BYTE_3(n)                 ((u8)(BYTE_0((n) >> (u8)24))) /*!< Returns the high byte of the 32-bit value */

/*============================== Assert Macros ====================================*/
#define IS_STATE_VALUE_OK(SensitivityValue) \
  (((SensitivityValue) == ENABLE) || \
   ((SensitivityValue) == DISABLE))

/*-----------------------------------------------------------------------------
Method : II
Description : Handle directly the bit.
Comments :    The idea is to handle directly with the bit name. For that, it is
              necessary to have RAM area descriptions (example: HW register...)
              and the following command line for each area.
              This method generates the most optimized code.
-----------------------------------------------------------------------------*/

#define AREA 0x00     /* The area of bits begins at address 0x10. */

#define BitClr(BIT)  ( *((unsigned char *) (AREA+(BIT)/8)) &= (~(1<<(7-(BIT)%8))) )
#define BitSet(BIT)  ( *((unsigned char *) (AREA+(BIT)/8)) |= (1<<(7-(BIT)%8)) )
#define BitVal(BIT)  ( *((unsigned char *) (AREA+(BIT)/8)) & (1<<(7-(BIT)%8)) )

/* Exported functions ------------------------------------------------------- */

#endif /* __STM8S_H */

/**
  * @}
  */

/******************* (C) COPYRIGHT 2009 STMicroelectronics *****END OF FILE****/
