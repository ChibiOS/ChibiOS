# ClockGen v1

Status: draft

## Summary

ClockGen v1 is a clean-sheet code generation tool for MCU clock trees.

The core idea is simple:

- one XML file describes one MCU clock tree
- the XML declares a code generation namespace, for example `stm32`
- the generator emits `<namespace>_clocktree.h`
- the generator emits `<namespace>_limits.h`
- additional generated files are allowed, but are not fixed yet
- clock-tree settings come from `mcuconf.h` or `xmcuconf.h`
- board-related inputs come from `board.h`

The old `STM32G4xx_OLD` prototype is useful as a reference for the idea only.
ClockGen v1 is not constrained by that design.

Existing hand-written STM32 clock trees, including STM32H5, are useful as
reference inputs and migration targets, but they are not normative for the new
tool. The purpose of ClockGen is to describe clock trees more systematically,
reduce hand-made errors, and later normalize legacy family implementations.

## Primary Goal

Define a declarative XML format able to describe an MCU clock tree in terms of:

- internal clock sources
- external clock sources
- clock points

Generated clock points are macros representing clock frequencies in Hz.

Rules:

- a clock point evaluates to its calculated frequency when enabled
- a clock point evaluates to `0` when not enabled
- a clock point may be derived from a source
- a clock point may be derived from a mux
- a clock point may be derived from a divider
- a clock point may be derived from a multiplier
- muxes select among previously calculated clock points
- dividers may use different encoded hardware models

The tool is intended to generate new clock trees with fewer structural and
semantic errors than the current hand-maintained approach.

## Non-goals for v1

- defining all generated files beyond `<namespace>_clocktree.h`
- binding the design to current HAL/XHAL implementation details
- generating raw register programming sequences
- generating the full runtime clock switching implementation in this first spec
- covering non-clock MCU subsystems
- forcing immediate one-pass normalization of all existing hand-written family
  trees

## Generator Technology

ClockGen v1 shall be implemented using `freemarker-codegen`, the ChibiOS
FreeMarker customization designed for code generation.

Execution model:

- template authoring is done in `freemarker-codegen`
- template execution is done through FMPP
- FMPP uses `freemarker-codegen` as its template engine layer in this workflow

This is not optional in the v1 design. The generator is not specified as a
generic XML-to-code tool; it is specifically a ChibiOS FMPP plus
`freemarker-codegen` generator.

### Rationale

This matches the existing ChibiOS code generation stack and is also a better
fit for clock-tree generation than classic text-oriented FreeMarker templates.

Important properties of `freemarker-codegen` for this use case:

- code-first syntax suited to logic-heavy generators
- explicit output emission, reducing whitespace-related output errors
- support for code-generation-oriented expression syntax
- compatibility with existing FreeMarker/FMPP-based workflows

### Toolchain Contract

ClockGen v1 templates should therefore be designed for the FMPP plus
`freemarker-codegen` pipeline already used by ChibiOS updater and generation
scripts.

The specification assumes:

- XML input data
- FMPP configuration files driving generation
- template sources written for `freemarker-codegen`
- generated outputs written into the repository tree by the existing ChibiOS
  generation flow

## Conceptual Model

ClockGen v1 intentionally starts from a small model.

When current family headers differ from the preferred model, the preferred
ClockGen model takes precedence unless explicit compatibility constraints are
documented.

### Namespace

The XML declares a namespace used to prefix generated artifacts.

Example:

- namespace: `stm32`
- primary output: `stm32_clocktree.h`

The namespace is a code generation identity, not a silicon family constraint.

For C macro naming, the namespace also has an uppercase form.

Example:

- XML namespace: `stm32`
- C macro namespace: `STM32`

## Configuration Source

Clock-tree configuration settings come from:

- `mcuconf.h`, or
- `xmcuconf.h`

Board-related inputs come from:

- `board.h`

The generated `<namespace>_clocktree.h` does not include either file directly.

Integration model:

- `hal_lld.h` includes the configuration header
- `hal_lld.h` includes `board.h` through the normal HAL include path
- `hal_lld.h` includes `<namespace>_clocktree.h`
- generated clock-tree expressions therefore rely on settings already being
  visible when `<namespace>_clocktree.h` is parsed

This inclusion order is part of the v1 contract.

### Board Inputs

`board.h` provides board-related inputs to the clock tree.

