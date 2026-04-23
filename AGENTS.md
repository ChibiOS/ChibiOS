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
- In ChibiOS, API suffix semantics are critical and must drive design decisions: no suffix = thread context, `X` = any context, `S` = system-locked context, `I` = interrupt/system-locked context. Semantic compatibility comes before implementation convenience; never place calls in fastcall/ISR paths unless their suffix contract explicitly allows it.
- While debugging new code, prefer enabling assertions, parameter checks, and the state checker in `chconf.h`. In ChibiOS, contract violations often halt early with those options enabled, which is desirable for catching context and state misuse quickly.
- STM32 shared IRQ handlers follow the `.inc` pattern used by SPI/USART/RTC: platform `stm32_isr.c` includes the right `.inc`, vector names and numbers belong in `stm32_isr.h`, and the corresponding `driver.mk` must export the include directory that contains the shared `.inc` files.
- For STM32 vectors shared by multiple peripherals, choose one peripheral as the primary owner and treat the others as secondaries. The shared `.inc` filename and the IRQ priority macro must name all participating peripherals, while the primary peripheral defines the overall handler structure and inclusion point.
- For primary/secondary shared vectors, keep the ISR body high level. The `.inc` should dispatch to driver service entry points, while peripheral-specific flag decoding and per-channel/per-instance detail should live inside the driver-side service functions unless the hardware has a dedicated vector and the established pattern already keeps that logic in the `.inc`.
- XML driver descriptions must remain schema-valid and include the documentation elements expected by code generation; otherwise generated Doxygen comments disappear and regeneration becomes lossy.

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
