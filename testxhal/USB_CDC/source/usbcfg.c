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

#include "hal.h"
#include "portab.h"

#include "usbcfg.h"

SerialUSBDriver PORTAB_SDU1;
hal_usb_binder_c usbcdc_binder;

#define USB1_DATA_REQUEST_EP              1U
#define USB1_DATA_AVAILABLE_EP            1U
#define USB1_INTERRUPT_REQUEST_EP         2U

#define USB_DESC_INDEX(i)                 ((uint8_t)(i))
#define USB_DESC_BYTE(b)                  ((uint8_t)(b))
#define USB_DESC_WORD(w)                  USB_DESC_BYTE(w), USB_DESC_BYTE((w) >> 8)
#define USB_DESC_BCD(bcd)                 USB_DESC_WORD(bcd)

#define USB_DESC_DEVICE_SIZE              18U
#define USB_DESC_CONFIGURATION_SIZE       9U
#define USB_DESC_INTERFACE_SIZE           9U
#define USB_DESC_ENDPOINT_SIZE            7U

#define USB_DESC_DEVICE(bcdUSB, bDeviceClass, bDeviceSubClass,              \
                        bDeviceProtocol, bMaxPacketSize, idVendor,          \
                        idProduct, bcdDevice, iManufacturer, iProduct,      \
                        iSerialNumber, bNumConfigurations)                  \
  USB_DESC_BYTE(USB_DESC_DEVICE_SIZE),                                      \
  USB_DESC_BYTE(USB_DESCRIPTOR_DEVICE),                                     \
  USB_DESC_BCD(bcdUSB),                                                     \
  USB_DESC_BYTE(bDeviceClass),                                              \
  USB_DESC_BYTE(bDeviceSubClass),                                           \
  USB_DESC_BYTE(bDeviceProtocol),                                           \
  USB_DESC_BYTE(bMaxPacketSize),                                            \
  USB_DESC_WORD(idVendor),                                                  \
  USB_DESC_WORD(idProduct),                                                 \
  USB_DESC_BCD(bcdDevice),                                                  \
  USB_DESC_INDEX(iManufacturer),                                            \
  USB_DESC_INDEX(iProduct),                                                 \
  USB_DESC_INDEX(iSerialNumber),                                            \
  USB_DESC_BYTE(bNumConfigurations)

#define USB_DESC_CONFIGURATION(wTotalLength, bNumInterfaces,                \
                               bConfigurationValue, iConfiguration,         \
                               bmAttributes, bMaxPower)                     \
  USB_DESC_BYTE(USB_DESC_CONFIGURATION_SIZE),                               \
  USB_DESC_BYTE(USB_DESCRIPTOR_CONFIGURATION),                              \
  USB_DESC_WORD(wTotalLength),                                              \
  USB_DESC_BYTE(bNumInterfaces),                                            \
  USB_DESC_BYTE(bConfigurationValue),                                       \
  USB_DESC_INDEX(iConfiguration),                                           \
  USB_DESC_BYTE(bmAttributes),                                              \
  USB_DESC_BYTE(bMaxPower)

#define USB_DESC_INTERFACE(bInterfaceNumber, bAlternateSetting,             \
                           bNumEndpoints, bInterfaceClass,                  \
                           bInterfaceSubClass, bInterfaceProtocol,          \
                           iInterface)                                      \
  USB_DESC_BYTE(USB_DESC_INTERFACE_SIZE),                                   \
  USB_DESC_BYTE(USB_DESCRIPTOR_INTERFACE),                                  \
  USB_DESC_BYTE(bInterfaceNumber),                                          \
  USB_DESC_BYTE(bAlternateSetting),                                         \
  USB_DESC_BYTE(bNumEndpoints),                                             \
  USB_DESC_BYTE(bInterfaceClass),                                           \
  USB_DESC_BYTE(bInterfaceSubClass),                                        \
  USB_DESC_BYTE(bInterfaceProtocol),                                        \
  USB_DESC_INDEX(iInterface)

#define USB_DESC_ENDPOINT(bEndpointAddress, bmAttributes, wMaxPacketSize,   \
                          bInterval)                                        \
  USB_DESC_BYTE(USB_DESC_ENDPOINT_SIZE),                                    \
  USB_DESC_BYTE(USB_DESCRIPTOR_ENDPOINT),                                   \
  USB_DESC_BYTE(bEndpointAddress),                                          \
  USB_DESC_BYTE(bmAttributes),                                              \
  USB_DESC_WORD(wMaxPacketSize),                                            \
  USB_DESC_BYTE(bInterval)

/*
 * USB Device Descriptor.
 */
static const uint8_t vcom_device_descriptor_data[18] = {
  USB_DESC_DEVICE(0x0110, 0x02, 0x00, 0x00, 0x40,
                  0x0483, 0x5740, 0x0200, 1, 2, 3, 1)
};

static const usb_descriptor_t vcom_device_descriptor = {
  sizeof vcom_device_descriptor_data,
  vcom_device_descriptor_data
};