Typical examples:

- external oscillator frequencies such as `STM32_HSECLK` and `STM32_LSECLK`
- oscillator bypass or drive-related options
- board supply definitions such as `VDD`
- any other board-specific electrical or clock-related setting required by the
  MCU family clock rules

These inputs are distinct from `mcuconf.h` or `xmcuconf.h` settings:

- `mcuconf.h` or `xmcuconf.h` selects the intended clock configuration
- `board.h` provides physical board characteristics and external clock values

For all externally provided inputs coming from `mcuconf.h`, `xmcuconf.h`, or
`board.h`, the generated clock-tree code must emit checks for:

- required macro presence
- allowed symbolic values
- allowed numeric ranges
- any other schema-defined validity constraints

## Operating Modes

HAL/XHAL can operate in two clock modes:

- static clock mode
- dynamic clock mode

ClockGen v1 must be designed to support both.

### Static clock mode

In static clock mode:

- the clock configuration is taken from `mcuconf.h` or `xmcuconf.h`
- calculated clock values are resolved from that configuration
- that configuration is applied statically at boot time
- the clock tree does not switch to alternative configurations at runtime

In this mode, the generated clock-tree headers primarily describe the single
boot-time configuration and its derived clock points.

### Dynamic clock mode

In dynamic clock mode:

- the clock configuration in `mcuconf.h` or `xmcuconf.h` is the default
  configuration
- that default configuration is typically applied at boot time
- the active clock configuration may later be switched at runtime
- additional runtime configurations are allowed beyond the default one

In this mode, the generated clock-tree model must remain valid for both:

- the default configuration described by `mcuconf.h` or `xmcuconf.h`
- runtime-switched configurations that follow the same MCU clock-tree rules

ClockGen v1 does not need to define the full runtime switch mechanism, but the
clock-tree description must not assume that only one configuration can ever
exist.

## Device Limits

Hard device clock limits are generated into:

- `<namespace>_limits.h`

The generated `<namespace>_clocktree.h` includes `<namespace>_limits.h`.

Those limits are used to validate calculated clock points against device
constraints.

Examples of such limits include:

- maximum system clock
- maximum bus clocks
- maximum peripheral kernel clocks
- PLL input and output limits
- oscillator-specific limits where relevant

Limits are not always globally constant. A common pattern is that active device
limits depend on power-related settings.

Typical examples:

- VOS or regulator range selection affecting maximum `SYSCLK`
- power mode selection affecting flash wait-state thresholds
- power configuration affecting allowable PLL or peripheral clock ceilings
- board-level electrical inputs such as `VDD` affecting allowable limits in
  families where voltage participates in clock constraints

ClockGen v1 must therefore support conditional limit sets selected by other
configuration settings or board-level inputs, especially power settings.

In dynamic clock mode, power-related settings may also change at runtime.
Because of that, `<namespace>_limits.h` must not export only the currently
selected active limits. It must also export limit macros for any defined power
mode or power profile that the clock tree can legally use.

This allows runtime clock-management code to validate a candidate
configuration against the limits of the target power mode before switching.

### Settings Naming

ClockGen v1 distinguishes two setting classes:

- register settings
- non-register settings

Register settings are not arbitrary values. They represent the actual values
that will be written into MCU registers by the HAL/XHAL low level code.

ClockGen v1 supports these practical naming forms, matching the current H5
style:

- whole-register settings
- symbolic field/selector settings

Whole-register settings represent the complete register image written to
hardware.

Example:

- `STM32_PWR_VOSCR`
- `STM32_PWR_BDCR`

Symbolic field/selector settings represent one logical register field or one
clock-related selector, but the macro name does not need to spell out the full
register path.

Examples:

- `STM32_SW`
- `STM32_PLL1SRC`
- `STM32_HPRE`
- `STM32_PPRE1`
- `STM32_USART1SEL`

The XML description must therefore carry the backing register and field
metadata independently from the C setting macro name.

Here `<namespace>` refers to the C macro namespace form, typically uppercase.

Non-register settings are either:

- boolean settings
- numeric value settings

Boolean settings are configuration switches and use `TRUE` or `FALSE` values.

Examples:

- `STM32_HSE_ENABLED`
- `STM32_HSI_ENABLED`
- `STM32_CSI_ENABLED`
- `STM32_CLOCK_DYNAMIC`

