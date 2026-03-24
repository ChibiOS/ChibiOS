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
 * @file    EFLv1/rp_efl_lld.h
 * @brief   RP EFL shared internal interfaces.
 */

#ifndef RP_EFL_LLD_H
#define RP_EFL_LLD_H

#if (HAL_USE_EFL == TRUE) || defined(__DOXYGEN__)

#if !defined(__ASSEMBLER__)

#ifdef __cplusplus
extern "C" {
#endif
  void rp_efl_lld_init(void);
  void rp_efl_lld_start(EFlashDriver *eflp);
  void rp_efl_lld_program_page_full(EFlashDriver *eflp,
                                    uint32_t offset,
                                    const uint8_t *data,
                                    size_t len);
  void rp_efl_lld_erase_full(EFlashDriver *eflp,
                             uint8_t cmd,
                             uint32_t offset);
  void rp_efl_lld_read_uid_full(EFlashDriver *eflp,
                                uint8_t *rx,
                                size_t count);
#ifdef __cplusplus
}
#endif

#endif /* !__ASSEMBLER__ */

#endif /* HAL_USE_EFL == TRUE */

#endif /* RP_EFL_LLD_H */
