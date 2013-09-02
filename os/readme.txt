*****************************************************************************
*** ChibiOS products directory organization                               ***
*****************************************************************************

--{root}                - Distribution directory.
  +--os/                - ChibiOS products, this directory.
  |  +--rt/             - ChibiOS/RT product.
  |  |  +--include/     - RT kernel headers.
  |  |  +--src/         - RT kernel sources.
  |  |  +--templates/   - RT kernel port template files.
  |  |  +--ports        - RT kernel port files for various architectures and
  |  |                    compilers.
  |  +--nil/            - ChibiOS/NIL product.
  |  |  +--include/     - Nil kernel headers.
  |  |  +--src/         - Nil kernel sources.
  |  |  +--templates/   - Nil kernel port template files.
  |  |  +--ports        - Nil kernel port files for various architectures and
  |  |                    compilers.
  |  +--hal/            - ChibiOS/HAL product.
  |  |  +--include/     - HAL high level headers.
  |  |  +--src/         - HAL high level sources.
  |  |  +--templates/   - HAL port template files.
  |  |  +--ports/       - HAL low level drivers implementations.
  |  |  +--boards/      - HAL board files.
  |  +--common/         - Files used by multiple ChibiOS products.
  |  |  +--ports        - Common port files for various architectures and
  |  |                    compilers.
  |  +--various/        - Various portable support files.
  |  +--ext/            - Vendor files used by ChibiOS products.
