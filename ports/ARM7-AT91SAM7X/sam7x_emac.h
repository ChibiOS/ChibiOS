/*
    ChibiOS/RT - Copyright (C) 2009 Giovanni Di Sirio.

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

                                      ---

    A special exception to the GPL can be applied should you wish to distribute
    a combined work that includes ChibiOS/RT, without being obliged to provide
    the source code for any proprietary components. See the file exception.txt
    for full details of how and when the exception can be applied.
*/

#ifndef _SAM7X_EMAC_H_
#define _SAM7X_EMAC_H_

typedef struct {
  uint32_t      w1;
  uint32_t      w2;
} BufDescriptorEntry;

#define W1_R_OWNERSHIP          0x00000001
#define W1_R_WRAP               0x00000002
#define W1_R_ADDRESS_MASK       0xFFFFFFFC

#define W2_R_LENGTH_MASK        0x00000FFF
#define W2_R_FRAME_START        0x00004000
#define W2_R_FRAME_END          0x00008000
#define W2_R_CFI                0x00010000
#define W2_R_VLAN_PRIO_MASK     0x000E0000
#define W2_R_PRIO_TAG_DETECTED  0x00100000
#define W2_R_VLAN_TAG_DETECTED  0x00200000
#define W2_R_TYPE_ID_MATCH      0x00400000
#define W2_R_ADDR4_MATCH        0x00800000
#define W2_R_ADDR3_MATCH        0x01000000
#define W2_R_ADDR2_MATCH        0x02000000
#define W2_R_ADDR1_MATCH        0x04000000
#define W2_R_RFU1               0x08000000
#define W2_R_ADDR_EXT_MATCH     0x10000000
#define W2_R_UNICAST_MATCH      0x20000000
#define W2_R_MULTICAST_MATCH    0x40000000
#define W2_R_BROADCAST_DETECTED 0x80000000

#define W2_T_LENGTH_MASK        0x000007FF
#define W2_T_LOCKED             0x00000800 /* Not an EMAC flag, used by the driver */
#define W2_T_RFU1               0x00003000
#define W2_T_LAST_BUFFER        0x00008000
#define W2_T_NO_CRC             0x00010000
#define W2_T_RFU2               0x07FE0000
#define W2_T_BUFFERS_EXHAUSTED  0x08000000
#define W2_T_TRANSMIT_UNDERRUN  0x10000000
#define W2_T_RETRY_LIMIT_EXC    0x20000000
#define W2_T_WRAP               0x40000000
#define W2_T_USED               0x80000000

#ifdef __cplusplus
extern "C" {
#endif
  void InitEMAC(int prio);
  void EMACSetAddress(const uint8_t *eaddr);
  bool_t EMACGetLinkStatus(void);
  BufDescriptorEntry *EMACGetTransmitBuffer(void);
  void EMACTransmit(BufDescriptorEntry *cptr, size_t size);
  bool_t EMACReceive(uint8_t *buf, size_t *sizep);
#ifdef __cplusplus
}
#endif

extern EventSource EMACFrameTransmitted, EMACFrameReceived;

#endif /* _SAM7X_EMAC_H_ */
