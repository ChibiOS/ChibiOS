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

#ifndef _MMCSD_H_
#define _MMCSD_H_

#define NICE_WAITING

#define CMD0_RETRY              10
#define CMD1_RETRY              100
#define POLLING_INTERVAL        10
#define MMC_WAIT_DATA           10000

#define CMDGOIDLE               0
#define CMDINIT                 1
#define CMDREADCSD              9
#define CMDSTOP                 12
#define CMDREAD                 17
#define CMDREADMULTIPLE         18
#define CMDWRITE                24
#define CMDWRITEMULTIPLE        25

typedef struct {
  uint32_t      csize;
  uint32_t      rdblklen;
} MMCCSD;

extern EventSource MMCInsertEventSource, MMCRemoveEventSource;

#ifdef __cplusplus
}
#endif
  void InitMMC(void);

  bool_t mmcInit(void);
  void mmcStartPolling(void);
  void mmcStopPolling(void);
  bool_t mmcCardInserted (void);
  uint8_t mmcSendCommand(uint8_t cmd, uint32_t arg);
  bool_t mmcGetSize(MMCCSD *data);
  bool_t mmcRead(uint8_t *buf, uint32_t blknum);
  bool_t mmcReadMultiple(uint8_t *buf, uint32_t blknum, uint32_t n);
  bool_t mmcWrite(uint8_t *buf, uint32_t blknum);
  bool_t mmcWriteMultiple(uint8_t *buf, uint32_t blknum, uint32_t n);
  void mmcSynch(void);
#ifdef __cplusplus
}
#endif

#endif /* _MMCSD_H_*/
