/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

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

/**
 * @file    RX62N/pal_lld.h
 * @brief   RX62N GPIO low level driver header.
 *
 * @addtogroup PAL
 * @{
 */

#ifndef _PAL_LLD_H_
#define _PAL_LLD_H_

#if HAL_USE_PAL || defined(__DOXYGEN__)

/*===========================================================================*/
/* Unsupported modes and specific modes                                      */
/*===========================================================================*/

#undef PAL_MODE_INPUT_ANALOG
#undef PAL_MODE_INPUT_PULLDOWN

/*===========================================================================*/
/* I/O Ports Types and constants.                                            */
/*===========================================================================*/

/**
 * @brief   RX62N GPIO registers block.
 */
typedef struct {
  volatile uint8_t    DDR;
  uint8_t             wk0[31];
  union {
    volatile uint8_t DR;
      struct {
        volatile unsigned char B0:1;
        volatile unsigned char B1:1;
        volatile unsigned char B2:1;
        volatile unsigned char B3:1;
        volatile unsigned char B4:1;
        volatile unsigned char B5:1;
        volatile unsigned char B6:1;
        volatile unsigned char B7:1;
    } BIT;
  };
  uint8_t             wk1[31];
  volatile uint8_t    PORT;
  uint8_t             wk2[31];
  volatile uint8_t    ICR;
  uint8_t             wk3[31];
  volatile uint8_t    ODR;
  uint8_t             wk4[63];
  volatile uint8_t    PCR;
} GPIO_TypeDef;

/**
 * @brief   GPIO port setup info.
 */
typedef struct {
  /** Initial value for DDR register.*/
  uint8_t               ddr;
  /** Initial value for DR register.*/
  uint8_t               dr;
  /** Initial value for ICR register.*/
  uint8_t               icr;
  /** Initial value for ODR register.*/
  uint8_t               odr;
  /** Initial value for PCR register.*/
  uint8_t               pcr;
} rx62n_gpio_setup_t;

/**
 * @brief   RX62N GPIO static initializer.
 * @details An instance of this structure must be passed to @p palInit() at
 *          system startup time in order to initialize the digital I/O
 *          subsystem. This represents only the initial setup, specific pads
 *          or whole ports can be reprogrammed at later time.
 */
typedef struct {
  /** @brief Port 0 setup data.*/
  rx62n_gpio_setup_t    P0Data;
  /** @brief Port 1 setup data.*/
  rx62n_gpio_setup_t    P1Data;
  /** @brief Port 2 setup data.*/
  rx62n_gpio_setup_t    P2Data;
  /** @brief Port 3 setup data.*/
  rx62n_gpio_setup_t    P3Data;
  /** @brief Port 4 setup data.*/
  rx62n_gpio_setup_t    P4Data;
  /** @brief Port 5 setup data.*/
  rx62n_gpio_setup_t    P5Data;
#if RX62N_HAS_PORT6
  /** @brief Port 6 setup data.*/
  rx62n_gpio_setup_t    P6Data;
#endif
#if RX62N_HAS_PORT7
  /** @brief Port 7 setup data.*/
  rx62n_gpio_setup_t    P7Data;
#endif
#if RX62N_HAS_PORT8
  /** @brief Port 8 setup data.*/
  rx62n_gpio_setup_t    P8Data;
#endif
#if RX62N_HAS_PORT9
  /** @brief Port 9 setup data.*/
  rx62n_gpio_setup_t    P9Data;
#endif
  /** @brief Port A setup data.*/
  rx62n_gpio_setup_t    PAData;
  /** @brief Port B setup data.*/
  rx62n_gpio_setup_t    PBData;
  /** @brief Port C setup data.*/
  rx62n_gpio_setup_t    PCData;
  /** @brief Port D setup data.*/
  rx62n_gpio_setup_t    PDData;
#if RX62N_HAS_PORTE
  /** @brief Port E setup data.*/
  rx62n_gpio_setup_t    PEData;
#endif
#if RX62N_HAS_PORTx
  /** @brief Port x setup data.*/
/*  rx62n_gpio_setup_t    PxData;*/
#endif
} PALConfig;

/**
 * @brief   Width, in bits, of an I/O port.
 */
#define PAL_IOPORTS_WIDTH 8

/**
 * @brief   Whole port mask.
 * @details This macro specifies all the valid bits into a port.
 */
#define PAL_WHOLE_PORT ( (ioportmask_t)( ( 1 << PAL_IOPORTS_WIDTH ) - 1 ) )

/**
 * @brief   Digital I/O port sized unsigned type.
 */
typedef uint8_t ioportmask_t;

/**
 * @brief   Digital I/O modes.
 */
typedef uint8_t iomode_t;