These boolean settings are intended for enable/disable decisions that are part
of the clock-tree configuration but are not themselves whole register images.

Numeric value settings are plain numeric parameters that do not directly match
the final register bitfield encoding.

These settings are postfixed by `_VALUE`.

Pattern:

- `<namespace>_<symbolic_name>_VALUE`

Examples:

- `STM32_HSIDIV_VALUE`
- `STM32_PLL1M_VALUE`
- `STM32_PLL1N_VALUE`
- `STM32_PLL1P_VALUE`

For these settings, the generated clock-tree code is responsible for deriving
the actual register field encoding from the numeric value.

The schema must be able to declare validity constraints for these values, such
as minimums, maximums, parity restrictions, or explicit allowed sets.

Reference note:

- current STM32H5 already uses `_VALUE` for PLL numeric parameters, which fits
  this rule
- current STM32H5 also uses `_VALUE` in a few places such as
  `STM32_RTCPRE_VALUE` and `STM32_MCO1PRE_VALUE` where the value behaves more
  like an encoded divider selector

ClockGen v1 should treat the numeric-parameter meaning as the target rule. H5
compatibility may therefore require normalizing a few legacy `_VALUE` settings
when that family is migrated to ClockGen.

This is intentional: existing family trees are migration references, not the
source of truth for new schema semantics.

### Internal Clock Sources

Internal sources are oscillators or internal roots provided by the MCU.

Examples:

- `HSI`
- `HSI16`
- `HSI48`
- `LSI`
- `MSI`

An internal source declares:

- symbolic name
- frequency expression
- enable condition
- optional description

Enable conditions may depend on non-register boolean settings such as
`<namespace>_*_ENABLED`.

Frequency or enable expressions may also reference field settings when an
internal source depends on PLL-style parameters.

Frequency expressions may also reference `_VALUE` settings when the generated
clock-tree logic needs the numeric parameter rather than the encoded bitfield.

### External Clock Sources

External sources are board- or application-provided clocks.

Examples:

- `HSE`
- `LSE`

An external source declares:

- symbolic name
- frequency expression or macro reference
- enable condition
- optional description

In practice these will often resolve to existing `board.h` macros.

Enable conditions may depend on non-register boolean settings such as
`<namespace>_*_ENABLED`.

External source frequency expressions may depend directly on `board.h` symbols.

The schema must be able to declare required presence and validity constraints
for those board-provided symbols.

### Clock Points

Clock points are named derived frequencies exported as macros.

Examples:

- `SYSCLK`
- `HCLK`
- `PCLK1`
- `PCLK2`
- `PLLCLK`
- `USART1CLK`

Each clock point declares:

- symbolic name
- description
- calculation kind
- inputs
- enable rule

Mandatory semantic rule:

- if the clock point is not enabled, its generated value is `0`

This rule applies even if the mathematical upstream expression would otherwise
produce a non-zero value.

## Clock Point Definition Kinds

ClockGen v1 only needs a small set of building blocks initially.

### 1. Source reference

A clock point can directly reference an internal or external source.

Example intent:

- `SYSCLK` comes from `HSI`

### 2. Mux

A clock point can be the result of selecting one input among multiple inputs.

Example intent:

- `SYSCLK` comes from one of `HSI`, `HSE`, or `PLL`

A mux definition needs:

- mux name
- an arbitrary number of selectable inputs
- selection expression
- optional default

Mux semantics:

- a mux selects from other calculated clock points
- each input maps one selector value to one upstream clock point
- the generator must not assume a fixed input count
- the selection expression is expected to come from a setting macro such as
  `<namespace>_<symbolic_field>` or, less commonly, a whole-register setting
- if the selected upstream clock point is `0`, then the mux output is `0`

### 3. Divider

A clock point can divide another clock point or source.

Example intent:

- `HCLK = SYSCLK / HPRE`
- `PCLK1 = HCLK / PPRE1`

A divider definition needs:

- upstream input
- divider kind
- division encoding
- optional allowed values or choices

ClockGen v1 must support these divider kinds:

- `0..N`
  - encoded range where `0` means disabled
- `1..N`
  - encoded range where the minimum valid divisor is `1`
- `2^N`
  - encoded exponent where the divisor is a power of two
- arbitrary enum
  - explicit mapping from encoded value to divisor

Divider semantics:

