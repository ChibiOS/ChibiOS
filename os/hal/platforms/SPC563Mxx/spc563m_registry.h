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
 * @file    SPC563Mxx/spc563m_registry.h
 * @brief   SPC563Mxx capabilities registry.
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
 * @name    SPC563Mxx capabilities
 * @{
 */
/* DSPI attribures.*/
#define SPC5_HAS_DSPI0                      FALSE
#define SPC5_HAS_DSPI1                      TRUE
#define SPC5_HAS_DSPI2                      TRUE
#define SPC5_HAS_DSPI3                      FALSE
#define SPC5_HAS_DSPI4                      FALSE
#define SPC5_DSPI_FIFO_DEPTH                16
#define SPC5_DSPI1_TX1_DMA_CH_ID            12
#define SPC5_DSPI1_TX2_DMA_CH_ID            25
#define SPC5_DSPI1_RX_DMA_CH_ID             13
#define SPC5_DSPI2_TX1_DMA_CH_ID            14
#define SPC5_DSPI2_TX2_DMA_CH_ID            26
#define SPC5_DSPI2_RX_DMA_CH_ID             15
#define SPC5_DSPI1_EOQF_HANDLER             vector132
#define SPC5_DSPI1_EOQF_NUMBER              132
#define SPC5_DSPI1_TFFF_HANDLER             vector133
#define SPC5_DSPI1_TFFF_NUMBER              133
#define SPC5_DSPI2_EOQF_HANDLER             vector137
#define SPC5_DSPI2_EOQF_NUMBER              137
#define SPC5_DSPI2_TFFF_HANDLER             vector138
#define SPC5_DSPI2_TFFF_NUMBER              138
#define SPC5_DSPI1_ENABLE_CLOCK()
#define SPC5_DSPI1_DISABLE_CLOCK()
#define SPC5_DSPI2_ENABLE_CLOCK()
#define SPC5_DSPI2_DISABLE_CLOCK()

/* eDMA attributes.*/
#define SPC5_HAS_EDMA                       TRUE
#define SPC5_EDMA_NCHANNELS                 32
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

#define SPC5_HAS_ESCIC                      FALSE

/* SIU attributes.*/
#define SPC5_HAS_SIU                        TRUE
#define SPC5_SIU_SUPPORTS_PORTS             FALSE

/* EMIOS attributes.*/
#define SPC5_HAS_EMIOS                      TRUE

#define SPC5_EMIOS_NUM_CHANNELS             16

#define SPC5_EMIOS_FLAG_F0_HANDLER          vector51
#define SPC5_EMIOS_FLAG_F1_HANDLER          vector52
#define SPC5_EMIOS_FLAG_F2_HANDLER          vector53
#define SPC5_EMIOS_FLAG_F3_HANDLER          vector54
#define SPC5_EMIOS_FLAG_F4_HANDLER          vector55
#define SPC5_EMIOS_FLAG_F5_HANDLER          vector56
#define SPC5_EMIOS_FLAG_F6_HANDLER          vector57
#define SPC5_EMIOS_FLAG_F8_HANDLER          vector59
#define SPC5_EMIOS_FLAG_F9_HANDLER          vector60
#define SPC5_EMIOS_FLAG_F10_HANDLER         vector61
#define SPC5_EMIOS_FLAG_F11_HANDLER         vector62
#define SPC5_EMIOS_FLAG_F12_HANDLER         vector63
#define SPC5_EMIOS_FLAG_F13_HANDLER         vector64
#define SPC5_EMIOS_FLAG_F14_HANDLER         vector65
#define SPC5_EMIOS_FLAG_F15_HANDLER         vector66
#define SPC5_EMIOS_FLAG_F23_HANDLER         vector209
#define SPC5_EMIOS_FLAG_F0_NUMBER           51
#define SPC5_EMIOS_FLAG_F1_NUMBER           52
#define SPC5_EMIOS_FLAG_F2_NUMBER           53
#define SPC5_EMIOS_FLAG_F3_NUMBER           54
#define SPC5_EMIOS_FLAG_F4_NUMBER           55
#define SPC5_EMIOS_FLAG_F5_NUMBER           56
#define SPC5_EMIOS_FLAG_F6_NUMBER           57
#define SPC5_EMIOS_FLAG_F8_NUMBER           59
#define SPC5_EMIOS_FLAG_F9_NUMBER           60
#define SPC5_EMIOS_FLAG_F10_NUMBER          61
#define SPC5_EMIOS_FLAG_F11_NUMBER          62
#define SPC5_EMIOS_FLAG_F12_NUMBER          63
#define SPC5_EMIOS_FLAG_F13_NUMBER          64
#define SPC5_EMIOS_FLAG_F14_NUMBER          65
#define SPC5_EMIOS_FLAG_F15_NUMBER          66
#define SPC5_EMIOS_FLAG_F23_NUMBER          209

#define SPC5_EMIOS_CLK                      (SPC5_SYSCLK /                  \
                                             SPC5_EMIOS_GLOBAL_PRESCALER)
#define SPC5_EMIOS_ENABLE_CLOCK()
#define SPC5_EMIOS_DISABLE_CLOCK()
/** @} */

#endif /* _SPC563M_REGISTRY_H_ */

/** @} */
