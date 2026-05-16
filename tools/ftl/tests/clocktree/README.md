Clock Tree Generator Fixtures
=============================

This directory contains small synthetic clock tree XML files used to exercise
the clock tree schema and generator logic independently from any STM32 device.

The `valid` directory contains XMLs expected to validate against
`tools/ftl/schema/clocks/clocktree.xsd` and generate a `clocktree.h`
successfully.  Each file focuses on one generator feature so regressions can be
isolated without using a full MCU clock tree.

The `invalid/schema` directory contains XMLs expected to fail XSD validation.
The `invalid/generator` directory contains schema-valid XMLs expected to fail
during generation because they violate semantic rules enforced by the
FreeMarker library.