- a divider consumes one upstream calculated clock point
- the divider encoding is expected to come from a setting macro such as
  `<namespace>_<symbolic_field>` or `<namespace>_<symbolic_field>_VALUE`
- if the upstream clock point is `0`, then the divider output is `0`
- if the divider encoding selects a disabled state, the divider output is `0`
- otherwise the divider output is `upstream / divisor`

For `0..N` and `1..N` dividers driven by a `_VALUE` setting, the numeric value
may directly be the divisor itself.

When a divider or PLL-style parameter is configured through a `_VALUE` setting,
the generated clock-tree code must derive the encoded field value from that
numeric parameter before assembling the final register image.

### 4. Multiplier

A clock point can multiply another clock point by a numeric factor.

Example intent:

- `VCOOUT = VCOIN * N`

A multiplier definition needs:

- upstream input
- multiplication factor source
- optional allowed values or constraints

Multiplier semantics:

- a multiplier consumes one upstream calculated clock point
- the factor is expected to come from a setting macro such as
  `<namespace>_<symbolic_field>` or `<namespace>_<symbolic_field>_VALUE`
- if the upstream clock point is `0`, then the multiplier output is `0`
- otherwise the multiplier output is `upstream * factor`

### 5. PLL core

ClockGen v1 should not bake STM32-specific PLL shapes into the core schema.

In particular:

- `P`, `Q`, `R`, `M`, and `N` are not universal PLL concepts
- not all STM32 families expose the same PLL structure
- other MCU families may have very different synthesizer blocks

The generic reusable concept is therefore a PLL core centered on:

- one input `VCOIN` clock point
- one formula deriving `VCOOUT` from `VCOIN` and attributes
- one derived `VCOOUT` clock point
- a set of additional attributes and constraints

The intended split is:

- input muxes remain ordinary mux definitions outside the PLL core
- output branches remain ordinary divider definitions outside the PLL core
- the PLL core itself describes only the synthesizer behavior between
  `VCOIN` and `VCOOUT`

This allows STM32 PLLs to be described without making STM32 terminology part
of the core schema:

- input selection is modeled by a normal mux
- any pre-divider contributing to `VCOIN` is modeled as an ordinary divider
  node outside the PLL core
- `VCOOUT` is produced by a formula over `VCOIN` and PLL-core attributes
- exported outputs such as `P`, `Q`, or `R` are just family-specific ordinary
  dividers from `VCOOUT`

This is important because different MCU families may use:

- integer multiplication
- fractional multiplication
- additional correction or scaling terms
- other synthesizer-specific attributes that do not fit a single built-in
  multiplier model

### 6. Chained derivation

Clock points may depend on previously defined clock points.

Example intent:

- `PCLK1` depends on `HCLK`
- `TIMCLK1` depends on `PCLK1`

The generator must therefore treat the clock tree as a dependency graph.

## XML Content Requirements

The XML schema is now defined at the structural level by this spec.

### Root element

The root element shall be:

```xml
<clocktree schema="clockgen-v1" namespace="stm32">
```

Required root attributes:

- `schema`
  - fixed to `clockgen-v1`
- `namespace`
  - lowercase generator namespace used for output files

Optional root children:

- `<description>`
- `<settings>`
- `<board-inputs>`
- `<power-profiles>`
- `<limits>`
- `<sources>`
- `<clock-blocks>`
- `<clock-points>`

### Top-level structure

The top-level structure is:

```xml
<clocktree schema="clockgen-v1" namespace="stm32">
  <description>...</description>
  <settings>...</settings>
  <board-inputs>...</board-inputs>
  <power-profiles>...</power-profiles>
  <limits>...</limits>
  <sources>...</sources>
  <clock-blocks>...</clock-blocks>
  <clock-points>...</clock-points>
</clocktree>
```

### Settings section

`<settings>` declares inputs coming from `mcuconf.h` or `xmcuconf.h`.

Supported child elements:

- `<register-setting>`
- `<boolean-setting>`
- `<value-setting>`

#### Register settings

Use `<register-setting>` for whole-register values and symbolic selector values.

Required attributes:

- `name`
  - logical XML identifier such as `SW` or `PWR_VOSCR`
- `macro`
  - C macro name such as `STM32_SW` or `STM32_PWR_VOSCR`

Optional attributes:

- `register`
  - backing hardware register name
