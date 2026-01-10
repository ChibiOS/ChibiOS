/*
    ChibiOS - Copyright (C) 2006..2025 Giovanni Di Sirio

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
 * @file    rp2350_imagedef.S
 * @brief   RP2350 PICOBIN boot block definitions.
 * @note    See RP2350 Datasheet 5.1.5 Blocks and block loops
 * @note    See RP2350 Datasheet 5.9.1 Blocks and block loops
 * @note    See RP2350 Datasheet 5.9.5 Minimum viable image metadata
 *
 * @addtogroup ARMCMx_RP2350_IMAGEDEF
 * @{
 */

/* Block markers */
#define PICOBIN_BLOCK_MARKER_START              0xffffded3
#define PICOBIN_BLOCK_MARKER_END                0xab123579

/* Block item type IDs */
#define PICOBIN_BLOCK_ITEM_1BS_IMAGE_TYPE       0x42
#define PICOBIN_BLOCK_ITEM_1BS_VECTOR_TABLE     0x03
#define PICOBIN_BLOCK_ITEM_1BS_ENTRY_POINT      0x44
#define PICOBIN_BLOCK_ITEM_2BS_LAST             0xff

/* Image type field values */
#define PICOBIN_IMAGE_TYPE_IMAGE_TYPE_EXE       0x0001  /* bits 0-3: type = 1 exe */
#define PICOBIN_IMAGE_TYPE_EXE_CPU_ARM          0x0000  /* bits 8-10: cpu = 0 ARM */
#define PICOBIN_IMAGE_TYPE_EXE_CPU_RISCV        0x0100  /* bits 8-10: cpu = 1 RISC-V */
#define PICOBIN_IMAGE_TYPE_EXE_CHIP_RP2350      0x1000  /* bits 12-14: chip = 1 RP2350 */
#define PICOBIN_IMAGE_TYPE_EXE_SECURITY_NS      0x0010  /* bits 4-5: security = 1 Non-Secure */
#define PICOBIN_IMAGE_TYPE_EXE_SECURITY_S       0x0020  /* bits 4-5: security = 2 Secure */

/*
 * The RP2350 supports ARM TrustZone with two security states:
 *   PICOBIN_IMAGE_TYPE_EXE_SECURITY_S  (0x0020) - Secure mode (default)
 *   PICOBIN_IMAGE_TYPE_EXE_SECURITY_NS (0x0010) - Non-Secure mode
 *
 * Unless you know what you are doing with TrustZone you want Secure Mode!
 */
#if !defined(RP2350_IMAGE_TYPE) || defined(__DOXYGEN__)
#define RP2350_IMAGE_TYPE       (PICOBIN_IMAGE_TYPE_IMAGE_TYPE_EXE |    \
                                 PICOBIN_IMAGE_TYPE_EXE_CPU_ARM |       \
                                 PICOBIN_IMAGE_TYPE_EXE_CHIP_RP2350 |   \
                                 PICOBIN_IMAGE_TYPE_EXE_SECURITY_S)
#endif

/*===========================================================================*/
/* Imagedef block - placed at start of flash                                 */
/*===========================================================================*/

#if !defined(__DOXYGEN__)

                .syntax unified
                .thumb

/*
 * PICOBIN image definition block.
 * The bootrom searches for this block within the first 4KB of flash.
 *
 * For our ChibiOS we use the following block structure:
 *   - MARKER_START
 *   - IMAGE_TYPE
 *   - VECTOR_TABLE
 *   - LAST
 *   - MARKER_END
 */
                .section .embedded_block, "a", %progbits
                .align  2
                .global __embedded_block
__embedded_block:
                .word   PICOBIN_BLOCK_MARKER_START

                .byte   PICOBIN_BLOCK_ITEM_1BS_IMAGE_TYPE
                .byte   0x01
                .hword  RP2350_IMAGE_TYPE

                .byte   PICOBIN_BLOCK_ITEM_1BS_VECTOR_TABLE
                .byte   0x02
                .hword  0x0000
                .word   __vectors_base__

                .byte   PICOBIN_BLOCK_ITEM_2BS_LAST
                .hword  ((__embedded_block_end - __embedded_block - 16) / 4)
                .byte   0x00
                .word   0x00000000

                .word   PICOBIN_BLOCK_MARKER_END
__embedded_block_end:

#endif /* !defined(__DOXYGEN__) */

/** @} */
