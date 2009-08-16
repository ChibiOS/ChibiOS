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

/*
 * Win32 ISO7816 port simulator (on socket).
 */

#include <chconf.h>

#include <windows.h>
#include <stdio.h>

#undef CDECL

#include <ch.h>

#define ISO7816PORT1 29021

typedef struct {

  BYTE8 io_b[16];
  SOCKET io_listen;
  SOCKET io_data;
} sim7816;

HalfDuplexDriver ISO1;
static sim7816 sim1;
static u_long nb = 1;

static void init(char *name, HalfDuplexDriver *sd, sim7816 *simp, unsigned short port) {

  struct sockaddr_in sad;
  struct protoent *prtp;

  chHDDInit(sd, simp->io_b, sizeof(simp->io_b), NULL, NULL);

  simp->io_listen = INVALID_SOCKET;
  simp->io_data = INVALID_SOCKET;

  if ((prtp = getprotobyname("tcp")) == NULL) {
    printf("%s: Error mapping protocol name to protocol number\n", name);
    goto abort;
  }

  simp->io_listen = socket(PF_INET, SOCK_STREAM, prtp->p_proto);
  if (simp->io_listen == INVALID_SOCKET) {
    printf("%s: Error creating simulator socket\n", name);
    goto abort;
  }

  if (ioctlsocket(simp->io_listen, FIONBIO, &nb) != 0) {
    printf("%s: Unable to setup non blocking mode on socket\n", name);
    goto abort;
  }

  memset(&sad, 0, sizeof(sad));
  sad.sin_family = AF_INET;
  sad.sin_addr.s_addr = INADDR_ANY;
  sad.sin_port = htons(port);
  if (bind(simp->io_listen, (struct sockaddr *)&sad, sizeof(sad))) {
    printf("%s: Error binding socket\n", name);
    goto abort;
  }

  if (listen(simp->io_listen, 1) != 0) {
    printf("%s: Error binding socket\n", name);
    goto abort;
  }
  printf("Half Duplex channel %s listening on port %d\n", name, port);
  return;

abort:
  if (simp->io_listen != INVALID_SOCKET)
    closesocket(simp->io_listen);
  WSACleanup();
  exit(1);
}

void InitSimIso1(void) {

  init("ISO1", &ISO1, &sim1, ISO7816PORT1);
}

static BOOL connint(char *name, HalfDuplexDriver *sd, sim7816 *simp) {

  if (simp->io_data == INVALID_SOCKET) {
    struct sockaddr addr;
    int addrlen = sizeof(addr);

    if ((simp->io_data = accept(simp->io_listen, &addr, &addrlen)) == INVALID_SOCKET)
      return FALSE;

    if (ioctlsocket(simp->io_data, FIONBIO, &nb) != 0) {
      printf("%s: Unable to setup non blocking mode on data socket\n", name);
      goto abort;
    }
    chHDDAddFlagsI(sd, SD_CONNECTED);
    return TRUE;
  }
  return FALSE;
abort:
  if (simp->io_listen != INVALID_SOCKET)
    closesocket(simp->io_listen);
  if (simp->io_data != INVALID_SOCKET)
    closesocket(simp->io_data);
  WSACleanup();
  exit(1);
}

BOOL Iso1ConnInterrupt(void) {

  return connint("ISO1", &ISO1, &sim1);
}

static BOOL inint(char *name, HalfDuplexDriver *sd, sim7816 *simp) {

  if (simp->io_data != INVALID_SOCKET) {
    int i;
    BYTE8 data[4];

    /*
     * Input.
     */
    int n = recv(simp->io_data, data, sizeof(data), 0);
    switch (n) {
    case 0:
      closesocket(simp->io_data);
      simp->io_data = INVALID_SOCKET;
      chHDDAddFlagsI(sd, SD_DISCONNECTED);
      return FALSE;
    case SOCKET_ERROR:
      if (WSAGetLastError() == WSAEWOULDBLOCK)
        return FALSE;
      closesocket(simp->io_data);
      simp->io_data = INVALID_SOCKET;
      return FALSE;
    }
    for (i = 0; i < n; i++)
      chHDDIncomingDataI(sd, data[i]);
    return TRUE;
  }
  return FALSE;
}

BOOL Iso1InInterrupt(void) {

  return inint("ISO", &ISO1, &sim1);
}

static BOOL outint(char *name, HalfDuplexDriver *sd, sim7816 *simp) {

  if (simp->io_data != INVALID_SOCKET) {
    short n;
    BYTE8 data[1];

    /*
     * Input.
     */
    n = chHDDRequestDataI(sd);
    if (n < 0)
      return FALSE;
    data[0] = (BYTE8)n;
    n = send(simp->io_data, data, sizeof(data), 0);
    switch (n) {
    case 0:
      closesocket(simp->io_data);
      simp->io_data = INVALID_SOCKET;
      chHDDAddFlagsI(sd, SD_DISCONNECTED);
      return FALSE;
    case SOCKET_ERROR:
      if (WSAGetLastError() == WSAEWOULDBLOCK)
        return FALSE;
      closesocket(simp->io_data);
      simp->io_data = INVALID_SOCKET;
      return FALSE;
    }
    return TRUE;
  }
  return FALSE;
}

BOOL Iso1OutInterrupt(void) {

  return outint("ISO1", &ISO1, &sim1);
}