- `field`
  - backing hardware field name when applicable
- `required`
  - `true` or `false`, default `true`

Optional child elements:

- `<allowed macro="..."/>`
- `<range min="..." max="..."/>`
- `<description>`

#### Boolean settings

Use `<boolean-setting>` for `TRUE`/`FALSE` settings.

Required attributes:

- `name`
- `macro`

Optional attributes:

- `required`
  - `true` or `false`, default `true`

Optional child elements:

- `<description>`

#### Value settings

Use `<value-setting>` for `_VALUE` settings.

Required attributes:

- `name`
- `macro`

Optional attributes:

- `register`
- `field`
- `required`
  - `true` or `false`, default `true`
- `encode`
  - encoding rule name such as `identity`, `minus-1`, `even-only`,
    `power-of-two`, or another schema-defined encoding

Optional child elements:

- `<range min="..." max="..."/>`
- `<allowed value="..."/>`
- `<constraint kind="even"/>`
- `<description>`

### Board inputs section

`<board-inputs>` declares values coming from `board.h`.

Supported child element:

- `<board-input>`

Required attributes:

- `name`
- `macro`
- `kind`
  - for example `frequency`, `voltage`, `boolean`, or `value`

Optional attributes:

- `required`
  - `true` or `false`, default `true`

Optional child elements:

- `<range min="..." max="..."/>`
- `<allowed macro="..."/>`
- `<description>`

### Power profiles section

`<power-profiles>` declares named limit-selection contexts.

Supported child element:

- `<power-profile>`

Required attributes:

- `name`

Required child element:

- one or more `<when .../>` selectors

Supported selector forms:

- `<when setting="PWR_VOSCR" equals="PWR_VOSCR_VOS_RANGE0"/>`
- `<when board-input="VDD" min="2700" max="3600"/>`

This section exists because active limits may depend on runtime power settings
or board-level electrical conditions.

### Limits section

`<limits>` declares generated limits for `<namespace>_limits.h`.

Supported child elements:

- `<limit>`
- `<profile-limits>`

#### Global limit

Use `<limit>` for unconditional limits.

Required attributes:

- `name`
- `value`

#### Profile-specific limits

Use `<profile-limits>` for conditional limit groups.

Required attributes:

- `profile`

Child elements:

- `<limit name="..." value="..."/>`

This is the primary way to model VOS-dependent or power-dependent limit tables.

### Sources section

`<sources>` declares oscillator and root clock sources.

Supported child elements:

- `<internal-source>`
- `<external-source>`

Both require:

- `name`

Optional child elements:

- `<enabled>`
- `<frequency>`
- `<description>`

`<external-source>` is expected to use `board.h` macro references inside
`<frequency>`.

### Clock blocks section

`<clock-blocks>` declares reusable compound synthesizer blocks.

Supported child elements:

- `<pll-core>`

This section is intentionally small in v1. It exists so the schema can model
common synthesizer behavior without baking vendor-specific output names into
top-level clock-point primitives.

#### PLL core

Use `<pll-core>` for a generic synthesizer block that transforms `VCOIN` into
`VCOOUT`.

Required attributes:

- `name`

Required child elements:

- `<vcoin ref="..."/>`
- `<vcoout-formula>...</vcoout-formula>`

Optional child elements:

- `<attribute name="..." setting="..."/>`
- `<attribute name="..." value-setting="..."/>`
- `<attribute name="..." const="..."/>`
- `<input-range min="..." max="..."/>`
- `<output-range min="..." max="..."/>`
- `<constraint kind="..."/>`
- `<description>`

`VCOIN` and `VCOOUT` are generic internal PLL-core stages, not generated
public macro names by themselves unless the family mapping explicitly exports
them.

`<vcoout-formula>` defines how `VCOOUT` is calculated from:

- `VCOIN`
- one or more declared PLL-core attributes
- integer constants where needed

The core requirement is the dependency relationship and validation metadata,
not the assumption that every PLL behaves as a simple integer multiplier.

#### PLL attribute schema

`<attribute>` declares a named PLL-core parameter used by constraints or the
`<vcoout-formula>`.

Required attributes:

- `name`

Then exactly one of:

- `setting="..."`
- `value-setting="..."`
- `const="..."`

Optional child elements:

- `<range min="..." max="..."/>`
- `<allowed value="..."/>`
- `<constraint kind="..."/>`
- `<description>`

