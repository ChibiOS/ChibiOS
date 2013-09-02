*****************************************************************************
*** ChibiOS products directory organization                               ***
*****************************************************************************

--{root}                - Distribution directory.
  +--os/                - ChibiOS products, this directory.
  |  +--ext/            - Vendor files.
  |  +--common/         - Files used by multiple ChibiOS products.
  |  |  +--ports        - Common port files for various architectures and
  |  |                    compilers.
  |  +--hal/            - Hardware Abstraction Layer product.
  |  |  +--include/     - HAL high level headers.
  |  |  +--src/         - HAL high level sources.
  |  |  +--platforms/   - HAL low level drivers implementations.
  |  |  +--templates/   - Driver template files.
  |  |     +--meta/     - Driver meta templates.
  |  +--rt/             - ChibiOS/RT kernel portable files.
  |  |  +--include/     - RT kernel headers.
  |  |  +--src/         - RT kernel sources.
  |  |  +--templates/   - RT kernel port template files.
  |  |  +--ports        - RT kernel port files for various architectures and
  |  |                    compilers.
  |  +--nil/            - ChibiOS/NIL kernel portable files.
  |  |  +--include/     - Nil kernel headers.
  |  |  +--src/         - Nil kernel sources.
  |  |  +--templates/   - Nil kernel port template files.
  |  |  +--ports        - Nil kernel port files for various architectures and
  |  |                    compilers.
  |  +--various/        - Various portable support files.
