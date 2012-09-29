/*
    ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
                 2011 Giovanni Di Sirio.

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

/*------------------------------------------------------------------------*/
/* Sample code of OS dependent controls for FatFs R0.08b                  */
/* (C)ChaN, 2011                                                          */
/*------------------------------------------------------------------------*/

#include "ch.h"
#include "ff.h"

#if _FS_REENTRANT
/*------------------------------------------------------------------------*/
/* Static array of Synchronization Objects                                */
/*------------------------------------------------------------------------*/
static Semaphore ff_sem[_VOLUMES];

/*------------------------------------------------------------------------*/
/* Create a Synchronization Object                                        */
/*------------------------------------------------------------------------*/
int ff_cre_syncobj(BYTE vol, _SYNC_t *sobj) {

  *sobj = &ff_sem[vol];
  chSemInit(*sobj, 1);
  return TRUE;
}

/*------------------------------------------------------------------------*/
/* Delete a Synchronization Object                                        */
/*------------------------------------------------------------------------*/
int ff_del_syncobj(_SYNC_t sobj) {

  chSemReset(sobj, 0);
  return TRUE;
}

/*------------------------------------------------------------------------*/
/* Request Grant to Access the Volume                                     */
/*------------------------------------------------------------------------*/
int ff_req_grant(_SYNC_t sobj) {

  msg_t msg = chSemWaitTimeout(sobj, (systime_t)_FS_TIMEOUT);
  return msg == RDY_OK;
}

/*------------------------------------------------------------------------*/
/* Release Grant to Access the Volume                                     */
/*------------------------------------------------------------------------*/
void ff_rel_grant(_SYNC_t sobj) {

  chSemSignal(sobj);
}
#endif /* _FS_REENTRANT */

#if _USE_LFN == 3	/* LFN with a working buffer on the heap */
/*------------------------------------------------------------------------*/
/* Allocate a memory block                                                */
/*------------------------------------------------------------------------*/
void *ff_memalloc(UINT size) {

  return chHeapAlloc(NULL, size);
}

/*------------------------------------------------------------------------*/
/* Free a memory block                                                    */
/*------------------------------------------------------------------------*/
void ff_memfree(void *mblock) {

  chHeapFree(mblock);
}
#endif /* _USE_LFN == 3 */
