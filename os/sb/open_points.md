# SB Open Points

This file tracks the current known follow-up work in `os/sb/`.
It is not meant to be a strict roadmap; it is a grouped backlog of the
remaining technical points across the SB subsystems.

## Common

- Review the SB syscall namespace in `common/sbsysc.h` after the recent VETH addition and keep the fastcall/syscall split semantically strict.
- Keep the ETH, SPI, UART, GPIO VIO call layout aligned and documented so client and host ports evolve together.
- Add a short protocol note for VETH in `os/sb/` describing the current direct-handle model, its assumptions, and the intended future zero-copy extension.

## Host

- Re-review syscall return and VRQ paths in `host/sbsyscall.c` and `host/sbvrq.c`, especially around restart/teardown windows and late producers.
- Decide whether sandbox memory-range violations in host services should keep returning `CH_RET_EFAULT` or escalate to a sandbox fault/termination policy.
- Add more host-side validation tests for multi-image bring-up, because incorrect flashing order can produce misleading startup failures during debug.
- Evaluate whether a host-side integration demo/test should be added for the working host + SB1 + SB2 configuration used during VETH/lwIP bring-up.

## Host VIO

### ETH

- Decide and document the final VETH ABI shape.
  Current model: direct validated native handles across the boundary.
  Alternative model kept as a backup: host-generated slot tokens.
- Add a real receive-size query or equivalent contract for copy-mode VETH so the client does not need to infer RX length from MTU-sized buffers.
- Add optional zero-copy support later, only as an explicit capability:
  host-mapped packet buffers, no descriptor sharing.
- Improve host-side VETH validation in `vio/sbvio_eth.c`.
  Current validation is intentionally structural, not ownership-aware.
- Review restart behavior and stale-handle behavior once sandbox restart and VETH reuse are exercised more aggressively.

### SPI

- Resolve the in-tree `TODO enforce fault instead.` sites in `vio/sbvio_spi.c` for invalid client buffers and invalid stop-result buffers.
- Decide whether SPI should continue returning `CH_RET_EFAULT` or should actively fault the sandbox on invalid memory access attempts.
- Add explicit tests for invalid-range handling and callback/VRQ behavior under malformed client requests.

### UART

- Resolve the in-tree `TODO enforce fault instead.` sites in `vio/sbvio_uart.c`.
- Review async read/write semantics and event signaling under heavy traffic and restart conditions.
- Verify whether the exported configuration copy remains sufficient if SIO configuration structures evolve.

### GPIO

- Review whether the current permission model in `vio/sbvio_gpio.c` is enough for future use cases or if read/write/setmode permissions need to be made more granular.

## User

- Re-review the SB user syscall wrappers in `user/sbuser.h` with emphasis on calling-context assumptions and early startup behavior.
- Add more user-side notes about the required flashing/debugging order for host + multiple sandbox images.
- Expand user-side helper coverage only after the host-side contracts are considered stable, especially for VETH.

## Apps

- Revisit the sample applications in `apps/` and decide which ones should become maintained regression/demo apps versus historical examples.
- Add a minimal networking-oriented SB sample once the current host + VETH + lwIP path is considered stable enough to preserve as a regression baseline.

## Cleanup

- Remove or archive the stale backup files `vio/sbvio_eth (copy).c` and `vio/sbvio_eth (copy).h` if they are no longer needed.
- Periodically sync this file with resolved items so it stays a useful working backlog rather than a historical dump.