static const uint8_t vcom_configuration_descriptor_data[67] = {
  USB_DESC_CONFIGURATION(67, 0x02, 0x01, 0, 0xC0, 50),
  USB_DESC_INTERFACE(0x00, 0x00, 0x01, 0x02, 0x02, 0x01, 0),
  USB_DESC_BYTE(5),
  USB_DESC_BYTE(0x24),
  USB_DESC_BYTE(0x00),
  USB_DESC_BCD(0x0110),
  USB_DESC_BYTE(5),
  USB_DESC_BYTE(0x24),
  USB_DESC_BYTE(0x01),
  USB_DESC_BYTE(0x00),
  USB_DESC_BYTE(0x01),
  USB_DESC_BYTE(4),
  USB_DESC_BYTE(0x24),
  USB_DESC_BYTE(0x02),
  USB_DESC_BYTE(0x02),
  USB_DESC_BYTE(5),
  USB_DESC_BYTE(0x24),
  USB_DESC_BYTE(0x06),
  USB_DESC_BYTE(0x00),
  USB_DESC_BYTE(0x01),
  USB_DESC_ENDPOINT(USB1_INTERRUPT_REQUEST_EP | 0x80U, 0x03, 0x0008, 0xFF),
  USB_DESC_INTERFACE(0x01, 0x00, 0x02, 0x0A, 0x00, 0x00, 0x00),
  USB_DESC_ENDPOINT(USB1_DATA_AVAILABLE_EP, 0x02, 0x0040, 0x00),
  USB_DESC_ENDPOINT(USB1_DATA_REQUEST_EP | 0x80U, 0x02, 0x0040, 0x00)
};

static const usb_descriptor_t vcom_configuration_descriptor = {
  sizeof vcom_configuration_descriptor_data,
  vcom_configuration_descriptor_data
};

static const uint8_t vcom_string0[] = {
  USB_DESC_BYTE(4),
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING),
  USB_DESC_WORD(0x0409)
};

static const uint8_t vcom_string1[] = {
  USB_DESC_BYTE(38),
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING),
  'S', 0, 'T', 0, 'M', 0, 'i', 0, 'c', 0, 'r', 0, 'o', 0, 'e', 0,
  'l', 0, 'e', 0, 'c', 0, 't', 0, 'r', 0, 'o', 0, 'n', 0, 'i', 0,
  'c', 0, 's', 0
};

static const uint8_t vcom_string2[] = {
  USB_DESC_BYTE(60),
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING),
  'C', 0, 'h', 0, 'i', 0, 'b', 0, 'i', 0, 'O', 0, 'S', 0, '/', 0,
  'X', 0, 'H', 0, 'A', 0, 'L', 0, ' ', 0, 'V', 0, 'i', 0, 'r', 0,
  't', 0, 'u', 0, 'a', 0, 'l', 0, ' ', 0, 'C', 0, 'O', 0, 'M', 0,
  ' ', 0, 'P', 0, 'o', 0, 'r', 0, 't', 0
};

static const uint8_t vcom_string3[] = {
  USB_DESC_BYTE(8),
  USB_DESC_BYTE(USB_DESCRIPTOR_STRING),
  '0' + CH_KERNEL_MAJOR, 0,
  '0' + CH_KERNEL_MINOR, 0,
  '0' + CH_KERNEL_PATCH, 0
};

static const usb_descriptor_t vcom_strings[] = {
  {sizeof vcom_string0, vcom_string0},
  {sizeof vcom_string1, vcom_string1},
  {sizeof vcom_string2, vcom_string2},
  {sizeof vcom_string3, vcom_string3}
};

static const usb_descriptor_t *usbcdc_get_descriptor_impl(void *ip,
                                                          uint8_t dtype,
                                                          uint8_t dindex,
                                                          uint16_t lang) {
  (void)ip;
  (void)lang;

  switch (dtype) {
  case USB_DESCRIPTOR_DEVICE:
    return &vcom_device_descriptor;
  case USB_DESCRIPTOR_CONFIGURATION:
    return &vcom_configuration_descriptor;
  case USB_DESCRIPTOR_STRING:
    if (dindex < 4U) {
      return &vcom_strings[dindex];
    }
    break;
  default:
    break;
  }

  return NULL;
}

static const struct hal_usb_binder_vmt usbcdc_binder_vmt = {
  .dispose        = __usbbnd_dispose_impl,
  .bind           = __usbbnd_bind_impl,
  .unbind         = __usbbnd_unbind_impl,
  .get_descriptor = usbcdc_get_descriptor_impl,
  .reset          = __usbbnd_reset_impl,
  .configure      = __usbbnd_configure_impl,
  .unconfigure    = __usbbnd_unconfigure_impl,
  .suspend        = __usbbnd_suspend_impl,
  .wakeup         = __usbbnd_wakeup_impl,
  .sof            = __usbbnd_sof_impl,
  .in             = __usbbnd_in_impl,
  .out            = __usbbnd_out_impl,
  .setup          = __usbbnd_setup_impl
};

const SerialUSBConfig serusbcfg = {
  .usbp             = &PORTAB_USB1,
  .control_if       = 0U,
  .bulk_in          = USB1_DATA_REQUEST_EP,
  .bulk_out         = USB1_DATA_AVAILABLE_EP,
  .int_in           = USB1_INTERRUPT_REQUEST_EP,
  .bulk_in_maxsize  = 0x0040U,
  .bulk_out_maxsize = 0x0040U,
  .int_in_maxsize   = 0x0008U
};

void usbcdcObjectInit(void) {
  usbBinderObjectInit(&usbcdc_binder, &usbcdc_binder_vmt);
}
