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

/*
 * Win32 COM port simulator (on socket).
 */

#include <chconf.h>

#include <windows.h>
#include <stdio.h>

#undef CDECL

#include <ch.h>

#define COM1PORT  29001
#define COM2PORT  29002

struct simcom {

  uint8_t com_ib[1024];
  uint8_t com_ob[1024];
  SOCKET com_listen;
  SOCKET com_data;
};

FullDuplexDriver COM1;
static struct simcom sc1;
FullDuplexDriver COM2;
static struct simcom sc2;
static u_long nb = 1;

static void init(char *name, FullDuplexDriver *sd,
                 struct simcom *sc, unsigned short port) {
  struct sockaddr_in sad;
  struct protoent *prtp;

  chFDDInit(sd,
            sc->com_ib, sizeof(sc->com_ib), NULL,
            sc->com_ob, sizeof(sc->com_ob), NULL);

  sc->com_listen = INVALID_SOCKET;
  sc->com_data = INVALID_SOCKET;

  if ((prtp = getprotobyname("tcp")) == NULL) {
    printf("%s: Error mapping protocol name to protocol number\n", name);
    goto abort;
  }

  sc->com_listen = socket(PF_INET, SOCK_STREAM, prtp->p_proto);
  if (sc->com_listen == INVALID_SOCKET) {
    printf("%s: Error creating simulator socket\n", name);
    goto abort;
  }

  if (ioctlsocket(sc->com_listen, FIONBIO, &nb) != 0) {
    printf("%s: Unable to setup non blocking mode on socket\n", name);
    goto abort;
  }

  memset(&sad, 0, sizeof(sad));
  sad.sin_family = AF_INET;
  sad.sin_addr.s_addr = INADDR_ANY;
  sad.sin_port = htons(port);
  if (bind(sc->com_listen, (struct sockaddr *)&sad, sizeof(sad))) {
    printf("%s: Error binding socket\n", name);
    goto abort;
  }

  if (listen(sc->com_listen, 1) != 0) {
    printf("%s: Error binding socket\n", name);
    goto abort;
  }
  printf("Full Duplex Channel %s listening on port %d\n", name, port);
  return;

abort:
  if (sc->com_listen != INVALID_SOCKET)
    closesocket(sc->com_listen);
  WSACleanup();
  exit(1);
}

void InitSimCom1(void) {

  init("COM1", &COM1, &sc1, COM1PORT);
}

void InitSimCom2(void) {

  init("COM2", &COM2, &sc2, COM2PORT);
}

static bool_t connint(char *name, FullDuplexDriver *sd, struct simcom *sc) {

  if (sc->com_data == INVALID_SOCKET) {
    struct sockaddr addr;
    int addrlen = sizeof(addr);

    if ((sc->com_data = accept(sc->com_listen, &addr, &addrlen)) == INVALID_SOCKET)
      return FALSE;

    if (ioctlsocket(sc->com_data, FIONBIO, &nb) != 0) {
      printf("%s: Unable to setup non blocking mode on data socket\n", name);
      goto abort;
    }
    chFDDAddFlagsI(sd, SD_CONNECTED);
    return TRUE;
  }
  return FALSE;
abort:
  if (sc->com_listen != INVALID_SOCKET)
    closesocket(sc->com_listen);
  if (sc->com_data != INVALID_SOCKET)
    closesocket(sc->com_data);
  WSACleanup();
  exit(1);
}

bool_t Com1ConnInterruptSimCom(void) {

  return connint("COM1", &COM1, &sc1);
}

bool_t Com2ConnInterruptSimCom(void) {

  return connint("COM2", &COM2, &sc2);
}

static bool_t inint(char *name, FullDuplexDriver *sd, struct simcom *sc) {

  if (sc->com_data != INVALID_SOCKET) {
    int i;
    uint8_t data[32];

    /*
     * Input.
     */
    int n = recv(sc->com_data, data, sizeof(data), 0);
    switch (n) {
    case 0:
      closesocket(sc->com_data);
      sc->com_data = INVALID_SOCKET;
      chFDDAddFlagsI(sd, SD_DISCONNECTED);
      return FALSE;
    case SOCKET_ERROR:
      if (WSAGetLastError() == WSAEWOULDBLOCK)
        return FALSE;
      closesocket(sc->com_data);
      sc->com_data = INVALID_SOCKET;
      return FALSE;
    }
    for (i = 0; i < n; i++)
      chFDDIncomingDataI(sd, data[i]);
    return TRUE;
  }
  return FALSE;
}

bool_t Com1InInterruptSimCom(void) {

  return inint("COM1", &COM1, &sc1);
}

bool_t Com2InInterruptSimCom(void) {

  return inint("COM2", &COM2, &sc2);
}

static bool_t outint(char *name, FullDuplexDriver *sd, struct simcom *sc) {

  if (sc->com_data != INVALID_SOCKET) {
    int n;
    uint8_t data[1];

    /*
     * Input.
     */
    n = chFDDRequestDataI(sd);
    if (n < 0)
      return FALSE;
    data[0] = (uint8_t)n;
    n = send(sc->com_data, data, sizeof(data), 0);
    switch (n) {
    case 0:
      closesocket(sc->com_data);
      sc->com_data = INVALID_SOCKET;
      chFDDAddFlagsI(sd, SD_DISCONNECTED);
      return FALSE;
    case SOCKET_ERROR:
      if (WSAGetLastError() == WSAEWOULDBLOCK)
        return FALSE;
      closesocket(sc->com_data);
      sc->com_data = INVALID_SOCKET;
      return FALSE;
    }
    return TRUE;
  }
  return FALSE;
}

bool_t Com1OutInterruptSimCom(void) {

  return outint("COM1", &COM1, &sc1);
}

bool_t Com2OutInterruptSimCom(void) {

  return outint("COM2", &COM2, &sc2);
}
