/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

    Licensed under the Apache License, Version 2.0 (the "License");
    you may not use this file except in compliance with the License.
    You may obtain a copy of the License at

        http://www.apache.org/licenses/LICENSE-2.0

    Unless required by applicable law or agreed to in writing, software
    distributed under the License is distributed on an "AS IS" BASIS,
    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
    See the License for the specific language governing permissions and
    limitations under the License.
*/

#ifndef USBCFG_H
#define USBCFG_H

#include "hal.h"
#include "hal_serial_usb.h"

#ifdef __cplusplus
extern "C" {
#endif
  extern hal_usb_binder_c usbcdc_binder;
  extern SerialUSBDriver PORTAB_SDU1;
  extern const SerialUSBConfig serusbcfg;

  void usbcdcObjectInit(void);
#ifdef __cplusplus
}
#endif

#endif /* USBCFG_H */
