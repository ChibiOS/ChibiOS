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
- XML driver descriptions must remain schema-valid and include the documentation elements expected by code generation; otherwise generated Doxygen comments disappear and regeneration becomes lossy.

## Build & Test Hygiene
- Clean projects after test builds unless otherwise specified.
- For generated configuration files such as `mcuconf.h`, `halconf.h`, and similar outputs, update the corresponding generator templates or updater scripts as part of the same change; otherwise a regeneration pass may overwrite manual edits.
- When changing XHAL driver settings, update both the `.ftl` templates and the generated `xhalconf.h`/`xmcuconf.h` files.
- `tools/updater` scripts share temporary files, so do not run them in parallel; regenerate sequentially.

## Repository
- Repository is subversion, ignore .svn directory, do not use git commands.
- Ask for confirmation before touching non-versioned files.
