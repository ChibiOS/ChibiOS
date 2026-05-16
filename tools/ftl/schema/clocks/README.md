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
- Terminal peripheral users are modeled in the optional top-level `consumers`
  list. A consumer names an input clock point and may specify a preprocessor
  `condition`; active consumers create downstream demand for `auto` clock
  points without becoming clock points themselves. An `auto` clock point with
  no downstream clock point or explicit consumer is treated as unconditionally
  demanded; add consumers only when demand has a condition.
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

## Clock Demand Modeling Procedure

Use `enable="always"` only for clock points that are intrinsically required by
the platform or that intentionally represent an unconditional configuration
selection. Core clocks such as `SYSCLK`, `HCLK`, `PCLKx`, and mandatory helper
points normally stay always-enabled.

Use `enable="auto"` for optional peripheral clocks and intermediate helper
clocks. Auto clocks are enabled when a downstream clock point or explicit
consumer demands them. This avoids optional peripherals accidentally enabling or
requiring upstream oscillators.

Use one consumer per independent reason to demand a clock. This keeps generated
`<POINT>_ENABLED` expressions readable and diagnostics meaningful. Prefer two
consumers over one large condition when two drivers or features can require the
same clock.

Driver-owned peripheral clocks should use existing HAL and STM32 instance
switches in their consumer conditions. Examples:

```xml
<consumer name="SPI1_DRIVER" input="SPI1"
          condition="(HAL_USE_SPI == TRUE) &amp;&amp; (STM32_SPI_USE_SPI1 == TRUE)">
  <description>SPI driver demands the SPI1 clock when enabled.</description>
</consumer>

<consumer name="USB1_DRIVER" input="USB"
          condition="(HAL_USE_USB == TRUE) &amp;&amp; (STM32_USB_USE_USB1 == TRUE)">
  <description>USB driver demands the USB clock when enabled.</description>
</consumer>
```

If several instances of one driver share a clock point, keep the driver part as
one AND term and OR only the instance switches inside the second term:

```xml
<consumer name="ADCDAC_ADC_DRIVER" input="ADCDAC"
          condition="(HAL_USE_ADC == TRUE) &amp;&amp; ((STM32_ADC_USE_ADC1 == TRUE) || (STM32_ADC_USE_ADC2 == TRUE))">
  <description>ADC driver demands the ADCDAC clock when enabled.</description>
</consumer>
```

If separate drivers can demand the same clock point, use separate consumers
instead of one top-level OR expression:

```xml
<consumer name="ADCDAC_ADC_DRIVER" input="ADCDAC"
          condition="(HAL_USE_ADC == TRUE) &amp;&amp; ((STM32_ADC_USE_ADC1 == TRUE) || (STM32_ADC_USE_ADC2 == TRUE))">
  <description>ADC driver demands the ADCDAC clock when enabled.</description>
</consumer>
<consumer name="ADCDAC_DAC_DRIVER" input="ADCDAC"
          condition="(HAL_USE_DAC == TRUE) &amp;&amp; ((STM32_DAC_USE_DAC1_CH1 == TRUE) || (STM32_DAC_USE_DAC1_CH2 == TRUE))">
  <description>DAC driver demands the ADCDAC clock when enabled.</description>
</consumer>
```

For clocks not owned by an existing driver setting, add a local boolean
configuration named `<POINT>_REQUIRED` using the configured configuration
prefix in generated code. The XML config name does not include the prefix:

```xml
<config name="OCTOSPI_REQUIRED" type="bool" default="FALSE">
  <description>Enables demand for the OCTOSPI clock.</description>
</config>

<consumer name="OCTOSPI_REQUIRED" input="OCTOSPI"
          condition="STM32_CFG_OCTOSPI_REQUIRED == TRUE">
  <description>Local option demands the OCTOSPI clock when enabled.</description>
</consumer>
```

Derived helper clocks such as intermediate muxes and dividers should usually be
`auto` and have no direct consumer. They become enabled through downstream
dependencies. For example, an intermediate divided clock used only by USB should
be demanded by the USB clock point, not by its own local option.

Clock points with no defined consumers are treated as unconditionally demanded.
This is useful for always-present generated outputs but means optional
peripheral clocks must have consumers if their demand is conditional.

Use `NONE` inputs only for real selectable no-clock mux values, such as output
pin clocks or backup-domain selectors that explicitly support a no-clock
selection. Do not use `NONE` to model a disabled peripheral selector encoding.

If a disabled peripheral clock requires register selector bits, put those bits
in clock-level disabled bits:

```xml
<clock point="RNG" enable="auto" dynamic="no">
  <description>RNG clock</description>
  <bits enabled="0U" disabled="RCC_CCIPR2_RNGSEL_IGNORE" />
  <mux name="RNG">
    <input point="HSI48"><bits value="RCC_CCIPR2_RNGSEL_HSI48" /></input>
    <input point="MSIK" default="yes"><bits value="RCC_CCIPR2_RNGSEL_MSIK" /></input>
  </mux>
</clock>
```

Expression style matters. Consumer conditions are parsed by generator helpers
for formatting and diagnostics, so keep them simple:

- Good: `(HAL_USE_SPI == TRUE) && (STM32_SPI_USE_SPI1 == TRUE)`
- Good: `(HAL_USE_ADC == TRUE) && ((STM32_ADC_USE_ADC1 == TRUE) || (STM32_ADC_USE_ADC2 == TRUE))`
- Avoid: one large top-level OR expression combining unrelated drivers.

When converting an existing clock point to conditional demand:

- Change the clock point from `enable="always"` to `enable="auto"`.
- Add a driver consumer or a local `<POINT>_REQUIRED` config plus consumer.
- Keep intermediate helper clocks `auto` and let downstream dependencies demand
  them.
- Move disabled-state selector values from fake `NONE` inputs to clock-level
  disabled bits unless no-clock is a real mux selection.
- Regenerate and inspect the affected `<POINT>_ENABLED` macros.
- Inspect upstream dependency checks to ensure they are conditional on the new
  enabled macro.
- Inspect affected `<POINT>_BITS` macros if disabled register bits are involved.
- Inspect affected `<POINT>_FREQ` macros and confirm disabled clocks produce
  `0U`.
- Run XML validation, FMPP generation, and the clocktree test suite.

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
