/*
 * rp2350_idle_loops.S — per-core idle loops for RP2350 SMP.
 *
 * Pure assembly so the C compiler cannot collapse them.  Core0 spins at
 * rp2350_idle_c0, Core1 spins at rp2350_idle_c1.
 *
 * .balign 4 before each symbol guarantees they fall in consecutive
 * 4-byte-aligned words → consecutive striped-SRAM banks.  The RP2350 bus
 * fabric can service simultaneous fetches from different banks without
 * arbitration stalls.
 *
 * Both symbols are in .ramtext so they live in SRAM, safe from flash XIP
 * blackouts during flash erase/write.
 */

    .thumb
    .syntax unified

    .section .ramtext, "ax", %progbits

    .balign 4
    .global rp2350_idle_c0
    .type   rp2350_idle_c0, %function
rp2350_idle_c0:
    b       rp2350_idle_c0
    .size   rp2350_idle_c0, . - rp2350_idle_c0

    .balign 4               /* next 4-byte word = consecutive bank, no conflict */
    .global rp2350_idle_c1
    .type   rp2350_idle_c1, %function
rp2350_idle_c1:
    nop                     /* distinct first opcode, also pads to 4 bytes total */
    b       rp2350_idle_c1
    .size   rp2350_idle_c1, . - rp2350_idle_c1
