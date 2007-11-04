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
  ULONG32       csize;
  ULONG32       rdblklen;
} MMCCSD;

extern EventSource MMCInsertEventSource, MMCRemoveEventSource;

void InitMMC(void);

BOOL mmcInit(void);
void mmcStartPolling(void);
void mmcStopPolling(void);
BOOL mmcCardInserted (void);
BYTE8 mmcSendCommand(BYTE8 cmd, ULONG32 arg);
BOOL mmcGetSize(MMCCSD *data);
BOOL mmcRead(BYTE8 *buf, ULONG32 blknum);
BOOL mmcReadMultiple(BYTE8 *buf, ULONG32 blknum, ULONG32 n);
BOOL mmcWrite(BYTE8 *buf, ULONG32 blknum);
BOOL mmcWriteMultiple(BYTE8 *buf, ULONG32 blknum, ULONG32 n);
void mmcSynch(void);

#endif /* _MMCSD_H_*/
