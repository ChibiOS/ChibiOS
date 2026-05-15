# Clock Tree Generator Notes

This generator is a work-in-progress restart point for STM32 clock tree
generation. It does not need to support the old XML format.

## Files

- Main template: `tools/ftl/processors/clocktree/clocktree.h.ftl`
- Codegen library: `tools/ftl/libs/libclocks.ftlc`
- Schema: `tools/ftl/schema/clocks/clocktree.xsd`
- G4 test XML: `os/hal/ports/STM32/STM32G4xx_TEST/cfg/clocktree.xml`
- Generated header: `os/hal/ports/STM32/STM32G4xx_TEST/clocktree.h`
- Prototype HAL port: `os/hal/ports/STM32/STM32G4xx_TEST`
- Stable build target:
  `demos/STM32/RT-STM32-MULTI/make/stm32g474re_nucleo64_clocktree.make`

## Template Setup

The `.ftl` file is intentionally mostly text and delegates logic to
`libclocks.ftlc`. Do not use mixed syntax in `.ftl`; codegen code should stay in
`.ftlc` library functions and macros.

FMPP does not differentiate between `.ftl` and `.ftlc`, and FMPP-specific
directives are expected to work in both. If `.ftlc` syntax unexpectedly fails,
first verify that the FMPP installation is using the latest freemarker-codegen
jar.

## Current XML Model

- `settings/prefixes` provides clock point, macro, and configuration prefixes.
- `settings/suffixes` provides frequency, value, bits, selector, enable, and
  derived enabled-state suffixes.
- `settings/configs/config` provides arbitrary pre-compile settings emitted in
  XML order.
- Generic configurations currently support `bool`, `value`, `set`, and `raw`
  types. `bool`, constrained `value`, and `set` configurations emit generated
  compile-time checks.
- Semantic generator settings are marked by `role`; `CLOCK_DYNAMIC` is a
  generic boolean config name with role `dynamic_mode`, emitted using the
  configured configuration prefix.
- Clock `enable` is required and currently accepts `manual`, `auto`, `always`,
  and `never`.
- Clock `dynamic` is required and marks membership in the dynamic clock point
  set, independently from enable mode.
- `settings/states`, `settings/limit-set`, and `settings/limit-values` define
  the selectable frequency limit states, the final limit macro set, and the
  state-specific values. The generator validates that each state provides all
  required final limits.
- Conditional state-specific limits use ordered `<case when="..."/>` elements
  followed by exactly one `<default/>` fallback. Simple unconditional limits can
  still use direct `min` and `max` attributes on the `<limit>` element.
- Clock points can specify optional `<limits ref="..."/>` references to final
  limit names. The generator emits the selected `<limit>_MIN` and `<limit>_MAX`
  definitions and uses them for frequency range checks. Disabled clock points
  are not range-checked.
- Mux inputs and scaler choices use nested `<bits value="..."/>` elements, not
  bits attributes.
- Divider and multiplier nodes must specify their input clock point.
- Mux, divider, and multiplier inputs are checked against the declared clock
  points.
- Selection bits for scaler values can use `%N%` as the configured numeric
  value placeholder.

## Generated Contract

Each clock point should emit, in XML order:

- configuration settings where applicable;
- an `<POINT><enabled-suffix>` macro;
- an `<POINT>_BITS` macro;
- an `<POINT><frequency-suffix>` macro, with disabled clocks producing zero
  frequency;
- compile-time value checks for generated configuration settings;
- grouped state-specific frequency limit definitions and one selected final
  limit block where a limit model is declared;
- compile-time frequency range checks where clock point limits are declared.

Generic configurations emit explicit settings using the configured
configuration prefix and their local names, for example `CLOCK_DYNAMIC` becomes
`STM32_CFG_CLOCK_DYNAMIC`. Manual clock points emit explicit configuration
settings using the configured configuration prefix and enable suffix, for example
`STM32_CFG_HSI16_ENABLE`. Auto clock points derive enablement from downstream
use. Always and never clock points still emit their own enabled flags for
uniformity.

Clock definitions with enable-state bits use a clock-level `<bits enabled="..."
disabled="..."/>` element. Those bits are combined with selection bits when the
clock has a mux/divider/multiplier selection.

## Validation Commands

Regenerate the test header:

```sh
fmpp -C os/hal/ports/STM32/STM32G4xx_TEST/cfg/config.fmpp
```

Validate the XML:

```sh
xmllint --noout --schema tools/ftl/schema/clocks/clocktree.xsd \
  os/hal/ports/STM32/STM32G4xx_TEST/cfg/clocktree.xml
```

Build the stable compile target:

```sh
make -C demos/STM32/RT-STM32-MULTI \
  -f make/stm32g474re_nucleo64_clocktree.make -j2
make -C demos/STM32/RT-STM32-MULTI \
  -f make/stm32g474re_nucleo64_clocktree.make clean
```

The Eclipse configuration is named:
`Build for STM32G474RE-Nucleo64 Clocktree`.

## Current Prototype State

`STM32G4xx_TEST/hal_lld.h` includes the generated `clocktree.h` after static
RCC/PWR/platform definitions and provides compatibility aliases expected by
existing STM32 LLD drivers. `STM32G4xx_TEST/hal_lld.c` is only a compile/link
stub for now; it is not execution-ready.

The immediate goal is to make the generated header progressively replace the
handwritten clock macro layer while preserving the legacy macro interface that
drivers consume.
