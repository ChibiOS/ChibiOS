/*
    SPC5 HAL - Copyright (C) 2013 STMicroelectronics

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
 * @file    SPC564Axx/spc564a_registry.h
 * @brief   SPC564Axx capabilities registry.
 *
 * @addtogroup HAL
 * @{
 */

#ifndef _SPC563M_REGISTRY_H_
#define _SPC563M_REGISTRY_H_

/*===========================================================================*/
/* Platform capabilities.                                                    */
/*===========================================================================*/

/**
 * @name    SPC564Axx capabilities
 * @{
 */
/* eDMA attributes.*/
#define SPC5_HAS_EDMA                       TRUE
#define SPC5_EDMA_NCHANNELS                 64
#define SPC5_EDMA_HAS_MUX                   FALSE

/* eQADC attributes.*/
#define SPC5_HAS_EQADC                      TRUE

/* eSCI attributes.*/
#define SPC5_HAS_ESCIA                      TRUE
#define SPC5_ESCIA_HANDLER                  vector146
#define SPC5_ESCIA_NUMBER                   146

#define SPC5_HAS_ESCIB                      TRUE
#define SPC5_ESCIB_HANDLER                  vector149
#define SPC5_ESCIB_NUMBER                   149

#define SPC5_HAS_ESCIC                      TRUE
#define SPC5_ESCIC_HANDLER                  vector473
#define SPC5_ESCIC_NUMBER                   473

/* SIU attributes.*/
#define SPC5_HAS_SIU                        TRUE
#define SPC5_SIU_SUPPORTS_PORTS             FALSE

/* EMIOS attributes.*/
#define SPC5_HAS_EMIOS                      TRUE

#define SPC5_EMIOS_NUM_CHANNELS		        24

#define SPC5_EMIOS_FLAG_F0_HANDLER          vector51
#define SPC5_EMIOS_FLAG_F1_HANDLER          vector52
#define SPC5_EMIOS_FLAG_F2_HANDLER          vector53
#define SPC5_EMIOS_FLAG_F3_HANDLER          vector54
#define SPC5_EMIOS_FLAG_F4_HANDLER          vector55
#define SPC5_EMIOS_FLAG_F5_HANDLER          vector56
#define SPC5_EMIOS_FLAG_F6_HANDLER          vector57
#define SPC5_EMIOS_FLAG_F7_HANDLER          vector58
#define SPC5_EMIOS_FLAG_F8_HANDLER          vector59
#define SPC5_EMIOS_FLAG_F9_HANDLER          vector60
#define SPC5_EMIOS_FLAG_F10_HANDLER         vector61
#define SPC5_EMIOS_FLAG_F11_HANDLER         vector62
#define SPC5_EMIOS_FLAG_F12_HANDLER         vector63
#define SPC5_EMIOS_FLAG_F13_HANDLER         vector64
#define SPC5_EMIOS_FLAG_F14_HANDLER         vector65
#define SPC5_EMIOS_FLAG_F15_HANDLER         vector66
#define SPC5_EMIOS_FLAG_F16_HANDLER         vector202
#define SPC5_EMIOS_FLAG_F17_HANDLER         vector203
#define SPC5_EMIOS_FLAG_F18_HANDLER         vector204
#define SPC5_EMIOS_FLAG_F19_HANDLER         vector205
#define SPC5_EMIOS_FLAG_F20_HANDLER         vector206
#define SPC5_EMIOS_FLAG_F21_HANDLER         vector207
#define SPC5_EMIOS_FLAG_F22_HANDLER         vector208
#define SPC5_EMIOS_FLAG_F23_HANDLER         vector209
#define SPC5_EMIOS_FLAG_F0_NUMBER           51
#define SPC5_EMIOS_FLAG_F1_NUMBER           52
#define SPC5_EMIOS_FLAG_F2_NUMBER           53
#define SPC5_EMIOS_FLAG_F3_NUMBER           54
#define SPC5_EMIOS_FLAG_F4_NUMBER           55
#define SPC5_EMIOS_FLAG_F5_NUMBER           56
#define SPC5_EMIOS_FLAG_F6_NUMBER           57
#define SPC5_EMIOS_FLAG_F7_NUMBER           58
#define SPC5_EMIOS_FLAG_F8_NUMBER           59
#define SPC5_EMIOS_FLAG_F9_NUMBER           60
#define SPC5_EMIOS_FLAG_F10_NUMBER          61
#define SPC5_EMIOS_FLAG_F11_NUMBER          62
#define SPC5_EMIOS_FLAG_F12_NUMBER          63
#define SPC5_EMIOS_FLAG_F13_NUMBER          64
#define SPC5_EMIOS_FLAG_F14_NUMBER          65
#define SPC5_EMIOS_FLAG_F15_NUMBER          66
#define SPC5_EMIOS_FLAG_F16_NUMBER          202
#define SPC5_EMIOS_FLAG_F17_NUMBER          203
#define SPC5_EMIOS_FLAG_F18_NUMBER          204
#define SPC5_EMIOS_FLAG_F19_NUMBER          205
#define SPC5_EMIOS_FLAG_F20_NUMBER          206
#define SPC5_EMIOS_FLAG_F21_NUMBER          207
#define SPC5_EMIOS_FLAG_F22_NUMBER          208
#define SPC5_EMIOS_FLAG_F23_NUMBER          209

#define SPC5_EMIOS_CLK                      (SPC5_SYSCLK /                  \
                                             SPC5_EMIOS_GLOBAL_PRESCALER)
#define SPC5_EMIOS_ENABLE_CLOCK()
#define SPC5_EMIOS_DISABLE_CLOCK()
/** @} */

#endif /* _SPC563M_REGISTRY_H_ */

/** @} */
