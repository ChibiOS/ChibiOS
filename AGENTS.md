# Repository Guidelines

## Project Structure & Module Organization
- Operating system modules are under `./os`.
- Core STM32 HAL sources live under `os/hal/ports/STM32`.
- XHAL sources live under `os/xhal`; generated driver interfaces and common code come from XML descriptions under `os/xhal/codegen`.
- Shared STM32 infrastructure (RCC helpers, registry, limits) sits alongside in the same tree, while board examples and demos are under `demos/`.
- Tests and validation harnesses reside in the `test*` directories; use them to verify changes before submitting.
- Configuration templates and regeneration helpers are under `tools/ftl` and `tools/updater`; generated `xhalconf.h`/`xmcuconf.h` outputs should be treated as derived files.

## Coding Style & Naming Conventions
- Follow existing style patterns: 2-space indentation for C sources, and `STM32_*` macro naming for register constants (see `hal_lld.h`/`stm32_rcc.h`).
- Keep functions `static` unless they are part of the public HAL API.
- Regenerate formatting by hand; clang-format is not used on this port—mirror the surrounding code style.
- General rule: line endings must be LF, except for externally provided files (non-ChibiOS copyright).
- In C functions, keep automatic variable declarations grouped at the start of the block and separate the declaration block from executable statements with an empty line.
- In ChibiOS, API suffix semantics are critical and must drive design decisions: no suffix = thread context, `X` = any context, `S` = system-locked context, `I` = interrupt/system-locked context. Semantic compatibility comes before implementation convenience; never place calls in fastcall/ISR paths unless their suffix contract explicitly allows it.
- Driver callbacks must follow their context contract. Generic XHAL driver callbacks are invoked from ISR context and out of system locks; do not call them from thread-context APIs, and do not invoke them while holding `osalSysLock*()`/`chSysLock*()`. If an ISR helper must wake waiters, lock only around the `I`-class wakeup/VRQ operations, unlock, then invoke the callback.
- While debugging new code, prefer enabling assertions, parameter checks, and the state checker in `chconf.h`. In ChibiOS, contract violations often halt early with those options enabled, which is desirable for catching context and state misuse quickly.
- STM32 shared IRQ handlers follow the `.inc` pattern used by SPI/USART/RTC: platform `stm32_isr.c` includes the right `.inc`, vector names and numbers belong in `stm32_isr.h`, and the corresponding `driver.mk` must export the include directory that contains the shared `.inc` files.
- For STM32 vectors shared by multiple peripherals, choose one peripheral as the primary owner and treat the others as secondaries. The shared `.inc` filename and the IRQ priority macro must name all participating peripherals, while the primary peripheral defines the overall handler structure and inclusion point.
- For primary/secondary shared vectors, keep the ISR body high level. The `.inc` should dispatch to driver service entry points, while peripheral-specific flag decoding and per-channel/per-instance detail should live inside the driver-side service functions unless the hardware has a dedicated vector and the established pattern already keeps that logic in the `.inc`.
- XML driver descriptions must remain schema-valid and include the documentation elements expected by code generation; otherwise generated Doxygen comments disappear and regeneration becomes lossy.

## XHAL Architecture Notes
- XHAL stateful drivers derive from `hal_base_driver`; lifecycle, configuration selection, mutual exclusion, and registry integration should be exposed through the base-driver API when the semantics are common.
- `drvStart()` and `drvStartS()` are the official XHAL lifecycle entry points. Driver-specific `xxxStart()`/`xxxStop()` wrappers should not be reintroduced unless there is a deliberate compatibility layer outside the generated XHAL driver API.
- `drvStart(ip, config)` performs initial hardware enable and configuration from `HAL_DRV_STATE_STOP`; `drvStart(ip, NULL)` uses configuration zero. Calling `drvStart(ip, NULL)` while already `HAL_DRV_STATE_READY` is idempotent, but passing a non-`NULL` config while ready is invalid.
- Live configuration APIs such as `drvSetCfgX()` and `drvSelectCfgX()` are only valid in `HAL_DRV_STATE_READY`. Use the `config` parameter of `drvStart()` for initial configuration because hardware may be reset or clock-gated before start.
- Driver-specific low-level start implementations need to be state-aware only where live reconfiguration requires it: STOP start enables resources and configures hardware, READY reconfiguration changes settings without tearing down resources.
- Synchronization is driver-specific in XHAL. Do not add generic synchronization methods to `hal_base_driver`; expose waits at driver level with names or parameters that identify the wait condition, such as SIO RX/TX waits or ADC state synchronization.
- XHAL synchronization feature switches use the `XXX_USE_SYNCHRONIZATION` naming convention. Do not introduce new `XXX_USE_WAIT` settings for driver synchronization; keep `XXX_USE_WAIT` only for legacy or non-driver-event semantics such as PAL wait support.
- Some waits are intentionally not generic driver synchronization points. Examples include SIO directional waits (`RX`, `RX idle`, `TX space`, `TX end`), USB endpoint-zero setup waits, ETH/CAN descriptor or mailbox waits, ICU capture waits, and flash erase polling.
- XHAL generated configuration migration should be done through `tools/updater/update_xhalconf.sh` after updating `tools/ftl/processors/conf/xhalconf/xhalconf.h.ftl`; do not manually edit only local `xhalconf.h` copies.
- XHAL generated driver sources and headers are derived from XML under `os/xhal/codegen`; when changing generated behavior, update the XML description and the checked-in generated file together.
- SB VIO host code must use public XHAL APIs and must not call XHAL low-level driver entry points directly. Fastcall handlers are restricted to `X`-class APIs and lock-free state queries; operations requiring thread context, blocking behavior, or non-`X` APIs belong in the syscall path.
- SB VIO sandbox LLDs should avoid requiring sandbox-visible hardware configuration structures. Prefer opaque configuration indices and VIO query operations for runtime state needed by sandbox code.

## Build & Test Hygiene
- Clean projects after test builds unless otherwise specified.
- For generated or updater-managed configuration files such as `mcuconf.h`, `xmcuconf.h`, `halconf.h`, `xhalconf.h`, and similar outputs, update the corresponding `.ftl` templates and/or updater scripts as part of the same change; otherwise a regeneration pass may overwrite manual edits.
- When changing XHAL driver settings, update both the `.ftl` templates and the generated `xhalconf.h`/`xmcuconf.h` files.
- Each demo or test must contain its own configuration files in its local `cfg/` directory. Do not use cross-demo or cross-test include shims for `chconf.h`, `halconf.h`, `xhalconf.h`, `mcuconf.h`, `xmcuconf.h`, or similar configuration headers.
- Eclipse metadata policy: keep `.project` and `.cproject` when useful for multi-target projects, but do not add generated `debug/*.launch` files unless there is a specific request to version them.
- `tools/updater` scripts share temporary files, so do not run them in parallel; regenerate sequentially.

## Repository
- Repository is subversion, ignore .svn directory, do not use git commands.
- Ask for confirmation before touching non-versioned files.
- Be careful when running `svn add` in newly built demo/test trees: generated `build/` and `.dep/` contents can be scheduled accidentally. Clean derived outputs first, or revert those paths before the final add/status check.
