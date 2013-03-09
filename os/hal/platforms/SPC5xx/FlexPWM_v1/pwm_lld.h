/*
 * Licensed under ST Liberty SW License Agreement V2, (the "License");
 * You may not use this file except in compliance with the License.
 * You may obtain a copy of the License at:
 *
 *        http://www.st.com/software_license_agreement_liberty_v2
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/**
 * @file    FlexPWM_v1/pwm_lld.h
 * @brief   SPC5xx low level PWM driver header.
 *
 * @addtogroup PWM
 * @{
 */

#ifndef _PWM_LLD_H_
#define _PWM_LLD_H_

#if HAL_USE_PWM || defined(__DOXYGEN__)

/*===========================================================================*/
/* Driver constants.                                                         */
/*===========================================================================*/

/**
 * @name    STS register bits definitions
 * @{
 */
#define SPC5_FLEXPWM_STS_CMPF0                          (1U << 0)
#define SPC5_FLEXPWM_STS_CMPF1                          (1U << 1)
#define SPC5_FLEXPWM_STS_CMPF2                          (1U << 2)
#define SPC5_FLEXPWM_STS_CMPF3                          (1U << 3)
#define SPC5_FLEXPWM_STS_CMPF4                          (1U << 4)
#define SPC5_FLEXPWM_STS_CMPF5                          (1U << 5)
#define SPC5_FLEXPWM_STS_CFX0                           (1U << 6)
#define SPC5_FLEXPWM_STS_CFX1                           (1U << 7)
#define SPC5_FLEXPWM_STS_RF                             (1U << 12)
#define SPC5_FLEXPWM_STS_REF                            (1U << 13)
#define SPC5_FLEXPWM_STS_RUF                            (1U << 14)
/** @} */

/**
 * @name    CTRL2 register bits definitions
 * @{
 */
#define SPC5_FLEXPWM_CNTL2_CLKSEL0                      (1U << 0)
#define SPC5_FLEXPWM_CNTL2_CLKSEL1                      (1U << 1)
#define SPC5_FLEXPWM_CNTL2_RELOAD_SEL                   (1U << 2)
#define SPC5_FLEXPWM_CNTL2_FORCE_SEL0                   (1U << 3)
#define SPC5_FLEXPWM_CNTL2_FORCE_SEL1                   (1U << 4)
#define SPC5_FLEXPWM_CNTL2_FORCE_SEL2                   (1U << 5)
#define SPC5_FLEXPWM_CNTL2_FORCE                        (1U << 6)
#define SPC5_FLEXPWM_CNTL2_FRCEN                        (1U << 7)
#define SPC5_FLEXPWM_CNTL2_INIT_SEL0                    (1U << 8)
#define SPC5_FLEXPWM_CNTL2_INIT_SEL1                    (1U << 9)
#define SPC5_FLEXPWM_CNTL2_PWM_X_INIT                   (1U << 10)
#define SPC5_FLEXPWM_CNTL2_PWM_B_INIT                   (1U << 11)
#define SPC5_FLEXPWM_CNTL2_PWM_A_INIT                   (1U << 12)
#define SPC5_FLEXPWM_CNTL2_INDEP                        (1U << 13)
#define SPC5_FLEXPWM_CNTL2_WAITEN                       (1U << 14)
#define SPC5_FLEXPWM_CNTL2_DGBEN                        (1U << 15)
/** @} */

/**
 * @name    CTRL1 register bits definitions
 * @{
 */
#define SPC5_FLEXPWM_CNTL1_DBLEN                        (1U << 0)
#define SPC5_FLEXPWM_CNTL1_PRSC0                        (1U << 4)
#define SPC5_FLEXPWM_CNTL1_PRSC1                        (1U << 5)
#define SPC5_FLEXPWM_CNTL1_PRSC2                        (1U << 6)
#define SPC5_FLEXPWM_CNTL1_FULL                         (1U << 10)
#define SPC5_FLEXPWM_CNTL1_HALF                         (1U << 11)
#define SPC5_FLEXPWM_CNTL1_LDFQ0                        (1U << 12)
#define SPC5_FLEXPWM_CNTL1_LDFQ1                        (1U << 13)
#define SPC5_FLEXPWM_CNTL1_LDFQ2                        (1U << 14)
#define SPC5_FLEXPWM_CNTL1_LDFQ3                        (1U << 15)
/** @} */