#### PLL formula schema

`<vcoout-formula>` is a structural arithmetic expression evaluated in the PLL
core context.

Supported primitive nodes:

- `<const value="..."/>`
- `<stage-ref stage="VCOIN"/>`
- `<attribute-ref name="..."/>`

Supported arithmetic nodes:

- `<mul>...</mul>`
- `<div>...</div>`
- `<add>...</add>`
- `<sub>...</sub>`

For v1, arithmetic is integer arithmetic only.

### Clock points section

`<clock-points>` declares generated clock-point macros.

Supported child element:

- `<clock-point>`

Required attributes:

- `name`

Optional child elements:

- `<description>`
- `<enabled>`
- exactly one calculation block

Supported calculation blocks:

- `<from-source ref="..."/>`
- `<from-clock ref="..."/>`
- `<mux ...>...</mux>`
- `<divider ...>...</divider>`
- `<multiplier .../>`
- `<from-block-stage block="..." stage="..."/>`

### Mux schema

A `<mux>` calculation block requires:

- `setting`
  - logical setting name that selects the input

Child elements:

- one or more `<input value="..." ref="..."/>`

Optional attributes:

- `default`

Each `value` refers to the symbolic macro value selected by the backing
setting.

### Divider schema

A `<divider>` calculation block requires:

- `input`
  - upstream source or clock point name
- `kind`
  - one of `0-n`, `1-n`, `pow2`, `enum`

Then one of:

- `setting="..."`
- `value-setting="..."`

Optional child elements:

- `<div value="0" disabled="true"/>`
- `<div value="1" divisor="1"/>`
- `<div value="2" divisor="2"/>`

For `enum`, explicit `<div>` children are required.

For `0-n`, `1-n`, and `pow2`, the encoding rule may be expressed through
attributes plus optional constraints instead of listing every value.

### Multiplier schema

A `<multiplier>` calculation block requires:

- `input`
  - upstream source or clock point name

Then one of:

- `setting="..."`
- `value-setting="..."`
- `factor="..."`

Optional child elements:

- `<range min="..." max="..."/>`
- `<allowed value="..."/>`
- `<constraint kind="..."/>`

This primitive is intentionally generic. It is used both for simple standalone
multiplied clocks. PLL cores may use it indirectly through their
`<vcoout-formula>` if that matches the target family model, but it is not
mandatory inside a PLL block.

### Block-stage reference schema

`<from-block-stage>` references a named internal stage exported by a compound
clock block.

Required attributes:

- `block`
- `stage`

For v1, the primary intended stages are:

- `VCOIN`
- `VCOOUT`

### Minimal example

