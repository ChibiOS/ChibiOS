This directory contains the ChibiOS bindings for the lwIP TCP/IP stack:
http://savannah.nongnu.org/projects/lwip

Layout
------

- `lwip_common.mk`
  Common source list and include paths shared by all bindings.

- `lwip_hal.mk`
  Binding for the classic HAL MAC driver API.

- `lwip_xhal.mk`
  Binding for the XHAL ETH driver API.

- `lwipthread.[ch]`
  Common wrapper thread used to bring up lwIP, manage link state, dispatch
  incoming frames and expose a uniform integration API to applications.

- `lwipthread_hal.c`
  HAL MAC specific backend.

- `lwipthread_xhal.c`
  XHAL ETH specific backend.

- `arch/`
  ChibiOS-specific lwIP architecture layer.

Usage
-----

1. Place lwIP under `ext/lwip`.
2. In the application makefile include one of:
   - `$(CHIBIOS)/os/various/lwip_bindings/lwip_hal.mk`
   - `$(CHIBIOS)/os/various/lwip_bindings/lwip_xhal.mk`
3. Provide the usual application configuration files:
   - `lwipopts.h`
   - `chconf.h`
   - `halconf.h` or `xhalconf.h`
   - MCU/port-specific configuration headers as needed
4. Call `lwipInit()` from the application, then start higher-level lwIP
   services such as `httpd_init()`.

Notes
-----

- The binding is backend-neutral at the `lwipthread` level. HAL and XHAL
  differences are isolated in `lwipthread_hal.c` and `lwipthread_xhal.c`.

- Ethernet driver configuration is owned by the application/driver, not by
  `lwipthread`. The effective MAC address is taken from the selected/applied
  driver configuration.

- In XHAL mode the ETH driver must be enabled explicitly and currently
  requires:
  - `HAL_USE_ETH == TRUE`
  - `ETH_USE_SYNCHRONIZATION == TRUE`
  - `ETH_USE_EVENTS == TRUE`

- In HAL mode the classic MAC driver must be enabled with
  `HAL_USE_MAC == TRUE`.

- The XHAL backend currently supports the normal copy path. Zero-copy support
  is optional and depends on the selected ETH implementation.