/**
 * @name    OCTRL register bits definitions
 * @{
 */
#define SPC5_FLEXPWM_OCNTL_PWMXFS0                      (1U << 0)
#define SPC5_FLEXPWM_OCNTL_PWMXFS1                      (1U << 1)
#define SPC5_FLEXPWM_OCNTL_PWMBFS0                      (1U << 2)
#define SPC5_FLEXPWM_OCNTL_PWMBFS1                      (1U << 3)
#define SPC5_FLEXPWM_OCNTL_PWMAFS0                      (1U << 4)
#define SPC5_FLEXPWM_OCNTL_PWMAFS1                      (1U << 5)
#define SPC5_FLEXPWM_OCNTL_POLX                         (1U << 8)
#define SPC5_FLEXPWM_OCNTL_POLB                         (1U << 9)
#define SPC5_FLEXPWM_OCNTL_POLA                         (1U << 10)
#define SPC5_FLEXPWM_OCNTL_PWMX_IN                      (1U << 13)
#define SPC5_FLEXPWM_OCNTL_PWMB_IN                      (1U << 14)
#define SPC5_FLEXPWM_OCNTL_PWMA_IN                      (1U << 15)
/** @} */

/**
 * @name    INTEN register bits definitions
 * @{
 */
#define SPC5_FLEXPWM_INTEN_CMPIE0                       (1U << 0)
#define SPC5_FLEXPWM_INTEN_CMPIE1                       (1U << 1)
#define SPC5_FLEXPWM_INTEN_CMPIE2                       (1U << 2)
#define SPC5_FLEXPWM_INTEN_CMPIE3                       (1U << 3)
#define SPC5_FLEXPWM_INTEN_CMPIE4                       (1U << 4)
#define SPC5_FLEXPWM_INTEN_CMPIE5                       (1U << 5)
#define SPC5_FLEXPWM_INTEN_CX0IE                        (1U << 6)
#define SPC5_FLEXPWM_INTEN_CX1IE                        (1U << 7)
#define SPC5_FLEXPWM_INTEN_RIE                          (1U << 12)
#define SPC5_FLEXPWM_INTEN_REIE                         (1U << 13)
/** @} */

/**
 * @name    OUTEN register bits definitions
 * @{
 */
#define SPC5_FLEXPWM_OUTEN_PWMX_EN0                     (1U << 0)
#define SPC5_FLEXPWM_OUTEN_PWMX_EN1                     (1U << 1)
#define SPC5_FLEXPWM_OUTEN_PWMX_EN2                     (1U << 2)
#define SPC5_FLEXPWM_OUTEN_PWMX_EN3                     (1U << 3)
#define SPC5_FLEXPWM_OUTEN_PWMB_EN0                     (1U << 4)
#define SPC5_FLEXPWM_OUTEN_PWMB_EN1                     (1U << 5)
#define SPC5_FLEXPWM_OUTEN_PWMB_EN2                     (1U << 6)
#define SPC5_FLEXPWM_OUTEN_PWMB_EN3                     (1U << 7)
#define SPC5_FLEXPWM_OUTEN_PWMA_EN0                     (1U << 8)
#define SPC5_FLEXPWM_OUTEN_PWMA_EN1                     (1U << 9)
#define SPC5_FLEXPWM_OUTEN_PWMA_EN2                     (1U << 10)
#define SPC5_FLEXPWM_OUTEN_PWMA_EN3                     (1U << 11)
/** @} */

/**
 * @name    MASK register bits definitions
 * @{
 */
#define SPC5_FLEXPWM_MASK_MASKX0                        (1U << 0)
#define SPC5_FLEXPWM_MASK_MASKX1                        (1U << 1)
#define SPC5_FLEXPWM_MASK_MASKX2                        (1U << 2)
#define SPC5_FLEXPWM_MASK_MASKX3                        (1U << 3)
#define SPC5_FLEXPWM_MASK_MASKB0                        (1U << 4)
#define SPC5_FLEXPWM_MASK_MASKB1                        (1U << 5)
#define SPC5_FLEXPWM_MASK_MASKB2                        (1U << 6)
#define SPC5_FLEXPWM_MASK_MASKB3                        (1U << 7)
#define SPC5_FLEXPWM_MASK_MASKA0                        (1U << 8)
#define SPC5_FLEXPWM_MASK_MASKA1                        (1U << 9)
#define SPC5_FLEXPWM_MASK_MASKA2                        (1U << 10)
#define SPC5_FLEXPWM_MASK_MASKA3                        (1U << 11)
/** @} */