```xml
<clocktree schema="clockgen-v1" namespace="stm32">
  <description>STM32H5-style clock tree</description>

  <settings>
    <boolean-setting name="HSE_ENABLED" macro="STM32_HSE_ENABLED"/>
    <boolean-setting name="HSI_ENABLED" macro="STM32_HSI_ENABLED"/>
    <register-setting name="SW" macro="STM32_SW"
                      register="RCC_CFGR1" field="SW">
      <allowed macro="RCC_CFGR1_SW_HSI"/>
      <allowed macro="RCC_CFGR1_SW_HSE"/>
      <allowed macro="RCC_CFGR1_SW_PLL1P"/>
    </register-setting>
    <register-setting name="PLL1SRC" macro="STM32_PLL1SRC"
                      register="RCC_PLL1CFGR" field="PLL1SRC">
      <allowed macro="RCC_PLL1CFGR_PLL1SRC_HSI"/>
      <allowed macro="RCC_PLL1CFGR_PLL1SRC_HSE"/>
    </register-setting>
    <value-setting name="PLL1M" macro="STM32_PLL1M_VALUE"
                   register="RCC_PLL1CFGR" field="PLL1M"
                   encode="identity">
      <range min="1" max="63"/>
    </value-setting>
    <value-setting name="PLL1N" macro="STM32_PLL1N_VALUE"
                   register="RCC_PLL1DIVR" field="PLL1N"
                   encode="identity">
      <range min="4" max="512"/>
    </value-setting>
    <value-setting name="PLL1P" macro="STM32_PLL1P_VALUE"
                   register="RCC_PLL1DIVR" field="PLL1P"
                   encode="identity">
      <range min="2" max="128"/>
    </value-setting>
    <register-setting name="HPRE" macro="STM32_HPRE"
                      register="RCC_CFGR2" field="HPRE"/>
    <register-setting name="PWR_VOSCR" macro="STM32_PWR_VOSCR"
                      register="PWR_VOSCR"/>
  </settings>

  <board-inputs>
    <board-input name="HSECLK" macro="STM32_HSECLK" kind="frequency">
      <range min="4000000" max="50000000"/>
    </board-input>
    <board-input name="VDD" macro="VDD" kind="voltage" required="false"/>
  </board-inputs>

  <power-profiles>
    <power-profile name="VOS0">
      <when setting="PWR_VOSCR" equals="PWR_VOSCR_VOS_RANGE0"/>
    </power-profile>
    <power-profile name="VOS1">
      <when setting="PWR_VOSCR" equals="PWR_VOSCR_VOS_RANGE1"/>
    </power-profile>
  </power-profiles>

  <limits>
    <profile-limits profile="VOS0">
      <limit name="SYSCLK_MAX" value="250000000"/>
      <limit name="PCLK1_MAX" value="250000000"/>
    </profile-limits>
    <profile-limits profile="VOS1">
      <limit name="SYSCLK_MAX" value="200000000"/>
      <limit name="PCLK1_MAX" value="200000000"/>
    </profile-limits>
  </limits>

  <sources>
    <internal-source name="HSI">
      <enabled>
        <setting-ref name="HSI_ENABLED"/>
      </enabled>
      <frequency>
        <const value="64000000"/>
      </frequency>
    </internal-source>
    <external-source name="HSE">
      <enabled>
        <setting-ref name="HSE_ENABLED"/>
      </enabled>
      <frequency>
        <board-ref name="HSECLK"/>
      </frequency>
    </external-source>
  </sources>

  <clock-blocks>
    <pll-core name="PLL1">
      <vcoin ref="PLL1VCOIN"/>
      <attribute name="N" value-setting="PLL1N">
        <range min="4" max="512"/>
      </attribute>
      <input-range min="1000000" max="16000000"/>
      <output-range min="150000000" max="836000000"/>
      <vcoout-formula>
        <mul>
          <stage-ref stage="VCOIN"/>
          <attribute-ref name="N"/>
        </mul>
      </vcoout-formula>
    </pll-core>
  </clock-blocks>

  <clock-points>
    <clock-point name="PLL1SRCCLK">
      <mux setting="PLL1SRC">
        <input value="RCC_PLL1CFGR_PLL1SRC_HSI" ref="HSI"/>
        <input value="RCC_PLL1CFGR_PLL1SRC_HSE" ref="HSE"/>
      </mux>
    </clock-point>

    <clock-point name="PLL1VCOIN">
      <divider input="PLL1SRCCLK" kind="1-n" value-setting="PLL1M"/>
    </clock-point>

    <clock-point name="PLL1VCOOUT">
      <from-block-stage block="PLL1" stage="VCOOUT"/>
    </clock-point>

    <clock-point name="PLL1P">
      <divider input="PLL1VCOOUT" kind="1-n" value-setting="PLL1P"/>
    </clock-point>

    <clock-point name="SYSCLK">
      <mux setting="SW">
        <input value="RCC_CFGR1_SW_HSI" ref="HSI"/>
        <input value="RCC_CFGR1_SW_HSE" ref="HSE"/>
        <input value="RCC_CFGR1_SW_PLL1P" ref="PLL1P"/>
      </mux>
    </clock-point>

    <clock-point name="HCLK">
      <divider input="SYSCLK" kind="enum" setting="HPRE">
        <div value="RCC_CFGR2_HPRE_DIV1" divisor="1"/>
        <div value="RCC_CFGR2_HPRE_DIV2" divisor="2"/>
        <div value="RCC_CFGR2_HPRE_DIV4" divisor="4"/>
      </divider>
    </clock-point>
  </clock-points>
</clocktree>
```

This example is illustrative. It defines the intended schema shape, not a full
STM32H5 migration file.

## Expression Semantics

The generator must be able to emit C preprocessor expressions for the clock
values.

The preferred schema form is structural. Primitive expression nodes are:

- `<const value="..."/>`
- `<setting-ref name="..."/>`
- `<board-ref name="..."/>`
- `<clock-ref name="..."/>`
- `<stage-ref stage="..."/>`
- `<attribute-ref name="..."/>`

