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
/* eSCI attributes.*/
#define SPC5_HAS_ESCIA                      TRUE
#define SPC5_ESCIA_HANDLER                  vector146
#define SPC5_ESCIA_NUMBER                   146

#define SPC5_HAS_ESCIB                      TRUE
#define SPC5_ESCIB_HANDLER                  vector149
#define SPC5_ESCIB_NUMBER                   149

/* SIU attributes.*/
#define SPC5_HAS_SIU                        TRUE
#define SPC5_SIU_SUPPORTS_PORTS             FALSE
/** @} */

#endif /* _SPC563M_REGISTRY_H_ */

/** @} */