/**
 * @name    MCTRL register bits definitions
 * @{
 */
#define SPC5_FLEXPWM_MCTRL_LDOK0                        (1U << 0)
#define SPC5_FLEXPWM_MCTRL_LDOK1                        (1U << 1)
#define SPC5_FLEXPWM_MCTRL_LODK2                        (1U << 2)
#define SPC5_FLEXPWM_MCTRL_LDOK3                        (1U << 3)
#define SPC5_FLEXPWM_MCTRL_CLDOK0                       (1U << 4)
#define SPC5_FLEXPWM_MCTRL_CLDOK1                       (1U << 5)
#define SPC5_FLEXPWM_MCTRL_CLDOK2                       (1U << 6)
#define SPC5_FLEXPWM_MCTRL_CLDOK3                       (1U << 7)
#define SPC5_FLEXPWM_MCTRL_RUN0                         (1U << 8)
#define SPC5_FLEXPWM_MCTRL_RUN1                         (1U << 9)
#define SPC5_FLEXPWM_MCTRL_RUN2                         (1U << 10)
#define SPC5_FLEXPWM_MCTRL_RUN3                         (1U << 11)
#define SPC5_FLEXPWM_MCTRL_IPOL0                        (1U << 12)
#define SPC5_FLEXPWM_MCTRL_IPOL1                        (1U << 13)
#define SPC5_FLEXPWM_MCTRL_IPOL2                        (1U << 14)
#define SPC5_FLEXPWM_MCTRL_IPOL3                        (1U << 15)
/** @} */

/**
 * @name    PSC values definition
 * @{
 */
#define SPC5_FLEXPWM_PSC_1                  0
#define SPC5_FLEXPWM_PSC_2                  SPC5_FLEXPWM_CNTL1_PRSC0
#define SPC5_FLEXPWM_PSC_4                  SPC5_FLEXPWM_CNTL1_PRSC1
#define SPC5_FLEXPWM_PSC_8                  SPC5_FLEXPWM_CNTL1_PRSC0 ||      \
                                            SPC5_FLEXPWM_CNTL1_PRSC1
#define SPC5_FLEXPWM_PSC_16                 SPC5_FLEXPWM_CNTL1_PRSC2
#define SPC5_FLEXPWM_PSC_32                 SPC5_FLEXPWM_CNTL1_PRSC0 ||      \
                                            SPC5_FLEXPWM_CNTL1_PRSC2
#define SPC5_FLEXPWM_PSC_64                 SPC5_FLEXPWM_CNTL1_PRSC1 ||      \
                                            SPC5_FLEXPWM_CNTL1_PRSC2
#define SPC5_FLEXPWM_PSC_128                SPC5_FLEXPWM_CNTL1_PRSC0 ||      \
                                            SPC5_FLEXPWM_CNTL1_PRSC1 ||      \
                                            SPC5_FLEXPWM_CNTL1_PRSC2
/** @} */

/**
 * @name    LDOK, CLDOK, RUN, IPOL masks definition
 * @{
 */
#define SPC5_FLEXPWM_LDOK_MASK              0x000F
#define SPC5_FLEXPWM_CLDOK_MASK             0x00F0
#define SPC5_FLEXPWM_RUN_MASK               0x0F00
#define SPC5_FLEXPWM_IPOL_MASK              0xF000
/** @} */

/**
 * @brief   Number of PWM channels per PWM driver.
 */
#define PWM_CHANNELS                            2

/**
 * @brief   Complementary output modes mask.
 * @note    This is an SPC5-specific setting.
 */
#define PWM_COMPLEMENTARY_OUTPUT_MASK           0xF0

/**
 * @brief   Complementary output not driven.
 * @note    This is an SPC5-specific setting.
 */
