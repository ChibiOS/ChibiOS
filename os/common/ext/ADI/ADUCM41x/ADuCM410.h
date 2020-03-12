/* ================================================================================
     Project     :   ADuCM410
     File        :   ADuCM410_device.h
     Description :   C typedef structures for bit-fields and enums for enumerations.

     Copyright (c) 2019 Analog Devices, Inc.  All Rights Reserved.
     This software is proprietary and confidential to Analog Devices, Inc. and
     its licensors.

     This file was auto-generated. Do not make local changes to this file.
   ================================================================================ */

#ifndef __ADUCM410_H__
#define __ADUCM410_H__

/*  =========================================================================
 *! \enum   IRQn_Type
 *! \brief  Interrupt Number Assignments
 *  ========================================================================= */

typedef enum
{
    RESET_IRQn                                  =   -15,    /*!<* RESET */
    NonMaskableInt_IRQn                         =   -14,    /*!<* NonMaskableInt */
    HardFault_IRQn                              =   -13,    /*!<* HardFault */
    MemoryManagement_IRQn                       =   -12,    /*!<* MemoryManagement */
    BusFault_IRQn                               =   -11,    /*!<* BusFault */
    UsageFault_IRQn                             =   -10,    /*!<* UsageFault */
    SVCall_IRQn                                 =    -5,    /*!<* SVCall */
    DebugMonitor_IRQn                           =    -4,    /*!<* DebugMonitor */
    PendSV_IRQn                                 =    -2,    /*!<* PendSV */
    SysTick_IRQn                                =    -1,    /*!<* SysTick */
    WUT_IRQn                                    =     0,    /*!<* WUT */
    EINT0_IRQn                                  =     1,    /*!<* EINT0 */
    EINT1_IRQn                                  =     2,    /*!<* EINT1 */
    EINT2_IRQn                                  =     3,    /*!<* EINT2 */
    EINT3_IRQn                                  =     4,    /*!<* EINT3 */
    EINT4_IRQn                                  =     5,    /*!<* EINT4 */
    EINT5_IRQn                                  =     6,    /*!<* EINT5 */
    EINT6_IRQn                                  =     7,    /*!<* EINT6 */
    EINT7_IRQn                                  =     8,    /*!<* EINT7 */
    EINT8_IRQn                                  =     9,    /*!<* EINT8 */
    EINT9_IRQn                                  =    10,    /*!<* EINT9 */
    WDT_IRQn                                    =    11,    /*!<* WDT */
    GPT0_IRQn                                   =    12,    /*!<* GPT0 */
    GPT1_IRQn                                   =    13,    /*!<* GPT1 */
    GPT2_IRQn                                   =    14,    /*!<* GPT2 */
    GPT3_IRQn                                   =    15,    /*!<* GPT3 */
    GPT4_IRQn                                   =    16,    /*!<* GPT4 */
    MDIO_IRQn                                   =    17,    /*!<* MDIO */
    FLASH_IRQn                                  =    18,    /*!<* FLASH */
    UART0_IRQn                                  =    19,    /*!<* UART0 */
    UART1_IRQn                                  =    20,    /*!<* UART1 */
    SPI0_IRQn                                   =    21,    /*!<* SPI0 */
    SPI1_IRQn                                   =    22,    /*!<* SPI1 */
    SPI2_IRQn                                   =    23,    /*!<* SPI2 */
    I2C0S_IRQn                                  =    24,    /*!<* I2C0S */
    I2C0M_IRQn                                  =    25,    /*!<* I2C0M */
    I2C1S_IRQn                                  =    26,    /*!<* I2C1S */
    I2C1M_IRQn                                  =    27,    /*!<* I2C1M */
    I2C2S_IRQn                                  =    28,    /*!<* I2C2S */
    I2C2M_IRQn                                  =    29,    /*!<* I2C2M */
    PLA0_IRQn                                   =    30,    /*!<* PLA0 */
    PLA1_IRQn                                   =    31,    /*!<* PLA1 */
    PLA2_IRQn                                   =    32,    /*!<* PLA2 */
    PLA3_IRQn                                   =    33,    /*!<* PLA3 */
    PWM_TRIP_IRQn                               =    34,    /*!<* PWM_TRIP */
    PWM0_IRQn                                   =    35,    /*!<* PWM0 */
    PWM1_IRQn                                   =    36,    /*!<* PWM1 */
    PWM2_IRQn                                   =    37,    /*!<* PWM2 */
    PWM3_IRQn                                   =    38,    /*!<* PWM3 */
    SRAM_ERR_IRQn                               =    39,    /*!<* SRAM_ERR */
    DMA_ERR_IRQn                                =    40,    /*!<* DMA_ERR */
    DMA_SPI0_TX_IRQn                            =    41,    /*!<* DMA_SPI0_TX */
    DMA_SPI0_RX_IRQn                            =    42,    /*!<* DMA_SPI0_RX */
    DMA_SPI1_TX_IRQn                            =    43,    /*!<* DMA_SPI1_TX */
    DMA_SPI1_RX_IRQn                            =    44,    /*!<* DMA_SPI1_RX */
    DMA_SPI2_TX_IRQn                            =    45,    /*!<* DMA_SPI2_TX */
    DMA_SPI2_RX_IRQn                            =    46,    /*!<* DMA_SPI2_RX */
    DMA_UART0_TX_IRQn                           =    47,    /*!<* DMA_UART0_TX */
    DMA_UART0_RX_IRQn                           =    48,    /*!<* DMA_UART0_RX */
    DMA_UART1_TX_IRQn                           =    49,    /*!<* DMA_UART1_TX */
    DMA_UART1_RX_IRQn                           =    50,    /*!<* DMA_UART1_RX */
    DMA_I2C0S_TX_IRQn                           =    51,    /*!<* DMA_I2C0S_TX */
    DMA_I2C0S_RX_IRQn                           =    52,    /*!<* DMA_I2C0S_RX */
    DMA_I2C0M_IRQn                              =    53,    /*!<* DMA_I2C0M */
    DMA_I2C1S_TX_IRQn                           =    54,    /*!<* DMA_I2C1S_TX */
    DMA_I2C1S_RX_IRQn                           =    55,    /*!<* DMA_I2C1S_RX */
    DMA_I2C1M_IRQn                              =    56,    /*!<* DMA_I2C1M */
    DMA_I2C2S_TX_IRQn                           =    57,    /*!<* DMA_I2C2S_TX */
    DMA_I2C2S_RX_IRQn                           =    58,    /*!<* DMA_I2C2S_RX */
    DMA_I2C2M_IRQn                              =    59,    /*!<* DMA_I2C2M */
    DMA_MDIO_TX_IRQn                            =    60,    /*!<* DMA_MDIO_TX */
    DMA_MDIO_RX_IRQn                            =    61,    /*!<* DMA_MDIO_RX */
    DMA_FLASH_IRQn                              =    62,    /*!<* DMA_FLASH */
    DMA_ADC_IRQn                                =    63,    /*!<* DMA_ADC */
    PLL_IRQn                                    =    64,    /*!<* PLL */
    HFOSC_IRQn                                  =    65,    /*!<* HFOSC */
    ADC_IRQn                                    =    66,    /*!<* ADC */
    SEQ_IRQn                                    =    67,    /*!<* SEQ */
    COMP0_IRQn                                  =    68,    /*!<* COMP0 */
    COMP1_IRQn                                  =    69,    /*!<* COMP1 */
    COMP2_IRQn                                  =    70,    /*!<* COMP2 */
    COMP3_IRQn                                  =    71,    /*!<* COMP3 */
    VDAC_IRQn                                   =    72,    /*!<* VDAC */
    DMA_TRIG0_IRQn                              =    81,    /*!<* DMA_TRIG0 */
    DMA_TRIG1_IRQn                              =    82,    /*!<* DMA_TRIG1 */
    DMA_SW0_IRQn                                =    83,    /*!<* DMA_SW0 */
    DMA_SW1_IRQn                                =    84,    /*!<* DMA_SW1 */
    CACHE_IRQn                                  =    85,    /*!<* CACHE */
    DIG_COMP_IRQn                               =    86,    /*!<* DIG_COMP */
    GPIO_INTA_IRQn                              =    87,    /*!<* GPIO_INTA */
    GPIO_INTB_IRQn                              =    88,    /*!<* GPIO_INTB */
} IRQn_Type;  /* typedef name for fixed interrupt numbers */


/** @addtogroup Configuration_of_CMSIS
  * @brief Configuration of the Cortex-M4 Processor and Core Peripherals
  * @todo switch to M33 when port is done.  
  * @{
  */
#define __CM4_REV                 0x0001U  /*!< Core revision r0p1                            */
#define __MPU_PRESENT             1U       /*!< ADUCM41X provides an MPU                      */
#define __NVIC_PRIO_BITS          3U       /*!< ADUCM41X uses 3 Bits for the Priority Levels  */
#define __Vendor_SysTickConfig    0U       /*!< Set to 1 if different SysTick Config is used  */
#define __FPU_PRESENT             1U       /*!< FPU present                                   */

//#include <core_cm33.h>                    /*!< Cortex-M33 processor and core peripherals */
#include <core_cm4.h>                       /*!< Cortex-M4 processor and core peripherals  */
#include "system_ADuCM410.h"                /*!< ADUCM410 System                           */
/** @} */

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TMR_ADDR_RDEF_H_
#define TMR_ADDR_RDEF_H_    /* TMR: GPT0 */

/* ====================================================================================================
        TMR Module Instances Address and Mask Definitions
   ==================================================================================================== */
#define INST_GPT0                                                (0X40000000U)    /* gpt0: */
#define INST_GPT1                                                (0X40000400U)    /* gpt1: */
#define INST_GPT2                                                (0X40000800U)    /* gpt2: */

#define MASK_TMR                                                 (0XFFFFFFFFU)    /* TMR: GPT0 */

/* ====================================================================================================
        TMR Module Register Address Offset Definitions
   ==================================================================================================== */
#define IDX_TMR_LD                                              (0X00000000U)    /* 16-bit Load Value Register */
#define IDX_TMR_VAL                                             (0X00000004U)    /* 16-bit Timer Value Register */
#define IDX_TMR_CON                                             (0X00000008U)    /* Control Register */
#define IDX_TMR_CLRI                                            (0X0000000CU)    /* Clear Interrupt Register */
#define IDX_TMR_STA                                             (0X0000001CU)    /* Status Register */

/* ====================================================================================================
        TMR Module Register ResetValue Definitions
   ==================================================================================================== */
#define RSTVAL_TMR_LD                                              (0X0) 
#define RSTVAL_TMR_VAL                                             (0X0) 
#define RSTVAL_TMR_CON                                             (0XA) 
#define RSTVAL_TMR_CLRI                                            (0X0) 
#define RSTVAL_TMR_STA                                             (0X0) 

/* ====================================================================================================
        TMR Module Register BitPositions, Lengths, Masks and Enumerations Definitions
   ==================================================================================================== */

/* ----------------------------------------------------------------------------------------------------
          LD                                                    Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_TMR_LD_LOAD                                      (0U)           /* Load Value */
#define BITL_TMR_LD_LOAD                                      (16U)          /* Load Value */
#define BITM_TMR_LD_LOAD                                      (0XFFFFU)      /* Load Value */

/* ----------------------------------------------------------------------------------------------------
          VAL                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_TMR_VAL_COUNT_VAL                                (0U)           /* Current Count */
#define BITL_TMR_VAL_COUNT_VAL                                (16U)          /* Current Count */
#define BITM_TMR_VAL_COUNT_VAL                                (0XFFFFU)      /* Current Count */

/* ----------------------------------------------------------------------------------------------------
          CON                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_TMR_CON_PRE                                      (0U)           /* Prescaler */
#define BITL_TMR_CON_PRE                                      (2U)           /* Prescaler */
#define BITM_TMR_CON_PRE                                      (0X0003U)      /* Prescaler */
#define BITP_TMR_CON_UP                                       (2U)           /* Count up */
#define BITL_TMR_CON_UP                                       (1U)           /* Count up */
#define BITM_TMR_CON_UP                                       (0X0004U)      /* Count up */
#define BITP_TMR_CON_MOD                                      (3U)           /* Timer Mode */
#define BITL_TMR_CON_MOD                                      (1U)           /* Timer Mode */
#define BITM_TMR_CON_MOD                                      (0X0008U)      /* Timer Mode */
#define BITP_TMR_CON_ENABLE                                   (4U)           /* Timer Enable */
#define BITL_TMR_CON_ENABLE                                   (1U)           /* Timer Enable */
#define BITM_TMR_CON_ENABLE                                   (0X0010U)      /* Timer Enable */
#define BITP_TMR_CON_CLK                                      (5U)           /* Clock Select */
#define BITL_TMR_CON_CLK                                      (2U)           /* Clock Select */
#define BITM_TMR_CON_CLK                                      (0X0060U)      /* Clock Select */
#define BITP_TMR_CON_RLD                                      (7U)           /* Reload Control */
#define BITL_TMR_CON_RLD                                      (1U)           /* Reload Control */
#define BITM_TMR_CON_RLD                                      (0X0080U)      /* Reload Control */

#define ENUM_TMR_CON_RLD_EN                                   (0X0001U)      /* Resets the Up/down Counter When GPTCLRI[0] is Set */
#define ENUM_TMR_CON_RLD_DIS                                  (0X0000U)      /* Up/Down Counter is Only Reset on a Timeout Event */
#define ENUM_TMR_CON_CLK_PCLK0                                (0X0000U)      /* PCLK. */
#define ENUM_TMR_CON_CLK_HCLK                                 (0X0001U)      /* ROOT_CLK */
#define ENUM_TMR_CON_CLK_LFOSC                                (0X0002U)      /* LFOSC. 32 KHz OSC */
#define ENUM_TMR_CON_CLK_HFXTAL                               (0X0003U)      /* HFXTAL. 16 MHz OSC or XTAL (Dependent on CLKCON0.11) */
#define ENUM_TMR_CON_ENABLE_DIS                               (0X0000U)      /* DIS. Timer is Disabled (default) */
#define ENUM_TMR_CON_ENABLE_EN                                (0X0001U)      /* EN. Timer is Enabled */
#define ENUM_TMR_CON_MOD_FREERUN                              (0X0000U)      /* FREERUN.Timer Runs in Free Running Mode */
#define ENUM_TMR_CON_MOD_PERIODIC                             (0X0001U)      /* PERIODIC. Timer Runs in Periodic Mode (default) */
#define ENUM_TMR_CON_UP_DIS                                   (0X0000U)      /* DIS. Timer is Set to Count Down (default) */
#define ENUM_TMR_CON_UP_EN                                    (0X0001U)      /* EN. Timer is Set to Count up */
#define ENUM_TMR_CON_PRE_DIV1OR4                              (0X0000U)      /* Source_clock / [1 or 4] */
#define ENUM_TMR_CON_PRE_DIV16                                (0X0001U)      /* Source_clock / 16 */
#define ENUM_TMR_CON_PRE_DIV256                               (0X0002U)      /* Source_clock / 256 */
#define ENUM_TMR_CON_PRE_DIV32768                             (0X0003U)      /* Source_clock / 32,768 */

/* ----------------------------------------------------------------------------------------------------
          CLRI                                                  Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_TMR_CLRI_TMOUT                                   (0U)           /* Clear Timeout Interrupt */
#define BITL_TMR_CLRI_TMOUT                                   (1U)           /* Clear Timeout Interrupt */
#define BITM_TMR_CLRI_TMOUT                                   (0X0001U)      /* Clear Timeout Interrupt */

/* ----------------------------------------------------------------------------------------------------
          STA                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_TMR_STA_TMOUT                                    (0U)           /* Timeout Event Occurred */
#define BITL_TMR_STA_TMOUT                                    (1U)           /* Timeout Event Occurred */
#define BITM_TMR_STA_TMOUT                                    (0X0001U)      /* Timeout Event Occurred */
#define BITP_TMR_STA_BUSY                                     (6U)           /* Timer Busy */
#define BITL_TMR_STA_BUSY                                     (1U)           /* Timer Busy */
#define BITM_TMR_STA_BUSY                                     (0X0040U)      /* Timer Busy */
#define BITP_TMR_STA_PDOK                                     (7U)           /* GPTCLRI Synchronization */
#define BITL_TMR_STA_PDOK                                     (1U)           /* GPTCLRI Synchronization */
#define BITM_TMR_STA_PDOK                                     (0X0080U)      /* GPTCLRI Synchronization */

#define ENUM_TMR_STA_PDOK_CLR_FLAG                            (0X0000U)      /* CLR. the Interrupt is Cleared in the Timer Clock Domain */
#define ENUM_TMR_STA_PDOK_SET_FLAG                            (0X0001U)      /* SET. GPTCLRI[0] is Being Updated in the Timer Clock Domain */
#define ENUM_TMR_STA_BUSY_CLR_FLAG                            (0X0000U)      /* Timer Ready to Receive Commands to GPTCON */
#define ENUM_TMR_STA_BUSY_SET_FLAG                            (0X0001U)      /* Timer Not Ready to Receive Commands to GPTCON */
#define ENUM_TMR_STA_TMOUT_CLR_FLAG                           (0X0000U)      /* No Timeout Event Has Occurred */
#define ENUM_TMR_STA_TMOUT_SET_FLAG                           (0X0001U)      /* a Timeout Event Has Occurred */

#endif  /* end ifndef TMR_ADDR_RDEF_H_ */


#ifndef TIMER_ADDR_RDEF_H_
#define TIMER_ADDR_RDEF_H_    /* TIMER: Timer */

/* ====================================================================================================
        TIMER Module Instances Address and Mask Definitions
   ==================================================================================================== */
#define INST_GPTH0                                               (0X40000C00U)    /* gpth0: */
#define INST_GPTH1                                               (0X40001000U)    /* gpth1: */

#define MASK_TIMER                                               (0X000000FFU)    /* TIMER: Timer */

/* ====================================================================================================
        TIMER Module Register Address Offset Definitions
   ==================================================================================================== */
#define IDX_TIMER_CTL                                           (0X00U)    /* Timer Control */
#define IDX_TIMER_CNT                                           (0X04U)    /* Count Value */
#define IDX_TIMER_STATUS                                        (0X08U)    /* Timer Status */
#define IDX_TIMER_CFG0                                          (0X10U)    /* Capture Compare Configuration */
#define IDX_TIMER_CFG1                                          (0X14U)    /* Capture Compare Configuration */
#define IDX_TIMER_CFG2                                          (0X18U)    /* Capture Compare Configuration */
#define IDX_TIMER_CFG3                                          (0X1CU)    /* Capture Compare Configuration */
#define IDX_TIMER_CC0                                           (0X20U)    /* Compare and Capture Value */
#define IDX_TIMER_CC1                                           (0X24U)    /* Compare and Capture Value */
#define IDX_TIMER_CC2                                           (0X28U)    /* Compare and Capture Value */
#define IDX_TIMER_CC3                                           (0X2CU)    /* Compare and Capture Value */

/* ====================================================================================================
        TIMER Module Register ResetValue Definitions
   ==================================================================================================== */
#define RSTVAL_TIMER_CTL                                           (0X170) 
#define RSTVAL_TIMER_CNT                                           (0X0) 
#define RSTVAL_TIMER_STATUS                                        (0X0) 
#define RSTVAL_TIMER_CFG0                                          (0X0) 
#define RSTVAL_TIMER_CFG1                                          (0X0) 
#define RSTVAL_TIMER_CFG2                                          (0X0) 
#define RSTVAL_TIMER_CFG3                                          (0X0) 
#define RSTVAL_TIMER_CC0                                           (0X0) 
#define RSTVAL_TIMER_CC1                                           (0X0) 
#define RSTVAL_TIMER_CC2                                           (0X0) 
#define RSTVAL_TIMER_CC3                                           (0X0) 

/* ====================================================================================================
        TIMER Module Register BitPositions, Lengths, Masks and Enumerations Definitions
   ==================================================================================================== */

/* ----------------------------------------------------------------------------------------------------
          CTL                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_TIMER_CTL_EN                                     (0U)           /* Timer Enable */
#define BITL_TIMER_CTL_EN                                     (1U)           /* Timer Enable */
#define BITM_TIMER_CTL_EN                                     (0X00000001U)  /* Timer Enable */
#define BITP_TIMER_CTL_SEL                                    (2U)           /* Clock Source Select */
#define BITL_TIMER_CTL_SEL                                    (2U)           /* Clock Source Select */
#define BITM_TIMER_CTL_SEL                                    (0X0000000CU)  /* Clock Source Select */
#define BITP_TIMER_CTL_PRE                                    (4U)           /* Timer Prescale */
#define BITL_TIMER_CTL_PRE                                    (5U)           /* Timer Prescale */
#define BITM_TIMER_CTL_PRE                                    (0X000001F0U)  /* Timer Prescale */

#define ENUM_TIMER_CTL_SEL_PCLK                               (0X00000000U)  /* PCLK */
#define ENUM_TIMER_CTL_SEL_SYSCLK                             (0X00000001U)  /* HCLK */
#define ENUM_TIMER_CTL_SEL_LFOSC                              (0X00000002U)  /* High Frequency Oscillator */
#define ENUM_TIMER_CTL_SEL_HFXSTAL                            (0X00000003U)  /* High Frequency XTALL */

/* ----------------------------------------------------------------------------------------------------
          CNT                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_TIMER_CNT_CNT                                    (0U)           /* Current Counter Value. */
#define BITL_TIMER_CNT_CNT                                    (32U)          /* Current Counter Value. */
#define BITM_TIMER_CNT_CNT                                    (0XFFFFFFFFU)  /* Current Counter Value. */

/* ----------------------------------------------------------------------------------------------------
          STATUS                                                Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_TIMER_STATUS_CC0_STATUS                          (0U)           /* CC0 Status */
#define BITL_TIMER_STATUS_CC0_STATUS                          (1U)           /* CC0 Status */
#define BITM_TIMER_STATUS_CC0_STATUS                          (0X00000001U)  /* CC0 Status */
#define BITP_TIMER_STATUS_CC1_STATUS                          (1U)           /* CC1 Status */
#define BITL_TIMER_STATUS_CC1_STATUS                          (1U)           /* CC1 Status */
#define BITM_TIMER_STATUS_CC1_STATUS                          (0X00000002U)  /* CC1 Status */
#define BITP_TIMER_STATUS_CC2_STATUS                          (2U)           /* CC2 Status */
#define BITL_TIMER_STATUS_CC2_STATUS                          (1U)           /* CC2 Status */
#define BITM_TIMER_STATUS_CC2_STATUS                          (0X00000004U)  /* CC2 Status */
#define BITP_TIMER_STATUS_CC3_STATUS                          (3U)           /* CC3 Status */
#define BITL_TIMER_STATUS_CC3_STATUS                          (1U)           /* CC3 Status */
#define BITM_TIMER_STATUS_CC3_STATUS                          (0X00000008U)  /* CC3 Status */

/* ----------------------------------------------------------------------------------------------------
          CFG0                                                  Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_TIMER_CFG_N__MODE                                (0U)           /* Capture or Compare Mode */
#define BITL_TIMER_CFG_N__MODE                                (1U)           /* Capture or Compare Mode */
#define BITM_TIMER_CFG_N__MODE                                (0X00000001U)  /* Capture or Compare Mode */
#define BITP_TIMER_CFG_N__CC_EN                               (1U)           /* Capture Compare Enabled */
#define BITL_TIMER_CFG_N__CC_EN                               (1U)           /* Capture Compare Enabled */
#define BITM_TIMER_CFG_N__CC_EN                               (0X00000002U)  /* Capture Compare Enabled */
#define BITP_TIMER_CFG_N__EVENT_SEL                           (2U)           /* Capture Events Select */
#define BITL_TIMER_CFG_N__EVENT_SEL                           (4U)           /* Capture Events Select */
#define BITM_TIMER_CFG_N__EVENT_SEL                           (0X0000003CU)  /* Capture Events Select */

#define ENUM_TIMER_CFG_N__MODE_CMP                            (0X00000000U)  /* Compare */
#define ENUM_TIMER_CFG_N__MODE_CAP                            (0X00000001U)  /* Capture */

/* ----------------------------------------------------------------------------------------------------
          CC0                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_TIMER_CC_N__CC                                   (0U)           /* Capture or Compare Value */
#define BITL_TIMER_CC_N__CC                                   (32U)          /* Capture or Compare Value */
#define BITM_TIMER_CC_N__CC                                   (0XFFFFFFFFU)  /* Capture or Compare Value */

#endif  /* end ifndef TIMER_ADDR_RDEF_H_ */


#ifndef MISC16_ADDR_RDEF_H_
#define MISC16_ADDR_RDEF_H_    /* MISC16: Your module description, here. */

/* ====================================================================================================
        MISC16 Module Instances Address and Mask Definitions
   ==================================================================================================== */
#define INST_MISC                                                (0X40002000U)    /* misc: */

#define MASK_MISC16                                              (0X00000FFFU)    /* MISC16: Your module description, here. */

/* ====================================================================================================
        MISC16 Module Register Address Offset Definitions
   ==================================================================================================== */
#define IDX_MISC16_ADIID                                        (0X020U)    /* ADI ID */
#define IDX_MISC16_CHIPID                                       (0X024U)    /* Chip ID */
#define IDX_MISC16_USERKEY                                      (0X134U)    /* Open to Customer to Protect Important Registers */

/* ====================================================================================================
        MISC16 Module Register ResetValue Definitions
   ==================================================================================================== */
#define RSTVAL_MISC16_ADIID                                        (0X4144) 
#define RSTVAL_MISC16_CHIPID                                       (0X571) 
#define RSTVAL_MISC16_USERKEY                                      (0X0) 

/* ====================================================================================================
        MISC16 Module Register BitPositions, Lengths, Masks and Enumerations Definitions
   ==================================================================================================== */

/* ----------------------------------------------------------------------------------------------------
          ADIID                                                 Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_MISC16_ADIID_ADIID                               (0U)           /* ADI ID */
#define BITL_MISC16_ADIID_ADIID                               (16U)          /* ADI ID */
#define BITM_MISC16_ADIID_ADIID                               (0X0000FFFFU)  /* ADI ID */

/* ----------------------------------------------------------------------------------------------------
          CHIPID                                                Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_MISC16_CHIPID_REVISION                           (0U)           /* Silicon Revision Number */
#define BITL_MISC16_CHIPID_REVISION                           (4U)           /* Silicon Revision Number */
#define BITM_MISC16_CHIPID_REVISION                           (0X0000000FU)  /* Silicon Revision Number */
#define BITP_MISC16_CHIPID_PARTID                             (4U)           /* Part Identifier */
#define BITL_MISC16_CHIPID_PARTID                             (12U)          /* Part Identifier */
#define BITM_MISC16_CHIPID_PARTID                             (0X0000FFF0U)  /* Part Identifier */

/* ----------------------------------------------------------------------------------------------------
          USERKEY                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_MISC16_USERKEY_KEY                               (0U)           /* User Key */
#define BITL_MISC16_USERKEY_KEY                               (32U)          /* User Key */
#define BITM_MISC16_USERKEY_KEY                               (0XFFFFFFFFU)  /* User Key */

#endif  /* end ifndef MISC16_ADDR_RDEF_H_ */


#ifndef WUT_ADDR_RDEF_H_
#define WUT_ADDR_RDEF_H_    /* WUT: WUT */

/* ====================================================================================================
        WUT Module Instances Address and Mask Definitions
   ==================================================================================================== */
#define INST_WUT                                                 (0X40003000U)    /* wut: */

#define MASK_WUT                                                 (0XFFFFFFFFU)    /* WUT: WUT */

/* ====================================================================================================
        WUT Module Register Address Offset Definitions
   ==================================================================================================== */
#define IDX_WUT_T4VAL0                                          (0X00000000U)    /* Current Count Value - LS 16 Bits */
#define IDX_WUT_T4VAL1                                          (0X00000004U)    /* Current Count Value - MS 16 Bits */
#define IDX_WUT_T4CON                                           (0X00000008U)    /* Control Register */
#define IDX_WUT_T4INC                                           (0X0000000CU)    /* 12-bit Interval for Wakeup Field a */
#define IDX_WUT_T4WUFB0                                         (0X00000010U)    /* Wakeup Field B - LS 16 Bits */
#define IDX_WUT_T4WUFB1                                         (0X00000014U)    /* Wakeup Field B - MS 16 Bits */
#define IDX_WUT_T4WUFC0                                         (0X00000018U)    /* Wakeup Field C - LS 16 Bits */
#define IDX_WUT_T4WUFC1                                         (0X0000001CU)    /* Wakeup Field C - MS 16 Bits */
#define IDX_WUT_T4WUFD0                                         (0X00000020U)    /* Wakeup Field D - LS 16 Bits */
#define IDX_WUT_T4WUFD1                                         (0X00000024U)    /* Wakeup Field D - MS 16 Bits */
#define IDX_WUT_T4IEN                                           (0X00000028U)    /* Interrupt Enable Register */
#define IDX_WUT_T4STA                                           (0X0000002CU)    /* Status Register */
#define IDX_WUT_T4CLRI                                          (0X00000030U)    /* Clear Interrupt Register */
#define IDX_WUT_T4WUFA0                                         (0X0000003CU)    /* Wakeup Field a - LS 16 Bits */
#define IDX_WUT_T4WUFA1                                         (0X00000040U)    /* Wakeup Field a - MS 16 Bits */

/* ====================================================================================================
        WUT Module Register ResetValue Definitions
   ==================================================================================================== */
#define RSTVAL_WUT_T4VAL0                                          (0X0) 
#define RSTVAL_WUT_T4VAL1                                          (0X0) 
#define RSTVAL_WUT_T4CON                                           (0X40) 
#define RSTVAL_WUT_T4INC                                           (0XC8) 
#define RSTVAL_WUT_T4WUFB0                                         (0X1FFF) 
#define RSTVAL_WUT_T4WUFB1                                         (0X0) 
#define RSTVAL_WUT_T4WUFC0                                         (0X2FFF) 
#define RSTVAL_WUT_T4WUFC1                                         (0X0) 
#define RSTVAL_WUT_T4WUFD0                                         (0X3FFF) 
#define RSTVAL_WUT_T4WUFD1                                         (0X0) 
#define RSTVAL_WUT_T4IEN                                           (0X0) 
#define RSTVAL_WUT_T4STA                                           (0X0) 
#define RSTVAL_WUT_T4CLRI                                          (0X0) 
#define RSTVAL_WUT_T4WUFA0                                         (0X1900) 
#define RSTVAL_WUT_T4WUFA1                                         (0X0) 

/* ====================================================================================================
        WUT Module Register BitPositions, Lengths, Masks and Enumerations Definitions
   ==================================================================================================== */

/* ----------------------------------------------------------------------------------------------------
          T4VAL0                                                Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_WUT_T4VAL0_T4VALL                                (0U)           /* Current Count Low */
#define BITL_WUT_T4VAL0_T4VALL                                (16U)          /* Current Count Low */
#define BITM_WUT_T4VAL0_T4VALL                                (0XFFFFU)      /* Current Count Low */

/* ----------------------------------------------------------------------------------------------------
          T4VAL1                                                Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_WUT_T4VAL1_T4VALH                                (0U)           /* Current Count High */
#define BITL_WUT_T4VAL1_T4VALH                                (16U)          /* Current Count High */
#define BITM_WUT_T4VAL1_T4VALH                                (0XFFFFU)      /* Current Count High */

/* ----------------------------------------------------------------------------------------------------
          T4CON                                                 Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_WUT_T4CON_PRE                                    (0U)           /* Prescaler */
#define BITL_WUT_T4CON_PRE                                    (2U)           /* Prescaler */
#define BITM_WUT_T4CON_PRE                                    (0X0003U)      /* Prescaler */
#define BITP_WUT_T4CON_FREEZE                                 (3U)           /* Freeze Enable */
#define BITL_WUT_T4CON_FREEZE                                 (1U)           /* Freeze Enable */
#define BITM_WUT_T4CON_FREEZE                                 (0X0008U)      /* Freeze Enable */
#define BITP_WUT_T4CON_TMODE                                  (6U)           /* Timer Mode */
#define BITL_WUT_T4CON_TMODE                                  (1U)           /* Timer Mode */
#define BITM_WUT_T4CON_TMODE                                  (0X0040U)      /* Timer Mode */
#define BITP_WUT_T4CON_ENABLE                                 (7U)           /* Timer Enable */
#define BITL_WUT_T4CON_ENABLE                                 (1U)           /* Timer Enable */
#define BITM_WUT_T4CON_ENABLE                                 (0X0080U)      /* Timer Enable */
#define BITP_WUT_T4CON_WUEN                                   (8U)           /* Wakeup Enable */
#define BITL_WUT_T4CON_WUEN                                   (1U)           /* Wakeup Enable */
#define BITM_WUT_T4CON_WUEN                                   (0X0100U)      /* Wakeup Enable */
#define BITP_WUT_T4CON_CLK                                    (9U)           /* Clock Select */
#define BITL_WUT_T4CON_CLK                                    (2U)           /* Clock Select */
#define BITM_WUT_T4CON_CLK                                    (0X0600U)      /* Clock Select */
#define BITP_WUT_T4CON_STOP_WUFA                              (11U)          /* Disables Updating Field a Register T4WUFA */
#define BITL_WUT_T4CON_STOP_WUFA                              (1U)           /* Disables Updating Field a Register T4WUFA */
#define BITM_WUT_T4CON_STOP_WUFA                              (0X0800U)      /* Disables Updating Field a Register T4WUFA */

#define ENUM_WUT_T4CON_CLK_PCLK                               (0X0000U)      /* PCLK: PCLK (default) */
#define ENUM_WUT_T4CON_CLK_LFOSC                              (0X0001U)      /* LFOSC: 32 KHz Internal Oscillator */
#define ENUM_WUT_T4CON_CLK_LFOSC1                             (0X0002U)      /* LFOSC: 32kHz Internal Oscillator */
#define ENUM_WUT_T4CON_CLK_ECLKIN                             (0X0003U)      /* ECLKIN: External Clock from P1.0 */
#define ENUM_WUT_T4CON_WUEN_DIS                               (0X0000U)      /* DIS: Cleared by User to Disable the Wake up Timer When the Core Clock is off */
#define ENUM_WUT_T4CON_WUEN_EN                                (0X0001U)      /* EN: Set by User to Enable the Wake up Timer Even When the Core Clock is Off. */
#define ENUM_WUT_T4CON_ENABLE_DIS                             (0X0000U)      /* DIS: Disable the Timer (default). */
#define ENUM_WUT_T4CON_ENABLE_EN                              (0X0001U)      /* EN: Enable the Timer. */
#define ENUM_WUT_T4CON_TMODE_PERIODIC                         (0X0000U)      /* PERIODIC: Cleared by User to Operate in Periodic Mode. in This Mode, the Timer Counts up to T4WUFD */
#define ENUM_WUT_T4CON_TMODE_FREERUN                          (0X0001U)      /* FREERUN: Set by User to Operate in Free Running Mode (default). */
#define ENUM_WUT_T4CON_FREEZE_DIS                             (0X0000U)      /* DIS: Cleared by User to Disable This Feature (default). */
#define ENUM_WUT_T4CON_FREEZE_EN                              (0X0001U)      /* EN: Set by User to Enable the Freeze of the High 16-bits After the Lower Bits Have Been Read from T4VAL0. This Ensures That the Software Will Read an Atomic Shot of the Timer. T4VAL1 Unfreezes After It Has Been Read. */
#define ENUM_WUT_T4CON_PRE_PREDIV1                            (0X0000U)      /* PREDIV1: Source Clock/1 (default). If the Selected Clock Source is PCLK, Then This Setting Results in a Prescaler of 4. */
#define ENUM_WUT_T4CON_PRE_PREDIV16                           (0X0001U)      /* PREDIV16: Source Clock/16 */
#define ENUM_WUT_T4CON_PRE_PREDIV256                          (0X0002U)      /* PREDIV256: Source Clock/256 */
#define ENUM_WUT_T4CON_PRE_PREDIV32768                        (0X0003U)      /* PREDIV32768: Source Clock/32,768 */

/* ----------------------------------------------------------------------------------------------------
          T4INC                                                 Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_WUT_T4INC_INTERVAL                               (0U)           /* Interval for Wakeup Field a */
#define BITL_WUT_T4INC_INTERVAL                               (12U)          /* Interval for Wakeup Field a */
#define BITM_WUT_T4INC_INTERVAL                               (0X0FFFU)      /* Interval for Wakeup Field a */

/* ----------------------------------------------------------------------------------------------------
          T4WUFB0                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_WUT_T4WUFB0_T4WUFBL                              (0U)           /* Wakeup Field B Low */
#define BITL_WUT_T4WUFB0_T4WUFBL                              (16U)          /* Wakeup Field B Low */
#define BITM_WUT_T4WUFB0_T4WUFBL                              (0XFFFFU)      /* Wakeup Field B Low */

/* ----------------------------------------------------------------------------------------------------
          T4WUFB1                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_WUT_T4WUFB1_T4WUFBH                              (0U)           /* Wakeup Field B High */
#define BITL_WUT_T4WUFB1_T4WUFBH                              (16U)          /* Wakeup Field B High */
#define BITM_WUT_T4WUFB1_T4WUFBH                              (0XFFFFU)      /* Wakeup Field B High */

/* ----------------------------------------------------------------------------------------------------
          T4WUFC0                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_WUT_T4WUFC0_T4WUFCL                              (0U)           /* Wakeup Field C Low */
#define BITL_WUT_T4WUFC0_T4WUFCL                              (16U)          /* Wakeup Field C Low */
#define BITM_WUT_T4WUFC0_T4WUFCL                              (0XFFFFU)      /* Wakeup Field C Low */

/* ----------------------------------------------------------------------------------------------------
          T4WUFC1                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_WUT_T4WUFC1_T4WUFCH                              (0U)           /* Wakeup Field C High */
#define BITL_WUT_T4WUFC1_T4WUFCH                              (16U)          /* Wakeup Field C High */
#define BITM_WUT_T4WUFC1_T4WUFCH                              (0XFFFFU)      /* Wakeup Field C High */

/* ----------------------------------------------------------------------------------------------------
          T4WUFD0                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_WUT_T4WUFD0_T4WUFD0                              (0U)           /* Wakeup Field D Low */
#define BITL_WUT_T4WUFD0_T4WUFD0                              (16U)          /* Wakeup Field D Low */
#define BITM_WUT_T4WUFD0_T4WUFD0                              (0XFFFFU)      /* Wakeup Field D Low */

/* ----------------------------------------------------------------------------------------------------
          T4WUFD1                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_WUT_T4WUFD1_T4WUFDH                              (0U)           /* Wakeup Field D High */
#define BITL_WUT_T4WUFD1_T4WUFDH                              (16U)          /* Wakeup Field D High */
#define BITM_WUT_T4WUFD1_T4WUFDH                              (0XFFFFU)      /* Wakeup Field D High */

/* ----------------------------------------------------------------------------------------------------
          T4IEN                                                 Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_WUT_T4IEN_WUFA                                   (0U)           /* T4WUFA Interrupt Enable */
#define BITL_WUT_T4IEN_WUFA                                   (1U)           /* T4WUFA Interrupt Enable */
#define BITM_WUT_T4IEN_WUFA                                   (0X0001U)      /* T4WUFA Interrupt Enable */
#define BITP_WUT_T4IEN_WUFB                                   (1U)           /* T4WUFB Interrupt Enable */
#define BITL_WUT_T4IEN_WUFB                                   (1U)           /* T4WUFB Interrupt Enable */
#define BITM_WUT_T4IEN_WUFB                                   (0X0002U)      /* T4WUFB Interrupt Enable */
#define BITP_WUT_T4IEN_WUFC                                   (2U)           /* T4WUFC Interrupt Enable */
#define BITL_WUT_T4IEN_WUFC                                   (1U)           /* T4WUFC Interrupt Enable */
#define BITM_WUT_T4IEN_WUFC                                   (0X0004U)      /* T4WUFC Interrupt Enable */
#define BITP_WUT_T4IEN_WUFD                                   (3U)           /* T4WUFD Interrupt Enable */
#define BITL_WUT_T4IEN_WUFD                                   (1U)           /* T4WUFD Interrupt Enable */
#define BITM_WUT_T4IEN_WUFD                                   (0X0008U)      /* T4WUFD Interrupt Enable */
#define BITP_WUT_T4IEN_ROLL                                   (4U)           /* Rollover Interrupt Enable */
#define BITL_WUT_T4IEN_ROLL                                   (1U)           /* Rollover Interrupt Enable */
#define BITM_WUT_T4IEN_ROLL                                   (0X0010U)      /* Rollover Interrupt Enable */

/* ----------------------------------------------------------------------------------------------------
          T4STA                                                 Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_WUT_T4STA_WUFA                                   (0U)           /* T4WUFA Interrupt Flag */
#define BITL_WUT_T4STA_WUFA                                   (1U)           /* T4WUFA Interrupt Flag */
#define BITM_WUT_T4STA_WUFA                                   (0X0001U)      /* T4WUFA Interrupt Flag */
#define BITP_WUT_T4STA_WUFB                                   (1U)           /* T4WUFB Interrupt Flag */
#define BITL_WUT_T4STA_WUFB                                   (1U)           /* T4WUFB Interrupt Flag */
#define BITM_WUT_T4STA_WUFB                                   (0X0002U)      /* T4WUFB Interrupt Flag */
#define BITP_WUT_T4STA_WUFC                                   (2U)           /* T4WUFC Interrupt Flag */
#define BITL_WUT_T4STA_WUFC                                   (1U)           /* T4WUFC Interrupt Flag */
#define BITM_WUT_T4STA_WUFC                                   (0X0004U)      /* T4WUFC Interrupt Flag */
#define BITP_WUT_T4STA_WUFD                                   (3U)           /* T4WUFD Interrupt Flag */
#define BITL_WUT_T4STA_WUFD                                   (1U)           /* T4WUFD Interrupt Flag */
#define BITM_WUT_T4STA_WUFD                                   (0X0008U)      /* T4WUFD Interrupt Flag */
#define BITP_WUT_T4STA_ROLL                                   (4U)           /* Rollover Interrupt Flag */
#define BITL_WUT_T4STA_ROLL                                   (1U)           /* Rollover Interrupt Flag */
#define BITM_WUT_T4STA_ROLL                                   (0X0010U)      /* Rollover Interrupt Flag */
#define BITP_WUT_T4STA_IRQCRY                                 (6U)           /* Wakeup Status to Powerdown */
#define BITL_WUT_T4STA_IRQCRY                                 (1U)           /* Wakeup Status to Powerdown */
#define BITM_WUT_T4STA_IRQCRY                                 (0X0040U)      /* Wakeup Status to Powerdown */
#define BITP_WUT_T4STA_FREEZE                                 (7U)           /* Timer Value Freeze */
#define BITL_WUT_T4STA_FREEZE                                 (1U)           /* Timer Value Freeze */
#define BITM_WUT_T4STA_FREEZE                                 (0X0080U)      /* Timer Value Freeze */
#define BITP_WUT_T4STA_PDOK                                   (8U)           /* Enable Bit Synchronized */
#define BITL_WUT_T4STA_PDOK                                   (1U)           /* Enable Bit Synchronized */
#define BITM_WUT_T4STA_PDOK                                   (0X0100U)      /* Enable Bit Synchronized */

/* ----------------------------------------------------------------------------------------------------
          T4CLRI                                                Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_WUT_T4CLRI_WUFA                                  (0U)           /* T4WUFA Interrupt Clear */
#define BITL_WUT_T4CLRI_WUFA                                  (1U)           /* T4WUFA Interrupt Clear */
#define BITM_WUT_T4CLRI_WUFA                                  (0X0001U)      /* T4WUFA Interrupt Clear */
#define BITP_WUT_T4CLRI_WUFB                                  (1U)           /* T4WUFB Interrupt Clear */
#define BITL_WUT_T4CLRI_WUFB                                  (1U)           /* T4WUFB Interrupt Clear */
#define BITM_WUT_T4CLRI_WUFB                                  (0X0002U)      /* T4WUFB Interrupt Clear */
#define BITP_WUT_T4CLRI_WUFC                                  (2U)           /* T4WUFC Interrupt Clear */
#define BITL_WUT_T4CLRI_WUFC                                  (1U)           /* T4WUFC Interrupt Clear */
#define BITM_WUT_T4CLRI_WUFC                                  (0X0004U)      /* T4WUFC Interrupt Clear */
#define BITP_WUT_T4CLRI_WUFD                                  (3U)           /* T4WUFD Interrupt Clear */
#define BITL_WUT_T4CLRI_WUFD                                  (1U)           /* T4WUFD Interrupt Clear */
#define BITM_WUT_T4CLRI_WUFD                                  (0X0008U)      /* T4WUFD Interrupt Clear */
#define BITP_WUT_T4CLRI_ROLL                                  (4U)           /* Rollover Interrupt Clear */
#define BITL_WUT_T4CLRI_ROLL                                  (1U)           /* Rollover Interrupt Clear */
#define BITM_WUT_T4CLRI_ROLL                                  (0X0010U)      /* Rollover Interrupt Clear */

/* ----------------------------------------------------------------------------------------------------
          T4WUFA0                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_WUT_T4WUFA0_T4WUFAL                              (0U)           /* Wakeup Field a Low */
#define BITL_WUT_T4WUFA0_T4WUFAL                              (16U)          /* Wakeup Field a Low */
#define BITM_WUT_T4WUFA0_T4WUFAL                              (0XFFFFU)      /* Wakeup Field a Low */

/* ----------------------------------------------------------------------------------------------------
          T4WUFA1                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_WUT_T4WUFA1_T4WUFAH                              (0U)           /* Wakeup Field a High */
#define BITL_WUT_T4WUFA1_T4WUFAH                              (16U)          /* Wakeup Field a High */
#define BITM_WUT_T4WUFA1_T4WUFAH                              (0XFFFFU)      /* Wakeup Field a High */

#endif  /* end ifndef WUT_ADDR_RDEF_H_ */


#ifndef WDT_ADDR_RDEF_H_
#define WDT_ADDR_RDEF_H_    /* WDT: Watchdog Timer Register Map */

/* ====================================================================================================
        WDT Module Instances Address and Mask Definitions
   ==================================================================================================== */
#define INST_WDT                                                 (0X40004000U)    /* wdt: */

#define MASK_WDT                                                 (0X000000FFU)    /* WDT: Watchdog Timer Register Map */

/* ====================================================================================================
        WDT Module Register Address Offset Definitions
   ==================================================================================================== */
#define IDX_WDT_LD                                              (0X00U)    /* Watchdog Timer Load Value */
#define IDX_WDT_VALS                                            (0X04U)    /* Current Count Value */
#define IDX_WDT_CON                                             (0X08U)    /* Watchdog Timer Control Register */
#define IDX_WDT_CLRI                                            (0X0CU)    /* Refresh Watchdog Register */
#define IDX_WDT_STA                                             (0X18U)    /* Timer Status */
#define IDX_WDT_MINLD                                           (0X1CU)    /* Minimum Load Value */

/* ====================================================================================================
        WDT Module Register ResetValue Definitions
   ==================================================================================================== */
#define RSTVAL_WDT_LD                                              (0X1000) 
#define RSTVAL_WDT_VALS                                            (0X1000) 
#define RSTVAL_WDT_CON                                             (0X69) 
#define RSTVAL_WDT_CLRI                                            (0X0) 
#define RSTVAL_WDT_STA                                             (0X0) 
#define RSTVAL_WDT_MINLD                                           (0X800) 

/* ====================================================================================================
        WDT Module Register BitPositions, Lengths, Masks and Enumerations Definitions
   ==================================================================================================== */

/* ----------------------------------------------------------------------------------------------------
          LD                                                    Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_WDT_LD_LOAD                                      (0U)           /* WDT Load Value */
#define BITL_WDT_LD_LOAD                                      (16U)          /* WDT Load Value */
#define BITM_WDT_LD_LOAD                                      (0XFFFFU)      /* WDT Load Value */

/* ----------------------------------------------------------------------------------------------------
          VALS                                                  Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_WDT_VALS_CCOUNT                                  (0U)           /* Current WDT Count Value. */
#define BITL_WDT_VALS_CCOUNT                                  (16U)          /* Current WDT Count Value. */
#define BITM_WDT_VALS_CCOUNT                                  (0XFFFFU)      /* Current WDT Count Value. */

/* ----------------------------------------------------------------------------------------------------
          CON                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_WDT_CON_PDSTOP                                   (0U)           /* Power Down Stop Enable */
#define BITL_WDT_CON_PDSTOP                                   (1U)           /* Power Down Stop Enable */
#define BITM_WDT_CON_PDSTOP                                   (0X0001U)      /* Power Down Stop Enable */
#define BITP_WDT_CON_IRQ                                      (1U)           /* WDT Interrupt Enable */
#define BITL_WDT_CON_IRQ                                      (1U)           /* WDT Interrupt Enable */
#define BITM_WDT_CON_IRQ                                      (0X0002U)      /* WDT Interrupt Enable */
#define BITP_WDT_CON_PRE                                      (2U)           /* Prescaler. */
#define BITL_WDT_CON_PRE                                      (2U)           /* Prescaler. */
#define BITM_WDT_CON_PRE                                      (0X000CU)      /* Prescaler. */
#define BITP_WDT_CON_EN                                       (5U)           /* Timer Enable */
#define BITL_WDT_CON_EN                                       (1U)           /* Timer Enable */
#define BITM_WDT_CON_EN                                       (0X0020U)      /* Timer Enable */
#define BITP_WDT_CON_MDE                                      (6U)           /* Timer Mode Select */
#define BITL_WDT_CON_MDE                                      (1U)           /* Timer Mode Select */
#define BITM_WDT_CON_MDE                                      (0X0040U)      /* Timer Mode Select */
#define BITP_WDT_CON_CLKDIV2                                  (8U)           /* Clock Source */
#define BITL_WDT_CON_CLKDIV2                                  (1U)           /* Clock Source */
#define BITM_WDT_CON_CLKDIV2                                  (0X0100U)      /* Clock Source */
#define BITP_WDT_CON_MINLOADEN                                (9U)           /* Timer Window Control */
#define BITL_WDT_CON_MINLOADEN                                (1U)           /* Timer Window Control */
#define BITM_WDT_CON_MINLOADEN                                (0X0200U)      /* Timer Window Control */
#define BITP_WDT_CON_WDTIRQEN                                 (10U)          /* WDT Interrupt Enable */
#define BITL_WDT_CON_WDTIRQEN                                 (1U)           /* WDT Interrupt Enable */
#define BITM_WDT_CON_WDTIRQEN                                 (0X0400U)      /* WDT Interrupt Enable */

#define ENUM_WDT_CON_MDE_FREE                                 (0X0000U)      /* Free-running Mode */
#define ENUM_WDT_CON_MDE_PERIODIC                             (0X0001U)      /* Periodic Mode (default) */
#define ENUM_WDT_CON_PRE_DIV1                                 (0X0000U)      /* Source Clock/1 */
#define ENUM_WDT_CON_PRE_DIV16                                (0X0001U)      /* Source Clock/16. */
#define ENUM_WDT_CON_PRE_DIV256                               (0X0002U)      /* Source Clock/256 (default). */
#define ENUM_WDT_CON_PRE_DIV4096                              (0X0003U)      /* Source Clock/4096 */
#define ENUM_WDT_CON_IRQ_RESET                                (0X0000U)      /* Watchdog Timer Timeout Creates a Reset. */
#define ENUM_WDT_CON_IRQ_INTERRUPT                            (0X0001U)      /* Watchdog Timer Timeout Creates an Interrupt Instead of Reset. */
#define ENUM_WDT_CON_PDSTOP_CONTINUE                          (0X0000U)      /* Continue Counting When in Hibernate */
#define ENUM_WDT_CON_PDSTOP_STOPED                            (0X0001U)      /* Stop Counter When in Hibernate. */

/* ----------------------------------------------------------------------------------------------------
          CLRI                                                  Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_WDT_CLRI_CLRWDG                                  (0U)           /* Refresh Register */
#define BITL_WDT_CLRI_CLRWDG                                  (16U)          /* Refresh Register */
#define BITM_WDT_CLRI_CLRWDG                                  (0XFFFFU)      /* Refresh Register */

/* ----------------------------------------------------------------------------------------------------
          STA                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_WDT_STA_IRQ                                      (0U)           /* WDT Interrupt */
#define BITL_WDT_STA_IRQ                                      (1U)           /* WDT Interrupt */
#define BITM_WDT_STA_IRQ                                      (0X0001U)      /* WDT Interrupt */
#define BITP_WDT_STA_CLRI                                     (1U)           /* WDTCLRI Write Status */
#define BITL_WDT_STA_CLRI                                     (1U)           /* WDTCLRI Write Status */
#define BITM_WDT_STA_CLRI                                     (0X0002U)      /* WDTCLRI Write Status */
#define BITP_WDT_STA_TLD                                      (2U)           /* WDTVAL Write Status */
#define BITL_WDT_STA_TLD                                      (1U)           /* WDTVAL Write Status */
#define BITM_WDT_STA_TLD                                      (0X0004U)      /* WDTVAL Write Status */
#define BITP_WDT_STA_CON                                      (3U)           /* WDTCON Write Status */
#define BITL_WDT_STA_CON                                      (1U)           /* WDTCON Write Status */
#define BITM_WDT_STA_CON                                      (0X0008U)      /* WDTCON Write Status */
#define BITP_WDT_STA_LOCKS                                    (4U)           /* Lock Status */
#define BITL_WDT_STA_LOCKS                                    (1U)           /* Lock Status */
#define BITM_WDT_STA_LOCKS                                    (0X0010U)      /* Lock Status */
#define BITP_WDT_STA_OTPWRDONE                                (5U)           /* Reset Type Status */
#define BITL_WDT_STA_OTPWRDONE                                (1U)           /* Reset Type Status */
#define BITM_WDT_STA_OTPWRDONE                                (0X0020U)      /* Reset Type Status */
#define BITP_WDT_STA_TMINLD                                   (6U)           /* WDTMINLD Write Status */
#define BITL_WDT_STA_TMINLD                                   (1U)           /* WDTMINLD Write Status */
#define BITM_WDT_STA_TMINLD                                   (0X0040U)      /* WDTMINLD Write Status */

#define ENUM_WDT_STA_LOCKS_OPENED                             (0X0000U)      /* Timer Operation Not Locked */
#define ENUM_WDT_STA_LOCKS_LOCKED                             (0X0001U)      /* Timer Enabled and Locked */
#define ENUM_WDT_STA_IRQ_CLEARED                              (0X0000U)      /* Watchdog Timer Interrupt Not Pending */
#define ENUM_WDT_STA_IRQ_PENDING                              (0X0001U)      /* Watchdog Timer Interrupt Pending */

/* ----------------------------------------------------------------------------------------------------
          MINLD                                                 Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_WDT_MINLD_MINLOAD                                (0U)           /* WDT Min Load Value */
#define BITL_WDT_MINLD_MINLOAD                                (16U)          /* WDT Min Load Value */
#define BITM_WDT_MINLD_MINLOAD                                (0XFFFFU)      /* WDT Min Load Value */

#endif  /* end ifndef WDT_ADDR_RDEF_H_ */


#ifndef ALWAYS_ON_ADDR_RDEF_H_
#define ALWAYS_ON_ADDR_RDEF_H_    /* ALWAYS_ON: Always on clock, reset, power management and test */

/* ====================================================================================================
        ALWAYS_ON Module Instances Address and Mask Definitions
   ==================================================================================================== */
#define INST_ALLON                                               (0X40005000U)    /* allon: */

#define MASK_ALWAYS_ON                                           (0XFFFFFFFFU)    /* ALWAYS_ON: Always on clock, reset, power management and test */

/* ====================================================================================================
        ALWAYS_ON Module Register Address Offset Definitions
   ==================================================================================================== */
#define IDX_ALWAYS_ON_PWRMOD                                    (0X00000000U)    /* Power Modes */
#define IDX_ALWAYS_ON_PWRKEY                                    (0X00000004U)    /* Key Protection for PWRMOD */
#define IDX_ALWAYS_ON_RSTCFG                                    (0X00000008U)    /* Reset Configuration */
#define IDX_ALWAYS_ON_RSTKEY                                    (0X0000000CU)    /* Key Protection for RSTCFG */
#define IDX_ALWAYS_ON_EI0CFG                                    (0X00000020U)    /* External Interrupt Configuration 0 */
#define IDX_ALWAYS_ON_EI1CFG                                    (0X00000024U)    /* External Interrupt Configuration 1 */
#define IDX_ALWAYS_ON_EI2CFG                                    (0X00000028U)    /* External Interrupt Configuration 2 */
#define IDX_ALWAYS_ON_EICLR                                     (0X00000030U)    /* External Interrupt Clear */
#define IDX_ALWAYS_ON_RSTSTA                                    (0X00000040U)    /* Reset Status */

/* ====================================================================================================
        ALWAYS_ON Module Register ResetValue Definitions
   ==================================================================================================== */
#define RSTVAL_ALWAYS_ON_PWRMOD                                    (0X0) 
#define RSTVAL_ALWAYS_ON_PWRKEY                                    (0X0) 
#define RSTVAL_ALWAYS_ON_RSTCFG                                    (0X0) 
#define RSTVAL_ALWAYS_ON_RSTKEY                                    (0X0) 
#define RSTVAL_ALWAYS_ON_EI0CFG                                    (0X0) 
#define RSTVAL_ALWAYS_ON_EI1CFG                                    (0X0) 
#define RSTVAL_ALWAYS_ON_EI2CFG                                    (0X0) 
#define RSTVAL_ALWAYS_ON_EICLR                                     (0X0) 
#define RSTVAL_ALWAYS_ON_RSTSTA                                    (0X0) 

/* ====================================================================================================
        ALWAYS_ON Module Register BitPositions, Lengths, Masks and Enumerations Definitions
   ==================================================================================================== */

/* ----------------------------------------------------------------------------------------------------
          PWRMOD                                                Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ALWAYS_ON_PWRMOD_PWRMOD                          (0U)           /* Power Modes Control Bits */
#define BITL_ALWAYS_ON_PWRMOD_PWRMOD                          (2U)           /* Power Modes Control Bits */
#define BITM_ALWAYS_ON_PWRMOD_PWRMOD                          (0X0003U)      /* Power Modes Control Bits */
#define BITP_ALWAYS_ON_PWRMOD_WICENACK                        (3U)           /* WIC Acknowledgment for SLEEPDEEP */
#define BITL_ALWAYS_ON_PWRMOD_WICENACK                        (1U)           /* WIC Acknowledgment for SLEEPDEEP */
#define BITM_ALWAYS_ON_PWRMOD_WICENACK                        (0X0008U)      /* WIC Acknowledgment for SLEEPDEEP */

#define ENUM_ALWAYS_ON_PWRMOD_WICENACK_NOACK                  (0X0000U)      /* No description provided */
#define ENUM_ALWAYS_ON_PWRMOD_WICENACK_ACK                    (0X0001U)      /* No description provided */
#define ENUM_ALWAYS_ON_PWRMOD_PWRMOD_FULLACTIVE               (0X0000U)      /* Active Mode */
#define ENUM_ALWAYS_ON_PWRMOD_PWRMOD_CORESLEEP                (0X0001U)      /* CORE_SLEEP Mode */
#define ENUM_ALWAYS_ON_PWRMOD_PWRMOD_SYSSLEEP                 (0X0002U)      /* SYS_SLEEP Mode */
#define ENUM_ALWAYS_ON_PWRMOD_PWRMOD_HIBERNATE                (0X0003U)      /* Hibernate Mode */

/* ----------------------------------------------------------------------------------------------------
          PWRKEY                                                Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ALWAYS_ON_PWRKEY_PWRKEY                          (0U)           /* Power Control Key Register */
#define BITL_ALWAYS_ON_PWRKEY_PWRKEY                          (16U)          /* Power Control Key Register */
#define BITM_ALWAYS_ON_PWRKEY_PWRKEY                          (0XFFFFU)      /* Power Control Key Register */

/* ----------------------------------------------------------------------------------------------------
          RSTCFG                                                Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ALWAYS_ON_RSTCFG_GPIO_PLA_RETAIN                 (0U)           /* GPIO/PLA Retain Their Status After WDT and Software Reset */
#define BITL_ALWAYS_ON_RSTCFG_GPIO_PLA_RETAIN                 (1U)           /* GPIO/PLA Retain Their Status After WDT and Software Reset */
#define BITM_ALWAYS_ON_RSTCFG_GPIO_PLA_RETAIN                 (0X0001U)      /* GPIO/PLA Retain Their Status After WDT and Software Reset */
#define BITP_ALWAYS_ON_RSTCFG_LV_RETAIN                       (2U)           /* LV Die Retain Status After Watchdog or Software Reset */
#define BITL_ALWAYS_ON_RSTCFG_LV_RETAIN                       (1U)           /* LV Die Retain Status After Watchdog or Software Reset */
#define BITM_ALWAYS_ON_RSTCFG_LV_RETAIN                       (0X0004U)      /* LV Die Retain Status After Watchdog or Software Reset */
#define BITP_ALWAYS_ON_RSTCFG_ANA_RETAIN                      (3U)           /* Analog Block Retain Status After Watchdog or Software Reset */
#define BITL_ALWAYS_ON_RSTCFG_ANA_RETAIN                      (1U)           /* Analog Block Retain Status After Watchdog or Software Reset */
#define BITM_ALWAYS_ON_RSTCFG_ANA_RETAIN                      (0X0008U)      /* Analog Block Retain Status After Watchdog or Software Reset */

#define ENUM_ALWAYS_ON_RSTCFG_ANA_RETAIN_DIS                  (0X0000U)      /* GPIO/PLA Retain Status After Watchdog or Software Reset. */
#define ENUM_ALWAYS_ON_RSTCFG_ANA_RETAIN_EN                   (0X0001U)      /* GPIO/PLA not Retain Status After Watchdog or Software Reset */
#define ENUM_ALWAYS_ON_RSTCFG_LV_RETAIN_DIS                   (0X0000U)      /* LV Die Retain Status After Watchdog or Software Reset. */
#define ENUM_ALWAYS_ON_RSTCFG_LV_RETAIN_EN                    (0X0001U)      /* LV Die not Retain Status After Watchdog or Software Reset */
#define ENUM_ALWAYS_ON_RSTCFG_GPIO_PLA_RETAIN_DIS             (0X0000U)      /* GPIO/PLA  Retain Status After Watchdog or Software Reset. */
#define ENUM_ALWAYS_ON_RSTCFG_GPIO_PLA_RETAIN_EN              (0X0001U)      /* GPIO/PLA not Retain Status After Watchdog or Software Reset */

/* ----------------------------------------------------------------------------------------------------
          RSTKEY                                                Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ALWAYS_ON_RSTKEY_RSTKEY                          (0U)           /* Reset Configuration Key Register */
#define BITL_ALWAYS_ON_RSTKEY_RSTKEY                          (16U)          /* Reset Configuration Key Register */
#define BITM_ALWAYS_ON_RSTKEY_RSTKEY                          (0XFFFFU)      /* Reset Configuration Key Register */

#define ENUM_ALWAYS_ON_RSTKEY_RSTKEY_RSTKEY1                  (0X2009U)      /* First Key to Write Reset Configuration */
#define ENUM_ALWAYS_ON_RSTKEY_RSTKEY_RSTKEY2                  (0X0426U)      /* Second Key to Write Reset Configuration */

/* ----------------------------------------------------------------------------------------------------
          EI0CFG                                                Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ALWAYS_ON_EI0CFG_IRQ0MDE                         (0U)           /* External Interrupt 0 Mode Registers */
#define BITL_ALWAYS_ON_EI0CFG_IRQ0MDE                         (3U)           /* External Interrupt 0 Mode Registers */
#define BITM_ALWAYS_ON_EI0CFG_IRQ0MDE                         (0X0007U)      /* External Interrupt 0 Mode Registers */
#define BITP_ALWAYS_ON_EI0CFG_IRQ0EN                          (3U)           /* External Interrupt 0 Enable Bit */
#define BITL_ALWAYS_ON_EI0CFG_IRQ0EN                          (1U)           /* External Interrupt 0 Enable Bit */
#define BITM_ALWAYS_ON_EI0CFG_IRQ0EN                          (0X0008U)      /* External Interrupt 0 Enable Bit */
#define BITP_ALWAYS_ON_EI0CFG_IRQ1MDE                         (4U)           /* External Interrupt 1 Mode Registers */
#define BITL_ALWAYS_ON_EI0CFG_IRQ1MDE                         (3U)           /* External Interrupt 1 Mode Registers */
#define BITM_ALWAYS_ON_EI0CFG_IRQ1MDE                         (0X0070U)      /* External Interrupt 1 Mode Registers */
#define BITP_ALWAYS_ON_EI0CFG_IRQ1EN                          (7U)           /* External Interrupt 1 Enable Bit */
#define BITL_ALWAYS_ON_EI0CFG_IRQ1EN                          (1U)           /* External Interrupt 1 Enable Bit */
#define BITM_ALWAYS_ON_EI0CFG_IRQ1EN                          (0X0080U)      /* External Interrupt 1 Enable Bit */
#define BITP_ALWAYS_ON_EI0CFG_IRQ2MDE                         (8U)           /* External Interrupt 2 Mode Registers */
#define BITL_ALWAYS_ON_EI0CFG_IRQ2MDE                         (3U)           /* External Interrupt 2 Mode Registers */
#define BITM_ALWAYS_ON_EI0CFG_IRQ2MDE                         (0X0700U)      /* External Interrupt 2 Mode Registers */
#define BITP_ALWAYS_ON_EI0CFG_IRQ2EN                          (11U)          /* External Interrupt 2 Enable Bit */
#define BITL_ALWAYS_ON_EI0CFG_IRQ2EN                          (1U)           /* External Interrupt 2 Enable Bit */
#define BITM_ALWAYS_ON_EI0CFG_IRQ2EN                          (0X0800U)      /* External Interrupt 2 Enable Bit */
#define BITP_ALWAYS_ON_EI0CFG_IRQ3MDE                         (12U)          /* External Interrupt 3 Mode Registers */
#define BITL_ALWAYS_ON_EI0CFG_IRQ3MDE                         (3U)           /* External Interrupt 3 Mode Registers */
#define BITM_ALWAYS_ON_EI0CFG_IRQ3MDE                         (0X7000U)      /* External Interrupt 3 Mode Registers */
#define BITP_ALWAYS_ON_EI0CFG_IRQ3EN                          (15U)          /* External Interrupt 3 Enable Bit */
#define BITL_ALWAYS_ON_EI0CFG_IRQ3EN                          (1U)           /* External Interrupt 3 Enable Bit */
#define BITM_ALWAYS_ON_EI0CFG_IRQ3EN                          (0X8000U)      /* External Interrupt 3 Enable Bit */

#define ENUM_ALWAYS_ON_EI0CFG_IRQ3EN_DIS                      (0X0000U)      /* External Interrupt 3 Disabled */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ3EN_EN                       (0X0001U)      /* External Interrupt 3 Enabled */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ3MDE_RISE                    (0X0000U)      /* Rising Edge */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ3MDE_FALL                    (0X0001U)      /* Falling Edge */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ3MDE_RISEORFALL              (0X0002U)      /* Rising or Falling Edge */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ3MDE_HIGHLEVEL               (0X0003U)      /* High Level */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ3MDE_LOWLEVEL                (0X0004U)      /* Low Level */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ3MDE_FALLAUX                 (0X0005U)      /* Falling Edge (same as 001) */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ3MDE_RISEORFALLAUX           (0X0006U)      /* Rising or Falling Edge (same as 010) */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ3MDE_HIGHLEVELAUX            (0X0007U)      /* High Level (same as 011) */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ2EN_DIS                      (0X0000U)      /* External Interrupt 2 Disabled */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ2EN_EN                       (0X0001U)      /* External Interrupt 2 Enabled */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ2MDE_RISE                    (0X0000U)      /* Rising Edge */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ2MDE_FALL                    (0X0001U)      /* Falling Edge */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ2MDE_RISEORFALL              (0X0002U)      /* Rising or Falling Edge */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ2MDE_HIGHLEVEL               (0X0003U)      /* High Level */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ2MDE_LOWLEVEL                (0X0004U)      /* Low Level */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ2MDE_FALLAUX                 (0X0005U)      /* Falling Edge (same as 001) */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ2MDE_RISEORFALLAUX           (0X0006U)      /* Rising or Falling Edge (same as 010) */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ2MDE_HIGHLEVELAUX            (0X0007U)      /* High Level (same as 011) */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ1EN_DIS                      (0X0000U)      /* External Interrupt 0 Disabled */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ1EN_EN                       (0X0001U)      /* External Interrupt 0 Enabled */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ1MDE_RISE                    (0X0000U)      /* Rising Edge */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ1MDE_FALL                    (0X0001U)      /* Falling Edge */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ1MDE_RISEORFALL              (0X0002U)      /* Rising or Falling Edge */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ1MDE_HIGHLEVEL               (0X0003U)      /* High Level */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ1MDE_LOWLEVEL                (0X0004U)      /* Low Level */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ1MDE_FALLAUX                 (0X0005U)      /* Falling Edge (same as 001) */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ1MDE_RISEORFALLAUX           (0X0006U)      /* Rising or Falling Edge (same as 010) */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ1MDE_HIGHLEVELAUX            (0X0007U)      /* High Level (same as 011) */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ0EN_DIS                      (0X0000U)      /* External Interrupt 0 Disabled */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ0EN_EN                       (0X0001U)      /* External Interrupt 0 Enabled */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ0MDE_RISE                    (0X0000U)      /* Rising Edge */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ0MDE_FALL                    (0X0001U)      /* Falling Edge */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ0MDE_RISEORFALL              (0X0002U)      /* Rising or Falling Edge */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ0MDE_HIGHLEVEL               (0X0003U)      /* High Level */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ0MDE_LOWLEVEL                (0X0004U)      /* Low Level */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ0MDE_FALLAUX                 (0X0005U)      /* Falling Edge (same as 001) */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ0MDE_RISEORFALLAUX           (0X0006U)      /* Rising or Falling Edge (same as 010) */
#define ENUM_ALWAYS_ON_EI0CFG_IRQ0MDE_HIGHLEVELAUX            (0X0007U)      /* High Level (same as 011) */

/* ----------------------------------------------------------------------------------------------------
          EI1CFG                                                Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ALWAYS_ON_EI1CFG_IRQ4MDE                         (0U)           /* External Interrupt 4 Mode Registers */
#define BITL_ALWAYS_ON_EI1CFG_IRQ4MDE                         (3U)           /* External Interrupt 4 Mode Registers */
#define BITM_ALWAYS_ON_EI1CFG_IRQ4MDE                         (0X0007U)      /* External Interrupt 4 Mode Registers */
#define BITP_ALWAYS_ON_EI1CFG_IRQ4EN                          (3U)           /* External Interrupt 4 Enable Bit */
#define BITL_ALWAYS_ON_EI1CFG_IRQ4EN                          (1U)           /* External Interrupt 4 Enable Bit */
#define BITM_ALWAYS_ON_EI1CFG_IRQ4EN                          (0X0008U)      /* External Interrupt 4 Enable Bit */
#define BITP_ALWAYS_ON_EI1CFG_IRQ5MDE                         (4U)           /* External Interrupt 5 Mode Registers */
#define BITL_ALWAYS_ON_EI1CFG_IRQ5MDE                         (3U)           /* External Interrupt 5 Mode Registers */
#define BITM_ALWAYS_ON_EI1CFG_IRQ5MDE                         (0X0070U)      /* External Interrupt 5 Mode Registers */
#define BITP_ALWAYS_ON_EI1CFG_IRQ5EN                          (7U)           /* External Interrupt 5 Enable Bit */
#define BITL_ALWAYS_ON_EI1CFG_IRQ5EN                          (1U)           /* External Interrupt 5 Enable Bit */
#define BITM_ALWAYS_ON_EI1CFG_IRQ5EN                          (0X0080U)      /* External Interrupt 5 Enable Bit */
#define BITP_ALWAYS_ON_EI1CFG_IRQ6MDE                         (8U)           /* External Interrupt 6 Mode Registers */
#define BITL_ALWAYS_ON_EI1CFG_IRQ6MDE                         (3U)           /* External Interrupt 6 Mode Registers */
#define BITM_ALWAYS_ON_EI1CFG_IRQ6MDE                         (0X0700U)      /* External Interrupt 6 Mode Registers */
#define BITP_ALWAYS_ON_EI1CFG_IRQ6EN                          (11U)          /* External Interrupt 6 Enable Bit */
#define BITL_ALWAYS_ON_EI1CFG_IRQ6EN                          (1U)           /* External Interrupt 6 Enable Bit */
#define BITM_ALWAYS_ON_EI1CFG_IRQ6EN                          (0X0800U)      /* External Interrupt 6 Enable Bit */
#define BITP_ALWAYS_ON_EI1CFG_IRQ7MDE                         (12U)          /* External Interrupt 7 Mode Registers */
#define BITL_ALWAYS_ON_EI1CFG_IRQ7MDE                         (3U)           /* External Interrupt 7 Mode Registers */
#define BITM_ALWAYS_ON_EI1CFG_IRQ7MDE                         (0X7000U)      /* External Interrupt 7 Mode Registers */
#define BITP_ALWAYS_ON_EI1CFG_IRQ7EN                          (15U)          /* External Interrupt 7 Enable Bit */
#define BITL_ALWAYS_ON_EI1CFG_IRQ7EN                          (1U)           /* External Interrupt 7 Enable Bit */
#define BITM_ALWAYS_ON_EI1CFG_IRQ7EN                          (0X8000U)      /* External Interrupt 7 Enable Bit */

#define ENUM_ALWAYS_ON_EI1CFG_IRQ7EN_DIS                      (0X0000U)      /* External Interrupt 7 Disabled */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ7EN_EN                       (0X0001U)      /* External Interrupt 7 Enabled */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ7MDE_RISE                    (0X0000U)      /* Rising Edge */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ7MDE_FALL                    (0X0001U)      /* Falling Edge */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ7MDE_RISEORFALL              (0X0002U)      /* Rising or Falling Edge */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ7MDE_HIGHLEVEL               (0X0003U)      /* High Level */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ7MDE_LOWLEVEL                (0X0004U)      /* Low Level */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ7MDE_FALLAUX                 (0X0005U)      /* Falling Edge (same as 001) */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ7MDE_RISEORFALLAUX           (0X0006U)      /* Rising or Falling Edge (same as 010) */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ7MDE_HIGHLEVELAUX            (0X0007U)      /* High Level (same as 011) */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ6EN_DIS                      (0X0000U)      /* External Interrupt 6 Disabled */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ6EN_EN                       (0X0001U)      /* External Interrupt 6 Enabled */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ6MDE_RISE                    (0X0000U)      /* Rising Edge */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ6MDE_FALL                    (0X0001U)      /* Falling Edge */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ6MDE_RISEORFALL              (0X0002U)      /* Rising or Falling Edge */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ6MDE_HIGHLEVEL               (0X0003U)      /* High Level */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ6MDE_LOWLEVEL                (0X0004U)      /* Low Level */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ6MDE_FALLAUX                 (0X0005U)      /* Falling Edge (same as 001) */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ6MDE_RISEORFALLAUX           (0X0006U)      /* Rising or Falling Edge (same as 010) */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ6MDE_HIGHLEVELAUX            (0X0007U)      /* High Level (same as 011) */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ5EN_DIS                      (0X0000U)      /* External Interrupt 5 Disabled */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ5EN_EN                       (0X0001U)      /* External Interrupt 5 Enabled */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ5MDE_RISING                  (0X0000U)      /* Rising Edge */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ5MDE_FALLING                 (0X0001U)      /* Falling Edge */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ5MDE_RISEORFALL              (0X0002U)      /* Rising or Falling Edge */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ5MDE_HIGHLEVEL               (0X0003U)      /* High Level */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ5MDE_LOWLEVEL                (0X0004U)      /* Low Level */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ5MDE_FALLAUX                 (0X0005U)      /* Falling Edge (same as 001) */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ5MDE_RISEORFALLAUX           (0X0006U)      /* Rising or Falling Edge (same as 010) */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ5MDE_HIGHLEVELAUX            (0X0007U)      /* High Level (same as 011) */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ4EN_DIS                      (0X0000U)      /* External Interrupt 4 Disabled */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ4EN_EN                       (0X0001U)      /* External Interrupt 4 Enabled */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ4MDE_RISE                    (0X0000U)      /* Rising Edge */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ4MDE_FALL                    (0X0001U)      /* Falling Edge */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ4MDE_RISEORFALL              (0X0002U)      /* Rising or Falling Edge */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ4MDE_HIGHLEVEL               (0X0003U)      /* High Level */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ4MDE_LOWLEVEL                (0X0004U)      /* Low Level */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ4MDE_FALLAUX                 (0X0005U)      /* Falling Edge (same as 001) */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ4MDE_RISEORFALLAUX           (0X0006U)      /* Rising or Falling Edge (same as 010) */
#define ENUM_ALWAYS_ON_EI1CFG_IRQ4MDE_HIGHLEVELAUX            (0X0007U)      /* High Level (same as 011) */

/* ----------------------------------------------------------------------------------------------------
          EI2CFG                                                Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ALWAYS_ON_EI2CFG_IRQ8MDE                         (0U)           /* External Interrupt 8 Mode Registers */
#define BITL_ALWAYS_ON_EI2CFG_IRQ8MDE                         (3U)           /* External Interrupt 8 Mode Registers */
#define BITM_ALWAYS_ON_EI2CFG_IRQ8MDE                         (0X0007U)      /* External Interrupt 8 Mode Registers */
#define BITP_ALWAYS_ON_EI2CFG_IRQ8EN                          (3U)           /* External Interrupt 8 Enable Bit */
#define BITL_ALWAYS_ON_EI2CFG_IRQ8EN                          (1U)           /* External Interrupt 8 Enable Bit */
#define BITM_ALWAYS_ON_EI2CFG_IRQ8EN                          (0X0008U)      /* External Interrupt 8 Enable Bit */
#define BITP_ALWAYS_ON_EI2CFG_IRQ9MDE                         (4U)           /* External Interrupt 8 Mode Registers */
#define BITL_ALWAYS_ON_EI2CFG_IRQ9MDE                         (3U)           /* External Interrupt 8 Mode Registers */
#define BITM_ALWAYS_ON_EI2CFG_IRQ9MDE                         (0X0070U)      /* External Interrupt 8 Mode Registers */
#define BITP_ALWAYS_ON_EI2CFG_IRQ9EN                          (7U)           /* External Interrupt 8 Enable Bit */
#define BITL_ALWAYS_ON_EI2CFG_IRQ9EN                          (1U)           /* External Interrupt 8 Enable Bit */
#define BITM_ALWAYS_ON_EI2CFG_IRQ9EN                          (0X0080U)      /* External Interrupt 8 Enable Bit */

#define ENUM_ALWAYS_ON_EI2CFG_IRQ9EN_DIS                      (0X0000U)      /* External Interrupt 8 Disabled */
#define ENUM_ALWAYS_ON_EI2CFG_IRQ9EN_EN                       (0X0001U)      /* External Interrupt 8 Enabled */
#define ENUM_ALWAYS_ON_EI2CFG_IRQ9MDE_RISE                    (0X0000U)      /* Rising Edge */
#define ENUM_ALWAYS_ON_EI2CFG_IRQ9MDE_FALL                    (0X0001U)      /* Falling Edge */
#define ENUM_ALWAYS_ON_EI2CFG_IRQ9MDE_RISEORFALL              (0X0002U)      /* Rising or Falling Edge */
#define ENUM_ALWAYS_ON_EI2CFG_IRQ9MDE_HIGHLEVEL               (0X0003U)      /* High Level */
#define ENUM_ALWAYS_ON_EI2CFG_IRQ9MDE_LOWLEVEL                (0X0004U)      /* Low Level */
#define ENUM_ALWAYS_ON_EI2CFG_IRQ9MDE_FALLAUX                 (0X0005U)      /* Falling Edge (same as 001) */
#define ENUM_ALWAYS_ON_EI2CFG_IRQ9MDE_RISEORFALLAUX           (0X0006U)      /* Rising or Falling Edge (same as 010) */
#define ENUM_ALWAYS_ON_EI2CFG_IRQ9MDE_HIGHLEVELAUX            (0X0007U)      /* High Level (same as 011) */
#define ENUM_ALWAYS_ON_EI2CFG_IRQ8EN_DIS                      (0X0000U)      /* External Interrupt 8 Disabled */
#define ENUM_ALWAYS_ON_EI2CFG_IRQ8EN_EN                       (0X0001U)      /* External Interrupt 8 Enabled */
#define ENUM_ALWAYS_ON_EI2CFG_IRQ8MDE_RISE                    (0X0000U)      /* Rising Edge */
#define ENUM_ALWAYS_ON_EI2CFG_IRQ8MDE_FALL                    (0X0001U)      /* Falling Edge */
#define ENUM_ALWAYS_ON_EI2CFG_IRQ8MDE_RISEORFALL              (0X0002U)      /* Rising or Falling Edge */
#define ENUM_ALWAYS_ON_EI2CFG_IRQ8MDE_HIGHLEVEL               (0X0003U)      /* High Level */
#define ENUM_ALWAYS_ON_EI2CFG_IRQ8MDE_LOWLEVEL                (0X0004U)      /* Low Level */
#define ENUM_ALWAYS_ON_EI2CFG_IRQ8MDE_FALLAUX                 (0X0005U)      /* Falling Edge (same as 001) */
#define ENUM_ALWAYS_ON_EI2CFG_IRQ8MDE_RISEORFALLAUX           (0X0006U)      /* Rising or Falling Edge (same as 010) */
#define ENUM_ALWAYS_ON_EI2CFG_IRQ8MDE_HIGHLEVELAUX            (0X0007U)      /* High Level (same as 011) */

/* ----------------------------------------------------------------------------------------------------
          EICLR                                                 Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ALWAYS_ON_EICLR_IRQ0                             (0U)           /* External Interrupt 0 */
#define BITL_ALWAYS_ON_EICLR_IRQ0                             (1U)           /* External Interrupt 0 */
#define BITM_ALWAYS_ON_EICLR_IRQ0                             (0X0001U)      /* External Interrupt 0 */
#define BITP_ALWAYS_ON_EICLR_IRQ1                             (1U)           /* External Interrupt 1 */
#define BITL_ALWAYS_ON_EICLR_IRQ1                             (1U)           /* External Interrupt 1 */
#define BITM_ALWAYS_ON_EICLR_IRQ1                             (0X0002U)      /* External Interrupt 1 */
#define BITP_ALWAYS_ON_EICLR_IRQ2                             (2U)           /* External Interrupt 2 */
#define BITL_ALWAYS_ON_EICLR_IRQ2                             (1U)           /* External Interrupt 2 */
#define BITM_ALWAYS_ON_EICLR_IRQ2                             (0X0004U)      /* External Interrupt 2 */
#define BITP_ALWAYS_ON_EICLR_IRQ3                             (3U)           /* External Interrupt 3 */
#define BITL_ALWAYS_ON_EICLR_IRQ3                             (1U)           /* External Interrupt 3 */
#define BITM_ALWAYS_ON_EICLR_IRQ3                             (0X0008U)      /* External Interrupt 3 */
#define BITP_ALWAYS_ON_EICLR_IRQ4                             (4U)           /* External Interrupt 4 */
#define BITL_ALWAYS_ON_EICLR_IRQ4                             (1U)           /* External Interrupt 4 */
#define BITM_ALWAYS_ON_EICLR_IRQ4                             (0X0010U)      /* External Interrupt 4 */
#define BITP_ALWAYS_ON_EICLR_IRQ5                             (5U)           /* External Interrupt 5 */
#define BITL_ALWAYS_ON_EICLR_IRQ5                             (1U)           /* External Interrupt 5 */
#define BITM_ALWAYS_ON_EICLR_IRQ5                             (0X0020U)      /* External Interrupt 5 */
#define BITP_ALWAYS_ON_EICLR_IRQ6                             (6U)           /* External Interrupt 6 */
#define BITL_ALWAYS_ON_EICLR_IRQ6                             (1U)           /* External Interrupt 6 */
#define BITM_ALWAYS_ON_EICLR_IRQ6                             (0X0040U)      /* External Interrupt 6 */
#define BITP_ALWAYS_ON_EICLR_IRQ7                             (7U)           /* External Interrupt 7 */
#define BITL_ALWAYS_ON_EICLR_IRQ7                             (1U)           /* External Interrupt 7 */
#define BITM_ALWAYS_ON_EICLR_IRQ7                             (0X0080U)      /* External Interrupt 7 */
#define BITP_ALWAYS_ON_EICLR_IRQ8                             (8U)           /* External Interrupt 8 */
#define BITL_ALWAYS_ON_EICLR_IRQ8                             (1U)           /* External Interrupt 8 */
#define BITM_ALWAYS_ON_EICLR_IRQ8                             (0X0100U)      /* External Interrupt 8 */
#define BITP_ALWAYS_ON_EICLR_IRQ9                             (9U)           /* External Interrupt 9 */
#define BITL_ALWAYS_ON_EICLR_IRQ9                             (1U)           /* External Interrupt 9 */
#define BITM_ALWAYS_ON_EICLR_IRQ9                             (0X0200U)      /* External Interrupt 9 */

#define ENUM_ALWAYS_ON_EICLR_IRQ9_NOTCLR                      (0X0000U)      /* No description provided */
#define ENUM_ALWAYS_ON_EICLR_IRQ9_CLR                         (0X0001U)      /* No description provided */
#define ENUM_ALWAYS_ON_EICLR_IRQ8_NOTCLR                      (0X0000U)      /* No description provided */
#define ENUM_ALWAYS_ON_EICLR_IRQ8_CLR                         (0X0001U)      /* No description provided */
#define ENUM_ALWAYS_ON_EICLR_IRQ7_NOTCLR                      (0X0000U)      /* No description provided */
#define ENUM_ALWAYS_ON_EICLR_IRQ7_CLR                         (0X0001U)      /* No description provided */
#define ENUM_ALWAYS_ON_EICLR_IRQ6_NOTCLR                      (0X0000U)      /* No description provided */
#define ENUM_ALWAYS_ON_EICLR_IRQ6_CLR                         (0X0001U)      /* No description provided */
#define ENUM_ALWAYS_ON_EICLR_IRQ5_NOTCLR                      (0X0000U)      /* No description provided */
#define ENUM_ALWAYS_ON_EICLR_IRQ5_CLR                         (0X0001U)      /* No description provided */
#define ENUM_ALWAYS_ON_EICLR_IRQ4_NOTCLR                      (0X0000U)      /* No description provided */
#define ENUM_ALWAYS_ON_EICLR_IRQ4_CLR                         (0X0001U)      /* No description provided */
#define ENUM_ALWAYS_ON_EICLR_IRQ3_NOTCLR                      (0X0000U)      /* No description provided */
#define ENUM_ALWAYS_ON_EICLR_IRQ3_CLR                         (0X0001U)      /* No description provided */
#define ENUM_ALWAYS_ON_EICLR_IRQ2_NOTCLR                      (0X0000U)      /* No description provided */
#define ENUM_ALWAYS_ON_EICLR_IRQ2_CLR                         (0X0001U)      /* No description provided */
#define ENUM_ALWAYS_ON_EICLR_IRQ1_NOTCLR                      (0X0000U)      /* No description provided */
#define ENUM_ALWAYS_ON_EICLR_IRQ1_CLR                         (0X0001U)      /* No description provided */
#define ENUM_ALWAYS_ON_EICLR_IRQ0_NOTCLR                      (0X0000U)      /* No description provided */
#define ENUM_ALWAYS_ON_EICLR_IRQ0_CLR                         (0X0001U)      /* No description provided */

/* ----------------------------------------------------------------------------------------------------
          RSTSTA                                                Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ALWAYS_ON_RSTSTA_POR                             (0U)           /* Power-on Reset */
#define BITL_ALWAYS_ON_RSTSTA_POR                             (1U)           /* Power-on Reset */
#define BITM_ALWAYS_ON_RSTSTA_POR                             (0X0001U)      /* Power-on Reset */
#define BITP_ALWAYS_ON_RSTSTA_EXTRST                          (1U)           /* External Reset */
#define BITL_ALWAYS_ON_RSTSTA_EXTRST                          (1U)           /* External Reset */
#define BITM_ALWAYS_ON_RSTSTA_EXTRST                          (0X0002U)      /* External Reset */
#define BITP_ALWAYS_ON_RSTSTA_WDRST                           (2U)           /* Watchdog Timeout */
#define BITL_ALWAYS_ON_RSTSTA_WDRST                           (1U)           /* Watchdog Timeout */
#define BITM_ALWAYS_ON_RSTSTA_WDRST                           (0X0004U)      /* Watchdog Timeout */
#define BITP_ALWAYS_ON_RSTSTA_SWRST                           (3U)           /* Software Reset */
#define BITL_ALWAYS_ON_RSTSTA_SWRST                           (1U)           /* Software Reset */
#define BITM_ALWAYS_ON_RSTSTA_SWRST                           (0X0008U)      /* Software Reset */

#endif  /* end ifndef ALWAYS_ON_ADDR_RDEF_H_ */


#ifndef PLA_ADDR_RDEF_H_
#define PLA_ADDR_RDEF_H_    /* PLA: PLA Register Map */

/* ====================================================================================================
        PLA Module Instances Address and Mask Definitions
   ==================================================================================================== */
#define INST_PLA                                                 (0X40006000U)    /* pla: */

#define MASK_PLA                                                 (0XFFFFFFFFU)    /* PLA: PLA Register Map */

/* ====================================================================================================
        PLA Module Register Address Offset Definitions
   ==================================================================================================== */
#define IDX_PLA_PLA_ELEM0                                       (0X00000000U)    /* ELEMx Configuration Register */
#define IDX_PLA_PLA_ELEM1                                       (0X00000004U)    /* ELEMx Configuration Register */
#define IDX_PLA_PLA_ELEM2                                       (0X00000008U)    /* ELEMx Configuration Register */
#define IDX_PLA_PLA_ELEM3                                       (0X0000000CU)    /* ELEMx Configuration Register */
#define IDX_PLA_PLA_ELEM4                                       (0X00000010U)    /* ELEMx Configuration Register */
#define IDX_PLA_PLA_ELEM5                                       (0X00000014U)    /* ELEMx Configuration Register */
#define IDX_PLA_PLA_ELEM6                                       (0X00000018U)    /* ELEMx Configuration Register */
#define IDX_PLA_PLA_ELEM7                                       (0X0000001CU)    /* ELEMx Configuration Register */
#define IDX_PLA_PLA_ELEM8                                       (0X00000020U)    /* ELEMx Configuration Register */
#define IDX_PLA_PLA_ELEM9                                       (0X00000024U)    /* ELEMx Configuration Register */
#define IDX_PLA_PLA_ELEM10                                      (0X00000028U)    /* ELEMx Configuration Register */
#define IDX_PLA_PLA_ELEM11                                      (0X0000002CU)    /* ELEMx Configuration Register */
#define IDX_PLA_PLA_ELEM12                                      (0X00000030U)    /* ELEMx Configuration Register */
#define IDX_PLA_PLA_ELEM13                                      (0X00000034U)    /* ELEMx Configuration Register */
#define IDX_PLA_PLA_ELEM14                                      (0X00000038U)    /* ELEMx Configuration Register */
#define IDX_PLA_PLA_ELEM15                                      (0X0000003CU)    /* ELEMx Configuration Register */
#define IDX_PLA_PLA_ELEM16                                      (0X00000040U)    /* ELEMx Configuration Register */
#define IDX_PLA_PLA_ELEM17                                      (0X00000044U)    /* ELEMx Configuration Register */
#define IDX_PLA_PLA_ELEM18                                      (0X00000048U)    /* ELEMx Configuration Register */
#define IDX_PLA_PLA_ELEM19                                      (0X0000004CU)    /* ELEMx Configuration Register */
#define IDX_PLA_PLA_ELEM20                                      (0X00000050U)    /* ELEMx Configuration Register */
#define IDX_PLA_PLA_ELEM21                                      (0X00000054U)    /* ELEMx Configuration Register */
#define IDX_PLA_PLA_ELEM22                                      (0X00000058U)    /* ELEMx Configuration Register */
#define IDX_PLA_PLA_ELEM23                                      (0X0000005CU)    /* ELEMx Configuration Register */
#define IDX_PLA_PLA_ELEM24                                      (0X00000060U)    /* ELEMx Configuration Register */
#define IDX_PLA_PLA_ELEM25                                      (0X00000064U)    /* ELEMx Configuration Register */
#define IDX_PLA_PLA_ELEM26                                      (0X00000068U)    /* ELEMx Configuration Register */
#define IDX_PLA_PLA_ELEM27                                      (0X0000006CU)    /* ELEMx Configuration Register */
#define IDX_PLA_PLA_ELEM28                                      (0X00000070U)    /* ELEMx Configuration Register */
#define IDX_PLA_PLA_ELEM29                                      (0X00000074U)    /* ELEMx Configuration Register */
#define IDX_PLA_PLA_ELEM30                                      (0X00000078U)    /* ELEMx Configuration Register */
#define IDX_PLA_PLA_ELEM31                                      (0X0000007CU)    /* ELEMx Configuration Register */
#define IDX_PLA_PLA_CLK                                         (0X00000080U)    /* PLA Clock Select */
#define IDX_PLA_PLA_IRQ0                                        (0X00000084U)    /* Interrupt Register for Block 0 */
#define IDX_PLA_PLA_IRQ1                                        (0X00000088U)    /* Interrupt Register for Block1 */
#define IDX_PLA_PLA_ADC                                         (0X0000008CU)    /* ADC Configuration Register */
#define IDX_PLA_PLA_DIN0                                        (0X00000090U)    /* AMBA Bus Data Input for Block 0 */
#define IDX_PLA_PLA_DOUT0                                       (0X00000098U)    /* AMBA Bus Data Output for Block 0 */
#define IDX_PLA_PLA_DOUT1                                       (0X0000009CU)    /* AMBA Bus Data Output for Block1 */
#define IDX_PLA_PLA_LCK                                         (0X000000A0U)    /* Write Lock Register. */
#define IDX_PLA_PLA_IRQTYPE                                     (0X000000A4U)    /* PLA Interrupt Request and DMA Request Type */

/* ====================================================================================================
        PLA Module Register ResetValue Definitions
   ==================================================================================================== */
#define RSTVAL_PLA_PLA_ELEM0                                       (0X0) 
#define RSTVAL_PLA_PLA_ELEM1                                       (0X0) 
#define RSTVAL_PLA_PLA_ELEM2                                       (0X0) 
#define RSTVAL_PLA_PLA_ELEM3                                       (0X0) 
#define RSTVAL_PLA_PLA_ELEM4                                       (0X0) 
#define RSTVAL_PLA_PLA_ELEM5                                       (0X0) 
#define RSTVAL_PLA_PLA_ELEM6                                       (0X0) 
#define RSTVAL_PLA_PLA_ELEM7                                       (0X0) 
#define RSTVAL_PLA_PLA_ELEM8                                       (0X0) 
#define RSTVAL_PLA_PLA_ELEM9                                       (0X0) 
#define RSTVAL_PLA_PLA_ELEM10                                      (0X0) 
#define RSTVAL_PLA_PLA_ELEM11                                      (0X0) 
#define RSTVAL_PLA_PLA_ELEM12                                      (0X0) 
#define RSTVAL_PLA_PLA_ELEM13                                      (0X0) 
#define RSTVAL_PLA_PLA_ELEM14                                      (0X0) 
#define RSTVAL_PLA_PLA_ELEM15                                      (0X0) 
#define RSTVAL_PLA_PLA_ELEM16                                      (0X0) 
#define RSTVAL_PLA_PLA_ELEM17                                      (0X0) 
#define RSTVAL_PLA_PLA_ELEM18                                      (0X0) 
#define RSTVAL_PLA_PLA_ELEM19                                      (0X0) 
#define RSTVAL_PLA_PLA_ELEM20                                      (0X0) 
#define RSTVAL_PLA_PLA_ELEM21                                      (0X0) 
#define RSTVAL_PLA_PLA_ELEM22                                      (0X0) 
#define RSTVAL_PLA_PLA_ELEM23                                      (0X0) 
#define RSTVAL_PLA_PLA_ELEM24                                      (0X0) 
#define RSTVAL_PLA_PLA_ELEM25                                      (0X0) 
#define RSTVAL_PLA_PLA_ELEM26                                      (0X0) 
#define RSTVAL_PLA_PLA_ELEM27                                      (0X0) 
#define RSTVAL_PLA_PLA_ELEM28                                      (0X0) 
#define RSTVAL_PLA_PLA_ELEM29                                      (0X0) 
#define RSTVAL_PLA_PLA_ELEM30                                      (0X0) 
#define RSTVAL_PLA_PLA_ELEM31                                      (0X0) 
#define RSTVAL_PLA_PLA_CLK                                         (0X0) 
#define RSTVAL_PLA_PLA_IRQ0                                        (0X0) 
#define RSTVAL_PLA_PLA_IRQ1                                        (0X0) 
#define RSTVAL_PLA_PLA_ADC                                         (0X0) 
#define RSTVAL_PLA_PLA_DIN0                                        (0X0) 
#define RSTVAL_PLA_PLA_DOUT0                                       (0X0) 
#define RSTVAL_PLA_PLA_DOUT1                                       (0X0) 
#define RSTVAL_PLA_PLA_LCK                                         (0X0) 
#define RSTVAL_PLA_PLA_IRQTYPE                                     (0X0) 

/* ====================================================================================================
        PLA Module Register BitPositions, Lengths, Masks and Enumerations Definitions
   ==================================================================================================== */

/* ----------------------------------------------------------------------------------------------------
          PLA_ELEM0                                             Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_PLA_PLA_ELEM_N__MUX4                             (0U)           /* Select or Bypass Flip-flop Output */
#define BITL_PLA_PLA_ELEM_N__MUX4                             (1U)           /* Select or Bypass Flip-flop Output */
#define BITM_PLA_PLA_ELEM_N__MUX4                             (0X0001U)      /* Select or Bypass Flip-flop Output */
#define BITP_PLA_PLA_ELEM_N__TBL                              (1U)           /* Configures Output */
#define BITL_PLA_PLA_ELEM_N__TBL                              (4U)           /* Configures Output */
#define BITM_PLA_PLA_ELEM_N__TBL                              (0X001EU)      /* Configures Output */
#define BITP_PLA_PLA_ELEM_N__MUX3                             (5U)           /* Mux Between GPIO Bus Input or Odd Feedback Input */
#define BITL_PLA_PLA_ELEM_N__MUX3                             (1U)           /* Mux Between GPIO Bus Input or Odd Feedback Input */
#define BITM_PLA_PLA_ELEM_N__MUX3                             (0X0020U)      /* Mux Between GPIO Bus Input or Odd Feedback Input */
#define BITP_PLA_PLA_ELEM_N__MUX2                             (6U)           /* Mux Between PLA_DINx Register/Block0 Output or Even Feedback */
#define BITL_PLA_PLA_ELEM_N__MUX2                             (1U)           /* Mux Between PLA_DINx Register/Block0 Output or Even Feedback */
#define BITM_PLA_PLA_ELEM_N__MUX2                             (0X0040U)      /* Mux Between PLA_DINx Register/Block0 Output or Even Feedback */
#define BITP_PLA_PLA_ELEM_N__MUX1                             (7U)           /* Mux for Odd Element Feedback (In Respective Block) */
#define BITL_PLA_PLA_ELEM_N__MUX1                             (3U)           /* Mux for Odd Element Feedback (In Respective Block) */
#define BITM_PLA_PLA_ELEM_N__MUX1                             (0X0380U)      /* Mux for Odd Element Feedback (In Respective Block) */
#define BITP_PLA_PLA_ELEM_N__MUX0                             (10U)          /* Mux for Even Element Feedback(in Respective Block) */
#define BITL_PLA_PLA_ELEM_N__MUX0                             (3U)           /* Mux for Even Element Feedback(in Respective Block) */
#define BITM_PLA_PLA_ELEM_N__MUX0                             (0X1C00U)      /* Mux for Even Element Feedback(in Respective Block) */

#define ENUM_PLA_PLA_ELEM_N__MUX0_ELEM0                       (0X0000U)      /* Feedback from Element 0 (all Except Element 0) / Input from Other Block (Element 0 Only) */
#define ENUM_PLA_PLA_ELEM_N__MUX0_ELEM2                       (0X0001U)      /* Feedback from Element 2 */
#define ENUM_PLA_PLA_ELEM_N__MUX0_ELEM4                       (0X0002U)      /* Feedback from Element 4 */
#define ENUM_PLA_PLA_ELEM_N__MUX0_ELEM6                       (0X0003U)      /* Feedback from Element 6 */
#define ENUM_PLA_PLA_ELEM_N__MUX0_ELEM8                       (0X0004U)      /* Feedback from Element 8 */
#define ENUM_PLA_PLA_ELEM_N__MUX0_ELEM10                      (0X0005U)      /* Feedback from Element 10 */
#define ENUM_PLA_PLA_ELEM_N__MUX0_ELEM12                      (0X0006U)      /* Feedback from Element 12 */
#define ENUM_PLA_PLA_ELEM_N__MUX0_ELEM14                      (0X0007U)      /* Feedback from Element 14 */
#define ENUM_PLA_PLA_ELEM_N__MUX1_ELEM1                       (0X0000U)      /* Feedback from Element 1 */
#define ENUM_PLA_PLA_ELEM_N__MUX1_ELEM3                       (0X0001U)      /* Feedback from Element 3 */
#define ENUM_PLA_PLA_ELEM_N__MUX1_ELEM5                       (0X0002U)      /* Feedback from Element 5 */
#define ENUM_PLA_PLA_ELEM_N__MUX1_ELEM7                       (0X0003U)      /* Feedback from Element 7 */
#define ENUM_PLA_PLA_ELEM_N__MUX1_ELEM9                       (0X0004U)      /* Feedback from Element 9 */
#define ENUM_PLA_PLA_ELEM_N__MUX1_ELEM11                      (0X0005U)      /* Feedback from Element 11 */
#define ENUM_PLA_PLA_ELEM_N__MUX1_ELEM13                      (0X0006U)      /* Feedback from Element 13 */
#define ENUM_PLA_PLA_ELEM_N__MUX1_ELEM15                      (0X0007U)      /* Feedback from Element 15 */
#define ENUM_PLA_PLA_ELEM_N__MUX2_MMR                         (0X0000U)      /* PLA_DINx Input */
#define ENUM_PLA_PLA_ELEM_N__MUX2_EVEN                        (0X0001U)      /* Even Feedback Mux */
#define ENUM_PLA_PLA_ELEM_N__MUX3_ODD                         (0X0000U)      /* Odd Feedback Mux */
#define ENUM_PLA_PLA_ELEM_N__MUX3_GPIO                        (0X0001U)      /* GPIO Input */
#define ENUM_PLA_PLA_ELEM_N__TBL_LOW                          (0X0000U)      /* 0. */
#define ENUM_PLA_PLA_ELEM_N__TBL_FUN_NOR                      (0X0001U)      /* NOR. */
#define ENUM_PLA_PLA_ELEM_N__TBL_BANDNOTA                     (0X0002U)      /* B and Not A. */
#define ENUM_PLA_PLA_ELEM_N__TBL_NOTA                         (0X0003U)      /* NOT A. */
#define ENUM_PLA_PLA_ELEM_N__TBL_AANDNOTB                     (0X0004U)      /* A and Not B. */
#define ENUM_PLA_PLA_ELEM_N__TBL_NOTB                         (0X0005U)      /* Not B. */
#define ENUM_PLA_PLA_ELEM_N__TBL_EXOR                         (0X0006U)      /* EXOR. */
#define ENUM_PLA_PLA_ELEM_N__TBL_FUN_NAND                     (0X0007U)      /* NAND. */
#define ENUM_PLA_PLA_ELEM_N__TBL_FUN_AND                      (0X0008U)      /* AND. */
#define ENUM_PLA_PLA_ELEM_N__TBL_EXNOR                        (0X0009U)      /* EXNOR. */
#define ENUM_PLA_PLA_ELEM_N__TBL_B                            (0X000AU)      /* B. */
#define ENUM_PLA_PLA_ELEM_N__TBL_BORNOTA                      (0X000BU)      /* B or Not A. */
#define ENUM_PLA_PLA_ELEM_N__TBL_A                            (0X000CU)      /* A. */
#define ENUM_PLA_PLA_ELEM_N__TBL_AORNOTB                      (0X000DU)      /* A or Not B. */
#define ENUM_PLA_PLA_ELEM_N__TBL_FUN_OR                       (0X000EU)      /* OR */
#define ENUM_PLA_PLA_ELEM_N__TBL_HIGH                         (0X000FU)      /* 1. */
#define ENUM_PLA_PLA_ELEM_N__MUX4_FF                          (0X0000U)      /* FF Output */
#define ENUM_PLA_PLA_ELEM_N__MUX4_BYPASS                      (0X0001U)      /* Bypass Output */

/* ----------------------------------------------------------------------------------------------------
          PLA_CLK                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_PLA_PLA_CLK_BLOCK0                               (0U)           /* Clock Select for Block 0 */
#define BITL_PLA_PLA_CLK_BLOCK0                               (3U)           /* Clock Select for Block 0 */
#define BITM_PLA_PLA_CLK_BLOCK0                               (0X0007U)      /* Clock Select for Block 0 */
#define BITP_PLA_PLA_CLK_BLOCK1                               (4U)           /* Clock Select for Block 1 */
#define BITL_PLA_PLA_CLK_BLOCK1                               (3U)           /* Clock Select for Block 1 */
#define BITM_PLA_PLA_CLK_BLOCK1                               (0X0070U)      /* Clock Select for Block 1 */

#define ENUM_PLA_PLA_CLK_BLOCK1_PLACLK0                       (0X0000U)      /* GPIO Clock 0 */
#define ENUM_PLA_PLA_CLK_BLOCK1_PLACLK1                       (0X0001U)      /* GPIO Clock 1 */
#define ENUM_PLA_PLA_CLK_BLOCK1_PLACLK2                       (0X0002U)      /* GPIO Clock 2 */
#define ENUM_PLA_PLA_CLK_BLOCK1_PCLK0                         (0X0003U)      /* PCLK0 */
#define ENUM_PLA_PLA_CLK_BLOCK1_MOSC                          (0X0004U)      /* MOSC (16 MHz) */
#define ENUM_PLA_PLA_CLK_BLOCK1_T0                            (0X0005U)      /* Timer 0 */
#define ENUM_PLA_PLA_CLK_BLOCK1_T2                            (0X0006U)      /* Timer 2 */
#define ENUM_PLA_PLA_CLK_BLOCK1_KOSC                          (0X0007U)      /* KOSC (32 KHz) */
#define ENUM_PLA_PLA_CLK_BLOCK0_PLACLK0                       (0X0000U)      /* GPIO Clock 0 */
#define ENUM_PLA_PLA_CLK_BLOCK0_PLACLK1                       (0X0001U)      /* GPIO Clock 1 */
#define ENUM_PLA_PLA_CLK_BLOCK0_PLACLK2                       (0X0002U)      /* GPIO Clock 2 */
#define ENUM_PLA_PLA_CLK_BLOCK0_PCLK0                         (0X0003U)      /* PCLK0 */
#define ENUM_PLA_PLA_CLK_BLOCK0_MOSC                          (0X0004U)      /* MOSC (16 MHz) */
#define ENUM_PLA_PLA_CLK_BLOCK0_T0                            (0X0005U)      /* Timer 0 */
#define ENUM_PLA_PLA_CLK_BLOCK0_T2                            (0X0006U)      /* Timer 2 */
#define ENUM_PLA_PLA_CLK_BLOCK0_KOSC                          (0X0007U)      /* KOSC (32 KHz) */

/* ----------------------------------------------------------------------------------------------------
          PLA_IRQ0                                              Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_PLA_PLA_IRQ0_IRQ0_SRC                            (0U)           /* IRQ0 Source Select */
#define BITL_PLA_PLA_IRQ0_IRQ0_SRC                            (4U)           /* IRQ0 Source Select */
#define BITM_PLA_PLA_IRQ0_IRQ0_SRC                            (0X000FU)      /* IRQ0 Source Select */
#define BITP_PLA_PLA_IRQ0_IRQ0_EN                             (4U)           /* IRQ0 Enable */
#define BITL_PLA_PLA_IRQ0_IRQ0_EN                             (1U)           /* IRQ0 Enable */
#define BITM_PLA_PLA_IRQ0_IRQ0_EN                             (0X0010U)      /* IRQ0 Enable */
#define BITP_PLA_PLA_IRQ0_IRQ1_SRC                            (8U)           /* IRQ1 Source Select */
#define BITL_PLA_PLA_IRQ0_IRQ1_SRC                            (4U)           /* IRQ1 Source Select */
#define BITM_PLA_PLA_IRQ0_IRQ1_SRC                            (0X0F00U)      /* IRQ1 Source Select */
#define BITP_PLA_PLA_IRQ0_IRQ1_EN                             (12U)          /* IRQ1 Enable */
#define BITL_PLA_PLA_IRQ0_IRQ1_EN                             (1U)           /* IRQ1 Enable */
#define BITM_PLA_PLA_IRQ0_IRQ1_EN                             (0X1000U)      /* IRQ1 Enable */

#define ENUM_PLA_PLA_IRQ0_IRQ1_EN_DIS                         (0X0000U)      /* Disable IRQ1 Interrupt */
#define ENUM_PLA_PLA_IRQ0_IRQ1_EN_EN                          (0X0001U)      /* Enable IRQ1 Interrupt */
#define ENUM_PLA_PLA_IRQ0_IRQ0_EN_DIS                         (0X0000U)      /* Disable IRQ0 Interrupt */
#define ENUM_PLA_PLA_IRQ0_IRQ0_EN_EN                          (0X0001U)      /* Enable IRQ0 Interrupt */

/* ----------------------------------------------------------------------------------------------------
          PLA_IRQ1                                              Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_PLA_PLA_IRQ1_IRQ2_SRC                            (0U)           /* IRQ2 Source Select */
#define BITL_PLA_PLA_IRQ1_IRQ2_SRC                            (4U)           /* IRQ2 Source Select */
#define BITM_PLA_PLA_IRQ1_IRQ2_SRC                            (0X000FU)      /* IRQ2 Source Select */
#define BITP_PLA_PLA_IRQ1_IRQ2_EN                             (4U)           /* IRQ2 Enable */
#define BITL_PLA_PLA_IRQ1_IRQ2_EN                             (1U)           /* IRQ2 Enable */
#define BITM_PLA_PLA_IRQ1_IRQ2_EN                             (0X0010U)      /* IRQ2 Enable */
#define BITP_PLA_PLA_IRQ1_IRQ3_SRC                            (8U)           /* IRQ3 Source Select */
#define BITL_PLA_PLA_IRQ1_IRQ3_SRC                            (4U)           /* IRQ3 Source Select */
#define BITM_PLA_PLA_IRQ1_IRQ3_SRC                            (0X0F00U)      /* IRQ3 Source Select */
#define BITP_PLA_PLA_IRQ1_IRQ3_EN                             (12U)          /* IRQ3 Enable */
#define BITL_PLA_PLA_IRQ1_IRQ3_EN                             (1U)           /* IRQ3 Enable */
#define BITM_PLA_PLA_IRQ1_IRQ3_EN                             (0X1000U)      /* IRQ3 Enable */

#define ENUM_PLA_PLA_IRQ1_IRQ3_EN_DIS                         (0X0000U)      /* Disable IRQ3 Interrupt */
#define ENUM_PLA_PLA_IRQ1_IRQ3_EN_EN                          (0X0001U)      /* Enable IRQ3 Interrupt */
#define ENUM_PLA_PLA_IRQ1_IRQ2_EN_DIS                         (0X0000U)      /* Disable IRQ2 Interrupt */
#define ENUM_PLA_PLA_IRQ1_IRQ2_EN_EN                          (0X0001U)      /* Enable IRQ2 Interrupt */

/* ----------------------------------------------------------------------------------------------------
          PLA_ADC                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_PLA_PLA_ADC_CONVST_SRC                           (0U)           /* Element for ADC Start Convert Source */
#define BITL_PLA_PLA_ADC_CONVST_SRC                           (5U)           /* Element for ADC Start Convert Source */
#define BITM_PLA_PLA_ADC_CONVST_SRC                           (0X001FU)      /* Element for ADC Start Convert Source */
#define BITP_PLA_PLA_ADC_CONVST_EN                            (5U)           /* Bit to Enable ADC Start Convert from PLA */
#define BITL_PLA_PLA_ADC_CONVST_EN                            (1U)           /* Bit to Enable ADC Start Convert from PLA */
#define BITM_PLA_PLA_ADC_CONVST_EN                            (0X0020U)      /* Bit to Enable ADC Start Convert from PLA */

#define ENUM_PLA_PLA_ADC_CONVST_EN_DISABLE                    (0X0000U)      /* Disable */
#define ENUM_PLA_PLA_ADC_CONVST_EN_ENABLE                     (0X0001U)      /* Enable */

/* ----------------------------------------------------------------------------------------------------
          PLA_DIN0                                              Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_PLA_PLA_DIN0_DIN                                 (0U)           /* Input Bit to Element 15 to Element 0. */
#define BITL_PLA_PLA_DIN0_DIN                                 (16U)          /* Input Bit to Element 15 to Element 0. */
#define BITM_PLA_PLA_DIN0_DIN                                 (0XFFFFU)      /* Input Bit to Element 15 to Element 0. */

/* ----------------------------------------------------------------------------------------------------
          PLA_DOUT0                                             Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_PLA_PLA_DOUT0_E0                                 (0U)           /* Output Bit from Element 0 */
#define BITL_PLA_PLA_DOUT0_E0                                 (1U)           /* Output Bit from Element 0 */
#define BITM_PLA_PLA_DOUT0_E0                                 (0X0001U)      /* Output Bit from Element 0 */
#define BITP_PLA_PLA_DOUT0_E1                                 (1U)           /* Output Bit from Element 1 */
#define BITL_PLA_PLA_DOUT0_E1                                 (1U)           /* Output Bit from Element 1 */
#define BITM_PLA_PLA_DOUT0_E1                                 (0X0002U)      /* Output Bit from Element 1 */
#define BITP_PLA_PLA_DOUT0_E2                                 (2U)           /* Output Bit from Element 2 */
#define BITL_PLA_PLA_DOUT0_E2                                 (1U)           /* Output Bit from Element 2 */
#define BITM_PLA_PLA_DOUT0_E2                                 (0X0004U)      /* Output Bit from Element 2 */
#define BITP_PLA_PLA_DOUT0_E3                                 (3U)           /* Output Bit from Element 3 */
#define BITL_PLA_PLA_DOUT0_E3                                 (1U)           /* Output Bit from Element 3 */
#define BITM_PLA_PLA_DOUT0_E3                                 (0X0008U)      /* Output Bit from Element 3 */
#define BITP_PLA_PLA_DOUT0_E4                                 (4U)           /* Output Bit from Element 4 */
#define BITL_PLA_PLA_DOUT0_E4                                 (1U)           /* Output Bit from Element 4 */
#define BITM_PLA_PLA_DOUT0_E4                                 (0X0010U)      /* Output Bit from Element 4 */
#define BITP_PLA_PLA_DOUT0_E5                                 (5U)           /* Output Bit from Element 5 */
#define BITL_PLA_PLA_DOUT0_E5                                 (1U)           /* Output Bit from Element 5 */
#define BITM_PLA_PLA_DOUT0_E5                                 (0X0020U)      /* Output Bit from Element 5 */
#define BITP_PLA_PLA_DOUT0_E6                                 (6U)           /* Output Bit from Element 6 */
#define BITL_PLA_PLA_DOUT0_E6                                 (1U)           /* Output Bit from Element 6 */
#define BITM_PLA_PLA_DOUT0_E6                                 (0X0040U)      /* Output Bit from Element 6 */
#define BITP_PLA_PLA_DOUT0_E7                                 (7U)           /* Output Bit from Element 7 */
#define BITL_PLA_PLA_DOUT0_E7                                 (1U)           /* Output Bit from Element 7 */
#define BITM_PLA_PLA_DOUT0_E7                                 (0X0080U)      /* Output Bit from Element 7 */
#define BITP_PLA_PLA_DOUT0_E8                                 (8U)           /* Output Bit from Element 8 */
#define BITL_PLA_PLA_DOUT0_E8                                 (1U)           /* Output Bit from Element 8 */
#define BITM_PLA_PLA_DOUT0_E8                                 (0X0100U)      /* Output Bit from Element 8 */
#define BITP_PLA_PLA_DOUT0_E9                                 (9U)           /* Output Bit from Element 9 */
#define BITL_PLA_PLA_DOUT0_E9                                 (1U)           /* Output Bit from Element 9 */
#define BITM_PLA_PLA_DOUT0_E9                                 (0X0200U)      /* Output Bit from Element 9 */
#define BITP_PLA_PLA_DOUT0_E10                                (10U)          /* Output Bit from Element 10 */
#define BITL_PLA_PLA_DOUT0_E10                                (1U)           /* Output Bit from Element 10 */
#define BITM_PLA_PLA_DOUT0_E10                                (0X0400U)      /* Output Bit from Element 10 */
#define BITP_PLA_PLA_DOUT0_E11                                (11U)          /* Output Bit from Element 11 */
#define BITL_PLA_PLA_DOUT0_E11                                (1U)           /* Output Bit from Element 11 */
#define BITM_PLA_PLA_DOUT0_E11                                (0X0800U)      /* Output Bit from Element 11 */
#define BITP_PLA_PLA_DOUT0_E12                                (12U)          /* Output Bit from Element 12 */
#define BITL_PLA_PLA_DOUT0_E12                                (1U)           /* Output Bit from Element 12 */
#define BITM_PLA_PLA_DOUT0_E12                                (0X1000U)      /* Output Bit from Element 12 */
#define BITP_PLA_PLA_DOUT0_E13                                (13U)          /* Output Bit from Element 13 */
#define BITL_PLA_PLA_DOUT0_E13                                (1U)           /* Output Bit from Element 13 */
#define BITM_PLA_PLA_DOUT0_E13                                (0X2000U)      /* Output Bit from Element 13 */
#define BITP_PLA_PLA_DOUT0_E14                                (14U)          /* Output Bit from Element 14 */
#define BITL_PLA_PLA_DOUT0_E14                                (1U)           /* Output Bit from Element 14 */
#define BITM_PLA_PLA_DOUT0_E14                                (0X4000U)      /* Output Bit from Element 14 */
#define BITP_PLA_PLA_DOUT0_E15                                (15U)          /* Output Bit from Element 15 */
#define BITL_PLA_PLA_DOUT0_E15                                (1U)           /* Output Bit from Element 15 */
#define BITM_PLA_PLA_DOUT0_E15                                (0X8000U)      /* Output Bit from Element 15 */

/* ----------------------------------------------------------------------------------------------------
          PLA_DOUT1                                             Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_PLA_PLA_DOUT1_E16                                (0U)           /* Output Bit from Element 16 */
#define BITL_PLA_PLA_DOUT1_E16                                (1U)           /* Output Bit from Element 16 */
#define BITM_PLA_PLA_DOUT1_E16                                (0X0001U)      /* Output Bit from Element 16 */
#define BITP_PLA_PLA_DOUT1_E17                                (1U)           /* Output Bit from Element 17 */
#define BITL_PLA_PLA_DOUT1_E17                                (1U)           /* Output Bit from Element 17 */
#define BITM_PLA_PLA_DOUT1_E17                                (0X0002U)      /* Output Bit from Element 17 */
#define BITP_PLA_PLA_DOUT1_E18                                (2U)           /* Output Bit from Element 18 */
#define BITL_PLA_PLA_DOUT1_E18                                (1U)           /* Output Bit from Element 18 */
#define BITM_PLA_PLA_DOUT1_E18                                (0X0004U)      /* Output Bit from Element 18 */
#define BITP_PLA_PLA_DOUT1_E19                                (3U)           /* Output Bit from Element 19 */
#define BITL_PLA_PLA_DOUT1_E19                                (1U)           /* Output Bit from Element 19 */
#define BITM_PLA_PLA_DOUT1_E19                                (0X0008U)      /* Output Bit from Element 19 */
#define BITP_PLA_PLA_DOUT1_E20                                (4U)           /* Output Bit from Element 20 */
#define BITL_PLA_PLA_DOUT1_E20                                (1U)           /* Output Bit from Element 20 */
#define BITM_PLA_PLA_DOUT1_E20                                (0X0010U)      /* Output Bit from Element 20 */
#define BITP_PLA_PLA_DOUT1_E21                                (5U)           /* Output Bit from Element 21 */
#define BITL_PLA_PLA_DOUT1_E21                                (1U)           /* Output Bit from Element 21 */
#define BITM_PLA_PLA_DOUT1_E21                                (0X0020U)      /* Output Bit from Element 21 */
#define BITP_PLA_PLA_DOUT1_E22                                (6U)           /* Output Bit from Element 22 */
#define BITL_PLA_PLA_DOUT1_E22                                (1U)           /* Output Bit from Element 22 */
#define BITM_PLA_PLA_DOUT1_E22                                (0X0040U)      /* Output Bit from Element 22 */
#define BITP_PLA_PLA_DOUT1_E23                                (7U)           /* Output Bit from Element 23 */
#define BITL_PLA_PLA_DOUT1_E23                                (1U)           /* Output Bit from Element 23 */
#define BITM_PLA_PLA_DOUT1_E23                                (0X0080U)      /* Output Bit from Element 23 */
#define BITP_PLA_PLA_DOUT1_E24                                (8U)           /* Output Bit from Element 24 */
#define BITL_PLA_PLA_DOUT1_E24                                (1U)           /* Output Bit from Element 24 */
#define BITM_PLA_PLA_DOUT1_E24                                (0X0100U)      /* Output Bit from Element 24 */
#define BITP_PLA_PLA_DOUT1_E25                                (9U)           /* Output Bit from Element 25 */
#define BITL_PLA_PLA_DOUT1_E25                                (1U)           /* Output Bit from Element 25 */
#define BITM_PLA_PLA_DOUT1_E25                                (0X0200U)      /* Output Bit from Element 25 */
#define BITP_PLA_PLA_DOUT1_E26                                (10U)          /* Output Bit from Element 26 */
#define BITL_PLA_PLA_DOUT1_E26                                (1U)           /* Output Bit from Element 26 */
#define BITM_PLA_PLA_DOUT1_E26                                (0X0400U)      /* Output Bit from Element 26 */
#define BITP_PLA_PLA_DOUT1_E27                                (11U)          /* Output Bit from Element 27 */
#define BITL_PLA_PLA_DOUT1_E27                                (1U)           /* Output Bit from Element 27 */
#define BITM_PLA_PLA_DOUT1_E27                                (0X0800U)      /* Output Bit from Element 27 */
#define BITP_PLA_PLA_DOUT1_E28                                (12U)          /* Output Bit from Element 28 */
#define BITL_PLA_PLA_DOUT1_E28                                (1U)           /* Output Bit from Element 28 */
#define BITM_PLA_PLA_DOUT1_E28                                (0X1000U)      /* Output Bit from Element 28 */
#define BITP_PLA_PLA_DOUT1_E29                                (13U)          /* Output Bit from Element 29 */
#define BITL_PLA_PLA_DOUT1_E29                                (1U)           /* Output Bit from Element 29 */
#define BITM_PLA_PLA_DOUT1_E29                                (0X2000U)      /* Output Bit from Element 29 */
#define BITP_PLA_PLA_DOUT1_E30                                (14U)          /* Output Bit from Element 30 */
#define BITL_PLA_PLA_DOUT1_E30                                (1U)           /* Output Bit from Element 30 */
#define BITM_PLA_PLA_DOUT1_E30                                (0X4000U)      /* Output Bit from Element 30 */
#define BITP_PLA_PLA_DOUT1_E31                                (15U)          /* Output Bit from Element 31 */
#define BITL_PLA_PLA_DOUT1_E31                                (1U)           /* Output Bit from Element 31 */
#define BITM_PLA_PLA_DOUT1_E31                                (0X8000U)      /* Output Bit from Element 31 */

/* ----------------------------------------------------------------------------------------------------
          PLA_LCK                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_PLA_PLA_LCK_LOCKED                               (0U)           /* Set to Disable Writing to Registers */
#define BITL_PLA_PLA_LCK_LOCKED                               (1U)           /* Set to Disable Writing to Registers */
#define BITM_PLA_PLA_LCK_LOCKED                               (0X0001U)      /* Set to Disable Writing to Registers */

#define ENUM_PLA_PLA_LCK_LOCKED_DIS                           (0X0000U)      /* Writing to Registers Allowed */
#define ENUM_PLA_PLA_LCK_LOCKED_EN                            (0X0001U)      /* Writing to Registers Disabled */

/* ----------------------------------------------------------------------------------------------------
          PLA_IRQTYPE                                           Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_PLA_PLA_IRQTYPE_IRQ0_TYPE                        (0U)           /* IRQ0 and DMA Request 0 Type */
#define BITL_PLA_PLA_IRQTYPE_IRQ0_TYPE                        (2U)           /* IRQ0 and DMA Request 0 Type */
#define BITM_PLA_PLA_IRQTYPE_IRQ0_TYPE                        (0X0003U)      /* IRQ0 and DMA Request 0 Type */
#define BITP_PLA_PLA_IRQTYPE_IRQ1_TYPE                        (2U)           /* IRQ1 and DMA Request 1Type */
#define BITL_PLA_PLA_IRQTYPE_IRQ1_TYPE                        (2U)           /* IRQ1 and DMA Request 1Type */
#define BITM_PLA_PLA_IRQTYPE_IRQ1_TYPE                        (0X000CU)      /* IRQ1 and DMA Request 1Type */
#define BITP_PLA_PLA_IRQTYPE_IRQ2_TYPE                        (4U)           /* IRQ2 and DMA Request 2 Type */
#define BITL_PLA_PLA_IRQTYPE_IRQ2_TYPE                        (2U)           /* IRQ2 and DMA Request 2 Type */
#define BITM_PLA_PLA_IRQTYPE_IRQ2_TYPE                        (0X0030U)      /* IRQ2 and DMA Request 2 Type */
#define BITP_PLA_PLA_IRQTYPE_IRQ3_TYPE                        (6U)           /* IRQ3 and DMA Request 3 Type */
#define BITL_PLA_PLA_IRQTYPE_IRQ3_TYPE                        (2U)           /* IRQ3 and DMA Request 3 Type */
#define BITM_PLA_PLA_IRQTYPE_IRQ3_TYPE                        (0X00C0U)      /* IRQ3 and DMA Request 3 Type */

#define ENUM_PLA_PLA_IRQTYPE_IRQ3_TYPE_HIGH_LEVEL             (0X0000U)      /* High Level Trigger Interrupt and High Level Trigger DMA Request */
#define ENUM_PLA_PLA_IRQTYPE_IRQ3_TYPE_RISING_EDGE            (0X0001U)      /* Rising Edge Trigger Interrupt and High Level Trigger DMA Request */
#define ENUM_PLA_PLA_IRQTYPE_IRQ3_TYPE_LOW_LEVEL              (0X0002U)      /* Low Level Trigger Interrupt and Low Level Trigger DMA Request */
#define ENUM_PLA_PLA_IRQTYPE_IRQ3_TYPE_FALLING_EDGE           (0X0003U)      /* Falling Edge Trigger Interrupt and Low Level Trigger DMA Request */
#define ENUM_PLA_PLA_IRQTYPE_IRQ2_TYPE_HIGH_LEVEL             (0X0000U)      /* High Level Trigger Interrupt and High Level Trigger DMA Request */
#define ENUM_PLA_PLA_IRQTYPE_IRQ2_TYPE_RISING_EDGE            (0X0001U)      /* Rising Edge Trigger Interrupt and High Level Trigger DMA Request */
#define ENUM_PLA_PLA_IRQTYPE_IRQ2_TYPE_LOW_LEVEL              (0X0002U)      /* Low Level Trigger Interrupt and Low Level Trigger DMA Request */
#define ENUM_PLA_PLA_IRQTYPE_IRQ2_TYPE_FALLING_EDGE           (0X0003U)      /* Falling Edge Trigger Interrupt and Low Level Trigger DMA Request */
#define ENUM_PLA_PLA_IRQTYPE_IRQ1_TYPE_HIGH_LEVEL             (0X0000U)      /* High Level Trigger Interrupt and High Level Trigger DMA Request */
#define ENUM_PLA_PLA_IRQTYPE_IRQ1_TYPE_RISING_EDGE            (0X0001U)      /* Rising Edge Trigger Interrupt and High Level Trigger DMA Request */
#define ENUM_PLA_PLA_IRQTYPE_IRQ1_TYPE_LOW_LEVEL              (0X0002U)      /* Low Level Trigger Interrupt and Low Level Trigger DMA Request */
#define ENUM_PLA_PLA_IRQTYPE_IRQ1_TYPE_FALLING_EDGE           (0X0003U)      /* Falling Edge Trigger Interrupt and Low Level Trigger DMA Request */
#define ENUM_PLA_PLA_IRQTYPE_IRQ0_TYPE_HIGH_LEVEL             (0X0000U)      /* High Level Trigger Interrupt and High Level Trigger DMA Request */
#define ENUM_PLA_PLA_IRQTYPE_IRQ0_TYPE_RISING_EDGE            (0X0001U)      /* Rising Edge Trigger Interrupt and Rising Edge Trigger DMA Request */
#define ENUM_PLA_PLA_IRQTYPE_IRQ0_TYPE_LOW_LEVEL              (0X0002U)      /* Low Level Trigger Interrupt and Low Level Trigger DMA Request */
#define ENUM_PLA_PLA_IRQTYPE_IRQ0_TYPE_FALLING_EDGE           (0X0003U)      /* Falling Edge Trigger Interrupt and Falling Edge Trigger DMA Request */

#endif  /* end ifndef PLA_ADDR_RDEF_H_ */


#ifndef DMAREQ_ADDR_RDEF_H_
#define DMAREQ_ADDR_RDEF_H_    /* DMAREQ: Your module description, here. */

/* ====================================================================================================
        DMAREQ Module Instances Address and Mask Definitions
   ==================================================================================================== */
#define INST_DMAREQ                                              (0X40007000U)    /* dmareq: */

#define MASK_DMAREQ                                              (0X000000FFU)    /* DMAREQ: Your module description, here. */

/* ====================================================================================================
        DMAREQ Module Register Address Offset Definitions
   ==================================================================================================== */
#define IDX_DMAREQ_REQEN                                        (0X00U)    /* GPT/GPT32 and PLA DMA Request Enable */
#define IDX_DMAREQ_REQ0SEL                                      (0X04U)    /* GPT/GPT32 and PLA DMA Request 0 Select */
#define IDX_DMAREQ_REQ1SEL                                      (0X08U)    /* GPT/GPT32 and PLA DMA Request 1 Select */
#define IDX_DMAREQ_PLAREQEN                                     (0X0CU)    /* PLA DMA Requests Enable */
#define IDX_DMAREQ_GPTREQEN                                     (0X10U)    /* GPT/GPT32 DMA Requests Enable */
#define IDX_DMAREQ_GPT_MDA_REQ_TTYPE                            (0X14U)    /* GPT and GPT32 Require Type */

/* ====================================================================================================
        DMAREQ Module Register ResetValue Definitions
   ==================================================================================================== */
#define RSTVAL_DMAREQ_REQEN                                        (0X0) 
#define RSTVAL_DMAREQ_REQ0SEL                                      (0X0) 
#define RSTVAL_DMAREQ_REQ1SEL                                      (0X0) 
#define RSTVAL_DMAREQ_PLAREQEN                                     (0X0) 
#define RSTVAL_DMAREQ_GPTREQEN                                     (0X0) 
#define RSTVAL_DMAREQ_GPT_MDA_REQ_TTYPE                            (0X0) 

/* ====================================================================================================
        DMAREQ Module Register BitPositions, Lengths, Masks and Enumerations Definitions
   ==================================================================================================== */

/* ----------------------------------------------------------------------------------------------------
          REQEN                                                 Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_DMAREQ_REQEN_GPLA_DMA_EN0                        (0U)           /* GPT/GPT32 and PLA DMA Request 0 Enable */
#define BITL_DMAREQ_REQEN_GPLA_DMA_EN0                        (1U)           /* GPT/GPT32 and PLA DMA Request 0 Enable */
#define BITM_DMAREQ_REQEN_GPLA_DMA_EN0                        (0X01U)        /* GPT/GPT32 and PLA DMA Request 0 Enable */
#define BITP_DMAREQ_REQEN_GPLA_DMA_EN1                        (1U)           /* GPT/GPT32 and PLA DMA Request 1 Enable */
#define BITL_DMAREQ_REQEN_GPLA_DMA_EN1                        (1U)           /* GPT/GPT32 and PLA DMA Request 1 Enable */
#define BITM_DMAREQ_REQEN_GPLA_DMA_EN1                        (0X02U)        /* GPT/GPT32 and PLA DMA Request 1 Enable */

/* ----------------------------------------------------------------------------------------------------
          REQ0SEL                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_DMAREQ_REQ0SEL_DMA_REQ0_SEL                      (0U)           /* GPT/GPT32 and PLA DMA Request 0 Source Select */
#define BITL_DMAREQ_REQ0SEL_DMA_REQ0_SEL                      (4U)           /* GPT/GPT32 and PLA DMA Request 0 Source Select */
#define BITM_DMAREQ_REQ0SEL_DMA_REQ0_SEL                      (0X0FU)        /* GPT/GPT32 and PLA DMA Request 0 Source Select */

#define ENUM_DMAREQ_REQ0SEL_DMA_REQ0_SEL_PLA_REQ0             (0X00U)        /* PLA DMA Request 0 */
#define ENUM_DMAREQ_REQ0SEL_DMA_REQ0_SEL_PLA_REQ1             (0X01U)        /* PLA DMA Request 1 */
#define ENUM_DMAREQ_REQ0SEL_DMA_REQ0_SEL_PLA_REQ2             (0X02U)        /* PLA DMA Request 2 */
#define ENUM_DMAREQ_REQ0SEL_DMA_REQ0_SEL_PLA_REQ3             (0X03U)        /* PLA DMA Request 3 */
#define ENUM_DMAREQ_REQ0SEL_DMA_REQ0_SEL_GPT_REQ0             (0X04U)        /* GPT 0 DMA Request */
#define ENUM_DMAREQ_REQ0SEL_DMA_REQ0_SEL_GPT_REQ1             (0X05U)        /* GPT 1 DMA Request */
#define ENUM_DMAREQ_REQ0SEL_DMA_REQ0_SEL_GPT_REQ2             (0X06U)        /* GPT 2 DMA Request */
#define ENUM_DMAREQ_REQ0SEL_DMA_REQ0_SEL_GPT32_REQ0           (0X07U)        /* GPT32 0 DMA Request */
#define ENUM_DMAREQ_REQ0SEL_DMA_REQ0_SEL_GPT32_REQ1           (0X08U)        /* GPT32 1 DMA Request */

/* ----------------------------------------------------------------------------------------------------
          REQ1SEL                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_DMAREQ_REQ1SEL_DMA_REQ1_SEL                      (0U)           /* GPT/GPT32 and PLA DMA Request 1 Source Select */
#define BITL_DMAREQ_REQ1SEL_DMA_REQ1_SEL                      (4U)           /* GPT/GPT32 and PLA DMA Request 1 Source Select */
#define BITM_DMAREQ_REQ1SEL_DMA_REQ1_SEL                      (0X0FU)        /* GPT/GPT32 and PLA DMA Request 1 Source Select */

#define ENUM_DMAREQ_REQ1SEL_DMA_REQ1_SEL_PLA_REQ0             (0X00U)        /* PLA DMA Request 0 */
#define ENUM_DMAREQ_REQ1SEL_DMA_REQ1_SEL_PLA_REQ1             (0X01U)        /* PLA DMA Request 1 */
#define ENUM_DMAREQ_REQ1SEL_DMA_REQ1_SEL_PLA_REQ2             (0X02U)        /* PLA DMA Request 2 */
#define ENUM_DMAREQ_REQ1SEL_DMA_REQ1_SEL_PLA_REQ3             (0X03U)        /* PLA DMA Request 3 */
#define ENUM_DMAREQ_REQ1SEL_DMA_REQ1_SEL_GPT_REQ0             (0X04U)        /* GPT 0 DMA Request */
#define ENUM_DMAREQ_REQ1SEL_DMA_REQ1_SEL_GPT_REQ1             (0X05U)        /* GPT 1 DMA Request */
#define ENUM_DMAREQ_REQ1SEL_DMA_REQ1_SEL_GPT_REQ2             (0X06U)        /* GPT 2 DMA Request */
#define ENUM_DMAREQ_REQ1SEL_DMA_REQ1_SEL_GPT32_REQ0           (0X07U)        /* GPT32 0 DMA Request */
#define ENUM_DMAREQ_REQ1SEL_DMA_REQ1_SEL_GPT32_REQ1           (0X08U)        /* GPT32 1 DMA Request */

/* ----------------------------------------------------------------------------------------------------
          PLAREQEN                                              Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_DMAREQ_PLAREQEN_PLA_DMA_REQ0_EN                  (0U)           /* PLA DMA Request 0 Enable */
#define BITL_DMAREQ_PLAREQEN_PLA_DMA_REQ0_EN                  (1U)           /* PLA DMA Request 0 Enable */
#define BITM_DMAREQ_PLAREQEN_PLA_DMA_REQ0_EN                  (0X01U)        /* PLA DMA Request 0 Enable */
#define BITP_DMAREQ_PLAREQEN_PLA_DMA_REQ1_EN                  (1U)           /* PLA DMA Request 1 Enable */
#define BITL_DMAREQ_PLAREQEN_PLA_DMA_REQ1_EN                  (1U)           /* PLA DMA Request 1 Enable */
#define BITM_DMAREQ_PLAREQEN_PLA_DMA_REQ1_EN                  (0X02U)        /* PLA DMA Request 1 Enable */
#define BITP_DMAREQ_PLAREQEN_PLA_DMA_REQ2_EN                  (2U)           /* PLA DMA Request 2 Enable */
#define BITL_DMAREQ_PLAREQEN_PLA_DMA_REQ2_EN                  (1U)           /* PLA DMA Request 2 Enable */
#define BITM_DMAREQ_PLAREQEN_PLA_DMA_REQ2_EN                  (0X04U)        /* PLA DMA Request 2 Enable */
#define BITP_DMAREQ_PLAREQEN_PLA_DMA_REQ3_EN                  (3U)           /* PLA DMA Request 3 Enable */
#define BITL_DMAREQ_PLAREQEN_PLA_DMA_REQ3_EN                  (1U)           /* PLA DMA Request 3 Enable */
#define BITM_DMAREQ_PLAREQEN_PLA_DMA_REQ3_EN                  (0X08U)        /* PLA DMA Request 3 Enable */

/* ----------------------------------------------------------------------------------------------------
          GPTREQEN                                              Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_DMAREQ_GPTREQEN_GPT_DMA_REQ0_EN                  (0U)           /* GPT 0 DMA Request Enable */
#define BITL_DMAREQ_GPTREQEN_GPT_DMA_REQ0_EN                  (1U)           /* GPT 0 DMA Request Enable */
#define BITM_DMAREQ_GPTREQEN_GPT_DMA_REQ0_EN                  (0X01U)        /* GPT 0 DMA Request Enable */
#define BITP_DMAREQ_GPTREQEN_GPT_DMA_REQ1_EN                  (1U)           /* GPT 1 DMA Request Enable */
#define BITL_DMAREQ_GPTREQEN_GPT_DMA_REQ1_EN                  (1U)           /* GPT 1 DMA Request Enable */
#define BITM_DMAREQ_GPTREQEN_GPT_DMA_REQ1_EN                  (0X02U)        /* GPT 1 DMA Request Enable */
#define BITP_DMAREQ_GPTREQEN_GPT_DMA_REQ2_EN                  (2U)           /* GPT 2 DMA Request Enable */
#define BITL_DMAREQ_GPTREQEN_GPT_DMA_REQ2_EN                  (1U)           /* GPT 2 DMA Request Enable */
#define BITM_DMAREQ_GPTREQEN_GPT_DMA_REQ2_EN                  (0X04U)        /* GPT 2 DMA Request Enable */
#define BITP_DMAREQ_GPTREQEN_GPT32_DMA_REQ0_EN                (3U)           /* GPT32 0 DMA Request Enable */
#define BITL_DMAREQ_GPTREQEN_GPT32_DMA_REQ0_EN                (1U)           /* GPT32 0 DMA Request Enable */
#define BITM_DMAREQ_GPTREQEN_GPT32_DMA_REQ0_EN                (0X08U)        /* GPT32 0 DMA Request Enable */
#define BITP_DMAREQ_GPTREQEN_GPT32_DMA_REQ1_EN                (4U)           /* GPT32 1 DMA Request Enable */
#define BITL_DMAREQ_GPTREQEN_GPT32_DMA_REQ1_EN                (1U)           /* GPT32 1 DMA Request Enable */
#define BITM_DMAREQ_GPTREQEN_GPT32_DMA_REQ1_EN                (0X10U)        /* GPT32 1 DMA Request Enable */

/* ----------------------------------------------------------------------------------------------------
          GPT_MDA_REQ_TTYPE                                     Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_DMAREQ_GPT_MDA_REQ_TTYPE_GPT_REQ0_TYPE           (0U)           /* GPT Req0 DMA Require Type */
#define BITL_DMAREQ_GPT_MDA_REQ_TTYPE_GPT_REQ0_TYPE           (2U)           /* GPT Req0 DMA Require Type */
#define BITM_DMAREQ_GPT_MDA_REQ_TTYPE_GPT_REQ0_TYPE           (0X0003U)      /* GPT Req0 DMA Require Type */
#define BITP_DMAREQ_GPT_MDA_REQ_TTYPE_GPT_REQ1_TYPE           (2U)           /* GPT Req1 DMA Require Type */
#define BITL_DMAREQ_GPT_MDA_REQ_TTYPE_GPT_REQ1_TYPE           (2U)           /* GPT Req1 DMA Require Type */
#define BITM_DMAREQ_GPT_MDA_REQ_TTYPE_GPT_REQ1_TYPE           (0X000CU)      /* GPT Req1 DMA Require Type */
#define BITP_DMAREQ_GPT_MDA_REQ_TTYPE_GPT_REQ2_TYPE           (4U)           /* GPT Req2 DMA Require Type */
#define BITL_DMAREQ_GPT_MDA_REQ_TTYPE_GPT_REQ2_TYPE           (2U)           /* GPT Req2 DMA Require Type */
#define BITM_DMAREQ_GPT_MDA_REQ_TTYPE_GPT_REQ2_TYPE           (0X0030U)      /* GPT Req2 DMA Require Type */
#define BITP_DMAREQ_GPT_MDA_REQ_TTYPE_GPT32_REQ0_TYPE         (6U)           /* GPT32 Req0 DMA Require Type */
#define BITL_DMAREQ_GPT_MDA_REQ_TTYPE_GPT32_REQ0_TYPE         (2U)           /* GPT32 Req0 DMA Require Type */
#define BITM_DMAREQ_GPT_MDA_REQ_TTYPE_GPT32_REQ0_TYPE         (0X00C0U)      /* GPT32 Req0 DMA Require Type */
#define BITP_DMAREQ_GPT_MDA_REQ_TTYPE_GPT32_REQ1_TYPE         (8U)           /* GPT32 Req1 DMA Require Type */
#define BITL_DMAREQ_GPT_MDA_REQ_TTYPE_GPT32_REQ1_TYPE         (2U)           /* GPT32 Req1 DMA Require Type */
#define BITM_DMAREQ_GPT_MDA_REQ_TTYPE_GPT32_REQ1_TYPE         (0X0300U)      /* GPT32 Req1 DMA Require Type */

#define ENUM_DMAREQ_GPT_MDA_REQ_TTYPE_GPT32_REQ1_TYPE_HIGH_LEVEL (0X0000U)      /* High Level Trigger Interrupt and High Level Trigger DMA Request */
#define ENUM_DMAREQ_GPT_MDA_REQ_TTYPE_GPT32_REQ1_TYPE_RISING_EDGE (0X0001U)      /* Rising Edge Trigger Interrupt and Rising Edge Trigger DMA Request */
#define ENUM_DMAREQ_GPT_MDA_REQ_TTYPE_GPT32_REQ1_TYPE_LOW_LEVEL (0X0002U)      /* Low Level Trigger Interrupt and Low Level Trigger DMA Request */
#define ENUM_DMAREQ_GPT_MDA_REQ_TTYPE_GPT32_REQ1_TYPE_FALLING_EDGE (0X0003U)      /* Falling Edge Trigger Interrupt and Falling Edge Trigger DMA Request */
#define ENUM_DMAREQ_GPT_MDA_REQ_TTYPE_GPT32_REQ0_TYPE_HIGH_LEVEL (0X0000U)      /* High Level Trigger Interrupt and High Level Trigger DMA Request */
#define ENUM_DMAREQ_GPT_MDA_REQ_TTYPE_GPT32_REQ0_TYPE_RISING_EDGE (0X0001U)      /* Rising Edge Trigger Interrupt and Rising Edge Trigger DMA Request */
#define ENUM_DMAREQ_GPT_MDA_REQ_TTYPE_GPT32_REQ0_TYPE_LOW_LEVEL (0X0002U)      /* Low Level Trigger Interrupt and Low Level Trigger DMA Request */
#define ENUM_DMAREQ_GPT_MDA_REQ_TTYPE_GPT32_REQ0_TYPE_FALLING_EDGE (0X0003U)      /* Falling Edge Trigger Interrupt and Falling Edge Trigger DMA Request */
#define ENUM_DMAREQ_GPT_MDA_REQ_TTYPE_GPT_REQ2_TYPE_HIGH_LEVEL (0X0000U)      /* High Level Trigger Interrupt and High Level Trigger DMA Request */
#define ENUM_DMAREQ_GPT_MDA_REQ_TTYPE_GPT_REQ2_TYPE_RISING_EDGE (0X0001U)      /* Rising Edge Trigger Interrupt and Rising Edge Trigger DMA Request */
#define ENUM_DMAREQ_GPT_MDA_REQ_TTYPE_GPT_REQ2_TYPE_LOW_LEVEL (0X0002U)      /* Low Level Trigger Interrupt and Low Level Trigger DMA Request */
#define ENUM_DMAREQ_GPT_MDA_REQ_TTYPE_GPT_REQ2_TYPE_FALLING_EDGE (0X0003U)      /* Falling Edge Trigger Interrupt and Falling Edge Trigger DMA Request */
#define ENUM_DMAREQ_GPT_MDA_REQ_TTYPE_GPT_REQ1_TYPE_HIGH_LEVEL (0X0000U)      /* High Level Trigger Interrupt and High Level Trigger DMA Request */
#define ENUM_DMAREQ_GPT_MDA_REQ_TTYPE_GPT_REQ1_TYPE_RISING_EDGE (0X0001U)      /* Rising Edge Trigger Interrupt and Rising Edge Trigger DMA Request */
#define ENUM_DMAREQ_GPT_MDA_REQ_TTYPE_GPT_REQ1_TYPE_LOW_LEVEL (0X0002U)      /* Low Level Trigger Interrupt and Low Level Trigger DMA Request */
#define ENUM_DMAREQ_GPT_MDA_REQ_TTYPE_GPT_REQ1_TYPE_FALLING_EDGE (0X0003U)      /* Falling Edge Trigger Interrupt and Falling Edge Trigger DMA Request */
#define ENUM_DMAREQ_GPT_MDA_REQ_TTYPE_GPT_REQ0_TYPE_HIGH_LEVEL (0X0000U)      /* High Level Trigger Interrupt and High Level Trigger DMA Request */
#define ENUM_DMAREQ_GPT_MDA_REQ_TTYPE_GPT_REQ0_TYPE_RISING_EDGE (0X0001U)      /* Rising Edge Trigger Interrupt and Rising Edge Trigger DMA Request */
#define ENUM_DMAREQ_GPT_MDA_REQ_TTYPE_GPT_REQ0_TYPE_LOW_LEVEL (0X0002U)      /* Low Level Trigger Interrupt and Low Level Trigger DMA Request */
#define ENUM_DMAREQ_GPT_MDA_REQ_TTYPE_GPT_REQ0_TYPE_FALLING_EDGE (0X0003U)      /* Falling Edge Trigger Interrupt and Falling Edge Trigger DMA Request */

#endif  /* end ifndef DMAREQ_ADDR_RDEF_H_ */


#ifndef UART_ADDR_RDEF_H_
#define UART_ADDR_RDEF_H_    /* UART: Universal Asynchronous Receiver/Transmitter */

/* ====================================================================================================
        UART Module Instances Address and Mask Definitions
   ==================================================================================================== */
#define INST_UART0                                               (0X40020000U)    /* uart0: */
#define INST_UART1                                               (0X40020400U)    /* uart1: */

#define MASK_UART                                                (0X000000FFU)    /* UART: Universal Asynchronous Receiver/Transmitter */

/* ====================================================================================================
        UART Module Register Address Offset Definitions
   ==================================================================================================== */
#define IDX_UART_RX                                             (0X00U)    /* Receive/transfer Buffer Register */
#define IDX_UART_IEN                                            (0X04U)    /* Interrupt Enable */
#define IDX_UART_IIR                                            (0X08U)    /* Interrupt ID */
#define IDX_UART_LCR                                            (0X0CU)    /* Line Control */
#define IDX_UART_MCR                                            (0X10U)    /* Modem Control */
#define IDX_UART_LSR                                            (0X14U)    /* Line Status */
#define IDX_UART_MSR                                            (0X18U)    /* Modem Status */
#define IDX_UART_SCR                                            (0X1CU)    /* Scratch Buffer */
#define IDX_UART_FCR                                            (0X20U)    /* FIFO Control */
#define IDX_UART_FBR                                            (0X24U)    /* Fractional Baud Rate */
#define IDX_UART_DIV                                            (0X28U)    /* Baudrate Divider */
#define IDX_UART_LCR2                                           (0X2CU)    /* Second Line Control */
#define IDX_UART_CTL                                            (0X30U)    /* UART Control Register */
#define IDX_UART_RFC                                            (0X34U)    /* RX FIFO Byte Count */
#define IDX_UART_TFC                                            (0X38U)    /* TX FIFO Byte Count */
#define IDX_UART_RSC                                            (0X3CU)    /* RS485 Half-duplex Control */
#define IDX_UART_ACR                                            (0X40U)    /* Auto Baud Control */
#define IDX_UART_ASRL                                           (0X44U)    /* Auto Baud Status (Low) */
#define IDX_UART_ASRH                                           (0X48U)    /* Auto Baud Status (High) */

/* ====================================================================================================
        UART Module Register ResetValue Definitions
   ==================================================================================================== */
#define RSTVAL_UART_RX                                             (0X0) 
#define RSTVAL_UART_IEN                                            (0X0) 
#define RSTVAL_UART_IIR                                            (0X1) 
#define RSTVAL_UART_LCR                                            (0X0) 
#define RSTVAL_UART_MCR                                            (0X0) 
#define RSTVAL_UART_LSR                                            (0X60) 
#define RSTVAL_UART_SCR                                            (0X0) 
#define RSTVAL_UART_FCR                                            (0X0) 
#define RSTVAL_UART_FBR                                            (0X0) 
#define RSTVAL_UART_DIV                                            (0X0) 
#define RSTVAL_UART_LCR2                                           (0X2) 
#define RSTVAL_UART_CTL                                            (0X100) 
#define RSTVAL_UART_RFC                                            (0X0) 
#define RSTVAL_UART_TFC                                            (0X0) 
#define RSTVAL_UART_RSC                                            (0X0) 
#define RSTVAL_UART_ACR                                            (0X0) 
#define RSTVAL_UART_ASRL                                           (0X0) 
#define RSTVAL_UART_ASRH                                           (0X0) 

/* ====================================================================================================
        UART Module Register BitPositions, Lengths, Masks and Enumerations Definitions
   ==================================================================================================== */

/* ----------------------------------------------------------------------------------------------------
          RX                                                    Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_UART_RX_RBR                                      (0U)           /* Receive Buffer Register */
#define BITL_UART_RX_RBR                                      (8U)           /* Receive Buffer Register */
#define BITM_UART_RX_RBR                                      (0X00FFU)      /* Receive Buffer Register */

/* ----------------------------------------------------------------------------------------------------
          IEN                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_UART_IEN_ERBFI                                   (0U)           /* Receive Buffer Full Interrupt */
#define BITL_UART_IEN_ERBFI                                   (1U)           /* Receive Buffer Full Interrupt */
#define BITM_UART_IEN_ERBFI                                   (0X0001U)      /* Receive Buffer Full Interrupt */
#define BITP_UART_IEN_ETBEI                                   (1U)           /* Transmit Buffer Empty Interrupt */
#define BITL_UART_IEN_ETBEI                                   (1U)           /* Transmit Buffer Empty Interrupt */
#define BITM_UART_IEN_ETBEI                                   (0X0002U)      /* Transmit Buffer Empty Interrupt */
#define BITP_UART_IEN_ELSI                                    (2U)           /* Rx Status Interrupt */
#define BITL_UART_IEN_ELSI                                    (1U)           /* Rx Status Interrupt */
#define BITM_UART_IEN_ELSI                                    (0X0004U)      /* Rx Status Interrupt */
#define BITP_UART_IEN_EDSSI                                   (3U)           /* Modem Status Interrupt */
#define BITL_UART_IEN_EDSSI                                   (1U)           /* Modem Status Interrupt */
#define BITM_UART_IEN_EDSSI                                   (0X0008U)      /* Modem Status Interrupt */
#define BITP_UART_IEN_EDMAT                                   (4U)           /* DMA Requests in Transmit Mode */
#define BITL_UART_IEN_EDMAT                                   (1U)           /* DMA Requests in Transmit Mode */
#define BITM_UART_IEN_EDMAT                                   (0X0010U)      /* DMA Requests in Transmit Mode */
#define BITP_UART_IEN_EDMAR                                   (5U)           /* DMA Requests in Receive Mode */
#define BITL_UART_IEN_EDMAR                                   (1U)           /* DMA Requests in Receive Mode */
#define BITM_UART_IEN_EDMAR                                   (0X0020U)      /* DMA Requests in Receive Mode */

#define ENUM_UART_IEN_EDMAR_DIS                               (0X0000U)      /* DMA Requests Disabled */
#define ENUM_UART_IEN_EDMAR_EN                                (0X0001U)      /* DMA Requests Enabled */
#define ENUM_UART_IEN_EDMAT_DIS                               (0X0000U)      /* DMA Requests are Disabled */
#define ENUM_UART_IEN_EDMAT_EN                                (0X0001U)      /* DMA Requests are Enabled */
#define ENUM_UART_IEN_EDSSI_DIS                               (0X0000U)      /* Interrupt Disabled */
#define ENUM_UART_IEN_EDSSI_EN                                (0X0001U)      /* Interrupt Enabled */
#define ENUM_UART_IEN_ELSI_DIS                                (0X0000U)      /* Interrupt Disabled */
#define ENUM_UART_IEN_ELSI_EN                                 (0X0001U)      /* Interrupt Enabled */
#define ENUM_UART_IEN_ETBEI_DIS                               (0X0000U)      /* Interrupt Disabled */
#define ENUM_UART_IEN_ETBEI_EN                                (0X0001U)      /* Interrupt Enabled */
#define ENUM_UART_IEN_ERBFI_DIS                               (0X0000U)      /* Interrupt Disabled */
#define ENUM_UART_IEN_ERBFI_EN                                (0X0001U)      /* Interrupt Enabled */

/* ----------------------------------------------------------------------------------------------------
          IIR                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_UART_IIR_NIRQ                                    (0U)           /* Interrupt Flag */
#define BITL_UART_IIR_NIRQ                                    (1U)           /* Interrupt Flag */
#define BITM_UART_IIR_NIRQ                                    (0X0001U)      /* Interrupt Flag */
#define BITP_UART_IIR_STAT                                    (1U)           /* Interrupt Status */
#define BITL_UART_IIR_STAT                                    (3U)           /* Interrupt Status */
#define BITM_UART_IIR_STAT                                    (0X000EU)      /* Interrupt Status */
#define BITP_UART_IIR_FEND                                    (6U)           /* FIFO Enabled */
#define BITL_UART_IIR_FEND                                    (2U)           /* FIFO Enabled */
#define BITM_UART_IIR_FEND                                    (0X00C0U)      /* FIFO Enabled */

#define ENUM_UART_IIR_FEND_DIS                                (0X0000U)      /* FIFO Not Enabled, 16450 Mode */
#define ENUM_UART_IIR_FEND_EN                                 (0X0003U)      /* FIFO Enabled, 16550 Mode */
#define ENUM_UART_IIR_STAT_MODEMINT                           (0X0000U)      /* Modem Status Interrupt (Read MSR Register to Clear) */
#define ENUM_UART_IIR_STAT_TXEMPTYINT                         (0X0001U)      /* Transmit Buffer Empty Interrupt (Write to Tx Register or Read IIR Register to Clear) */
#define ENUM_UART_IIR_STAT_RXFULLINT                          (0X0002U)      /* Receive Buffer Full Interrupt (Read Rx Register to Clear) */
#define ENUM_UART_IIR_STAT_RXTIMEOUTINT                       (0X0006U)      /* Receive FIFO Time-out Interrupt (Read Rx Register to Clear) */
#define ENUM_UART_IIR_STAT_RXLINEINT                          (0X0003U)      /* Receive Line Status Interrupt (Read LSR Register to Clear) */

/* ----------------------------------------------------------------------------------------------------
          LCR                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_UART_LCR_WLS                                     (0U)           /* Word Length Select */
#define BITL_UART_LCR_WLS                                     (2U)           /* Word Length Select */
#define BITM_UART_LCR_WLS                                     (0X0003U)      /* Word Length Select */
#define BITP_UART_LCR_STOPPED                                 (2U)           /* Stop Bit */
#define BITL_UART_LCR_STOPPED                                 (1U)           /* Stop Bit */
#define BITM_UART_LCR_STOPPED                                 (0X0004U)      /* Stop Bit */
#define BITP_UART_LCR_PEN                                     (3U)           /* Parity Enable */
#define BITL_UART_LCR_PEN                                     (1U)           /* Parity Enable */
#define BITM_UART_LCR_PEN                                     (0X0008U)      /* Parity Enable */
#define BITP_UART_LCR_EPS                                     (4U)           /* Parity Select */
#define BITL_UART_LCR_EPS                                     (1U)           /* Parity Select */
#define BITM_UART_LCR_EPS                                     (0X0010U)      /* Parity Select */
#define BITP_UART_LCR_SP                                      (5U)           /* Stick Parity */
#define BITL_UART_LCR_SP                                      (1U)           /* Stick Parity */
#define BITM_UART_LCR_SP                                      (0X0020U)      /* Stick Parity */
#define BITP_UART_LCR_BRK                                     (6U)           /* Set Break */
#define BITL_UART_LCR_BRK                                     (1U)           /* Set Break */
#define BITM_UART_LCR_BRK                                     (0X0040U)      /* Set Break */

#define ENUM_UART_LCR_SP_STICK                                (0X0000U)      /* Parity Will Not Be Forced Based on Parity Select and Parity Enable Bits. */
#define ENUM_UART_LCR_SP_EPSPEN                               (0X0001U)      /* Parity Forced Based on Parity Select and Parity Enable Bits. */
#define ENUM_UART_LCR_EPS_ODD                                 (0X0000U)      /* Odd Parity Will Be Transmitted and Checked. */
#define ENUM_UART_LCR_EPS_EVEN                                (0X0001U)      /* Even Parity Will Be Transmitted and Checked. */
#define ENUM_UART_LCR_PEN_DIS                                 (0X0000U)      /* Parity Will Not Be Transmitted or Checked. */
#define ENUM_UART_LCR_PEN_EN                                  (0X0001U)      /* Parity Will Be Transmitted and Checked. */
#define ENUM_UART_LCR_STOPPED_ONTBIT                          (0X0000U)      /* Send 1 Stop Bit Regardless of the Word Length Select Bit */
#define ENUM_UART_LCR_STOPPED_MULTIBIT                        (0X0001U)      /* Send a Number of Stop Bits Based on the Word Length as Follows: WLS = 00, 1.5 Stop Bits Transmitted (5-bit Word Length) WLS = 01 or 10 or 11, 2 Stop Bits Transmitted (6 or 7 or 8-bit Word Length) */
#define ENUM_UART_LCR_WLS_BITS5                               (0X0000U)      /* 5 Bits */
#define ENUM_UART_LCR_WLS_BITS6                               (0X0001U)      /* 6 Bits */
#define ENUM_UART_LCR_WLS_BITS7                               (0X0002U)      /* 7 Bits */
#define ENUM_UART_LCR_WLS_BITS8                               (0X0003U)      /* 8 Bits */

/* ----------------------------------------------------------------------------------------------------
          MCR                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_UART_MCR_DTR                                     (0U)           /* Data Terminal Ready */
#define BITL_UART_MCR_DTR                                     (1U)           /* Data Terminal Ready */
#define BITM_UART_MCR_DTR                                     (0X0001U)      /* Data Terminal Ready */
#define BITP_UART_MCR_RTS                                     (1U)           /* Request to Send */
#define BITL_UART_MCR_RTS                                     (1U)           /* Request to Send */
#define BITM_UART_MCR_RTS                                     (0X0002U)      /* Request to Send */
#define BITP_UART_MCR_OUT1                                    (2U)           /* Output 1 */
#define BITL_UART_MCR_OUT1                                    (1U)           /* Output 1 */
#define BITM_UART_MCR_OUT1                                    (0X0004U)      /* Output 1 */
#define BITP_UART_MCR_OUT2                                    (3U)           /* Output 2 */
#define BITL_UART_MCR_OUT2                                    (1U)           /* Output 2 */
#define BITM_UART_MCR_OUT2                                    (0X0008U)      /* Output 2 */
#define BITP_UART_MCR_LOOPBACK                                (4U)           /* Loop Back Mode */
#define BITL_UART_MCR_LOOPBACK                                (1U)           /* Loop Back Mode */
#define BITM_UART_MCR_LOOPBACK                                (0X0010U)      /* Loop Back Mode */

#define ENUM_UART_MCR_LOOPBACK_DIS                            (0X0000U)      /* Normal Operation - Loopback Disabled */
#define ENUM_UART_MCR_LOOPBACK_EN                             (0X0001U)      /* Loopback Enabled */
#define ENUM_UART_MCR_OUT2_FORCE1                             (0X0000U)      /* Force NOUT2 to a Logic 1 */
#define ENUM_UART_MCR_OUT2_FORCE0                             (0X0001U)      /* Force NOUT2 to a Logic 0 */
#define ENUM_UART_MCR_OUT1_FORCE1                             (0X0000U)      /* Force NOUT1 to a Logic 1 */
#define ENUM_UART_MCR_OUT1_FORCE0                             (0X0001U)      /* Force NOUT1 to a Logic 0 */
#define ENUM_UART_MCR_RTS_FORCE1                              (0X0000U)      /* Force NRTS to a Logic 1 */
#define ENUM_UART_MCR_RTS_FORCE0                              (0X0001U)      /* Force NRTS to a Logic 0 */
#define ENUM_UART_MCR_DTR_FORCE1                              (0X0000U)      /* Force NDTR to a Logic 1 */
#define ENUM_UART_MCR_DTR_FORCE0                              (0X0001U)      /* Force NDTR to a Logic 0 */

/* ----------------------------------------------------------------------------------------------------
          LSR                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_UART_LSR_DR                                      (0U)           /* Data Ready */
#define BITL_UART_LSR_DR                                      (1U)           /* Data Ready */
#define BITM_UART_LSR_DR                                      (0X0001U)      /* Data Ready */
#define BITP_UART_LSR_OE                                      (1U)           /* Overrun Error */
#define BITL_UART_LSR_OE                                      (1U)           /* Overrun Error */
#define BITM_UART_LSR_OE                                      (0X0002U)      /* Overrun Error */
#define BITP_UART_LSR_PE                                      (2U)           /* Parity Error */
#define BITL_UART_LSR_PE                                      (1U)           /* Parity Error */
#define BITM_UART_LSR_PE                                      (0X0004U)      /* Parity Error */
#define BITP_UART_LSR_FE                                      (3U)           /* Framing Error */
#define BITL_UART_LSR_FE                                      (1U)           /* Framing Error */
#define BITM_UART_LSR_FE                                      (0X0008U)      /* Framing Error */
#define BITP_UART_LSR_BI                                      (4U)           /* Break Indicator */
#define BITL_UART_LSR_BI                                      (1U)           /* Break Indicator */
#define BITM_UART_LSR_BI                                      (0X0010U)      /* Break Indicator */
#define BITP_UART_LSR_THRE                                    (5U)           /* Transmit Register Empty */
#define BITL_UART_LSR_THRE                                    (1U)           /* Transmit Register Empty */
#define BITM_UART_LSR_THRE                                    (0X0020U)      /* Transmit Register Empty */
#define BITP_UART_LSR_TEMT                                    (6U)           /* Transmit and Shift Register Empty Status */
#define BITL_UART_LSR_TEMT                                    (1U)           /* Transmit and Shift Register Empty Status */
#define BITM_UART_LSR_TEMT                                    (0X0040U)      /* Transmit and Shift Register Empty Status */
#define BITP_UART_LSR_FIFOERR                                 (7U)           /* FIFO Error */
#define BITL_UART_LSR_FIFOERR                                 (1U)           /* FIFO Error */
#define BITM_UART_LSR_FIFOERR                                 (0X0080U)      /* FIFO Error */

#define ENUM_UART_LSR_TEMT_NOTEMPTY                           (0X0000U)      /* Tx Register Has Been Written to and Contains Data to Be Transmitted. Care Should Be Taken Not to Overwrite Its Value. */
#define ENUM_UART_LSR_TEMT_EMPTY                              (0X0001U)      /* Tx Register and the Transmit Shift Register are Empty and It is Safe to Write New Data to the Tx Register. Data Has Been Transmitted. */
#define ENUM_UART_LSR_THRE_NOTEMPTY                           (0X0000U)      /* Tx Register Has Been Written to and Contains Data to Be Transmitted. Care Should Be Taken Not to Overwrite Its Value. */
#define ENUM_UART_LSR_THRE_EMPTY                              (0X0001U)      /* Tx Register is Empty and It is Safe to Write New Data to Tx Register the Previous Data May Not Have Been Transmitted Yet and Can Still Be Present in the Shift Register. */
#define ENUM_UART_LSR_FE_OKAY                                 (0X0000U)      /* No Invalid Stop Bit Was Detected. */
#define ENUM_UART_LSR_FE_INVALIDSTOP                          (0X0001U)      /* An Invalid Stop Bit Was Detected on a Received Word. */
#define ENUM_UART_LSR_PE_NOERR                                (0X0000U)      /* No Parity Error Was Detected. */
#define ENUM_UART_LSR_PE_ERRORS                               (0X0001U)      /* A Parity Error Occurred on a Received Word. */
#define ENUM_UART_LSR_OE_OKAY                                 (0X0000U)      /* Receive Data Has Not Been Overwritten. */
#define ENUM_UART_LSR_OE_OVERWRITTEN                          (0X0001U)      /* Receive Data Was Overwritten by New Data Before Rx Register Was Read. */
#define ENUM_UART_LSR_DR_NOTREADY                             (0X0000U)      /* Rx Register Does Not Contain New Receive Data. */
#define ENUM_UART_LSR_DR_READY                                (0X0001U)      /* Rx Register Contains Receive Data That Should Be Read. */

/* ----------------------------------------------------------------------------------------------------
          MSR                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_UART_MSR_DCTS                                    (0U)           /* Delta CTS */
#define BITL_UART_MSR_DCTS                                    (1U)           /* Delta CTS */
#define BITM_UART_MSR_DCTS                                    (0X0001U)      /* Delta CTS */
#define BITP_UART_MSR_DDSR                                    (1U)           /* Delta DSR */
#define BITL_UART_MSR_DDSR                                    (1U)           /* Delta DSR */
#define BITM_UART_MSR_DDSR                                    (0X0002U)      /* Delta DSR */
#define BITP_UART_MSR_TERI                                    (2U)           /* Trailing Edge RI */
#define BITL_UART_MSR_TERI                                    (1U)           /* Trailing Edge RI */
#define BITM_UART_MSR_TERI                                    (0X0004U)      /* Trailing Edge RI */
#define BITP_UART_MSR_DDCD                                    (3U)           /* Delta DCD */
#define BITL_UART_MSR_DDCD                                    (1U)           /* Delta DCD */
#define BITM_UART_MSR_DDCD                                    (0X0008U)      /* Delta DCD */
#define BITP_UART_MSR_CTS                                     (4U)           /* Clear to Send */
#define BITL_UART_MSR_CTS                                     (1U)           /* Clear to Send */
#define BITM_UART_MSR_CTS                                     (0X0010U)      /* Clear to Send */
#define BITP_UART_MSR_DSR                                     (5U)           /* Data Set Ready */
#define BITL_UART_MSR_DSR                                     (1U)           /* Data Set Ready */
#define BITM_UART_MSR_DSR                                     (0X0020U)      /* Data Set Ready */
#define BITP_UART_MSR_RI                                      (6U)           /* Ring Indicator */
#define BITL_UART_MSR_RI                                      (1U)           /* Ring Indicator */
#define BITM_UART_MSR_RI                                      (0X0040U)      /* Ring Indicator */
#define BITP_UART_MSR_DCD                                     (7U)           /* Data Carrier Detect */
#define BITL_UART_MSR_DCD                                     (1U)           /* Data Carrier Detect */
#define BITM_UART_MSR_DCD                                     (0X0080U)      /* Data Carrier Detect */

#define ENUM_UART_MSR_DCD_HIGH                                (0X0000U)      /* NDCD is Currently Logic High. */
#define ENUM_UART_MSR_DCD_LOW                                 (0X0001U)      /* NDCD is Currently Logic Low. */
#define ENUM_UART_MSR_RI_HIGH                                 (0X0000U)      /* NRI is Currently Logic High. */
#define ENUM_UART_MSR_RI_LOW                                  (0X0001U)      /* NRI is Currently Logic Low. */
#define ENUM_UART_MSR_DSR_HIGH                                (0X0000U)      /* NDSR is Currently Logic High */
#define ENUM_UART_MSR_DSR_LOW                                 (0X0001U)      /* NDSR is Currently Logic Low */
#define ENUM_UART_MSR_CTS_HIGH                                (0X0000U)      /* NCTS is Currently Logic High */
#define ENUM_UART_MSR_CTS_LOW                                 (0X0001U)      /* NCTS is Currently Logic Low */
#define ENUM_UART_MSR_DDCD_NOCHANGE                           (0X0000U)      /* Data Carrier Detect Bit Has Not Changed State Since MSR Register Was Last Read */
#define ENUM_UART_MSR_DDCD_CHANGED                            (0X0001U)      /* Data Carrier Detect Bit Changed State Since MSR Register Last Read */
#define ENUM_UART_MSR_TERI_NOTCHANGED                         (0X0000U)      /* Ring Indicator Bit Has Not Changed from 0 to 1 Since MSR Register Last Read */
#define ENUM_UART_MSR_TERI_CHANGED                            (0X0001U)      /* Ring Indicator Bit Changed from 0 to 1 Since MSR Register Last Read */
#define ENUM_UART_MSR_DDSR_NOCHANGE                           (0X0000U)      /* Data Set Ready Bit Has Not Changed State Since MSR Register Was Last Read */
#define ENUM_UART_MSR_DDSR_CHANGED                            (0X0001U)      /* Data Set Ready Bit Changed State Since MSR Register Last Read */
#define ENUM_UART_MSR_DCTS_NOCHANGE                           (0X0000U)      /* Clear to Send Bit Has Not Changed State Since MSR Register Was Last Read */
#define ENUM_UART_MSR_DCTS_CHANGED                            (0X0001U)      /* Clear to Send Bit Changed State Since MSR Register Last Read */

/* ----------------------------------------------------------------------------------------------------
          SCR                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_UART_SCR_SCR                                     (0U)           /* Scratch */
#define BITL_UART_SCR_SCR                                     (8U)           /* Scratch */
#define BITM_UART_SCR_SCR                                     (0X00FFU)      /* Scratch */

/* ----------------------------------------------------------------------------------------------------
          FCR                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_UART_FCR_FIFOEN                                  (0U)           /* FIFO Enable as to Work in 16550 Mode */
#define BITL_UART_FCR_FIFOEN                                  (1U)           /* FIFO Enable as to Work in 16550 Mode */
#define BITM_UART_FCR_FIFOEN                                  (0X0001U)      /* FIFO Enable as to Work in 16550 Mode */
#define BITP_UART_FCR_RFCLR                                   (1U)           /* Clear RX FIFO */
#define BITL_UART_FCR_RFCLR                                   (1U)           /* Clear RX FIFO */
#define BITM_UART_FCR_RFCLR                                   (0X0002U)      /* Clear RX FIFO */
#define BITP_UART_FCR_TFCLR                                   (2U)           /* Clear TX FIFO */
#define BITL_UART_FCR_TFCLR                                   (1U)           /* Clear TX FIFO */
#define BITM_UART_FCR_TFCLR                                   (0X0004U)      /* Clear TX FIFO */
#define BITP_UART_FCR_FDMAMD                                  (3U)           /* FIFO DMA Mode */
#define BITL_UART_FCR_FDMAMD                                  (1U)           /* FIFO DMA Mode */
#define BITM_UART_FCR_FDMAMD                                  (0X0008U)      /* FIFO DMA Mode */
#define BITP_UART_FCR_RFTRIG                                  (6U)           /* RX FIFO Trig Level */
#define BITL_UART_FCR_RFTRIG                                  (2U)           /* RX FIFO Trig Level */
#define BITM_UART_FCR_RFTRIG                                  (0X00C0U)      /* RX FIFO Trig Level */

#define ENUM_UART_FCR_RFTRIG_BYTE1                            (0X0000U)      /* 1 Byte to Trig RX Interrupt */
#define ENUM_UART_FCR_RFTRIG_BYTE4                            (0X0001U)      /* 4 Byte to Trig RX Interrupt */
#define ENUM_UART_FCR_RFTRIG_BYTE8                            (0X0002U)      /* 8 Byte to Trig RX Interrupt */
#define ENUM_UART_FCR_RFTRIG_BYTE14                           (0X0003U)      /* 14 Byte to Trig RX Interrupt */
#define ENUM_UART_FCR_FDMAMD_MODE0                            (0X0000U)      /* In DMA Mode 0, RX DMA Request Will Be Asserted Whenever There's Data in RBR or RX FIFO and De-assert Whenever RBR or RX FIFO is Empty; TX DMA Request Will Be Asserted Whenever THR or TX FIFO is Empty and De-assert Whenever Data Written To. */
#define ENUM_UART_FCR_FDMAMD_MODE1                            (0X0001U)      /* In DMA Mode 1, RX DMA Request Will Be Asserted Whenever RX FIFO Trig Level or Time Out Reached and De-assert Thereafter When RX FIFO is Empty; TX DMA Request Will Be Asserted Whenever TX FIFO is Empty and De-assert Thereafter When TX FIFO is Completely Filled up Full. */

/* ----------------------------------------------------------------------------------------------------
          FBR                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_UART_FBR_DIVN                                    (0U)           /* Fractional Baud Rate N Divide Bits 0 to 2047 */
#define BITL_UART_FBR_DIVN                                    (11U)          /* Fractional Baud Rate N Divide Bits 0 to 2047 */
#define BITM_UART_FBR_DIVN                                    (0X07FFU)      /* Fractional Baud Rate N Divide Bits 0 to 2047 */
#define BITP_UART_FBR_DIVM                                    (11U)          /* Fractional Baud Rate M Divide Bits 1 to 3 */
#define BITL_UART_FBR_DIVM                                    (2U)           /* Fractional Baud Rate M Divide Bits 1 to 3 */
#define BITM_UART_FBR_DIVM                                    (0X1800U)      /* Fractional Baud Rate M Divide Bits 1 to 3 */
#define BITP_UART_FBR_FBEN                                    (15U)          /* Fractional Baud Rate Generator Enable */
#define BITL_UART_FBR_FBEN                                    (1U)           /* Fractional Baud Rate Generator Enable */
#define BITM_UART_FBR_FBEN                                    (0X8000U)      /* Fractional Baud Rate Generator Enable */

/* ----------------------------------------------------------------------------------------------------
          DIV                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_UART_DIV_DIV                                     (0U)           /* Baud Rate Divider */
#define BITL_UART_DIV_DIV                                     (16U)          /* Baud Rate Divider */
#define BITM_UART_DIV_DIV                                     (0XFFFFU)      /* Baud Rate Divider */

/* ----------------------------------------------------------------------------------------------------
          LCR2                                                  Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_UART_LCR2_OSR                                    (0U)           /* Over Sample Rate */
#define BITL_UART_LCR2_OSR                                    (2U)           /* Over Sample Rate */
#define BITM_UART_LCR2_OSR                                    (0X0003U)      /* Over Sample Rate */

#define ENUM_UART_LCR2_OSR_OSR4                               (0X0000U)      /* Over Sample by 4 */
#define ENUM_UART_LCR2_OSR_OSR8                               (0X0001U)      /* Over Sample by 8 */
#define ENUM_UART_LCR2_OSR_OSR16                              (0X0002U)      /* Over Sample by 16 */
#define ENUM_UART_LCR2_OSR_OSR32                              (0X0003U)      /* Over Sample by 32 */

/* ----------------------------------------------------------------------------------------------------
          CTL                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_UART_CTL_FORCECLK                                (1U)           /* Force UCLK on */
#define BITL_UART_CTL_FORCECLK                                (1U)           /* Force UCLK on */
#define BITM_UART_CTL_FORCECLK                                (0X0002U)      /* Force UCLK on */
#define BITP_UART_CTL_RXINV                                   (4U)           /* Invert Receiver Line */
#define BITL_UART_CTL_RXINV                                   (1U)           /* Invert Receiver Line */
#define BITM_UART_CTL_RXINV                                   (0X0010U)      /* Invert Receiver Line */
#define BITP_UART_CTL_REV                                     (8U)           /* UART Revision ID */
#define BITL_UART_CTL_REV                                     (8U)           /* UART Revision ID */
#define BITM_UART_CTL_REV                                     (0XFF00U)      /* UART Revision ID */

#define ENUM_UART_CTL_RXINV_EN000                             (0X0000U)      /* Don't Invert Receiver Line (idling High) */
#define ENUM_UART_CTL_RXINV_EN001                             (0X0001U)      /* Invert Receiver Line (idling Low) */
#define ENUM_UART_CTL_FORCECLK_GATEUCLK                       (0X0000U)      /* UCLK Automatically Gated */
#define ENUM_UART_CTL_FORCECLK_UCLKEN                         (0X0001U)      /* UCLK Always Working */

/* ----------------------------------------------------------------------------------------------------
          RFC                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_UART_RFC_RFC                                     (0U)           /* Current RX FIFO Data Bytes */
#define BITL_UART_RFC_RFC                                     (5U)           /* Current RX FIFO Data Bytes */
#define BITM_UART_RFC_RFC                                     (0X001FU)      /* Current RX FIFO Data Bytes */

/* ----------------------------------------------------------------------------------------------------
          TFC                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_UART_TFC_TFC                                     (0U)           /* Current TX FIFO Data Bytes */
#define BITL_UART_TFC_TFC                                     (5U)           /* Current TX FIFO Data Bytes */
#define BITM_UART_TFC_TFC                                     (0X001FU)      /* Current TX FIFO Data Bytes */

/* ----------------------------------------------------------------------------------------------------
          RSC                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_UART_RSC_OENP                                    (0U)           /* SOUT_EN Polarity */
#define BITL_UART_RSC_OENP                                    (1U)           /* SOUT_EN Polarity */
#define BITM_UART_RSC_OENP                                    (0X0001U)      /* SOUT_EN Polarity */
#define BITP_UART_RSC_OENSP                                   (1U)           /* SOUT_EN De-assert Before Full Stop Bit(s) */
#define BITL_UART_RSC_OENSP                                   (1U)           /* SOUT_EN De-assert Before Full Stop Bit(s) */
#define BITM_UART_RSC_OENSP                                   (0X0002U)      /* SOUT_EN De-assert Before Full Stop Bit(s) */
#define BITP_UART_RSC_DISRX                                   (2U)           /* Disable RX When Transmitting */
#define BITL_UART_RSC_DISRX                                   (1U)           /* Disable RX When Transmitting */
#define BITM_UART_RSC_DISRX                                   (0X0004U)      /* Disable RX When Transmitting */
#define BITP_UART_RSC_DISTX                                   (3U)           /* Hold off TX When Receiving */
#define BITL_UART_RSC_DISTX                                   (1U)           /* Hold off TX When Receiving */
#define BITM_UART_RSC_DISTX                                   (0X0008U)      /* Hold off TX When Receiving */

#define ENUM_UART_RSC_OENSP_FULLBIT                           (0X0000U)      /* SOUT_EN De-assert Same Time as Full Stop Bit(s) */
#define ENUM_UART_RSC_OENSP_HALFBIT                           (0X0001U)      /* SOUT_EN De-assert Half-bit Earlier Than Full Stop Bit(s) */
#define ENUM_UART_RSC_OENP_HIGHACTIVE                         (0X0000U)      /* High Active */
#define ENUM_UART_RSC_OENP_LOWACTIVE                          (0X0001U)      /* Low Active */

/* ----------------------------------------------------------------------------------------------------
          ACR                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_UART_ACR_ABE                                     (0U)           /* Auto Baud Enable */
#define BITL_UART_ACR_ABE                                     (1U)           /* Auto Baud Enable */
#define BITM_UART_ACR_ABE                                     (0X0001U)      /* Auto Baud Enable */
#define BITP_UART_ACR_DNIEN                                   (1U)           /* Enable Done Interrupt */
#define BITL_UART_ACR_DNIEN                                   (1U)           /* Enable Done Interrupt */
#define BITM_UART_ACR_DNIEN                                   (0X0002U)      /* Enable Done Interrupt */
#define BITP_UART_ACR_TOIEN                                   (2U)           /* Enable Time-out Interrupt */
#define BITL_UART_ACR_TOIEN                                   (1U)           /* Enable Time-out Interrupt */
#define BITM_UART_ACR_TOIEN                                   (0X0004U)      /* Enable Time-out Interrupt */
#define BITP_UART_ACR_SEC                                     (4U)           /* Starting Edge Count */
#define BITL_UART_ACR_SEC                                     (3U)           /* Starting Edge Count */
#define BITM_UART_ACR_SEC                                     (0X0070U)      /* Starting Edge Count */
#define BITP_UART_ACR_EEC                                     (8U)           /* Ending Edge Count */
#define BITL_UART_ACR_EEC                                     (4U)           /* Ending Edge Count */
#define BITM_UART_ACR_EEC                                     (0X0F00U)      /* Ending Edge Count */

#define ENUM_UART_ACR_EEC_FIRST                               (0X0000U)      /* First Edge */
#define ENUM_UART_ACR_EEC_SECOND                              (0X0001U)      /* Second Edge */
#define ENUM_UART_ACR_EEC_THIRD                               (0X0002U)      /* Third Edge */
#define ENUM_UART_ACR_EEC_FOURTH                              (0X0003U)      /* Fourth Edge */
#define ENUM_UART_ACR_EEC_FIFTH                               (0X0004U)      /* Fifth Edge */
#define ENUM_UART_ACR_EEC_SIXTH                               (0X0005U)      /* Sixth Edge */
#define ENUM_UART_ACR_EEC_SEVENTH                             (0X0006U)      /* Seventh Edge */
#define ENUM_UART_ACR_EEC_EIGHTH                              (0X0007U)      /* Eighth Edge */
#define ENUM_UART_ACR_EEC_NINTH                               (0X0008U)      /* Ninth Edge */
#define ENUM_UART_ACR_SEC_FIRST                               (0X0000U)      /* First Edge (always the Falling Edge of START Bit) */
#define ENUM_UART_ACR_SEC_SECOND                              (0X0001U)      /* Second Edge */
#define ENUM_UART_ACR_SEC_THIRD                               (0X0002U)      /* Third Edge */
#define ENUM_UART_ACR_SEC_FOURTH                              (0X0003U)      /* Fourth Edge */
#define ENUM_UART_ACR_SEC_FIFTH                               (0X0004U)      /* Fifth Edge */
#define ENUM_UART_ACR_SEC_SIXTH                               (0X0005U)      /* Sixth Edge */
#define ENUM_UART_ACR_SEC_SEVENTH                             (0X0006U)      /* Seventh Edge */
#define ENUM_UART_ACR_SEC_EIGHTH                              (0X0007U)      /* Eighth Edge */

/* ----------------------------------------------------------------------------------------------------
          ASRL                                                  Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_UART_ASRL_DONE                                   (0U)           /* Auto Baud Done Successfully */
#define BITL_UART_ASRL_DONE                                   (1U)           /* Auto Baud Done Successfully */
#define BITM_UART_ASRL_DONE                                   (0X0001U)      /* Auto Baud Done Successfully */
#define BITP_UART_ASRL_BRKTO                                  (1U)           /* Timed Out Due to Long Time Break Condition */
#define BITL_UART_ASRL_BRKTO                                  (1U)           /* Timed Out Due to Long Time Break Condition */
#define BITM_UART_ASRL_BRKTO                                  (0X0002U)      /* Timed Out Due to Long Time Break Condition */
#define BITP_UART_ASRL_NSETO                                  (2U)           /* Timed Out Due to No Valid Start Edge Found */
#define BITL_UART_ASRL_NSETO                                  (1U)           /* Timed Out Due to No Valid Start Edge Found */
#define BITM_UART_ASRL_NSETO                                  (0X0004U)      /* Timed Out Due to No Valid Start Edge Found */
#define BITP_UART_ASRL_NEETO                                  (3U)           /* Timed Out Due to No Valid Ending Edge Found */
#define BITL_UART_ASRL_NEETO                                  (1U)           /* Timed Out Due to No Valid Ending Edge Found */
#define BITM_UART_ASRL_NEETO                                  (0X0008U)      /* Timed Out Due to No Valid Ending Edge Found */
#define BITP_UART_ASRL_CNT                                    (4U)           /* Auto Baud Counter Value */
#define BITL_UART_ASRL_CNT                                    (12U)          /* Auto Baud Counter Value */
#define BITM_UART_ASRL_CNT                                    (0XFFF0U)      /* Auto Baud Counter Value */

/* ----------------------------------------------------------------------------------------------------
          ASRH                                                  Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_UART_ASRH_CNT                                    (0U)           /* Auto Baud Counter Value */
#define BITL_UART_ASRH_CNT                                    (8U)           /* Auto Baud Counter Value */
#define BITM_UART_ASRH_CNT                                    (0X00FFU)      /* Auto Baud Counter Value */

#endif  /* end ifndef UART_ADDR_RDEF_H_ */


#ifndef I2C_ADDR_RDEF_H_
#define I2C_ADDR_RDEF_H_    /* I2C: I2C Master/Slave */

/* ====================================================================================================
        I2C Module Instances Address and Mask Definitions
   ==================================================================================================== */
#define INST_I2C0                                                (0X40020800U)    /* i2c0: */
#define INST_I2C1                                                (0X40020C00U)    /* i2c1: */
#define INST_I2C2                                                (0X40021000U)    /* i2c2: */

#define MASK_I2C                                                 (0X0000007FU)    /* I2C: I2C Master/Slave */

/* ====================================================================================================
        I2C Module Register Address Offset Definitions
   ==================================================================================================== */
#define IDX_I2C_MCTL                                            (0X00U)    /* Master Control */
#define IDX_I2C_MSTAT                                           (0X04U)    /* Master Status */
#define IDX_I2C_MRX                                             (0X08U)    /* Master Receive Data */
#define IDX_I2C_MTX                                             (0X0CU)    /* Master Transmit Data */
#define IDX_I2C_MRXCNT                                          (0X10U)    /* Master Receive Data Count */
#define IDX_I2C_MCRXCNT                                         (0X14U)    /* Master Current Receive Data Count */
#define IDX_I2C_ADDR0                                           (0X18U)    /* 1st Master Address Byte */
#define IDX_I2C_ADDR1                                           (0X1CU)    /* 2nd Master Address Byte */
#define IDX_I2C_BYT                                             (0X20U)    /* Start Byte */
#define IDX_I2C_DIV                                             (0X24U)    /* Serial Clock Period Divisor */
#define IDX_I2C_SCTL                                            (0X28U)    /* Slave Control */
#define IDX_I2C_SSTAT                                           (0X2CU)    /* Slave I2C Status/Error/IRQ */
#define IDX_I2C_SRX                                             (0X30U)    /* Slave Receive */
#define IDX_I2C_STX                                             (0X34U)    /* Slave Transmit */
#define IDX_I2C_ALT                                             (0X38U)    /* Hardware General Call ID */
#define IDX_I2C_ID0                                             (0X3CU)    /* 1st Slave Address Device ID */
#define IDX_I2C_ID1                                             (0X40U)    /* 2nd Slave Address Device ID */
#define IDX_I2C_ID2                                             (0X44U)    /* 3rd Slave Address Device ID */
#define IDX_I2C_ID3                                             (0X48U)    /* 4th Slave Address Device ID */
#define IDX_I2C_STAT                                            (0X4CU)    /* Master and Slave FIFO Status */
#define IDX_I2C_SHCTL                                           (0X50U)    /* Shared Control */
#define IDX_I2C_TCTL                                            (0X54U)    /* Timing Control Register */
#define IDX_I2C_ASTRETCH_SCL                                    (0X58U)    /* Automatic Stretch SCL Register */
#define IDX_I2C_IDFSTA                                          (0X5CU)    /* ID FIFO Status Register */
#define IDX_I2C_SLV_ADDR1                                       (0X60U)    /* Slave 10 Bits Address 1st Byte. */
#define IDX_I2C_SLV_ADDR2                                       (0X64U)    /* Slave 10 Bits Address 2nd Byte. */
#define IDX_I2C_SSTAT2                                          (0X68U)    /* Slave I2C Status/IRQ 2 */

/* ====================================================================================================
        I2C Module Register ResetValue Definitions
   ==================================================================================================== */
#define RSTVAL_I2C_MCTL                                            (0X0) 
#define RSTVAL_I2C_MSTAT                                           (0X6000) 
#define RSTVAL_I2C_MRX                                             (0X0) 
#define RSTVAL_I2C_MTX                                             (0X0) 
#define RSTVAL_I2C_MRXCNT                                          (0X0) 
#define RSTVAL_I2C_MCRXCNT                                         (0X0) 
#define RSTVAL_I2C_ADDR0                                           (0X0) 
#define RSTVAL_I2C_ADDR1                                           (0X0) 
#define RSTVAL_I2C_BYT                                             (0X0) 
#define RSTVAL_I2C_DIV                                             (0XC6C7) 
#define RSTVAL_I2C_SCTL                                            (0X0) 
#define RSTVAL_I2C_SSTAT                                           (0X1) 
#define RSTVAL_I2C_SRX                                             (0X0) 
#define RSTVAL_I2C_STX                                             (0X0) 
#define RSTVAL_I2C_ALT                                             (0X0) 
#define RSTVAL_I2C_ID0                                             (0X0) 
#define RSTVAL_I2C_ID1                                             (0X0) 
#define RSTVAL_I2C_ID2                                             (0X0) 
#define RSTVAL_I2C_ID3                                             (0X0) 
#define RSTVAL_I2C_STAT                                            (0X0) 
#define RSTVAL_I2C_SHCTL                                           (0X0) 
#define RSTVAL_I2C_TCTL                                            (0X205) 
#define RSTVAL_I2C_ASTRETCH_SCL                                    (0X0) 
#define RSTVAL_I2C_IDFSTA                                          (0X0) 
#define RSTVAL_I2C_SLV_ADDR1                                       (0X0) 
#define RSTVAL_I2C_SLV_ADDR2                                       (0X0) 
#define RSTVAL_I2C_SSTAT2                                          (0X0) 

/* ====================================================================================================
        I2C Module Register BitPositions, Lengths, Masks and Enumerations Definitions
   ==================================================================================================== */

/* ----------------------------------------------------------------------------------------------------
          MCTL                                                  Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_I2C_MCTL_MASEN                                   (0U)           /* Master Enable */
#define BITL_I2C_MCTL_MASEN                                   (1U)           /* Master Enable */
#define BITM_I2C_MCTL_MASEN                                   (0X0001U)      /* Master Enable */
#define BITP_I2C_MCTL_COMPETE                                 (1U)           /* Start Back-off Disable */
#define BITL_I2C_MCTL_COMPETE                                 (1U)           /* Start Back-off Disable */
#define BITM_I2C_MCTL_COMPETE                                 (0X0002U)      /* Start Back-off Disable */
#define BITP_I2C_MCTL_LOOPBACK                                (2U)           /* Internal Loopback Enable */
#define BITL_I2C_MCTL_LOOPBACK                                (1U)           /* Internal Loopback Enable */
#define BITM_I2C_MCTL_LOOPBACK                                (0X0004U)      /* Internal Loopback Enable */
#define BITP_I2C_MCTL_IENMRX                                  (4U)           /* Receive Request Interrupt Enable */
#define BITL_I2C_MCTL_IENMRX                                  (1U)           /* Receive Request Interrupt Enable */
#define BITM_I2C_MCTL_IENMRX                                  (0X0010U)      /* Receive Request Interrupt Enable */
#define BITP_I2C_MCTL_IENMTX                                  (5U)           /* Transmit Request Interrupt Enable. */
#define BITL_I2C_MCTL_IENMTX                                  (1U)           /* Transmit Request Interrupt Enable. */
#define BITM_I2C_MCTL_IENMTX                                  (0X0020U)      /* Transmit Request Interrupt Enable. */
#define BITP_I2C_MCTL_IENALOST                                (6U)           /* Arbitration Lost Interrupt Enable */
#define BITL_I2C_MCTL_IENALOST                                (1U)           /* Arbitration Lost Interrupt Enable */
#define BITM_I2C_MCTL_IENALOST                                (0X0040U)      /* Arbitration Lost Interrupt Enable */
#define BITP_I2C_MCTL_IENACK                                  (7U)           /* ACK Not Received Interrupt Enable */
#define BITL_I2C_MCTL_IENACK                                  (1U)           /* ACK Not Received Interrupt Enable */
#define BITM_I2C_MCTL_IENACK                                  (0X0080U)      /* ACK Not Received Interrupt Enable */
#define BITP_I2C_MCTL_IENCMP                                  (8U)           /* Transaction Completed (Or Stop Detected) Interrupt Enable */
#define BITL_I2C_MCTL_IENCMP                                  (1U)           /* Transaction Completed (Or Stop Detected) Interrupt Enable */
#define BITM_I2C_MCTL_IENCMP                                  (0X0100U)      /* Transaction Completed (Or Stop Detected) Interrupt Enable */
#define BITP_I2C_MCTL_MXMITDEC                                (9U)           /* Decrement Master TX FIFO Status When Transmitted One Byte */
#define BITL_I2C_MCTL_MXMITDEC                                (1U)           /* Decrement Master TX FIFO Status When Transmitted One Byte */
#define BITM_I2C_MCTL_MXMITDEC                                (0X0200U)      /* Decrement Master TX FIFO Status When Transmitted One Byte */
#define BITP_I2C_MCTL_MRXDMA                                  (10U)          /* Enable Master Rx DMA Request */
#define BITL_I2C_MCTL_MRXDMA                                  (1U)           /* Enable Master Rx DMA Request */
#define BITM_I2C_MCTL_MRXDMA                                  (0X0400U)      /* Enable Master Rx DMA Request */
#define BITP_I2C_MCTL_MTXDMA                                  (11U)          /* Enable Master Tx DMA Request */
#define BITL_I2C_MCTL_MTXDMA                                  (1U)           /* Enable Master Tx DMA Request */
#define BITM_I2C_MCTL_MTXDMA                                  (0X0800U)      /* Enable Master Tx DMA Request */
#define BITP_I2C_MCTL_BUS_CLR_EN                              (12U)          /* Bus-Clear Enable */
#define BITL_I2C_MCTL_BUS_CLR_EN                              (1U)           /* Bus-Clear Enable */
#define BITM_I2C_MCTL_BUS_CLR_EN                              (0X1000U)      /* Bus-Clear Enable */
#define BITP_I2C_MCTL_PRESTOP_BUS_CLR                         (13U)          /* Prestop Bus-Clear */
#define BITL_I2C_MCTL_PRESTOP_BUS_CLR                         (1U)           /* Prestop Bus-Clear */
#define BITM_I2C_MCTL_PRESTOP_BUS_CLR                         (0X2000U)      /* Prestop Bus-Clear */

/* ----------------------------------------------------------------------------------------------------
          MSTAT                                                 Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_I2C_MSTAT_MTXFSTA                                (0U)           /* Master Transmit FIFO Status */
#define BITL_I2C_MSTAT_MTXFSTA                                (2U)           /* Master Transmit FIFO Status */
#define BITM_I2C_MSTAT_MTXFSTA                                (0X0003U)      /* Master Transmit FIFO Status */
#define BITP_I2C_MSTAT_MTXREQ                                 (2U)           /* Master Transmit Interrupt Bit */
#define BITL_I2C_MSTAT_MTXREQ                                 (1U)           /* Master Transmit Interrupt Bit */
#define BITM_I2C_MSTAT_MTXREQ                                 (0X0004U)      /* Master Transmit Interrupt Bit */
#define BITP_I2C_MSTAT_MRXREQ                                 (3U)           /* Master Receive Request */
#define BITL_I2C_MSTAT_MRXREQ                                 (1U)           /* Master Receive Request */
#define BITM_I2C_MSTAT_MRXREQ                                 (0X0008U)      /* Master Receive Request */
#define BITP_I2C_MSTAT_NACKADDR                               (4U)           /* ACK Not Received in Response to an Address */
#define BITL_I2C_MSTAT_NACKADDR                               (1U)           /* ACK Not Received in Response to an Address */
#define BITM_I2C_MSTAT_NACKADDR                               (0X0010U)      /* ACK Not Received in Response to an Address */
#define BITP_I2C_MSTAT_ALOST                                  (5U)           /* Arbitration Lost */
#define BITL_I2C_MSTAT_ALOST                                  (1U)           /* Arbitration Lost */
#define BITM_I2C_MSTAT_ALOST                                  (0X0020U)      /* Arbitration Lost */
#define BITP_I2C_MSTAT_MBUSY                                  (6U)           /* Master Busy */
#define BITL_I2C_MSTAT_MBUSY                                  (1U)           /* Master Busy */
#define BITM_I2C_MSTAT_MBUSY                                  (0X0040U)      /* Master Busy */
#define BITP_I2C_MSTAT_NACKDATA                               (7U)           /* ACK Not Received in Response to Data Write */
#define BITL_I2C_MSTAT_NACKDATA                               (1U)           /* ACK Not Received in Response to Data Write */
#define BITM_I2C_MSTAT_NACKDATA                               (0X0080U)      /* ACK Not Received in Response to Data Write */
#define BITP_I2C_MSTAT_TCOMP                                  (8U)           /* Transaction Complete or Stop Detected */
#define BITL_I2C_MSTAT_TCOMP                                  (1U)           /* Transaction Complete or Stop Detected */
#define BITM_I2C_MSTAT_TCOMP                                  (0X0100U)      /* Transaction Complete or Stop Detected */
#define BITP_I2C_MSTAT_MRXOF                                  (9U)           /* Master Receive FIFO Overflow */
#define BITL_I2C_MSTAT_MRXOF                                  (1U)           /* Master Receive FIFO Overflow */
#define BITM_I2C_MSTAT_MRXOF                                  (0X0200U)      /* Master Receive FIFO Overflow */
#define BITP_I2C_MSTAT_LINEBUSY                               (10U)          /* Line is Busy */
#define BITL_I2C_MSTAT_LINEBUSY                               (1U)           /* Line is Busy */
#define BITM_I2C_MSTAT_LINEBUSY                               (0X0400U)      /* Line is Busy */
#define BITP_I2C_MSTAT_MSTOP                                  (11U)          /* STOP Driven by This I2C Master */
#define BITL_I2C_MSTAT_MSTOP                                  (1U)           /* STOP Driven by This I2C Master */
#define BITM_I2C_MSTAT_MSTOP                                  (0X0800U)      /* STOP Driven by This I2C Master */
#define BITP_I2C_MSTAT_MTXUFLOW                               (12U)          /* Master Transmit Underflow */
#define BITL_I2C_MSTAT_MTXUFLOW                               (1U)           /* Master Transmit Underflow */
#define BITM_I2C_MSTAT_MTXUFLOW                               (0X1000U)      /* Master Transmit Underflow */
#define BITP_I2C_MSTAT_SDA_FILTERED                           (13U)          /* State of SDA Line */
#define BITL_I2C_MSTAT_SDA_FILTERED                           (1U)           /* State of SDA Line */
#define BITM_I2C_MSTAT_SDA_FILTERED                           (0X2000U)      /* State of SDA Line */
#define BITP_I2C_MSTAT_SCL_FILTERED                           (14U)          /* State of SCL Line */
#define BITL_I2C_MSTAT_SCL_FILTERED                           (1U)           /* State of SCL Line */
#define BITM_I2C_MSTAT_SCL_FILTERED                           (0X4000U)      /* State of SCL Line */
#define BITP_I2C_MSTAT_MSTR_HS_MODE                           (15U)          /* Master High Speed Mode Flag */
#define BITL_I2C_MSTAT_MSTR_HS_MODE                           (1U)           /* Master High Speed Mode Flag */
#define BITM_I2C_MSTAT_MSTR_HS_MODE                           (0X8000U)      /* Master High Speed Mode Flag */

/* ----------------------------------------------------------------------------------------------------
          MRX                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_I2C_MRX_ICMRX                                    (0U)           /* Master Receive Register */
#define BITL_I2C_MRX_ICMRX                                    (8U)           /* Master Receive Register */
#define BITM_I2C_MRX_ICMRX                                    (0X00FFU)      /* Master Receive Register */

/* ----------------------------------------------------------------------------------------------------
          MTX                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_I2C_MTX_I2CMRX                                   (0U)           /* Master Transmit Register */
#define BITL_I2C_MTX_I2CMRX                                   (8U)           /* Master Transmit Register */
#define BITM_I2C_MTX_I2CMRX                                   (0X00FFU)      /* Master Transmit Register */

/* ----------------------------------------------------------------------------------------------------
          MRXCNT                                                Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_I2C_MRXCNT_COUNT                                 (0U)           /* Receive Count */
#define BITL_I2C_MRXCNT_COUNT                                 (8U)           /* Receive Count */
#define BITM_I2C_MRXCNT_COUNT                                 (0X00FFU)      /* Receive Count */
#define BITP_I2C_MRXCNT_EXTEND                                (8U)           /* Extended Read */
#define BITL_I2C_MRXCNT_EXTEND                                (1U)           /* Extended Read */
#define BITM_I2C_MRXCNT_EXTEND                                (0X0100U)      /* Extended Read */

/* ----------------------------------------------------------------------------------------------------
          MCRXCNT                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_I2C_MCRXCNT_COUNT                                (0U)           /* Current Receive Count */
#define BITL_I2C_MCRXCNT_COUNT                                (8U)           /* Current Receive Count */
#define BITM_I2C_MCRXCNT_COUNT                                (0X00FFU)      /* Current Receive Count */

/* ----------------------------------------------------------------------------------------------------
          ADDR0                                                 Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_I2C_ADDR0_ADR1                                   (0U)           /* Address Byte 1 */
#define BITL_I2C_ADDR0_ADR1                                   (8U)           /* Address Byte 1 */
#define BITM_I2C_ADDR0_ADR1                                   (0X00FFU)      /* Address Byte 1 */

/* ----------------------------------------------------------------------------------------------------
          ADDR1                                                 Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_I2C_ADDR1_ADR2                                   (0U)           /* Address Byte 2 */
#define BITL_I2C_ADDR1_ADR2                                   (8U)           /* Address Byte 2 */
#define BITM_I2C_ADDR1_ADR2                                   (0X00FFU)      /* Address Byte 2 */

/* ----------------------------------------------------------------------------------------------------
          BYT                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_I2C_BYT_SBYTE                                    (0U)           /* Start Byte */
#define BITL_I2C_BYT_SBYTE                                    (8U)           /* Start Byte */
#define BITM_I2C_BYT_SBYTE                                    (0X00FFU)      /* Start Byte */

/* ----------------------------------------------------------------------------------------------------
          DIV                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_I2C_DIV_LOW                                      (0U)           /* Serial Clock Low Time */
#define BITL_I2C_DIV_LOW                                      (8U)           /* Serial Clock Low Time */
#define BITM_I2C_DIV_LOW                                      (0X00FFU)      /* Serial Clock Low Time */
#define BITP_I2C_DIV_HIGH                                     (8U)           /* Serial Clock High Time */
#define BITL_I2C_DIV_HIGH                                     (8U)           /* Serial Clock High Time */
#define BITM_I2C_DIV_HIGH                                     (0XFF00U)      /* Serial Clock High Time */

/* ----------------------------------------------------------------------------------------------------
          SCTL                                                  Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_I2C_SCTL_SLVEN                                   (0U)           /* Slave Enable */
#define BITL_I2C_SCTL_SLVEN                                   (1U)           /* Slave Enable */
#define BITM_I2C_SCTL_SLVEN                                   (0X0001U)      /* Slave Enable */
#define BITP_I2C_SCTL_ADR10EN                                 (1U)           /* Enabled 10-bit Addressing */
#define BITL_I2C_SCTL_ADR10EN                                 (1U)           /* Enabled 10-bit Addressing */
#define BITM_I2C_SCTL_ADR10EN                                 (0X0002U)      /* Enabled 10-bit Addressing */
#define BITP_I2C_SCTL_GCEN                                    (2U)           /* General Call Enable */
#define BITL_I2C_SCTL_GCEN                                    (1U)           /* General Call Enable */
#define BITM_I2C_SCTL_GCEN                                    (0X0004U)      /* General Call Enable */
#define BITP_I2C_SCTL_HGCEN                                   (3U)           /* Hardware General Call Enable */
#define BITL_I2C_SCTL_HGCEN                                   (1U)           /* Hardware General Call Enable */
#define BITM_I2C_SCTL_HGCEN                                   (0X0008U)      /* Hardware General Call Enable */
#define BITP_I2C_SCTL_GCSBCLR                                 (4U)           /* General Call Status Bit Clear */
#define BITL_I2C_SCTL_GCSBCLR                                 (1U)           /* General Call Status Bit Clear */
#define BITM_I2C_SCTL_GCSBCLR                                 (0X0010U)      /* General Call Status Bit Clear */
#define BITP_I2C_SCTL_EARLYTXR                                (5U)           /* Early Transmit Request Mode */
#define BITL_I2C_SCTL_EARLYTXR                                (1U)           /* Early Transmit Request Mode */
#define BITM_I2C_SCTL_EARLYTXR                                (0X0020U)      /* Early Transmit Request Mode */
#define BITP_I2C_SCTL_NACK                                    (7U)           /* NACK Next Communication */
#define BITL_I2C_SCTL_NACK                                    (1U)           /* NACK Next Communication */
#define BITM_I2C_SCTL_NACK                                    (0X0080U)      /* NACK Next Communication */
#define BITP_I2C_SCTL_IENSTOP                                 (8U)           /* Stop Condition Detected Interrupt Enable */
#define BITL_I2C_SCTL_IENSTOP                                 (1U)           /* Stop Condition Detected Interrupt Enable */
#define BITM_I2C_SCTL_IENSTOP                                 (0X0100U)      /* Stop Condition Detected Interrupt Enable */
#define BITP_I2C_SCTL_IENSRX                                  (9U)           /* Slave Receive Request Interrupt Enable */
#define BITL_I2C_SCTL_IENSRX                                  (1U)           /* Slave Receive Request Interrupt Enable */
#define BITM_I2C_SCTL_IENSRX                                  (0X0200U)      /* Slave Receive Request Interrupt Enable */
#define BITP_I2C_SCTL_IENSTX                                  (10U)          /* Slave Transmit Request Interrupt Enable */
#define BITL_I2C_SCTL_IENSTX                                  (1U)           /* Slave Transmit Request Interrupt Enable */
#define BITM_I2C_SCTL_IENSTX                                  (0X0400U)      /* Slave Transmit Request Interrupt Enable */
#define BITP_I2C_SCTL_SXMITDEC                                (11U)          /* Decrement Slave Tx FIFO Status When Transmitted a Byte */
#define BITL_I2C_SCTL_SXMITDEC                                (1U)           /* Decrement Slave Tx FIFO Status When Transmitted a Byte */
#define BITM_I2C_SCTL_SXMITDEC                                (0X0800U)      /* Decrement Slave Tx FIFO Status When Transmitted a Byte */
#define BITP_I2C_SCTL_IENREPST                                (12U)          /* Repeated Start Interrupt Enable */
#define BITL_I2C_SCTL_IENREPST                                (1U)           /* Repeated Start Interrupt Enable */
#define BITM_I2C_SCTL_IENREPST                                (0X1000U)      /* Repeated Start Interrupt Enable */
#define BITP_I2C_SCTL_SRXDMA                                  (13U)          /* Enable Slave Rx DMA Request */
#define BITL_I2C_SCTL_SRXDMA                                  (1U)           /* Enable Slave Rx DMA Request */
#define BITM_I2C_SCTL_SRXDMA                                  (0X2000U)      /* Enable Slave Rx DMA Request */
#define BITP_I2C_SCTL_STXDMA                                  (14U)          /* Enable Slave Tx DMA Request */
#define BITL_I2C_SCTL_STXDMA                                  (1U)           /* Enable Slave Tx DMA Request */
#define BITM_I2C_SCTL_STXDMA                                  (0X4000U)      /* Enable Slave Tx DMA Request */
#define BITP_I2C_SCTL_ID_FIFO_EN                              (15U)          /* ID FIFO Enable */
#define BITL_I2C_SCTL_ID_FIFO_EN                              (1U)           /* ID FIFO Enable */
#define BITM_I2C_SCTL_ID_FIFO_EN                              (0X8000U)      /* ID FIFO Enable */

/* ----------------------------------------------------------------------------------------------------
          SSTAT                                                 Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_I2C_SSTAT_STXFSEREQ                              (0U)           /* Slave Tx FIFO Status or Early Request */
#define BITL_I2C_SSTAT_STXFSEREQ                              (1U)           /* Slave Tx FIFO Status or Early Request */
#define BITM_I2C_SSTAT_STXFSEREQ                              (0X0001U)      /* Slave Tx FIFO Status or Early Request */
#define BITP_I2C_SSTAT_STXUR                                  (1U)           /* Slave Transmit FIFO Underflow */
#define BITL_I2C_SSTAT_STXUR                                  (1U)           /* Slave Transmit FIFO Underflow */
#define BITM_I2C_SSTAT_STXUR                                  (0X0002U)      /* Slave Transmit FIFO Underflow */
#define BITP_I2C_SSTAT_STXREQ                                 (2U)           /* Slave Transmit Interrupt Bit */
#define BITL_I2C_SSTAT_STXREQ                                 (1U)           /* Slave Transmit Interrupt Bit */
#define BITM_I2C_SSTAT_STXREQ                                 (0X0004U)      /* Slave Transmit Interrupt Bit */
#define BITP_I2C_SSTAT_SRXREQ                                 (3U)           /* Slave Receive Request */
#define BITL_I2C_SSTAT_SRXREQ                                 (1U)           /* Slave Receive Request */
#define BITM_I2C_SSTAT_SRXREQ                                 (0X0008U)      /* Slave Receive Request */
#define BITP_I2C_SSTAT_SRXOF                                  (4U)           /* Slave Receive FIFO Overflow */
#define BITL_I2C_SSTAT_SRXOF                                  (1U)           /* Slave Receive FIFO Overflow */
#define BITM_I2C_SSTAT_SRXOF                                  (0X0010U)      /* Slave Receive FIFO Overflow */
#define BITP_I2C_SSTAT_NOACK                                  (5U)           /* Ack Not Generated by the Slave */
#define BITL_I2C_SSTAT_NOACK                                  (1U)           /* Ack Not Generated by the Slave */
#define BITM_I2C_SSTAT_NOACK                                  (0X0020U)      /* Ack Not Generated by the Slave */
#define BITP_I2C_SSTAT_SBUSY                                  (6U)           /* Slave Busy */
#define BITL_I2C_SSTAT_SBUSY                                  (1U)           /* Slave Busy */
#define BITM_I2C_SSTAT_SBUSY                                  (0X0040U)      /* Slave Busy */
#define BITP_I2C_SSTAT_GCINT                                  (7U)           /* General Call Interrupt */
#define BITL_I2C_SSTAT_GCINT                                  (1U)           /* General Call Interrupt */
#define BITM_I2C_SSTAT_GCINT                                  (0X0080U)      /* General Call Interrupt */
#define BITP_I2C_SSTAT_GCID                                   (8U)           /* General ID */
#define BITL_I2C_SSTAT_GCID                                   (2U)           /* General ID */
#define BITM_I2C_SSTAT_GCID                                   (0X0300U)      /* General ID */
#define BITP_I2C_SSTAT_STOP                                   (10U)          /* Stop After Start and Matching Address */
#define BITL_I2C_SSTAT_STOP                                   (1U)           /* Stop After Start and Matching Address */
#define BITM_I2C_SSTAT_STOP                                   (0X0400U)      /* Stop After Start and Matching Address */
#define BITP_I2C_SSTAT_IDMAT                                  (11U)          /* Device ID Matched */
#define BITL_I2C_SSTAT_IDMAT                                  (2U)           /* Device ID Matched */
#define BITM_I2C_SSTAT_IDMAT                                  (0X1800U)      /* Device ID Matched */
#define BITP_I2C_SSTAT_REPSTART                               (13U)          /* Repeated Start and Matching Address */
#define BITL_I2C_SSTAT_REPSTART                               (1U)           /* Repeated Start and Matching Address */
#define BITM_I2C_SSTAT_REPSTART                               (0X2000U)      /* Repeated Start and Matching Address */
#define BITP_I2C_SSTAT_START                                  (14U)          /* Start and Matching Address */
#define BITL_I2C_SSTAT_START                                  (1U)           /* Start and Matching Address */
#define BITM_I2C_SSTAT_START                                  (0X4000U)      /* Start and Matching Address */
#define BITP_I2C_SSTAT_SLV_HS_MODE                            (15U)          /* Slave High Speed Mode Flag */
#define BITL_I2C_SSTAT_SLV_HS_MODE                            (1U)           /* Slave High Speed Mode Flag */
#define BITM_I2C_SSTAT_SLV_HS_MODE                            (0X8000U)      /* Slave High Speed Mode Flag */

#define ENUM_I2C_SSTAT_IDMAT_ID0                              (0X0000U)      /* Received Address Matched ID Register 0 */
#define ENUM_I2C_SSTAT_IDMAT_ID1                              (0X0001U)      /* Received Address Matched ID Register 1 */
#define ENUM_I2C_SSTAT_IDMAT_ID2                              (0X0002U)      /* Received Address Matched ID Register 2 */
#define ENUM_I2C_SSTAT_IDMAT_ID3                              (0X0003U)      /* Received Address Matched ID Register 3 */
#define ENUM_I2C_SSTAT_GCID_NONE                              (0X0000U)      /* No general call */
#define ENUM_I2C_SSTAT_GCID_RESETPROGADDR                     (0X0001U)      /* General call Reset and Program Address */
#define ENUM_I2C_SSTAT_GCID_PROGADDR                          (0X0002U)      /* General Call Program Address */
#define ENUM_I2C_SSTAT_GCID_MATCHID                           (0X0003U)      /* General Call Matching Alternative ID */

/* ----------------------------------------------------------------------------------------------------
          SRX                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_I2C_SRX_I2CSRX                                   (0U)           /* Slave Receive Register */
#define BITL_I2C_SRX_I2CSRX                                   (8U)           /* Slave Receive Register */
#define BITM_I2C_SRX_I2CSRX                                   (0X00FFU)      /* Slave Receive Register */

/* ----------------------------------------------------------------------------------------------------
          STX                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_I2C_STX_I2CSTX                                   (0U)           /* Slave Transmit Register */
#define BITL_I2C_STX_I2CSTX                                   (8U)           /* Slave Transmit Register */
#define BITM_I2C_STX_I2CSTX                                   (0X00FFU)      /* Slave Transmit Register */
#define BITP_I2C_STX_ID_SEL                                   (8U)           /* ID FIFO Selection */
#define BITL_I2C_STX_ID_SEL                                   (2U)           /* ID FIFO Selection */
#define BITM_I2C_STX_ID_SEL                                   (0X0300U)      /* ID FIFO Selection */

#define ENUM_I2C_STX_ID_SEL_FIFO0                             (0X0000U)      /* Select Address Matched ID Register 0 */
#define ENUM_I2C_STX_ID_SEL_FIFO1                             (0X0001U)      /* Select Address Matched ID Register 1 */
#define ENUM_I2C_STX_ID_SEL_FIFO2                             (0X0002U)      /* Select Address Matched ID Register 2 */
#define ENUM_I2C_STX_ID_SEL_FIFO3                             (0X0003U)      /* Select Address Matched ID Register 3 */

/* ----------------------------------------------------------------------------------------------------
          ALT                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_I2C_ALT_ALT                                      (0U)           /* Slave Alt */
#define BITL_I2C_ALT_ALT                                      (8U)           /* Slave Alt */
#define BITM_I2C_ALT_ALT                                      (0X00FFU)      /* Slave Alt */

/* ----------------------------------------------------------------------------------------------------
          ID0                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_I2C_ID0_ID0                                      (0U)           /* Slave Device ID 0 */
#define BITL_I2C_ID0_ID0                                      (8U)           /* Slave Device ID 0 */
#define BITM_I2C_ID0_ID0                                      (0X00FFU)      /* Slave Device ID 0 */

/* ----------------------------------------------------------------------------------------------------
          ID1                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_I2C_ID1_ID1                                      (0U)           /* Slave Device ID 1 */
#define BITL_I2C_ID1_ID1                                      (8U)           /* Slave Device ID 1 */
#define BITM_I2C_ID1_ID1                                      (0X00FFU)      /* Slave Device ID 1 */

/* ----------------------------------------------------------------------------------------------------
          ID2                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_I2C_ID2_ID2                                      (0U)           /* Slave Device ID 2 */
#define BITL_I2C_ID2_ID2                                      (8U)           /* Slave Device ID 2 */
#define BITM_I2C_ID2_ID2                                      (0X00FFU)      /* Slave Device ID 2 */

/* ----------------------------------------------------------------------------------------------------
          ID3                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_I2C_ID3_ID3                                      (0U)           /* Slave Device ID 3 */
#define BITL_I2C_ID3_ID3                                      (8U)           /* Slave Device ID 3 */
#define BITM_I2C_ID3_ID3                                      (0X00FFU)      /* Slave Device ID 3 */

/* ----------------------------------------------------------------------------------------------------
          STAT                                                  Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_I2C_STAT_STXFSTA                                 (0U)           /* Slave Transmit FIFO Status */
#define BITL_I2C_STAT_STXFSTA                                 (2U)           /* Slave Transmit FIFO Status */
#define BITM_I2C_STAT_STXFSTA                                 (0X0003U)      /* Slave Transmit FIFO Status */
#define BITP_I2C_STAT_SRXFSTA                                 (2U)           /* Slave Receive FIFO Status */
#define BITL_I2C_STAT_SRXFSTA                                 (2U)           /* Slave Receive FIFO Status */
#define BITM_I2C_STAT_SRXFSTA                                 (0X000CU)      /* Slave Receive FIFO Status */
#define BITP_I2C_STAT_MTXSFA                                  (4U)           /* Master Transmit FIFO Status */
#define BITL_I2C_STAT_MTXSFA                                  (2U)           /* Master Transmit FIFO Status */
#define BITM_I2C_STAT_MTXSFA                                  (0X0030U)      /* Master Transmit FIFO Status */
#define BITP_I2C_STAT_MRXFSTA                                 (6U)           /* Master Receive FIFO Status */
#define BITL_I2C_STAT_MRXFSTA                                 (2U)           /* Master Receive FIFO Status */
#define BITM_I2C_STAT_MRXFSTA                                 (0X00C0U)      /* Master Receive FIFO Status */
#define BITP_I2C_STAT_SFLUSH                                  (8U)           /* Flush the Slave Transmit FIFO */
#define BITL_I2C_STAT_SFLUSH                                  (1U)           /* Flush the Slave Transmit FIFO */
#define BITM_I2C_STAT_SFLUSH                                  (0X0100U)      /* Flush the Slave Transmit FIFO */
#define BITP_I2C_STAT_MFLUSH                                  (9U)           /* Flush the Master Transmit FIFO */
#define BITL_I2C_STAT_MFLUSH                                  (1U)           /* Flush the Master Transmit FIFO */
#define BITM_I2C_STAT_MFLUSH                                  (0X0200U)      /* Flush the Master Transmit FIFO */
#define BITP_I2C_STAT_STX_FLUSH_ALL                           (10U)          /* Flush All the Slave Transmit ID FIFOs. */
#define BITL_I2C_STAT_STX_FLUSH_ALL                           (1U)           /* Flush All the Slave Transmit ID FIFOs. */
#define BITM_I2C_STAT_STX_FLUSH_ALL                           (0X0400U)      /* Flush All the Slave Transmit ID FIFOs. */

#define ENUM_I2C_STAT_MRXFSTA_EMPTY                           (0X0000U)      /* FIFO Empty */
#define ENUM_I2C_STAT_MRXFSTA_ONE                             (0X0001U)      /* 1 Bytes In The FIFO */
#define ENUM_I2C_STAT_MRXFSTA_TWO                             (0X0002U)      /* 2 Bytes in the FIFO */
#define ENUM_I2C_STAT_MRXFSTA_RESERVED                        (0X0003U)      /* Reserved */
#define ENUM_I2C_STAT_MTXSFA_EMPTY                            (0X0000U)      /* FIFO Empty */
#define ENUM_I2C_STAT_MTXSFA_ONE                              (0X0001U)      /* 1 Bytes in the FIFO */
#define ENUM_I2C_STAT_MTXSFA_TWO                              (0X0002U)      /* 2 Bytes in the FIFO */
#define ENUM_I2C_STAT_MTXSFA_RESERVED                         (0X0003U)      /* Reserved */
#define ENUM_I2C_STAT_SRXFSTA_EMPTY                           (0X0000U)      /* FIFO Empty */
#define ENUM_I2C_STAT_SRXFSTA_ONE                             (0X0001U)      /* 1 Bytes in the FIFO */
#define ENUM_I2C_STAT_SRXFSTA_TWO                             (0X0002U)      /* 2 Bytes in the FIFO */
#define ENUM_I2C_STAT_SRXFSTA_RESERVED                        (0X0003U)      /* Reserved */
#define ENUM_I2C_STAT_STXFSTA_EMPTY                           (0X0000U)      /* FIFO Empty */
#define ENUM_I2C_STAT_STXFSTA_ONE                             (0X0001U)      /* 1 Bytes in the FIFO */
#define ENUM_I2C_STAT_STXFSTA_TWO                             (0X0002U)      /* 2 Bytes in the FIFO */
#define ENUM_I2C_STAT_STXFSTA_RESERVED                        (0X0003U)      /* Reserved */

/* ----------------------------------------------------------------------------------------------------
          SHCTL                                                 Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_I2C_SHCTL_RESET                                  (0U)           /* Reset START STOP Detect Circuit */
#define BITL_I2C_SHCTL_RESET                                  (1U)           /* Reset START STOP Detect Circuit */
#define BITM_I2C_SHCTL_RESET                                  (0X0001U)      /* Reset START STOP Detect Circuit */
#define BITP_I2C_SHCTL_ADR_BUSY                               (6U)           /* ADR Busy Flag */
#define BITL_I2C_SHCTL_ADR_BUSY                               (1U)           /* ADR Busy Flag */
#define BITM_I2C_SHCTL_ADR_BUSY                               (0X0040U)      /* ADR Busy Flag */
#define BITP_I2C_SHCTL_SDA_DLY                                (8U)           /* Delay Between SDAOUT and SDAOEN */
#define BITL_I2C_SHCTL_SDA_DLY                                (4U)           /* Delay Between SDAOUT and SDAOEN */
#define BITM_I2C_SHCTL_SDA_DLY                                (0X0F00U)      /* Delay Between SDAOUT and SDAOEN */
#define BITP_I2C_SHCTL_SCL_DLY                                (12U)          /* Delay Between SCLOUT and SCLOEN */
#define BITL_I2C_SHCTL_SCL_DLY                                (4U)           /* Delay Between SCLOUT and SCLOEN */
#define BITM_I2C_SHCTL_SCL_DLY                                (0XF000U)      /* Delay Between SCLOUT and SCLOEN */

/* ----------------------------------------------------------------------------------------------------
          TCTL                                                  Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_I2C_TCTL_THDATIN                                 (0U)           /* Data in Hold Start */
#define BITL_I2C_TCTL_THDATIN                                 (6U)           /* Data in Hold Start */
#define BITM_I2C_TCTL_THDATIN                                 (0X003FU)      /* Data in Hold Start */
#define BITP_I2C_TCTL_FILTEROFF                               (8U)           /* Input Filter Control */
#define BITL_I2C_TCTL_FILTEROFF                               (1U)           /* Input Filter Control */
#define BITM_I2C_TCTL_FILTEROFF                               (0X0100U)      /* Input Filter Control */
#define BITP_I2C_TCTL_PRE_DIV                                 (9U)           /* Presale Divide Counter for SCK DIV */
#define BITL_I2C_TCTL_PRE_DIV                                 (3U)           /* Presale Divide Counter for SCK DIV */
#define BITM_I2C_TCTL_PRE_DIV                                 (0X0E00U)      /* Presale Divide Counter for SCK DIV */
#define BITP_I2C_TCTL_FILTER_TICKS                            (12U)          /* SCK & SDA Gglitch Filter Ticks. */
#define BITL_I2C_TCTL_FILTER_TICKS                            (4U)           /* SCK & SDA Gglitch Filter Ticks. */
#define BITM_I2C_TCTL_FILTER_TICKS                            (0XF000U)      /* SCK & SDA Gglitch Filter Ticks. */

/* ----------------------------------------------------------------------------------------------------
          ASTRETCH_SCL                                          Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_I2C_ASTRETCH_SCL_STRETCH_MODE_MAS                (0U)           /* Master Automatic Stretch Mode */
#define BITL_I2C_ASTRETCH_SCL_STRETCH_MODE_MAS                (4U)           /* Master Automatic Stretch Mode */
#define BITM_I2C_ASTRETCH_SCL_STRETCH_MODE_MAS                (0X000FU)      /* Master Automatic Stretch Mode */
#define BITP_I2C_ASTRETCH_SCL_STRECTH_MODE_SLV                (4U)           /* Slave Automatic Stretch Mode */
#define BITL_I2C_ASTRETCH_SCL_STRECTH_MODE_SLV                (4U)           /* Slave Automatic Stretch Mode */
#define BITM_I2C_ASTRETCH_SCL_STRECTH_MODE_SLV                (0X00F0U)      /* Slave Automatic Stretch Mode */
#define BITP_I2C_ASTRETCH_SCL_TIMEOUT_SSCL_MAS                (8U)           /* Master Automatic Stretch Timeout */
#define BITL_I2C_ASTRETCH_SCL_TIMEOUT_SSCL_MAS                (1U)           /* Master Automatic Stretch Timeout */
#define BITM_I2C_ASTRETCH_SCL_TIMEOUT_SSCL_MAS                (0X0100U)      /* Master Automatic Stretch Timeout */
#define BITP_I2C_ASTRETCH_SCL_TIMEOUT_SSCL_SLV                (9U)           /* Slave Automatic Stretch Timeout */
#define BITL_I2C_ASTRETCH_SCL_TIMEOUT_SSCL_SLV                (1U)           /* Slave Automatic Stretch Timeout */
#define BITM_I2C_ASTRETCH_SCL_TIMEOUT_SSCL_SLV                (0X0200U)      /* Slave Automatic Stretch Timeout */
#define BITP_I2C_ASTRETCH_SCL_MAN_RLS_EN                      (10U)          /* Manual Release Stretch Enable */
#define BITL_I2C_ASTRETCH_SCL_MAN_RLS_EN                      (1U)           /* Manual Release Stretch Enable */
#define BITM_I2C_ASTRETCH_SCL_MAN_RLS_EN                      (0X0400U)      /* Manual Release Stretch Enable */
#define BITP_I2C_ASTRETCH_SCL_SSCL_AFTER_ACK                  (11U)          /* Slave RX Stretch After ACK Mode */
#define BITL_I2C_ASTRETCH_SCL_SSCL_AFTER_ACK                  (1U)           /* Slave RX Stretch After ACK Mode */
#define BITM_I2C_ASTRETCH_SCL_SSCL_AFTER_ACK                  (0X0800U)      /* Slave RX Stretch After ACK Mode */
#define BITP_I2C_ASTRETCH_SCL_SSCL_RLS                        (12U)          /* Slave Manual Clear Stretch */
#define BITL_I2C_ASTRETCH_SCL_SSCL_RLS                        (1U)           /* Slave Manual Clear Stretch */
#define BITM_I2C_ASTRETCH_SCL_SSCL_RLS                        (0X1000U)      /* Slave Manual Clear Stretch */
#define BITP_I2C_ASTRETCH_SCL_ADDR_ACK_IEN                    (13U)          /* Slave ADDRESS ACK Interruption Enable */
#define BITL_I2C_ASTRETCH_SCL_ADDR_ACK_IEN                    (1U)           /* Slave ADDRESS ACK Interruption Enable */
#define BITM_I2C_ASTRETCH_SCL_ADDR_ACK_IEN                    (0X2000U)      /* Slave ADDRESS ACK Interruption Enable */
#define BITP_I2C_ASTRETCH_SCL_SSCL_IRQ_IEN                    (14U)          /* Slave Stretch Interruption Enable */
#define BITL_I2C_ASTRETCH_SCL_SSCL_IRQ_IEN                    (1U)           /* Slave Stretch Interruption Enable */
#define BITM_I2C_ASTRETCH_SCL_SSCL_IRQ_IEN                    (0X4000U)      /* Slave Stretch Interruption Enable */
#define BITP_I2C_ASTRETCH_SCL_CLR_ADDR_ACK_IRQ                (15U)          /* Write Clear the Address ACK IRQ. */
#define BITL_I2C_ASTRETCH_SCL_CLR_ADDR_ACK_IRQ                (1U)           /* Write Clear the Address ACK IRQ. */
#define BITM_I2C_ASTRETCH_SCL_CLR_ADDR_ACK_IRQ                (0X8000U)      /* Write Clear the Address ACK IRQ. */

/* ----------------------------------------------------------------------------------------------------
          IDFSTA                                                Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_I2C_IDFSTA_STX0FSTA                              (0U)           /* Slave Transmit ID0 FIFO Status */
#define BITL_I2C_IDFSTA_STX0FSTA                              (2U)           /* Slave Transmit ID0 FIFO Status */
#define BITM_I2C_IDFSTA_STX0FSTA                              (0X0003U)      /* Slave Transmit ID0 FIFO Status */
#define BITP_I2C_IDFSTA_STX1FSTA                              (2U)           /* Slave Transmit ID1 FIFO Status */
#define BITL_I2C_IDFSTA_STX1FSTA                              (2U)           /* Slave Transmit ID1 FIFO Status */
#define BITM_I2C_IDFSTA_STX1FSTA                              (0X000CU)      /* Slave Transmit ID1 FIFO Status */
#define BITP_I2C_IDFSTA_STX2FSTA                              (4U)           /* Slave Transmit ID2 FIFO Status */
#define BITL_I2C_IDFSTA_STX2FSTA                              (2U)           /* Slave Transmit ID2 FIFO Status */
#define BITM_I2C_IDFSTA_STX2FSTA                              (0X0030U)      /* Slave Transmit ID2 FIFO Status */
#define BITP_I2C_IDFSTA_STX3FSTA                              (6U)           /* Slave Transmit ID3 FIFO Status */
#define BITL_I2C_IDFSTA_STX3FSTA                              (2U)           /* Slave Transmit ID3 FIFO Status */
#define BITM_I2C_IDFSTA_STX3FSTA                              (0X00C0U)      /* Slave Transmit ID3 FIFO Status */
#define BITP_I2C_IDFSTA_SFLUSH0                               (8U)           /* Flush the Slave Transmit ID0 FIFO */
#define BITL_I2C_IDFSTA_SFLUSH0                               (1U)           /* Flush the Slave Transmit ID0 FIFO */
#define BITM_I2C_IDFSTA_SFLUSH0                               (0X0100U)      /* Flush the Slave Transmit ID0 FIFO */
#define BITP_I2C_IDFSTA_SFLUSH1                               (9U)           /* Flush the Slave Transmit ID1 FIFO */
#define BITL_I2C_IDFSTA_SFLUSH1                               (1U)           /* Flush the Slave Transmit ID1 FIFO */
#define BITM_I2C_IDFSTA_SFLUSH1                               (0X0200U)      /* Flush the Slave Transmit ID1 FIFO */
#define BITP_I2C_IDFSTA_SFLUSH2                               (10U)          /* Flush the Slave Transmit ID2 FIFO */
#define BITL_I2C_IDFSTA_SFLUSH2                               (1U)           /* Flush the Slave Transmit ID2 FIFO */
#define BITM_I2C_IDFSTA_SFLUSH2                               (0X0400U)      /* Flush the Slave Transmit ID2 FIFO */
#define BITP_I2C_IDFSTA_SFLUSH3                               (11U)          /* Flush the Slave Transmit ID3 FIFO */
#define BITL_I2C_IDFSTA_SFLUSH3                               (1U)           /* Flush the Slave Transmit ID3 FIFO */
#define BITM_I2C_IDFSTA_SFLUSH3                               (0X0800U)      /* Flush the Slave Transmit ID3 FIFO */
#define BITP_I2C_IDFSTA_STX0UR                                (12U)          /* Slave Transmit ID0 FIFO Underflow */
#define BITL_I2C_IDFSTA_STX0UR                                (1U)           /* Slave Transmit ID0 FIFO Underflow */
#define BITM_I2C_IDFSTA_STX0UR                                (0X1000U)      /* Slave Transmit ID0 FIFO Underflow */
#define BITP_I2C_IDFSTA_STX1UR                                (13U)          /* Slave Transmit ID1 FIFO Underflow */
#define BITL_I2C_IDFSTA_STX1UR                                (1U)           /* Slave Transmit ID1 FIFO Underflow */
#define BITM_I2C_IDFSTA_STX1UR                                (0X2000U)      /* Slave Transmit ID1 FIFO Underflow */
#define BITP_I2C_IDFSTA_STX2UR                                (14U)          /* Slave Transmit ID2 FIFO Underflow */
#define BITL_I2C_IDFSTA_STX2UR                                (1U)           /* Slave Transmit ID2 FIFO Underflow */
#define BITM_I2C_IDFSTA_STX2UR                                (0X4000U)      /* Slave Transmit ID2 FIFO Underflow */
#define BITP_I2C_IDFSTA_STX3UR                                (15U)          /* Slave Transmit ID3 FIFO Underflow */
#define BITL_I2C_IDFSTA_STX3UR                                (1U)           /* Slave Transmit ID3 FIFO Underflow */
#define BITM_I2C_IDFSTA_STX3UR                                (0X8000U)      /* Slave Transmit ID3 FIFO Underflow */

#define ENUM_I2C_IDFSTA_STX3FSTA_EMPTY                        (0X0000U)      /* FIFO Empty */
#define ENUM_I2C_IDFSTA_STX3FSTA_ONE                          (0X0001U)      /* 1 Bytes in the FIFO */
#define ENUM_I2C_IDFSTA_STX3FSTA_TWO                          (0X0002U)      /* 2 Bytes in the FIFO */
#define ENUM_I2C_IDFSTA_STX3FSTA_RESERVED                     (0X0003U)      /* Reserved */
#define ENUM_I2C_IDFSTA_STX2FSTA_EMPTY                        (0X0000U)      /* FIFO Empty */
#define ENUM_I2C_IDFSTA_STX2FSTA_ONE                          (0X0001U)      /* 1 Bytes in the FIFO */
#define ENUM_I2C_IDFSTA_STX2FSTA_TWO                          (0X0002U)      /* 2 Bytes in the FIFO */
#define ENUM_I2C_IDFSTA_STX2FSTA_RESERVED                     (0X0003U)      /* Reserved */
#define ENUM_I2C_IDFSTA_STX1FSTA_EMPTY                        (0X0000U)      /* FIFO Empty */
#define ENUM_I2C_IDFSTA_STX1FSTA_ONE                          (0X0001U)      /* 1 Bytes in the FIFO */
#define ENUM_I2C_IDFSTA_STX1FSTA_TWO                          (0X0002U)      /* 2 Bytes in the FIFO */
#define ENUM_I2C_IDFSTA_STX1FSTA_RESERVED                     (0X0003U)      /* Reserved */
#define ENUM_I2C_IDFSTA_STX0FSTA_EMPTY                        (0X0000U)      /* FIFO Empty */
#define ENUM_I2C_IDFSTA_STX0FSTA_ONE                          (0X0001U)      /* 1 Bytes in the FIFO */
#define ENUM_I2C_IDFSTA_STX0FSTA_TWO                          (0X0002U)      /* 2 Bytes in the FIFO */
#define ENUM_I2C_IDFSTA_STX0FSTA_RESERVED                     (0X0003U)      /* Reserved */

/* ----------------------------------------------------------------------------------------------------
          SLV_ADDR1                                             Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_I2C_SLV_ADDR1_SLV_ADR1                           (0U)           /* Slave 10 Bits Address 1st Byte */
#define BITL_I2C_SLV_ADDR1_SLV_ADR1                           (8U)           /* Slave 10 Bits Address 1st Byte */
#define BITM_I2C_SLV_ADDR1_SLV_ADR1                           (0X00FFU)      /* Slave 10 Bits Address 1st Byte */

/* ----------------------------------------------------------------------------------------------------
          SLV_ADDR2                                             Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_I2C_SLV_ADDR2_SLV_ADR2                           (0U)           /* Slave 10 Bit Address 2nd Byte */
#define BITL_I2C_SLV_ADDR2_SLV_ADR2                           (8U)           /* Slave 10 Bit Address 2nd Byte */
#define BITM_I2C_SLV_ADDR2_SLV_ADR2                           (0X00FFU)      /* Slave 10 Bit Address 2nd Byte */

/* ----------------------------------------------------------------------------------------------------
          SSTAT2                                                Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_I2C_SSTAT2_SSCL_IRQ                              (0U)           /* Stretch Interruption State. */
#define BITL_I2C_SSTAT2_SSCL_IRQ                              (1U)           /* Stretch Interruption State. */
#define BITM_I2C_SSTAT2_SSCL_IRQ                              (0X0001U)      /* Stretch Interruption State. */
#define BITP_I2C_SSTAT2_ADDR_ACK_IRQ                          (1U)           /* Slave ADDR ACK Interruption State. */
#define BITL_I2C_SSTAT2_ADDR_ACK_IRQ                          (1U)           /* Slave ADDR ACK Interruption State. */
#define BITM_I2C_SSTAT2_ADDR_ACK_IRQ                          (0X0002U)      /* Slave ADDR ACK Interruption State. */
#define BITP_I2C_SSTAT2_RW_DIRECTION                          (2U)           /* Slave I2C RW Direction. */
#define BITL_I2C_SSTAT2_RW_DIRECTION                          (1U)           /* Slave I2C RW Direction. */
#define BITM_I2C_SSTAT2_RW_DIRECTION                          (0X0004U)      /* Slave I2C RW Direction. */

#endif  /* end ifndef I2C_ADDR_RDEF_H_ */


#ifndef MDIO_ADDR_RDEF_H_
#define MDIO_ADDR_RDEF_H_    /* MDIO: MDIO Interface */

/* ====================================================================================================
        MDIO Module Instances Address and Mask Definitions
   ==================================================================================================== */
#define INST_MDIO                                                (0X40022000U)    /* mdio: */

#define MASK_MDIO                                                (0XFFFFFFFFU)    /* MDIO: MDIO Interface */

/* ====================================================================================================
        MDIO Module Register Address Offset Definitions
   ==================================================================================================== */
#define IDX_MDIO_MDCON                                          (0X00000000U)    /* MDIO Block Control */
#define IDX_MDIO_MDFRM                                          (0X00000004U)    /* MDIO Received Frame Control Information */
#define IDX_MDIO_MDRXD                                          (0X00000008U)    /* MDIO Received Data */
#define IDX_MDIO_MDADR                                          (0X0000000CU)    /* MDIO Received Address */
#define IDX_MDIO_MDTXD                                          (0X00000010U)    /* MDIO Data for Transmission */
#define IDX_MDIO_MDPHY                                          (0X00000014U)    /* MDIO PHYADDR Software Values and Selection and DEVADD */
#define IDX_MDIO_MDSTA                                          (0X00000018U)    /* MDIO Progress Signaling Through Frame */
#define IDX_MDIO_MDIEN                                          (0X0000001CU)    /* MDIO Interrupt Enables */
#define IDX_MDIO_MDPIN                                          (0X00000020U)    /* MDIO Read PHYADDR Pins */
#define IDX_MDIO_DMAEN                                          (0X00000028U)    /* MDIO DMA Enable */
#define IDX_MDIO_MDTESTCON                                      (0X0000002CU)    /* MDIO Test Controller Protected by Test Key */

/* ====================================================================================================
        MDIO Module Register ResetValue Definitions
   ==================================================================================================== */
#define RSTVAL_MDIO_MDCON                                          (0X0) 
#define RSTVAL_MDIO_MDFRM                                          (0X0) 
#define RSTVAL_MDIO_MDTXD                                          (0X0) 
#define RSTVAL_MDIO_MDPHY                                          (0X400) 
#define RSTVAL_MDIO_MDSTA                                          (0X0) 
#define RSTVAL_MDIO_MDIEN                                          (0X0) 
#define RSTVAL_MDIO_MDPIN                                          (0X0) 
#define RSTVAL_MDIO_DMAEN                                          (0X0) 
#define RSTVAL_MDIO_MDTESTCON                                      (0X800) 

/* ====================================================================================================
        MDIO Module Register BitPositions, Lengths, Masks and Enumerations Definitions
   ==================================================================================================== */

/* ----------------------------------------------------------------------------------------------------
          MDCON                                                 Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_MDIO_MDCON_MD_RST                                (0U)           /* Write 1 to Reset MDIO Block. */
#define BITL_MDIO_MDCON_MD_RST                                (1U)           /* Write 1 to Reset MDIO Block. */
#define BITM_MDIO_MDCON_MD_RST                                (0X0001U)      /* Write 1 to Reset MDIO Block. */
#define BITP_MDIO_MDCON_MD_PHM                                (1U)           /* Enable PHY Address Bit Width */
#define BITL_MDIO_MDCON_MD_PHM                                (1U)           /* Enable PHY Address Bit Width */
#define BITM_MDIO_MDCON_MD_PHM                                (0X0002U)      /* Enable PHY Address Bit Width */
#define BITP_MDIO_MDCON_MD_DRV                                (2U)           /* Enable Open-drain or Push-pull of MDIO Drive */
#define BITL_MDIO_MDCON_MD_DRV                                (1U)           /* Enable Open-drain or Push-pull of MDIO Drive */
#define BITM_MDIO_MDCON_MD_DRV                                (0X0004U)      /* Enable Open-drain or Push-pull of MDIO Drive */
#define BITP_MDIO_MDCON_MD_EN                                 (8U)           /* MD Enable */
#define BITL_MDIO_MDCON_MD_EN                                 (1U)           /* MD Enable */
#define BITM_MDIO_MDCON_MD_EN                                 (0X0100U)      /* MD Enable */

#define ENUM_MDIO_MDCON_MD_DRV_MD_DRV_OD                      (0X0000U)      /* MDIO Drive Open Drain. */
#define ENUM_MDIO_MDCON_MD_DRV_MD_DRV_PP                      (0X0001U)      /* MDIO Drive Push-pull. */
#define ENUM_MDIO_MDCON_MD_PHM_EN_5BIT                        (0X0000U)      /* 5bits PHYADD is active */
#define ENUM_MDIO_MDCON_MD_PHM_EN_3BITS                       (0X0001U)      /* 3bits PHYADD is active, two MSBS are ignored */
#define ENUM_MDIO_MDCON_MD_RST_RSTACTIVE                      (0X0001U)      /* Reset MDIO */
#define ENUM_MDIO_MDCON_MD_RST_RSTINACTIVE                    (0X0000U)      /* Reset MDIO */

/* ----------------------------------------------------------------------------------------------------
          MDFRM                                                 Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_MDIO_MDFRM_MD_OP                                 (0U)           /* Received OP */
#define BITL_MDIO_MDFRM_MD_OP                                 (2U)           /* Received OP */
#define BITM_MDIO_MDFRM_MD_OP                                 (0X0003U)      /* Received OP */
#define BITP_MDIO_MDFRM_MD_PHY                                (2U)           /* Received PHYADR. */
#define BITL_MDIO_MDFRM_MD_PHY                                (5U)           /* Received PHYADR. */
#define BITM_MDIO_MDFRM_MD_PHY                                (0X007CU)      /* Received PHYADR. */
#define BITP_MDIO_MDFRM_MD_DEV                                (7U)           /* Received DEVADD */
#define BITL_MDIO_MDFRM_MD_DEV                                (5U)           /* Received DEVADD */
#define BITM_MDIO_MDFRM_MD_DEV                                (0X0F80U)      /* Received DEVADD */

#define ENUM_MDIO_MDFRM_MD_OP_MD_OP_ADF                       (0X0000U)      /* Address Frame. */
#define ENUM_MDIO_MDFRM_MD_OP_MD_OP_WRF                       (0X0001U)      /* Write Frame. */
#define ENUM_MDIO_MDFRM_MD_OP_MD_OP_INCF                      (0X0002U)      /* PostReadIncAdd Frame. */
#define ENUM_MDIO_MDFRM_MD_OP_MD_OP_RDF                       (0X0003U)      /* Read Frame. */

/* ----------------------------------------------------------------------------------------------------
          MDRXD                                                 Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_MDIO_MDRXD_MD_RXD                                (0U)           /* Received Data */
#define BITL_MDIO_MDRXD_MD_RXD                                (16U)          /* Received Data */
#define BITM_MDIO_MDRXD_MD_RXD                                (0XFFFFU)      /* Received Data */

/* ----------------------------------------------------------------------------------------------------
          MDADR                                                 Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_MDIO_MDADR_MD_ADR                                (0U)           /* Received Address */
#define BITL_MDIO_MDADR_MD_ADR                                (16U)          /* Received Address */
#define BITM_MDIO_MDADR_MD_ADR                                (0XFFFFU)      /* Received Address */

/* ----------------------------------------------------------------------------------------------------
          MDTXD                                                 Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_MDIO_MDTXD_MD_TXD                                (0U)           /* TX Data */
#define BITL_MDIO_MDTXD_MD_TXD                                (16U)          /* TX Data */
#define BITM_MDIO_MDTXD_MD_TXD                                (0XFFFFU)      /* TX Data */

/* ----------------------------------------------------------------------------------------------------
          MDPHY                                                 Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_MDIO_MDPHY_MD_PHYSW                              (0U)           /* Software Provided PHYADR */
#define BITL_MDIO_MDPHY_MD_PHYSW                              (5U)           /* Software Provided PHYADR */
#define BITM_MDIO_MDPHY_MD_PHYSW                              (0X001FU)      /* Software Provided PHYADR */
#define BITP_MDIO_MDPHY_MD_PHYSEL                             (5U)           /* Selects Expected PHYADR Bits */
#define BITL_MDIO_MDPHY_MD_PHYSEL                             (5U)           /* Selects Expected PHYADR Bits */
#define BITM_MDIO_MDPHY_MD_PHYSEL                             (0X03E0U)      /* Selects Expected PHYADR Bits */
#define BITP_MDIO_MDPHY_MD_DEVADD                             (10U)          /* Expected DEVADD */
#define BITL_MDIO_MDPHY_MD_DEVADD                             (5U)           /* Expected DEVADD */
#define BITM_MDIO_MDPHY_MD_DEVADD                             (0X7C00U)      /* Expected DEVADD */

/* ----------------------------------------------------------------------------------------------------
          MDSTA                                                 Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_MDIO_MDSTA_MD_WRF                                (0U)           /* Write Frame Status */
#define BITL_MDIO_MDSTA_MD_WRF                                (1U)           /* Write Frame Status */
#define BITM_MDIO_MDSTA_MD_WRF                                (0X0001U)      /* Write Frame Status */
#define BITP_MDIO_MDSTA_MD_ADRF                               (1U)           /* Address Frame Status */
#define BITL_MDIO_MDSTA_MD_ADRF                               (1U)           /* Address Frame Status */
#define BITM_MDIO_MDSTA_MD_ADRF                               (0X0002U)      /* Address Frame Status */
#define BITP_MDIO_MDSTA_MD_INCF                               (2U)           /* Post Read Increment Address Frame Status */
#define BITL_MDIO_MDSTA_MD_INCF                               (1U)           /* Post Read Increment Address Frame Status */
#define BITM_MDIO_MDSTA_MD_INCF                               (0X0004U)      /* Post Read Increment Address Frame Status */
#define BITP_MDIO_MDSTA_MD_RDF                                (3U)           /* Read Frame Status */
#define BITL_MDIO_MDSTA_MD_RDF                                (1U)           /* Read Frame Status */
#define BITM_MDIO_MDSTA_MD_RDF                                (0X0008U)      /* Read Frame Status */
#define BITP_MDIO_MDSTA_MD_DEVM                               (4U)           /* Device Address Matching Status */
#define BITL_MDIO_MDSTA_MD_DEVM                               (1U)           /* Device Address Matching Status */
#define BITM_MDIO_MDSTA_MD_DEVM                               (0X0010U)      /* Device Address Matching Status */
#define BITP_MDIO_MDSTA_MD_DEVN                               (5U)           /* Device Address None Match Status */
#define BITL_MDIO_MDSTA_MD_DEVN                               (1U)           /* Device Address None Match Status */
#define BITM_MDIO_MDSTA_MD_DEVN                               (0X0020U)      /* Device Address None Match Status */
#define BITP_MDIO_MDSTA_MD_PHYM                               (6U)           /* PHY Address Matching Status */
#define BITL_MDIO_MDSTA_MD_PHYM                               (1U)           /* PHY Address Matching Status */
#define BITM_MDIO_MDSTA_MD_PHYM                               (0X0040U)      /* PHY Address Matching Status */
#define BITP_MDIO_MDSTA_MD_PHYN                               (7U)           /* PHY Address Non Matching Status */
#define BITL_MDIO_MDSTA_MD_PHYN                               (1U)           /* PHY Address Non Matching Status */
#define BITM_MDIO_MDSTA_MD_PHYN                               (0X0080U)      /* PHY Address Non Matching Status */

/* ----------------------------------------------------------------------------------------------------
          MDIEN                                                 Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_MDIO_MDIEN_MD_WRFI                               (0U)           /* Interrupt Enable for MD_WRF */
#define BITL_MDIO_MDIEN_MD_WRFI                               (1U)           /* Interrupt Enable for MD_WRF */
#define BITM_MDIO_MDIEN_MD_WRFI                               (0X0001U)      /* Interrupt Enable for MD_WRF */
#define BITP_MDIO_MDIEN_MD_ADRI                               (1U)           /* Interrupt Enable for MD_ADRF */
#define BITL_MDIO_MDIEN_MD_ADRI                               (1U)           /* Interrupt Enable for MD_ADRF */
#define BITM_MDIO_MDIEN_MD_ADRI                               (0X0002U)      /* Interrupt Enable for MD_ADRF */
#define BITP_MDIO_MDIEN_MD_INCFI                              (2U)           /* Interrupt Enable for MD_INCF */
#define BITL_MDIO_MDIEN_MD_INCFI                              (1U)           /* Interrupt Enable for MD_INCF */
#define BITM_MDIO_MDIEN_MD_INCFI                              (0X0004U)      /* Interrupt Enable for MD_INCF */
#define BITP_MDIO_MDIEN_MD_RDFI                               (3U)           /* Interrupt Enable for MD_RDF */
#define BITL_MDIO_MDIEN_MD_RDFI                               (1U)           /* Interrupt Enable for MD_RDF */
#define BITM_MDIO_MDIEN_MD_RDFI                               (0X0008U)      /* Interrupt Enable for MD_RDF */
#define BITP_MDIO_MDIEN_MD_DEVMI                              (4U)           /* Interrupt Enable for MD_DEVM */
#define BITL_MDIO_MDIEN_MD_DEVMI                              (1U)           /* Interrupt Enable for MD_DEVM */
#define BITM_MDIO_MDIEN_MD_DEVMI                              (0X0010U)      /* Interrupt Enable for MD_DEVM */
#define BITP_MDIO_MDIEN_MD_DEVNI                              (5U)           /* Interrupt Enable for MD_DEVN */
#define BITL_MDIO_MDIEN_MD_DEVNI                              (1U)           /* Interrupt Enable for MD_DEVN */
#define BITM_MDIO_MDIEN_MD_DEVNI                              (0X0020U)      /* Interrupt Enable for MD_DEVN */
#define BITP_MDIO_MDIEN_MD_PHYMI                              (6U)           /* Interrupt Enable for MD_PHYM */
#define BITL_MDIO_MDIEN_MD_PHYMI                              (1U)           /* Interrupt Enable for MD_PHYM */
#define BITM_MDIO_MDIEN_MD_PHYMI                              (0X0040U)      /* Interrupt Enable for MD_PHYM */
#define BITP_MDIO_MDIEN_MD_PHYNI                              (7U)           /* Interrupt Enable for MD_PHYN */
#define BITL_MDIO_MDIEN_MD_PHYNI                              (1U)           /* Interrupt Enable for MD_PHYN */
#define BITM_MDIO_MDIEN_MD_PHYNI                              (0X0080U)      /* Interrupt Enable for MD_PHYN */

/* ----------------------------------------------------------------------------------------------------
          MDPIN                                                 Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_MDIO_MDPIN_MD_PIN                                (0U)           /* PRTADR Pins */
#define BITL_MDIO_MDPIN_MD_PIN                                (5U)           /* PRTADR Pins */
#define BITM_MDIO_MDPIN_MD_PIN                                (0X001FU)      /* PRTADR Pins */

/* ----------------------------------------------------------------------------------------------------
          DMAEN                                                 Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_MDIO_DMAEN_RD_DATA                               (0U)           /* Read Data */
#define BITL_MDIO_DMAEN_RD_DATA                               (1U)           /* Read Data */
#define BITM_MDIO_DMAEN_RD_DATA                               (0X0001U)      /* Read Data */
#define BITP_MDIO_DMAEN_INCRD_DATA                            (1U)           /* Increment Read Data */
#define BITL_MDIO_DMAEN_INCRD_DATA                            (1U)           /* Increment Read Data */
#define BITM_MDIO_DMAEN_INCRD_DATA                            (0X0002U)      /* Increment Read Data */
#define BITP_MDIO_DMAEN_WR_ADR                                (2U)           /* Write Address */
#define BITL_MDIO_DMAEN_WR_ADR                                (1U)           /* Write Address */
#define BITM_MDIO_DMAEN_WR_ADR                                (0X0004U)      /* Write Address */
#define BITP_MDIO_DMAEN_WR_DATA                               (3U)           /* Write Data */
#define BITL_MDIO_DMAEN_WR_DATA                               (1U)           /* Write Data */
#define BITM_MDIO_DMAEN_WR_DATA                               (0X0008U)      /* Write Data */

/* ----------------------------------------------------------------------------------------------------
          MDTESTCON                                             Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_MDIO_MDTESTCON_TA_0_VALUE                        (11U)          /* Output Value During First Bit of OP */
#define BITL_MDIO_MDTESTCON_TA_0_VALUE                        (1U)           /* Output Value During First Bit of OP */
#define BITM_MDIO_MDTESTCON_TA_0_VALUE                        (0X0800U)      /* Output Value During First Bit of OP */
#define BITP_MDIO_MDTESTCON_TA_1_VALUE                        (12U)          /* Output Value During Second Bit of OP */
#define BITL_MDIO_MDTESTCON_TA_1_VALUE                        (1U)           /* Output Value During Second Bit of OP */
#define BITM_MDIO_MDTESTCON_TA_1_VALUE                        (0X1000U)      /* Output Value During Second Bit of OP */
#define BITP_MDIO_MDTESTCON_EN_TA_OUTPUT                      (13U)          /* Enable Output Different During OP Phase */
#define BITL_MDIO_MDTESTCON_EN_TA_OUTPUT                      (1U)           /* Enable Output Different During OP Phase */
#define BITM_MDIO_MDTESTCON_EN_TA_OUTPUT                      (0X2000U)      /* Enable Output Different During OP Phase */

#endif  /* end ifndef MDIO_ADDR_RDEF_H_ */


#ifndef DMA_ADDR_RDEF_H_
#define DMA_ADDR_RDEF_H_    /* DMA: DMA */

/* ====================================================================================================
        DMA Module Instances Address and Mask Definitions
   ==================================================================================================== */
#define INST_DMA                                                 (0X40040000U)    /* dma: */

#define MASK_DMA                                                 (0X00000FFFU)    /* DMA: DMA */

/* ====================================================================================================
        DMA Module Register Address Offset Definitions
   ==================================================================================================== */
#define IDX_DMA_STAT                                            (0X000U)    /* DMA Status */
#define IDX_DMA_CFG                                             (0X004U)    /* DMA Configuration */
#define IDX_DMA_PDBPTR                                          (0X008U)    /* DMA Channel Primary Control Data Base Pointer */
#define IDX_DMA_ADBPTR                                          (0X00CU)    /* DMA Channel Alternate Control Data Base Pointer */
#define IDX_DMA_SWREQ                                           (0X014U)    /* DMA Channel Software Request */
#define IDX_DMA_RMSKSET                                         (0X020U)    /* DMA Channel Request Mask Set */
#define IDX_DMA_RMSKCLR                                         (0X024U)    /* DMA Channel Request Mask Clear */
#define IDX_DMA_ENSET                                           (0X028U)    /* DMA Channel Enable Set */
#define IDX_DMA_ENCLR                                           (0X02CU)    /* DMA Channel Enable Clear */
#define IDX_DMA_ALTSET                                          (0X030U)    /* DMA Channel Primary-alternate Set */
#define IDX_DMA_ALTCLR                                          (0X034U)    /* DMA Channel Primary-alternate Clear */
#define IDX_DMA_PRISET                                          (0X038U)    /* DMA Channel Priority Set */
#define IDX_DMA_PRICLR                                          (0X03CU)    /* DMA Channel Priority Clear */
#define IDX_DMA_ERRCHNLCLR                                      (0X048U)    /* DMA per Channel Error Clear */
#define IDX_DMA_ERRCLR                                          (0X04CU)    /* DMA Bus Error Clear */
#define IDX_DMA_INVALIDDESCCLR                                  (0X050U)    /* DMA per Channel Invalid Descriptor Clear */
#define IDX_DMA_BSSET                                           (0X800U)    /* DMA Channel Bytes Swap Enable Set */
#define IDX_DMA_BSCLR                                           (0X804U)    /* DMA Channel Bytes Swap Enable Clear */
#define IDX_DMA_SRCADDRSET                                      (0X810U)    /* DMA Channel Source Address Decrement Enable Set */
#define IDX_DMA_SRCADDRCLR                                      (0X814U)    /* DMA Channel Source Address Decrement Enable Clear */
#define IDX_DMA_DSTADDRSET                                      (0X818U)    /* DMA Channel Destination Address Decrement Enable Set */
#define IDX_DMA_DSTADDRCLR                                      (0X81CU)    /* DMA Channel Destination Address Decrement Enable Clear */
#define IDX_DMA_REVID                                           (0XFE0U)    /* DMA Controller Revision ID */

/* ====================================================================================================
        DMA Module Register ResetValue Definitions
   ==================================================================================================== */
#define RSTVAL_DMA_STAT                                            (0X1E0000) 
#define RSTVAL_DMA_CFG                                             (0X0) 
#define RSTVAL_DMA_PDBPTR                                          (0X0) 
#define RSTVAL_DMA_ADBPTR                                          (0X200) 
#define RSTVAL_DMA_SWREQ                                           (0X0) 
#define RSTVAL_DMA_RMSKSET                                         (0X0) 
#define RSTVAL_DMA_RMSKCLR                                         (0X0) 
#define RSTVAL_DMA_ENSET                                           (0X0) 
#define RSTVAL_DMA_ENCLR                                           (0X0) 
#define RSTVAL_DMA_ALTSET                                          (0X0) 
#define RSTVAL_DMA_ALTCLR                                          (0X0) 
#define RSTVAL_DMA_PRISET                                          (0X0) 
#define RSTVAL_DMA_PRICLR                                          (0X0) 
#define RSTVAL_DMA_ERRCHNLCLR                                      (0X0) 
#define RSTVAL_DMA_ERRCLR                                          (0X0) 
#define RSTVAL_DMA_INVALIDDESCCLR                                  (0X0) 
#define RSTVAL_DMA_BSSET                                           (0X0) 
#define RSTVAL_DMA_BSCLR                                           (0X0) 
#define RSTVAL_DMA_SRCADDRSET                                      (0X0) 
#define RSTVAL_DMA_SRCADDRCLR                                      (0X0) 
#define RSTVAL_DMA_DSTADDRSET                                      (0X0) 
#define RSTVAL_DMA_DSTADDRCLR                                      (0X0) 
#define RSTVAL_DMA_REVID                                           (0X2) 

/* ====================================================================================================
        DMA Module Register BitPositions, Lengths, Masks and Enumerations Definitions
   ==================================================================================================== */

/* ----------------------------------------------------------------------------------------------------
          STAT                                                  Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_DMA_STAT_MEN                                     (0U)           /* Enable Status of the Controller */
#define BITL_DMA_STAT_MEN                                     (1U)           /* Enable Status of the Controller */
#define BITM_DMA_STAT_MEN                                     (0X00000001U)  /* Enable Status of the Controller */
#define BITP_DMA_STAT_CHANM1                                  (16U)          /* Number of Available DMA Channels Minus 1 */
#define BITL_DMA_STAT_CHANM1                                  (5U)           /* Number of Available DMA Channels Minus 1 */
#define BITM_DMA_STAT_CHANM1                                  (0X001F0000U)  /* Number of Available DMA Channels Minus 1 */

/* ----------------------------------------------------------------------------------------------------
          CFG                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_DMA_CFG_MEN                                      (0U)           /* Controller Enable */
#define BITL_DMA_CFG_MEN                                      (1U)           /* Controller Enable */
#define BITM_DMA_CFG_MEN                                      (0X00000001U)  /* Controller Enable */

/* ----------------------------------------------------------------------------------------------------
          PDBPTR                                                Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_DMA_PDBPTR_ADDR                                  (0U)           /* Pointer to the Base Address of the Primary Data Structure */
#define BITL_DMA_PDBPTR_ADDR                                  (32U)          /* Pointer to the Base Address of the Primary Data Structure */
#define BITM_DMA_PDBPTR_ADDR                                  (0XFFFFFFFFU)  /* Pointer to the Base Address of the Primary Data Structure */

/* ----------------------------------------------------------------------------------------------------
          ADBPTR                                                Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_DMA_ADBPTR_ADDR                                  (0U)           /* Base Address of the Alternate Data Structure */
#define BITL_DMA_ADBPTR_ADDR                                  (32U)          /* Base Address of the Alternate Data Structure */
#define BITM_DMA_ADBPTR_ADDR                                  (0XFFFFFFFFU)  /* Base Address of the Alternate Data Structure */

/* ----------------------------------------------------------------------------------------------------
          SWREQ                                                 Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_DMA_SWREQ_CHAN                                   (0U)           /* Generate Software Request */
#define BITL_DMA_SWREQ_CHAN                                   (31U)          /* Generate Software Request */
#define BITM_DMA_SWREQ_CHAN                                   (0X7FFFFFFFU)  /* Generate Software Request */

/* ----------------------------------------------------------------------------------------------------
          RMSKSET                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_DMA_RMSKSET_CHAN                                 (0U)           /* Mask Requests from DMA Channels */
#define BITL_DMA_RMSKSET_CHAN                                 (31U)          /* Mask Requests from DMA Channels */
#define BITM_DMA_RMSKSET_CHAN                                 (0X7FFFFFFFU)  /* Mask Requests from DMA Channels */

/* ----------------------------------------------------------------------------------------------------
          RMSKCLR                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_DMA_RMSKCLR_CHAN                                 (0U)           /* Clear REQ_MASK_SET Bits in DMARMSKSET */
#define BITL_DMA_RMSKCLR_CHAN                                 (31U)          /* Clear REQ_MASK_SET Bits in DMARMSKSET */
#define BITM_DMA_RMSKCLR_CHAN                                 (0X7FFFFFFFU)  /* Clear REQ_MASK_SET Bits in DMARMSKSET */

/* ----------------------------------------------------------------------------------------------------
          ENSET                                                 Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_DMA_ENSET_CHAN                                   (0U)           /* Enable DMA Channels */
#define BITL_DMA_ENSET_CHAN                                   (31U)          /* Enable DMA Channels */
#define BITM_DMA_ENSET_CHAN                                   (0X7FFFFFFFU)  /* Enable DMA Channels */

/* ----------------------------------------------------------------------------------------------------
          ENCLR                                                 Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_DMA_ENCLR_CHAN                                   (0U)           /* Disable DMA Channels */
#define BITL_DMA_ENCLR_CHAN                                   (31U)          /* Disable DMA Channels */
#define BITM_DMA_ENCLR_CHAN                                   (0X7FFFFFFFU)  /* Disable DMA Channels */

/* ----------------------------------------------------------------------------------------------------
          ALTSET                                                Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_DMA_ALTSET_CHAN                                  (0U)           /* Control Structure Status / Select Alt Structure */
#define BITL_DMA_ALTSET_CHAN                                  (31U)          /* Control Structure Status / Select Alt Structure */
#define BITM_DMA_ALTSET_CHAN                                  (0X7FFFFFFFU)  /* Control Structure Status / Select Alt Structure */

/* ----------------------------------------------------------------------------------------------------
          ALTCLR                                                Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_DMA_ALTCLR_CHAN                                  (0U)           /* Select Primary Data Structure */
#define BITL_DMA_ALTCLR_CHAN                                  (31U)          /* Select Primary Data Structure */
#define BITM_DMA_ALTCLR_CHAN                                  (0X7FFFFFFFU)  /* Select Primary Data Structure */

/* ----------------------------------------------------------------------------------------------------
          PRISET                                                Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_DMA_PRISET_CHAN                                  (0U)           /* Configure Channel for High Priority */
#define BITL_DMA_PRISET_CHAN                                  (31U)          /* Configure Channel for High Priority */
#define BITM_DMA_PRISET_CHAN                                  (0X7FFFFFFFU)  /* Configure Channel for High Priority */

/* ----------------------------------------------------------------------------------------------------
          PRICLR                                                Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_DMA_PRICLR_CHPRICLR                              (0U)           /* Configure Channel for Default Priority Level */
#define BITL_DMA_PRICLR_CHPRICLR                              (31U)          /* Configure Channel for Default Priority Level */
#define BITM_DMA_PRICLR_CHPRICLR                              (0X7FFFFFFFU)  /* Configure Channel for Default Priority Level */

/* ----------------------------------------------------------------------------------------------------
          ERRCHNLCLR                                            Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_DMA_ERRCHNLCLR_CHAN                              (0U)           /* Per Channel Bus Error Status/ Clear */
#define BITL_DMA_ERRCHNLCLR_CHAN                              (31U)          /* Per Channel Bus Error Status/ Clear */
#define BITM_DMA_ERRCHNLCLR_CHAN                              (0X7FFFFFFFU)  /* Per Channel Bus Error Status/ Clear */

/* ----------------------------------------------------------------------------------------------------
          ERRCLR                                                Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_DMA_ERRCLR_CHAN                                  (0U)           /* Bus Error Status */
#define BITL_DMA_ERRCLR_CHAN                                  (31U)          /* Bus Error Status */
#define BITM_DMA_ERRCLR_CHAN                                  (0X7FFFFFFFU)  /* Bus Error Status */

/* ----------------------------------------------------------------------------------------------------
          INVALIDDESCCLR                                        Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_DMA_INVALIDDESCCLR_CHAN                          (0U)           /* Per Channel Invalid Descriptor Status */
#define BITL_DMA_INVALIDDESCCLR_CHAN                          (31U)          /* Per Channel Invalid Descriptor Status */
#define BITM_DMA_INVALIDDESCCLR_CHAN                          (0X7FFFFFFFU)  /* Per Channel Invalid Descriptor Status */

/* ----------------------------------------------------------------------------------------------------
          BSSET                                                 Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_DMA_BSSET_CHAN                                   (0U)           /* Byte Swap Status */
#define BITL_DMA_BSSET_CHAN                                   (31U)          /* Byte Swap Status */
#define BITM_DMA_BSSET_CHAN                                   (0X7FFFFFFFU)  /* Byte Swap Status */

/* ----------------------------------------------------------------------------------------------------
          BSCLR                                                 Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_DMA_BSCLR_CHAN                                   (0U)           /* Disable Byte Swap */
#define BITL_DMA_BSCLR_CHAN                                   (31U)          /* Disable Byte Swap */
#define BITM_DMA_BSCLR_CHAN                                   (0X7FFFFFFFU)  /* Disable Byte Swap */

/* ----------------------------------------------------------------------------------------------------
          SRCADDRSET                                            Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_DMA_SRCADDRSET_CHAN                              (0U)           /* Source Address Decrement Status / Configure */
#define BITL_DMA_SRCADDRSET_CHAN                              (31U)          /* Source Address Decrement Status / Configure */
#define BITM_DMA_SRCADDRSET_CHAN                              (0X7FFFFFFFU)  /* Source Address Decrement Status / Configure */

/* ----------------------------------------------------------------------------------------------------
          SRCADDRCLR                                            Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_DMA_SRCADDRCLR_CHAN                              (0U)           /* Disable Source Address Decrement */
#define BITL_DMA_SRCADDRCLR_CHAN                              (31U)          /* Disable Source Address Decrement */
#define BITM_DMA_SRCADDRCLR_CHAN                              (0X7FFFFFFFU)  /* Disable Source Address Decrement */

/* ----------------------------------------------------------------------------------------------------
          DSTADDRSET                                            Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_DMA_DSTADDRSET_CHAN                              (0U)           /* Destination Address Decrement Status */
#define BITL_DMA_DSTADDRSET_CHAN                              (31U)          /* Destination Address Decrement Status */
#define BITM_DMA_DSTADDRSET_CHAN                              (0X7FFFFFFFU)  /* Destination Address Decrement Status */

/* ----------------------------------------------------------------------------------------------------
          DSTADDRCLR                                            Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_DMA_DSTADDRCLR_CHAN                              (0U)           /* Disable Destination Address Decrement */
#define BITL_DMA_DSTADDRCLR_CHAN                              (31U)          /* Disable Destination Address Decrement */
#define BITM_DMA_DSTADDRCLR_CHAN                              (0X7FFFFFFFU)  /* Disable Destination Address Decrement */

/* ----------------------------------------------------------------------------------------------------
          REVID                                                 Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_DMA_REVID_DMAREVID                               (0U)           /* DMA Controller Revision ID */
#define BITL_DMA_REVID_DMAREVID                               (8U)           /* DMA Controller Revision ID */
#define BITM_DMA_REVID_DMAREVID                               (0X000000FFU)  /* DMA Controller Revision ID */

#endif  /* end ifndef DMA_ADDR_RDEF_H_ */


#ifndef CC_ADDR_RDEF_H_
#define CC_ADDR_RDEF_H_    /* CC: Cache Controller */

/* ====================================================================================================
        CC Module Instances Address and Mask Definitions
   ==================================================================================================== */
#define INST_CACHE                                               (0X40044000U)    /* cache: */

#define MASK_CC                                                  (0X000000FFU)    /* CC: Cache Controller */

/* ====================================================================================================
        CC Module Register Address Offset Definitions
   ==================================================================================================== */
#define IDX_CC_STAT                                             (0X00U)    /* Cache Status Register */
#define IDX_CC_SETUP                                            (0X04U)    /* Cache Setup Register */
#define IDX_CC_KEY                                              (0X08U)    /* Cache Key Register */
#define IDX_CC_PERFSETUP                                        (0X0CU)    /* Cache Performance Monitor Setup Register */
#define IDX_CC_ACCESSCNTR                                       (0X10U)    /* Cache Miss Counter */
#define IDX_CC_MSTRSETUP                                        (0X24U)    /* Cache Master Setup Register */
#define IDX_CC_ECCSTAT                                          (0X34U)    /* Cache SRAM ECC Status Register */
#define IDX_CC_ECCADDR                                          (0X38U)    /* Cache SRAM ECC Address Register */

/* ====================================================================================================
        CC Module Register ResetValue Definitions
   ==================================================================================================== */
#define RSTVAL_CC_STAT                                             (0X0) 
#define RSTVAL_CC_SETUP                                            (0X0) 
#define RSTVAL_CC_KEY                                              (0X0) 
#define RSTVAL_CC_PERFSETUP                                        (0X0) 
#define RSTVAL_CC_ACCESSCNTR                                       (0X0) 
#define RSTVAL_CC_MSTRSETUP                                        (0X0) 
#define RSTVAL_CC_ECCSTAT                                          (0X0) 
#define RSTVAL_CC_ECCADDR                                          (0X0) 

/* ====================================================================================================
        CC Module Register BitPositions, Lengths, Masks and Enumerations Definitions
   ==================================================================================================== */

/* ----------------------------------------------------------------------------------------------------
          STAT                                                  Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_CC_STAT_CCEN                                     (0U)           /* Code Cache Enable Status */
#define BITL_CC_STAT_CCEN                                     (1U)           /* Code Cache Enable Status */
#define BITM_CC_STAT_CCEN                                     (0X00000001U)  /* Code Cache Enable Status */
#define BITP_CC_STAT_CCLCK                                    (1U)           /* Code Cache Lock Status */
#define BITL_CC_STAT_CCLCK                                    (1U)           /* Code Cache Lock Status */
#define BITM_CC_STAT_CCLCK                                    (0X00000002U)  /* Code Cache Lock Status */
#define BITP_CC_STAT_CCWIPE                                   (3U)           /* Code Cache Memory Wipe in Progress */
#define BITL_CC_STAT_CCWIPE                                   (1U)           /* Code Cache Memory Wipe in Progress */
#define BITM_CC_STAT_CCWIPE                                   (0X00000008U)  /* Code Cache Memory Wipe in Progress */

/* ----------------------------------------------------------------------------------------------------
          SETUP                                                 Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_CC_SETUP_CCEN                                    (0U)           /* Code Cache Enable */
#define BITL_CC_SETUP_CCEN                                    (1U)           /* Code Cache Enable */
#define BITM_CC_SETUP_CCEN                                    (0X00000001U)  /* Code Cache Enable */
#define BITP_CC_SETUP_CCFLUSHDIS                              (8U)           /* Disable Automatic Cache and Buffer Flush on Any Flash Update */
#define BITL_CC_SETUP_CCFLUSHDIS                              (1U)           /* Disable Automatic Cache and Buffer Flush on Any Flash Update */
#define BITM_CC_SETUP_CCFLUSHDIS                              (0X00000100U)  /* Disable Automatic Cache and Buffer Flush on Any Flash Update */

/* ----------------------------------------------------------------------------------------------------
          KEY                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_CC_KEY_KEY                                       (0U)           /* Cache Key Register */
#define BITL_CC_KEY_KEY                                       (32U)          /* Cache Key Register */
#define BITM_CC_KEY_KEY                                       (0XFFFFFFFFU)  /* Cache Key Register */

/* ----------------------------------------------------------------------------------------------------
          PERFSETUP                                             Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_CC_PERFSETUP_STOPCNTR                            (0U)           /* Performance Counter Stop */
#define BITL_CC_PERFSETUP_STOPCNTR                            (1U)           /* Performance Counter Stop */
#define BITM_CC_PERFSETUP_STOPCNTR                            (0X00000001U)  /* Performance Counter Stop */
#define BITP_CC_PERFSETUP_STRTCNTR                            (1U)           /* Performance Counter Start */
#define BITL_CC_PERFSETUP_STRTCNTR                            (1U)           /* Performance Counter Start */
#define BITM_CC_PERFSETUP_STRTCNTR                            (0X00000002U)  /* Performance Counter Start */

/* ----------------------------------------------------------------------------------------------------
          ACCESSCNTR                                            Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_CC_ACCESSCNTR_CNT                                (0U)           /* Cache Internal Performance Counter */
#define BITL_CC_ACCESSCNTR_CNT                                (32U)          /* Cache Internal Performance Counter */
#define BITM_CC_ACCESSCNTR_CNT                                (0XFFFFFFFFU)  /* Cache Internal Performance Counter */

/* ----------------------------------------------------------------------------------------------------
          MSTRSETUP                                             Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_CC_MSTRSETUP_MSTRNRA                             (0U)           /* No Read Allocate for Master */
#define BITL_CC_MSTRSETUP_MSTRNRA                             (8U)           /* No Read Allocate for Master */
#define BITM_CC_MSTRSETUP_MSTRNRA                             (0X000000FFU)  /* No Read Allocate for Master */

/* ----------------------------------------------------------------------------------------------------
          ECCSTAT                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_CC_ECCSTAT_ECCINTSTA                             (0U)           /* Cache SRAM ECC Error Interrupt Status */
#define BITL_CC_ECCSTAT_ECCINTSTA                             (2U)           /* Cache SRAM ECC Error Interrupt Status */
#define BITM_CC_ECCSTAT_ECCINTSTA                             (0X00000003U)  /* Cache SRAM ECC Error Interrupt Status */
#define BITP_CC_ECCSTAT_ECCHRESPSTA                           (2U)           /* Cache SRAM ECC Error Hresp Status */
#define BITL_CC_ECCSTAT_ECCHRESPSTA                           (2U)           /* Cache SRAM ECC Error Hresp Status */
#define BITM_CC_ECCSTAT_ECCHRESPSTA                           (0X0000000CU)  /* Cache SRAM ECC Error Hresp Status */
#define BITP_CC_ECCSTAT_ECCERRORCNT                           (4U)           /* Cache SRAM ECC Error Counter */
#define BITL_CC_ECCSTAT_ECCERRORCNT                           (3U)           /* Cache SRAM ECC Error Counter */
#define BITM_CC_ECCSTAT_ECCERRORCNT                           (0X00000070U)  /* Cache SRAM ECC Error Counter */

#define ENUM_CC_ECCSTAT_ECCHRESPSTA_NOERROR                   (0X00000000U)  /* No Error */
#define ENUM_CC_ECCSTAT_ECCHRESPSTA_ERROR2BIT                 (0X00000001U)  /* 2 Bit Error */
#define ENUM_CC_ECCSTAT_ECCHRESPSTA_ERROR1BIT                 (0X00000002U)  /* 1 Bit Error */
#define ENUM_CC_ECCSTAT_ECCHRESPSTA_RESERVED                  (0X00000003U)  /* Reserved */
#define ENUM_CC_ECCSTAT_ECCINTSTA_NOERROR                     (0X00000000U)  /* No Error */
#define ENUM_CC_ECCSTAT_ECCINTSTA_ERROR2BIT                   (0X00000001U)  /* 2 Bit Error */
#define ENUM_CC_ECCSTAT_ECCINTSTA_ERROR1BIT                   (0X00000002U)  /* 1 Bit Error */
#define ENUM_CC_ECCSTAT_ECCINTSTA_ERROR1OR2BIT                (0X00000003U)  /* Either 1 Bit Error or 2 Bit Error */

/* ----------------------------------------------------------------------------------------------------
          ECCADDR                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_CC_ECCADDR_ECCADDR                               (0U)           /* Cache SRAM ECC Error Interrupt Address */
#define BITL_CC_ECCADDR_ECCADDR                               (11U)          /* Cache SRAM ECC Error Interrupt Address */
#define BITM_CC_ECCADDR_ECCADDR                               (0X000007FFU)  /* Cache SRAM ECC Error Interrupt Address */

#endif  /* end ifndef CC_ADDR_RDEF_H_ */


#ifndef FLASH_ADDR_RDEF_H_
#define FLASH_ADDR_RDEF_H_    /* FLASH: Flash Controller */

/* ====================================================================================================
        FLASH Module Instances Address and Mask Definitions
   ==================================================================================================== */
#define INST_FLASH                                               (0X40048000U)    /* flash: */

#define MASK_FLASH                                               (0XFFFFFFFFU)    /* FLASH: Flash Controller */

/* ====================================================================================================
        FLASH Module Register Address Offset Definitions
   ==================================================================================================== */
#define IDX_FLASH_FEESTA                                        (0X00000000U)    /* Status Register */
#define IDX_FLASH_FEECON0                                       (0X00000004U)    /* Command Control Register – Interrupt Enable Register */
#define IDX_FLASH_FEECMD                                        (0X00000008U)    /* Command Register */
#define IDX_FLASH_FEEFLADR                                      (0X0000000CU)    /* Flash Address Key - Hole Register */
#define IDX_FLASH_FEEFLDATA0                                    (0X00000010U)    /* Flash Data Register - Key - Hole Interface Lower 32 Bits */
#define IDX_FLASH_FEEFLDATA1                                    (0X00000014U)    /* Flash Data Register - Key - Hole Interface Upper 32 Bits */
#define IDX_FLASH_FEEADR0                                       (0X00000018U)    /* Lower Page Address */
#define IDX_FLASH_FEEADR1                                       (0X0000001CU)    /* Upper Page Address */
#define IDX_FLASH_FEEKEY                                        (0X00000020U)    /* Flash Key Register. */
#define IDX_FLASH_FEEPRO0                                       (0X00000028U)    /* Write Protection Register for Flash0 */
#define IDX_FLASH_FEEPRO1                                       (0X0000002CU)    /* Write Protection Register for Flash1 */
#define IDX_FLASH_FEESIG                                        (0X00000034U)    /* Flash Signature */
#define IDX_FLASH_FEECON1                                       (0X00000038U)    /* User Setup Register */
#define IDX_FLASH_FEEWRADDRA                                    (0X00000040U)    /* Write Abort Address Register */
#define IDX_FLASH_FEEAEN0                                       (0X00000048U)    /* Lower 32 Bits of the Sys Irq Abort Enable Register. */
#define IDX_FLASH_FEEAEN1                                       (0X0000004CU)    /* Middle 32 Bits of the Sys Irq Abort Enable Register. */
#define IDX_FLASH_FEEAEN2                                       (0X00000050U)    /* Upper 32 Bits of the Sys Irq Abort Enable Register. */
#define IDX_FLASH_FEEECCCONFIG                                  (0X00000064U)    /* Configurable ECC Enable/disable, Error Response */
#define IDX_FLASH_FEEECCADDRC0                                  (0X00000074U)    /* Flash 0 ECC Error Address via CODE Bus */
#define IDX_FLASH_FEEECCADDRC1                                  (0X00000078U)    /* Flash 1 ECC Error Address via CODE Bus */
#define IDX_FLASH_FEEECCADDRD0                                  (0X00000094U)    /* Flash 0 ECC Error Address via DMA Bus */
#define IDX_FLASH_FEEECCADDRD1                                  (0X00000098U)    /* Flash 1 ECC Error Address via DMA Bus */

/* ====================================================================================================
        FLASH Module Register ResetValue Definitions
   ==================================================================================================== */
#define RSTVAL_FLASH_FEESTA                                        (0X0) 
#define RSTVAL_FLASH_FEECON0                                       (0X0) 
#define RSTVAL_FLASH_FEECMD                                        (0X0) 
#define RSTVAL_FLASH_FEEFLADR                                      (0X0) 
#define RSTVAL_FLASH_FEEFLDATA0                                    (0X0) 
#define RSTVAL_FLASH_FEEFLDATA1                                    (0X0) 
#define RSTVAL_FLASH_FEEADR0                                       (0X0) 
#define RSTVAL_FLASH_FEEADR1                                       (0X0) 
#define RSTVAL_FLASH_FEEKEY                                        (0X0) 
#define RSTVAL_FLASH_FEEPRO0                                       (0XFFFFFFFF) 
#define RSTVAL_FLASH_FEEPRO1                                       (0XFFFFFFFF) 
#define RSTVAL_FLASH_FEECON1                                       (0X0) 
#define RSTVAL_FLASH_FEEAEN0                                       (0X0) 
#define RSTVAL_FLASH_FEEAEN1                                       (0X0) 
#define RSTVAL_FLASH_FEEAEN2                                       (0X0) 
#define RSTVAL_FLASH_FEEECCCONFIG                                  (0X12) 
#define RSTVAL_FLASH_FEEECCADDRC0                                  (0X0) 
#define RSTVAL_FLASH_FEEECCADDRC1                                  (0X0) 
#define RSTVAL_FLASH_FEEECCADDRD0                                  (0X0) 
#define RSTVAL_FLASH_FEEECCADDRD1                                  (0X0) 

/* ====================================================================================================
        FLASH Module Register BitPositions, Lengths, Masks and Enumerations Definitions
   ==================================================================================================== */

/* ----------------------------------------------------------------------------------------------------
          FEESTA                                                Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_FLASH_FEESTA_CMDBUSY                             (0U)           /* Command Busy. */
#define BITL_FLASH_FEESTA_CMDBUSY                             (1U)           /* Command Busy. */
#define BITM_FLASH_FEESTA_CMDBUSY                             (0X00000001U)  /* Command Busy. */
#define BITP_FLASH_FEESTA_WRCLOSE                             (1U)           /* Key-hole Registers Closed for Access */
#define BITL_FLASH_FEESTA_WRCLOSE                             (1U)           /* Key-hole Registers Closed for Access */
#define BITM_FLASH_FEESTA_WRCLOSE                             (0X00000002U)  /* Key-hole Registers Closed for Access */
#define BITP_FLASH_FEESTA_CMDCOMP                             (2U)           /* Command Complete */
#define BITL_FLASH_FEESTA_CMDCOMP                             (1U)           /* Command Complete */
#define BITM_FLASH_FEESTA_CMDCOMP                             (0X00000004U)  /* Command Complete */
#define BITP_FLASH_FEESTA_WRALCOMP                            (3U)           /* Write Almost Complete – Key-hole Registers Open for Access */
#define BITL_FLASH_FEESTA_WRALCOMP                            (1U)           /* Write Almost Complete – Key-hole Registers Open for Access */
#define BITM_FLASH_FEESTA_WRALCOMP                            (0X00000008U)  /* Write Almost Complete – Key-hole Registers Open for Access */
#define BITP_FLASH_FEESTA_CMDFAIL                             (4U)           /* Command Failed */
#define BITL_FLASH_FEESTA_CMDFAIL                             (2U)           /* Command Failed */
#define BITM_FLASH_FEESTA_CMDFAIL                             (0X00000030U)  /* Command Failed */
#define BITP_FLASH_FEESTA_ECCERRCMD                           (7U)           /* ECC Errors Produced During Signature Commands */
#define BITL_FLASH_FEESTA_ECCERRCMD                           (2U)           /* ECC Errors Produced During Signature Commands */
#define BITM_FLASH_FEESTA_ECCERRCMD                           (0X00000180U)  /* ECC Errors Produced During Signature Commands */
#define BITP_FLASH_FEESTA_ECCREADERRFLSH0                     (9U)           /* ECC Interrupt Errors During AHB Read to Flash 0. */
#define BITL_FLASH_FEESTA_ECCREADERRFLSH0                     (2U)           /* ECC Interrupt Errors During AHB Read to Flash 0. */
#define BITM_FLASH_FEESTA_ECCREADERRFLSH0                     (0X00000600U)  /* ECC Interrupt Errors During AHB Read to Flash 0. */
#define BITP_FLASH_FEESTA_ECCREADERRFLSH1                     (11U)          /* ECC Interrupt Errors During AHB Read to Flash 1. */
#define BITL_FLASH_FEESTA_ECCREADERRFLSH1                     (2U)           /* ECC Interrupt Errors During AHB Read to Flash 1. */
#define BITM_FLASH_FEESTA_ECCREADERRFLSH1                     (0X00001800U)  /* ECC Interrupt Errors During AHB Read to Flash 1. */
#define BITP_FLASH_FEESTA_SIGNERR                             (13U)          /* Initial Signature Check Error on Info Space */
#define BITL_FLASH_FEESTA_SIGNERR                             (1U)           /* Initial Signature Check Error on Info Space */
#define BITM_FLASH_FEESTA_SIGNERR                             (0X00002000U)  /* Initial Signature Check Error on Info Space */
#define BITP_FLASH_FEESTA_INIT                                (14U)          /* Initialization Upload in Progress. */
#define BITL_FLASH_FEESTA_INIT                                (1U)           /* Initialization Upload in Progress. */
#define BITM_FLASH_FEESTA_INIT                                (0X00004000U)  /* Initialization Upload in Progress. */
#define BITP_FLASH_FEESTA_ECCERRINITSIGN                      (15U)          /* ECC Error on Initial Info Signature Check */
#define BITL_FLASH_FEESTA_ECCERRINITSIGN                      (2U)           /* ECC Error on Initial Info Signature Check */
#define BITM_FLASH_FEESTA_ECCERRINITSIGN                      (0X00018000U)  /* ECC Error on Initial Info Signature Check */
#define BITP_FLASH_FEESTA_ECCERRCNTC0                         (17U)          /* ECC Error Count via CODE Bus of Flash 0 */
#define BITL_FLASH_FEESTA_ECCERRCNTC0                         (3U)           /* ECC Error Count via CODE Bus of Flash 0 */
#define BITM_FLASH_FEESTA_ECCERRCNTC0                         (0X000E0000U)  /* ECC Error Count via CODE Bus of Flash 0 */
#define BITP_FLASH_FEESTA_ECCERRCNTD0                         (20U)          /* ECC Error Count via DMA Bus of Flash 0 */
#define BITL_FLASH_FEESTA_ECCERRCNTD0                         (2U)           /* ECC Error Count via DMA Bus of Flash 0 */
#define BITM_FLASH_FEESTA_ECCERRCNTD0                         (0X00300000U)  /* ECC Error Count via DMA Bus of Flash 0 */
#define BITP_FLASH_FEESTA_ECCERRCNTC1                         (22U)          /* ECC Error Count via CODE Bus of Flash 1 */
#define BITL_FLASH_FEESTA_ECCERRCNTC1                         (3U)           /* ECC Error Count via CODE Bus of Flash 1 */
#define BITM_FLASH_FEESTA_ECCERRCNTC1                         (0X01C00000U)  /* ECC Error Count via CODE Bus of Flash 1 */
#define BITP_FLASH_FEESTA_ECCHRESPCODE                        (25U)          /* ECC Error Response on CODE Bus */
#define BITL_FLASH_FEESTA_ECCHRESPCODE                        (2U)           /* ECC Error Response on CODE Bus */
#define BITM_FLASH_FEESTA_ECCHRESPCODE                        (0X06000000U)  /* ECC Error Response on CODE Bus */
#define BITP_FLASH_FEESTA_ECCHRESPDMA                         (27U)          /* ECC Error Response on DMA Bus */
#define BITL_FLASH_FEESTA_ECCHRESPDMA                         (2U)           /* ECC Error Response on DMA Bus */
#define BITM_FLASH_FEESTA_ECCHRESPDMA                         (0X18000000U)  /* ECC Error Response on DMA Bus */
#define BITP_FLASH_FEESTA_ECCERRCNTD1                         (29U)          /* ECC Error Count via DMA Bus of Flash 1 */
#define BITL_FLASH_FEESTA_ECCERRCNTD1                         (2U)           /* ECC Error Count via DMA Bus of Flash 1 */
#define BITM_FLASH_FEESTA_ECCERRCNTD1                         (0X60000000U)  /* ECC Error Count via DMA Bus of Flash 1 */

#define ENUM_FLASH_FEESTA_ECCHRESPDMA_NOERROR                 (0X00000000U)  /* No Error. Successful Read from Program Flash via AHB Bus */
#define ENUM_FLASH_FEESTA_ECCHRESPDMA_ERROR2BIT               (0X00000001U)  /* During AHB Read to Flash, 2 Bit Error Detected, Not Corrected. */
#define ENUM_FLASH_FEESTA_ECCHRESPDMA_ERROR1BIT               (0X00000002U)  /* 1 Bit Error is Corrected for One Flash Location While Doing AHB Read to Program Flash */
#define ENUM_FLASH_FEESTA_ECCHRESPDMA_RESERVED                (0X00000003U)  /* Reserved */
#define ENUM_FLASH_FEESTA_ECCHRESPCODE_NOERROR                (0X00000000U)  /* No Error. Successful Read from Program Flash via AHB Bus */
#define ENUM_FLASH_FEESTA_ECCHRESPCODE_ERROR2BIT              (0X00000001U)  /* During AHB Read to Flash, 2 Bit Error Detected, Not Corrected. */
#define ENUM_FLASH_FEESTA_ECCHRESPCODE_ERROR1BIT              (0X00000002U)  /* 1 Bit Error is Corrected for One Flash Location While Doing AHB Read to Program Flash */
#define ENUM_FLASH_FEESTA_ECCHRESPCODE_RESERVED               (0X00000003U)  /* Reserved */
#define ENUM_FLASH_FEESTA_ECCERRINITSIGN_NOERROR              (0X00000000U)  /* No Error, Successful Flash Read Operation During Initial Signature Check, Page Signature Check */
#define ENUM_FLASH_FEESTA_ECCERRINITSIGN_ERROR2BIT            (0X00000001U)  /* During Initial Signature Check, 2 Bit Error Detected, Not Corrected for at Least One Flash Location */
#define ENUM_FLASH_FEESTA_ECCERRINITSIGN_ERROR1BIT            (0X00000002U)  /* 1 Bit Error is Corrected for One Flash Location While Doing Signature Commands */
#define ENUM_FLASH_FEESTA_ECCERRINITSIGN_ERROR1OR2BIT         (0X00000003U)  /* During Initial Signature Command, 1 Bit Error and 2 Bit Errors are Detected on One or More Flash Locations */
#define ENUM_FLASH_FEESTA_ECCREADERRFLSH1_NOERROR             (0X00000000U)  /* No Error. Successful Read from Data Flash via AHB Bus */
#define ENUM_FLASH_FEESTA_ECCREADERRFLSH1_ERROR2BIT           (0X00000001U)  /* 1 Bit Error is Corrected for One Flash Location While Doing AHB Read to Data Flash */
#define ENUM_FLASH_FEESTA_ECCREADERRFLSH1_ERROR1BIT           (0X00000002U)  /* During AHB Read to Flash, 2 Bit Error Detected, Not Corrected. */
#define ENUM_FLASH_FEESTA_ECCREADERRFLSH1_ERROR1OR2BIT        (0X00000003U)  /* During AHB Read, It is Either ECC Error or ECC Corrected Error Only. but This Condition Can Occur If the Consecutive Access Had AHB HRESP Error Due to ECC Error, Before the Status Register is Read. */
#define ENUM_FLASH_FEESTA_ECCREADERRFLSH0_NOERROR             (0X00000000U)  /* No Error. Successful Read from Program Flash via AHB Bus */
#define ENUM_FLASH_FEESTA_ECCREADERRFLSH0_ERROR2BIT           (0X00000001U)  /* During AHB Read to Flash, 2 Bit Error Detected, Not Corrected. */
#define ENUM_FLASH_FEESTA_ECCREADERRFLSH0_ERROR1BIT           (0X00000002U)  /* 1 Bit Error is Corrected for One Flash Location While Doing AHB Read to Program Flash */
#define ENUM_FLASH_FEESTA_ECCREADERRFLSH0_ERROR1OR2BIT        (0X00000003U)  /* During AHB Read, It is Either ECC Error or ECC Corrected Error Only. but This Condition Can Occur If the Consecutive Access Had AHB HRESP Error Due to ECC Error, Before the Status Register is Read. */
#define ENUM_FLASH_FEESTA_ECCERRCMD_NOERROR                   (0X00000000U)  /* No Error, Successful Flash Read Operation During Signature Check */
#define ENUM_FLASH_FEESTA_ECCERRCMD_ERROR2BIT                 (0X00000001U)  /* During Signature Commands, 2 Bit Error is Detected on One or More Flash Locations, Not Corrected. */
#define ENUM_FLASH_FEESTA_ECCERRCMD_ERROR1BIT                 (0X00000002U)  /* 1 Bit Error is Corrected for One or More Flash Locations While Doing Signature Commands */
#define ENUM_FLASH_FEESTA_ECCERRCMD_ERROR1OR2BIT              (0X00000003U)  /* During Signature Commands, 1 Bit Error and 2 Bit Errors are Detected on One or More Flash Locations */
#define ENUM_FLASH_FEESTA_CMDFAIL_SUCCESS                     (0X00000000U)  /* Successful Completion of a Command or a Write */
#define ENUM_FLASH_FEESTA_CMDFAIL_DENIED                      (0X00000001U)  /* Attempted Signcheck,write or Erase of a Protected Location or Out of Memory Location. the Command is Ignored. */
#define ENUM_FLASH_FEESTA_CMDFAIL_VERIFYERR                   (0X00000002U)  /* Read Verify Error */
#define ENUM_FLASH_FEESTA_CMDFAIL_ABORTED                     (0X00000003U)  /* Indicates That a Command or a Write Was Aborted by an Abort Command or a System Interrupt Has Caused an Abort */

/* ----------------------------------------------------------------------------------------------------
          FEECON0                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_FLASH_FEECON0_IENCMD                             (0U)           /* Command Complete Interrupt Enable */
#define BITL_FLASH_FEECON0_IENCMD                             (1U)           /* Command Complete Interrupt Enable */
#define BITM_FLASH_FEECON0_IENCMD                             (0X00000001U)  /* Command Complete Interrupt Enable */
#define BITP_FLASH_FEECON0_IWRALCOMP                          (1U)           /* Write Almost Complete Interrupt Enable */
#define BITL_FLASH_FEECON0_IWRALCOMP                          (1U)           /* Write Almost Complete Interrupt Enable */
#define BITM_FLASH_FEECON0_IWRALCOMP                          (0X00000002U)  /* Write Almost Complete Interrupt Enable */
#define BITP_FLASH_FEECON0_IENERR                             (2U)           /* Command Fail Interrupt Enable */
#define BITL_FLASH_FEECON0_IENERR                             (1U)           /* Command Fail Interrupt Enable */
#define BITM_FLASH_FEECON0_IENERR                             (0X00000004U)  /* Command Fail Interrupt Enable */

/* ----------------------------------------------------------------------------------------------------
          FEECMD                                                Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_FLASH_FEECMD_CMD                                 (0U)           /* Commands */
#define BITL_FLASH_FEECMD_CMD                                 (5U)           /* Commands */
#define BITM_FLASH_FEECMD_CMD                                 (0X0000001FU)  /* Commands */

#define ENUM_FLASH_FEECMD_CMD_IDLE                            (0X00000000U)  /* IDLE */
#define ENUM_FLASH_FEECMD_CMD_ERASEPAGE                       (0X00000001U)  /* ERASEPAGE */
#define ENUM_FLASH_FEECMD_CMD_SIGN                            (0X00000002U)  /* SIGN */
#define ENUM_FLASH_FEECMD_CMD_WRITE                           (0X00000004U)  /* WRITE */
#define ENUM_FLASH_FEECMD_CMD_MASSERASE_ACTIVE                (0X00000005U)  /* MASSERASE_ACTIVE */
#define ENUM_FLASH_FEECMD_CMD_MASSERASE_PASSIVE               (0X00000006U)  /* MASSERASE_PASSIVE */
#define ENUM_FLASH_FEECMD_CMD_ABORT                           (0X00000008U)  /* ABORT */

/* ----------------------------------------------------------------------------------------------------
          FEEFLADR                                              Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_FLASH_FEEFLADR_FLADDR                            (3U)           /* Memory Mapped Address for the Flash Location */
#define BITL_FLASH_FEEFLADR_FLADDR                            (18U)          /* Memory Mapped Address for the Flash Location */
#define BITM_FLASH_FEEFLADR_FLADDR                            (0X001FFFF8U)  /* Memory Mapped Address for the Flash Location */

/* ----------------------------------------------------------------------------------------------------
          FEEFLDATA0                                            Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_FLASH_FEEFLDATA0_FLDATA0                         (0U)           /* Lower 32 Bit of 64 Bit Data to Be Written to Flash */
#define BITL_FLASH_FEEFLDATA0_FLDATA0                         (32U)          /* Lower 32 Bit of 64 Bit Data to Be Written to Flash */
#define BITM_FLASH_FEEFLDATA0_FLDATA0                         (0XFFFFFFFFU)  /* Lower 32 Bit of 64 Bit Data to Be Written to Flash */

/* ----------------------------------------------------------------------------------------------------
          FEEFLDATA1                                            Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_FLASH_FEEFLDATA1_FLDATA1                         (0U)           /* Upper 32 Bit of 64 Bit Data to Be Written to Flash */
#define BITL_FLASH_FEEFLDATA1_FLDATA1                         (32U)          /* Upper 32 Bit of 64 Bit Data to Be Written to Flash */
#define BITM_FLASH_FEEFLDATA1_FLDATA1                         (0XFFFFFFFFU)  /* Upper 32 Bit of 64 Bit Data to Be Written to Flash */

/* ----------------------------------------------------------------------------------------------------
          FEEADR0                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_FLASH_FEEADR0_PAGEADDR0                          (13U)          /* Page Address 0 */
#define BITL_FLASH_FEEADR0_PAGEADDR0                          (8U)           /* Page Address 0 */
#define BITM_FLASH_FEEADR0_PAGEADDR0                          (0X001FE000U)  /* Page Address 0 */

/* ----------------------------------------------------------------------------------------------------
          FEEADR1                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_FLASH_FEEADR1_PAGEADDR1                          (13U)          /* Page Address 1 */
#define BITL_FLASH_FEEADR1_PAGEADDR1                          (8U)           /* Page Address 1 */
#define BITM_FLASH_FEEADR1_PAGEADDR1                          (0X001FE000U)  /* Page Address 1 */

/* ----------------------------------------------------------------------------------------------------
          FEEKEY                                                Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_FLASH_FEEKEY_KEY                                 (0U)           /* Key Register */
#define BITL_FLASH_FEEKEY_KEY                                 (32U)          /* Key Register */
#define BITM_FLASH_FEEKEY_KEY                                 (0XFFFFFFFFU)  /* Key Register */

/* ----------------------------------------------------------------------------------------------------
          FEEPRO0                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_FLASH_FEEPRO0_WRPROT0                            (0U)           /* Write Protection for Flash0 – 32 Bits. */
#define BITL_FLASH_FEEPRO0_WRPROT0                            (32U)          /* Write Protection for Flash0 – 32 Bits. */
#define BITM_FLASH_FEEPRO0_WRPROT0                            (0XFFFFFFFFU)  /* Write Protection for Flash0 – 32 Bits. */

/* ----------------------------------------------------------------------------------------------------
          FEEPRO1                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_FLASH_FEEPRO1_WRPROT1                            (0U)           /* Write Protection for Flash1 – 32 Bits. */
#define BITL_FLASH_FEEPRO1_WRPROT1                            (32U)          /* Write Protection for Flash1 – 32 Bits. */
#define BITM_FLASH_FEEPRO1_WRPROT1                            (0XFFFFFFFFU)  /* Write Protection for Flash1 – 32 Bits. */

/* ----------------------------------------------------------------------------------------------------
          FEESIG                                                Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_FLASH_FEESIG_SIGN                                (0U)           /* Signature */
#define BITL_FLASH_FEESIG_SIGN                                (24U)          /* Signature */
#define BITM_FLASH_FEESIG_SIGN                                (0X00FFFFFFU)  /* Signature */

/* ----------------------------------------------------------------------------------------------------
          FEECON1                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_FLASH_FEECON1_JTAGDEBUGEN                        (0U)           /* JTAG Debug Enable */
#define BITL_FLASH_FEECON1_JTAGDEBUGEN                        (1U)           /* JTAG Debug Enable */
#define BITM_FLASH_FEECON1_JTAGDEBUGEN                        (0X00000001U)  /* JTAG Debug Enable */
#define BITP_FLASH_FEECON1_KHDMAEN                            (1U)           /* Key – Hole DMA Enable. */
#define BITL_FLASH_FEECON1_KHDMAEN                            (1U)           /* Key – Hole DMA Enable. */
#define BITM_FLASH_FEECON1_KHDMAEN                            (0X00000002U)  /* Key – Hole DMA Enable. */
#define BITP_FLASH_FEECON1_AUTOINCREN                         (2U)           /* Auto Address Increment for Key Hole Access. */
#define BITL_FLASH_FEECON1_AUTOINCREN                         (1U)           /* Auto Address Increment for Key Hole Access. */
#define BITM_FLASH_FEECON1_AUTOINCREN                         (0X00000004U)  /* Auto Address Increment for Key Hole Access. */
#define BITP_FLASH_FEECON1_SWAPPROGRAMCODE                    (3U)           /* Swap Program Code for MDIO Mode. */
#define BITL_FLASH_FEECON1_SWAPPROGRAMCODE                    (1U)           /* Swap Program Code for MDIO Mode. */
#define BITM_FLASH_FEECON1_SWAPPROGRAMCODE                    (0X00000008U)  /* Swap Program Code for MDIO Mode. */
#define BITP_FLASH_FEECON1_MDIOMODE                           (4U)           /* MDIO Mode */
#define BITL_FLASH_FEECON1_MDIOMODE                           (1U)           /* MDIO Mode */
#define BITM_FLASH_FEECON1_MDIOMODE                           (0X00000010U)  /* MDIO Mode */
#define BITP_FLASH_FEECON1_SWAPFLASH0                         (5U)           /* Swap Top and Bottom Image Inside Flash 0. */
#define BITL_FLASH_FEECON1_SWAPFLASH0                         (1U)           /* Swap Top and Bottom Image Inside Flash 0. */
#define BITM_FLASH_FEECON1_SWAPFLASH0                         (0X00000020U)  /* Swap Top and Bottom Image Inside Flash 0. */
#define BITP_FLASH_FEECON1_SWAPFLASH1                         (6U)           /* Swap Top and Bottom Image Inside Flash 1. */
#define BITL_FLASH_FEECON1_SWAPFLASH1                         (1U)           /* Swap Top and Bottom Image Inside Flash 1. */
#define BITM_FLASH_FEECON1_SWAPFLASH1                         (0X00000040U)  /* Swap Top and Bottom Image Inside Flash 1. */
#define BITP_FLASH_FEECON1_SWAPINFLASHEN                      (8U)           /* Swap Inside Flash Enable */
#define BITL_FLASH_FEECON1_SWAPINFLASHEN                      (1U)           /* Swap Inside Flash Enable */
#define BITM_FLASH_FEECON1_SWAPINFLASHEN                      (0X00000100U)  /* Swap Inside Flash Enable */

/* ----------------------------------------------------------------------------------------------------
          FEEWRADDRA                                            Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_FLASH_FEEWRADDRA_WRABORTADDR                     (0U)           /* Write Abort Address */
#define BITL_FLASH_FEEWRADDRA_WRABORTADDR                     (32U)          /* Write Abort Address */
#define BITM_FLASH_FEEWRADDRA_WRABORTADDR                     (0XFFFFFFFFU)  /* Write Abort Address */

/* ----------------------------------------------------------------------------------------------------
          FEEAEN0                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_FLASH_FEEAEN0_SYSIRQABORTEN                      (0U)           /* Lower 32 Bits of System Interrupt Abort Enable. */
#define BITL_FLASH_FEEAEN0_SYSIRQABORTEN                      (32U)          /* Lower 32 Bits of System Interrupt Abort Enable. */
#define BITM_FLASH_FEEAEN0_SYSIRQABORTEN                      (0XFFFFFFFFU)  /* Lower 32 Bits of System Interrupt Abort Enable. */

/* ----------------------------------------------------------------------------------------------------
          FEEAEN1                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_FLASH_FEEAEN1_SYSIRQABORTEN                      (0U)           /* Middle 32 Bits of System Interrupt Abort Enable. */
#define BITL_FLASH_FEEAEN1_SYSIRQABORTEN                      (32U)          /* Middle 32 Bits of System Interrupt Abort Enable. */
#define BITM_FLASH_FEEAEN1_SYSIRQABORTEN                      (0XFFFFFFFFU)  /* Middle 32 Bits of System Interrupt Abort Enable. */

/* ----------------------------------------------------------------------------------------------------
          FEEAEN2                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_FLASH_FEEAEN2_SYSIRQABORTEN                      (0U)           /* Upper 32 Bits of System Interrupt Abort Enable. */
#define BITL_FLASH_FEEAEN2_SYSIRQABORTEN                      (32U)          /* Upper 32 Bits of System Interrupt Abort Enable. */
#define BITM_FLASH_FEEAEN2_SYSIRQABORTEN                      (0XFFFFFFFFU)  /* Upper 32 Bits of System Interrupt Abort Enable. */

/* ----------------------------------------------------------------------------------------------------
          FEEECCCONFIG                                          Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_FLASH_FEEECCCONFIG_ECCDISABLE                    (0U)           /* ECC Disable Bit. */
#define BITL_FLASH_FEEECCCONFIG_ECCDISABLE                    (1U)           /* ECC Disable Bit. */
#define BITM_FLASH_FEEECCCONFIG_ECCDISABLE                    (0X00000001U)  /* ECC Disable Bit. */
#define BITP_FLASH_FEEECCCONFIG_ECCAHBERROR                   (1U)           /* Signifies How to Generate AHB Error on ECC */
#define BITL_FLASH_FEEECCCONFIG_ECCAHBERROR                   (2U)           /* Signifies How to Generate AHB Error on ECC */
#define BITM_FLASH_FEEECCCONFIG_ECCAHBERROR                   (0X00000006U)  /* Signifies How to Generate AHB Error on ECC */
#define BITP_FLASH_FEEECCCONFIG_ECCINTRERROR                  (3U)           /* Interrupt Enable When a ECC Error Happens During an AHB Read */
#define BITL_FLASH_FEEECCCONFIG_ECCINTRERROR                  (2U)           /* Interrupt Enable When a ECC Error Happens During an AHB Read */
#define BITM_FLASH_FEEECCCONFIG_ECCINTRERROR                  (0X00000018U)  /* Interrupt Enable When a ECC Error Happens During an AHB Read */
#define BITP_FLASH_FEEECCCONFIG_ECCADDRCON                    (5U)           /* ECC Error Address Control Bit. */
#define BITL_FLASH_FEEECCCONFIG_ECCADDRCON                    (2U)           /* ECC Error Address Control Bit. */
#define BITM_FLASH_FEEECCCONFIG_ECCADDRCON                    (0X00000060U)  /* ECC Error Address Control Bit. */

#define ENUM_FLASH_FEEECCCONFIG_ECCADDRCON_NBUFNCLR           (0X00000000U)  /* ECC Error Address Register Can't Be Cleared Until Reset; Current ECC Error Address Will Be Returned Directly When Reading ECC_ADDR MMR. */
#define ENUM_FLASH_FEEECCCONFIG_ECCADDRCON_BUFNCLR            (0X00000001U)  /* ECC Error Address Register Can't Be Cleared Until Reset; ECC Error Address is Latched on Read of STATUS MMR, and It Will Be Returned When Reading ECC_ADDR MMR. */
#define ENUM_FLASH_FEEECCCONFIG_ECCINTRERROR_DISABLE          (0X00000000U)  /* Interrupt is Not Generated Even If There is an ECC Error While Reading from Flash via AHB. This Applies to Both Flash. */
#define ENUM_FLASH_FEEECCCONFIG_ECCINTRERROR_ERROR2BITEN      (0X00000001U)  /* Interrupt is Generated Only If 2 Bit Error is Detected During AHB Read to Flash 0 or 1. */
#define ENUM_FLASH_FEEECCCONFIG_ECCINTRERROR_ERROR1BITEN      (0X00000002U)  /* Interrupt is Generated Only If 1 Bit Error Corrected During AHB Read to Flash 0 or 1. */
#define ENUM_FLASH_FEEECCCONFIG_ECCINTRERROR_ERROR1OR2BITEN   (0X00000003U)  /* Interrupt is Generated If Either 2 Bit Detected or 1 Bit Error Corrected During AHB Read to Flash 0 or 1. */
#define ENUM_FLASH_FEEECCCONFIG_ECCAHBERROR_NOERROR           (0X00000000U)  /* AHB Error (HRESP = 1) is Not Generated Even If There is an ECC Error While Reading from Flash via AHB. This Applies to Both Flash. */
#define ENUM_FLASH_FEEECCCONFIG_ECCAHBERROR_ERROR2BIT         (0X00000001U)  /* AHB Error (HRESP = 1) is Generated Only If 2 Bit Error is Detected During AHB Read to Flash 0 or 1. */
#define ENUM_FLASH_FEEECCCONFIG_ECCAHBERROR_ERROR1BIT         (0X00000002U)  /* AHB Error (HRESP = 1) is Generated Only If 1 Bit Error Corrected During AHB Read to Flash 0 or 1 */
#define ENUM_FLASH_FEEECCCONFIG_ECCAHBERROR_ERROR1OR2BIT      (0X00000003U)  /* AHB Error (HRESP = 1) is Generated If Either 2 Bit Detected or 1 Bit Error Corrected During AHB Read to Flash 0 or 1 */

/* ----------------------------------------------------------------------------------------------------
          FEEECCADDRC0                                          Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_FLASH_FEEECCADDRC0_FLECCADDRC0                   (0U)           /* Flash0 Address for Which ECC Error is Detected via CODE Bus. */
#define BITL_FLASH_FEEECCADDRC0_FLECCADDRC0                   (21U)          /* Flash0 Address for Which ECC Error is Detected via CODE Bus. */
#define BITM_FLASH_FEEECCADDRC0_FLECCADDRC0                   (0X001FFFFFU)  /* Flash0 Address for Which ECC Error is Detected via CODE Bus. */

/* ----------------------------------------------------------------------------------------------------
          FEEECCADDRC1                                          Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_FLASH_FEEECCADDRC1_FLECCADDRC1                   (0U)           /* Flash1 Address for Which ECC Error is Detected via CODE Bus. */
#define BITL_FLASH_FEEECCADDRC1_FLECCADDRC1                   (21U)          /* Flash1 Address for Which ECC Error is Detected via CODE Bus. */
#define BITM_FLASH_FEEECCADDRC1_FLECCADDRC1                   (0X001FFFFFU)  /* Flash1 Address for Which ECC Error is Detected via CODE Bus. */

/* ----------------------------------------------------------------------------------------------------
          FEEECCADDRD0                                          Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_FLASH_FEEECCADDRD0_FLECCADDRD0                   (0U)           /* Flash0 Address for Which ECC Error is Detected via DMA Bus. */
#define BITL_FLASH_FEEECCADDRD0_FLECCADDRD0                   (21U)          /* Flash0 Address for Which ECC Error is Detected via DMA Bus. */
#define BITM_FLASH_FEEECCADDRD0_FLECCADDRD0                   (0X001FFFFFU)  /* Flash0 Address for Which ECC Error is Detected via DMA Bus. */

/* ----------------------------------------------------------------------------------------------------
          FEEECCADDRD1                                          Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_FLASH_FEEECCADDRD1_FLECCADDRD1                   (0U)           /* Flash1 Address for Which ECC Error is Detected via DMA Bus. */
#define BITL_FLASH_FEEECCADDRD1_FLECCADDRD1                   (21U)          /* Flash1 Address for Which ECC Error is Detected via DMA Bus. */
#define BITM_FLASH_FEEECCADDRD1_FLECCADDRD1                   (0X001FFFFFU)  /* Flash1 Address for Which ECC Error is Detected via DMA Bus. */

#endif  /* end ifndef FLASH_ADDR_RDEF_H_ */


#ifndef GPIO_ADDR_RDEF_H_
#define GPIO_ADDR_RDEF_H_    /* GPIO: GPIO */

/* ====================================================================================================
        GPIO Module Instances Address and Mask Definitions
   ==================================================================================================== */
#define INST_GPIO0                                               (0X40050000U)    /* gpio0: */
#define INST_GPIO1                                               (0X40050050U)    /* gpio1: */
#define INST_GPIO2                                               (0X400500A0U)    /* gpio2: */
#define INST_GPIO3                                               (0X400500F0U)    /* gpio3: */
#define INST_GPIO4                                               (0X40050140U)    /* gpio4: */
#define INST_GPIO5                                               (0X40050190U)    /* gpio5: */

#define MASK_GPIO                                                (0XFFFFFFFFU)    /* GPIO: GPIO */

/* ====================================================================================================
        GPIO Module Register Address Offset Definitions
   ==================================================================================================== */
#define IDX_GPIO_CON                                            (0X00000000U)    /* Port Configuration */
#define IDX_GPIO_OE                                             (0X00000004U)    /* Port Output Enable */
#define IDX_GPIO_IE                                             (0X00000008U)    /* Port Input Path Enable */
#define IDX_GPIO_IN                                             (0X0000000CU)    /* Port Registered Data Input */
#define IDX_GPIO_OUT                                            (0X00000010U)    /* Port Data Output */
#define IDX_GPIO_SET                                            (0X00000014U)    /* Port Data Out Set */
#define IDX_GPIO_CLR                                            (0X00000018U)    /* Port Data Out Clear */
#define IDX_GPIO_TGL                                            (0X0000001CU)    /* Port Pin Toggle */
#define IDX_GPIO_ODE                                            (0X00000020U)    /* Port Open Drain Enable */
#define IDX_GPIO_IS                                             (0X00000024U)    /* Port Input Select */
#define IDX_GPIO_PE                                             (0X00000028U)    /* Port Pull Enable */
#define IDX_GPIO_PS                                             (0X0000002CU)    /* Port Pull Select */
#define IDX_GPIO_SR                                             (0X00000030U)    /* Port Slew Rate */
#define IDX_GPIO_DS                                             (0X00000034U)    /* Port Drive Select */
#define IDX_GPIO_PWR                                            (0X00000038U)    /* Port Power Select */
#define IDX_GPIO_POL                                            (0X0000003CU)    /* GPIO Interrupt Polarity Select */
#define IDX_GPIO_IENA                                           (0X00000040U)    /* InterruptA Enable */
#define IDX_GPIO_IENB                                           (0X00000044U)    /* InterruptB Enable */
#define IDX_GPIO_INT                                            (0X00000048U)    /* Interrupt Status */

/* ====================================================================================================
        GPIO Module Register ResetValue Definitions
   ==================================================================================================== */
#define RSTVAL_GPIO_CON                                            (0X0) 
#define RSTVAL_GPIO_OE                                             (0X0) 
#define RSTVAL_GPIO_IE                                             (0X0) 
#define RSTVAL_GPIO_IN                                             (0X0) 
#define RSTVAL_GPIO_OUT                                            (0X0) 
#define RSTVAL_GPIO_SET                                            (0X0) 
#define RSTVAL_GPIO_CLR                                            (0X0) 
#define RSTVAL_GPIO_TGL                                            (0X0) 
#define RSTVAL_GPIO_ODE                                            (0X0) 
#define RSTVAL_GPIO_IS                                             (0XFF) 
#define RSTVAL_GPIO_PE                                             (0X0) 
#define RSTVAL_GPIO_PS                                             (0XFF) 
#define RSTVAL_GPIO_SR                                             (0X0) 
#define RSTVAL_GPIO_DS                                             (0X0) 
#define RSTVAL_GPIO_PWR                                            (0XFF) 
#define RSTVAL_GPIO_POL                                            (0X0) 
#define RSTVAL_GPIO_IENA                                           (0X0) 
#define RSTVAL_GPIO_IENB                                           (0X0) 
#define RSTVAL_GPIO_INT                                            (0X0) 

/* ====================================================================================================
        GPIO Module Register BitPositions, Lengths, Masks and Enumerations Definitions
   ==================================================================================================== */

/* ----------------------------------------------------------------------------------------------------
          CON                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_GPIO_CON_CON0                                    (0U)           /* PIN 0 Configuration Bits */
#define BITL_GPIO_CON_CON0                                    (2U)           /* PIN 0 Configuration Bits */
#define BITM_GPIO_CON_CON0                                    (0X0003U)      /* PIN 0 Configuration Bits */
#define BITP_GPIO_CON_CON1                                    (2U)           /* PIN 1 Configuration Bits */
#define BITL_GPIO_CON_CON1                                    (2U)           /* PIN 1 Configuration Bits */
#define BITM_GPIO_CON_CON1                                    (0X000CU)      /* PIN 1 Configuration Bits */
#define BITP_GPIO_CON_CON2                                    (4U)           /* PIN 2 Configuration Bits */
#define BITL_GPIO_CON_CON2                                    (2U)           /* PIN 2 Configuration Bits */
#define BITM_GPIO_CON_CON2                                    (0X0030U)      /* PIN 2 Configuration Bits */
#define BITP_GPIO_CON_CON3                                    (6U)           /* PIN 3 Configuration Bits */
#define BITL_GPIO_CON_CON3                                    (2U)           /* PIN 3 Configuration Bits */
#define BITM_GPIO_CON_CON3                                    (0X00C0U)      /* PIN 3 Configuration Bits */
#define BITP_GPIO_CON_CON4                                    (8U)           /* PIN 4 Configuration Bits */
#define BITL_GPIO_CON_CON4                                    (2U)           /* PIN 4 Configuration Bits */
#define BITM_GPIO_CON_CON4                                    (0X0300U)      /* PIN 4 Configuration Bits */
#define BITP_GPIO_CON_CON5                                    (10U)          /* PIN 5 Configuration Bits */
#define BITL_GPIO_CON_CON5                                    (2U)           /* PIN 5 Configuration Bits */
#define BITM_GPIO_CON_CON5                                    (0X0C00U)      /* PIN 5 Configuration Bits */
#define BITP_GPIO_CON_CON6                                    (12U)          /* PIN 6 Configuration Bits */
#define BITL_GPIO_CON_CON6                                    (2U)           /* PIN 6 Configuration Bits */
#define BITM_GPIO_CON_CON6                                    (0X3000U)      /* PIN 6 Configuration Bits */
#define BITP_GPIO_CON_CON7                                    (14U)          /* PIN 6 Configuration Bits */
#define BITL_GPIO_CON_CON7                                    (2U)           /* PIN 6 Configuration Bits */
#define BITM_GPIO_CON_CON7                                    (0XC000U)      /* PIN 6 Configuration Bits */

#define ENUM_GPIO_CON_CON7_FUNCTION0                          (0X0000U)      /* No description provided */
#define ENUM_GPIO_CON_CON7_FUNCTION1                          (0X0001U)      /* No description provided */
#define ENUM_GPIO_CON_CON7_FUNCTION2                          (0X0002U)      /* No description provided */
#define ENUM_GPIO_CON_CON7_FUNCTION3                          (0X0003U)      /* No description provided */
#define ENUM_GPIO_CON_CON6_FUNCTION0                          (0X0000U)      /* No description provided */
#define ENUM_GPIO_CON_CON6_FUNCTION1                          (0X0001U)      /* No description provided */
#define ENUM_GPIO_CON_CON6_FUNCTION2                          (0X0002U)      /* No description provided */
#define ENUM_GPIO_CON_CON6_FUNCTION3                          (0X0003U)      /* No description provided */
#define ENUM_GPIO_CON_CON5_FUNCTION0                          (0X0000U)      /* No description provided */
#define ENUM_GPIO_CON_CON5_FUNCTION1                          (0X0001U)      /* No description provided */
#define ENUM_GPIO_CON_CON5_FUNCTION2                          (0X0002U)      /* No description provided */
#define ENUM_GPIO_CON_CON5_FUNCTION3                          (0X0003U)      /* No description provided */
#define ENUM_GPIO_CON_CON4_FUNCTION0                          (0X0000U)      /* No description provided */
#define ENUM_GPIO_CON_CON4_FUNCTION1                          (0X0001U)      /* No description provided */
#define ENUM_GPIO_CON_CON4_FUNCTION2                          (0X0002U)      /* No description provided */
#define ENUM_GPIO_CON_CON4_FUNCTION3                          (0X0003U)      /* No description provided */
#define ENUM_GPIO_CON_CON3_FUNCTION0                          (0X0000U)      /* No description provided */
#define ENUM_GPIO_CON_CON3_FUNCTION1                          (0X0001U)      /* No description provided */
#define ENUM_GPIO_CON_CON3_FUNCTION2                          (0X0002U)      /* No description provided */
#define ENUM_GPIO_CON_CON3_FUNCTION3                          (0X0003U)      /* No description provided */
#define ENUM_GPIO_CON_CON2_FUNCTION0                          (0X0000U)      /* No description provided */
#define ENUM_GPIO_CON_CON2_FUNCTION1                          (0X0001U)      /* No description provided */
#define ENUM_GPIO_CON_CON2_FUNCTION2                          (0X0002U)      /* No description provided */
#define ENUM_GPIO_CON_CON2_FUNCTION3                          (0X0003U)      /* No description provided */
#define ENUM_GPIO_CON_CON1_FUNCTION0                          (0X0000U)      /* No description provided */
#define ENUM_GPIO_CON_CON1_FUNCTION1                          (0X0001U)      /* No description provided */
#define ENUM_GPIO_CON_CON1_FUNCTION2                          (0X0002U)      /* No description provided */
#define ENUM_GPIO_CON_CON1_FUNCTION3                          (0X0003U)      /* No description provided */
#define ENUM_GPIO_CON_CON0_FUNCTION0                          (0X0000U)      /* No description provided */
#define ENUM_GPIO_CON_CON0_FUNCTION1                          (0X0001U)      /* No description provided */
#define ENUM_GPIO_CON_CON0_FUNCTION2                          (0X0002U)      /* No description provided */
#define ENUM_GPIO_CON_CON0_FUNCTION3                          (0X0003U)      /* No description provided */

/* ----------------------------------------------------------------------------------------------------
          OE                                                    Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_GPIO_OE_OE                                       (0U)           /* Open Drain Enable */
#define BITL_GPIO_OE_OE                                       (8U)           /* Open Drain Enable */
#define BITM_GPIO_OE_OE                                       (0XFFU)        /* Open Drain Enable */

/* ----------------------------------------------------------------------------------------------------
          IE                                                    Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_GPIO_IE_IE                                       (0U)           /* Input Enable */
#define BITL_GPIO_IE_IE                                       (8U)           /* Input Enable */
#define BITM_GPIO_IE_IE                                       (0XFFU)        /* Input Enable */

/* ----------------------------------------------------------------------------------------------------
          IN                                                    Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_GPIO_IN_A                                        (0U)           /* Diver Data Input */
#define BITL_GPIO_IN_A                                        (8U)           /* Diver Data Input */
#define BITM_GPIO_IN_A                                        (0XFFU)        /* Diver Data Input */

/* ----------------------------------------------------------------------------------------------------
          OUT                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_GPIO_OUT_Y                                       (0U)           /* Input Data from Pad */
#define BITL_GPIO_OUT_Y                                       (8U)           /* Input Data from Pad */
#define BITM_GPIO_OUT_Y                                       (0XFFU)        /* Input Data from Pad */

/* ----------------------------------------------------------------------------------------------------
          SET                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_GPIO_SET_SETUP                                   (0U)           /* Set the Output HIGH for the Pin */
#define BITL_GPIO_SET_SETUP                                   (8U)           /* Set the Output HIGH for the Pin */
#define BITM_GPIO_SET_SETUP                                   (0XFFU)        /* Set the Output HIGH for the Pin */

/* ----------------------------------------------------------------------------------------------------
          CLR                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_GPIO_CLR_CLR                                     (0U)           /* Set the Output Low for the Port Pin */
#define BITL_GPIO_CLR_CLR                                     (8U)           /* Set the Output Low for the Port Pin */
#define BITM_GPIO_CLR_CLR                                     (0XFFU)        /* Set the Output Low for the Port Pin */

/* ----------------------------------------------------------------------------------------------------
          TGL                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_GPIO_TGL_TGL                                     (0U)           /* Toggle the Output of the Port Pin */
#define BITL_GPIO_TGL_TGL                                     (8U)           /* Toggle the Output of the Port Pin */
#define BITM_GPIO_TGL_TGL                                     (0XFFU)        /* Toggle the Output of the Port Pin */

/* ----------------------------------------------------------------------------------------------------
          ODE                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_GPIO_ODE_ODE                                     (0U)           /* Open Drain Enable */
#define BITL_GPIO_ODE_ODE                                     (8U)           /* Open Drain Enable */
#define BITM_GPIO_ODE_ODE                                     (0XFFU)        /* Open Drain Enable */

/* ----------------------------------------------------------------------------------------------------
          IS                                                    Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_GPIO_IS_IS                                       (0U)           /* Input Select */
#define BITL_GPIO_IS_IS                                       (8U)           /* Input Select */
#define BITM_GPIO_IS_IS                                       (0XFFU)        /* Input Select */

/* ----------------------------------------------------------------------------------------------------
          PE                                                    Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_GPIO_PE_PE                                       (0U)           /* Pull Enable */
#define BITL_GPIO_PE_PE                                       (8U)           /* Pull Enable */
#define BITM_GPIO_PE_PE                                       (0XFFU)        /* Pull Enable */

/* ----------------------------------------------------------------------------------------------------
          PS                                                    Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_GPIO_PS_PS                                       (0U)           /* Pull Select */
#define BITL_GPIO_PS_PS                                       (8U)           /* Pull Select */
#define BITM_GPIO_PS_PS                                       (0XFFU)        /* Pull Select */

/* ----------------------------------------------------------------------------------------------------
          SR                                                    Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_GPIO_SR_SR                                       (0U)           /* Slew Rate */
#define BITL_GPIO_SR_SR                                       (8U)           /* Slew Rate */
#define BITM_GPIO_SR_SR                                       (0XFFU)        /* Slew Rate */

/* ----------------------------------------------------------------------------------------------------
          DS                                                    Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_GPIO_DS_DS0                                      (0U)           /* PIN 0 Drive Select */
#define BITL_GPIO_DS_DS0                                      (2U)           /* PIN 0 Drive Select */
#define BITM_GPIO_DS_DS0                                      (0X0003U)      /* PIN 0 Drive Select */
#define BITP_GPIO_DS_DS1                                      (2U)           /* PIN 1 Drive Select */
#define BITL_GPIO_DS_DS1                                      (2U)           /* PIN 1 Drive Select */
#define BITM_GPIO_DS_DS1                                      (0X000CU)      /* PIN 1 Drive Select */
#define BITP_GPIO_DS_DS2                                      (4U)           /* PIN 2 Drive Select */
#define BITL_GPIO_DS_DS2                                      (2U)           /* PIN 2 Drive Select */
#define BITM_GPIO_DS_DS2                                      (0X0030U)      /* PIN 2 Drive Select */
#define BITP_GPIO_DS_DS3                                      (6U)           /* PIN 3 Drive Select */
#define BITL_GPIO_DS_DS3                                      (2U)           /* PIN 3 Drive Select */
#define BITM_GPIO_DS_DS3                                      (0X00C0U)      /* PIN 3 Drive Select */
#define BITP_GPIO_DS_DS4                                      (8U)           /* PIN 4 Drive Select */
#define BITL_GPIO_DS_DS4                                      (2U)           /* PIN 4 Drive Select */
#define BITM_GPIO_DS_DS4                                      (0X0300U)      /* PIN 4 Drive Select */
#define BITP_GPIO_DS_DS5                                      (10U)          /* PIN 5 Drive Select */
#define BITL_GPIO_DS_DS5                                      (2U)           /* PIN 5 Drive Select */
#define BITM_GPIO_DS_DS5                                      (0X0C00U)      /* PIN 5 Drive Select */
#define BITP_GPIO_DS_DS6                                      (12U)          /* PIN6 Drive Select */
#define BITL_GPIO_DS_DS6                                      (2U)           /* PIN6 Drive Select */
#define BITM_GPIO_DS_DS6                                      (0X3000U)      /* PIN6 Drive Select */
#define BITP_GPIO_DS_DS7                                      (14U)          /* PIN 7 Drive Select */
#define BITL_GPIO_DS_DS7                                      (2U)           /* PIN 7 Drive Select */
#define BITM_GPIO_DS_DS7                                      (0XC000U)      /* PIN 7 Drive Select */

#define ENUM_GPIO_DS_DS7_STRENGTH1                            (0X0000U)      /* Drive Strength 1 */
#define ENUM_GPIO_DS_DS7_STRENGTH2                            (0X0001U)      /* Drive Strength 2 */
#define ENUM_GPIO_DS_DS7_STRENGTH3                            (0X0002U)      /* Drive Strength 3 */
#define ENUM_GPIO_DS_DS7_STRENGTH4                            (0X0003U)      /* Drive Strength 4 */
#define ENUM_GPIO_DS_DS6_STRENGTH1                            (0X0000U)      /* Drive Strength 1 */
#define ENUM_GPIO_DS_DS6_STRENGTH2                            (0X0001U)      /* Drive Strength 2 */
#define ENUM_GPIO_DS_DS6_STRENGTH3                            (0X0002U)      /* Drive Strength 3 */
#define ENUM_GPIO_DS_DS6_STRENGTH4                            (0X0003U)      /* Drive Strength 4 */
#define ENUM_GPIO_DS_DS5_STRENGTH1                            (0X0000U)      /* Drive Strength 1 */
#define ENUM_GPIO_DS_DS5_STRENGTH2                            (0X0001U)      /* Drive Strength 2 */
#define ENUM_GPIO_DS_DS5_STRENGTH3                            (0X0002U)      /* Drive Strength 3 */
#define ENUM_GPIO_DS_DS5_STRENGTH4                            (0X0003U)      /* Drive Strength 4 */
#define ENUM_GPIO_DS_DS4_STRENGTH1                            (0X0000U)      /* Drive Strength 1 */
#define ENUM_GPIO_DS_DS4_STRENGTH2                            (0X0001U)      /* Drive Strength 2 */
#define ENUM_GPIO_DS_DS4_STRENGTH3                            (0X0002U)      /* Drive Strength 3 */
#define ENUM_GPIO_DS_DS4_STRENGTH4                            (0X0003U)      /* Drive Strength 4 */
#define ENUM_GPIO_DS_DS3_STRENGTH1                            (0X0000U)      /* Drive Strength 1 */
#define ENUM_GPIO_DS_DS3_STRENGTH2                            (0X0001U)      /* Drive Strength 2 */
#define ENUM_GPIO_DS_DS3_STRENGTH3                            (0X0002U)      /* Drive Strength 3 */
#define ENUM_GPIO_DS_DS3_STRENGTH4                            (0X0003U)      /* Drive Strength 4 */
#define ENUM_GPIO_DS_DS2_STRENGTH1                            (0X0000U)      /* Drive Strength 1 */
#define ENUM_GPIO_DS_DS2_STRENGTH2                            (0X0001U)      /* Drive Strength 2 */
#define ENUM_GPIO_DS_DS2_STRENGTH3                            (0X0002U)      /* Drive Strength 3 */
#define ENUM_GPIO_DS_DS2_STRENGTH4                            (0X0003U)      /* Drive Strength 4 */
#define ENUM_GPIO_DS_DS1_STRENGTH1                            (0X0000U)      /* Drive Strength 1 */
#define ENUM_GPIO_DS_DS1_STRENGTH2                            (0X0001U)      /* Drive Strength 2 */
#define ENUM_GPIO_DS_DS1_STRENGTH3                            (0X0002U)      /* Drive Strength 3 */
#define ENUM_GPIO_DS_DS1_STRENGTH4                            (0X0003U)      /* Drive Strength 4 */
#define ENUM_GPIO_DS_DS0_STRENGTH1                            (0X0000U)      /* Drive Strength 1 */
#define ENUM_GPIO_DS_DS0_STRENGTH2                            (0X0001U)      /* Drive Strength 2 */
#define ENUM_GPIO_DS_DS0_STRENGTH3                            (0X0002U)      /* Drive Strength 3 */
#define ENUM_GPIO_DS_DS0_STRENGTH4                            (0X0003U)      /* Drive Strength 4 */

/* ----------------------------------------------------------------------------------------------------
          PWR                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_GPIO_PWR_PWR                                     (0U)           /* Pad Power Select */
#define BITL_GPIO_PWR_PWR                                     (8U)           /* Pad Power Select */
#define BITM_GPIO_PWR_PWR                                     (0XFFU)        /* Pad Power Select */

/* ----------------------------------------------------------------------------------------------------
          POL                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_GPIO_POL_INTPOL                                  (0U)           /* Parametric Output */
#define BITL_GPIO_POL_INTPOL                                  (8U)           /* Parametric Output */
#define BITM_GPIO_POL_INTPOL                                  (0XFFU)        /* Parametric Output */

/* ----------------------------------------------------------------------------------------------------
          IENA                                                  Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_GPIO_IENA_INTAEN                                 (0U)           /* InterruptA Enable */
#define BITL_GPIO_IENA_INTAEN                                 (8U)           /* InterruptA Enable */
#define BITM_GPIO_IENA_INTAEN                                 (0XFFU)        /* InterruptA Enable */

/* ----------------------------------------------------------------------------------------------------
          IENB                                                  Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_GPIO_IENB_INTBEN                                 (0U)           /* InterruptA Enable */
#define BITL_GPIO_IENB_INTBEN                                 (8U)           /* InterruptA Enable */
#define BITM_GPIO_IENB_INTBEN                                 (0XFFU)        /* InterruptA Enable */

/* ----------------------------------------------------------------------------------------------------
          INT                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_GPIO_INT_INTSTATUS                               (0U)           /* Interrupt Status */
#define BITL_GPIO_INT_INTSTATUS                               (8U)           /* Interrupt Status */
#define BITM_GPIO_INT_INTSTATUS                               (0XFFU)        /* Interrupt Status */

#endif  /* end ifndef GPIO_ADDR_RDEF_H_ */


#ifndef SPI_ADDR_RDEF_H_
#define SPI_ADDR_RDEF_H_    /* SPI: Serial Peripheral Interface */

/* ====================================================================================================
        SPI Module Instances Address and Mask Definitions
   ==================================================================================================== */
#define INST_SPI0                                                (0X40054000U)    /* spi0: */
#define INST_SPI1                                                (0X40058000U)    /* spi1: */
#define INST_SPI2                                                (0X4005C000U)    /* spi2: */

#define MASK_SPI                                                 (0X000000FFU)    /* SPI: Serial Peripheral Interface */

/* ====================================================================================================
        SPI Module Register Address Offset Definitions
   ==================================================================================================== */
#define IDX_SPI_STAT                                            (0X00U)    /* Status */
#define IDX_SPI_RX                                              (0X04U)    /* Receive */
#define IDX_SPI_TX                                              (0X08U)    /* Transmit */
#define IDX_SPI_DIV                                             (0X0CU)    /* SPI Baud Rate Selection */
#define IDX_SPI_CTL                                             (0X10U)    /* SPI Configuration 1 */
#define IDX_SPI_IEN                                             (0X14U)    /* SPI Configuration 2 */
#define IDX_SPI_CNT                                             (0X18U)    /* Transfer Byte Count */
#define IDX_SPI_DMA                                             (0X1CU)    /* SPI DMA Enable */
#define IDX_SPI_FIFOSTAT                                        (0X20U)    /* FIFO Status */
#define IDX_SPI_RDCTL                                           (0X24U)    /* Read Control */
#define IDX_SPI_FLOWCTL                                         (0X28U)    /* Flow Control */
#define IDX_SPI_WAITTMR                                         (0X2CU)    /* Wait Timer for Flow Control */
#define IDX_SPI_CSOVERRIDE                                      (0X34U)    /* Chip-Select Override */

/* ====================================================================================================
        SPI Module Register ResetValue Definitions
   ==================================================================================================== */
#define RSTVAL_SPI_STAT                                            (0X800) 
#define RSTVAL_SPI_RX                                              (0X0) 
#define RSTVAL_SPI_TX                                              (0X0) 
#define RSTVAL_SPI_DIV                                             (0X0) 
#define RSTVAL_SPI_CTL                                             (0X0) 
#define RSTVAL_SPI_IEN                                             (0X0) 
#define RSTVAL_SPI_CNT                                             (0X0) 
#define RSTVAL_SPI_DMA                                             (0X0) 
#define RSTVAL_SPI_FIFOSTAT                                        (0X0) 
#define RSTVAL_SPI_RDCTL                                           (0X0) 
#define RSTVAL_SPI_FLOWCTL                                         (0X0) 
#define RSTVAL_SPI_WAITTMR                                         (0X0) 
#define RSTVAL_SPI_CSOVERRIDE                                      (0X0) 

/* ====================================================================================================
        SPI Module Register BitPositions, Lengths, Masks and Enumerations Definitions
   ==================================================================================================== */

/* ----------------------------------------------------------------------------------------------------
          STAT                                                  Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_SPI_STAT_IRQ                                     (0U)           /* SPI Interrupt Status */
#define BITL_SPI_STAT_IRQ                                     (1U)           /* SPI Interrupt Status */
#define BITM_SPI_STAT_IRQ                                     (0X0001U)      /* SPI Interrupt Status */
#define BITP_SPI_STAT_XFRDONE                                 (1U)           /* SPI Transfer Completion */
#define BITL_SPI_STAT_XFRDONE                                 (1U)           /* SPI Transfer Completion */
#define BITM_SPI_STAT_XFRDONE                                 (0X0002U)      /* SPI Transfer Completion */
#define BITP_SPI_STAT_TXEMPTY                                 (2U)           /* SPI Tx FIFO Empty Interrupt */
#define BITL_SPI_STAT_TXEMPTY                                 (1U)           /* SPI Tx FIFO Empty Interrupt */
#define BITM_SPI_STAT_TXEMPTY                                 (0X0004U)      /* SPI Tx FIFO Empty Interrupt */
#define BITP_SPI_STAT_TXDONE                                  (3U)           /* SPI Tx Done in Read Command Mode */
#define BITL_SPI_STAT_TXDONE                                  (1U)           /* SPI Tx Done in Read Command Mode */
#define BITM_SPI_STAT_TXDONE                                  (0X0008U)      /* SPI Tx Done in Read Command Mode */
#define BITP_SPI_STAT_TXUNDR                                  (4U)           /* SPI Tx FIFO Underflow */
#define BITL_SPI_STAT_TXUNDR                                  (1U)           /* SPI Tx FIFO Underflow */
#define BITM_SPI_STAT_TXUNDR                                  (0X0010U)      /* SPI Tx FIFO Underflow */
#define BITP_SPI_STAT_TXIRQ                                   (5U)           /* SPI Tx IRQ */
#define BITL_SPI_STAT_TXIRQ                                   (1U)           /* SPI Tx IRQ */
#define BITM_SPI_STAT_TXIRQ                                   (0X0020U)      /* SPI Tx IRQ */
#define BITP_SPI_STAT_RXIRQ                                   (6U)           /* SPI Rx IRQ */
#define BITL_SPI_STAT_RXIRQ                                   (1U)           /* SPI Rx IRQ */
#define BITM_SPI_STAT_RXIRQ                                   (0X0040U)      /* SPI Rx IRQ */
#define BITP_SPI_STAT_RXOVR                                   (7U)           /* SPI Rx FIFO Overflow */
#define BITL_SPI_STAT_RXOVR                                   (1U)           /* SPI Rx FIFO Overflow */
#define BITM_SPI_STAT_RXOVR                                   (0X0080U)      /* SPI Rx FIFO Overflow */
#define BITP_SPI_STAT_CS                                      (11U)          /* CS Status */
#define BITL_SPI_STAT_CS                                      (1U)           /* CS Status */
#define BITM_SPI_STAT_CS                                      (0X0800U)      /* CS Status */
#define BITP_SPI_STAT_CSERR                                   (12U)          /* Detected a CS Error Condition in Slave Mode */
#define BITL_SPI_STAT_CSERR                                   (1U)           /* Detected a CS Error Condition in Slave Mode */
#define BITM_SPI_STAT_CSERR                                   (0X1000U)      /* Detected a CS Error Condition in Slave Mode */
#define BITP_SPI_STAT_CSRISE                                  (13U)          /* Detected a Rising Edge on CS, in Slave CON Mode */
#define BITL_SPI_STAT_CSRISE                                  (1U)           /* Detected a Rising Edge on CS, in Slave CON Mode */
#define BITM_SPI_STAT_CSRISE                                  (0X2000U)      /* Detected a Rising Edge on CS, in Slave CON Mode */
#define BITP_SPI_STAT_CSFALL                                  (14U)          /* Detected a Falling Edge on CS, in Slave CON Mode */
#define BITL_SPI_STAT_CSFALL                                  (1U)           /* Detected a Falling Edge on CS, in Slave CON Mode */
#define BITM_SPI_STAT_CSFALL                                  (0X4000U)      /* Detected a Falling Edge on CS, in Slave CON Mode */
#define BITP_SPI_STAT_RDY                                     (15U)          /* Detected an Edge on Ready Indicator for Flow-control */
#define BITL_SPI_STAT_RDY                                     (1U)           /* Detected an Edge on Ready Indicator for Flow-control */
#define BITM_SPI_STAT_RDY                                     (0X8000U)      /* Detected an Edge on Ready Indicator for Flow-control */

/* ----------------------------------------------------------------------------------------------------
          RX                                                    Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_SPI_RX_BYTE1                                     (0U)           /* 8-bit Receive Buffer */
#define BITL_SPI_RX_BYTE1                                     (8U)           /* 8-bit Receive Buffer */
#define BITM_SPI_RX_BYTE1                                     (0X00FFU)      /* 8-bit Receive Buffer */
#define BITP_SPI_RX_BYTE2                                     (8U)           /* 8-bit Receive Buffer, Used Only in DMA Modes */
#define BITL_SPI_RX_BYTE2                                     (8U)           /* 8-bit Receive Buffer, Used Only in DMA Modes */
#define BITM_SPI_RX_BYTE2                                     (0XFF00U)      /* 8-bit Receive Buffer, Used Only in DMA Modes */

/* ----------------------------------------------------------------------------------------------------
          TX                                                    Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_SPI_TX_BYTE1                                     (0U)           /* 8-bit Transmit Buffer */
#define BITL_SPI_TX_BYTE1                                     (8U)           /* 8-bit Transmit Buffer */
#define BITM_SPI_TX_BYTE1                                     (0X00FFU)      /* 8-bit Transmit Buffer */
#define BITP_SPI_TX_BYTE2                                     (8U)           /* 8-bit Transmit Buffer, Used Only in DMA Modes */
#define BITL_SPI_TX_BYTE2                                     (8U)           /* 8-bit Transmit Buffer, Used Only in DMA Modes */
#define BITM_SPI_TX_BYTE2                                     (0XFF00U)      /* 8-bit Transmit Buffer, Used Only in DMA Modes */

/* ----------------------------------------------------------------------------------------------------
          DIV                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_SPI_DIV_DIV                                      (0U)           /* SPI Clock Divider */
#define BITL_SPI_DIV_DIV                                      (6U)           /* SPI Clock Divider */
#define BITM_SPI_DIV_DIV                                      (0X003FU)      /* SPI Clock Divider */
#define BITP_SPI_DIV_SFR                                      (8U)           /* Slave Free Run Mode */
#define BITL_SPI_DIV_SFR                                      (1U)           /* Slave Free Run Mode */
#define BITM_SPI_DIV_SFR                                      (0X0100U)      /* Slave Free Run Mode */

/* ----------------------------------------------------------------------------------------------------
          CTL                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_SPI_CTL_SPIEN                                    (0U)           /* SPI Enable */
#define BITL_SPI_CTL_SPIEN                                    (1U)           /* SPI Enable */
#define BITM_SPI_CTL_SPIEN                                    (0X0001U)      /* SPI Enable */
#define BITP_SPI_CTL_MASEN                                    (1U)           /* Master Mode Enable */
#define BITL_SPI_CTL_MASEN                                    (1U)           /* Master Mode Enable */
#define BITM_SPI_CTL_MASEN                                    (0X0002U)      /* Master Mode Enable */
#define BITP_SPI_CTL_CPHA                                     (2U)           /* Serial Clock Phase Mode */
#define BITL_SPI_CTL_CPHA                                     (1U)           /* Serial Clock Phase Mode */
#define BITM_SPI_CTL_CPHA                                     (0X0004U)      /* Serial Clock Phase Mode */
#define BITP_SPI_CTL_CPOL                                     (3U)           /* Serial Clock Polarity */
#define BITL_SPI_CTL_CPOL                                     (1U)           /* Serial Clock Polarity */
#define BITM_SPI_CTL_CPOL                                     (0X0008U)      /* Serial Clock Polarity */
#define BITP_SPI_CTL_WOM                                      (4U)           /* SPI Wired or Mode */
#define BITL_SPI_CTL_WOM                                      (1U)           /* SPI Wired or Mode */
#define BITM_SPI_CTL_WOM                                      (0X0010U)      /* SPI Wired or Mode */
#define BITP_SPI_CTL_LSB                                      (5U)           /* LSB First Transfer Enable */
#define BITL_SPI_CTL_LSB                                      (1U)           /* LSB First Transfer Enable */
#define BITM_SPI_CTL_LSB                                      (0X0020U)      /* LSB First Transfer Enable */
#define BITP_SPI_CTL_TIM                                      (6U)           /* SPI Transfer and Interrupt Mode */
#define BITL_SPI_CTL_TIM                                      (1U)           /* SPI Transfer and Interrupt Mode */
#define BITM_SPI_CTL_TIM                                      (0X0040U)      /* SPI Transfer and Interrupt Mode */
#define BITP_SPI_CTL_ZEN                                      (7U)           /* Transmit Zeros Enable */
#define BITL_SPI_CTL_ZEN                                      (1U)           /* Transmit Zeros Enable */
#define BITM_SPI_CTL_ZEN                                      (0X0080U)      /* Transmit Zeros Enable */
#define BITP_SPI_CTL_RXOF                                     (8U)           /* RX Overflow Overwrite Enable */
#define BITL_SPI_CTL_RXOF                                     (1U)           /* RX Overflow Overwrite Enable */
#define BITM_SPI_CTL_RXOF                                     (0X0100U)      /* RX Overflow Overwrite Enable */
#define BITP_SPI_CTL_OEN                                      (9U)           /* Slave MISO Output Enable */
#define BITL_SPI_CTL_OEN                                      (1U)           /* Slave MISO Output Enable */
#define BITM_SPI_CTL_OEN                                      (0X0200U)      /* Slave MISO Output Enable */
#define BITP_SPI_CTL_LOOPBACK                                 (10U)          /* Loopback Enable */
#define BITL_SPI_CTL_LOOPBACK                                 (1U)           /* Loopback Enable */
#define BITM_SPI_CTL_LOOPBACK                                 (0X0400U)      /* Loopback Enable */
#define BITP_SPI_CTL_CON                                      (11U)          /* Continuous Transfer Enable */
#define BITL_SPI_CTL_CON                                      (1U)           /* Continuous Transfer Enable */
#define BITM_SPI_CTL_CON                                      (0X0800U)      /* Continuous Transfer Enable */
#define BITP_SPI_CTL_RFLUSH                                   (12U)          /* SPI Rx FIFO Flush Enable */
#define BITL_SPI_CTL_RFLUSH                                   (1U)           /* SPI Rx FIFO Flush Enable */
#define BITM_SPI_CTL_RFLUSH                                   (0X1000U)      /* SPI Rx FIFO Flush Enable */
#define BITP_SPI_CTL_TFLUSH                                   (13U)          /* SPI Tx FIFO Flush Enable */
#define BITL_SPI_CTL_TFLUSH                                   (1U)           /* SPI Tx FIFO Flush Enable */
#define BITM_SPI_CTL_TFLUSH                                   (0X2000U)      /* SPI Tx FIFO Flush Enable */
#define BITP_SPI_CTL_CSRST                                    (14U)          /* Reset Mode for CS Error Bit */
#define BITL_SPI_CTL_CSRST                                    (1U)           /* Reset Mode for CS Error Bit */
#define BITM_SPI_CTL_CSRST                                    (0X4000U)      /* Reset Mode for CS Error Bit */

/* ----------------------------------------------------------------------------------------------------
          IEN                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_SPI_IEN_IRQMODE                                  (0U)           /* SPI IRQ Mode Bits */
#define BITL_SPI_IEN_IRQMODE                                  (3U)           /* SPI IRQ Mode Bits */
#define BITM_SPI_IEN_IRQMODE                                  (0X0007U)      /* SPI IRQ Mode Bits */
#define BITP_SPI_IEN_CS                                       (8U)           /* Enable Interrupt on Every CS Edge in Slave CON Mode */
#define BITL_SPI_IEN_CS                                       (1U)           /* Enable Interrupt on Every CS Edge in Slave CON Mode */
#define BITM_SPI_IEN_CS                                       (0X0100U)      /* Enable Interrupt on Every CS Edge in Slave CON Mode */
#define BITP_SPI_IEN_TXUNDR                                   (9U)           /* Tx-underflow Interrupt Enable */
#define BITL_SPI_IEN_TXUNDR                                   (1U)           /* Tx-underflow Interrupt Enable */
#define BITM_SPI_IEN_TXUNDR                                   (0X0200U)      /* Tx-underflow Interrupt Enable */
#define BITP_SPI_IEN_RXOVR                                    (10U)          /* Rx-overflow Interrupt Enable */
#define BITL_SPI_IEN_RXOVR                                    (1U)           /* Rx-overflow Interrupt Enable */
#define BITM_SPI_IEN_RXOVR                                    (0X0400U)      /* Rx-overflow Interrupt Enable */
#define BITP_SPI_IEN_RDY                                      (11U)          /* Ready Signal Edge Interrupt Enable */
#define BITL_SPI_IEN_RDY                                      (1U)           /* Ready Signal Edge Interrupt Enable */
#define BITM_SPI_IEN_RDY                                      (0X0800U)      /* Ready Signal Edge Interrupt Enable */
#define BITP_SPI_IEN_TXDONE                                   (12U)          /* SPI Transmit Done Interrupt Enable */
#define BITL_SPI_IEN_TXDONE                                   (1U)           /* SPI Transmit Done Interrupt Enable */
#define BITM_SPI_IEN_TXDONE                                   (0X1000U)      /* SPI Transmit Done Interrupt Enable */
#define BITP_SPI_IEN_XFRDONE                                  (13U)          /* SPI Transfer Completion Interrupt Enable */
#define BITL_SPI_IEN_XFRDONE                                  (1U)           /* SPI Transfer Completion Interrupt Enable */
#define BITM_SPI_IEN_XFRDONE                                  (0X2000U)      /* SPI Transfer Completion Interrupt Enable */
#define BITP_SPI_IEN_TXEMPTY                                  (14U)          /* Tx-FIFO Empty Interrupt Enable */
#define BITL_SPI_IEN_TXEMPTY                                  (1U)           /* Tx-FIFO Empty Interrupt Enable */
#define BITM_SPI_IEN_TXEMPTY                                  (0X4000U)      /* Tx-FIFO Empty Interrupt Enable */

#define ENUM_SPI_IEN_IRQMODE_TX1RX1                           (0X0000U)      /* Interrupt Occurs After 1 Byte is Transfered or Received */
#define ENUM_SPI_IEN_IRQMODE_TX2RX2                           (0X0001U)      /* Interrupt Occurs After 2 Byte is Transfered or Received */
#define ENUM_SPI_IEN_IRQMODE_TX3RX3                           (0X0002U)      /* Interrupt Occurs After 3 Byte is Transfered or Received */
#define ENUM_SPI_IEN_IRQMODE_TX4RX4                           (0X0003U)      /* Interrupt Occurs After 4 Byte is Transfered or Received */
#define ENUM_SPI_IEN_IRQMODE_TX5RX5                           (0X0004U)      /* Interrupt Occurs After 5 Byte is Transfered or Received */
#define ENUM_SPI_IEN_IRQMODE_TX6RX6                           (0X0005U)      /* Interrupt Occurs After 6 Byte is Transfered or Received */
#define ENUM_SPI_IEN_IRQMODE_TX7RX7                           (0X0006U)      /* Interrupt Occurs After 7 Byte is Transfered or Received */
#define ENUM_SPI_IEN_IRQMODE_TX8RX8                           (0X0007U)      /* Interrupt Occurs After 8 Byte is Transfered or Received */

/* ----------------------------------------------------------------------------------------------------
          CNT                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_SPI_CNT_VALUES                                   (0U)           /* Transfer Byte Count */
#define BITL_SPI_CNT_VALUES                                   (14U)          /* Transfer Byte Count */
#define BITM_SPI_CNT_VALUES                                   (0X3FFFU)      /* Transfer Byte Count */
#define BITP_SPI_CNT_FRAMECONT                                (15U)          /* Continue Frame */
#define BITL_SPI_CNT_FRAMECONT                                (1U)           /* Continue Frame */
#define BITM_SPI_CNT_FRAMECONT                                (0X8000U)      /* Continue Frame */

/* ----------------------------------------------------------------------------------------------------
          DMA                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_SPI_DMA_EN                                       (0U)           /* Enable DMA for Data Transfer */
#define BITL_SPI_DMA_EN                                       (1U)           /* Enable DMA for Data Transfer */
#define BITM_SPI_DMA_EN                                       (0X0001U)      /* Enable DMA for Data Transfer */
#define BITP_SPI_DMA_TXEN                                     (1U)           /* Enable Transmit DMA Request */
#define BITL_SPI_DMA_TXEN                                     (1U)           /* Enable Transmit DMA Request */
#define BITM_SPI_DMA_TXEN                                     (0X0002U)      /* Enable Transmit DMA Request */
#define BITP_SPI_DMA_RXEN                                     (2U)           /* Enable Receive DMA Request */
#define BITL_SPI_DMA_RXEN                                     (1U)           /* Enable Receive DMA Request */
#define BITM_SPI_DMA_RXEN                                     (0X0004U)      /* Enable Receive DMA Request */

/* ----------------------------------------------------------------------------------------------------
          FIFOSTAT                                              Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_SPI_FIFOSTAT_TX                                  (0U)           /* SPI Tx FIFO Status */
#define BITL_SPI_FIFOSTAT_TX                                  (4U)           /* SPI Tx FIFO Status */
#define BITM_SPI_FIFOSTAT_TX                                  (0X000FU)      /* SPI Tx FIFO Status */
#define BITP_SPI_FIFOSTAT_RX                                  (8U)           /* SPI Rx FIFO Status */
#define BITL_SPI_FIFOSTAT_RX                                  (4U)           /* SPI Rx FIFO Status */
#define BITM_SPI_FIFOSTAT_RX                                  (0X0F00U)      /* SPI Rx FIFO Status */

#define ENUM_SPI_FIFOSTAT_RX_EMPTY                            (0X0000U)      /* Rx FIFO Empty */
#define ENUM_SPI_FIFOSTAT_RX_ONE                              (0X0001U)      /* 1 Valid Byte/half-word in Rx FIFO */
#define ENUM_SPI_FIFOSTAT_RX_TWO                              (0X0002U)      /* 2 Valid Bytes/half-words in Rx FIFO */
#define ENUM_SPI_FIFOSTAT_RX_THREE                            (0X0003U)      /* 3 Valid Bytes/half-words in Rx FIFO */
#define ENUM_SPI_FIFOSTAT_RX_FOUR                             (0X0004U)      /* 4 Valid Bytes/half-words in Rx FIFO */
#define ENUM_SPI_FIFOSTAT_RX_FIVE                             (0X0005U)      /* 5 Valid Bytes/half-words in Rx FIFO */
#define ENUM_SPI_FIFOSTAT_RX_SIX                              (0X0006U)      /* 6 Valid Bytes/half-words in Rx FIFO */
#define ENUM_SPI_FIFOSTAT_RX_SEVEN                            (0X0007U)      /* 7 Valid Bytes/half-words in Rx FIFO */
#define ENUM_SPI_FIFOSTAT_RX_FULL                             (0X0008U)      /* 8 Valid Bytes/half-words in Rx FIFO (Rx FIFO Full) */
#define ENUM_SPI_FIFOSTAT_TX_EMPTY                            (0X0000U)      /* Tx FIFO Empty */
#define ENUM_SPI_FIFOSTAT_TX_ONE                              (0X0001U)      /* 1 Valid Byte/half-word in Tx FIFO */
#define ENUM_SPI_FIFOSTAT_TX_TWO                              (0X0002U)      /* 2 Valid Bytes/half-words in Tx FIFO */
#define ENUM_SPI_FIFOSTAT_TX_THREE                            (0X0003U)      /* 3 Valid Bytes/half-words in Tx FIFO */
#define ENUM_SPI_FIFOSTAT_TX_FOUR                             (0X0004U)      /* 4 Valid Bytes/half-words in Tx FIFO */
#define ENUM_SPI_FIFOSTAT_TX_FIVE                             (0X0005U)      /* 5 Valid Bytes/half-words in Tx FIFO */
#define ENUM_SPI_FIFOSTAT_TX_SIX                              (0X0006U)      /* 6 Valid Bytes/half-words in Tx FIFO */
#define ENUM_SPI_FIFOSTAT_TX_SEVEN                            (0X0007U)      /* 7 Valid Bytes/half-words in Tx FIFO */
#define ENUM_SPI_FIFOSTAT_TX_FULL                             (0X0008U)      /* 8 Valid Bytes/half-words in Tx FIFO (Tx FIFO Full) */

/* ----------------------------------------------------------------------------------------------------
          RDCTL                                                 Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_SPI_RDCTL_CMDEN                                  (0U)           /* Read Command Enable */
#define BITL_SPI_RDCTL_CMDEN                                  (1U)           /* Read Command Enable */
#define BITM_SPI_RDCTL_CMDEN                                  (0X0001U)      /* Read Command Enable */
#define BITP_SPI_RDCTL_OVERLAP                                (1U)           /* Tx/Rx Overlap Mode */
#define BITL_SPI_RDCTL_OVERLAP                                (1U)           /* Tx/Rx Overlap Mode */
#define BITM_SPI_RDCTL_OVERLAP                                (0X0002U)      /* Tx/Rx Overlap Mode */
#define BITP_SPI_RDCTL_TXBYTES                                (2U)           /* Transmit Byte Count Minus 1 for Read Command */
#define BITL_SPI_RDCTL_TXBYTES                                (4U)           /* Transmit Byte Count Minus 1 for Read Command */
#define BITM_SPI_RDCTL_TXBYTES                                (0X003CU)      /* Transmit Byte Count Minus 1 for Read Command */
#define BITP_SPI_RDCTL_THREEPIN                               (8U)           /* Three Pin SPI Mode */
#define BITL_SPI_RDCTL_THREEPIN                               (1U)           /* Three Pin SPI Mode */
#define BITM_SPI_RDCTL_THREEPIN                               (0X0100U)      /* Three Pin SPI Mode */

/* ----------------------------------------------------------------------------------------------------
          FLOWCTL                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_SPI_FLOWCTL_MODE                                 (0U)           /* Flow Control Mode */
#define BITL_SPI_FLOWCTL_MODE                                 (2U)           /* Flow Control Mode */
#define BITM_SPI_FLOWCTL_MODE                                 (0X0003U)      /* Flow Control Mode */
#define BITP_SPI_FLOWCTL_RDYPOL                               (4U)           /* Polarity of RDY/MISO Line */
#define BITL_SPI_FLOWCTL_RDYPOL                               (1U)           /* Polarity of RDY/MISO Line */
#define BITM_SPI_FLOWCTL_RDYPOL                               (0X0010U)      /* Polarity of RDY/MISO Line */
#define BITP_SPI_FLOWCTL_RDBURSTSZ                            (6U)           /* Read Data Burst Size Minus 1 */
#define BITL_SPI_FLOWCTL_RDBURSTSZ                            (10U)          /* Read Data Burst Size Minus 1 */
#define BITM_SPI_FLOWCTL_RDBURSTSZ                            (0XFFC0U)      /* Read Data Burst Size Minus 1 */

#define ENUM_SPI_FLOWCTL_RDYPOL_HIGH                          (0X0000U)      /* Polarity is Active HIGH. SPI Master Waits Until RDY/MISO Becomes HIGH. */
#define ENUM_SPI_FLOWCTL_RDYPOL_LOW                           (0X0001U)      /* Polarity is Active LOW. SPI Master Waits Until RDY/MISO Becomes LOW. */
#define ENUM_SPI_FLOWCTL_MODE_DISABLE                         (0X0000U)      /* Flow Control is Disabled. */
#define ENUM_SPI_FLOWCTL_MODE_TIMER                           (0X0001U)      /* Flow Control is Based on Timer (WAIT_TMR). */
#define ENUM_SPI_FLOWCTL_MODE_RDY                             (0X0002U)      /* Flow Control is Based on RDY Signal. */
#define ENUM_SPI_FLOWCTL_MODE_MISO                            (0X0003U)      /* Flow Control is Based on MISO Pin. */

/* ----------------------------------------------------------------------------------------------------
          WAITTMR                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_SPI_WAITTMR_TRIMS                                (0U)           /* Wait Timer for Flow-control */
#define BITL_SPI_WAITTMR_TRIMS                                (16U)          /* Wait Timer for Flow-control */
#define BITM_SPI_WAITTMR_TRIMS                                (0XFFFFU)      /* Wait Timer for Flow-control */

/* ----------------------------------------------------------------------------------------------------
          CSOVERRIDE                                            Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_SPI_CSOVERRIDE_CTL                               (0U)           /* CS Override Control */
#define BITL_SPI_CSOVERRIDE_CTL                               (2U)           /* CS Override Control */
#define BITM_SPI_CSOVERRIDE_CTL                               (0X0003U)      /* CS Override Control */

#define ENUM_SPI_CSOVERRIDE_CTL_NOTFORCE                      (0X0000U)      /* CS is Not Forced. */
#define ENUM_SPI_CSOVERRIDE_CTL_FORCETO1                      (0X0001U)      /* CS is Forced to Drive 1'b1. */
#define ENUM_SPI_CSOVERRIDE_CTL_FORCETO0                      (0X0002U)      /* CS is Forced to Drive 1'b0. */

#endif  /* end ifndef SPI_ADDR_RDEF_H_ */


#ifndef CLOCK_ADDR_RDEF_H_
#define CLOCK_ADDR_RDEF_H_    /* CLOCK: Clock Gating and Other Settings */

/* ====================================================================================================
        CLOCK Module Instances Address and Mask Definitions
   ==================================================================================================== */
#define INST_CLK                                                 (0X40060000U)    /* clk: */

#define MASK_CLOCK                                               (0X000003FFU)    /* CLOCK: Clock Gating and Other Settings */

/* ====================================================================================================
        CLOCK Module Register Address Offset Definitions
   ==================================================================================================== */
#define IDX_CLOCK_CLKCON0                                       (0X000U)    /* Misc Clock Settings Register */
#define IDX_CLOCK_CLKCON1                                       (0X004U)    /* Clock Dividers Register */
#define IDX_CLOCK_CLKSTAT0                                      (0X008U)    /* Clocking Status */

/* ====================================================================================================
        CLOCK Module Register ResetValue Definitions
   ==================================================================================================== */
#define RSTVAL_CLOCK_CLKCON0                                       (0X43C) 
#define RSTVAL_CLOCK_CLKCON1                                       (0X48) 
#define RSTVAL_CLOCK_CLKSTAT0                                      (0X0) 

/* ====================================================================================================
        CLOCK Module Register BitPositions, Lengths, Masks and Enumerations Definitions
   ==================================================================================================== */

/* ----------------------------------------------------------------------------------------------------
          CLKCON0                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_CLOCK_CLKCON0_CLKMUX                             (0U)           /* Clock Mux Select */
#define BITL_CLOCK_CLKCON0_CLKMUX                             (2U)           /* Clock Mux Select */
#define BITM_CLOCK_CLKCON0_CLKMUX                             (0X0003U)      /* Clock Mux Select */
#define BITP_CLOCK_CLKCON0_CLKOUT                             (2U)           /* GPIO CLK Out Select */
#define BITL_CLOCK_CLKCON0_CLKOUT                             (4U)           /* GPIO CLK Out Select */
#define BITM_CLOCK_CLKCON0_CLKOUT                             (0X003CU)      /* GPIO CLK Out Select */
#define BITP_CLOCK_CLKCON0_ANACLKMUX                          (7U)           /* Mux Selection Analog Clock Source */
#define BITL_CLOCK_CLKCON0_ANACLKMUX                          (2U)           /* Mux Selection Analog Clock Source */
#define BITM_CLOCK_CLKCON0_ANACLKMUX                          (0X0180U)      /* Mux Selection Analog Clock Source */
#define BITP_CLOCK_CLKCON0_SPLLIE                             (9U)           /* PLL Unlock and Lock Interrupt Enable */
#define BITL_CLOCK_CLKCON0_SPLLIE                             (1U)           /* PLL Unlock and Lock Interrupt Enable */
#define BITM_CLOCK_CLKCON0_SPLLIE                             (0X0200U)      /* PLL Unlock and Lock Interrupt Enable */
#define BITP_CLOCK_CLKCON0_ANAROOTCLKMUX                      (10U)          /* Clock Mux Select */
#define BITL_CLOCK_CLKCON0_ANAROOTCLKMUX                      (2U)           /* Clock Mux Select */
#define BITM_CLOCK_CLKCON0_ANAROOTCLKMUX                      (0X0C00U)      /* Clock Mux Select */

#define ENUM_CLOCK_CLKCON0_ANAROOTCLKMUX_OSC16                (0X0000U)      /* 16MHz Oscillator Clock */
#define ENUM_CLOCK_CLKCON0_ANAROOTCLKMUX_OSC32                (0X0001U)      /* 32MHz Oscillator Clock */
#define ENUM_CLOCK_CLKCON0_ANAROOTCLKMUX_PLL                  (0X0002U)      /* PLL Clock */
#define ENUM_CLOCK_CLKCON0_ANAROOTCLKMUX_GPIO                 (0X0003U)      /* External GPIO Clock */
#define ENUM_CLOCK_CLKCON0_SPLLIE_DIS                         (0X0000U)      /* PLL Interrupt Will Not Be Generated */
#define ENUM_CLOCK_CLKCON0_SPLLIE_EN                          (0X0001U)      /* PLL Interrupt Will Be Generated */
#define ENUM_CLOCK_CLKCON0_ANACLKMUX_HFOSC                    (0X0000U)      /* Internal Oscillator is Selected (HFOSC) */
#define ENUM_CLOCK_CLKCON0_ANACLKMUX_GPIOCLK                  (0X0002U)      /* GPIO Clock */
#define ENUM_CLOCK_CLKCON0_CLKOUT_HFOSC                       (0X0000U)      /* HFOSC (16 MHz) */
#define ENUM_CLOCK_CLKCON0_CLKOUT_ROOT                        (0X0001U)      /* Root Clock */
#define ENUM_CLOCK_CLKCON0_CLKOUT_LFOSC                       (0X0003U)      /* 32K OSC */
#define ENUM_CLOCK_CLKCON0_CLKOUT_CORE                        (0X0004U)      /* Core Clock */
#define ENUM_CLOCK_CLKCON0_CLKOUT_B0_PCLK                     (0X0005U)      /* Bridge 0 Pclk */
#define ENUM_CLOCK_CLKCON0_CLKOUT_B1_PCLK                     (0X0006U)      /* Bridge 1 PCLK */
#define ENUM_CLOCK_CLKCON0_CLKOUT_B2_PCLK                     (0X0007U)      /* Bridge 2 PCLK */
#define ENUM_CLOCK_CLKCON0_CLKOUT_ANA_CLK                     (0X0008U)      /* Analog Test Signal */
#define ENUM_CLOCK_CLKCON0_CLKOUT_T0                          (0X0009U)      /* Timer 0 Clock */
#define ENUM_CLOCK_CLKCON0_CLKOUT_WUT                         (0X000AU)      /* Wake up Timer Clock */
#define ENUM_CLOCK_CLKCON0_CLKOUT_T3                          (0X000BU)      /* Timer 3 Clock */
#define ENUM_CLOCK_CLKCON0_CLKOUT_HCLKBUS                     (0X000CU)      /* Hclk_bus */
#define ENUM_CLOCK_CLKCON0_CLKOUT_SPLL_CLK                    (0X000DU)      /* SPLL clock */
#define ENUM_CLOCK_CLKCON0_CLKMUX_HFOSC                       (0X0000U)      /* High Frequency Internal Oscillator (HFOSC) */
#define ENUM_CLOCK_CLKCON0_CLKMUX_SPLL                        (0X0001U)      /* System PLL is Selected (160 MHz) */
#define ENUM_CLOCK_CLKCON0_CLKMUX_EXTCLK                      (0X0003U)      /* External GPIO Port is Selected (ECLKIN) */

/* ----------------------------------------------------------------------------------------------------
          CLKCON1                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_CLOCK_CLKCON1_CDHCLK                             (0U)           /* HCLK Divide Bits */
#define BITL_CLOCK_CLKCON1_CDHCLK                             (3U)           /* HCLK Divide Bits */
#define BITM_CLOCK_CLKCON1_CDHCLK                             (0X0007U)      /* HCLK Divide Bits */
#define BITP_CLOCK_CLKCON1_CDPCLK0                            (3U)           /* APB0 PCLK Divide Bits */
#define BITL_CLOCK_CLKCON1_CDPCLK0                            (3U)           /* APB0 PCLK Divide Bits */
#define BITM_CLOCK_CLKCON1_CDPCLK0                            (0X0038U)      /* APB0 PCLK Divide Bits */
#define BITP_CLOCK_CLKCON1_CDPCLK1                            (6U)           /* APB1 PCLK Divide Bits */
#define BITL_CLOCK_CLKCON1_CDPCLK1                            (3U)           /* APB1 PCLK Divide Bits */
#define BITM_CLOCK_CLKCON1_CDPCLK1                            (0X01C0U)      /* APB1 PCLK Divide Bits */
#define BITP_CLOCK_CLKCON1_CDADCCLK                           (9U)           /* ADCCLK Divide Bits */
#define BITL_CLOCK_CLKCON1_CDADCCLK                           (3U)           /* ADCCLK Divide Bits */
#define BITM_CLOCK_CLKCON1_CDADCCLK                           (0X0E00U)      /* ADCCLK Divide Bits */

#define ENUM_CLOCK_CLKCON1_CDADCCLK_DIV1                      (0X0000U)      /* DIV1. Divide by 1 (ADCCLK is Equal to Root Clock) */
#define ENUM_CLOCK_CLKCON1_CDADCCLK_DIV2                      (0X0001U)      /* DIV2. Divide by 2 (ADCCLK is Half the Frequency of Root Clock) */
#define ENUM_CLOCK_CLKCON1_CDADCCLK_DIV4                      (0X0002U)      /* DIV4. Divide by 4 (ADCCLK is Quarter the Frequency of Root Clock, 20 MHz) */
#define ENUM_CLOCK_CLKCON1_CDADCCLK_DIV8                      (0X0003U)      /* DIV8. Divide by 8 */
#define ENUM_CLOCK_CLKCON1_CDADCCLK_DIV16                     (0X0004U)      /* DIV16. Divide by 16 */
#define ENUM_CLOCK_CLKCON1_CDADCCLK_DIV32                     (0X0005U)      /* DIV32. Divide by 32 */
#define ENUM_CLOCK_CLKCON1_CDADCCLK_DIV64                     (0X0006U)      /* DIV64. Divide by 164 */
#define ENUM_CLOCK_CLKCON1_CDADCCLK_DIV128                    (0X0007U)      /* DIV128. Divide by 128 */
#define ENUM_CLOCK_CLKCON1_CDPCLK1_DIV1                       (0X0000U)      /* DIV1. Divide by 1 (PCLK is Equal to Root Clock) */
#define ENUM_CLOCK_CLKCON1_CDPCLK1_DIV2                       (0X0001U)      /* DIV2. Divide by 2 (PCLK is Half the Frequency of Root Clock) */
#define ENUM_CLOCK_CLKCON1_CDPCLK1_DIV4                       (0X0002U)      /* DIV4. Divide by 4 (PCLK is Quarter the Frequency of Root Clock, 20 MHz) */
#define ENUM_CLOCK_CLKCON1_CDPCLK1_DIV8                       (0X0003U)      /* DIV8. Divide by 8 */
#define ENUM_CLOCK_CLKCON1_CDPCLK1_DIV16                      (0X0004U)      /* DIV16. Divide by 16 */
#define ENUM_CLOCK_CLKCON1_CDPCLK1_DIV32                      (0X0005U)      /* DIV32. Divide by 32 */
#define ENUM_CLOCK_CLKCON1_CDPCLK1_DIV64                      (0X0006U)      /* DIV64. Divide by 164 */
#define ENUM_CLOCK_CLKCON1_CDPCLK1_DIV128                     (0X0007U)      /* DIV128. Divide by 128 */
#define ENUM_CLOCK_CLKCON1_CDPCLK0_DIV1                       (0X0000U)      /* DIV1. Divide by 1 (PCLK is Equal to Root Clock) */
#define ENUM_CLOCK_CLKCON1_CDPCLK0_DIV2                       (0X0001U)      /* DIV2. Divide by 2 (PCLK is Half the Frequency of Root Clock) */
#define ENUM_CLOCK_CLKCON1_CDPCLK0_DIV4                       (0X0002U)      /* DIV4. Divide by 4 (PCLK is Quarter the Frequency of Root Clock, 20 MHz) */
#define ENUM_CLOCK_CLKCON1_CDPCLK0_DIV8                       (0X0003U)      /* DIV8. Divide by 8 */
#define ENUM_CLOCK_CLKCON1_CDPCLK0_DIV16                      (0X0004U)      /* DIV16. Divide by 16 */
#define ENUM_CLOCK_CLKCON1_CDPCLK0_DIV32                      (0X0005U)      /* DIV32. Divide by 32 */
#define ENUM_CLOCK_CLKCON1_CDPCLK0_DIV64                      (0X0006U)      /* DIV64. Divide by 164 */
#define ENUM_CLOCK_CLKCON1_CDPCLK0_DIV128                     (0X0007U)      /* DIV128. Divide by 128 */
#define ENUM_CLOCK_CLKCON1_CDHCLK_DIV1                        (0X0000U)      /* DIV1. Divide by 1 (HCLK is Equal to Root Clock) */
#define ENUM_CLOCK_CLKCON1_CDHCLK_DIV2                        (0X0001U)      /* DIV2. Divide by 2 (HCLK is Half the Frequency of Root Clock) */
#define ENUM_CLOCK_CLKCON1_CDHCLK_DIV4                        (0X0002U)      /* DIV4. Divide by 4 (HCLK is Quarter the Frequency of Root Clock) */
#define ENUM_CLOCK_CLKCON1_CDHCLK_DIV8                        (0X0003U)      /* DIV8. Divide by 8 */
#define ENUM_CLOCK_CLKCON1_CDHCLK_DIV16                       (0X0004U)      /* DIV16.Divide by 16 */
#define ENUM_CLOCK_CLKCON1_CDHCLK_DIV32                       (0X0005U)      /* DIV32.Divide by 32 */
#define ENUM_CLOCK_CLKCON1_CDHCLK_DIV64                       (0X0006U)      /* DIV64.Divide by 64 */
#define ENUM_CLOCK_CLKCON1_CDHCLK_DIV128                      (0X0007U)      /* DIV128. Divide by 128 */

/* ----------------------------------------------------------------------------------------------------
          CLKSTAT0                                              Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_CLOCK_CLKSTAT0_SPLLSTATUS                        (0U)           /* System PLL Status */
#define BITL_CLOCK_CLKSTAT0_SPLLSTATUS                        (1U)           /* System PLL Status */
#define BITM_CLOCK_CLKSTAT0_SPLLSTATUS                        (0X0001U)      /* System PLL Status */
#define BITP_CLOCK_CLKSTAT0_SPLLLOCKCLR                       (1U)           /* System PLL Lock */
#define BITL_CLOCK_CLKSTAT0_SPLLLOCKCLR                       (1U)           /* System PLL Lock */
#define BITM_CLOCK_CLKSTAT0_SPLLLOCKCLR                       (0X0002U)      /* System PLL Lock */
#define BITP_CLOCK_CLKSTAT0_SPLLUNLOCKCLR                     (2U)           /* System PLL Unlock */
#define BITL_CLOCK_CLKSTAT0_SPLLUNLOCKCLR                     (1U)           /* System PLL Unlock */
#define BITM_CLOCK_CLKSTAT0_SPLLUNLOCKCLR                     (0X0004U)      /* System PLL Unlock */
#define BITP_CLOCK_CLKSTAT0_SPLLLOCK                          (3U)           /* Sticky System PLL Lock Flag */
#define BITL_CLOCK_CLKSTAT0_SPLLLOCK                          (1U)           /* Sticky System PLL Lock Flag */
#define BITM_CLOCK_CLKSTAT0_SPLLLOCK                          (0X0008U)      /* Sticky System PLL Lock Flag */
#define BITP_CLOCK_CLKSTAT0_SPLLUNLOCK                        (4U)           /* Sticky System PLL Unlock Flag */
#define BITL_CLOCK_CLKSTAT0_SPLLUNLOCK                        (1U)           /* Sticky System PLL Unlock Flag */
#define BITM_CLOCK_CLKSTAT0_SPLLUNLOCK                        (0X0010U)      /* Sticky System PLL Unlock Flag */

#define ENUM_CLOCK_CLKSTAT0_SPLLUNLOCK_UNLOCKED               (0X0000U)      /* No PLL Lock Event Was Detected */
#define ENUM_CLOCK_CLKSTAT0_SPLLUNLOCK_LOCKED                 (0X0001U)      /* A PLL Lock Event Was Detected */
#define ENUM_CLOCK_CLKSTAT0_SPLLLOCK_UNLOCKED                 (0X0000U)      /* No PLL Lock Event Was Detected */
#define ENUM_CLOCK_CLKSTAT0_SPLLLOCK_LOCKED                   (0X0001U)      /* A PLL Lock Event Was Detected */
#define ENUM_CLOCK_CLKSTAT0_SPLLUNLOCKCLR_NOLOSS              (0X0000U)      /* No Loss of PLL Lock Was Detected */
#define ENUM_CLOCK_CLKSTAT0_SPLLUNLOCKCLR_LOSS                (0X0001U)      /* A PLL Loss of Lock Was Detected */
#define ENUM_CLOCK_CLKSTAT0_SPLLLOCKCLR_UNLOCKED              (0X0000U)      /* No PLL Lock Event Was Detected */
#define ENUM_CLOCK_CLKSTAT0_SPLLLOCKCLR_LOCKED                (0X0001U)      /* A PLL Lock Event Was Detected */
#define ENUM_CLOCK_CLKSTAT0_SPLLSTATUS_UNLOCKED               (0X0000U)      /* The PLL is Not Locked or Not Properly Configured. the PLL is Not Ready for Use as the System Clock Source */
#define ENUM_CLOCK_CLKSTAT0_SPLLSTATUS_LOCKED                 (0X0001U)      /* The PLL is Locked and is Ready for Use as the System Clock Source */

#endif  /* end ifndef CLOCK_ADDR_RDEF_H_ */


#ifndef PWM_ADDR_RDEF_H_
#define PWM_ADDR_RDEF_H_    /* PWM: PWM MMR */

/* ====================================================================================================
        PWM Module Instances Address and Mask Definitions
   ==================================================================================================== */
#define INST_PWM                                                 (0X40064000U)    /* pwm: */

#define MASK_PWM                                                 (0XFFFFFFFFU)    /* PWM: PWM MMR */

/* ====================================================================================================
        PWM Module Register Address Offset Definitions
   ==================================================================================================== */
#define IDX_PWM_PWMCON0                                         (0X00000000U)    /* PWM Control Register */
#define IDX_PWM_PWMCON1                                         (0X00000004U)    /* ADC Conversion Start and Trip Control Register */
#define IDX_PWM_PWMICLR                                         (0X00000008U)    /* Hardware Trip Configuration Register */
#define IDX_PWM_PWM0COM0                                        (0X00000010U)    /* Compare Register 0 for PWM0 and PWM1 */
#define IDX_PWM_PWM0COM1                                        (0X00000014U)    /* Compare Register 1 for PWM0 and PWM1 */
#define IDX_PWM_PWM0COM2                                        (0X00000018U)    /* Compare Register 2 for PWM0 and PWM1 */
#define IDX_PWM_PWM0LEN                                         (0X0000001CU)    /* Period Value Register for PWM0 and PWM1 */
#define IDX_PWM_PWM1COM0                                        (0X00000020U)    /* Compare Register 0 for PWM2 and PWM3 */
#define IDX_PWM_PWM1COM1                                        (0X00000024U)    /* Compare Register 1 for PWM2 and PWM3 */
#define IDX_PWM_PWM1COM2                                        (0X00000028U)    /* Compare Register 2 for PWM2 and PWM3 */
#define IDX_PWM_PWM1LEN                                         (0X0000002CU)    /* Period Value Register for PWM2 and PWM3 */
#define IDX_PWM_PWM2COM0                                        (0X00000030U)    /* Compare Register 0 for PWM4 and PWM5 */
#define IDX_PWM_PWM2COM1                                        (0X00000034U)    /* Compare Register 1 for PWM4 and PWM5 */
#define IDX_PWM_PWM2COM2                                        (0X00000038U)    /* Compare Register 2 for PWM4 and PWM5 */
#define IDX_PWM_PWM2LEN                                         (0X0000003CU)    /* Period Value Register for PWM4 and PWM5 */

/* ====================================================================================================
        PWM Module Register ResetValue Definitions
   ==================================================================================================== */
#define RSTVAL_PWM_PWMCON0                                         (0X12) 
#define RSTVAL_PWM_PWMCON1                                         (0X0) 
#define RSTVAL_PWM_PWMICLR                                         (0X0) 
#define RSTVAL_PWM_PWM0COM0                                        (0X0) 
#define RSTVAL_PWM_PWM0COM1                                        (0X0) 
#define RSTVAL_PWM_PWM0COM2                                        (0X0) 
#define RSTVAL_PWM_PWM0LEN                                         (0X0) 
#define RSTVAL_PWM_PWM1COM0                                        (0X0) 
#define RSTVAL_PWM_PWM1COM1                                        (0X0) 
#define RSTVAL_PWM_PWM1COM2                                        (0X0) 
#define RSTVAL_PWM_PWM1LEN                                         (0X0) 
#define RSTVAL_PWM_PWM2COM0                                        (0X0) 
#define RSTVAL_PWM_PWM2COM1                                        (0X0) 
#define RSTVAL_PWM_PWM2COM2                                        (0X0) 
#define RSTVAL_PWM_PWM2LEN                                         (0X0) 

/* ====================================================================================================
        PWM Module Register BitPositions, Lengths, Masks and Enumerations Definitions
   ==================================================================================================== */

/* ----------------------------------------------------------------------------------------------------
          PWMCON0                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_PWM_PWMCON0_PWMEN                                (0U)           /* Master Enable for PWM */
#define BITL_PWM_PWMCON0_PWMEN                                (1U)           /* Master Enable for PWM */
#define BITM_PWM_PWMCON0_PWMEN                                (0X0001U)      /* Master Enable for PWM */
#define BITP_PWM_PWMCON0_HMODE                                (1U)           /* Set to Enable H-bridge Mode */
#define BITL_PWM_PWMCON0_HMODE                                (1U)           /* Set to Enable H-bridge Mode */
#define BITM_PWM_PWMCON0_HMODE                                (0X0002U)      /* Set to Enable H-bridge Mode */
#define BITP_PWM_PWMCON0_HDIR                                 (2U)           /* Direction Control When PWM is in H-bridge Mode */
#define BITL_PWM_PWMCON0_HDIR                                 (1U)           /* Direction Control When PWM is in H-bridge Mode */
#define BITM_PWM_PWMCON0_HDIR                                 (0X0004U)      /* Direction Control When PWM is in H-bridge Mode */
#define BITP_PWM_PWMCON0_LCOMP                                (3U)           /* Signal to Load a New Set of Compare Register Values */
#define BITL_PWM_PWMCON0_LCOMP                                (1U)           /* Signal to Load a New Set of Compare Register Values */
#define BITM_PWM_PWMCON0_LCOMP                                (0X0008U)      /* Signal to Load a New Set of Compare Register Values */
#define BITP_PWM_PWMCON0_HOFF                                 (4U)           /* Set to Turn off the High-side for Pair 0/1 in H-bridge Mode */
#define BITL_PWM_PWMCON0_HOFF                                 (1U)           /* Set to Turn off the High-side for Pair 0/1 in H-bridge Mode */
#define BITM_PWM_PWMCON0_HOFF                                 (0X0010U)      /* Set to Turn off the High-side for Pair 0/1 in H-bridge Mode */
#define BITP_PWM_PWMCON0_POINV                                (5U)           /* Set to Invert PWM Outputs for Pair 0/1 in H-bridge Mode */
#define BITL_PWM_PWMCON0_POINV                                (1U)           /* Set to Invert PWM Outputs for Pair 0/1 in H-bridge Mode */
#define BITM_PWM_PWMCON0_POINV                                (0X0020U)      /* Set to Invert PWM Outputs for Pair 0/1 in H-bridge Mode */
#define BITP_PWM_PWMCON0_PWMCMP                               (6U)           /* PWM Clock Prescaler. Sets UCLK Divider. */
#define BITL_PWM_PWMCON0_PWMCMP                               (3U)           /* PWM Clock Prescaler. Sets UCLK Divider. */
#define BITM_PWM_PWMCON0_PWMCMP                               (0X01C0U)      /* PWM Clock Prescaler. Sets UCLK Divider. */
#define BITP_PWM_PWMCON0_ENA                                  (9U)           /* Enable for Pair 0 and 1 When HOFF=0 and HMODE=1 */
#define BITL_PWM_PWMCON0_ENA                                  (1U)           /* Enable for Pair 0 and 1 When HOFF=0 and HMODE=1 */
#define BITM_PWM_PWMCON0_ENA                                  (0X0200U)      /* Enable for Pair 0 and 1 When HOFF=0 and HMODE=1 */
#define BITP_PWM_PWMCON0_PWMIEN                               (10U)          /* Set to Enable Interrupts for PWM */
#define BITL_PWM_PWMCON0_PWMIEN                               (1U)           /* Set to Enable Interrupts for PWM */
#define BITM_PWM_PWMCON0_PWMIEN                               (0X0400U)      /* Set to Enable Interrupts for PWM */
#define BITP_PWM_PWMCON0_PWM1INV                              (11U)          /* Set to Invert PWM1 Output */
#define BITL_PWM_PWMCON0_PWM1INV                              (1U)           /* Set to Invert PWM1 Output */
#define BITM_PWM_PWMCON0_PWM1INV                              (0X0800U)      /* Set to Invert PWM1 Output */
#define BITP_PWM_PWMCON0_PWM3INV                              (12U)          /* Set to Invert PWM3 Output */
#define BITL_PWM_PWMCON0_PWM3INV                              (1U)           /* Set to Invert PWM3 Output */
#define BITM_PWM_PWMCON0_PWM3INV                              (0X1000U)      /* Set to Invert PWM3 Output */
#define BITP_PWM_PWMCON0_PWM5INV                              (13U)          /* Set to Invert PWM5 Output */
#define BITL_PWM_PWMCON0_PWM5INV                              (1U)           /* Set to Invert PWM5 Output */
#define BITM_PWM_PWMCON0_PWM5INV                              (0X2000U)      /* Set to Invert PWM5 Output */
#define BITP_PWM_PWMCON0_SYNC                                 (15U)          /* Set to Enable PWM Synchronization from the SYNC Pin */
#define BITL_PWM_PWMCON0_SYNC                                 (1U)           /* Set to Enable PWM Synchronization from the SYNC Pin */
#define BITM_PWM_PWMCON0_SYNC                                 (0X8000U)      /* Set to Enable PWM Synchronization from the SYNC Pin */

#define ENUM_PWM_PWMCON0_SYNC_DIS                             (0X0000U)      /* Ignore Transition from the SYNC Pin */
#define ENUM_PWM_PWMCON0_SYNC_EN                              (0X0001U)      /* All PWM Counters are Reset on the Next Clock Cycle After Detection of a Falling Edge from SYNC Pin */
#define ENUM_PWM_PWMCON0_ENA_DIS                              (0X0000U)      /* Disable Pair 0 and 1 */
#define ENUM_PWM_PWMCON0_ENA_EN                               (0X0001U)      /* Enable Pair 0 and 1 */
#define ENUM_PWM_PWMCON0_PWMCMP_DIV2                          (0X0000U)      /* UCLK/2 */
#define ENUM_PWM_PWMCON0_PWMCMP_DIV4                          (0X0001U)      /* UCLK/4 */
#define ENUM_PWM_PWMCON0_PWMCMP_DIV8                          (0X0002U)      /* UCLK/8 */
#define ENUM_PWM_PWMCON0_PWMCMP_DIV16                         (0X0003U)      /* UCLK/16 */
#define ENUM_PWM_PWMCON0_PWMCMP_DIV32                         (0X0004U)      /* UCLK/32 */
#define ENUM_PWM_PWMCON0_PWMCMP_DIV64                         (0X0005U)      /* UCLK/64 */
#define ENUM_PWM_PWMCON0_PWMCMP_DIV128                        (0X0006U)      /* UCLK/128 */
#define ENUM_PWM_PWMCON0_PWMCMP_DIV256                        (0X0007U)      /* UCLK/256 */
#define ENUM_PWM_PWMCON0_LCOMP_DIS                            (0X0000U)      /* Use the Values Previously Store in the Compare and Length Registers */
#define ENUM_PWM_PWMCON0_LCOMP_EN                             (0X0001U)      /* Load the Internal Compare Registers with Values Stored in the PWMxCOMx and PWMxLEN Registers */
#define ENUM_PWM_PWMCON0_HDIR_DIS                             (0X0000U)      /* PWM2 and PWM3 Act as Output Signals While PWM0 and PWM1 are Held Low */
#define ENUM_PWM_PWMCON0_HDIR_EN                              (0X0001U)      /* PWM0 and PWM1 Act as Output Signals While PWM2 and PWM3 are Held Low */
#define ENUM_PWM_PWMCON0_PWMEN_DIS                            (0X0000U)      /* Disable All PWM Outputs */
#define ENUM_PWM_PWMCON0_PWMEN_EN                             (0X0001U)      /* Enable All PWM Outputs */

/* ----------------------------------------------------------------------------------------------------
          PWMCON1                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_PWM_PWMCON1_REVREG0                              (4U)           /* Reserved */
#define BITL_PWM_PWMCON1_REVREG0                              (2U)           /* Reserved */
#define BITM_PWM_PWMCON1_REVREG0                              (0X0030U)      /* Reserved */
#define BITP_PWM_PWMCON1_TRIP_EN                              (6U)           /* Set to Enable PWM Trip Functionality */
#define BITL_PWM_PWMCON1_TRIP_EN                              (1U)           /* Set to Enable PWM Trip Functionality */
#define BITM_PWM_PWMCON1_TRIP_EN                              (0X0040U)      /* Set to Enable PWM Trip Functionality */
#define BITP_PWM_PWMCON1_REVREG1                              (8U)           /* Reserved. Return 0 on Reads */
#define BITL_PWM_PWMCON1_REVREG1                              (8U)           /* Reserved. Return 0 on Reads */
#define BITM_PWM_PWMCON1_REVREG1                              (0XFF00U)      /* Reserved. Return 0 on Reads */

/* ----------------------------------------------------------------------------------------------------
          PWMICLR                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_PWM_PWMICLR_PWM0                                 (0U)           /* Write a 1 to Clear Latched IRQPWM0 Interrupt. */
#define BITL_PWM_PWMICLR_PWM0                                 (1U)           /* Write a 1 to Clear Latched IRQPWM0 Interrupt. */
#define BITM_PWM_PWMICLR_PWM0                                 (0X0001U)      /* Write a 1 to Clear Latched IRQPWM0 Interrupt. */
#define BITP_PWM_PWMICLR_PWM1                                 (1U)           /* Write a 1 to Clear Latched IRQPWM1 Interrupt. */
#define BITL_PWM_PWMICLR_PWM1                                 (1U)           /* Write a 1 to Clear Latched IRQPWM1 Interrupt. */
#define BITM_PWM_PWMICLR_PWM1                                 (0X0002U)      /* Write a 1 to Clear Latched IRQPWM1 Interrupt. */
#define BITP_PWM_PWMICLR_PWM2                                 (2U)           /* Write a 1 to Clear Latched IRQPWM2 Interrupt. */
#define BITL_PWM_PWMICLR_PWM2                                 (1U)           /* Write a 1 to Clear Latched IRQPWM2 Interrupt. */
#define BITM_PWM_PWMICLR_PWM2                                 (0X0004U)      /* Write a 1 to Clear Latched IRQPWM2 Interrupt. */
#define BITP_PWM_PWMICLR_TRIP                                 (4U)           /* Write a 1 to Clear Latched IRQPWMTrip Interrupt. */
#define BITL_PWM_PWMICLR_TRIP                                 (1U)           /* Write a 1 to Clear Latched IRQPWMTrip Interrupt. */
#define BITM_PWM_PWMICLR_TRIP                                 (0X0010U)      /* Write a 1 to Clear Latched IRQPWMTrip Interrupt. */
#define BITP_PWM_PWMICLR_REVREG2                              (5U)           /* Reserved. Return 0 on Reads */
#define BITL_PWM_PWMICLR_REVREG2                              (11U)          /* Reserved. Return 0 on Reads */
#define BITM_PWM_PWMICLR_REVREG2                              (0XFFE0U)      /* Reserved. Return 0 on Reads */

/* ----------------------------------------------------------------------------------------------------
          PWM0COM0                                              Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_PWM_PWM0COM0_PWM0COM0                            (0U)           /* Compare Register 0 for PWM0 and PWM1 */
#define BITL_PWM_PWM0COM0_PWM0COM0                            (16U)          /* Compare Register 0 for PWM0 and PWM1 */
#define BITM_PWM_PWM0COM0_PWM0COM0                            (0XFFFFU)      /* Compare Register 0 for PWM0 and PWM1 */

/* ----------------------------------------------------------------------------------------------------
          PWM0COM1                                              Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_PWM_PWM0COM1_PWM0COM1                            (0U)           /* Compare Register 1 for PWM0 and PWM1 */
#define BITL_PWM_PWM0COM1_PWM0COM1                            (16U)          /* Compare Register 1 for PWM0 and PWM1 */
#define BITM_PWM_PWM0COM1_PWM0COM1                            (0XFFFFU)      /* Compare Register 1 for PWM0 and PWM1 */

/* ----------------------------------------------------------------------------------------------------
          PWM0COM2                                              Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_PWM_PWM0COM2_PWM0COM2                            (0U)           /* Compare Register 2 for PWM0 and PWM1 */
#define BITL_PWM_PWM0COM2_PWM0COM2                            (16U)          /* Compare Register 2 for PWM0 and PWM1 */
#define BITM_PWM_PWM0COM2_PWM0COM2                            (0XFFFFU)      /* Compare Register 2 for PWM0 and PWM1 */

/* ----------------------------------------------------------------------------------------------------
          PWM0LEN                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_PWM_PWM0LEN_PWM0LEN                              (0U)           /* Period Value Register for PWM0 and PWM1 */
#define BITL_PWM_PWM0LEN_PWM0LEN                              (16U)          /* Period Value Register for PWM0 and PWM1 */
#define BITM_PWM_PWM0LEN_PWM0LEN                              (0XFFFFU)      /* Period Value Register for PWM0 and PWM1 */

/* ----------------------------------------------------------------------------------------------------
          PWM1COM0                                              Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_PWM_PWM1COM0_PWM1COM0                            (0U)           /* Compare Register 0 for PWM2 and PWM3 */
#define BITL_PWM_PWM1COM0_PWM1COM0                            (16U)          /* Compare Register 0 for PWM2 and PWM3 */
#define BITM_PWM_PWM1COM0_PWM1COM0                            (0XFFFFU)      /* Compare Register 0 for PWM2 and PWM3 */

/* ----------------------------------------------------------------------------------------------------
          PWM1COM1                                              Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_PWM_PWM1COM1_PWM1COM1                            (0U)           /* Compare Register 1 for PWM2 and PWM3 */
#define BITL_PWM_PWM1COM1_PWM1COM1                            (16U)          /* Compare Register 1 for PWM2 and PWM3 */
#define BITM_PWM_PWM1COM1_PWM1COM1                            (0XFFFFU)      /* Compare Register 1 for PWM2 and PWM3 */

/* ----------------------------------------------------------------------------------------------------
          PWM1COM2                                              Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_PWM_PWM1COM2_PWM1COM2                            (0U)           /* Compare Register 2 for PWM2 and PWM3 */
#define BITL_PWM_PWM1COM2_PWM1COM2                            (16U)          /* Compare Register 2 for PWM2 and PWM3 */
#define BITM_PWM_PWM1COM2_PWM1COM2                            (0XFFFFU)      /* Compare Register 2 for PWM2 and PWM3 */

/* ----------------------------------------------------------------------------------------------------
          PWM1LEN                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_PWM_PWM1LEN_PWM1LEN                              (0U)           /* Period Value Register for PWM2 and PWM3 */
#define BITL_PWM_PWM1LEN_PWM1LEN                              (16U)          /* Period Value Register for PWM2 and PWM3 */
#define BITM_PWM_PWM1LEN_PWM1LEN                              (0XFFFFU)      /* Period Value Register for PWM2 and PWM3 */

/* ----------------------------------------------------------------------------------------------------
          PWM2COM0                                              Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_PWM_PWM2COM0_PWM2COM0                            (0U)           /* Compare Register 0 for PWM4 and PWM5 */
#define BITL_PWM_PWM2COM0_PWM2COM0                            (16U)          /* Compare Register 0 for PWM4 and PWM5 */
#define BITM_PWM_PWM2COM0_PWM2COM0                            (0XFFFFU)      /* Compare Register 0 for PWM4 and PWM5 */

/* ----------------------------------------------------------------------------------------------------
          PWM2COM1                                              Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_PWM_PWM2COM1_PWM2COM1                            (0U)           /* Compare Register 1 for PWM4 and PWM5 */
#define BITL_PWM_PWM2COM1_PWM2COM1                            (16U)          /* Compare Register 1 for PWM4 and PWM5 */
#define BITM_PWM_PWM2COM1_PWM2COM1                            (0XFFFFU)      /* Compare Register 1 for PWM4 and PWM5 */

/* ----------------------------------------------------------------------------------------------------
          PWM2COM2                                              Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_PWM_PWM2COM2_PWM2COM2                            (0U)           /* Compare Register 2 for PWM4 and PWM5 */
#define BITL_PWM_PWM2COM2_PWM2COM2                            (16U)          /* Compare Register 2 for PWM4 and PWM5 */
#define BITM_PWM_PWM2COM2_PWM2COM2                            (0XFFFFU)      /* Compare Register 2 for PWM4 and PWM5 */

/* ----------------------------------------------------------------------------------------------------
          PWM2LEN                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_PWM_PWM2LEN_PWM2LEN                              (0U)           /* Period Value Register for PWM4 and PWM5 */
#define BITL_PWM_PWM2LEN_PWM2LEN                              (16U)          /* Period Value Register for PWM4 and PWM5 */
#define BITM_PWM_PWM2LEN_PWM2LEN                              (0XFFFFU)      /* Period Value Register for PWM4 and PWM5 */

#endif  /* end ifndef PWM_ADDR_RDEF_H_ */


#ifndef SUBSYS_ADDR_RDEF_H_
#define SUBSYS_ADDR_RDEF_H_    /* SUBSYS: Your module description, here. */

/* ====================================================================================================
        SUBSYS Module Instances Address and Mask Definitions
   ==================================================================================================== */
#define INST_SRAM                                                (0X40065000U)    /* sram: */

#define MASK_SUBSYS                                              (0X00000FFFU)    /* SUBSYS: Your module description, here. */

/* ====================================================================================================
        SUBSYS Module Register Address Offset Definitions
   ==================================================================================================== */
#define IDX_SUBSYS_SRAMCON                                      (0X000U)    /* SRAM Control Register */
#define IDX_SUBSYS_SRAMECCCON                                   (0X00CU)    /* SRAM ECC Control Register */
#define IDX_SUBSYS_SRAMECCSTA                                   (0X010U)    /* SRAM ECC Status Register */
#define IDX_SUBSYS_SRAMECCA0                                    (0X014U)    /* SRAM0 ECC Error Address Register */
#define IDX_SUBSYS_SRAMECCD0                                    (0X018U)    /* SRAM0 ECC Error Data Register */
#define IDX_SUBSYS_SRAMECCP0                                    (0X01CU)    /* SRAM0 ECC Error Parity Register */
#define IDX_SUBSYS_SRAMECCA1                                    (0X020U)    /* SRAM1 ECC Error Address Register */
#define IDX_SUBSYS_SRAMECCD1                                    (0X024U)    /* SRAM1 ECC Error Data Register */
#define IDX_SUBSYS_SRAMECCP1                                    (0X028U)    /* SRAM1 ECC Error Parity Register */
#define IDX_SUBSYS_SRAMECCA2                                    (0X02CU)    /* SRAM2 ECC Error Address Register */
#define IDX_SUBSYS_SRAMECCD2                                    (0X030U)    /* SRAM2 ECC Error Data Register */
#define IDX_SUBSYS_SRAMECCP2                                    (0X034U)    /* SRAM2 ECC Error Parity Register */

/* ====================================================================================================
        SUBSYS Module Register ResetValue Definitions
   ==================================================================================================== */
#define RSTVAL_SUBSYS_SRAMCON                                      (0X2) 
#define RSTVAL_SUBSYS_SRAMECCCON                                   (0XCF) 
#define RSTVAL_SUBSYS_SRAMECCSTA                                   (0X0) 
#define RSTVAL_SUBSYS_SRAMECCA0                                    (0X0) 
#define RSTVAL_SUBSYS_SRAMECCD0                                    (0X0) 
#define RSTVAL_SUBSYS_SRAMECCP0                                    (0X0) 
#define RSTVAL_SUBSYS_SRAMECCA1                                    (0X0) 
#define RSTVAL_SUBSYS_SRAMECCD1                                    (0X0) 
#define RSTVAL_SUBSYS_SRAMECCP1                                    (0X0) 
#define RSTVAL_SUBSYS_SRAMECCA2                                    (0X0) 
#define RSTVAL_SUBSYS_SRAMECCD2                                    (0X0) 
#define RSTVAL_SUBSYS_SRAMECCP2                                    (0X0) 

/* ====================================================================================================
        SUBSYS Module Register BitPositions, Lengths, Masks and Enumerations Definitions
   ==================================================================================================== */

/* ----------------------------------------------------------------------------------------------------
          SRAMCON                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_SUBSYS_SRAMCON_REMAP                             (0U)           /* Using SRAM0 as Instruction SRAM */
#define BITL_SUBSYS_SRAMCON_REMAP                             (1U)           /* Using SRAM0 as Instruction SRAM */
#define BITM_SUBSYS_SRAMCON_REMAP                             (0X00000001U)  /* Using SRAM0 as Instruction SRAM */
#define BITP_SUBSYS_SRAMCON_CSEL                              (1U)           /* Use Cache as a Part of SRAM0 */
#define BITL_SUBSYS_SRAMCON_CSEL                              (1U)           /* Use Cache as a Part of SRAM0 */
#define BITM_SUBSYS_SRAMCON_CSEL                              (0X00000002U)  /* Use Cache as a Part of SRAM0 */

#define ENUM_SUBSYS_SRAMCON_CSEL_CAS                          (0X00000001U)  /* Cache as SRAM */
#define ENUM_SUBSYS_SRAMCON_CSEL_NCAS                         (0X00000000U)  /* Cache not as SRAM */
#define ENUM_SUBSYS_SRAMCON_REMAP_CODRAM                      (0X00000000U)  /* Using as Code SRAM */
#define ENUM_SUBSYS_SRAMCON_REMAP_SYSRAM                      (0X00000001U)  /* Using as System SRAM */

/* ----------------------------------------------------------------------------------------------------
          SRAMECCCON                                            Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_SUBSYS_SRAMECCCON_EN                             (0U)           /* ECC Check Enable */
#define BITL_SUBSYS_SRAMECCCON_EN                             (3U)           /* ECC Check Enable */
#define BITM_SUBSYS_SRAMECCCON_EN                             (0X00000007U)  /* ECC Check Enable */
#define BITP_SUBSYS_SRAMECCCON_BUSERRTYPE                     (3U)           /* ECC AHB Bus Error Response Type */
#define BITL_SUBSYS_SRAMECCCON_BUSERRTYPE                     (2U)           /* ECC AHB Bus Error Response Type */
#define BITM_SUBSYS_SRAMECCCON_BUSERRTYPE                     (0X00000018U)  /* ECC AHB Bus Error Response Type */
#define BITP_SUBSYS_SRAMECCCON_INTERRTYPE                     (5U)           /* ECC Interrupt Error Response Type */
#define BITL_SUBSYS_SRAMECCCON_INTERRTYPE                     (2U)           /* ECC Interrupt Error Response Type */
#define BITM_SUBSYS_SRAMECCCON_INTERRTYPE                     (0X00000060U)  /* ECC Interrupt Error Response Type */
#define BITP_SUBSYS_SRAMECCCON_RECERRTYPE                     (7U)           /* ECC Error Address and Data Record Type */
#define BITL_SUBSYS_SRAMECCCON_RECERRTYPE                     (2U)           /* ECC Error Address and Data Record Type */
#define BITM_SUBSYS_SRAMECCCON_RECERRTYPE                     (0X00000180U)  /* ECC Error Address and Data Record Type */

#define ENUM_SUBSYS_SRAMECCCON_RECERRTYPE_CURERR              (0X00000000U)  /* Return Current Error Address and Data Information */
#define ENUM_SUBSYS_SRAMECCCON_RECERRTYPE_STRERR0             (0X00000001U)  /* Return Stored Error Address and Data Information */
#define ENUM_SUBSYS_SRAMECCCON_INTERRTYPE_NOERR               (0X00000000U)  /* No Error Report */
#define ENUM_SUBSYS_SRAMECCCON_INTERRTYPE_ERR2B               (0X00000001U)  /* 2 Bits Error Report */
#define ENUM_SUBSYS_SRAMECCCON_INTERRTYPE_ERR1B               (0X00000002U)  /* 1 Bit Error Report */
#define ENUM_SUBSYS_SRAMECCCON_INTERRTYPE_ALERR               (0X00000003U)  /* 1 or 2 Bits Error Report */
#define ENUM_SUBSYS_SRAMECCCON_BUSERRTYPE_NOERR               (0X00000000U)  /* No Error Report */
#define ENUM_SUBSYS_SRAMECCCON_BUSERRTYPE_ERR2B               (0X00000001U)  /* 2 Bits Error Report */
#define ENUM_SUBSYS_SRAMECCCON_BUSERRTYPE_ERR1B               (0X00000002U)  /* 1 Bit Error Report */
#define ENUM_SUBSYS_SRAMECCCON_BUSERRTYPE_ALERR               (0X00000003U)  /* 1 or 2 Bits Error Report */
#define ENUM_SUBSYS_SRAMECCCON_EN_EN0                         (0X00000001U)  /* SRAM0 ECC Enable */
#define ENUM_SUBSYS_SRAMECCCON_EN_EN1                         (0X00000002U)  /* SRAM1 ECC Enable */
#define ENUM_SUBSYS_SRAMECCCON_EN_EN2                         (0X00000004U)  /* SRAM2 ECC Enable */
#define ENUM_SUBSYS_SRAMECCCON_EN_EN01                        (0X00000003U)  /* SRAM0 and SRAM1 ECC Enable */
#define ENUM_SUBSYS_SRAMECCCON_EN_EN02                        (0X00000005U)  /* SRAM0 and SRAM2 ECC Enable */
#define ENUM_SUBSYS_SRAMECCCON_EN_EN12                        (0X00000006U)  /* SRAM1 and SRAM2 ECC Enable */
#define ENUM_SUBSYS_SRAMECCCON_EN_EN012                       (0X00000007U)  /* SRAM0 and SRAM1 and SRAM2 ECC Enable */
#define ENUM_SUBSYS_SRAMECCCON_EN_ENNA                        (0X00000000U)  /* None SRAM ECC Enable */

/* ----------------------------------------------------------------------------------------------------
          SRAMECCSTA                                            Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_SUBSYS_SRAMECCSTA_S0ERR2B                        (0U)           /* SRAM0 ECC 2 Bits Error */
#define BITL_SUBSYS_SRAMECCSTA_S0ERR2B                        (1U)           /* SRAM0 ECC 2 Bits Error */
#define BITM_SUBSYS_SRAMECCSTA_S0ERR2B                        (0X00000001U)  /* SRAM0 ECC 2 Bits Error */
#define BITP_SUBSYS_SRAMECCSTA_S0ERR1B                        (1U)           /* SRAM0 ECC 1 Bit Error */
#define BITL_SUBSYS_SRAMECCSTA_S0ERR1B                        (1U)           /* SRAM0 ECC 1 Bit Error */
#define BITM_SUBSYS_SRAMECCSTA_S0ERR1B                        (0X00000002U)  /* SRAM0 ECC 1 Bit Error */
#define BITP_SUBSYS_SRAMECCSTA_S1ERR2B                        (2U)           /* SRAM1 ECC 2 Bits Error */
#define BITL_SUBSYS_SRAMECCSTA_S1ERR2B                        (1U)           /* SRAM1 ECC 2 Bits Error */
#define BITM_SUBSYS_SRAMECCSTA_S1ERR2B                        (0X00000004U)  /* SRAM1 ECC 2 Bits Error */
#define BITP_SUBSYS_SRAMECCSTA_S1ERR1B                        (3U)           /* SRAM1 ECC 1 Bit Error */
#define BITL_SUBSYS_SRAMECCSTA_S1ERR1B                        (1U)           /* SRAM1 ECC 1 Bit Error */
#define BITM_SUBSYS_SRAMECCSTA_S1ERR1B                        (0X00000008U)  /* SRAM1 ECC 1 Bit Error */
#define BITP_SUBSYS_SRAMECCSTA_S2ERR2B                        (4U)           /* SRAM2 ECC 2 Bits Error */
#define BITL_SUBSYS_SRAMECCSTA_S2ERR2B                        (1U)           /* SRAM2 ECC 2 Bits Error */
#define BITM_SUBSYS_SRAMECCSTA_S2ERR2B                        (0X00000010U)  /* SRAM2 ECC 2 Bits Error */
#define BITP_SUBSYS_SRAMECCSTA_S2ERR1B                        (5U)           /* SRAM2 ECC 1 Bits Error */
#define BITL_SUBSYS_SRAMECCSTA_S2ERR1B                        (1U)           /* SRAM2 ECC 1 Bits Error */
#define BITM_SUBSYS_SRAMECCSTA_S2ERR1B                        (0X00000020U)  /* SRAM2 ECC 1 Bits Error */
#define BITP_SUBSYS_SRAMECCSTA_ERRCNT                         (8U)           /* ECC Error Counter */
#define BITL_SUBSYS_SRAMECCSTA_ERRCNT                         (3U)           /* ECC Error Counter */
#define BITM_SUBSYS_SRAMECCSTA_ERRCNT                         (0X00000700U)  /* ECC Error Counter */

/* ----------------------------------------------------------------------------------------------------
          SRAMECCA0                                             Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_SUBSYS_SRAMECCA0_ADDR                            (0U)           /* ECC Error Address */
#define BITL_SUBSYS_SRAMECCA0_ADDR                            (32U)          /* ECC Error Address */
#define BITM_SUBSYS_SRAMECCA0_ADDR                            (0XFFFFFFFFU)  /* ECC Error Address */

/* ----------------------------------------------------------------------------------------------------
          SRAMECCD0                                             Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_SUBSYS_SRAMECCD0_DATA                            (0U)           /* ECC Error Raw Data */
#define BITL_SUBSYS_SRAMECCD0_DATA                            (32U)          /* ECC Error Raw Data */
#define BITM_SUBSYS_SRAMECCD0_DATA                            (0XFFFFFFFFU)  /* ECC Error Raw Data */

/* ----------------------------------------------------------------------------------------------------
          SRAMECCP0                                             Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_SUBSYS_SRAMECCP0_PARITY                          (0U)           /* ECC Error Raw Parity */
#define BITL_SUBSYS_SRAMECCP0_PARITY                          (7U)           /* ECC Error Raw Parity */
#define BITM_SUBSYS_SRAMECCP0_PARITY                          (0X0000007FU)  /* ECC Error Raw Parity */

/* ----------------------------------------------------------------------------------------------------
          SRAMECCA1                                             Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_SUBSYS_SRAMECCA1_ADDR                            (0U)           /* ECC Error Address */
#define BITL_SUBSYS_SRAMECCA1_ADDR                            (32U)          /* ECC Error Address */
#define BITM_SUBSYS_SRAMECCA1_ADDR                            (0XFFFFFFFFU)  /* ECC Error Address */

/* ----------------------------------------------------------------------------------------------------
          SRAMECCD1                                             Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_SUBSYS_SRAMECCD1_DATA                            (0U)           /* ECC Error Raw Data */
#define BITL_SUBSYS_SRAMECCD1_DATA                            (32U)          /* ECC Error Raw Data */
#define BITM_SUBSYS_SRAMECCD1_DATA                            (0XFFFFFFFFU)  /* ECC Error Raw Data */

/* ----------------------------------------------------------------------------------------------------
          SRAMECCP1                                             Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_SUBSYS_SRAMECCP1_PARITY                          (0U)           /* ECC Error Raw Parity */
#define BITL_SUBSYS_SRAMECCP1_PARITY                          (7U)           /* ECC Error Raw Parity */
#define BITM_SUBSYS_SRAMECCP1_PARITY                          (0X0000007FU)  /* ECC Error Raw Parity */

/* ----------------------------------------------------------------------------------------------------
          SRAMECCA2                                             Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_SUBSYS_SRAMECCA2_ADDR                            (0U)           /* ECC Error Address */
#define BITL_SUBSYS_SRAMECCA2_ADDR                            (32U)          /* ECC Error Address */
#define BITM_SUBSYS_SRAMECCA2_ADDR                            (0XFFFFFFFFU)  /* ECC Error Address */

/* ----------------------------------------------------------------------------------------------------
          SRAMECCD2                                             Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_SUBSYS_SRAMECCD2_DATA                            (0U)           /* ECC Error Raw Data */
#define BITL_SUBSYS_SRAMECCD2_DATA                            (32U)          /* ECC Error Raw Data */
#define BITM_SUBSYS_SRAMECCD2_DATA                            (0XFFFFFFFFU)  /* ECC Error Raw Data */

/* ----------------------------------------------------------------------------------------------------
          SRAMECCP2                                             Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_SUBSYS_SRAMECCP2_PARITY                          (0U)           /* ECC Error Raw Parity */
#define BITL_SUBSYS_SRAMECCP2_PARITY                          (7U)           /* ECC Error Raw Parity */
#define BITM_SUBSYS_SRAMECCP2_PARITY                          (0X0000007FU)  /* ECC Error Raw Parity */

#endif  /* end ifndef SUBSYS_ADDR_RDEF_H_ */


#ifndef CRC_ADDR_RDEF_H_
#define CRC_ADDR_RDEF_H_    /* CRC: CRC Accelerator */

/* ====================================================================================================
        CRC Module Instances Address and Mask Definitions
   ==================================================================================================== */
#define INST_CRC                                                 (0X40066000U)    /* crc: */

#define MASK_CRC                                                 (0X000000FFU)    /* CRC: CRC Accelerator */

/* ====================================================================================================
        CRC Module Register Address Offset Definitions
   ==================================================================================================== */
#define IDX_CRC_CTL                                             (0X00U)    /* CRC Control Register */
#define IDX_CRC_IPDATA                                          (0X04U)    /* Input Data Word Register */
#define IDX_CRC_RESULT                                          (0X08U)    /* CRC Result Register */
#define IDX_CRC_POLY                                            (0X0CU)    /* Programmable CRC Polynomial */

/* ====================================================================================================
        CRC Module Register ResetValue Definitions
   ==================================================================================================== */
#define RSTVAL_CRC_CTL                                             (0X10000000) 
#define RSTVAL_CRC_IPDATA                                          (0X0) 
#define RSTVAL_CRC_RESULT                                          (0X0) 
#define RSTVAL_CRC_POLY                                            (0X4C11DB7) 

/* ====================================================================================================
        CRC Module Register BitPositions, Lengths, Masks and Enumerations Definitions
   ==================================================================================================== */

/* ----------------------------------------------------------------------------------------------------
          CTL                                                   Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_CRC_CTL_EN                                       (0U)           /* CRC Peripheral Enable */
#define BITL_CRC_CTL_EN                                       (1U)           /* CRC Peripheral Enable */
#define BITM_CRC_CTL_EN                                       (0X00000001U)  /* CRC Peripheral Enable */
#define BITP_CRC_CTL_LSBFIRST                                 (1U)           /* LSB First Calculation Order */
#define BITL_CRC_CTL_LSBFIRST                                 (1U)           /* LSB First Calculation Order */
#define BITM_CRC_CTL_LSBFIRST                                 (0X00000002U)  /* LSB First Calculation Order */
#define BITP_CRC_CTL_BITMIRR                                  (2U)           /* Bit Mirroring */
#define BITL_CRC_CTL_BITMIRR                                  (1U)           /* Bit Mirroring */
#define BITM_CRC_CTL_BITMIRR                                  (0X00000004U)  /* Bit Mirroring */
#define BITP_CRC_CTL_BYTMIRR                                  (3U)           /* Byte Mirroring */
#define BITL_CRC_CTL_BYTMIRR                                  (1U)           /* Byte Mirroring */
#define BITM_CRC_CTL_BYTMIRR                                  (0X00000008U)  /* Byte Mirroring */
#define BITP_CRC_CTL_W16SWP                                   (4U)           /* Word16 Swap */
#define BITL_CRC_CTL_W16SWP                                   (1U)           /* Word16 Swap */
#define BITM_CRC_CTL_W16SWP                                   (0X00000010U)  /* Word16 Swap */
#define BITP_CRC_CTL_REVID                                    (28U)          /* Revision ID */
#define BITL_CRC_CTL_REVID                                    (4U)           /* Revision ID */
#define BITM_CRC_CTL_REVID                                    (0XF0000000U)  /* Revision ID */

/* ----------------------------------------------------------------------------------------------------
          IPDATA                                                Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_CRC_IPDATA_DATA_WORD                             (0U)           /* Data Input. */
#define BITL_CRC_IPDATA_DATA_WORD                             (32U)          /* Data Input. */
#define BITM_CRC_IPDATA_DATA_WORD                             (0XFFFFFFFFU)  /* Data Input. */

/* ----------------------------------------------------------------------------------------------------
          RESULT                                                Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_CRC_RESULT_RESIDUE                               (0U)           /* CRC Residue */
#define BITL_CRC_RESULT_RESIDUE                               (32U)          /* CRC Residue */
#define BITM_CRC_RESULT_RESIDUE                               (0XFFFFFFFFU)  /* CRC Residue */

/* ----------------------------------------------------------------------------------------------------
          POLY                                                  Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_CRC_POLY_REDUCTION_POLY                          (0U)           /* CRC Reduction Polynomial */
#define BITL_CRC_POLY_REDUCTION_POLY                          (32U)          /* CRC Reduction Polynomial */
#define BITM_CRC_POLY_REDUCTION_POLY                          (0XFFFFFFFFU)  /* CRC Reduction Polynomial */

#endif  /* end ifndef CRC_ADDR_RDEF_H_ */


#ifndef ADC_ADDR_RDEF_H_
#define ADC_ADDR_RDEF_H_    /* ADC: Your module description, here. */

/* ====================================================================================================
        ADC Module Instances Address and Mask Definitions
   ==================================================================================================== */
#define INST_ADC                                                 (0X40068000U)    /* adc: */

#define MASK_ADC                                                 (0X000007FFU)    /* ADC: Your module description, here. */

/* ====================================================================================================
        ADC Module Register Address Offset Definitions
   ==================================================================================================== */
#define IDX_ADC_ADCDAT0                                         (0X000U)    /* ADCx Data and Flags */
#define IDX_ADC_ADCDAT1                                         (0X004U)    /* ADCx Data and Flags */
#define IDX_ADC_ADCDAT2                                         (0X008U)    /* ADCx Data and Flags */
#define IDX_ADC_ADCDAT3                                         (0X00CU)    /* ADCx Data and Flags */
#define IDX_ADC_ADCDAT4                                         (0X010U)    /* ADCx Data and Flags */
#define IDX_ADC_ADCDAT5                                         (0X014U)    /* ADCx Data and Flags */
#define IDX_ADC_ADCDAT6                                         (0X018U)    /* ADCx Data and Flags */
#define IDX_ADC_ADCDAT7                                         (0X01CU)    /* ADCx Data and Flags */
#define IDX_ADC_ADCDAT8                                         (0X020U)    /* ADCx Data and Flags */
#define IDX_ADC_ADCDAT9                                         (0X024U)    /* ADCx Data and Flags */
#define IDX_ADC_ADCDAT10                                        (0X028U)    /* ADCx Data and Flags */
#define IDX_ADC_ADCDAT11                                        (0X02CU)    /* ADCx Data and Flags */
#define IDX_ADC_ADCDAT12                                        (0X030U)    /* ADCx Data and Flags */
#define IDX_ADC_ADCDAT13                                        (0X034U)    /* ADCx Data and Flags */
#define IDX_ADC_ADCDAT14                                        (0X038U)    /* ADCx Data and Flags */
#define IDX_ADC_ADCDAT15                                        (0X03CU)    /* ADCx Data and Flags */
#define IDX_ADC_ADCDAT16                                        (0X040U)    /* ADCx Data and Flags */
#define IDX_ADC_ADCDAT17                                        (0X044U)    /* ADCx Data and Flags */
#define IDX_ADC_ADCDAT18                                        (0X048U)    /* ADCx Data and Flags */
#define IDX_ADC_ADCDAT19                                        (0X04CU)    /* ADCx Data and Flags */
#define IDX_ADC_ADCDAT20                                        (0X050U)    /* ADCx Data and Flags */
#define IDX_ADC_ADCDAT21                                        (0X054U)    /* ADCx Data and Flags */
#define IDX_ADC_ADCDAT22                                        (0X058U)    /* ADCx Data and Flags */
#define IDX_ADC_ADCDAT23                                        (0X05CU)    /* ADCx Data and Flags */
#define IDX_ADC_ADCDAT24                                        (0X060U)    /* ADCx Data and Flags */
#define IDX_ADC_ADCDAT25                                        (0X064U)    /* ADCx Data and Flags */
#define IDX_ADC_ADCDAT26                                        (0X068U)    /* ADCx Data and Flags */
#define IDX_ADC_ADCDAT27                                        (0X06CU)    /* ADCx Data and Flags */
#define IDX_ADC_ADCDAT28                                        (0X070U)    /* ADCx Data and Flags */
#define IDX_ADC_ADCDAT29                                        (0X074U)    /* ADCx Data and Flags */
#define IDX_ADC_ADCCON                                          (0X078U)    /* ADC Configuration */
#define IDX_ADC_PREBUFCON                                       (0X07CU)    /* Pre-charge Buffer Control */
#define IDX_ADC_ADCCNVC                                         (0X080U)    /* ADC Conversion Cycle for Positive Input Channels */
#define IDX_ADC_ADCCNVCSLOW                                     (0X084U)    /* ADC Conversion Cycle for Positive Input Channels */
#define IDX_ADC_ADCCHA                                          (0X088U)    /* ADC Channel Select */
#define IDX_ADC_ADCIRQSTAT                                      (0X08CU)    /* ADC Interrupt Status */
#define IDX_ADC_ADCSEQ                                          (0X090U)    /* ADC Sequencer Control */
#define IDX_ADC_ADCSEQC                                         (0X094U)    /* ADC Sequencer Configuration */
#define IDX_ADC_ADCSEQS                                         (0X098U)    /* ADC Sequencer Status */
#define IDX_ADC_ADCSEQCH                                        (0X09CU)    /* ADC Sequencer Channel 0 */
#define IDX_ADC_ADCSEQCHMUX0                                    (0X0A0U)    /* ADC Sequencer Channel 1 */
#define IDX_ADC_ADCSEQCHMUX1                                    (0X0A4U)    /* ADC Sequencer Channel 1 */
#define IDX_ADC_ADCCMP                                          (0X0A8U)    /* Digital Comparator 0 Configuration */
#define IDX_ADC_ADCCMPIRQSTAT                                   (0X0ACU)    /* Digital Comparator Interrupt Status */
#define IDX_ADC_ADCOFGNDIFF                                     (0X0B0U)    /* ADC Offset Gain Differential Channel Error Correction */
#define IDX_ADC_ADCOFTEMP                                       (0X0B4U)    /* ADC Offset Gain Temp Sensor Channel Error Correction */
#define IDX_ADC_ADCGNTEMP                                       (0X0B8U)    /* ADC Offset Gain Temp Sensor Channel Error Correction */
#define IDX_ADC_ADCOFGNPGA0                                     (0X0BCU)    /* ADC Offset Gain PGA0 Channel Error Correction */
#define IDX_ADC_ADCOFGNPGA1                                     (0X0C0U)    /* ADC Offset Gain PGA1 Channel Error Correction */
#define IDX_ADC_ADCOFGNPGA2                                     (0X0C4U)    /* ADC Offset Gain PGA2 Channel Error Correction */
#define IDX_ADC_ADCOFGNPGA3                                     (0X0C8U)    /* ADC Offset Gain PGA3 Channel Error Correction */
#define IDX_ADC_ADCOFGNPGA0TIA                                  (0X0CCU)    /* ADC Offset Gain PGA0 Channel Error Correction */
#define IDX_ADC_ADCOFGNPGA1TIA                                  (0X0D0U)    /* ADC Offset Gain PGA1 Channel Error Correction */
#define IDX_ADC_ADCOFGNPGA2TIA                                  (0X0D4U)    /* ADC Offset Gain PGA2 Channel Error Correction */
#define IDX_ADC_ADCOFGNPGA3TIA                                  (0X0D8U)    /* ADC Offset Gain PGA3 Channel Error Correction */
#define IDX_ADC_ADCCMP1                                         (0X154U)    /* Digital Comparator 1 Configuration */
#define IDX_ADC_ADCCMP2                                         (0X158U)    /* Digital Comparator 2 Configuration */
#define IDX_ADC_ADCCMP3                                         (0X15CU)    /* Digital Comparator 3 Configuration */

/* ====================================================================================================
        ADC Module Register ResetValue Definitions
   ==================================================================================================== */
#define RSTVAL_ADC_ADCDAT0                                         (0X0) 
#define RSTVAL_ADC_ADCDAT1                                         (0X0) 
#define RSTVAL_ADC_ADCDAT2                                         (0X0) 
#define RSTVAL_ADC_ADCDAT3                                         (0X0) 
#define RSTVAL_ADC_ADCDAT4                                         (0X0) 
#define RSTVAL_ADC_ADCDAT5                                         (0X0) 
#define RSTVAL_ADC_ADCDAT6                                         (0X0) 
#define RSTVAL_ADC_ADCDAT7                                         (0X0) 
#define RSTVAL_ADC_ADCDAT8                                         (0X0) 
#define RSTVAL_ADC_ADCDAT9                                         (0X0) 
#define RSTVAL_ADC_ADCDAT10                                        (0X0) 
#define RSTVAL_ADC_ADCDAT11                                        (0X0) 
#define RSTVAL_ADC_ADCDAT12                                        (0X0) 
#define RSTVAL_ADC_ADCDAT13                                        (0X0) 
#define RSTVAL_ADC_ADCDAT14                                        (0X0) 
#define RSTVAL_ADC_ADCDAT15                                        (0X0) 
#define RSTVAL_ADC_ADCDAT16                                        (0X0) 
#define RSTVAL_ADC_ADCDAT17                                        (0X0) 
#define RSTVAL_ADC_ADCDAT18                                        (0X0) 
#define RSTVAL_ADC_ADCDAT19                                        (0X0) 
#define RSTVAL_ADC_ADCDAT20                                        (0X0) 
#define RSTVAL_ADC_ADCDAT21                                        (0X0) 
#define RSTVAL_ADC_ADCDAT22                                        (0X0) 
#define RSTVAL_ADC_ADCDAT23                                        (0X0) 
#define RSTVAL_ADC_ADCDAT24                                        (0X0) 
#define RSTVAL_ADC_ADCDAT25                                        (0X0) 
#define RSTVAL_ADC_ADCDAT26                                        (0X0) 
#define RSTVAL_ADC_ADCDAT27                                        (0X0) 
#define RSTVAL_ADC_ADCDAT28                                        (0X0) 
#define RSTVAL_ADC_ADCDAT29                                        (0X0) 
#define RSTVAL_ADC_ADCCON                                          (0X200) 
#define RSTVAL_ADC_PREBUFCON                                       (0X3) 
#define RSTVAL_ADC_ADCCNVC                                         (0X10) 
#define RSTVAL_ADC_ADCCNVCSLOW                                     (0X140) 
#define RSTVAL_ADC_ADCCHA                                          (0X0) 
#define RSTVAL_ADC_ADCIRQSTAT                                      (0X0) 
#define RSTVAL_ADC_ADCSEQ                                          (0X0) 
#define RSTVAL_ADC_ADCSEQC                                         (0X0) 
#define RSTVAL_ADC_ADCSEQS                                         (0X0) 
#define RSTVAL_ADC_ADCSEQCH                                        (0X0) 
#define RSTVAL_ADC_ADCSEQCHMUX0                                    (0X0) 
#define RSTVAL_ADC_ADCSEQCHMUX1                                    (0X0) 
#define RSTVAL_ADC_ADCCMP                                          (0X40000) 
#define RSTVAL_ADC_ADCCMPIRQSTAT                                   (0X0) 
#define RSTVAL_ADC_ADCOFGNDIFF                                     (0X4000) 
#define RSTVAL_ADC_ADCOFTEMP                                       (0X0) 
#define RSTVAL_ADC_ADCGNTEMP                                       (0X80000) 
#define RSTVAL_ADC_ADCOFGNPGA0                                     (0X4000) 
#define RSTVAL_ADC_ADCOFGNPGA1                                     (0X4000) 
#define RSTVAL_ADC_ADCOFGNPGA2                                     (0X4000) 
#define RSTVAL_ADC_ADCOFGNPGA3                                     (0X4000) 
#define RSTVAL_ADC_ADCOFGNPGA0TIA                                  (0X4000) 
#define RSTVAL_ADC_ADCOFGNPGA1TIA                                  (0X4000) 
#define RSTVAL_ADC_ADCOFGNPGA2TIA                                  (0X4000) 
#define RSTVAL_ADC_ADCOFGNPGA3TIA                                  (0X4000) 
#define RSTVAL_ADC_ADCCMP1                                         (0X40000) 
#define RSTVAL_ADC_ADCCMP2                                         (0X40000) 
#define RSTVAL_ADC_ADCCMP3                                         (0X40000) 

/* ====================================================================================================
        ADC Module Register BitPositions, Lengths, Masks and Enumerations Definitions
   ==================================================================================================== */

/* ----------------------------------------------------------------------------------------------------
          ADCDAT0                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ADC_ADCDAT_N__OVF                                (0U)           /* Overflow Flag */
#define BITL_ADC_ADCDAT_N__OVF                                (1U)           /* Overflow Flag */
#define BITM_ADC_ADCDAT_N__OVF                                (0X00000001U)  /* Overflow Flag */
#define BITP_ADC_ADCDAT_N__UVF                                (1U)           /* Underflow Flag */
#define BITL_ADC_ADCDAT_N__UVF                                (1U)           /* Underflow Flag */
#define BITM_ADC_ADCDAT_N__UVF                                (0X00000002U)  /* Underflow Flag */
#define BITP_ADC_ADCDAT_N__RDY                                (2U)           /* Data Read Flag */
#define BITL_ADC_ADCDAT_N__RDY                                (1U)           /* Data Read Flag */
#define BITM_ADC_ADCDAT_N__RDY                                (0X00000004U)  /* Data Read Flag */
#define BITP_ADC_ADCDAT_N__DAT                                (4U)           /* ADCx Data */
#define BITL_ADC_ADCDAT_N__DAT                                (16U)          /* ADCx Data */
#define BITM_ADC_ADCDAT_N__DAT                                (0X000FFFF0U)  /* ADCx Data */

#define ENUM_ADC_ADCDAT_N__RDY_NORDY                          (0X00000000U)  /* Data is Not Ready or Has Been Read Out */
#define ENUM_ADC_ADCDAT_N__RDY_RDY                            (0X00000001U)  /* Data is Ready to Be Read */
#define ENUM_ADC_ADCDAT_N__UVF_NUNF                           (0X00000000U)  /* Not Underflow */
#define ENUM_ADC_ADCDAT_N__UVF_UNF                            (0X00000001U)  /* Underflow */
#define ENUM_ADC_ADCDAT_N__OVF_NOVF                           (0X00000000U)  /* Not Overflow */
#define ENUM_ADC_ADCDAT_N__OVF_OVF                            (0X00000001U)  /* Overflow */

/* ----------------------------------------------------------------------------------------------------
          ADCCON                                                Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ADC_ADCCON_CONVTYPE                              (0U)           /* ADC Conversion Type Selection */
#define BITL_ADC_ADCCON_CONVTYPE                              (3U)           /* ADC Conversion Type Selection */
#define BITM_ADC_ADCCON_CONVTYPE                              (0X00000007U)  /* ADC Conversion Type Selection */
#define BITP_ADC_ADCCON_CNVDMA                                (3U)           /* DMA Request Enable for ADC Non-sequence Conversion */
#define BITL_ADC_ADCCON_CNVDMA                                (1U)           /* DMA Request Enable for ADC Non-sequence Conversion */
#define BITM_ADC_ADCCON_CNVDMA                                (0X00000008U)  /* DMA Request Enable for ADC Non-sequence Conversion */
#define BITP_ADC_ADCCON_SEQDMA                                (4U)           /* DMA Request Enable for ADC Sequence Conversion */
#define BITL_ADC_ADCCON_SEQDMA                                (1U)           /* DMA Request Enable for ADC Sequence Conversion */
#define BITM_ADC_ADCCON_SEQDMA                                (0X00000010U)  /* DMA Request Enable for ADC Sequence Conversion */
#define BITP_ADC_ADCCON_PINMOD                                (5U)           /* PIN Conversion Mode Selection */
#define BITL_ADC_ADCCON_PINMOD                                (1U)           /* PIN Conversion Mode Selection */
#define BITM_ADC_ADCCON_PINMOD                                (0X00000020U)  /* PIN Conversion Mode Selection */
#define BITP_ADC_ADCCON_RESTARTADC                            (6U)           /* Restart ADC, Reset Analog Part of ADC */
#define BITL_ADC_ADCCON_RESTARTADC                            (1U)           /* Restart ADC, Reset Analog Part of ADC */
#define BITM_ADC_ADCCON_RESTARTADC                            (0X00000040U)  /* Restart ADC, Reset Analog Part of ADC */
#define BITP_ADC_ADCCON_PDREFBUF                              (7U)           /* ADC Refbuf Power Down */
#define BITL_ADC_ADCCON_PDREFBUF                              (1U)           /* ADC Refbuf Power Down */
#define BITM_ADC_ADCCON_PDREFBUF                              (0X00000080U)  /* ADC Refbuf Power Down */
#define BITP_ADC_ADCCON_VDDSEL                                (8U)           /* Select Whether Channel 29 is DVDD Channel or AVDD Channel */
#define BITL_ADC_ADCCON_VDDSEL                                (1U)           /* Select Whether Channel 29 is DVDD Channel or AVDD Channel */
#define BITM_ADC_ADCCON_VDDSEL                                (0X00000100U)  /* Select Whether Channel 29 is DVDD Channel or AVDD Channel */
#define BITP_ADC_ADCCON_PDADC                                 (9U)           /* ADC Power Down */
#define BITL_ADC_ADCCON_PDADC                                 (1U)           /* ADC Power Down */
#define BITM_ADC_ADCCON_PDADC                                 (0X00000200U)  /* ADC Power Down */
#define BITP_ADC_ADCCON_OSR                                   (10U)          /* Oversampling Ratio */
#define BITL_ADC_ADCCON_OSR                                   (3U)           /* Oversampling Ratio */
#define BITM_ADC_ADCCON_OSR                                   (0X00001C00U)  /* Oversampling Ratio */
#define BITP_ADC_ADCCON_CNVIRQEN                              (14U)          /* Enable Conversion Interrupt Generation */
#define BITL_ADC_ADCCON_CNVIRQEN                              (1U)           /* Enable Conversion Interrupt Generation */
#define BITM_ADC_ADCCON_CNVIRQEN                              (0X00004000U)  /* Enable Conversion Interrupt Generation */
#define BITP_ADC_ADCCON_GPTEVENTEN                            (15U)          /* Enable GPT Event to Trigger Conversion */
#define BITL_ADC_ADCCON_GPTEVENTEN                            (5U)           /* Enable GPT Event to Trigger Conversion */
#define BITM_ADC_ADCCON_GPTEVENTEN                            (0X000F8000U)  /* Enable GPT Event to Trigger Conversion */
#define BITP_ADC_ADCCON_GPTTRIGMD                             (20U)          /* Mux Select GP Timer Trigger Mode */
#define BITL_ADC_ADCCON_GPTTRIGMD                             (1U)           /* Mux Select GP Timer Trigger Mode */
#define BITM_ADC_ADCCON_GPTTRIGMD                             (0X00100000U)  /* Mux Select GP Timer Trigger Mode */

#define ENUM_ADC_ADCCON_GPTTRIGMD_MD0                         (0X00000000U)  /* Two Timeout Events Trigger One Conversion */
#define ENUM_ADC_ADCCON_GPTTRIGMD_MD1                         (0X00000001U)  /* One Timeout Events Trigger One Conversion */
#define ENUM_ADC_ADCCON_OSR_OSR_NS1                           (0X00000000U)  /* Oversampling Disable */
#define ENUM_ADC_ADCCON_OSR_OSR2                              (0X00000001U)  /* Oversampling X2 */
#define ENUM_ADC_ADCCON_OSR_OSR4                              (0X00000002U)  /* Oversampling X4 */
#define ENUM_ADC_ADCCON_OSR_OSR8                              (0X00000003U)  /* Oversampling X8 */
#define ENUM_ADC_ADCCON_OSR_OSR16                             (0X00000004U)  /* Oversampling X16 */
#define ENUM_ADC_ADCCON_OSR_OSR_NS32                          (0X00000005U)  /* Oversampling X32 */
#define ENUM_ADC_ADCCON_OSR_OSR_NS64                          (0X00000006U)  /* Oversampling Disable */
#define ENUM_ADC_ADCCON_OSR_OSR_NS128                         (0X00000007U)  /* Oversampling Disable */
#define ENUM_ADC_ADCCON_PDADC_ADCPU                           (0X00000000U)  /* Not Power Down ADC */
#define ENUM_ADC_ADCCON_PDADC_ADCPD                           (0X00000001U)  /* Power Down ADC */
#define ENUM_ADC_ADCCON_VDDSEL_DVDD                           (0X00000000U)  /* Channel 29 is Half of DVDD Channel */
#define ENUM_ADC_ADCCON_VDDSEL_AVSS                           (0X00000001U)  /* Channel 29 is AVSS Channel */
#define ENUM_ADC_ADCCON_PDREFBUF_REFBUFPU                     (0X00000000U)  /* Normal Mode */
#define ENUM_ADC_ADCCON_PDREFBUF_REFBUFPD                     (0X00000001U)  /* Power Down Reference Mode */
#define ENUM_ADC_ADCCON_RESTARTADC_ADCPU                      (0X00000000U)  /* Not Power Down ADC */
#define ENUM_ADC_ADCCON_RESTARTADC_ADCPD                      (0X00000001U)  /* Power Down ADC */
#define ENUM_ADC_ADCCON_PINMOD_PIN_LVL                        (0X00000000U)  /* CNV is Controlled by PIN Level */
#define ENUM_ADC_ADCCON_PINMOD_PIN_EDGE                       (0X00000001U)  /* CNV is Controlled by PIN Edge */
#define ENUM_ADC_ADCCON_CONVTYPE_IDLE                         (0X00000000U)  /* No Conversion */
#define ENUM_ADC_ADCCON_CONVTYPE_GPIO                         (0X00000001U)  /* ADC Controlled by GPIO Pin */
#define ENUM_ADC_ADCCON_CONVTYPE_SINGL                        (0X00000002U)  /* Software Single Conversion */
#define ENUM_ADC_ADCCON_CONVTYPE_CONT                         (0X00000003U)  /* Software Continue Conversion */
#define ENUM_ADC_ADCCON_CONVTYPE_PLA                          (0X00000004U)  /* PLA Conversion */
#define ENUM_ADC_ADCCON_CONVTYPE_GPT                          (0X00000005U)  /* GPT Triggered Conversion */

/* ----------------------------------------------------------------------------------------------------
          PREBUFCON                                             Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ADC_PREBUFCON_PRGBYPP                            (0U)           /* Bypass P Channel Pre_buf */
#define BITL_ADC_PREBUFCON_PRGBYPP                            (1U)           /* Bypass P Channel Pre_buf */
#define BITM_ADC_PREBUFCON_PRGBYPP                            (0X00000001U)  /* Bypass P Channel Pre_buf */
#define BITP_ADC_PREBUFCON_PRGBYPN                            (1U)           /* Bypass N Channel Pre_buf */
#define BITL_ADC_PREBUFCON_PRGBYPN                            (1U)           /* Bypass N Channel Pre_buf */
#define BITM_ADC_PREBUFCON_PRGBYPN                            (0X00000002U)  /* Bypass N Channel Pre_buf */

/* ----------------------------------------------------------------------------------------------------
          ADCCNVC                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ADC_ADCCNVC_CNVC                                 (0U)           /* CNV Frequency Configuration for Positive Channels */
#define BITL_ADC_ADCCNVC_CNVC                                 (32U)          /* CNV Frequency Configuration for Positive Channels */
#define BITM_ADC_ADCCNVC_CNVC                                 (0XFFFFFFFFU)  /* CNV Frequency Configuration for Positive Channels */

/* ----------------------------------------------------------------------------------------------------
          ADCCNVCSLOW                                           Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ADC_ADCCNVCSLOW_CNVCSLOW                         (0U)           /* CNV Frequency Configuration for 100KSPS Channels */
#define BITL_ADC_ADCCNVCSLOW_CNVCSLOW                         (32U)          /* CNV Frequency Configuration for 100KSPS Channels */
#define BITM_ADC_ADCCNVCSLOW_CNVCSLOW                         (0XFFFFFFFFU)  /* CNV Frequency Configuration for 100KSPS Channels */

/* ----------------------------------------------------------------------------------------------------
          ADCCHA                                                Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ADC_ADCCHA_ADCCP                                 (0U)           /* ADC P Channel Selection */
#define BITL_ADC_ADCCHA_ADCCP                                 (5U)           /* ADC P Channel Selection */
#define BITM_ADC_ADCCHA_ADCCP                                 (0X0000001FU)  /* ADC P Channel Selection */
#define BITP_ADC_ADCCHA_ADCCN                                 (5U)           /* ADC N Channel Selection */
#define BITL_ADC_ADCCHA_ADCCN                                 (4U)           /* ADC N Channel Selection */
#define BITM_ADC_ADCCHA_ADCCN                                 (0X000001E0U)  /* ADC N Channel Selection */

#define ENUM_ADC_ADCCHA_ADCCP_AIN0                            (0X00000000U)  /* No description provided */
#define ENUM_ADC_ADCCHA_ADCCP_AIN1                            (0X00000001U)  /* No description provided */
#define ENUM_ADC_ADCCHA_ADCCP_AIN2                            (0X00000002U)  /* No description provided */
#define ENUM_ADC_ADCCHA_ADCCP_AIN3                            (0X00000003U)  /* No description provided */
#define ENUM_ADC_ADCCHA_ADCCP_AIN4                            (0X00000004U)  /* No description provided */
#define ENUM_ADC_ADCCHA_ADCCP_AIN5                            (0X00000005U)  /* No description provided */
#define ENUM_ADC_ADCCHA_ADCCP_AIN6                            (0X00000006U)  /* No description provided */
#define ENUM_ADC_ADCCHA_ADCCP_AIN7                            (0X00000007U)  /* No description provided */
#define ENUM_ADC_ADCCHA_ADCCP_AIN8                            (0X00000008U)  /* No description provided */
#define ENUM_ADC_ADCCHA_ADCCP_AIN9                            (0X00000009U)  /* No description provided */
#define ENUM_ADC_ADCCHA_ADCCP_AIN10                           (0X0000000AU)  /* No description provided */
#define ENUM_ADC_ADCCHA_ADCCP_AIN11                           (0X0000000BU)  /* No description provided */
#define ENUM_ADC_ADCCHA_ADCCP_AIN12                           (0X0000000CU)  /* No description provided */
#define ENUM_ADC_ADCCHA_ADCCP_AIN13                           (0X0000000DU)  /* No description provided */
#define ENUM_ADC_ADCCHA_ADCCP_AIN14                           (0X0000000EU)  /* No description provided */
#define ENUM_ADC_ADCCHA_ADCCP_AIN15                           (0X0000000FU)  /* No description provided */
#define ENUM_ADC_ADCCHA_ADCCP_PGA0                            (0X00000010U)  /* No description provided */
#define ENUM_ADC_ADCCHA_ADCCP_PGA1                            (0X00000011U)  /* No description provided */
#define ENUM_ADC_ADCCHA_ADCCP_PGA2                            (0X00000012U)  /* No description provided */
#define ENUM_ADC_ADCCHA_ADCCP_PGA3                            (0X00000013U)  /* No description provided */
#define ENUM_ADC_ADCCHA_ADCCP_TEMPSNS                         (0X00000014U)  /* No description provided */
#define ENUM_ADC_ADCCHA_ADCCP_AVDDDIV2                        (0X00000015U)  /* No description provided */
#define ENUM_ADC_ADCCHA_ADCCP_IOVDD0DIV2                      (0X00000016U)  /* No description provided */
#define ENUM_ADC_ADCCHA_ADCCP_IOVDD1                          (0X00000017U)  /* No description provided */
#define ENUM_ADC_ADCCHA_ADCCP_HVTIA0                          (0X00000018U)  /* No description provided */
#define ENUM_ADC_ADCCHA_ADCCP_HVTIA1                          (0X00000019U)  /* No description provided */
#define ENUM_ADC_ADCCHA_ADCCP_HVTIA2                          (0X0000001AU)  /* No description provided */
#define ENUM_ADC_ADCCHA_ADCCP_HVTIA3                          (0X0000001BU)  /* No description provided */
#define ENUM_ADC_ADCCHA_ADCCP_HVTIA4                          (0X0000001CU)  /* No description provided */
#define ENUM_ADC_ADCCHA_ADCCP_AVSS                            (0X0000001DU)  /* No description provided */

/* ----------------------------------------------------------------------------------------------------
          ADCIRQSTAT                                            Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ADC_ADCIRQSTAT_CNVIRQSTAT                        (0U)           /* Single Conversion IRQ Status */
#define BITL_ADC_ADCIRQSTAT_CNVIRQSTAT                        (1U)           /* Single Conversion IRQ Status */
#define BITM_ADC_ADCIRQSTAT_CNVIRQSTAT                        (0X00000001U)  /* Single Conversion IRQ Status */
#define BITP_ADC_ADCIRQSTAT_SEQIRQSTAT                        (1U)           /* Sequence Conversion IRQ Status */
#define BITL_ADC_ADCIRQSTAT_SEQIRQSTAT                        (1U)           /* Sequence Conversion IRQ Status */
#define BITM_ADC_ADCIRQSTAT_SEQIRQSTAT                        (0X00000002U)  /* Sequence Conversion IRQ Status */

#define ENUM_ADC_ADCIRQSTAT_SEQIRQSTAT_IRQCLR                 (0X00000000U)  /* Interrupt Clear */
#define ENUM_ADC_ADCIRQSTAT_SEQIRQSTAT_IRQSET                 (0X00000001U)  /* Interrupt Set */
#define ENUM_ADC_ADCIRQSTAT_CNVIRQSTAT_IRQCLR                 (0X00000000U)  /* Interrupt Clear */
#define ENUM_ADC_ADCIRQSTAT_CNVIRQSTAT_IRQSET                 (0X00000001U)  /* Interrupt Set */

/* ----------------------------------------------------------------------------------------------------
          ADCSEQ                                                Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ADC_ADCSEQ_SEQEN                                 (0U)           /* Sequence Enable */
#define BITL_ADC_ADCSEQ_SEQEN                                 (1U)           /* Sequence Enable */
#define BITM_ADC_ADCSEQ_SEQEN                                 (0X00000001U)  /* Sequence Enable */
#define BITP_ADC_ADCSEQ_SEQREN                                (1U)           /* Sequence Restart */
#define BITL_ADC_ADCSEQ_SEQREN                                (1U)           /* Sequence Restart */
#define BITM_ADC_ADCSEQ_SEQREN                                (0X00000002U)  /* Sequence Restart */
#define BITP_ADC_ADCSEQ_SEQSTL                                (2U)           /* Sequence Stall */
#define BITL_ADC_ADCSEQ_SEQSTL                                (1U)           /* Sequence Stall */
#define BITM_ADC_ADCSEQ_SEQSTL                                (0X00000004U)  /* Sequence Stall */
#define BITP_ADC_ADCSEQ_SEQIRQEN                              (3U)           /* Enable Sequencer Interrupt Generation */
#define BITL_ADC_ADCSEQ_SEQIRQEN                              (1U)           /* Enable Sequencer Interrupt Generation */
#define BITM_ADC_ADCSEQ_SEQIRQEN                              (0X00000008U)  /* Enable Sequencer Interrupt Generation */

#define ENUM_ADC_ADCSEQ_SEQSTL_SEQNSTALL                      (0X00000000U)  /* Running Sequence */
#define ENUM_ADC_ADCSEQ_SEQSTL_SEQSTALL                       (0X00000001U)  /* Stalling Sequence */

/* ----------------------------------------------------------------------------------------------------
          ADCSEQC                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ADC_ADCSEQC_SEQT                                 (0U)           /* Repeat Sequence Interval */
#define BITL_ADC_ADCSEQC_SEQT                                 (8U)           /* Repeat Sequence Interval */
#define BITM_ADC_ADCSEQC_SEQT                                 (0X000000FFU)  /* Repeat Sequence Interval */

/* ----------------------------------------------------------------------------------------------------
          ADCSEQS                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ADC_ADCSEQS_CNVSTAT                              (0U)           /* ADC Conversion Idle/Busy Flag */
#define BITL_ADC_ADCSEQS_CNVSTAT                              (1U)           /* ADC Conversion Idle/Busy Flag */
#define BITM_ADC_ADCSEQS_CNVSTAT                              (0X00000001U)  /* ADC Conversion Idle/Busy Flag */
#define BITP_ADC_ADCSEQS_SEQSTLSTAT                           (1U)           /* Stall Sequencer Status */
#define BITL_ADC_ADCSEQS_SEQSTLSTAT                           (1U)           /* Stall Sequencer Status */
#define BITM_ADC_ADCSEQS_SEQSTLSTAT                           (0X00000002U)  /* Stall Sequencer Status */
#define BITP_ADC_ADCSEQS_SEQSTAT                              (2U)           /* Sequencer Status */
#define BITL_ADC_ADCSEQS_SEQSTAT                              (1U)           /* Sequencer Status */
#define BITM_ADC_ADCSEQS_SEQSTAT                              (0X00000004U)  /* Sequencer Status */

#define ENUM_ADC_ADCSEQS_SEQSTAT_SEQBUSY                      (0X00000001U)  /* Sequence is Busy */
#define ENUM_ADC_ADCSEQS_SEQSTAT_SEQNBUSY                     (0X00000000U)  /* Sequence is Idle */
#define ENUM_ADC_ADCSEQS_SEQSTLSTAT_SEQNSTOP                  (0X00000000U)  /* Sequence Still Run */
#define ENUM_ADC_ADCSEQS_SEQSTLSTAT_SEQSTOP                   (0X00000001U)  /* Sequence Has Stalled */
#define ENUM_ADC_ADCSEQS_CNVSTAT_CNVIDLE                      (0X00000000U)  /* ADC Conversion is Idle */
#define ENUM_ADC_ADCSEQS_CNVSTAT_CNVBUSY                      (0X00000001U)  /* ADC Conversion is Busy */

/* ----------------------------------------------------------------------------------------------------
          ADCSEQCH                                              Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ADC_ADCSEQCH_SEQCH                               (0U)           /* Sequence Channel Selection */
#define BITL_ADC_ADCSEQCH_SEQCH                               (30U)          /* Sequence Channel Selection */
#define BITM_ADC_ADCSEQCH_SEQCH                               (0X3FFFFFFFU)  /* Sequence Channel Selection */

/* ----------------------------------------------------------------------------------------------------
          ADCSEQCHMUX0                                          Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ADC_ADCSEQCHMUX0_DIF0                            (0U)           /* When AIN0 is P Ch. N Ch. Mux Selection in Sequencer Mode */
#define BITL_ADC_ADCSEQCHMUX0_DIF0                            (4U)           /* When AIN0 is P Ch. N Ch. Mux Selection in Sequencer Mode */
#define BITM_ADC_ADCSEQCHMUX0_DIF0                            (0X0000000FU)  /* When AIN0 is P Ch. N Ch. Mux Selection in Sequencer Mode */
#define BITP_ADC_ADCSEQCHMUX0_DIF1                            (4U)           /* When AIN1 is N Ch. P Ch. Mux Selection in Sequencer Mode */
#define BITL_ADC_ADCSEQCHMUX0_DIF1                            (1U)           /* When AIN1 is N Ch. P Ch. Mux Selection in Sequencer Mode */
#define BITM_ADC_ADCSEQCHMUX0_DIF1                            (0X00000010U)  /* When AIN1 is N Ch. P Ch. Mux Selection in Sequencer Mode */
#define BITP_ADC_ADCSEQCHMUX0_DIF2                            (5U)           /* When AIN2 is P Ch. N Ch. Mux Selection in Sequencer Mode */
#define BITL_ADC_ADCSEQCHMUX0_DIF2                            (4U)           /* When AIN2 is P Ch. N Ch. Mux Selection in Sequencer Mode */
#define BITM_ADC_ADCSEQCHMUX0_DIF2                            (0X000001E0U)  /* When AIN2 is P Ch. N Ch. Mux Selection in Sequencer Mode */
#define BITP_ADC_ADCSEQCHMUX0_DIF3                            (9U)           /* When AIN3 is N Ch. P Ch. Mux Selection in Sequencer Mode */
#define BITL_ADC_ADCSEQCHMUX0_DIF3                            (1U)           /* When AIN3 is N Ch. P Ch. Mux Selection in Sequencer Mode */
#define BITM_ADC_ADCSEQCHMUX0_DIF3                            (0X00000200U)  /* When AIN3 is N Ch. P Ch. Mux Selection in Sequencer Mode */
#define BITP_ADC_ADCSEQCHMUX0_DIF4                            (10U)          /* When AIN4 is P Ch. N Ch. Mux Selection in Sequencer Mode */
#define BITL_ADC_ADCSEQCHMUX0_DIF4                            (4U)           /* When AIN4 is P Ch. N Ch. Mux Selection in Sequencer Mode */
#define BITM_ADC_ADCSEQCHMUX0_DIF4                            (0X00003C00U)  /* When AIN4 is P Ch. N Ch. Mux Selection in Sequencer Mode */
#define BITP_ADC_ADCSEQCHMUX0_DIF5                            (14U)          /* When AIN5 is N Ch. P Ch. Mux Selection in Sequencer Mode */
#define BITL_ADC_ADCSEQCHMUX0_DIF5                            (1U)           /* When AIN5 is N Ch. P Ch. Mux Selection in Sequencer Mode */
#define BITM_ADC_ADCSEQCHMUX0_DIF5                            (0X00004000U)  /* When AIN5 is N Ch. P Ch. Mux Selection in Sequencer Mode */
#define BITP_ADC_ADCSEQCHMUX0_DIF6                            (15U)          /* When AIN6 is P Ch. N Ch. Mux Selection in Sequencer Mode */
#define BITL_ADC_ADCSEQCHMUX0_DIF6                            (4U)           /* When AIN6 is P Ch. N Ch. Mux Selection in Sequencer Mode */
#define BITM_ADC_ADCSEQCHMUX0_DIF6                            (0X00078000U)  /* When AIN6 is P Ch. N Ch. Mux Selection in Sequencer Mode */
#define BITP_ADC_ADCSEQCHMUX0_DIF7                            (19U)          /* When AIN7 is N Ch. P Ch. Mux Selection in Sequencer Mode */
#define BITL_ADC_ADCSEQCHMUX0_DIF7                            (1U)           /* When AIN7 is N Ch. P Ch. Mux Selection in Sequencer Mode */
#define BITM_ADC_ADCSEQCHMUX0_DIF7                            (0X00080000U)  /* When AIN7 is N Ch. P Ch. Mux Selection in Sequencer Mode */
#define BITP_ADC_ADCSEQCHMUX0_DIF8                            (20U)          /* When AIN8 is P Ch. N Ch. Mux Selection in Sequencer Mode */
#define BITL_ADC_ADCSEQCHMUX0_DIF8                            (4U)           /* When AIN8 is P Ch. N Ch. Mux Selection in Sequencer Mode */
#define BITM_ADC_ADCSEQCHMUX0_DIF8                            (0X00F00000U)  /* When AIN8 is P Ch. N Ch. Mux Selection in Sequencer Mode */
#define BITP_ADC_ADCSEQCHMUX0_DIF9                            (24U)          /* When AIN9 is N Ch. P Ch. Mux Selection in Sequencer Mode */
#define BITL_ADC_ADCSEQCHMUX0_DIF9                            (1U)           /* When AIN9 is N Ch. P Ch. Mux Selection in Sequencer Mode */
#define BITM_ADC_ADCSEQCHMUX0_DIF9                            (0X01000000U)  /* When AIN9 is N Ch. P Ch. Mux Selection in Sequencer Mode */
#define BITP_ADC_ADCSEQCHMUX0_DIF10                           (25U)          /* When AIN10 is P Ch. N Ch. Mux Selection in Sequencer Mode */
#define BITL_ADC_ADCSEQCHMUX0_DIF10                           (4U)           /* When AIN10 is P Ch. N Ch. Mux Selection in Sequencer Mode */
#define BITM_ADC_ADCSEQCHMUX0_DIF10                           (0X1E000000U)  /* When AIN10 is P Ch. N Ch. Mux Selection in Sequencer Mode */
#define BITP_ADC_ADCSEQCHMUX0_DIF11                           (29U)          /* When AIN11 is N Ch. P Ch. Mux Selection in Sequencer Mode */
#define BITL_ADC_ADCSEQCHMUX0_DIF11                           (1U)           /* When AIN11 is N Ch. P Ch. Mux Selection in Sequencer Mode */
#define BITM_ADC_ADCSEQCHMUX0_DIF11                           (0X20000000U)  /* When AIN11 is N Ch. P Ch. Mux Selection in Sequencer Mode */

#define ENUM_ADC_ADCSEQCHMUX0_DIF11_REFN                      (0X00000000U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF11_REFP                      (0X00000001U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF10_REFN                      (0X00000000U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF10_REFP                      (0X00000001U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF10_AIN1                      (0X00000002U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF10_AIN3                      (0X00000003U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF10_AIN5                      (0X00000004U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF10_AIN7                      (0X00000005U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF10_AIN9                      (0X00000006U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF10_AIN11                     (0X00000007U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF10_AIN13                     (0X00000008U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF10_AIN15                     (0X00000009U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF9_REFN                       (0X00000000U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF9_REFP                       (0X00000001U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF8_REFN                       (0X00000000U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF8_REFP                       (0X00000001U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF8_AIN1                       (0X00000002U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF8_AIN3                       (0X00000003U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF8_AIN5                       (0X00000004U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF8_AIN7                       (0X00000005U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF8_AIN9                       (0X00000006U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF8_AIN11                      (0X00000007U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF8_AIN13                      (0X00000008U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF8_AIN15                      (0X00000009U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF7_REFN                       (0X00000000U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF7_REFP                       (0X00000001U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF6_REFN                       (0X00000000U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF6_REFP                       (0X00000001U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF6_AIN1                       (0X00000002U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF6_AIN3                       (0X00000003U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF6_AIN5                       (0X00000004U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF6_AIN7                       (0X00000005U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF6_AIN9                       (0X00000006U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF6_AIN11                      (0X00000007U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF6_AIN13                      (0X00000008U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF6_AIN15                      (0X00000009U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF5_REFN                       (0X00000000U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF5_REFP                       (0X00000001U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF4_REFN                       (0X00000000U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF4_REFP                       (0X00000001U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF4_AIN1                       (0X00000002U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF4_AIN3                       (0X00000003U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF4_AIN5                       (0X00000004U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF4_AIN7                       (0X00000005U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF4_AIN9                       (0X00000006U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF4_AIN11                      (0X00000007U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF4_AIN13                      (0X00000008U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF4_AIN15                      (0X00000009U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF3_REFN                       (0X00000000U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF3_REFP                       (0X00000001U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF2_REFN                       (0X00000000U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF2_REFP                       (0X00000001U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF2_AIN1                       (0X00000002U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF2_AIN3                       (0X00000003U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF2_AIN5                       (0X00000004U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF2_AIN7                       (0X00000005U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF2_AIN9                       (0X00000006U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF2_AIN11                      (0X00000007U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF2_AIN13                      (0X00000008U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF2_AIN15                      (0X00000009U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF1_REFN                       (0X00000000U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF1_REFP                       (0X00000001U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF0_REFN                       (0X00000000U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF0_REFP                       (0X00000001U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF0_AIN1                       (0X00000002U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF0_AIN3                       (0X00000003U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF0_AIN5                       (0X00000004U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF0_AIN7                       (0X00000005U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF0_AIN9                       (0X00000006U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF0_AIN11                      (0X00000007U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF0_AIN13                      (0X00000008U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX0_DIF0_AIN15                      (0X00000009U)  /* No description provided */

/* ----------------------------------------------------------------------------------------------------
          ADCSEQCHMUX1                                          Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ADC_ADCSEQCHMUX1_DIF12                           (0U)           /* When AIN12 is P Ch. N Ch. Mux Selection in Sequencer Mode */
#define BITL_ADC_ADCSEQCHMUX1_DIF12                           (4U)           /* When AIN12 is P Ch. N Ch. Mux Selection in Sequencer Mode */
#define BITM_ADC_ADCSEQCHMUX1_DIF12                           (0X0000000FU)  /* When AIN12 is P Ch. N Ch. Mux Selection in Sequencer Mode */
#define BITP_ADC_ADCSEQCHMUX1_DIF13                           (4U)           /* When AIN13 is N Ch. P Ch. Mux Selection in Sequencer Mode */
#define BITL_ADC_ADCSEQCHMUX1_DIF13                           (1U)           /* When AIN13 is N Ch. P Ch. Mux Selection in Sequencer Mode */
#define BITM_ADC_ADCSEQCHMUX1_DIF13                           (0X00000010U)  /* When AIN13 is N Ch. P Ch. Mux Selection in Sequencer Mode */
#define BITP_ADC_ADCSEQCHMUX1_DIF14                           (5U)           /* When AIN14 is P Ch. N Ch. Mux Selection in Sequencer Mode */
#define BITL_ADC_ADCSEQCHMUX1_DIF14                           (4U)           /* When AIN14 is P Ch. N Ch. Mux Selection in Sequencer Mode */
#define BITM_ADC_ADCSEQCHMUX1_DIF14                           (0X000001E0U)  /* When AIN14 is P Ch. N Ch. Mux Selection in Sequencer Mode */
#define BITP_ADC_ADCSEQCHMUX1_DIF15                           (9U)           /* When AIN15 is N Ch. P Ch. Mux Selection in Sequencer Mode */
#define BITL_ADC_ADCSEQCHMUX1_DIF15                           (1U)           /* When AIN15 is N Ch. P Ch. Mux Selection in Sequencer Mode */
#define BITM_ADC_ADCSEQCHMUX1_DIF15                           (0X00000200U)  /* When AIN15 is N Ch. P Ch. Mux Selection in Sequencer Mode */

#define ENUM_ADC_ADCSEQCHMUX1_DIF15_REFN                      (0X00000000U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX1_DIF15_REFP                      (0X00000001U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX1_DIF14_REFN                      (0X00000000U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX1_DIF14_REFP                      (0X00000001U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX1_DIF14_AIN1                      (0X00000002U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX1_DIF14_AIN3                      (0X00000003U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX1_DIF14_AIN5                      (0X00000004U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX1_DIF14_AIN7                      (0X00000005U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX1_DIF14_AIN9                      (0X00000006U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX1_DIF14_AIN11                     (0X00000007U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX1_DIF14_AIN13                     (0X00000008U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX1_DIF14_AIN15                     (0X00000009U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX1_DIF13_REFN                      (0X00000000U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX1_DIF13_REFP                      (0X00000001U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX1_DIF12_REFN                      (0X00000000U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX1_DIF12_REFP                      (0X00000001U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX1_DIF12_AIN1                      (0X00000002U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX1_DIF12_AIN3                      (0X00000003U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX1_DIF12_AIN5                      (0X00000004U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX1_DIF12_AIN7                      (0X00000005U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX1_DIF12_AIN9                      (0X00000006U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX1_DIF12_AIN11                     (0X00000007U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX1_DIF12_AIN13                     (0X00000008U)  /* No description provided */
#define ENUM_ADC_ADCSEQCHMUX1_DIF12_AIN15                     (0X00000009U)  /* No description provided */

/* ----------------------------------------------------------------------------------------------------
          ADCCMP                                                Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ADC_ADCCMP_EN                                    (0U)           /* Digital Comparator Enable */
#define BITL_ADC_ADCCMP_EN                                    (1U)           /* Digital Comparator Enable */
#define BITM_ADC_ADCCMP_EN                                    (0X00000001U)  /* Digital Comparator Enable */
#define BITP_ADC_ADCCMP_CMPDIR                                (1U)           /* Select Digital Comparator Direction */
#define BITL_ADC_ADCCMP_CMPDIR                                (1U)           /* Select Digital Comparator Direction */
#define BITM_ADC_ADCCMP_CMPDIR                                (0X00000002U)  /* Select Digital Comparator Direction */
#define BITP_ADC_ADCCMP_THR                                   (2U)           /* Compare Threshold */
#define BITL_ADC_ADCCMP_THR                                   (16U)          /* Compare Threshold */
#define BITM_ADC_ADCCMP_THR                                   (0X0003FFFCU)  /* Compare Threshold */
#define BITP_ADC_ADCCMP_IRQEN                                 (18U)          /* Enable IRQ Generation */
#define BITL_ADC_ADCCMP_IRQEN                                 (1U)           /* Enable IRQ Generation */
#define BITM_ADC_ADCCMP_IRQEN                                 (0X00040000U)  /* Enable IRQ Generation */
#define BITP_ADC_ADCCMP_CH                                    (19U)          /* Channel Index for Data Comparison */
#define BITL_ADC_ADCCMP_CH                                    (5U)           /* Channel Index for Data Comparison */
#define BITM_ADC_ADCCMP_CH                                    (0X00F80000U)  /* Channel Index for Data Comparison */

/* ----------------------------------------------------------------------------------------------------
          ADCCMPIRQSTAT                                         Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ADC_ADCCMPIRQSTAT_COMP0IRQSTA                    (0U)           /* Comparator0 Interrupt Status */
#define BITL_ADC_ADCCMPIRQSTAT_COMP0IRQSTA                    (1U)           /* Comparator0 Interrupt Status */
#define BITM_ADC_ADCCMPIRQSTAT_COMP0IRQSTA                    (0X00000001U)  /* Comparator0 Interrupt Status */
#define BITP_ADC_ADCCMPIRQSTAT_COMP1IRQSTA                    (1U)           /* Comparator1 Interrupt Status */
#define BITL_ADC_ADCCMPIRQSTAT_COMP1IRQSTA                    (1U)           /* Comparator1 Interrupt Status */
#define BITM_ADC_ADCCMPIRQSTAT_COMP1IRQSTA                    (0X00000002U)  /* Comparator1 Interrupt Status */
#define BITP_ADC_ADCCMPIRQSTAT_COMP2IRQSTA                    (2U)           /* Comparator2 Interrupt Status */
#define BITL_ADC_ADCCMPIRQSTAT_COMP2IRQSTA                    (1U)           /* Comparator2 Interrupt Status */
#define BITM_ADC_ADCCMPIRQSTAT_COMP2IRQSTA                    (0X00000004U)  /* Comparator2 Interrupt Status */
#define BITP_ADC_ADCCMPIRQSTAT_COMP3IRQSTA                    (3U)           /* Comparator3 Interrupt Status */
#define BITL_ADC_ADCCMPIRQSTAT_COMP3IRQSTA                    (1U)           /* Comparator3 Interrupt Status */
#define BITM_ADC_ADCCMPIRQSTAT_COMP3IRQSTA                    (0X00000008U)  /* Comparator3 Interrupt Status */
#define BITP_ADC_ADCCMPIRQSTAT_COMP0IRQCLR                    (4U)           /* Comparator0 Interrupt Clear */
#define BITL_ADC_ADCCMPIRQSTAT_COMP0IRQCLR                    (1U)           /* Comparator0 Interrupt Clear */
#define BITM_ADC_ADCCMPIRQSTAT_COMP0IRQCLR                    (0X00000010U)  /* Comparator0 Interrupt Clear */
#define BITP_ADC_ADCCMPIRQSTAT_COMP1IRQCLR                    (5U)           /* Comparator1 Interrupt Clear */
#define BITL_ADC_ADCCMPIRQSTAT_COMP1IRQCLR                    (1U)           /* Comparator1 Interrupt Clear */
#define BITM_ADC_ADCCMPIRQSTAT_COMP1IRQCLR                    (0X00000020U)  /* Comparator1 Interrupt Clear */
#define BITP_ADC_ADCCMPIRQSTAT_COMP2IRQCLR                    (6U)           /* Comparator2 Interrupt Clear */
#define BITL_ADC_ADCCMPIRQSTAT_COMP2IRQCLR                    (1U)           /* Comparator2 Interrupt Clear */
#define BITM_ADC_ADCCMPIRQSTAT_COMP2IRQCLR                    (0X00000040U)  /* Comparator2 Interrupt Clear */
#define BITP_ADC_ADCCMPIRQSTAT_COMP3IRQCLR                    (7U)           /* Comparator3 Interrupt Clear */
#define BITL_ADC_ADCCMPIRQSTAT_COMP3IRQCLR                    (1U)           /* Comparator3 Interrupt Clear */
#define BITM_ADC_ADCCMPIRQSTAT_COMP3IRQCLR                    (0X00000080U)  /* Comparator3 Interrupt Clear */
#define BITP_ADC_ADCCMPIRQSTAT_COMP0PLACLR                    (8U)           /* Comparator0 to PLA Clear */
#define BITL_ADC_ADCCMPIRQSTAT_COMP0PLACLR                    (1U)           /* Comparator0 to PLA Clear */
#define BITM_ADC_ADCCMPIRQSTAT_COMP0PLACLR                    (0X00000100U)  /* Comparator0 to PLA Clear */
#define BITP_ADC_ADCCMPIRQSTAT_COMP1PLACLR                    (9U)           /* Comparator1 to PLA Clear */
#define BITL_ADC_ADCCMPIRQSTAT_COMP1PLACLR                    (1U)           /* Comparator1 to PLA Clear */
#define BITM_ADC_ADCCMPIRQSTAT_COMP1PLACLR                    (0X00000200U)  /* Comparator1 to PLA Clear */
#define BITP_ADC_ADCCMPIRQSTAT_COMP2PLACLR                    (10U)          /* Comparator2 to PLA Clear */
#define BITL_ADC_ADCCMPIRQSTAT_COMP2PLACLR                    (1U)           /* Comparator2 to PLA Clear */
#define BITM_ADC_ADCCMPIRQSTAT_COMP2PLACLR                    (0X00000400U)  /* Comparator2 to PLA Clear */
#define BITP_ADC_ADCCMPIRQSTAT_COMP3PLACLR                    (11U)          /* Comparator3 to PLA Clear */
#define BITL_ADC_ADCCMPIRQSTAT_COMP3PLACLR                    (1U)           /* Comparator3 to PLA Clear */
#define BITM_ADC_ADCCMPIRQSTAT_COMP3PLACLR                    (0X00000800U)  /* Comparator3 to PLA Clear */

/* ----------------------------------------------------------------------------------------------------
          ADCOFGNDIFF                                           Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ADC_ADCOFGNDIFF_GAIN                             (0U)           /* Gain Error Correction */
#define BITL_ADC_ADCOFGNDIFF_GAIN                             (15U)          /* Gain Error Correction */
#define BITM_ADC_ADCOFGNDIFF_GAIN                             (0X00007FFFU)  /* Gain Error Correction */
#define BITP_ADC_ADCOFGNDIFF_OFFSET                           (15U)          /* Offset Error Correction */
#define BITL_ADC_ADCOFGNDIFF_OFFSET                           (17U)          /* Offset Error Correction */
#define BITM_ADC_ADCOFGNDIFF_OFFSET                           (0XFFFF8000U)  /* Offset Error Correction */

/* ----------------------------------------------------------------------------------------------------
          ADCOFTEMP                                             Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ADC_ADCOFTEMP_OFFSET                             (0U)           /* Offset Error Correction */
#define BITL_ADC_ADCOFTEMP_OFFSET                             (17U)          /* Offset Error Correction */
#define BITM_ADC_ADCOFTEMP_OFFSET                             (0X0001FFFFU)  /* Offset Error Correction */

/* ----------------------------------------------------------------------------------------------------
          ADCGNTEMP                                             Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ADC_ADCGNTEMP_GAIN                               (0U)           /* Gain Error Correction */
#define BITL_ADC_ADCGNTEMP_GAIN                               (20U)          /* Gain Error Correction */
#define BITM_ADC_ADCGNTEMP_GAIN                               (0X000FFFFFU)  /* Gain Error Correction */

/* ----------------------------------------------------------------------------------------------------
          ADCOFGNPGA0                                           Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ADC_ADCOFGNPGA0_GAIN                             (0U)           /* Gain Error Correction */
#define BITL_ADC_ADCOFGNPGA0_GAIN                             (15U)          /* Gain Error Correction */
#define BITM_ADC_ADCOFGNPGA0_GAIN                             (0X00007FFFU)  /* Gain Error Correction */

/* ----------------------------------------------------------------------------------------------------
          ADCOFGNPGA1                                           Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ADC_ADCOFGNPGA1_GAIN                             (0U)           /* Gain Error Correction */
#define BITL_ADC_ADCOFGNPGA1_GAIN                             (15U)          /* Gain Error Correction */
#define BITM_ADC_ADCOFGNPGA1_GAIN                             (0X00007FFFU)  /* Gain Error Correction */

/* ----------------------------------------------------------------------------------------------------
          ADCOFGNPGA2                                           Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ADC_ADCOFGNPGA2_GAIN                             (0U)           /* Gain Error Correction */
#define BITL_ADC_ADCOFGNPGA2_GAIN                             (15U)          /* Gain Error Correction */
#define BITM_ADC_ADCOFGNPGA2_GAIN                             (0X00007FFFU)  /* Gain Error Correction */

/* ----------------------------------------------------------------------------------------------------
          ADCOFGNPGA3                                           Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ADC_ADCOFGNPGA3_GAIN                             (0U)           /* Gain Error Correction */
#define BITL_ADC_ADCOFGNPGA3_GAIN                             (15U)          /* Gain Error Correction */
#define BITM_ADC_ADCOFGNPGA3_GAIN                             (0X00007FFFU)  /* Gain Error Correction */

/* ----------------------------------------------------------------------------------------------------
          ADCOFGNPGA0TIA                                        Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ADC_ADCOFGNPGA0TIA_GAIN                          (0U)           /* Gain Error Correction */
#define BITL_ADC_ADCOFGNPGA0TIA_GAIN                          (15U)          /* Gain Error Correction */
#define BITM_ADC_ADCOFGNPGA0TIA_GAIN                          (0X00007FFFU)  /* Gain Error Correction */

/* ----------------------------------------------------------------------------------------------------
          ADCOFGNPGA1TIA                                        Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ADC_ADCOFGNPGA1TIA_GAIN                          (0U)           /* Gain Error Correction */
#define BITL_ADC_ADCOFGNPGA1TIA_GAIN                          (15U)          /* Gain Error Correction */
#define BITM_ADC_ADCOFGNPGA1TIA_GAIN                          (0X00007FFFU)  /* Gain Error Correction */

/* ----------------------------------------------------------------------------------------------------
          ADCOFGNPGA2TIA                                        Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ADC_ADCOFGNPGA2TIA_GAIN                          (0U)           /* Gain Error Correction */
#define BITL_ADC_ADCOFGNPGA2TIA_GAIN                          (15U)          /* Gain Error Correction */
#define BITM_ADC_ADCOFGNPGA2TIA_GAIN                          (0X00007FFFU)  /* Gain Error Correction */

/* ----------------------------------------------------------------------------------------------------
          ADCOFGNPGA3TIA                                        Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ADC_ADCOFGNPGA3TIA_GAIN                          (0U)           /* Gain Error Correction */
#define BITL_ADC_ADCOFGNPGA3TIA_GAIN                          (15U)          /* Gain Error Correction */
#define BITM_ADC_ADCOFGNPGA3TIA_GAIN                          (0X00007FFFU)  /* Gain Error Correction */

/* ----------------------------------------------------------------------------------------------------
          ADCCMP1                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ADC_ADCCMP1_EN                                   (0U)           /* Digital Comparator Enable */
#define BITL_ADC_ADCCMP1_EN                                   (1U)           /* Digital Comparator Enable */
#define BITM_ADC_ADCCMP1_EN                                   (0X00000001U)  /* Digital Comparator Enable */
#define BITP_ADC_ADCCMP1_CMPDIR                               (1U)           /* Select Digital Comparator Direction */
#define BITL_ADC_ADCCMP1_CMPDIR                               (1U)           /* Select Digital Comparator Direction */
#define BITM_ADC_ADCCMP1_CMPDIR                               (0X00000002U)  /* Select Digital Comparator Direction */
#define BITP_ADC_ADCCMP1_THR                                  (2U)           /* Compare Threshold */
#define BITL_ADC_ADCCMP1_THR                                  (16U)          /* Compare Threshold */
#define BITM_ADC_ADCCMP1_THR                                  (0X0003FFFCU)  /* Compare Threshold */
#define BITP_ADC_ADCCMP1_IRQEN                                (18U)          /* Enable IRQ Generation */
#define BITL_ADC_ADCCMP1_IRQEN                                (1U)           /* Enable IRQ Generation */
#define BITM_ADC_ADCCMP1_IRQEN                                (0X00040000U)  /* Enable IRQ Generation */
#define BITP_ADC_ADCCMP1_CH                                   (19U)          /* Channel Index for Data Comparison */
#define BITL_ADC_ADCCMP1_CH                                   (5U)           /* Channel Index for Data Comparison */
#define BITM_ADC_ADCCMP1_CH                                   (0X00F80000U)  /* Channel Index for Data Comparison */

/* ----------------------------------------------------------------------------------------------------
          ADCCMP2                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ADC_ADCCMP2_EN                                   (0U)           /* Digital Comparator Enable */
#define BITL_ADC_ADCCMP2_EN                                   (1U)           /* Digital Comparator Enable */
#define BITM_ADC_ADCCMP2_EN                                   (0X00000001U)  /* Digital Comparator Enable */
#define BITP_ADC_ADCCMP2_CMPDIR                               (1U)           /* Select Digital Comparator Direction */
#define BITL_ADC_ADCCMP2_CMPDIR                               (1U)           /* Select Digital Comparator Direction */
#define BITM_ADC_ADCCMP2_CMPDIR                               (0X00000002U)  /* Select Digital Comparator Direction */
#define BITP_ADC_ADCCMP2_THR                                  (2U)           /* Compare Threshold */
#define BITL_ADC_ADCCMP2_THR                                  (16U)          /* Compare Threshold */
#define BITM_ADC_ADCCMP2_THR                                  (0X0003FFFCU)  /* Compare Threshold */
#define BITP_ADC_ADCCMP2_IRQEN                                (18U)          /* Enable IRQ Generation */
#define BITL_ADC_ADCCMP2_IRQEN                                (1U)           /* Enable IRQ Generation */
#define BITM_ADC_ADCCMP2_IRQEN                                (0X00040000U)  /* Enable IRQ Generation */
#define BITP_ADC_ADCCMP2_CH                                   (19U)          /* Channel Index for Data Comparison */
#define BITL_ADC_ADCCMP2_CH                                   (5U)           /* Channel Index for Data Comparison */
#define BITM_ADC_ADCCMP2_CH                                   (0X00F80000U)  /* Channel Index for Data Comparison */

/* ----------------------------------------------------------------------------------------------------
          ADCCMP3                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_ADC_ADCCMP3_EN                                   (0U)           /* Digital Comparator Enable */
#define BITL_ADC_ADCCMP3_EN                                   (1U)           /* Digital Comparator Enable */
#define BITM_ADC_ADCCMP3_EN                                   (0X00000001U)  /* Digital Comparator Enable */
#define BITP_ADC_ADCCMP3_CMPDIR                               (1U)           /* Select Digital Comparator Direction */
#define BITL_ADC_ADCCMP3_CMPDIR                               (1U)           /* Select Digital Comparator Direction */
#define BITM_ADC_ADCCMP3_CMPDIR                               (0X00000002U)  /* Select Digital Comparator Direction */
#define BITP_ADC_ADCCMP3_THR                                  (2U)           /* Compare Threshold */
#define BITL_ADC_ADCCMP3_THR                                  (16U)          /* Compare Threshold */
#define BITM_ADC_ADCCMP3_THR                                  (0X0003FFFCU)  /* Compare Threshold */
#define BITP_ADC_ADCCMP3_IRQEN                                (18U)          /* Enable IRQ Generation */
#define BITL_ADC_ADCCMP3_IRQEN                                (1U)           /* Enable IRQ Generation */
#define BITM_ADC_ADCCMP3_IRQEN                                (0X00040000U)  /* Enable IRQ Generation */
#define BITP_ADC_ADCCMP3_CH                                   (19U)          /* Channel Index for Data Comparison */
#define BITL_ADC_ADCCMP3_CH                                   (5U)           /* Channel Index for Data Comparison */
#define BITM_ADC_ADCCMP3_CH                                   (0X00F80000U)  /* Channel Index for Data Comparison */

#endif  /* end ifndef ADC_ADDR_RDEF_H_ */


#ifndef COMP_ADDR_RDEF_H_
#define COMP_ADDR_RDEF_H_    /* COMP: Your module description, here. */

/* ====================================================================================================
        COMP Module Instances Address and Mask Definitions
   ==================================================================================================== */
#define INST_COMP                                                (0X40068A00U)    /* comp: */

#define MASK_COMP                                                (0X000001FFU)    /* COMP: Your module description, here. */

/* ====================================================================================================
        COMP Module Register Address Offset Definitions
   ==================================================================================================== */
#define IDX_COMP_COMPCON0                                       (0X000U)    /* No description provided */
#define IDX_COMP_COMPCON1                                       (0X004U)    /* No description provided */
#define IDX_COMP_COMPCON2                                       (0X008U)    /* No description provided */
#define IDX_COMP_COMPCON3                                       (0X00CU)    /* No description provided */
#define IDX_COMP_COMPIRQSTAT                                    (0X010U)    /* No description provided */

/* ====================================================================================================
        COMP Module Register ResetValue Definitions
   ==================================================================================================== */
#define RSTVAL_COMP_COMPCON0                                       (0X60) 
#define RSTVAL_COMP_COMPCON1                                       (0X60) 
#define RSTVAL_COMP_COMPCON2                                       (0X60) 
#define RSTVAL_COMP_COMPCON3                                       (0X60) 
#define RSTVAL_COMP_COMPIRQSTAT                                    (0X0) 

/* ====================================================================================================
        COMP Module Register BitPositions, Lengths, Masks and Enumerations Definitions
   ==================================================================================================== */

/* ----------------------------------------------------------------------------------------------------
          COMPCON0                                              Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_COMP_COMPCON_N__HYS                              (0U)           /* Comp Hysteresis Register */
#define BITL_COMP_COMPCON_N__HYS                              (5U)           /* Comp Hysteresis Register */
#define BITM_COMP_COMPCON_N__HYS                              (0X0000001FU)  /* Comp Hysteresis Register */
#define BITP_COMP_COMPCON_N__INV                              (7U)           /* Select Output Logic State */
#define BITL_COMP_COMPCON_N__INV                              (1U)           /* Select Output Logic State */
#define BITM_COMP_COMPCON_N__INV                              (0X00000080U)  /* Select Output Logic State */
#define BITP_COMP_COMPCON_N__OUT                              (8U)           /* Comp Interrupt Select */
#define BITL_COMP_COMPCON_N__OUT                              (2U)           /* Comp Interrupt Select */
#define BITM_COMP_COMPCON_N__OUT                              (0X00000300U)  /* Comp Interrupt Select */
#define BITP_COMP_COMPCON_N__INNEG                            (10U)          /* Select Comparator Negative Input Source */
#define BITL_COMP_COMPCON_N__INNEG                            (3U)           /* Select Comparator Negative Input Source */
#define BITM_COMP_COMPCON_N__INNEG                            (0X00001C00U)  /* Select Comparator Negative Input Source */
#define BITP_COMP_COMPCON_N__INPOS                            (13U)          /* Select Comparator Positive Input Source */
#define BITL_COMP_COMPCON_N__INPOS                            (3U)           /* Select Comparator Positive Input Source */
#define BITM_COMP_COMPCON_N__INPOS                            (0X0000E000U)  /* Select Comparator Positive Input Source */
#define BITP_COMP_COMPCON_N__HYSTYP                           (16U)          /* Select Hysteresis Type */
#define BITL_COMP_COMPCON_N__HYSTYP                           (1U)           /* Select Hysteresis Type */
#define BITM_COMP_COMPCON_N__HYSTYP                           (0X00010000U)  /* Select Hysteresis Type */
#define BITP_COMP_COMPCON_N__EN                               (17U)          /* Enable Comparator */
#define BITL_COMP_COMPCON_N__EN                               (1U)           /* Enable Comparator */
#define BITM_COMP_COMPCON_N__EN                               (0X00020000U)  /* Enable Comparator */
#define BITP_COMP_COMPCON_N__INTEN                            (18U)          /* Interrupt Enable */
#define BITL_COMP_COMPCON_N__INTEN                            (1U)           /* Interrupt Enable */
#define BITM_COMP_COMPCON_N__INTEN                            (0X00040000U)  /* Interrupt Enable */
#define BITP_COMP_COMPCON_N__INTMODE                          (19U)          /* Interrupt Mode */
#define BITL_COMP_COMPCON_N__INTMODE                          (2U)           /* Interrupt Mode */
#define BITM_COMP_COMPCON_N__INTMODE                          (0X00180000U)  /* Interrupt Mode */

#define ENUM_COMP_COMPCON_N__INTMODE_RISEEDGE                 (0X00000000U)  /* Generate Interrupt if Rising Edge Happens */
#define ENUM_COMP_COMPCON_N__INTMODE_FALLEDGE                 (0X00000001U)  /* Generate Interrupt if Falling Edge Happens */
#define ENUM_COMP_COMPCON_N__INTMODE_LOWLEVEL                 (0X00000002U)  /* Generate Interrupt if Low Level Happens */
#define ENUM_COMP_COMPCON_N__INTMODE_HIGHLEVEL                (0X00000003U)  /* Generate Interrupt if High Level Happens */
#define ENUM_COMP_COMPCON_N__HYSTYP_EXTHYS                    (0X00000000U)  /* Select External Hysteresis */
#define ENUM_COMP_COMPCON_N__HYSTYP_INTHYS                    (0X00000001U)  /* Select Internal Hysteresis */
#define ENUM_COMP_COMPCON_N__INPOS_NO_INPUT                   (0X00000000U)  /* All Input Switches Off */
#define ENUM_COMP_COMPCON_N__INPOS_AINXP                      (0X00000001U)  /* Enable AIN8/10/12/14 for comp 0/1/2/3 */
#define ENUM_COMP_COMPCON_N__INPOS_PGAIN                      (0X00000002U)  /* Enable PGA0/1/2/3 for comp 0/1/2/3 */
#define ENUM_COMP_COMPCON_N__INPOS_GPIO_ANAIN                 (0X00000003U)  /* Enable GPIO0.6/0.7/2.0/2.1 Analog Signal Input for comp 0/1/2/3 */
#define ENUM_COMP_COMPCON_N__INNEG_NO_INPUT                   (0X00000000U)  /* All Input Switches Off */
#define ENUM_COMP_COMPCON_N__INNEG_AHIDIV2IN                  (0X00000001U)  /* Enable Half Avdd Input */
#define ENUM_COMP_COMPCON_N__INNEG_AINXN                      (0X00000002U)  /* Enable AIN9/11/13/15 for Comp 0/1/2/3 */
#define ENUM_COMP_COMPCON_N__INNEG_VDAC8IN                    (0X00000003U)  /* Enable VDAC8 Input */
#define ENUM_COMP_COMPCON_N__INNEG_VDAC9IN                    (0X00000004U)  /* Enable VDAC9 Input */
#define ENUM_COMP_COMPCON_N__INNEG_VDAC10IN                   (0X00000005U)  /* Enable VDAC10 Input */
#define ENUM_COMP_COMPCON_N__INNEG_VDAC11IN                   (0X00000006U)  /* Enable VDAC11 Input */
#define ENUM_COMP_COMPCON_N__INNEG_V1P25REFIN                 (0X00000007U)  /* Enable 1.25V Ref Input from AIN15(BUF1) */
#define ENUM_COMP_COMPCON_N__OUT_DIS00                        (0X00000000U)  /* Output to Test Pad Disable */
#define ENUM_COMP_COMPCON_N__OUT_DIS01                        (0X00000001U)  /* Output to Test Pad Disable */
#define ENUM_COMP_COMPCON_N__OUT_EN10                         (0X00000002U)  /* Output to Test Pad Enable */
#define ENUM_COMP_COMPCON_N__OUT_DIS11                        (0X00000003U)  /* Output to Test Pad Disable */
#define ENUM_COMP_COMPCON_N__INV_NOM                          (0X00000000U)  /* Output is High if +ve is higher than -ve input */
#define ENUM_COMP_COMPCON_N__INV_COV                          (0X00000001U)  /* Output is High if -ve is higher than +ve input */
#define ENUM_COMP_COMPCON_N__HYS_DIS                          (0X00000000U)  /* hysteresis disabled */
#define ENUM_COMP_COMPCON_N__HYS_EN10MV                       (0X00000001U)  /* 10mv hysteresis enabled */
#define ENUM_COMP_COMPCON_N__HYS_EN25MV                       (0X00000002U)  /* 25mv hysteresis */
#define ENUM_COMP_COMPCON_N__HYS_EN35MV                       (0X00000003U)  /* 35mv hysteresis */
#define ENUM_COMP_COMPCON_N__HYS_EN50MV                       (0X00000006U)  /* 50mv hysteresis */
#define ENUM_COMP_COMPCON_N__HYS_EN60MV                       (0X00000007U)  /* 60mv hysteresis */
#define ENUM_COMP_COMPCON_N__HYS_EN75MV                       (0X0000000CU)  /* 75mv hysteresis */
#define ENUM_COMP_COMPCON_N__HYS_EN85MV                       (0X0000000DU)  /* 85mv hysteresis */
#define ENUM_COMP_COMPCON_N__HYS_EN100MV                      (0X0000000EU)  /* 100mv hysteresis */
#define ENUM_COMP_COMPCON_N__HYS_EN110MV                      (0X00000011U)  /* 110mv hysteresis */
#define ENUM_COMP_COMPCON_N__HYS_EN125MV                      (0X00000012U)  /* 125mv hysteresis */
#define ENUM_COMP_COMPCON_N__HYS_EN135MV                      (0X00000013U)  /* 135mv hysteresis */
#define ENUM_COMP_COMPCON_N__HYS_EN150MV                      (0X00000016U)  /* 150mv hysteresis */
#define ENUM_COMP_COMPCON_N__HYS_EN160MV                      (0X00000017U)  /* 160mv hysteresis */
#define ENUM_COMP_COMPCON_N__HYS_EN175MV                      (0X0000001CU)  /* 175mv hysteresis */
#define ENUM_COMP_COMPCON_N__HYS_EN185MV                      (0X0000001DU)  /* 185mv hysteresis */
#define ENUM_COMP_COMPCON_N__HYS_EN200MV                      (0X0000001EU)  /* 200mv hysteresis */
#define ENUM_COMP_COMPCON_N__HYS_EN210MV                      (0X0000001FU)  /* 210mv hysteresis */

/* ----------------------------------------------------------------------------------------------------
          COMPIRQSTAT                                           Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_COMP_COMPIRQSTAT_COMP0                           (0U)           /* Comparator 0 Interrupt Status */
#define BITL_COMP_COMPIRQSTAT_COMP0                           (1U)           /* Comparator 0 Interrupt Status */
#define BITM_COMP_COMPIRQSTAT_COMP0                           (0X00000001U)  /* Comparator 0 Interrupt Status */
#define BITP_COMP_COMPIRQSTAT_COMP1                           (1U)           /* Comparator 1 Interrupt Status */
#define BITL_COMP_COMPIRQSTAT_COMP1                           (1U)           /* Comparator 1 Interrupt Status */
#define BITM_COMP_COMPIRQSTAT_COMP1                           (0X00000002U)  /* Comparator 1 Interrupt Status */
#define BITP_COMP_COMPIRQSTAT_COMP2                           (2U)           /* Comparator 2 Interrupt Status */
#define BITL_COMP_COMPIRQSTAT_COMP2                           (1U)           /* Comparator 2 Interrupt Status */
#define BITM_COMP_COMPIRQSTAT_COMP2                           (0X00000004U)  /* Comparator 2 Interrupt Status */
#define BITP_COMP_COMPIRQSTAT_COMP3                           (3U)           /* Comparator 3 Interrupt Status */
#define BITL_COMP_COMPIRQSTAT_COMP3                           (1U)           /* Comparator 3 Interrupt Status */
#define BITM_COMP_COMPIRQSTAT_COMP3                           (0X00000008U)  /* Comparator 3 Interrupt Status */

#endif  /* end ifndef COMP_ADDR_RDEF_H_ */


#ifndef OSC_MMRS_ADDR_RDEF_H_
#define OSC_MMRS_ADDR_RDEF_H_    /* OSC_MMRS: Your module description, here. */

/* ====================================================================================================
        OSC_MMRS Module Instances Address and Mask Definitions
   ==================================================================================================== */
#define INST_OSC                                                 (0X40068E00U)    /* osc: */

#define MASK_OSC_MMRS                                            (0X000001FFU)    /* OSC_MMRS: Your module description, here. */

/* ====================================================================================================
        OSC_MMRS Module Register Address Offset Definitions
   ==================================================================================================== */
#define IDX_OSC_MMRS_HFOSCCTRL                                  (0X004U)    /* No description provided */
#define IDX_OSC_MMRS_HFXTALCTRL                                 (0X018U)    /* No description provided */

/* ====================================================================================================
        OSC_MMRS Module Register ResetValue Definitions
   ==================================================================================================== */
#define RSTVAL_OSC_MMRS_HFOSCCTRL                                  (0X0) 
#define RSTVAL_OSC_MMRS_HFXTALCTRL                                 (0X2) 

/* ====================================================================================================
        OSC_MMRS Module Register BitPositions, Lengths, Masks and Enumerations Definitions
   ==================================================================================================== */

/* ----------------------------------------------------------------------------------------------------
          HFOSCCTRL                                             Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_OSC_MMRS_HFOSCCTRL_CLKSEL                        (0U)           /* Selects Either Xtal RC */
#define BITL_OSC_MMRS_HFOSCCTRL_CLKSEL                        (2U)           /* Selects Either Xtal RC */
#define BITM_OSC_MMRS_HFOSCCTRL_CLKSEL                        (0X00000003U)  /* Selects Either Xtal RC */
#define BITP_OSC_MMRS_HFOSCCTRL_PDOSC16M                      (2U)           /* Power Down Pin Oscillator */
#define BITL_OSC_MMRS_HFOSCCTRL_PDOSC16M                      (1U)           /* Power Down Pin Oscillator */
#define BITM_OSC_MMRS_HFOSCCTRL_PDOSC16M                      (0X00000004U)  /* Power Down Pin Oscillator */

#define ENUM_OSC_MMRS_HFOSCCTRL_CLKSEL_SELRC                  (0X00000000U)  /* RC clock */
#define ENUM_OSC_MMRS_HFOSCCTRL_CLKSEL_SELXTAL                (0X00000001U)  /* Xtal clock */
#define ENUM_OSC_MMRS_HFOSCCTRL_CLKSEL_SELEXT                 (0X00000002U)  /* External Clock */
#define ENUM_OSC_MMRS_HFOSCCTRL_CLKSEL_SELGND                 (0X00000003U)  /* gnd */

/* ----------------------------------------------------------------------------------------------------
          HFXTALCTRL                                            Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_OSC_MMRS_HFXTALCTRL_ENXTAL                       (0U)           /* Enable Xtal */
#define BITL_OSC_MMRS_HFXTALCTRL_ENXTAL                       (1U)           /* Enable Xtal */
#define BITM_OSC_MMRS_HFXTALCTRL_ENXTAL                       (0X00000001U)  /* Enable Xtal */

#endif  /* end ifndef OSC_MMRS_ADDR_RDEF_H_ */


#ifndef PGA_ADDR_RDEF_H_
#define PGA_ADDR_RDEF_H_    /* PGA: Your module description, here. */

/* ====================================================================================================
        PGA Module Instances Address and Mask Definitions
   ==================================================================================================== */
#define INST_PGA                                                 (0X40069000U)    /* pga: */

#define MASK_PGA                                                 (0X000001FFU)    /* PGA: Your module description, here. */

/* ====================================================================================================
        PGA Module Register Address Offset Definitions
   ==================================================================================================== */
#define IDX_PGA_PGABIASCON                                      (0X000U)    /* PGA Bias Circuit Control Signal */
#define IDX_PGA_PGA0CON                                         (0X020U)    /* PGA0 Control Register */
#define IDX_PGA_PGA0CHPCON                                      (0X024U)    /* PGA0 Chop Function Ctrl */
#define IDX_PGA_PGA3CHPCON                                      (0X028U)    /* PGA3 Chop Function Ctrl */
#define IDX_PGA_PGA1CON                                         (0X070U)    /* PGA1 Control Register */
#define IDX_PGA_PGA1CHPCON                                      (0X074U)    /* PGA1 Chop Function Ctrl */
#define IDX_PGA_PGA2CON                                         (0X0A0U)    /* PGA2 Control Register */
#define IDX_PGA_PGA2CHPCON                                      (0X0A4U)    /* PGA2 Chop Function Ctrl */
#define IDX_PGA_PGA3CON                                         (0X0D0U)    /* PGA3 Control Register */

/* ====================================================================================================
        PGA Module Register ResetValue Definitions
   ==================================================================================================== */
#define RSTVAL_PGA_PGABIASCON                                      (0X3F) 
#define RSTVAL_PGA_PGA0CON                                         (0X19) 
#define RSTVAL_PGA_PGA0CHPCON                                      (0X1) 
#define RSTVAL_PGA_PGA3CHPCON                                      (0X1) 
#define RSTVAL_PGA_PGA1CON                                         (0X11) 
#define RSTVAL_PGA_PGA1CHPCON                                      (0X1) 
#define RSTVAL_PGA_PGA2CON                                         (0X19) 
#define RSTVAL_PGA_PGA2CHPCON                                      (0X1) 
#define RSTVAL_PGA_PGA3CON                                         (0X11) 

/* ====================================================================================================
        PGA Module Register BitPositions, Lengths, Masks and Enumerations Definitions
   ==================================================================================================== */

/* ----------------------------------------------------------------------------------------------------
          PGABIASCON                                            Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_PGA_PGABIASCON_PD0BUF0P2                         (0U)           /* Buf_200mv Power Down */
#define BITL_PGA_PGABIASCON_PD0BUF0P2                         (1U)           /* Buf_200mv Power Down */
#define BITM_PGA_PGABIASCON_PD0BUF0P2                         (0X00000001U)  /* Buf_200mv Power Down */
#define BITP_PGA_PGABIASCON_PD1BUF0P2                         (1U)           /* Buf_200mv Power Down */
#define BITL_PGA_PGABIASCON_PD1BUF0P2                         (1U)           /* Buf_200mv Power Down */
#define BITM_PGA_PGABIASCON_PD1BUF0P2                         (0X00000002U)  /* Buf_200mv Power Down */
#define BITP_PGA_PGABIASCON_PD0BUF1P25                        (2U)           /* BUF1P25 Power Down */
#define BITL_PGA_PGABIASCON_PD0BUF1P25                        (1U)           /* BUF1P25 Power Down */
#define BITM_PGA_PGABIASCON_PD0BUF1P25                        (0X00000004U)  /* BUF1P25 Power Down */
#define BITP_PGA_PGABIASCON_PD1BUF1P25                        (3U)           /* BUF1P25 Power Down */
#define BITL_PGA_PGABIASCON_PD1BUF1P25                        (1U)           /* BUF1P25 Power Down */
#define BITM_PGA_PGABIASCON_PD1BUF1P25                        (0X00000008U)  /* BUF1P25 Power Down */
#define BITP_PGA_PGABIASCON_PD2BUF1P25                        (4U)           /* BUF1P25 Power Down */
#define BITL_PGA_PGABIASCON_PD2BUF1P25                        (1U)           /* BUF1P25 Power Down */
#define BITM_PGA_PGABIASCON_PD2BUF1P25                        (0X00000010U)  /* BUF1P25 Power Down */
#define BITP_PGA_PGABIASCON_PD3BUF1P25                        (5U)           /* BUF1P25 Power Down */
#define BITL_PGA_PGABIASCON_PD3BUF1P25                        (1U)           /* BUF1P25 Power Down */
#define BITM_PGA_PGABIASCON_PD3BUF1P25                        (0X00000020U)  /* BUF1P25 Power Down */

#define ENUM_PGA_PGABIASCON_PD3BUF1P25_ENABLE                 (0X00000000U)  /* BUF1p25 Enable */
#define ENUM_PGA_PGABIASCON_PD3BUF1P25_OFF                    (0X00000001U)  /* BUF1p25 Power Down */
#define ENUM_PGA_PGABIASCON_PD2BUF1P25_ENABLE                 (0X00000000U)  /* BUF1p25 Enable */
#define ENUM_PGA_PGABIASCON_PD2BUF1P25_OFF                    (0X00000001U)  /* BUF1p25 Power Down */
#define ENUM_PGA_PGABIASCON_PD1BUF1P25_ENABLE                 (0X00000000U)  /* BUF1p25 Enable */
#define ENUM_PGA_PGABIASCON_PD1BUF1P25_OFF                    (0X00000001U)  /* BUF1p25 Power Down */
#define ENUM_PGA_PGABIASCON_PD0BUF1P25_ENABLE                 (0X00000000U)  /* BUF1p25 Enable */
#define ENUM_PGA_PGABIASCON_PD0BUF1P25_OFF                    (0X00000001U)  /* BUF1p25 Power Down */
#define ENUM_PGA_PGABIASCON_PD1BUF0P2_ENABLE                  (0X00000000U)  /* Buf_200mV Enable */
#define ENUM_PGA_PGABIASCON_PD1BUF0P2_OFF                     (0X00000001U)  /* Buf_200mv Power Down */
#define ENUM_PGA_PGABIASCON_PD0BUF0P2_ENABLE                  (0X00000000U)  /* Buf_200mV Enable */
#define ENUM_PGA_PGABIASCON_PD0BUF0P2_OFF                     (0X00000001U)  /* Buf_200mv Power Down */

/* ----------------------------------------------------------------------------------------------------
          PGA0CON                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_PGA_PGA0CON_PDPGACORE                            (0U)           /* PGA Core Power Down */
#define BITL_PGA_PGA0CON_PDPGACORE                            (1U)           /* PGA Core Power Down */
#define BITM_PGA_PGA0CON_PDPGACORE                            (0X00000001U)  /* PGA Core Power Down */
#define BITP_PGA_PGA0CON_MODE                                 (1U)           /* PGA or TIA Mode Selection */
#define BITL_PGA_PGA0CON_MODE                                 (1U)           /* PGA or TIA Mode Selection */
#define BITM_PGA_PGA0CON_MODE                                 (0X00000002U)  /* PGA or TIA Mode Selection */
#define BITP_PGA_PGA0CON_PGAMODE                              (2U)           /* PGA DC Mode or AC Couple Mode Selection */
#define BITL_PGA_PGA0CON_PGAMODE                              (1U)           /* PGA DC Mode or AC Couple Mode Selection */
#define BITM_PGA_PGA0CON_PGAMODE                              (0X00000004U)  /* PGA DC Mode or AC Couple Mode Selection */
#define BITP_PGA_PGA0CON_CAPBYPASS                            (3U)           /* Bypass the External Cap */
#define BITL_PGA_PGA0CON_CAPBYPASS                            (1U)           /* Bypass the External Cap */
#define BITM_PGA_PGA0CON_CAPBYPASS                            (0X00000008U)  /* Bypass the External Cap */
#define BITP_PGA_PGA0CON_PGAGAIN                              (5U)           /* PGA Gain Configuration */
#define BITL_PGA_PGA0CON_PGAGAIN                              (3U)           /* PGA Gain Configuration */
#define BITM_PGA_PGA0CON_PGAGAIN                              (0X000000E0U)  /* PGA Gain Configuration */
#define BITP_PGA_PGA0CON_TIAGAIN                              (8U)           /* TIA Gain Configuration */
#define BITL_PGA_PGA0CON_TIAGAIN                              (3U)           /* TIA Gain Configuration */
#define BITM_PGA_PGA0CON_TIAGAIN                              (0X00000700U)  /* TIA Gain Configuration */
#define BITP_PGA_PGA0CON_TIAVDACSEL                           (11U)          /* TIA Vbias Selection of VDAC Channel */
#define BITL_PGA_PGA0CON_TIAVDACSEL                           (2U)           /* TIA Vbias Selection of VDAC Channel */
#define BITM_PGA_PGA0CON_TIAVDACSEL                           (0X00001800U)  /* TIA Vbias Selection of VDAC Channel */
#define BITP_PGA_PGA0CON_DRVEN                                (14U)          /* Sink Current Ability Improve */
#define BITL_PGA_PGA0CON_DRVEN                                (1U)           /* Sink Current Ability Improve */
#define BITM_PGA_PGA0CON_DRVEN                                (0X00004000U)  /* Sink Current Ability Improve */
#define BITP_PGA_PGA0CON_TIASRCEN                             (15U)          /* Select Between Source Mode and Sink Mode */
#define BITL_PGA_PGA0CON_TIASRCEN                             (1U)           /* Select Between Source Mode and Sink Mode */
#define BITM_PGA_PGA0CON_TIASRCEN                             (0X00008000U)  /* Select Between Source Mode and Sink Mode */

#define ENUM_PGA_PGA0CON_TIASRCEN_SINKMD                      (0X00000000U)  /* Sink Mode */
#define ENUM_PGA_PGA0CON_TIASRCEN_SOURCEMD                    (0X00000001U)  /* Source Mode */
#define ENUM_PGA_PGA0CON_DRVEN_DRVDISABLE                     (0X00000000U)  /* Disable Isink Ability Improvement, Normal Drive */
#define ENUM_PGA_PGA0CON_DRVEN_DRVENABLE                      (0X00000001U)  /* Enable Isink Ability Improvement */
#define ENUM_PGA_PGA0CON_TIAVDACSEL_VDAC8SEL                  (0X00000000U)  /* Select VDAC8 as TIA Vbias */
#define ENUM_PGA_PGA0CON_TIAVDACSEL_VDAC9SEL                  (0X00000001U)  /* Select VDAC9 as TIA Vbias */
#define ENUM_PGA_PGA0CON_TIAVDACSEL_VDAC10SEL                 (0X00000002U)  /* Select VDAC10 as TIA Vbias */
#define ENUM_PGA_PGA0CON_TIAVDACSEL_VDAC11SEL                 (0X00000003U)  /* Select VDAC11 as TIA Vbias */
#define ENUM_PGA_PGA0CON_TIAGAIN_RES250                       (0X00000000U)  /* TIARES=250ohm */
#define ENUM_PGA_PGA0CON_TIAGAIN_RES750                       (0X00000001U)  /* TIARES=750ohm */
#define ENUM_PGA_PGA0CON_TIAGAIN_RES2K                        (0X00000002U)  /* TIARES=2kohm */
#define ENUM_PGA_PGA0CON_TIAGAIN_RES5K                        (0X00000003U)  /* TIARES=5kohm */
#define ENUM_PGA_PGA0CON_TIAGAIN_RES10K                       (0X00000004U)  /* TIARES=10kohm */
#define ENUM_PGA_PGA0CON_TIAGAIN_RES20K                       (0X00000005U)  /* TIARES=20kohm */
#define ENUM_PGA_PGA0CON_TIAGAIN_RES100K                      (0X00000006U)  /* TIARES=100kohm */
#define ENUM_PGA_PGA0CON_PGAGAIN_GAIN1                        (0X00000000U)  /* Gain=1 */
#define ENUM_PGA_PGA0CON_PGAGAIN_GAIN2                        (0X00000001U)  /* GAIN=2 */
#define ENUM_PGA_PGA0CON_PGAGAIN_GAIN4                        (0X00000002U)  /* GAIN=4 */
#define ENUM_PGA_PGA0CON_PGAGAIN_GAIN6                        (0X00000003U)  /* GAIN=6 */
#define ENUM_PGA_PGA0CON_PGAGAIN_GAIN8                        (0X00000004U)  /* GAIN=8 */
#define ENUM_PGA_PGA0CON_PGAGAIN_GAIN10                       (0X00000005U)  /* GAIN=10 */
#define ENUM_PGA_PGA0CON_CAPBYPASS_BYPEN                      (0X00000001U)  /* Bypass the External Cap */
#define ENUM_PGA_PGA0CON_CAPBYPASS_BYPDISABLE                 (0X00000000U)  /* Select the External CAP */
#define ENUM_PGA_PGA0CON_PGAMODE_DCCOUPLE                     (0X00000000U)  /* PGA DC Mode Enable */
#define ENUM_PGA_PGA0CON_PGAMODE_ACCOUPLE                     (0X00000001U)  /* PGA AC Coupling Mode Enable */
#define ENUM_PGA_PGA0CON_MODE_PGAMODE                         (0X00000000U)  /* PGA Mode Enable */
#define ENUM_PGA_PGA0CON_MODE_TIAMODE                         (0X00000001U)  /* TIA Mode Enable */

/* ----------------------------------------------------------------------------------------------------
          PGA0CHPCON                                            Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_PGA_PGA0CHPCON_CHOPOFF                           (0U)           /* Disable Chop Function */
#define BITL_PGA_PGA0CHPCON_CHOPOFF                           (1U)           /* Disable Chop Function */
#define BITM_PGA_PGA0CHPCON_CHOPOFF                           (0X00000001U)  /* Disable Chop Function */

#define ENUM_PGA_PGA0CHPCON_CHOPOFF_CHOPEN                    (0X00000000U)  /* Enable Chop Function */
#define ENUM_PGA_PGA0CHPCON_CHOPOFF_CHOPDIS                   (0X00000001U)  /* Disable Chop Function */

/* ----------------------------------------------------------------------------------------------------
          PGA3CHPCON                                            Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_PGA_PGA3CHPCON_CHOPOFF                           (0U)           /* Disable Chop Function */
#define BITL_PGA_PGA3CHPCON_CHOPOFF                           (1U)           /* Disable Chop Function */
#define BITM_PGA_PGA3CHPCON_CHOPOFF                           (0X00000001U)  /* Disable Chop Function */

#define ENUM_PGA_PGA3CHPCON_CHOPOFF_CHOPEN                    (0X00000000U)  /* Enable Chop Function */
#define ENUM_PGA_PGA3CHPCON_CHOPOFF_CHOPDIS                   (0X00000001U)  /* Disable Chop Function */

/* ----------------------------------------------------------------------------------------------------
          PGA1CON                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_PGA_PGA1CON_PDPGACORE                            (0U)           /* PGA Core Power Down */
#define BITL_PGA_PGA1CON_PDPGACORE                            (1U)           /* PGA Core Power Down */
#define BITM_PGA_PGA1CON_PDPGACORE                            (0X00000001U)  /* PGA Core Power Down */
#define BITP_PGA_PGA1CON_MODE                                 (1U)           /* PGA or TIA Mode Selection */
#define BITL_PGA_PGA1CON_MODE                                 (1U)           /* PGA or TIA Mode Selection */
#define BITM_PGA_PGA1CON_MODE                                 (0X00000002U)  /* PGA or TIA Mode Selection */
#define BITP_PGA_PGA1CON_PGAMODE                              (2U)           /* PGA DC Mode or AC Couple Mode Selection */
#define BITL_PGA_PGA1CON_PGAMODE                              (1U)           /* PGA DC Mode or AC Couple Mode Selection */
#define BITM_PGA_PGA1CON_PGAMODE                              (0X00000004U)  /* PGA DC Mode or AC Couple Mode Selection */
#define BITP_PGA_PGA1CON_PGAGAIN                              (5U)           /* PGA Gain Configuration */
#define BITL_PGA_PGA1CON_PGAGAIN                              (3U)           /* PGA Gain Configuration */
#define BITM_PGA_PGA1CON_PGAGAIN                              (0X000000E0U)  /* PGA Gain Configuration */
#define BITP_PGA_PGA1CON_TIAGAIN                              (8U)           /* TIA Gain Configuration */
#define BITL_PGA_PGA1CON_TIAGAIN                              (3U)           /* TIA Gain Configuration */
#define BITM_PGA_PGA1CON_TIAGAIN                              (0X00000700U)  /* TIA Gain Configuration */
#define BITP_PGA_PGA1CON_TIAVDACSEL                           (11U)          /* TIA Vbias Selection of VDAC Channel */
#define BITL_PGA_PGA1CON_TIAVDACSEL                           (2U)           /* TIA Vbias Selection of VDAC Channel */
#define BITM_PGA_PGA1CON_TIAVDACSEL                           (0X00001800U)  /* TIA Vbias Selection of VDAC Channel */
#define BITP_PGA_PGA1CON_DRVEN                                (14U)          /* Sink Current Ability Improve */
#define BITL_PGA_PGA1CON_DRVEN                                (1U)           /* Sink Current Ability Improve */
#define BITM_PGA_PGA1CON_DRVEN                                (0X00004000U)  /* Sink Current Ability Improve */
#define BITP_PGA_PGA1CON_TIASRCEN                             (15U)          /* Select Between Source Mode and Sink Mode */
#define BITL_PGA_PGA1CON_TIASRCEN                             (1U)           /* Select Between Source Mode and Sink Mode */
#define BITM_PGA_PGA1CON_TIASRCEN                             (0X00008000U)  /* Select Between Source Mode and Sink Mode */

#define ENUM_PGA_PGA1CON_TIASRCEN_SINKMD                      (0X00000000U)  /* Sink Mode */
#define ENUM_PGA_PGA1CON_TIASRCEN_SOURCEMD                    (0X00000001U)  /* Source Mode */
#define ENUM_PGA_PGA1CON_DRVEN_DRVDISABLE                     (0X00000000U)  /* Disable Isink Ability Improvement, Normal Drive */
#define ENUM_PGA_PGA1CON_DRVEN_DRVENABLE                      (0X00000001U)  /* Enable Isink Ability Improvement */
#define ENUM_PGA_PGA1CON_TIAVDACSEL_VDAC8SEL                  (0X00000000U)  /* Select VDAC8 as TIA Vbias */
#define ENUM_PGA_PGA1CON_TIAVDACSEL_VDAC9SEL                  (0X00000001U)  /* Select VDAC9 as TIA Vbias */
#define ENUM_PGA_PGA1CON_TIAVDACSEL_VDAC10SEL                 (0X00000002U)  /* Select VDAC10 as TIA Vbias */
#define ENUM_PGA_PGA1CON_TIAVDACSEL_VDAC11SEL                 (0X00000003U)  /* Select VDAC11 as TIA Vbias */
#define ENUM_PGA_PGA1CON_TIAGAIN_RES250                       (0X00000000U)  /* TIARES=250ohm */
#define ENUM_PGA_PGA1CON_TIAGAIN_RES750                       (0X00000001U)  /* TIARES=750ohm */
#define ENUM_PGA_PGA1CON_TIAGAIN_RES2K                        (0X00000002U)  /* TIARES=2kohm */
#define ENUM_PGA_PGA1CON_TIAGAIN_RES5K                        (0X00000003U)  /* TIARES=5kohm */
#define ENUM_PGA_PGA1CON_TIAGAIN_RES10K                       (0X00000004U)  /* TIARES=10kohm */
#define ENUM_PGA_PGA1CON_TIAGAIN_RES20K                       (0X00000005U)  /* TIARES=20kohm */
#define ENUM_PGA_PGA1CON_TIAGAIN_RES100K                      (0X00000006U)  /* TIARES=100kohm */
#define ENUM_PGA_PGA1CON_PGAGAIN_GAIN1                        (0X00000000U)  /* Gain=1 */
#define ENUM_PGA_PGA1CON_PGAGAIN_GAIN2                        (0X00000001U)  /* GAIN=2 */
#define ENUM_PGA_PGA1CON_PGAGAIN_GAIN4                        (0X00000002U)  /* GAIN=4 */
#define ENUM_PGA_PGA1CON_PGAGAIN_GAIN6                        (0X00000003U)  /* GAIN=6 */
#define ENUM_PGA_PGA1CON_PGAGAIN_GAIN8                        (0X00000004U)  /* GAIN=8 */
#define ENUM_PGA_PGA1CON_PGAGAIN_GAIN10                       (0X00000005U)  /* GAIN=10 */
#define ENUM_PGA_PGA1CON_PGAMODE_DCCOUPLE                     (0X00000000U)  /* PGA DC Mode Enable */
#define ENUM_PGA_PGA1CON_PGAMODE_ACCOUPLE                     (0X00000001U)  /* PGA AC Coupling Mode Enable */
#define ENUM_PGA_PGA1CON_MODE_PGAMODE                         (0X00000000U)  /* PGA Mode Enable */
#define ENUM_PGA_PGA1CON_MODE_TIAMODE                         (0X00000001U)  /* TIA Mode Enable */

/* ----------------------------------------------------------------------------------------------------
          PGA1CHPCON                                            Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_PGA_PGA1CHPCON_CHOPOFF                           (0U)           /* Disable Chop Function */
#define BITL_PGA_PGA1CHPCON_CHOPOFF                           (1U)           /* Disable Chop Function */
#define BITM_PGA_PGA1CHPCON_CHOPOFF                           (0X00000001U)  /* Disable Chop Function */

#define ENUM_PGA_PGA1CHPCON_CHOPOFF_CHOPEN                    (0X00000000U)  /* Enable Chop Function */
#define ENUM_PGA_PGA1CHPCON_CHOPOFF_CHOPDIS                   (0X00000001U)  /* Disable Chop Function */

/* ----------------------------------------------------------------------------------------------------
          PGA2CON                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_PGA_PGA2CON_PDPGACORE                            (0U)           /* PGA Core Power Down */
#define BITL_PGA_PGA2CON_PDPGACORE                            (1U)           /* PGA Core Power Down */
#define BITM_PGA_PGA2CON_PDPGACORE                            (0X00000001U)  /* PGA Core Power Down */
#define BITP_PGA_PGA2CON_MODE                                 (1U)           /* PGA or TIA Mode Selection */
#define BITL_PGA_PGA2CON_MODE                                 (1U)           /* PGA or TIA Mode Selection */
#define BITM_PGA_PGA2CON_MODE                                 (0X00000002U)  /* PGA or TIA Mode Selection */
#define BITP_PGA_PGA2CON_PGAMODE                              (2U)           /* PGA DC Mode or AC Couple Mode Selection */
#define BITL_PGA_PGA2CON_PGAMODE                              (1U)           /* PGA DC Mode or AC Couple Mode Selection */
#define BITM_PGA_PGA2CON_PGAMODE                              (0X00000004U)  /* PGA DC Mode or AC Couple Mode Selection */
#define BITP_PGA_PGA2CON_CAPBYPASS                            (3U)           /* Bypass the External Cap */
#define BITL_PGA_PGA2CON_CAPBYPASS                            (1U)           /* Bypass the External Cap */
#define BITM_PGA_PGA2CON_CAPBYPASS                            (0X00000008U)  /* Bypass the External Cap */
#define BITP_PGA_PGA2CON_PGAGAIN                              (5U)           /* PGA Gain Configuration */
#define BITL_PGA_PGA2CON_PGAGAIN                              (3U)           /* PGA Gain Configuration */
#define BITM_PGA_PGA2CON_PGAGAIN                              (0X000000E0U)  /* PGA Gain Configuration */
#define BITP_PGA_PGA2CON_TIAGAIN                              (8U)           /* TIA Gain Configuration */
#define BITL_PGA_PGA2CON_TIAGAIN                              (3U)           /* TIA Gain Configuration */
#define BITM_PGA_PGA2CON_TIAGAIN                              (0X00000700U)  /* TIA Gain Configuration */
#define BITP_PGA_PGA2CON_TIAVDACSEL                           (11U)          /* TIA Vbias Selection of VDAC Channel */
#define BITL_PGA_PGA2CON_TIAVDACSEL                           (2U)           /* TIA Vbias Selection of VDAC Channel */
#define BITM_PGA_PGA2CON_TIAVDACSEL                           (0X00001800U)  /* TIA Vbias Selection of VDAC Channel */
#define BITP_PGA_PGA2CON_DRVEN                                (14U)          /* Sink Current Ability Improve */
#define BITL_PGA_PGA2CON_DRVEN                                (1U)           /* Sink Current Ability Improve */
#define BITM_PGA_PGA2CON_DRVEN                                (0X00004000U)  /* Sink Current Ability Improve */
#define BITP_PGA_PGA2CON_TIASRCEN                             (15U)          /* Select Between Source Mode and Sink Mode */
#define BITL_PGA_PGA2CON_TIASRCEN                             (1U)           /* Select Between Source Mode and Sink Mode */
#define BITM_PGA_PGA2CON_TIASRCEN                             (0X00008000U)  /* Select Between Source Mode and Sink Mode */

#define ENUM_PGA_PGA2CON_TIASRCEN_SINKMD                      (0X00000000U)  /* Sink Mode */
#define ENUM_PGA_PGA2CON_TIASRCEN_SOURCEMD                    (0X00000001U)  /* Source Mode */
#define ENUM_PGA_PGA2CON_DRVEN_DRVDISABLE                     (0X00000000U)  /* Disable Isink Ability Improvement, Normal Drive */
#define ENUM_PGA_PGA2CON_DRVEN_DRVENABLE                      (0X00000001U)  /* Enable Isink Ability Improvement */
#define ENUM_PGA_PGA2CON_TIAVDACSEL_VDAC8SEL                  (0X00000000U)  /* Select VDAC8 as TIA Vbias */
#define ENUM_PGA_PGA2CON_TIAVDACSEL_VDAC9SEL                  (0X00000001U)  /* Select VDAC9 as TIA Vbias */
#define ENUM_PGA_PGA2CON_TIAVDACSEL_VDAC10SEL                 (0X00000002U)  /* Select VDAC10 as TIA Vbias */
#define ENUM_PGA_PGA2CON_TIAVDACSEL_VDAC11SEL                 (0X00000003U)  /* Select VDAC11 as TIA Vbias */
#define ENUM_PGA_PGA2CON_TIAGAIN_RES250                       (0X00000000U)  /* TIARES=250ohm */
#define ENUM_PGA_PGA2CON_TIAGAIN_RES750                       (0X00000001U)  /* TIARES=750ohm */
#define ENUM_PGA_PGA2CON_TIAGAIN_RES2K                        (0X00000002U)  /* TIARES=2kohm */
#define ENUM_PGA_PGA2CON_TIAGAIN_RES5K                        (0X00000003U)  /* TIARES=5kohm */
#define ENUM_PGA_PGA2CON_TIAGAIN_RES10K                       (0X00000004U)  /* TIARES=10kohm */
#define ENUM_PGA_PGA2CON_TIAGAIN_RES20K                       (0X00000005U)  /* TIARES=20kohm */
#define ENUM_PGA_PGA2CON_TIAGAIN_RES100K                      (0X00000006U)  /* TIARES=100kohm */
#define ENUM_PGA_PGA2CON_PGAGAIN_GAIN1                        (0X00000000U)  /* Gain=1 */
#define ENUM_PGA_PGA2CON_PGAGAIN_GAIN2                        (0X00000001U)  /* GAIN=2 */
#define ENUM_PGA_PGA2CON_PGAGAIN_GAIN4                        (0X00000002U)  /* GAIN=4 */
#define ENUM_PGA_PGA2CON_PGAGAIN_GAIN6                        (0X00000003U)  /* GAIN=6 */
#define ENUM_PGA_PGA2CON_PGAGAIN_GAIN8                        (0X00000004U)  /* GAIN=8 */
#define ENUM_PGA_PGA2CON_PGAGAIN_GAIN10                       (0X00000005U)  /* GAIN=10 */
#define ENUM_PGA_PGA2CON_CAPBYPASS_BYPEN                      (0X00000001U)  /* Bypass the External Cap */
#define ENUM_PGA_PGA2CON_CAPBYPASS_BYPDISABLE                 (0X00000000U)  /* Select the External CAP */
#define ENUM_PGA_PGA2CON_PGAMODE_DCCOUPLE                     (0X00000000U)  /* PGA DC Mode Enable */
#define ENUM_PGA_PGA2CON_PGAMODE_ACCOUPLE                     (0X00000001U)  /* PGA AC Coupling Mode Enable */
#define ENUM_PGA_PGA2CON_MODE_PGAMODE                         (0X00000000U)  /* PGA Mode Enable */
#define ENUM_PGA_PGA2CON_MODE_TIAMODE                         (0X00000001U)  /* TIA Mode Enable */

/* ----------------------------------------------------------------------------------------------------
          PGA2CHPCON                                            Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_PGA_PGA2CHPCON_CHOPOFF                           (0U)           /* Disable Chop Function */
#define BITL_PGA_PGA2CHPCON_CHOPOFF                           (1U)           /* Disable Chop Function */
#define BITM_PGA_PGA2CHPCON_CHOPOFF                           (0X00000001U)  /* Disable Chop Function */

#define ENUM_PGA_PGA2CHPCON_CHOPOFF_CHOPEN                    (0X00000000U)  /* Enable Chop Function */
#define ENUM_PGA_PGA2CHPCON_CHOPOFF_CHOPDIS                   (0X00000001U)  /* Disable Chop Function */

/* ----------------------------------------------------------------------------------------------------
          PGA3CON                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_PGA_PGA3CON_PDPGACORE                            (0U)           /* PGA Core Power Down */
#define BITL_PGA_PGA3CON_PDPGACORE                            (1U)           /* PGA Core Power Down */
#define BITM_PGA_PGA3CON_PDPGACORE                            (0X00000001U)  /* PGA Core Power Down */
#define BITP_PGA_PGA3CON_MODE                                 (1U)           /* PGA or TIA Mode Selection */
#define BITL_PGA_PGA3CON_MODE                                 (1U)           /* PGA or TIA Mode Selection */
#define BITM_PGA_PGA3CON_MODE                                 (0X00000002U)  /* PGA or TIA Mode Selection */
#define BITP_PGA_PGA3CON_PGAMODE                              (2U)           /* PGA DC Mode or AC Couple Mode Selection */
#define BITL_PGA_PGA3CON_PGAMODE                              (1U)           /* PGA DC Mode or AC Couple Mode Selection */
#define BITM_PGA_PGA3CON_PGAMODE                              (0X00000004U)  /* PGA DC Mode or AC Couple Mode Selection */
#define BITP_PGA_PGA3CON_PGAGAIN                              (5U)           /* PGA Gain Configuration */
#define BITL_PGA_PGA3CON_PGAGAIN                              (3U)           /* PGA Gain Configuration */
#define BITM_PGA_PGA3CON_PGAGAIN                              (0X000000E0U)  /* PGA Gain Configuration */
#define BITP_PGA_PGA3CON_TIAGAIN                              (8U)           /* TIA Gain Configuration */
#define BITL_PGA_PGA3CON_TIAGAIN                              (3U)           /* TIA Gain Configuration */
#define BITM_PGA_PGA3CON_TIAGAIN                              (0X00000700U)  /* TIA Gain Configuration */
#define BITP_PGA_PGA3CON_TIAVDACSEL                           (11U)          /* TIA Vbias Selection of VDAC Channel */
#define BITL_PGA_PGA3CON_TIAVDACSEL                           (2U)           /* TIA Vbias Selection of VDAC Channel */
#define BITM_PGA_PGA3CON_TIAVDACSEL                           (0X00001800U)  /* TIA Vbias Selection of VDAC Channel */
#define BITP_PGA_PGA3CON_DRVEN                                (14U)          /* Sink Current Ability Improve */
#define BITL_PGA_PGA3CON_DRVEN                                (1U)           /* Sink Current Ability Improve */
#define BITM_PGA_PGA3CON_DRVEN                                (0X00004000U)  /* Sink Current Ability Improve */
#define BITP_PGA_PGA3CON_TIASRCEN                             (15U)          /* Select Between Source Mode and Sink Mode */
#define BITL_PGA_PGA3CON_TIASRCEN                             (1U)           /* Select Between Source Mode and Sink Mode */
#define BITM_PGA_PGA3CON_TIASRCEN                             (0X00008000U)  /* Select Between Source Mode and Sink Mode */

#define ENUM_PGA_PGA3CON_TIASRCEN_SINKMD                      (0X00000000U)  /* Sink Mode */
#define ENUM_PGA_PGA3CON_TIASRCEN_SOURCEMD                    (0X00000001U)  /* Source Mode */
#define ENUM_PGA_PGA3CON_DRVEN_DRVDISABLE                     (0X00000000U)  /* Disable Isink Ability Improvement, Normal Drive */
#define ENUM_PGA_PGA3CON_DRVEN_DRVENABLE                      (0X00000001U)  /* Enable Isink Ability Improvement */
#define ENUM_PGA_PGA3CON_TIAVDACSEL_VDAC8SEL                  (0X00000000U)  /* Select VDAC8 as TIA Vbias */
#define ENUM_PGA_PGA3CON_TIAVDACSEL_VDAC9SEL                  (0X00000001U)  /* Select VDAC9 as TIA Vbias */
#define ENUM_PGA_PGA3CON_TIAVDACSEL_VDAC10SEL                 (0X00000002U)  /* Select VDAC10 as TIA Vbias */
#define ENUM_PGA_PGA3CON_TIAVDACSEL_VDAC11SEL                 (0X00000003U)  /* Select VDAC11 as TIA Vbias */
#define ENUM_PGA_PGA3CON_TIAGAIN_RES250                       (0X00000000U)  /* TIARES=250ohm */
#define ENUM_PGA_PGA3CON_TIAGAIN_RES750                       (0X00000001U)  /* TIARES=750ohm */
#define ENUM_PGA_PGA3CON_TIAGAIN_RES2K                        (0X00000002U)  /* TIARES=2kohm */
#define ENUM_PGA_PGA3CON_TIAGAIN_RES5K                        (0X00000003U)  /* TIARES=5kohm */
#define ENUM_PGA_PGA3CON_TIAGAIN_RES10K                       (0X00000004U)  /* TIARES=10kohm */
#define ENUM_PGA_PGA3CON_TIAGAIN_RES20K                       (0X00000005U)  /* TIARES=20kohm */
#define ENUM_PGA_PGA3CON_TIAGAIN_RES100K                      (0X00000006U)  /* TIARES=100kohm */
#define ENUM_PGA_PGA3CON_PGAGAIN_GAIN1                        (0X00000000U)  /* Gain=1 */
#define ENUM_PGA_PGA3CON_PGAGAIN_GAIN2                        (0X00000001U)  /* GAIN=2 */
#define ENUM_PGA_PGA3CON_PGAGAIN_GAIN4                        (0X00000002U)  /* GAIN=4 */
#define ENUM_PGA_PGA3CON_PGAGAIN_GAIN6                        (0X00000003U)  /* GAIN=6 */
#define ENUM_PGA_PGA3CON_PGAGAIN_GAIN8                        (0X00000004U)  /* GAIN=8 */
#define ENUM_PGA_PGA3CON_PGAGAIN_GAIN10                       (0X00000005U)  /* GAIN=10 */
#define ENUM_PGA_PGA3CON_PGAMODE_DCCOUPLE                     (0X00000000U)  /* PGA DC Mode Enable */
#define ENUM_PGA_PGA3CON_PGAMODE_ACCOUPLE                     (0X00000001U)  /* PGA AC Coupling Mode Enable */
#define ENUM_PGA_PGA3CON_MODE_PGAMODE                         (0X00000000U)  /* PGA Mode Enable */
#define ENUM_PGA_PGA3CON_MODE_TIAMODE                         (0X00000001U)  /* TIA Mode Enable */

#endif  /* end ifndef PGA_ADDR_RDEF_H_ */


#ifndef PLL_MMRS_ADDR_RDEF_H_
#define PLL_MMRS_ADDR_RDEF_H_    /* PLL_MMRS: Your module description, here. */

/* ====================================================================================================
        PLL_MMRS Module Instances Address and Mask Definitions
   ==================================================================================================== */
#define INST_PLL                                                 (0X40069200U)    /* pll: */

#define MASK_PLL_MMRS                                            (0X000001FFU)    /* PLL_MMRS: Your module description, here. */

/* ====================================================================================================
        PLL_MMRS Module Register Address Offset Definitions
   ==================================================================================================== */
#define IDX_PLL_MMRS_PLLPDCTRL                                  (0X000U)    /* No description provided */

/* ====================================================================================================
        PLL_MMRS Module Register ResetValue Definitions
   ==================================================================================================== */
#define RSTVAL_PLL_MMRS_PLLPDCTRL                                  (0X3F) 

/* ====================================================================================================
        PLL_MMRS Module Register BitPositions, Lengths, Masks and Enumerations Definitions
   ==================================================================================================== */

/* ----------------------------------------------------------------------------------------------------
          PLLPDCTRL                                             Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_PLL_MMRS_PLLPDCTRL_TOTPDB                        (6U)           /* PDB of PLL All */
#define BITL_PLL_MMRS_PLLPDCTRL_TOTPDB                        (1U)           /* PDB of PLL All */
#define BITM_PLL_MMRS_PLLPDCTRL_TOTPDB                        (0X00000040U)  /* PDB of PLL All */

#endif  /* end ifndef PLL_MMRS_ADDR_RDEF_H_ */


#ifndef VCM_MMRS_ADDR_RDEF_H_
#define VCM_MMRS_ADDR_RDEF_H_    /* VCM_MMRS: Your module description, here. */

/* ====================================================================================================
        VCM_MMRS Module Instances Address and Mask Definitions
   ==================================================================================================== */
#define INST_TMPSNS                                              (0X40069600U)    /* tmpsns: */

#define MASK_VCM_MMRS                                            (0X000001FFU)    /* VCM_MMRS: Your module description, here. */

/* ====================================================================================================
        VCM_MMRS Module Register Address Offset Definitions
   ==================================================================================================== */
#define IDX_VCM_MMRS_TMPSNSCON                                  (0X000U)    /* No description provided */
#define IDX_VCM_MMRS_TMPSNSCHPCON                               (0X004U)    /* No description provided */
#define IDX_VCM_MMRS_VCMREFCON                                  (0X008U)    /* No description provided */
#define IDX_VCM_MMRS_VCMBUFCON                                  (0X010U)    /* No description provided */

/* ====================================================================================================
        VCM_MMRS Module Register ResetValue Definitions
   ==================================================================================================== */
#define RSTVAL_VCM_MMRS_TMPSNSCON                                  (0X2) 
#define RSTVAL_VCM_MMRS_TMPSNSCHPCON                               (0X0) 
#define RSTVAL_VCM_MMRS_VCMREFCON                                  (0X0) 
#define RSTVAL_VCM_MMRS_VCMBUFCON                                  (0X13) 

/* ====================================================================================================
        VCM_MMRS Module Register BitPositions, Lengths, Masks and Enumerations Definitions
   ==================================================================================================== */

/* ----------------------------------------------------------------------------------------------------
          TMPSNSCON                                             Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_VCM_MMRS_TMPSNSCON_ENTMPSNS                      (1U)           /* Enable Temp Sensor */
#define BITL_VCM_MMRS_TMPSNSCON_ENTMPSNS                      (1U)           /* Enable Temp Sensor */
#define BITM_VCM_MMRS_TMPSNSCON_ENTMPSNS                      (0X00000002U)  /* Enable Temp Sensor */

/* ----------------------------------------------------------------------------------------------------
          TMPSNSCHPCON                                          Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_VCM_MMRS_TMPSNSCHPCON_CHOFFTMPSNS                (0U)           /* Chopping Disable = 1 Enable = 0 */
#define BITL_VCM_MMRS_TMPSNSCHPCON_CHOFFTMPSNS                (1U)           /* Chopping Disable = 1 Enable = 0 */
#define BITM_VCM_MMRS_TMPSNSCHPCON_CHOFFTMPSNS                (0X00000001U)  /* Chopping Disable = 1 Enable = 0 */

/* ----------------------------------------------------------------------------------------------------
          VCMREFCON                                             Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_VCM_MMRS_VCMREFCON_PGAREFSEL                     (1U)           /* 0.2V or 0.5V Selection for PGA */
#define BITL_VCM_MMRS_VCMREFCON_PGAREFSEL                     (1U)           /* 0.2V or 0.5V Selection for PGA */
#define BITM_VCM_MMRS_VCMREFCON_PGAREFSEL                     (0X00000002U)  /* 0.2V or 0.5V Selection for PGA */

#define ENUM_VCM_MMRS_VCMREFCON_PGAREFSEL_SEL0P2V             (0X00000000U)  /* Select 0.2V Reference */
#define ENUM_VCM_MMRS_VCMREFCON_PGAREFSEL_SEL0P5V             (0X00000001U)  /* Select 0.5V Reference */

/* ----------------------------------------------------------------------------------------------------
          VCMBUFCON                                             Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_VCM_MMRS_VCMBUFCON_PDBUF0                        (0U)           /* Power Down Unit Gain Buffer0 */
#define BITL_VCM_MMRS_VCMBUFCON_PDBUF0                        (1U)           /* Power Down Unit Gain Buffer0 */
#define BITM_VCM_MMRS_VCMBUFCON_PDBUF0                        (0X00000001U)  /* Power Down Unit Gain Buffer0 */
#define BITP_VCM_MMRS_VCMBUFCON_PDBUF1                        (1U)           /* Power Down Unit Gain Buffer1 */
#define BITL_VCM_MMRS_VCMBUFCON_PDBUF1                        (1U)           /* Power Down Unit Gain Buffer1 */
#define BITM_VCM_MMRS_VCMBUFCON_PDBUF1                        (0X00000002U)  /* Power Down Unit Gain Buffer1 */
#define BITP_VCM_MMRS_VCMBUFCON_MUXSEL0                       (2U)           /* Selection 0=2.5V 1=1.25V */
#define BITL_VCM_MMRS_VCMBUFCON_MUXSEL0                       (1U)           /* Selection 0=2.5V 1=1.25V */
#define BITM_VCM_MMRS_VCMBUFCON_MUXSEL0                       (0X00000004U)  /* Selection 0=2.5V 1=1.25V */
#define BITP_VCM_MMRS_VCMBUFCON_MUXSEL1                       (4U)           /* Selection 0=2.5V 1=1.25V */
#define BITL_VCM_MMRS_VCMBUFCON_MUXSEL1                       (1U)           /* Selection 0=2.5V 1=1.25V */
#define BITM_VCM_MMRS_VCMBUFCON_MUXSEL1                       (0X00000010U)  /* Selection 0=2.5V 1=1.25V */

#define ENUM_VCM_MMRS_VCMBUFCON_MUXSEL1_BUF1_2P5              (0X00000000U)  /* Buffer1 Input 2.5V */
#define ENUM_VCM_MMRS_VCMBUFCON_MUXSEL1_BUF1_1P25             (0X00000001U)  /* Buffer1 Input 1.25V */
#define ENUM_VCM_MMRS_VCMBUFCON_MUXSEL0_BUF0_2P5              (0X00000000U)  /* Input Buffer0 2.5V */
#define ENUM_VCM_MMRS_VCMBUFCON_MUXSEL0_BUF0_1P25             (0X00000001U)  /* Input Buffer0 1.25V */

#endif  /* end ifndef VCM_MMRS_ADDR_RDEF_H_ */


#ifndef DAC_ADDR_RDEF_H_
#define DAC_ADDR_RDEF_H_    /* DAC: Your module description, here. */

/* ====================================================================================================
        DAC Module Instances Address and Mask Definitions
   ==================================================================================================== */
#define INST_VDAC                                                (0X40069800U)    /* vdac: */

#define MASK_DAC                                                 (0X000001FFU)    /* DAC: Your module description, here. */

/* ====================================================================================================
        DAC Module Register Address Offset Definitions
   ==================================================================================================== */
#define IDX_DAC_DACCON0                                         (0X000U)    /* DAC Control Register */
#define IDX_DAC_DACCON1                                         (0X004U)    /* DAC Control Register */
#define IDX_DAC_DACCON2                                         (0X008U)    /* DAC Control Register */
#define IDX_DAC_DACCON3                                         (0X00CU)    /* DAC Control Register */
#define IDX_DAC_DACCON4                                         (0X010U)    /* DAC Control Register */
#define IDX_DAC_DACCON5                                         (0X014U)    /* DAC Control Register */
#define IDX_DAC_DACCON6                                         (0X018U)    /* DAC Control Register */
#define IDX_DAC_DACCON7                                         (0X01CU)    /* DAC Control Register */
#define IDX_DAC_DACCON8                                         (0X020U)    /* DAC Control Register */
#define IDX_DAC_DACCON9                                         (0X024U)    /* DAC Control Register */
#define IDX_DAC_DACCON10                                        (0X028U)    /* DAC Control Register */
#define IDX_DAC_DACCON11                                        (0X02CU)    /* DAC Control Register */
#define IDX_DAC_DACDAT0                                         (0X030U)    /* DAC Data Register */
#define IDX_DAC_DACDAT1                                         (0X034U)    /* DAC Data Register */
#define IDX_DAC_DACDAT2                                         (0X038U)    /* DAC Data Register */
#define IDX_DAC_DACDAT3                                         (0X03CU)    /* DAC Data Register */
#define IDX_DAC_DACDAT4                                         (0X040U)    /* DAC Data Register */
#define IDX_DAC_DACDAT5                                         (0X044U)    /* DAC Data Register */
#define IDX_DAC_DACDAT6                                         (0X048U)    /* DAC Data Register */
#define IDX_DAC_DACDAT7                                         (0X04CU)    /* DAC Data Register */
#define IDX_DAC_DACDAT8                                         (0X050U)    /* DAC Data Register */
#define IDX_DAC_DACDAT9                                         (0X054U)    /* DAC Data Register */
#define IDX_DAC_DACDAT10                                        (0X058U)    /* DAC Data Register */
#define IDX_DAC_DACDAT11                                        (0X05CU)    /* DAC Data Register */

/* ====================================================================================================
        DAC Module Register ResetValue Definitions
   ==================================================================================================== */
#define RSTVAL_DAC_DACCON0                                         (0XD02) 
#define RSTVAL_DAC_DACCON1                                         (0XD02) 
#define RSTVAL_DAC_DACCON2                                         (0XD02) 
#define RSTVAL_DAC_DACCON3                                         (0XD02) 
#define RSTVAL_DAC_DACCON4                                         (0XD02) 
#define RSTVAL_DAC_DACCON5                                         (0XD02) 
#define RSTVAL_DAC_DACCON6                                         (0XD02) 
#define RSTVAL_DAC_DACCON7                                         (0XD02) 
#define RSTVAL_DAC_DACCON8                                         (0XD02) 
#define RSTVAL_DAC_DACCON9                                         (0XD02) 
#define RSTVAL_DAC_DACCON10                                        (0XD02) 
#define RSTVAL_DAC_DACCON11                                        (0XD02) 
#define RSTVAL_DAC_DACDAT0                                         (0X0) 
#define RSTVAL_DAC_DACDAT1                                         (0X0) 
#define RSTVAL_DAC_DACDAT2                                         (0X0) 
#define RSTVAL_DAC_DACDAT3                                         (0X0) 
#define RSTVAL_DAC_DACDAT4                                         (0X0) 
#define RSTVAL_DAC_DACDAT5                                         (0X0) 
#define RSTVAL_DAC_DACDAT6                                         (0X0) 
#define RSTVAL_DAC_DACDAT7                                         (0X0) 
#define RSTVAL_DAC_DACDAT8                                         (0X0) 
#define RSTVAL_DAC_DACDAT9                                         (0X0) 
#define RSTVAL_DAC_DACDAT10                                        (0X0) 
#define RSTVAL_DAC_DACDAT11                                        (0X0) 

/* ====================================================================================================
        DAC Module Register BitPositions, Lengths, Masks and Enumerations Definitions
   ==================================================================================================== */

/* ----------------------------------------------------------------------------------------------------
          DACCON0                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_DAC_DACCON_N__EN                                 (4U)           /* DAC Input Data Clear, Won't Be Controlled by Sync Mode. */
#define BITL_DAC_DACCON_N__EN                                 (1U)           /* DAC Input Data Clear, Won't Be Controlled by Sync Mode. */
#define BITM_DAC_DACCON_N__EN                                 (0X00000010U)  /* DAC Input Data Clear, Won't Be Controlled by Sync Mode. */
#define BITP_DAC_DACCON_N__FSLVL                              (5U)           /* Select Output Full Scale */
#define BITL_DAC_DACCON_N__FSLVL                              (1U)           /* Select Output Full Scale */
#define BITM_DAC_DACCON_N__FSLVL                              (0X00000020U)  /* Select Output Full Scale */
#define BITP_DAC_DACCON_N__PD                                 (8U)           /* DAC Top Power Down */
#define BITL_DAC_DACCON_N__PD                                 (1U)           /* DAC Top Power Down */
#define BITM_DAC_DACCON_N__PD                                 (0X00000100U)  /* DAC Top Power Down */
#define BITP_DAC_DACCON_N__DRV                                (9U)           /* Drive Ability Boost Enable, Can Drive 10mA Load. */
#define BITL_DAC_DACCON_N__DRV                                (1U)           /* Drive Ability Boost Enable, Can Drive 10mA Load. */
#define BITM_DAC_DACCON_N__DRV                                (0X00000200U)  /* Drive Ability Boost Enable, Can Drive 10mA Load. */

#define ENUM_DAC_DACCON_N__DRV_NORMAL                         (0X00000000U)  /* Normal Work Mode */
#define ENUM_DAC_DACCON_N__DRV_DRIVEBOOST                     (0X00000001U)  /* Drive Ability Boost Mode */
#define ENUM_DAC_DACCON_N__PD_DACON                           (0X00000000U)  /* DAC Top Enable */
#define ENUM_DAC_DACCON_N__PD_DACOFF                          (0X00000001U)  /* DAC TOP Power Down */
#define ENUM_DAC_DACCON_N__FSLVL_OUT2P5                       (0X00000000U)  /* Full Scale is 2.5V */
#define ENUM_DAC_DACCON_N__FSLVL_OUT3V3                       (0X00000001U)  /* Full Scale is 3.3V */
#define ENUM_DAC_DACCON_N__EN_CLEAR                           (0X00000000U)  /* DAC DATA Clear */
#define ENUM_DAC_DACCON_N__EN_CLEAROFF                        (0X00000001U)  /* DAC DATA Normal Input */

/* ----------------------------------------------------------------------------------------------------
          DACDAT0                                               Value             Description
   ---------------------------------------------------------------------------------------------------- */
#define BITP_DAC_DACDAT_N__DATAIN                             (12U)          /* DAC Input Data */
#define BITL_DAC_DACDAT_N__DATAIN                             (16U)          /* DAC Input Data */
#define BITM_DAC_DACDAT_N__DATAIN                             (0X0FFFF000U)  /* DAC Input Data */

#endif  /* end ifndef DAC_ADDR_RDEF_H_ */

/*! ========================================================================
 *  \struct ADI_TMR_TypeDef
 *  \brief GPT0
 *  ======================================================================== */
typedef struct _ADI_TMR_TypeDef
{
    volatile       uint16_t LD;                              /**< 0 16-bit Load Value Register */
    volatile const uint8_t RESERVED0[2];                   
    volatile const uint16_t VAL;                             /**< 4 16-bit Timer Value Register */
    volatile const uint8_t RESERVED1[2];                   
    volatile       uint16_t CON;                             /**< 8 Control Register */
    volatile const uint8_t RESERVED2[2];                   
    volatile       uint16_t CLRI;                            /**< C Clear Interrupt Register */
    volatile const uint8_t RESERVED3[14];                  
    volatile const uint16_t STA;                             /**< 1C Status Register */
} ADI_TMR_TypeDef;

/*! ========================================================================
 *  \struct ADI_TIMER_TypeDef
 *  \brief Timer
 *  ======================================================================== */
typedef struct _ADI_TIMER_TypeDef
{
    volatile       uint32_t CTL;                             /**< 0 Timer Control */
    volatile       uint32_t CNT;                             /**< 4 Count Value */
    volatile       uint32_t STATUS;                          /**< 8 Timer Status */
    volatile const uint8_t RESERVED0[4];                   
    volatile       uint32_t CFG0;                            /**< 10 Capture Compare Configuration */
    volatile       uint32_t CFG1;                            /**< 14 Capture Compare Configuration */
    volatile       uint32_t CFG2;                            /**< 18 Capture Compare Configuration */
    volatile       uint32_t CFG3;                            /**< 1C Capture Compare Configuration */
    volatile       uint32_t CC0;                             /**< 20 Compare and Capture Value */
    volatile       uint32_t CC1;                             /**< 24 Compare and Capture Value */
    volatile       uint32_t CC2;                             /**< 28 Compare and Capture Value */
    volatile       uint32_t CC3;                             /**< 2C Compare and Capture Value */
} ADI_TIMER_TypeDef;

/*! ========================================================================
 *  \struct ADI_MISC16_TypeDef
 *  \brief Your module description, here.
 *  ======================================================================== */
typedef struct _ADI_MISC16_TypeDef
{
    volatile const uint8_t RESERVED0[32];                  
    volatile const uint32_t ADIID;                           /**< 20 ADI ID */
    volatile const uint32_t CHIPID;                          /**< 24 Chip ID */
    volatile const uint8_t RESERVED1[268];                 
    volatile       uint32_t USERKEY;                         /**< 134 Open to Customer to Protect Important Registers */
} ADI_MISC16_TypeDef;

/*! ========================================================================
 *  \struct ADI_WUT_TypeDef
 *  \brief WUT
 *  ======================================================================== */
typedef struct _ADI_WUT_TypeDef
{
    volatile const uint16_t T4VAL0;                          /**< 0 Current Count Value - LS 16 Bits */
    volatile const uint8_t RESERVED0[2];                   
    volatile const uint16_t T4VAL1;                          /**< 4 Current Count Value - MS 16 Bits */
    volatile const uint8_t RESERVED1[2];                   
    volatile       uint16_t T4CON;                           /**< 8 Control Register */
    volatile const uint8_t RESERVED2[2];                   
    volatile       uint16_t T4INC;                           /**< C 12-bit Interval for Wakeup Field a */
    volatile const uint8_t RESERVED3[2];                   
    volatile       uint16_t T4WUFB0;                         /**< 10 Wakeup Field B - LS 16 Bits */
    volatile const uint8_t RESERVED4[2];                   
    volatile       uint16_t T4WUFB1;                         /**< 14 Wakeup Field B - MS 16 Bits */
    volatile const uint8_t RESERVED5[2];                   
    volatile       uint16_t T4WUFC0;                         /**< 18 Wakeup Field C - LS 16 Bits */
    volatile const uint8_t RESERVED6[2];                   
    volatile       uint16_t T4WUFC1;                         /**< 1C Wakeup Field C - MS 16 Bits */
    volatile const uint8_t RESERVED7[2];                   
    volatile       uint16_t T4WUFD0;                         /**< 20 Wakeup Field D - LS 16 Bits */
    volatile const uint8_t RESERVED8[2];                   
    volatile       uint16_t T4WUFD1;                         /**< 24 Wakeup Field D - MS 16 Bits */
    volatile const uint8_t RESERVED9[2];                   
    volatile       uint16_t T4IEN;                           /**< 28 Interrupt Enable Register */
    volatile const uint8_t RESERVED10[2];                  
    volatile const uint16_t T4STA;                           /**< 2C Status Register */
    volatile const uint8_t RESERVED11[2];                  
    volatile       uint16_t T4CLRI;                          /**< 30 Clear Interrupt Register */
    volatile const uint8_t RESERVED12[10];                 
    volatile       uint16_t T4WUFA0;                         /**< 3C Wakeup Field a - LS 16 Bits */
    volatile const uint8_t RESERVED13[2];                  
    volatile       uint16_t T4WUFA1;                         /**< 40 Wakeup Field a - MS 16 Bits */
} ADI_WUT_TypeDef;

/*! ========================================================================
 *  \struct ADI_WDT_TypeDef
 *  \brief Watchdog Timer Register Map
 *  ======================================================================== */
typedef struct _ADI_WDT_TypeDef
{
    volatile       uint16_t LD;                              /**< 0 Watchdog Timer Load Value */
    volatile const uint8_t RESERVED0[2];                   
    volatile const uint16_t VALS;                            /**< 4 Current Count Value */
    volatile const uint8_t RESERVED1[2];                   
    volatile       uint16_t CON;                             /**< 8 Watchdog Timer Control Register */
    volatile const uint8_t RESERVED2[2];                   
    volatile       uint16_t CLRI;                            /**< C Refresh Watchdog Register */
    volatile const uint8_t RESERVED3[10];                  
    volatile const uint16_t STA;                             /**< 18 Timer Status */
    volatile const uint8_t RESERVED4[2];                   
    volatile       uint16_t MINLD;                           /**< 1C Minimum Load Value */
} ADI_WDT_TypeDef;

/*! ========================================================================
 *  \struct ADI_ALWAYS_ON_TypeDef
 *  \brief Always on clock, reset, power management and test
 *  ======================================================================== */
typedef struct _ADI_ALWAYS_ON_TypeDef
{
    volatile       uint16_t PWRMOD;                          /**< 0 Power Modes */
    volatile const uint8_t RESERVED0[2];                   
    volatile       uint16_t PWRKEY;                          /**< 4 Key Protection for PWRMOD */
    volatile const uint8_t RESERVED1[2];                   
    volatile       uint16_t RSTCFG;                          /**< 8 Reset Configuration */
    volatile const uint8_t RESERVED2[2];                   
    volatile       uint16_t RSTKEY;                          /**< C Key Protection for RSTCFG */
    volatile const uint8_t RESERVED3[18];                  
    volatile       uint16_t EI0CFG;                          /**< 20 External Interrupt Configuration 0 */
    volatile const uint8_t RESERVED4[2];                   
    volatile       uint16_t EI1CFG;                          /**< 24 External Interrupt Configuration 1 */
    volatile const uint8_t RESERVED5[2];                   
    volatile       uint16_t EI2CFG;                          /**< 28 External Interrupt Configuration 2 */
    volatile const uint8_t RESERVED6[6];                   
    volatile       uint16_t EICLR;                           /**< 30 External Interrupt Clear */
    volatile const uint8_t RESERVED7[14];                  
    volatile       uint16_t RSTSTA;                          /**< 40 Reset Status */
} ADI_ALWAYS_ON_TypeDef;

/*! ========================================================================
 *  \struct ADI_PLA_TypeDef
 *  \brief PLA Register Map
 *  ======================================================================== */
typedef struct _ADI_PLA_TypeDef
{
    volatile       uint16_t PLA_ELEM0;                       /**< 0 ELEMx Configuration Register */
    volatile const uint8_t RESERVED0[2];                   
    volatile       uint16_t PLA_ELEM1;                       /**< 4 ELEMx Configuration Register */
    volatile const uint8_t RESERVED1[2];                   
    volatile       uint16_t PLA_ELEM2;                       /**< 8 ELEMx Configuration Register */
    volatile const uint8_t RESERVED2[2];                   
    volatile       uint16_t PLA_ELEM3;                       /**< C ELEMx Configuration Register */
    volatile const uint8_t RESERVED3[2];                   
    volatile       uint16_t PLA_ELEM4;                       /**< 10 ELEMx Configuration Register */
    volatile const uint8_t RESERVED4[2];                   
    volatile       uint16_t PLA_ELEM5;                       /**< 14 ELEMx Configuration Register */
    volatile const uint8_t RESERVED5[2];                   
    volatile       uint16_t PLA_ELEM6;                       /**< 18 ELEMx Configuration Register */
    volatile const uint8_t RESERVED6[2];                   
    volatile       uint16_t PLA_ELEM7;                       /**< 1C ELEMx Configuration Register */
    volatile const uint8_t RESERVED7[2];                   
    volatile       uint16_t PLA_ELEM8;                       /**< 20 ELEMx Configuration Register */
    volatile const uint8_t RESERVED8[2];                   
    volatile       uint16_t PLA_ELEM9;                       /**< 24 ELEMx Configuration Register */
    volatile const uint8_t RESERVED9[2];                   
    volatile       uint16_t PLA_ELEM10;                      /**< 28 ELEMx Configuration Register */
    volatile const uint8_t RESERVED10[2];                  
    volatile       uint16_t PLA_ELEM11;                      /**< 2C ELEMx Configuration Register */
    volatile const uint8_t RESERVED11[2];                  
    volatile       uint16_t PLA_ELEM12;                      /**< 30 ELEMx Configuration Register */
    volatile const uint8_t RESERVED12[2];                  
    volatile       uint16_t PLA_ELEM13;                      /**< 34 ELEMx Configuration Register */
    volatile const uint8_t RESERVED13[2];                  
    volatile       uint16_t PLA_ELEM14;                      /**< 38 ELEMx Configuration Register */
    volatile const uint8_t RESERVED14[2];                  
    volatile       uint16_t PLA_ELEM15;                      /**< 3C ELEMx Configuration Register */
    volatile const uint8_t RESERVED15[2];                  
    volatile       uint16_t PLA_ELEM16;                      /**< 40 ELEMx Configuration Register */
    volatile const uint8_t RESERVED16[2];                  
    volatile       uint16_t PLA_ELEM17;                      /**< 44 ELEMx Configuration Register */
    volatile const uint8_t RESERVED17[2];                  
    volatile       uint16_t PLA_ELEM18;                      /**< 48 ELEMx Configuration Register */
    volatile const uint8_t RESERVED18[2];                  
    volatile       uint16_t PLA_ELEM19;                      /**< 4C ELEMx Configuration Register */
    volatile const uint8_t RESERVED19[2];                  
    volatile       uint16_t PLA_ELEM20;                      /**< 50 ELEMx Configuration Register */
    volatile const uint8_t RESERVED20[2];                  
    volatile       uint16_t PLA_ELEM21;                      /**< 54 ELEMx Configuration Register */
    volatile const uint8_t RESERVED21[2];                  
    volatile       uint16_t PLA_ELEM22;                      /**< 58 ELEMx Configuration Register */
    volatile const uint8_t RESERVED22[2];                  
    volatile       uint16_t PLA_ELEM23;                      /**< 5C ELEMx Configuration Register */
    volatile const uint8_t RESERVED23[2];                  
    volatile       uint16_t PLA_ELEM24;                      /**< 60 ELEMx Configuration Register */
    volatile const uint8_t RESERVED24[2];                  
    volatile       uint16_t PLA_ELEM25;                      /**< 64 ELEMx Configuration Register */
    volatile const uint8_t RESERVED25[2];                  
    volatile       uint16_t PLA_ELEM26;                      /**< 68 ELEMx Configuration Register */
    volatile const uint8_t RESERVED26[2];                  
    volatile       uint16_t PLA_ELEM27;                      /**< 6C ELEMx Configuration Register */
    volatile const uint8_t RESERVED27[2];                  
    volatile       uint16_t PLA_ELEM28;                      /**< 70 ELEMx Configuration Register */
    volatile const uint8_t RESERVED28[2];                  
    volatile       uint16_t PLA_ELEM29;                      /**< 74 ELEMx Configuration Register */
    volatile const uint8_t RESERVED29[2];                  
    volatile       uint16_t PLA_ELEM30;                      /**< 78 ELEMx Configuration Register */
    volatile const uint8_t RESERVED30[2];                  
    volatile       uint16_t PLA_ELEM31;                      /**< 7C ELEMx Configuration Register */
    volatile const uint8_t RESERVED31[2];                  
    volatile       uint16_t PLA_CLK;                         /**< 80 PLA Clock Select */
    volatile const uint8_t RESERVED32[2];                  
    volatile       uint16_t PLA_IRQ0;                        /**< 84 Interrupt Register for Block 0 */
    volatile const uint8_t RESERVED33[2];                  
    volatile       uint16_t PLA_IRQ1;                        /**< 88 Interrupt Register for Block1 */
    volatile const uint8_t RESERVED34[2];                  
    volatile       uint16_t PLA_ADC;                         /**< 8C ADC Configuration Register */
    volatile const uint8_t RESERVED35[2];                  
    volatile       int16_t PLA_DIN0;                        /**< 90 AMBA Bus Data Input for Block 0 */
    volatile const uint8_t RESERVED36[6];                  
    volatile const uint16_t PLA_DOUT0;                       /**< 98 AMBA Bus Data Output for Block 0 */
    volatile const uint8_t RESERVED37[2];                  
    volatile const uint16_t PLA_DOUT1;                       /**< 9C AMBA Bus Data Output for Block1 */
    volatile const uint8_t RESERVED38[2];                  
    volatile       uint16_t PLA_LCK;                         /**< A0 Write Lock Register. */
    volatile const uint8_t RESERVED39[2];                  
    volatile       uint16_t PLA_IRQTYPE;                     /**< A4 PLA Interrupt Request and DMA Request Type */
} ADI_PLA_TypeDef;

/*! ========================================================================
 *  \struct ADI_DMAREQ_TypeDef
 *  \brief Your module description, here.
 *  ======================================================================== */
typedef struct _ADI_DMAREQ_TypeDef
{
    volatile       uint8_t REQEN;                           /**< 0 GPT/GPT32 and PLA DMA Request Enable */
    volatile const uint8_t RESERVED0[3];                   
    volatile       uint8_t REQ0SEL;                         /**< 4 GPT/GPT32 and PLA DMA Request 0 Select */
    volatile const uint8_t RESERVED1[3];                   
    volatile       uint8_t REQ1SEL;                         /**< 8 GPT/GPT32 and PLA DMA Request 1 Select */
    volatile const uint8_t RESERVED2[3];                   
    volatile       uint8_t PLAREQEN;                        /**< C PLA DMA Requests Enable */
    volatile const uint8_t RESERVED3[3];                   
    volatile       uint8_t GPTREQEN;                        /**< 10 GPT/GPT32 DMA Requests Enable */
    volatile const uint8_t RESERVED4[3];                   
    volatile       uint16_t GPT_MDA_REQ_TTYPE;               /**< 14 GPT and GPT32 Require Type */
} ADI_DMAREQ_TypeDef;

/*! ========================================================================
 *  \struct ADI_UART_TypeDef
 *  \brief Universal Asynchronous Receiver/Transmitter
 *  ======================================================================== */
typedef struct _ADI_UART_TypeDef
{
    union {
    volatile const uint16_t   RX;                         /*!< Receive Buffer Register */
    volatile       uint16_t   TX;                         /*!< Transmit Holding Register */
    };
    volatile const uint8_t RESERVED0[2];                   
    volatile       uint16_t IEN;                             /**< 4 Interrupt Enable */
    volatile const uint8_t RESERVED1[2];                   
    volatile const uint16_t IIR;                             /**< 8 Interrupt ID */
    volatile const uint8_t RESERVED2[2];                   
    volatile       uint16_t LCR;                             /**< C Line Control */
    volatile const uint8_t RESERVED3[2];                   
    volatile       uint16_t MCR;                             /**< 10 Modem Control */
    volatile const uint8_t RESERVED4[2];                   
    volatile const uint16_t LSR;                             /**< 14 Line Status */
    volatile const uint8_t RESERVED5[2];                   
    volatile const uint16_t MSR;                             /**< 18 Modem Status */
    volatile const uint8_t RESERVED6[2];                   
    volatile       uint16_t SCR;                             /**< 1C Scratch Buffer */
    volatile const uint8_t RESERVED7[2];                   
    volatile       uint16_t FCR;                             /**< 20 FIFO Control */
    volatile const uint8_t RESERVED8[2];                   
    volatile       uint16_t FBR;                             /**< 24 Fractional Baud Rate */
    volatile const uint8_t RESERVED9[2];                   
    volatile       uint16_t DIV;                             /**< 28 Baudrate Divider */
    volatile const uint8_t RESERVED10[2];                  
    volatile       uint16_t LCR2;                            /**< 2C Second Line Control */
    volatile const uint8_t RESERVED11[2];                  
    volatile       uint16_t CTL;                             /**< 30 UART Control Register */
    volatile const uint8_t RESERVED12[2];                  
    volatile const uint16_t RFC;                             /**< 34 RX FIFO Byte Count */
    volatile const uint8_t RESERVED13[2];                  
    volatile const uint16_t TFC;                             /**< 38 TX FIFO Byte Count */
    volatile const uint8_t RESERVED14[2];                  
    volatile       uint16_t RSC;                             /**< 3C RS485 Half-duplex Control */
    volatile const uint8_t RESERVED15[2];                  
    volatile       uint16_t ACR;                             /**< 40 Auto Baud Control */
    volatile const uint8_t RESERVED16[2];                  
    volatile const uint16_t ASRL;                            /**< 44 Auto Baud Status (Low) */
    volatile const uint8_t RESERVED17[2];                  
    volatile const uint16_t ASRH;                            /**< 48 Auto Baud Status (High) */
} ADI_UART_TypeDef;

/*! ========================================================================
 *  \struct ADI_I2C_TypeDef
 *  \brief I2C Master/Slave
 *  ======================================================================== */
typedef struct _ADI_I2C_TypeDef
{
    volatile       uint16_t MCTL;                            /**< 0 Master Control */
    volatile const uint8_t RESERVED0[2];                   
    volatile       uint16_t MSTAT;                           /**< 4 Master Status */
    volatile const uint8_t RESERVED1[2];                   
    volatile const uint16_t MRX;                             /**< 8 Master Receive Data */
    volatile const uint8_t RESERVED2[2];                   
    volatile       uint16_t MTX;                             /**< C Master Transmit Data */
    volatile const uint8_t RESERVED3[2];                   
    volatile       uint16_t MRXCNT;                          /**< 10 Master Receive Data Count */
    volatile const uint8_t RESERVED4[2];                   
    volatile const uint16_t MCRXCNT;                         /**< 14 Master Current Receive Data Count */
    volatile const uint8_t RESERVED5[2];                   
    volatile       uint16_t ADDR0;                           /**< 18 1st Master Address Byte */
    volatile const uint8_t RESERVED6[2];                   
    volatile       uint16_t ADDR1;                           /**< 1C 2nd Master Address Byte */
    volatile const uint8_t RESERVED7[2];                   
    volatile       uint16_t BYT;                             /**< 20 Start Byte */
    volatile const uint8_t RESERVED8[2];                   
    volatile       uint16_t DIV;                             /**< 24 Serial Clock Period Divisor */
    volatile const uint8_t RESERVED9[2];                   
    volatile       uint16_t SCTL;                            /**< 28 Slave Control */
    volatile const uint8_t RESERVED10[2];                  
    volatile       uint16_t SSTAT;                           /**< 2C Slave I2C Status/Error/IRQ */
    volatile const uint8_t RESERVED11[2];                  
    volatile const uint16_t SRX;                             /**< 30 Slave Receive */
    volatile const uint8_t RESERVED12[2];                  
    volatile       uint16_t STX;                             /**< 34 Slave Transmit */
    volatile const uint8_t RESERVED13[2];                  
    volatile       uint16_t ALT;                             /**< 38 Hardware General Call ID */
    volatile const uint8_t RESERVED14[2];                  
    volatile       uint16_t ID0;                             /**< 3C 1st Slave Address Device ID */
    volatile const uint8_t RESERVED15[2];                  
    volatile       uint16_t ID1;                             /**< 40 2nd Slave Address Device ID */
    volatile const uint8_t RESERVED16[2];                  
    volatile       uint16_t ID2;                             /**< 44 3rd Slave Address Device ID */
    volatile const uint8_t RESERVED17[2];                  
    volatile       uint16_t ID3;                             /**< 48 4th Slave Address Device ID */
    volatile const uint8_t RESERVED18[2];                  
    volatile       uint16_t STAT;                            /**< 4C Master and Slave FIFO Status */
    volatile const uint8_t RESERVED19[2];                  
    volatile       uint16_t SHCTL;                           /**< 50 Shared Control */
    volatile const uint8_t RESERVED20[2];                  
    volatile       uint16_t TCTL;                            /**< 54 Timing Control Register */
    volatile const uint8_t RESERVED21[2];                  
    volatile       uint16_t ASTRETCH_SCL;                    /**< 58 Automatic Stretch SCL Register */
    volatile const uint8_t RESERVED22[2];                  
    volatile       uint16_t IDFSTA;                          /**< 5C ID FIFO Status Register */
    volatile const uint8_t RESERVED23[2];                  
    volatile       uint16_t SLV_ADDR1;                       /**< 60 Slave 10 Bits Address 1st Byte. */
    volatile const uint8_t RESERVED24[2];                  
    volatile       uint16_t SLV_ADDR2;                       /**< 64 Slave 10 Bits Address 2nd Byte. */
    volatile const uint8_t RESERVED25[2];                  
    volatile const uint16_t SSTAT2;                          /**< 68 Slave I2C Status/IRQ 2 */
} ADI_I2C_TypeDef;

/*! ========================================================================
 *  \struct ADI_MDIO_TypeDef
 *  \brief MDIO Interface
 *  ======================================================================== */
typedef struct _ADI_MDIO_TypeDef
{
    volatile       uint16_t MDCON;                           /**< 0 MDIO Block Control */
    volatile const uint8_t RESERVED0[2];                   
    volatile const uint16_t MDFRM;                           /**< 4 MDIO Received Frame Control Information */
    volatile const uint8_t RESERVED1[2];                   
    volatile const uint16_t MDRXD;                           /**< 8 MDIO Received Data */
    volatile const uint8_t RESERVED2[2];                   
    volatile const uint16_t MDADR;                           /**< C MDIO Received Address */
    volatile const uint8_t RESERVED3[2];                   
    volatile       uint16_t MDTXD;                           /**< 10 MDIO Data for Transmission */
    volatile const uint8_t RESERVED4[2];                   
    volatile       uint16_t MDPHY;                           /**< 14 MDIO PHYADDR Software Values and Selection and DEVADD */
    volatile const uint8_t RESERVED5[2];                   
    volatile const uint16_t MDSTA;                           /**< 18 MDIO Progress Signaling Through Frame */
    volatile const uint8_t RESERVED6[2];                   
    volatile       uint16_t MDIEN;                           /**< 1C MDIO Interrupt Enables */
    volatile const uint8_t RESERVED7[2];                   
    volatile const uint16_t MDPIN;                           /**< 20 MDIO Read PHYADDR Pins */
    volatile const uint8_t RESERVED8[6];                   
    volatile       uint16_t DMAEN;                           /**< 28 MDIO DMA Enable */
    volatile const uint8_t RESERVED9[2];                   
    volatile       uint16_t MDTESTCON;                       /**< 2C MDIO Test Controller Protected by Test Key */
} ADI_MDIO_TypeDef;

/*! ========================================================================
 *  \struct ADI_DMA_TypeDef
 *  \brief DMA
 *  ======================================================================== */
typedef struct _ADI_DMA_TypeDef
{
    volatile const uint32_t STAT;                            /**< 0 DMA Status */
    volatile       uint32_t CFG;                             /**< 4 DMA Configuration */
    volatile       uint32_t PDBPTR;                          /**< 8 DMA Channel Primary Control Data Base Pointer */
    volatile const uint32_t ADBPTR;                          /**< C DMA Channel Alternate Control Data Base Pointer */
    volatile const uint8_t RESERVED0[4];                   
    volatile       uint32_t SWREQ;                           /**< 14 DMA Channel Software Request */
    volatile const uint8_t RESERVED1[8];                   
    volatile       uint32_t RMSKSET;                         /**< 20 DMA Channel Request Mask Set */
    volatile       uint32_t RMSKCLR;                         /**< 24 DMA Channel Request Mask Clear */
    volatile       uint32_t ENSET;                           /**< 28 DMA Channel Enable Set */
    volatile       uint32_t ENCLR;                           /**< 2C DMA Channel Enable Clear */
    volatile       uint32_t ALTSET;                          /**< 30 DMA Channel Primary-alternate Set */
    volatile       uint32_t ALTCLR;                          /**< 34 DMA Channel Primary-alternate Clear */
    volatile       uint32_t PRISET;                          /**< 38 DMA Channel Priority Set */
    volatile       uint32_t PRICLR;                          /**< 3C DMA Channel Priority Clear */
    volatile const uint8_t RESERVED2[8];                   
    volatile       uint32_t ERRCHNLCLR;                      /**< 48 DMA per Channel Error Clear */
    volatile       uint32_t ERRCLR;                          /**< 4C DMA Bus Error Clear */
    volatile       uint32_t INVALIDDESCCLR;                  /**< 50 DMA per Channel Invalid Descriptor Clear */
    volatile const uint8_t RESERVED3[1964];                
    volatile       uint32_t BSSET;                           /**< 800 DMA Channel Bytes Swap Enable Set */
    volatile       uint32_t BSCLR;                           /**< 804 DMA Channel Bytes Swap Enable Clear */
    volatile const uint8_t RESERVED4[8];                   
    volatile       uint32_t SRCADDRSET;                      /**< 810 DMA Channel Source Address Decrement Enable Set */
    volatile       uint32_t SRCADDRCLR;                      /**< 814 DMA Channel Source Address Decrement Enable Clear */
    volatile       uint32_t DSTADDRSET;                      /**< 818 DMA Channel Destination Address Decrement Enable Set */
    volatile       uint32_t DSTADDRCLR;                      /**< 81C DMA Channel Destination Address Decrement Enable Clear */
    volatile const uint8_t RESERVED5[1984];                
    volatile const uint32_t REVID;                           /**< FE0 DMA Controller Revision ID */
} ADI_DMA_TypeDef;

/*! ========================================================================
 *  \struct ADI_CC_TypeDef
 *  \brief Cache Controller
 *  ======================================================================== */
typedef struct _ADI_CC_TypeDef
{
    volatile const uint32_t STAT;                            /**< 0 Cache Status Register */
    volatile       uint32_t SETUP;                           /**< 4 Cache Setup Register */
    volatile       uint32_t KEY;                             /**< 8 Cache Key Register */
    volatile       uint32_t PERFSETUP;                       /**< C Cache Performance Monitor Setup Register */
    volatile const uint32_t ACCESSCNTR;                      /**< 10 Cache Miss Counter */
    volatile const uint8_t RESERVED0[16];                  
    volatile       uint32_t MSTRSETUP;                       /**< 24 Cache Master Setup Register */
    volatile const uint8_t RESERVED1[12];                  
    volatile const uint32_t ECCSTAT;                         /**< 34 Cache SRAM ECC Status Register */
    volatile const uint32_t ECCADDR;                         /**< 38 Cache SRAM ECC Address Register */
} ADI_CC_TypeDef;

/*! ========================================================================
 *  \struct ADI_FLASH_TypeDef
 *  \brief Flash Controller
 *  ======================================================================== */
typedef struct _ADI_FLASH_TypeDef
{
    volatile const uint32_t FEESTA;                          /**< 0 Status Register */
    volatile       uint32_t FEECON0;                         /**< 4 Command Control Register – Interrupt Enable Register */
    volatile       uint32_t FEECMD;                          /**< 8 Command Register */
    volatile       uint32_t FEEFLADR;                        /**< C Flash Address Key - Hole Register */
    volatile       uint32_t FEEFLDATA0;                      /**< 10 Flash Data Register - Key - Hole Interface Lower 32 Bits */
    volatile       uint32_t FEEFLDATA1;                      /**< 14 Flash Data Register - Key - Hole Interface Upper 32 Bits */
    volatile       uint32_t FEEADR0;                         /**< 18 Lower Page Address */
    volatile       uint32_t FEEADR1;                         /**< 1C Upper Page Address */
    volatile       uint32_t FEEKEY;                          /**< 20 Flash Key Register. */
    volatile const uint8_t RESERVED0[4];                   
    volatile       uint32_t FEEPRO0;                         /**< 28 Write Protection Register for Flash0 */
    volatile       uint32_t FEEPRO1;                         /**< 2C Write Protection Register for Flash1 */
    volatile const uint8_t RESERVED1[4];                   
    volatile const uint32_t FEESIG;                          /**< 34 Flash Signature */
    volatile       uint32_t FEECON1;                         /**< 38 User Setup Register */
    volatile const uint8_t RESERVED2[4];                   
    volatile const uint32_t FEEWRADDRA;                      /**< 40 Write Abort Address Register */
    volatile const uint8_t RESERVED3[4];                   
    volatile       uint32_t FEEAEN0;                         /**< 48 Lower 32 Bits of the Sys Irq Abort Enable Register. */
    volatile       uint32_t FEEAEN1;                         /**< 4C Middle 32 Bits of the Sys Irq Abort Enable Register. */
    volatile       uint32_t FEEAEN2;                         /**< 50 Upper 32 Bits of the Sys Irq Abort Enable Register. */
    volatile const uint8_t RESERVED4[16];                  
    volatile       uint32_t FEEECCCONFIG;                    /**< 64 Configurable ECC Enable/disable, Error Response */
    volatile const uint8_t RESERVED5[12];                  
    volatile const uint32_t FEEECCADDRC0;                    /**< 74 Flash 0 ECC Error Address via CODE Bus */
    volatile const uint32_t FEEECCADDRC1;                    /**< 78 Flash 1 ECC Error Address via CODE Bus */
    volatile const uint8_t RESERVED6[24];                  
    volatile const uint32_t FEEECCADDRD0;                    /**< 94 Flash 0 ECC Error Address via DMA Bus */
    volatile const uint32_t FEEECCADDRD1;                    /**< 98 Flash 1 ECC Error Address via DMA Bus */
} ADI_FLASH_TypeDef;

/*! ========================================================================
 *  \struct ADI_GPIO_TypeDef
 *  \brief GPIO
 *  ======================================================================== */
typedef struct _ADI_GPIO_TypeDef
{
    volatile       uint16_t CON;                             /**< 0 Port Configuration */
    volatile const uint8_t RESERVED0[2];                   
    volatile       uint8_t OE;                              /**< 4 Port Output Enable */
    volatile const uint8_t RESERVED1[3];                   
    volatile       uint8_t IE;                              /**< 8 Port Input Path Enable */
    volatile const uint8_t RESERVED2[3];                   
    volatile const uint8_t IN;                              /**< C Port Registered Data Input */
    volatile const uint8_t RESERVED3[3];                   
    volatile       uint8_t OUT;                             /**< 10 Port Data Output */
    volatile const uint8_t RESERVED4[3];                   
    volatile       uint8_t SET;                             /**< 14 Port Data Out Set */
    volatile const uint8_t RESERVED5[3];                   
    volatile       uint8_t CLR;                             /**< 18 Port Data Out Clear */
    volatile const uint8_t RESERVED6[3];                   
    volatile       uint8_t TGL;                             /**< 1C Port Pin Toggle */
    volatile const uint8_t RESERVED7[3];                   
    volatile       uint8_t ODE;                             /**< 20 Port Open Drain Enable */
    volatile const uint8_t RESERVED8[3];                   
    volatile       uint8_t IS;                              /**< 24 Port Input Select */
    volatile const uint8_t RESERVED9[3];                   
    volatile       uint8_t PE;                              /**< 28 Port Pull Enable */
    volatile const uint8_t RESERVED10[3];                  
    volatile       uint8_t PS;                              /**< 2C Port Pull Select */
    volatile const uint8_t RESERVED11[3];                  
    volatile       uint8_t SR;                              /**< 30 Port Slew Rate */
    volatile const uint8_t RESERVED12[3];                  
    volatile       uint16_t DS;                              /**< 34 Port Drive Select */
    volatile const uint8_t RESERVED13[2];                  
    volatile       uint8_t PWR;                             /**< 38 Port Power Select */
    volatile const uint8_t RESERVED14[3];                  
    volatile       uint8_t POL;                             /**< 3C GPIO Interrupt Polarity Select */
    volatile const uint8_t RESERVED15[3];                  
    volatile       uint8_t IENA;                            /**< 40 InterruptA Enable */
    volatile const uint8_t RESERVED16[3];                  
    volatile       uint8_t IENB;                            /**< 44 InterruptB Enable */
    volatile const uint8_t RESERVED17[3];                  
    volatile       uint8_t INT;                             /**< 48 Interrupt Status */
    volatile const uint8_t RESERVED18[3]; 
} ADI_GPIO_TypeDef;

/*! ========================================================================
 *  \struct ADI_SPI_TypeDef
 *  \brief Serial Peripheral Interface
 *  ======================================================================== */
typedef struct _ADI_SPI_TypeDef
{
    volatile       uint16_t STAT;                            /**< 0 Status */
    volatile const uint8_t RESERVED0[2];                   
    volatile const uint16_t RX;                              /**< 4 Receive */
    volatile const uint8_t RESERVED1[2];                   
    volatile       uint16_t TX;                              /**< 8 Transmit */
    volatile const uint8_t RESERVED2[2];                   
    volatile       uint16_t DIV;                             /**< C SPI Baud Rate Selection */
    volatile const uint8_t RESERVED3[2];                   
    volatile       uint16_t CTL;                             /**< 10 SPI Configuration 1 */
    volatile const uint8_t RESERVED4[2];                   
    volatile       uint16_t IEN;                             /**< 14 SPI Configuration 2 */
    volatile const uint8_t RESERVED5[2];                   
    volatile       uint16_t CNT;                             /**< 18 Transfer Byte Count */
    volatile const uint8_t RESERVED6[2];                   
    volatile       uint16_t DMA;                             /**< 1C SPI DMA Enable */
    volatile const uint8_t RESERVED7[2];                   
    volatile const uint16_t FIFOSTAT;                        /**< 20 FIFO Status */
    volatile const uint8_t RESERVED8[2];                   
    volatile       uint16_t RDCTL;                           /**< 24 Read Control */
    volatile const uint8_t RESERVED9[2];                   
    volatile       uint16_t FLOWCTL;                         /**< 28 Flow Control */
    volatile const uint8_t RESERVED10[2];                  
    volatile       uint16_t WAITTMR;                         /**< 2C Wait Timer for Flow Control */
    volatile const uint8_t RESERVED11[6];                  
    volatile       uint16_t CSOVERRIDE;                      /**< 34 Chip-Select Override */
} ADI_SPI_TypeDef;

/*! ========================================================================
 *  \struct ADI_CLOCK_TypeDef
 *  \brief Clock Gating and Other Settings
 *  ======================================================================== */
typedef struct _ADI_CLOCK_TypeDef
{
    volatile       uint16_t CLKCON0;                         /**< 0 Misc Clock Settings Register */
    volatile const uint8_t RESERVED0[2];                   
    volatile       uint16_t CLKCON1;                         /**< 4 Clock Dividers Register */
    volatile const uint8_t RESERVED1[2];                   
    volatile       uint16_t CLKSTAT0;                        /**< 8 Clocking Status */
} ADI_CLOCK_TypeDef;

/*! ========================================================================
 *  \struct ADI_PWM_TypeDef
 *  \brief PWM MMR
 *  ======================================================================== */
typedef struct _ADI_PWM_TypeDef
{
    volatile       uint16_t PWMCON0;                         /**< 0 PWM Control Register */
    volatile const uint8_t RESERVED0[2];                   
    volatile       uint16_t PWMCON1;                         /**< 4 ADC Conversion Start and Trip Control Register */
    volatile const uint8_t RESERVED1[2];                   
    volatile       uint16_t PWMICLR;                         /**< 8 Hardware Trip Configuration Register */
    volatile const uint8_t RESERVED2[6];                   
    volatile       uint16_t PWM0COM0;                        /**< 10 Compare Register 0 for PWM0 and PWM1 */
    volatile const uint8_t RESERVED3[2];                   
    volatile       uint16_t PWM0COM1;                        /**< 14 Compare Register 1 for PWM0 and PWM1 */
    volatile const uint8_t RESERVED4[2];                   
    volatile       uint16_t PWM0COM2;                        /**< 18 Compare Register 2 for PWM0 and PWM1 */
    volatile const uint8_t RESERVED5[2];                   
    volatile       uint16_t PWM0LEN;                         /**< 1C Period Value Register for PWM0 and PWM1 */
    volatile const uint8_t RESERVED6[2];                   
    volatile       uint16_t PWM1COM0;                        /**< 20 Compare Register 0 for PWM2 and PWM3 */
    volatile const uint8_t RESERVED7[2];                   
    volatile       uint16_t PWM1COM1;                        /**< 24 Compare Register 1 for PWM2 and PWM3 */
    volatile const uint8_t RESERVED8[2];                   
    volatile       uint16_t PWM1COM2;                        /**< 28 Compare Register 2 for PWM2 and PWM3 */
    volatile const uint8_t RESERVED9[2];                   
    volatile       uint16_t PWM1LEN;                         /**< 2C Period Value Register for PWM2 and PWM3 */
    volatile const uint8_t RESERVED10[2];                  
    volatile       uint16_t PWM2COM0;                        /**< 30 Compare Register 0 for PWM4 and PWM5 */
    volatile const uint8_t RESERVED11[2];                  
    volatile       uint16_t PWM2COM1;                        /**< 34 Compare Register 1 for PWM4 and PWM5 */
    volatile const uint8_t RESERVED12[2];                  
    volatile       uint16_t PWM2COM2;                        /**< 38 Compare Register 2 for PWM4 and PWM5 */
    volatile const uint8_t RESERVED13[2];                  
    volatile       uint16_t PWM2LEN;                         /**< 3C Period Value Register for PWM4 and PWM5 */
} ADI_PWM_TypeDef;

/*! ========================================================================
 *  \struct ADI_SUBSYS_TypeDef
 *  \brief Your module description, here.
 *  ======================================================================== */
typedef struct _ADI_SUBSYS_TypeDef
{
    volatile       uint32_t SRAMCON;                         /**< 0 SRAM Control Register */
    volatile const uint8_t RESERVED0[8];                   
    volatile       uint32_t SRAMECCCON;                      /**< C SRAM ECC Control Register */
    volatile const uint32_t SRAMECCSTA;                      /**< 10 SRAM ECC Status Register */
    volatile const uint32_t SRAMECCA0;                       /**< 14 SRAM0 ECC Error Address Register */
    volatile const uint32_t SRAMECCD0;                       /**< 18 SRAM0 ECC Error Data Register */
    volatile const uint32_t SRAMECCP0;                       /**< 1C SRAM0 ECC Error Parity Register */
    volatile const uint32_t SRAMECCA1;                       /**< 20 SRAM1 ECC Error Address Register */
    volatile const uint32_t SRAMECCD1;                       /**< 24 SRAM1 ECC Error Data Register */
    volatile const uint32_t SRAMECCP1;                       /**< 28 SRAM1 ECC Error Parity Register */
    volatile const uint32_t SRAMECCA2;                       /**< 2C SRAM2 ECC Error Address Register */
    volatile const uint32_t SRAMECCD2;                       /**< 30 SRAM2 ECC Error Data Register */
    volatile const uint32_t SRAMECCP2;                       /**< 34 SRAM2 ECC Error Parity Register */
} ADI_SUBSYS_TypeDef;

/*! ========================================================================
 *  \struct ADI_CRC_TypeDef
 *  \brief CRC Accelerator
 *  ======================================================================== */
typedef struct _ADI_CRC_TypeDef
{
    volatile       uint32_t CTL;                             /**< 0 CRC Control Register */
    volatile       int32_t IPDATA;                          /**< 4 Input Data Word Register */
    volatile       int32_t RESULT;                          /**< 8 CRC Result Register */
    volatile       uint32_t POLY;                            /**< C Programmable CRC Polynomial */
} ADI_CRC_TypeDef;

/*! ========================================================================
 *  \struct ADI_ADC_TypeDef
 *  \brief Your module description, here.
 *  ======================================================================== */
typedef struct _ADI_ADC_TypeDef
{
    volatile const uint32_t ADCDAT0;                         /**< 0 ADCx Data and Flags */
    volatile const uint32_t ADCDAT1;                         /**< 4 ADCx Data and Flags */
    volatile const uint32_t ADCDAT2;                         /**< 8 ADCx Data and Flags */
    volatile const uint32_t ADCDAT3;                         /**< C ADCx Data and Flags */
    volatile const uint32_t ADCDAT4;                         /**< 10 ADCx Data and Flags */
    volatile const uint32_t ADCDAT5;                         /**< 14 ADCx Data and Flags */
    volatile const uint32_t ADCDAT6;                         /**< 18 ADCx Data and Flags */
    volatile const uint32_t ADCDAT7;                         /**< 1C ADCx Data and Flags */
    volatile const uint32_t ADCDAT8;                         /**< 20 ADCx Data and Flags */
    volatile const uint32_t ADCDAT9;                         /**< 24 ADCx Data and Flags */
    volatile const uint32_t ADCDAT10;                        /**< 28 ADCx Data and Flags */
    volatile const uint32_t ADCDAT11;                        /**< 2C ADCx Data and Flags */
    volatile const uint32_t ADCDAT12;                        /**< 30 ADCx Data and Flags */
    volatile const uint32_t ADCDAT13;                        /**< 34 ADCx Data and Flags */
    volatile const uint32_t ADCDAT14;                        /**< 38 ADCx Data and Flags */
    volatile const uint32_t ADCDAT15;                        /**< 3C ADCx Data and Flags */
    volatile const uint32_t ADCDAT16;                        /**< 40 ADCx Data and Flags */
    volatile const uint32_t ADCDAT17;                        /**< 44 ADCx Data and Flags */
    volatile const uint32_t ADCDAT18;                        /**< 48 ADCx Data and Flags */
    volatile const uint32_t ADCDAT19;                        /**< 4C ADCx Data and Flags */
    volatile const uint32_t ADCDAT20;                        /**< 50 ADCx Data and Flags */
    volatile const uint32_t ADCDAT21;                        /**< 54 ADCx Data and Flags */
    volatile const uint32_t ADCDAT22;                        /**< 58 ADCx Data and Flags */
    volatile const uint32_t ADCDAT23;                        /**< 5C ADCx Data and Flags */
    volatile const uint32_t ADCDAT24;                        /**< 60 ADCx Data and Flags */
    volatile const uint32_t ADCDAT25;                        /**< 64 ADCx Data and Flags */
    volatile const uint32_t ADCDAT26;                        /**< 68 ADCx Data and Flags */
    volatile const uint32_t ADCDAT27;                        /**< 6C ADCx Data and Flags */
    volatile const uint32_t ADCDAT28;                        /**< 70 ADCx Data and Flags */
    volatile const uint32_t ADCDAT29;                        /**< 74 ADCx Data and Flags */
    volatile       uint32_t ADCCON;                          /**< 78 ADC Configuration */
    volatile       uint32_t PREBUFCON;                       /**< 7C Pre-charge Buffer Control */
    volatile       uint32_t ADCCNVC;                         /**< 80 ADC Conversion Cycle for Positive Input Channels */
    volatile       uint32_t ADCCNVCSLOW;                     /**< 84 ADC Conversion Cycle for Positive Input Channels */
    volatile       uint32_t ADCCHA;                          /**< 88 ADC Channel Select */
    volatile const uint32_t ADCIRQSTAT;                      /**< 8C ADC Interrupt Status */
    volatile       uint32_t ADCSEQ;                          /**< 90 ADC Sequencer Control */
    volatile       uint32_t ADCSEQC;                         /**< 94 ADC Sequencer Configuration */
    volatile const uint32_t ADCSEQS;                         /**< 98 ADC Sequencer Status */
    volatile       uint32_t ADCSEQCH;                        /**< 9C ADC Sequencer Channel 0 */
    volatile       uint32_t ADCSEQCHMUX0;                    /**< A0 ADC Sequencer Channel 1 */
    volatile       uint32_t ADCSEQCHMUX1;                    /**< A4 ADC Sequencer Channel 1 */
    volatile       uint32_t ADCCMP;                          /**< A8 Digital Comparator 0 Configuration */
    volatile       uint32_t ADCCMPIRQSTAT;                   /**< AC Digital Comparator Interrupt Status */
    volatile       uint32_t ADCOFGNDIFF;                     /**< B0 ADC Offset Gain Differential Channel Error Correction */
    volatile       uint32_t ADCOFTEMP;                       /**< B4 ADC Offset Gain Temp Sensor Channel Error Correction */
    volatile       uint32_t ADCGNTEMP;                       /**< B8 ADC Offset Gain Temp Sensor Channel Error Correction */
    volatile       uint32_t ADCOFGNPGA0;                     /**< BC ADC Offset Gain PGA0 Channel Error Correction */
    volatile       uint32_t ADCOFGNPGA1;                     /**< C0 ADC Offset Gain PGA1 Channel Error Correction */
    volatile       uint32_t ADCOFGNPGA2;                     /**< C4 ADC Offset Gain PGA2 Channel Error Correction */
    volatile       uint32_t ADCOFGNPGA3;                     /**< C8 ADC Offset Gain PGA3 Channel Error Correction */
    volatile       uint32_t ADCOFGNPGA0TIA;                  /**< CC ADC Offset Gain PGA0 Channel Error Correction */
    volatile       uint32_t ADCOFGNPGA1TIA;                  /**< D0 ADC Offset Gain PGA1 Channel Error Correction */
    volatile       uint32_t ADCOFGNPGA2TIA;                  /**< D4 ADC Offset Gain PGA2 Channel Error Correction */
    volatile       uint32_t ADCOFGNPGA3TIA;                  /**< D8 ADC Offset Gain PGA3 Channel Error Correction */
    volatile const uint8_t RESERVED0[120];                 
    volatile       uint32_t ADCCMP1;                         /**< 154 Digital Comparator 1 Configuration */
    volatile       uint32_t ADCCMP2;                         /**< 158 Digital Comparator 2 Configuration */
    volatile       uint32_t ADCCMP3;                         /**< 15C Digital Comparator 3 Configuration */
} ADI_ADC_TypeDef;

/*! ========================================================================
 *  \struct ADI_COMP_TypeDef
 *  \brief Your module description, here.
 *  ======================================================================== */
typedef struct _ADI_COMP_TypeDef
{
    volatile       uint32_t COMPCON0;                        /**< 0 No description provided */
    volatile       uint32_t COMPCON1;                        /**< 4 No description provided */
    volatile       uint32_t COMPCON2;                        /**< 8 No description provided */
    volatile       uint32_t COMPCON3;                        /**< C No description provided */
    volatile const uint32_t COMPIRQSTAT;                     /**< 10 No description provided */
} ADI_COMP_TypeDef;

/*! ========================================================================
 *  \struct ADI_OSC_MMRS_TypeDef
 *  \brief Your module description, here.
 *  ======================================================================== */
typedef struct _ADI_OSC_MMRS_TypeDef
{
    volatile const uint8_t RESERVED0[3];                   
    volatile       uint32_t HFOSCCTRL;                       /**< 4 No description provided */
    volatile const uint8_t RESERVED1[16];                  
    volatile       uint32_t HFXTALCTRL;                      /**< 18 No description provided */
} ADI_OSC_MMRS_TypeDef;

/*! ========================================================================
 *  \struct ADI_PGA_TypeDef
 *  \brief Your module description, here.
 *  ======================================================================== */
typedef struct _ADI_PGA_TypeDef
{
    volatile       uint32_t PGABIASCON;                      /**< 0 PGA Bias Circuit Control Signal */
    volatile const uint8_t RESERVED0[28];                  
    volatile       uint32_t PGA0CON;                         /**< 20 PGA0 Control Register */
    volatile       uint32_t PGA0CHPCON;                      /**< 24 PGA0 Chop Function Ctrl */
    volatile       uint32_t PGA3CHPCON;                      /**< 28 PGA3 Chop Function Ctrl */
    volatile const uint8_t RESERVED1[68];                  
    volatile       uint32_t PGA1CON;                         /**< 70 PGA1 Control Register */
    volatile       uint32_t PGA1CHPCON;                      /**< 74 PGA1 Chop Function Ctrl */
    volatile const uint8_t RESERVED2[40];                  
    volatile       uint32_t PGA2CON;                         /**< A0 PGA2 Control Register */
    volatile       uint32_t PGA2CHPCON;                      /**< A4 PGA2 Chop Function Ctrl */
    volatile const uint8_t RESERVED3[40];                  
    volatile       uint32_t PGA3CON;                         /**< D0 PGA3 Control Register */
} ADI_PGA_TypeDef;

/*! ========================================================================
 *  \struct ADI_PLL_MMRS_TypeDef
 *  \brief Your module description, here.
 *  ======================================================================== */
typedef struct _ADI_PLL_MMRS_TypeDef
{
    volatile       uint32_t PLLPDCTRL;                       /**< 0 No description provided */
} ADI_PLL_MMRS_TypeDef;

/*! ========================================================================
 *  \struct ADI_VCM_MMRS_TypeDef
 *  \brief Your module description, here.
 *  ======================================================================== */
typedef struct _ADI_VCM_MMRS_TypeDef
{
    volatile       uint32_t TMPSNSCON;                       /**< 0 No description provided */
    volatile       uint32_t TMPSNSCHPCON;                    /**< 4 No description provided */
    volatile       uint32_t VCMREFCON;                       /**< 8 No description provided */
    volatile const uint8_t RESERVED0[4];                   
    volatile       uint32_t VCMBUFCON;                       /**< 10 No description provided */
} ADI_VCM_MMRS_TypeDef;

/*! ========================================================================
 *  \struct ADI_DAC_TypeDef
 *  \brief Your module description, here.
 *  ======================================================================== */
typedef struct _ADI_DAC_TypeDef
{
    volatile       uint32_t DACCON0;                         /**< 0 DAC Control Register */
    volatile       uint32_t DACCON1;                         /**< 4 DAC Control Register */
    volatile       uint32_t DACCON2;                         /**< 8 DAC Control Register */
    volatile       uint32_t DACCON3;                         /**< C DAC Control Register */
    volatile       uint32_t DACCON4;                         /**< 10 DAC Control Register */
    volatile       uint32_t DACCON5;                         /**< 14 DAC Control Register */
    volatile       uint32_t DACCON6;                         /**< 18 DAC Control Register */
    volatile       uint32_t DACCON7;                         /**< 1C DAC Control Register */
    volatile       uint32_t DACCON8;                         /**< 20 DAC Control Register */
    volatile       uint32_t DACCON9;                         /**< 24 DAC Control Register */
    volatile       uint32_t DACCON10;                        /**< 28 DAC Control Register */
    volatile       uint32_t DACCON11;                        /**< 2C DAC Control Register */
    volatile       uint32_t DACDAT0;                         /**< 30 DAC Data Register */
    volatile       uint32_t DACDAT1;                         /**< 34 DAC Data Register */
    volatile       uint32_t DACDAT2;                         /**< 38 DAC Data Register */
    volatile       uint32_t DACDAT3;                         /**< 3C DAC Data Register */
    volatile       uint32_t DACDAT4;                         /**< 40 DAC Data Register */
    volatile       uint32_t DACDAT5;                         /**< 44 DAC Data Register */
    volatile       uint32_t DACDAT6;                         /**< 48 DAC Data Register */
    volatile       uint32_t DACDAT7;                         /**< 4C DAC Data Register */
    volatile       uint32_t DACDAT8;                         /**< 50 DAC Data Register */
    volatile       uint32_t DACDAT9;                         /**< 54 DAC Data Register */
    volatile       uint32_t DACDAT10;                        /**< 58 DAC Data Register */
    volatile       uint32_t DACDAT11;                        /**< 5C DAC Data Register */
} ADI_DAC_TypeDef;


/*  =========================================================================
 * Peripheral Memory Map Declarations
 *  ========================================================================= */

#define ADI_GPT0_BASE                       (0x40000000U)    /* Base address of gpt0: */
#define ADI_GPT1_BASE                       (0x40000400U)    /* Base address of gpt1: */
#define ADI_GPT2_BASE                       (0x40000800U)    /* Base address of gpt2: */
#define ADI_GPTH0_BASE                      (0x40000C00U)    /* Base address of gpth0: */
#define ADI_GPTH1_BASE                      (0x40001000U)    /* Base address of gpth1: */
#define ADI_MISC_BASE                       (0x40002000U)    /* Base address of misc: */
#define ADI_WUT_BASE                        (0x40003000U)    /* Base address of wut: */
#define ADI_WDT_BASE                        (0x40004000U)    /* Base address of wdt: */
#define ADI_ALLON_BASE                      (0x40005000U)    /* Base address of allon: */
#define ADI_PLA_BASE                        (0x40006000U)    /* Base address of pla: */
#define ADI_DMAREQ_BASE                     (0x40007000U)    /* Base address of dmareq: */
#define ADI_UART0_BASE                      (0x40020000U)    /* Base address of uart0: */
#define ADI_UART1_BASE                      (0x40020400U)    /* Base address of uart1: */
#define ADI_I2C0_BASE                       (0x40020800U)    /* Base address of i2c0: */
#define ADI_I2C1_BASE                       (0x40020C00U)    /* Base address of i2c1: */
#define ADI_I2C2_BASE                       (0x40021000U)    /* Base address of i2c2: */
#define ADI_MDIO_BASE                       (0x40022000U)    /* Base address of mdio: */
#define ADI_DMA_BASE                        (0x40040000U)    /* Base address of dma: */
#define ADI_CACHE_BASE                      (0x40044000U)    /* Base address of cache: */
#define ADI_FLASH_BASE                      (0x40048000U)    /* Base address of flash: */
#define ADI_GPIO0_BASE                      (0x40050000U)    /* Base address of gpio0: */
#define ADI_GPIO1_BASE                      (0x40050050U)    /* Base address of gpio1: */
#define ADI_GPIO2_BASE                      (0x400500A0U)    /* Base address of gpio2: */
#define ADI_GPIO3_BASE                      (0x400500F0U)    /* Base address of gpio3: */
#define ADI_GPIO4_BASE                      (0x40050140U)    /* Base address of gpio4: */
#define ADI_GPIO5_BASE                      (0x40050190U)    /* Base address of gpio5: */
#define ADI_SPI0_BASE                       (0x40054000U)    /* Base address of spi0: */
#define ADI_SPI1_BASE                       (0x40058000U)    /* Base address of spi1: */
#define ADI_SPI2_BASE                       (0x4005C000U)    /* Base address of spi2: */
#define ADI_CLK_BASE                        (0x40060000U)    /* Base address of clk: */
#define ADI_PWM_BASE                        (0x40064000U)    /* Base address of pwm: */
#define ADI_SRAM_BASE                       (0x40065000U)    /* Base address of sram: */
#define ADI_CRC_BASE                        (0x40066000U)    /* Base address of crc: */
#define ADI_ADC_BASE                        (0x40068000U)    /* Base address of adc: */
#define ADI_COMP_BASE                       (0x40068A00U)    /* Base address of comp: */
#define ADI_OSC_BASE                        (0x40068E00U)    /* Base address of osc: */
#define ADI_PGA_BASE                        (0x40069000U)    /* Base address of pga: */
#define ADI_PLL_BASE                        (0x40069200U)    /* Base address of pll: */
#define ADI_TMPSNS_BASE                     (0x40069600U)    /* Base address of tmpsns: */
#define ADI_VDAC_BASE                       (0x40069800U)    /* Base address of vdac: */


/*  =========================================================================
 * Peripheral Pointer Declarations
 *  ========================================================================= */

#define pADI_GPT0                           ((ADI_TMR_TypeDef *) ADI_GPT0_BASE)                   /* Pointer to  (gpt0)*/
#define pADI_GPT1                           ((ADI_TMR_TypeDef *) ADI_GPT1_BASE)                   /* Pointer to  (gpt1)*/
#define pADI_GPT2                           ((ADI_TMR_TypeDef *) ADI_GPT2_BASE)                   /* Pointer to  (gpt2)*/
#define pADI_GPTH0                          ((ADI_TIMER_TypeDef *) ADI_GPTH0_BASE)                /* Pointer to  (gpth0)*/
#define pADI_GPTH1                          ((ADI_TIMER_TypeDef *) ADI_GPTH1_BASE)                /* Pointer to  (gpth1)*/
#define pADI_MISC                           ((ADI_MISC16_TypeDef *) ADI_MISC_BASE)                /* Pointer to  (misc)*/
#define pADI_WUT                            ((ADI_WUT_TypeDef *) ADI_WUT_BASE)                    /* Pointer to  (wut)*/
#define pADI_WDT                            ((ADI_WDT_TypeDef *) ADI_WDT_BASE)                    /* Pointer to  (wdt)*/
#define pADI_ALLON                          ((ADI_ALWAYS_ON_TypeDef *) ADI_ALLON_BASE)            /* Pointer to  (allon)*/
#define pADI_PLA                            ((ADI_PLA_TypeDef *) ADI_PLA_BASE)                    /* Pointer to  (pla)*/
#define pADI_DMAREQ                         ((ADI_DMAREQ_TypeDef *) ADI_DMAREQ_BASE)              /* Pointer to  (dmareq)*/
#define pADI_UART0                          ((ADI_UART_TypeDef *) ADI_UART0_BASE)                 /* Pointer to  (uart0)*/
#define pADI_UART1                          ((ADI_UART_TypeDef *) ADI_UART1_BASE)                 /* Pointer to  (uart1)*/
#define pADI_I2C0                           ((ADI_I2C_TypeDef *) ADI_I2C0_BASE)                   /* Pointer to  (i2c0)*/
#define pADI_I2C1                           ((ADI_I2C_TypeDef *) ADI_I2C1_BASE)                   /* Pointer to  (i2c1)*/
#define pADI_I2C2                           ((ADI_I2C_TypeDef *) ADI_I2C2_BASE)                   /* Pointer to  (i2c2)*/
#define pADI_MDIO                           ((ADI_MDIO_TypeDef *) ADI_MDIO_BASE)                  /* Pointer to  (mdio)*/
#define pADI_DMA                            ((ADI_DMA_TypeDef *) ADI_DMA_BASE)                    /* Pointer to  (dma)*/
#define pADI_CACHE                          ((ADI_CC_TypeDef *) ADI_CACHE_BASE)                   /* Pointer to  (cache)*/
#define pADI_FLASH                          ((ADI_FLASH_TypeDef *) ADI_FLASH_BASE)                /* Pointer to  (flash)*/
#define pADI_GPIO0                          ((ADI_GPIO_TypeDef *) ADI_GPIO0_BASE)                 /* Pointer to  (gpio0)*/
#define pADI_GPIO1                          ((ADI_GPIO_TypeDef *) ADI_GPIO1_BASE)                 /* Pointer to  (gpio1)*/
#define pADI_GPIO2                          ((ADI_GPIO_TypeDef *) ADI_GPIO2_BASE)                 /* Pointer to  (gpio2)*/
#define pADI_GPIO3                          ((ADI_GPIO_TypeDef *) ADI_GPIO3_BASE)                 /* Pointer to  (gpio3)*/
#define pADI_GPIO4                          ((ADI_GPIO_TypeDef *) ADI_GPIO4_BASE)                 /* Pointer to  (gpio4)*/
#define pADI_GPIO5                          ((ADI_GPIO_TypeDef *) ADI_GPIO5_BASE)                 /* Pointer to  (gpio5)*/
#define pADI_SPI0                           ((ADI_SPI_TypeDef *) ADI_SPI0_BASE)                   /* Pointer to  (spi0)*/
#define pADI_SPI1                           ((ADI_SPI_TypeDef *) ADI_SPI1_BASE)                   /* Pointer to  (spi1)*/
#define pADI_SPI2                           ((ADI_SPI_TypeDef *) ADI_SPI2_BASE)                   /* Pointer to  (spi2)*/
#define pADI_CLK                            ((ADI_CLOCK_TypeDef *) ADI_CLK_BASE)                  /* Pointer to  (clk)*/
#define pADI_PWM                            ((ADI_PWM_TypeDef *) ADI_PWM_BASE)                    /* Pointer to  (pwm)*/
#define pADI_SRAM                           ((ADI_SUBSYS_TypeDef *) ADI_SRAM_BASE)                /* Pointer to  (sram)*/
#define pADI_CRC                            ((ADI_CRC_TypeDef *) ADI_CRC_BASE)                    /* Pointer to  (crc)*/
#define pADI_ADC                            ((ADI_ADC_TypeDef *) ADI_ADC_BASE)                    /* Pointer to  (adc)*/
#define pADI_COMP                           ((ADI_COMP_TypeDef *) ADI_COMP_BASE)                  /* Pointer to  (comp)*/
#define pADI_OSC                            ((ADI_OSC_MMRS_TypeDef *) ADI_OSC_BASE)               /* Pointer to  (osc)*/
#define pADI_PGA                            ((ADI_PGA_TypeDef *) ADI_PGA_BASE)                    /* Pointer to  (pga)*/
#define pADI_PLL                            ((ADI_PLL_MMRS_TypeDef *) ADI_PLL_BASE)               /* Pointer to  (pll)*/
#define pADI_TMPSNS                         ((ADI_VCM_MMRS_TypeDef *) ADI_TMPSNS_BASE)            /* Pointer to  (tmpsns)*/
#define pADI_VDAC                           ((ADI_DAC_TypeDef *) ADI_VDAC_BASE)                   /* Pointer to  (vdac)*/

/*  =========================================================================
 *! \enum   DMA_CHANn_TypeDef
 *! \brief  DMAChannel Number Assignments
 *  ========================================================================= */

typedef enum
{
    SPI0_TX_CHANn                               =     0,    /*!<* SPI0_TX */
    SPI0_RX_CHANn                               =     1,    /*!<* SPI0_RX */
    SPI1_TX_CHANn                               =     2,    /*!<* SPI1_TX */
    SPI1_RX_CHANn                               =     3,    /*!<* SPI1_RX */
    SPI2_TX_CHANn                               =     4,    /*!<* SPI2_TX */
    SPI2_RX_CHANn                               =     5,    /*!<* SPI2_RX */
    UART0_TX_CHANn                              =     6,    /*!<* UART0_TX */
    UART0_RX_CHANn                              =     7,    /*!<* UART0_RX */
    UART1_TX_CHANn                              =     8,    /*!<* UART1_TX */
    UART1_RX_CHANn                              =     9,    /*!<* UART1_RX */
    I2C0S_TX_CHANn                              =    10,    /*!<* I2C0S_TX */
    I2C0S_RX_CHANn                              =    11,    /*!<* I2C0S_RX */
    I2C0M_CHANn                                 =    12,    /*!<* I2C0M */
    I2C1S_TX_CHANn                              =    13,    /*!<* I2C1S_TX */
    I2C1S_RX_CHANn                              =    14,    /*!<* I2C1S_RX */
    I2C1M_CHANn                                 =    15,    /*!<* I2C1M */
    I2C2S_TX_CHANn                              =    16,    /*!<* I2C2S_TX */
    I2C2S_RX_CHANn                              =    17,    /*!<* I2C2S_RX */
    I2C2M_CHANn                                 =    18,    /*!<* I2C2M */
    MDIO_TX_CHANn                               =    19,    /*!<* MDIO_TX */
    MDIO_RX_CHANn                               =    20,    /*!<* MDIO_RX */
    FLASH_KHWR_CHANn                            =    21,    /*!<* FLASH_KHWR */
    ADC_RD_CHANn                                =    22,    /*!<* ADC_RD */
    sys_Trig0_CHANn                             =    27,    /*!<* sys_Trig0 */
    sys_Trig1_CHANn                             =    28,    /*!<* sys_Trig1 */
    sys_SW0_CHANn                               =    29,    /*!<* sys_SW0 */
    sys_SW1_CHANn                               =    30,    /*!<* sys_SW1 */
} DMA_CHANn_TypeDef;  /* typedef name for fixed DMA channel assignment */


#ifdef __cplusplus
}
#endif

#endif // __ADUCM410_H__