/**
 * @brief   Port Identifier.
 * @details This type can be a scalar or some kind of pointer, do not make
 *          any assumption about it, use the provided macros when populating
 *          variables of this type.
 */
typedef GPIO_TypeDef * ioportid_t;

/*===========================================================================*/
/* I/O Ports Identifiers.                                                    */
/* The low level driver wraps the definitions already present in the RX62N   */
/* iodefine_gcc62n.h.                                                        */
/*===========================================================================*/

/**
 * @brief   GPIO port 0 identifier.
 */
#if RX62N_HAS_PORT0 || defined(__DOXYGEN__)
#define GPIO0           ((GPIO_TypeDef *)(&PORT0))
#define IOPORT1         ((GPIO_TypeDef *)(&PORT0))
#endif

/**
 * @brief   GPIO port 1 identifier.
 */
#if RX62N_HAS_PORT1 || defined(__DOXYGEN__)
#define GPIO1           ((GPIO_TypeDef *)(&PORT1))
#define IOPORT2         ((GPIO_TypeDef *)(&PORT1))
#endif

/**
 * @brief   GPIO port 2 identifier.
 */
#if RX62N_HAS_PORT2 || defined(__DOXYGEN__)
#define GPIO2           ((GPIO_TypeDef *)(&PORT2))
#define IOPORT3         ((GPIO_TypeDef *)(&PORT2))
#endif

/**
 * @brief   GPIO port 3 identifier.
 */
#if RX62N_HAS_PORT3 || defined(__DOXYGEN__)
#define GPIO3           ((GPIO_TypeDef *)(&PORT3))
#define IOPORT4         ((GPIO_TypeDef *)(&PORT3))
#endif

/**
 * @brief   GPIO port 4 identifier.
 */
#if RX62N_HAS_PORT4 || defined(__DOXYGEN__)
#define GPIO4           ((GPIO_TypeDef *)(&PORT4))
#define IOPORT5         ((GPIO_TypeDef *)(&PORT4))
#endif

/**
 * @brief   GPIO port 5 identifier.
 */
#if RX62N_HAS_PORT5 || defined(__DOXYGEN__)
#define GPIO5           ((GPIO_TypeDef *)(&PORT5))
#define IOPORT6         ((GPIO_TypeDef *)(&PORT5))
#endif

/**
 * @brief   GPIO port 6 identifier.
 */
#if RX62N_HAS_PORT6 || defined(__DOXYGEN__)
#define GPIO6           ((GPIO_TypeDef *)(&PORT6))
#define IOPORT7         ((GPIO_TypeDef *)(&PORT6))
#endif

/**
 * @brief   GPIO port 7 identifier.
 */
#if RX62N_HAS_PORT7 || defined(__DOXYGEN__)
#define GPIO7           ((GPIO_TypeDef *)(&PORT7))
#define IOPORT8         ((GPIO_TypeDef *)(&PORT7))
#endif

/**
 * @brief   GPIO port 8 identifier.
 */
#if RX62N_HAS_PORT8 || defined(__DOXYGEN__)
#define GPIO8           ((GPIO_TypeDef *)(&PORT8))
#define IOPORT9         ((GPIO_TypeDef *)(&PORT8))
#endif

/**
 * @brief   GPIO port 9 identifier.
 */
#if RX62N_HAS_PORT9 || defined(__DOXYGEN__)
#define GPIO9            ((GPIO_TypeDef *)(&PORT9))
#define IOPORT10         ((GPIO_TypeDef *)(&PORT9))
#endif

/**
 * @brief   GPIO port 10 identifier.
 */
#if RX62N_HAS_PORTA || defined(__DOXYGEN__)
#define GPIO10           ((GPIO_TypeDef *)(&PORTA))
#define IOPORT11         ((GPIO_TypeDef *)(&PORTA))
#endif

/**
 * @brief   GPIO port 11 identifier.
 */
#if RX62N_HAS_PORTB || defined(__DOXYGEN__)
#define GPIO11           ((GPIO_TypeDef *)(&PORTB))
#define IOPORT12         ((GPIO_TypeDef *)(&PORTB))
#endif

/**
 * @brief   GPIO port 12 identifier.
 */
#if RX62N_HAS_PORTC || defined(__DOXYGEN__)
#define GPIO12           ((GPIO_TypeDef *)(&PORTC))
#define IOPORT13         ((GPIO_TypeDef *)(&PORTC))
#endif

/**
 * @brief   GPIO port 13 identifier.
 */
#if RX62N_HAS_PORTD || defined(__DOXYGEN__)
#define GPIO13           ((GPIO_TypeDef *)(&PORTD))
#define IOPORT14         ((GPIO_TypeDef *)(&PORTD))
#endif

/**
 * @brief   GPIO port 14 identifier.
 */