#define PWM_COMPLEMENTARY_OUTPUT_DISABLED       0x00

/**
 * @brief   Complementary output, active is logic level one.
 * @note    This is an SPC5-specific setting.
 */
#define PWM_COMPLEMENTARY_OUTPUT_ACTIVE_HIGH    0x10

/**
 * @brief   Complementary output, active is logic level zero.
 * @note    This is an SPC5-specific setting.
 */
#define PWM_COMPLEMENTARY_OUTPUT_ACTIVE_LOW     0x20

/**
 * @brief   Edge-Aligned PWM functional mode.
 * @note    This is an SPC5-specific setting.
 */
#define EDGE_ALIGNED_PWM                        0x01

/**
 * @brief   Center-Aligned PWM functional mode.
 * @note    This is an SPC5-specific setting.
 */
#define CENTER_ALIGNED_PWM                      0x02

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @name    Configuration options
 * @{
 */
#if SPC5_HAS_FLEXPWM0
/**
 * @brief   PWMD1 driver enable switch.
 * @details If set to @p TRUE the support for PWMD1 is included.
 * @note    The default is @p TRUE.
 */
#if !defined(SPC5_PWM_USE_SMOD0) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_SMOD0                  TRUE
#endif

/**
 * @brief   PWMD2 driver enable switch.
 * @details If set to @p TRUE the support for PWMD2 is included.
 * @note    The default is @p TRUE.
 */
#if !defined(SPC5_PWM_USE_SMOD1) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_SMOD1                  TRUE
#endif

/**
 * @brief   PWMD3 driver enable switch.
 * @details If set to @p TRUE the support for PWMD3 is included.
 * @note    The default is @p TRUE.
 */
#if !defined(SPC5_PWM_USE_SMOD2) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_SMOD2                  TRUE
#endif

/**
 * @brief   PWMD4 driver enable switch.
 * @details If set to @p TRUE the support for PWMD4 is included.
 * @note    The default is @p TRUE.
 */
#if !defined(SPC5_PWM_USE_SMOD3) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_SMOD3                  TRUE
#endif

/**
 * @brief   PWMD1 interrupt priority level setting.
 */
#if !defined(SPC5_PWM_SMOD0_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_PWM_SMOD0_PRIORITY             7
#endif

/**
 * @brief   PWMD2 interrupt priority level setting.
 */
#if !defined(SPC5_PWM_SMOD1_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_PWM_SMOD1_PRIORITY             7
#endif

/**
 * @brief   PWMD3 interrupt priority level setting.
 */
#if !defined(SPC5_PWM_SMOD2_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_PWM_SMOD2_PRIORITY             7
#endif

/**
 * @brief   PWMD4 interrupt priority level setting.
 */
#if !defined(SPC5_PWM_SMOD3_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_PWM_SMOD3_PRIORITY             7
#endif

/**
 * @brief   FlexPWM-0 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode. The defaults of the run modes
 *          are defined in @p hal_lld.h.
 */
#if !defined(SPC5_PWM_FLEXPWM0_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_PWM_FLEXPWM0_START_PCTL        (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   FlexPWM-0 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode. The defaults of the run modes
 *          are defined in @p hal_lld.h.
 */
#if !defined(SPC5_PWM_FLEXPWM0_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_PWM_FLEXPWM0_STOP_PCTL         (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif
#endif

#if SPC5_HAS_FLEXPWM1
/**
 * @brief   PWMD5 driver enable switch.
 * @details If set to @p TRUE the support for PWMD4 is included.
 * @note    The default is @p TRUE.
 */
#if !defined(SPC5_PWM_USE_SMOD4) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_SMOD4                  TRUE
#endif

/**
 * @brief   PWMD6 driver enable switch.
 * @details If set to @p TRUE the support for PWMD4 is included.
 * @note    The default is @p TRUE.
 */
#if !defined(SPC5_PWM_USE_SMOD5) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_SMOD5                  TRUE
#endif

/**
 * @brief   PWMD7 driver enable switch.
 * @details If set to @p TRUE the support for PWMD4 is included.
 * @note    The default is @p TRUE.
 */
#if !defined(SPC5_PWM_USE_SMOD6) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_SMOD6                  FALSE
#endif

/**
 * @brief   PWMD8 driver enable switch.
 * @details If set to @p TRUE the support for PWMD4 is included.
 * @note    The default is @p TRUE.
 */
#if !defined(SPC5_PWM_USE_SMOD7) || defined(__DOXYGEN__)
#define SPC5_PWM_USE_SMOD7                  TRUE
#endif

/**
 * @brief   PWMD5 interrupt priority level setting.
 */
#if !defined(SPC5_PWM_SMOD4_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_PWM_SMOD4_PRIORITY          7
#endif

/**
 * @brief   PWMD6 interrupt priority level setting.
 */
#if !defined(SPC5_PWM_SMOD5_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_PWM_SMOD5_PRIORITY          7
#endif

/**
 * @brief   PWMD7 interrupt priority level setting.
 */
#if !defined(SPC5_PWM_SMOD6_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_PWM_SMOD6_PRIORITY          7
#endif

/**
 * @brief   PWMD8 interrupt priority level setting.
 */
#if !defined(SPC5_PWM_SMOD7_PRIORITY) || defined(__DOXYGEN__)
#define SPC5_PWM_SMOD7_PRIORITY          7
#endif

/**
 * @brief   FlexPWM-1 peripheral configuration when started.
 * @note    The default configuration is 1 (always run) in run mode and
 *          2 (only halt) in low power mode. The defaults of the run modes
 *          are defined in @p hal_lld.h.
 */
#if !defined(SPC5_PWM_FLEXPWM1_START_PCTL) || defined(__DOXYGEN__)
#define SPC5_PWM_FLEXPWM1_START_PCTL        (SPC5_ME_PCTL_RUN(1) |          \
                                             SPC5_ME_PCTL_LP(2))