At minimum the expression model must support:

- integer constants
- macro references
- source references
- clock point references
- block-stage references
- PLL attribute references
- mux selection
- division
- multiplication
- addition
- subtraction

Macro references include configuration settings coming from `mcuconf.h` or
`xmcuconf.h`, typically following the
`<namespace>_<symbolic_name>` or
`<namespace>_<symbolic_name>_VALUE` naming conventions.

Those macro references can be either:

- register settings representing actual register values
- boolean settings representing enable switches
- numeric `_VALUE` settings representing plain numeric parameters

Clock-tree expressions may also reference board-level macros coming from
`board.h`.

The XML should prefer structural mux and divider elements over free-form
expressions for encoded hardware fields.

The first implementation should avoid a large general-purpose expression
language. The XML should stay structural and clock-specific.

## Generated Output

### Required output

The generator must emit:

- `<namespace>_clocktree.h`
- `<namespace>_limits.h`

These headers are the stable v1 output contract.

### Minimum header contents

`<namespace>_clocktree.h` must contain generated clock value macros for the
declared clock points.

Properties of generated macros:

- one macro per clock point
- value is frequency in Hz
- value is `0` when the clock point is disabled
- names are derived deterministically from XML names
- expressions may depend on configuration macros already included via
  `hal_lld.h`

The final macro naming convention is still open, but it must be namespace-safe
and predictable.

`<namespace>_limits.h` must contain generated hard device limits used by the
clock-tree checks.

Properties of generated limit macros:

- represent hard device constraints
- are expressed as C preprocessor macros
- are included by `<namespace>_clocktree.h`

### Additional outputs

ClockGen v1 may emit more files later, for example helper headers or source
fragments, but those outputs are not part of the core spec yet.

In addition to generated C headers, the implementation is expected to include
the supporting FMPP configuration and updater-script glue needed to run the
generator in the standard ChibiOS workflow.

## Dependency and Validation Rules

The generator must validate the clock tree before emitting code.

Required checks:

- required external input macros are present
- external symbolic settings use allowed values
- external numeric settings are within allowed ranges
- no duplicate source names
- no duplicate clock point names
- all references resolve
- no dependency cycles
- muxes have at least one input
- mux inputs reference valid calculable clock points
- divider definitions are structurally valid
- divider kinds are explicit
- enum divider mappings are unambiguous
- multiplier definitions are structurally valid
- block-stage references resolve
- PLL-core definitions expose valid internal stages and constraints
- PLL-core formulas reference only valid stages and declared attributes

If the XML is invalid, generation must fail.

Generated clock-tree code must also validate calculated clock points against
the hard limits defined in `<namespace>_limits.h`.

When limits depend on power-related settings, validation must be performed
against the active limit set selected by those settings.

In dynamic mode, validation of a runtime configuration switch must be performed
against the limit set associated with the target power mode, not just the
current one.

One of the explicit reasons for ClockGen is to move these checks from scattered
hand-written family logic into a more uniform generated model.

## Clean-Sheet Constraints

The design should not assume the old prototype structure.

Specifically, ClockGen v1 should not inherit these limitations unless later
chosen deliberately:

- hard-coded `CLK_` naming model
- STM32-specific concepts baked into the schema
- output limited to the exact old `clocktree.h` form
- mux-only modeling of derived clocks
- STM32-specific PLL output names baked into the core XML model

The schema should be MCU-clock-centric first, with STM32 as the first intended
consumer.

Current hand-written families may influence compatibility choices, but they do
not define correctness by themselves.

## Open Questions

These items remain intentionally open:

- exact generated macro naming convention
- whether clock-point enable rules are always explicit or may be inferred
- what additional generated files should exist beyond `<namespace>_clocktree.h`
- how much of current HAL/XHAL integration should be generated versus
  handwritten
- how aggressively existing family trees should be normalized during migration
  versus preserved behind compatibility shims
- whether the schema should eventually be formalized as XSD, Relax NG, or a
  custom validator over the XML tree

## Initial Direction for Next Revision

The next revision of this spec should add:

- a proposed naming convention for generated macros
- precise enable semantics for sources and clock points
- a complete catalog of supported expression nodes and encoding rules
- a decision on whether source frequencies can be zero independently of enable
  state
