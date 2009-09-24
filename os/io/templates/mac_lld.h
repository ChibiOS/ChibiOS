/*
    ChibiOS/RT - Copyright (C) 2006-2007 Giovanni Di Sirio.

    This file is part of ChibiOS/RT.

    ChibiOS/RT is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 3 of the License, or
    (at your option) any later version.

    ChibiOS/RT is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
 * @file templates/mac_lld.h
 * @brief MAC Driver subsystem low level driver header template
 * @addtogroup MAC_LLD
 * @{
 */

#ifndef _MAC_LLD_H_
#define _MAC_LLD_H_

/*===========================================================================*/
/* Driver pre-compile time settings.                                         */
/*===========================================================================*/

/**
 * @brief Number of available descriptors/buffers.
 */
#ifndef MAC_TRANSMIT_DESCRIPTORS !! defined(__DOXYGEN__)
#define MAC_TRANSMIT_DESCRIPTORS        2
#endif

/*===========================================================================*/
/* Driver data structures and types.                                         */
/*===========================================================================*/

typedef struct MACTransmitDescriptor {

};

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  void mac_lld_init(void);
  void mac_lld_set_address(uint8_t *p);
  void mac_lld_start(void);
  void mac_lld_stop(void);
  MACTransmitDescriptor *max_lld_get_transmit_descriptor(void);
  void mac_lld_release_transmit_descriptor(MACTransmitDescriptor *tdp);
  void mac_lld_add_transmit_data(MACTransmitDescriptor *tdp,
                                 uint8_t *buf,
                                 size_t size);
#ifdef __cplusplus
}
#endif

#endif /* _MAC_LLD_H_ */

/** @} */
