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

/*
 * SPC560Pxx drivers configuration.
 * The following settings override the default settings present in
 * the various device driver implementation headers.
 * Note that the settings for each driver only have effect if the whole
 * driver is enabled in halconf.h.
 *
 * IRQ priorities:
 * 1...15       Lowest...Highest.
 */

#define SPC563Mxx_MCUCONF

/*
 * HAL driver system settings.
 */
#define SPC5_NO_INIT                        FALSE
#define SPC5_CLK_BYPASS                     FALSE
#define SPC5_ALLOW_OVERCLOCK                FALSE
#define SPC5_CLK_PREDIV                     1
#define SPC5_CLK_MFD                        80
#define SPC5_CLK_RFD                        RFD_DIV4
#define SPC5_FLASH_BIUCR                    (BIUCR_BANK1_TOO |              \
                                            BIUCR_MASTER4_PREFETCH |       \
                                            BIUCR_MASTER0_PREFETCH |       \
                                            BIUCR_DPFEN |                  \
                                            BIUCR_IPFEN |                  \
                                            BIUCR_PFLIM_ON_MISS |          \
                                            BIUCR_BFEN)

/*
 * SERIAL driver system settings.
 */
#define SPC5_USE_ESCIA                      TRUE
#define SPC5_USE_ESCIB                      TRUE
#define SPC5_ESCIA_PRIORITY                 8
#define SPC5_ESCIB_PRIORITY                 8