#endif

/**
 * @brief   FlexPWM-1 peripheral configuration when stopped.
 * @note    The default configuration is 0 (never run) in run mode and
 *          0 (never run) in low power mode. The defaults of the run modes
 *          are defined in @p hal_lld.h.
 */
#if !defined(SPC5_PWM_FLEXPWM1_STOP_PCTL) || defined(__DOXYGEN__)
#define SPC5_PWM_FLEXPWM1_STOP_PCTL         (SPC5_ME_PCTL_RUN(0) |          \
                                             SPC5_ME_PCTL_LP(0))
#endif
#endif

/*===========================================================================*/
/* Configuration checks.                                                     */
/*===========================================================================*/

#if SPC5_PWM_USE_SMOD0 && !SPC5_HAS_FLEXPWM0
#error "SMOD0 not present in the selected device"
#endif

#if SPC5_PWM_USE_SMOD1 && !SPC5_HAS_FLEXPWM0
#error "SMOD1 not present in the selected device"
#endif

#if SPC5_PWM_USE_SMDO2 && !SPC5_HAS_FLEXPWM0
#error "SMOD2 not present in the selected device"
#endif

#if SPC5_PWM_USE_SMOD3 && !SPC5_HAS_FLEXPWM0
#error "SMOD3 not present in the selected device"
#endif

#if (!SPC5_PWM_USE_SMOD0 && !SPC5_PWM_USE_SMOD1 &&                           \
    !SPC5_PWM_USE_SMOD2 && !SPC5_PWM_USE_SMOD3) && SPC5_PWM_USE_FLEXPWM0
#error "PWM driver activated but no SubModule assigned"
#endif

#if SPC5_PWM_USE_SMOD4 && !SPC5_HAS_FLEXPWM1
#error "SMOD4 not present in the selected device"
#endif

#if SPC5_PWM_USE_SMOD5 && !SPC5_HAS_FLEXPWM1
#error "SMOD5 not present in the selected device"
#endif

#if SPC5_PWM_USE_SMDO6 && !SPC5_HAS_FLEXPWM1
#error "SMOD6 not present in the selected device"
#endif

#if SPC5_PWM_USE_SMOD7 && !SPC5_HAS_FLEXPWM1
#error "SMOD7 not present in the selected device"
#endif

#if (!SPC5_PWM_USE_SMOD4 && !SPC5_PWM_USE_SMOD5 &&                           \
    !SPC5_PWM_USE_SMOD6 && !SPC5_PWM_USE_SMOD7) && SPC5_PWM_USE_FLEXPWM1