#if RX62N_HAS_PORTE || defined(__DOXYGEN__)
#define GPIO14           ((GPIO_TypeDef *)(&PORTE))
#define IOPORT15         ((GPIO_TypeDef *)(&PORTE))
#endif

/**
 * @brief   GPIO port 15 identifier.
 */
#if RX62N_HAS_PORTF || defined(__DOXYGEN__)
#define GPIO15           ((GPIO_TypeDef *)(&PORTF))
#define IOPORT16         ((GPIO_TypeDef *)(&PORTF))
#endif

/**
 * @brief   GPIO port 16 identifier.
 */
#if RX62N_HAS_PORTG || defined(__DOXYGEN__)
#define GPIO16           ((GPIO_TypeDef *)(&PORTG))
#define IOPORT17         ((GPIO_TypeDef *)(&PORTG))
#endif

/*===========================================================================*/
/* Implementation, some of the following macros could be implemented as      */
/* functions, if so please put them in pal_lld.c.                            */
/*===========================================================================*/

/**
 * @brief   GPIO ports subsystem initialization.
 *
 * @notapi
 */
#define pal_lld_init(config) _pal_lld_init(config)

/**
 * @brief   Reads an I/O port.
 * @details This function is implemented by reading the GPIO IDR register, the
 *          implementation has no side effects.
 * @note    This function is not meant to be invoked directly by the application
 *          code.
 *
 * @param[in] port      port identifier
 * @return              The port bits.
 *
 * @notapi
 */
#define pal_lld_readport(port) ((port)->PORT)

/**
 * @brief   Reads the output latch.
 * @details This function is implemented by reading the GPIO ODR register, the
 *          implementation has no side effects.
 * @note    This function is not meant to be invoked directly by the application
 *          code.
 *
 * @param[in] port      port identifier
 * @return              The latched logical states.
 *
 * @notapi
 */
#define pal_lld_readlatch(port) ((port)->DR)

/**
 * @brief   Writes on a I/O port.
 * @details This function is implemented by writing the GPIO ODR register, the
 *          implementation has no side effects.
 *
 * @param[in] port      port identifier
 * @param[in] bits      bits to be written on the specified port
 *
 * @notapi
 */
#define pal_lld_writeport(port, bits) ((port)->DR = (bits))

/**
 * @brief   Sets a bits mask on a I/O port.
 * @details This function is implemented by writing the GPIO BSRR register, the
 *          implementation has no side effects.
 *
 * @param[in] port      port identifier
 * @param[in] bits      bits to be ORed on the specified port
 *
 * @notapi
 */
/*
#define pal_lld_setport(port, bits)
*/

/**
 * @brief   Clears a bits mask on a I/O port.
 * @details This function is implemented by writing the GPIO BSRR register, the
 *          implementation has no side effects.
 *
 * @param[in] port      port identifier
 * @param[in] bits      bits to be cleared on the specified port
 *
 * @notapi
 */
/*
#define pal_lld_clearport(port, bits)
*/

/**
 * @brief   Writes a group of bits.
 * @details This function is implemented by writing the GPIO BSRR register, the
 *          implementation has no side effects.
 *
 * @param[in] port      port identifier
 * @param[in] mask      group mask
 * @param[in] offset    the group bit offset within the port
 * @param[in] bits      bits to be written. Values exceeding the group
 *                      width are masked.
 *
 * @notapi
 */
/*
#define pal_lld_writegroup(port, mask, offset, bits)
*/

/**
 * @brief   Pads group mode setup.
 * @details This function programs a pads group belonging to the same port
 *          with the specified mode.
 *
 * @param[in] port      port identifier
 * @param[in] mask      group mask
 * @param[in] offset    group bit offset within the port
 * @param[in] mode      group mode
 *
 * @notapi
 */
#define pal_lld_setgroupmode(port, mask, offset, mode)                      \
  _pal_lld_setgroupmode(port, mask << offset, mode)

/**
 * @brief   Writes a logical state on an output pad.
 *
 * @param[in] port      port identifier
 * @param[in] pad       pad number within the port
 * @param[in] bit       logical value, the value must be @p PAL_LOW or
 *                      @p PAL_HIGH
 *
 * @notapi
 */
#define pal_lld_writepad(port, pad, bit) pal_lld_writegroup(port, 1, pad, bit)

extern const PALConfig pal_default_config;

#ifdef __cplusplus
extern "C" {
#endif
  void _pal_lld_init(const PALConfig *config);
  void _pal_lld_setgroupmode(ioportid_t port,
                             ioportmask_t mask,
                             iomode_t mode);
#ifdef __cplusplus
}
#endif

#endif /* HAL_USE_PAL */

#endif /* _PAL_LLD_H_ */

/** @} */