#error "PWM driver activated but no SubModule assigned"
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/
#include "flexpwm.h"

/**
 * @brief PWM mode type.
 */
typedef uint32_t pwmmode_t;

/**
 * @brief   PWM channel type.
 */
typedef uint8_t pwmchannel_t;

/**
 * @brief   PWM counter type.
 */
typedef uint16_t pwmcnt_t;

/**
 * @brief   PWM driver channel configuration structure.
 */
typedef struct {
  /**
   * @brief Channel active logic level.
   */
  pwmmode_t                 mode;
  /**
   * @brief Channel callback pointer.
   * @note  This callback is invoked on the channel compare event. If set to
   *        @p NULL then the callback is disabled.
   */
  pwmcallback_t             callback;
  /* End of the mandatory fields.*/
} PWMChannelConfig;

/**
 * @brief   PWM driver configuration structure.
 */
typedef struct {
  /**
   * @brief   Timer clock in Hz.
   * @note    The low level can use assertions in order to catch invalid
   *          frequency specifications.
   */
  uint32_t                  frequency;
  /**
   * @brief   PWM period in ticks.
   * @note    The low level can use assertions in order to catch invalid
   *          period specifications.
   */
  pwmcnt_t                  period;
  /**
   * @brief Periodic callback pointer.
   * @note  This callback is invoked on PWM counter reset. If set to
   *        @p NULL then the callback is disabled.
   */
  pwmcallback_t             callback;
  /**
   * @brief Channels configurations.
   */
  PWMChannelConfig          channels[PWM_CHANNELS];
  /* End of the mandatory fields.*/
  /**
   * @brief PWM functional mode.
   */
  pwmmode_t                 mode;
} PWMConfig;

/**
 * @brief   Structure representing a PWM driver.
 */
struct PWMDriver {
  /**
   * @brief Driver state.
   */
  pwmstate_t                state;
  /**
   * @brief Current driver configuration data.
   */
  const PWMConfig           *config;
  /**
   * @brief   Current PWM period in ticks.
   */
  pwmcnt_t                  period;
#if defined(PWM_DRIVER_EXT_FIELDS)
  PWM_DRIVER_EXT_FIELDS
#endif
  /* End of the mandatory fields.*/
  /**
   * @Pointer to the volatile FlexPWM registers block.
   */
  volatile struct spc5_flexpwm *flexpwmp;
};

/*===========================================================================*/
/* Driver macros.                                                            */
/*===========================================================================*/


/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#if SPC5_PWM_USE_SMOD0 && !defined(__DOXYGEN__)
extern PWMDriver PWMD1;
#endif

#if SPC5_PWM_USE_SMOD1 && !defined(__DOXYGEN__)
extern PWMDriver PWMD2;
#endif

#if SPC5_PWM_USE_SMOD2 && !defined(__DOXYGEN__)
extern PWMDriver PWMD3;
#endif

#if SPC5_PWM_USE_SMOD3 && !defined(__DOXYGEN__)
extern PWMDriver PWMD4;
#endif

#if SPC5_PWM_USE_SMOD4 && !defined(__DOXYGEN__)
extern PWMDriver PWMD5;
#endif

#if SPC5_PWM_USE_SMOD5 && !defined(__DOXYGEN__)
extern PWMDriver PWMD6;
#endif

#if SPC5_PWM_USE_SMOD6 && !defined(__DOXYGEN__)
extern PWMDriver PWMD7;
#endif

#if SPC5_PWM_USE_SMOD7 && !defined(__DOXYGEN__)
extern PWMDriver PWMD8;
#endif

#ifdef __cplusplus
extern "C" {
#endif
  void pwm_lld_init(void);
  void pwm_lld_start(PWMDriver *pwmp);
  void pwm_lld_stop(PWMDriver *pwmp);
  void pwm_lld_enable_channel(PWMDriver *pwmp,
                              pwmchannel_t channel,
                              pwmcnt_t width);
  void pwm_lld_disable_channel(PWMDriver *pwmp, pwmchannel_t channel);
  void pwm_lld_change_period(PWMDriver *pwmp, pwmcnt_t period);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_PWM */

#endif /* _PWM_LLD_H_ */

/** @} */
