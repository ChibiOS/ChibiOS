/*
    ChibiOS - Copyright (C) 2006..2018 Giovanni Di Sirio

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
 * @file    tssockstub.c
 * @brief   Sockets stub for trusted services.
 *
 */

#include "ch.h"
#include "chobjfifos.h"
#include "chtssi.h"
#include "tsproxystubs.h"
#include "tssockstub.h"
#include <string.h>
#include <ctype.h>

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/
static stub_ctx_t stub_ctx;

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief     The sockets stub service.
 */
THD_WORKING_AREA(waTsSocksStubsService, 1024);
THD_FUNCTION(TsSocksStubsService, tsstatep) {
  stub_ctx.event_flag = EVT_F_SOCK_NEW_OP;
  TsStubService((ts_state_t *)tsstatep, &stub_ctx);
}

/**
 * @brief The sockets API.
 */
int socket(int domain, int type, int protocol) {
  stub_op_t *op = getNewOp(&stub_ctx);
  op->op_code = SOCK_OP_SOCKET;
  op->op_p[0].dir = OP_PRMDIR_NONE;
  op->op_p[0].val = (uint32_t)domain;
  op->op_p[1].dir = OP_PRMDIR_NONE;
  op->op_p[1].val = (uint32_t)type;
  op->op_p[2].dir = OP_PRMDIR_NONE;
  op->op_p[2].val = (uint32_t)protocol;
  return (int)callRemote(op);
}

int connect(int s, const struct sockaddr *name, socklen_t namelen) {
  stub_op_t *op = getNewOp(&stub_ctx);
  op->op_code = SOCK_OP_CONNECT;
  op->op_p[0].dir = OP_PRMDIR_NONE;
  op->op_p[0].val = (uint32_t)s;
  op->op_p[1].dir = OP_PRMDIR_IN;
  op->op_p[1].val = (uint32_t)name;
  op->op_p[1].size = (uint32_t)namelen;
  op->op_p[2].dir = OP_PRMDIR_NONE;
  op->op_p[2].val = (uint32_t)namelen;
  return (int)callRemote(op);
}

int close(int s) {
  stub_op_t *op = getNewOp(&stub_ctx);
  op->op_code = SOCK_OP_CLOSE;
  op->op_p[0].dir = OP_PRMDIR_NONE;
  op->op_p[0].val = (uint32_t)s;
  return (int)callRemote(op);
}

int recv(int s, void *mem, size_t len, int flags) {
  stub_op_t *op = getNewOp(&stub_ctx);
  op->op_code = SOCK_OP_RECV;
  op->op_p[0].dir = OP_PRMDIR_NONE;
  op->op_p[0].val = (uint32_t)s;
  op->op_p[1].dir = OP_PRMDIR_OUT;
  op->op_p[1].val = (uint32_t)mem;
  op->op_p[1].size = (uint32_t)len;
  op->op_p[2].dir = OP_PRMDIR_NONE;
  op->op_p[2].val = (uint32_t)len;
  op->op_p[3].dir = OP_PRMDIR_NONE;
  op->op_p[3].val = (uint32_t)flags;
  return (int)callRemote(op);
}

int send(int s, const void *dataptr, size_t size, int flags) {
  stub_op_t *op = getNewOp(&stub_ctx);
  op->op_code = SOCK_OP_SEND;
  op->op_p[0].dir = OP_PRMDIR_NONE;
  op->op_p[0].val = (uint32_t)s;
  op->op_p[1].dir = OP_PRMDIR_IN;
  op->op_p[1].val = (uint32_t)dataptr;
  op->op_p[1].size = (uint32_t)size;
  op->op_p[2].dir = OP_PRMDIR_NONE;
  op->op_p[2].val = (uint32_t)size;
  op->op_p[3].dir = OP_PRMDIR_NONE;
  op->op_p[3].val = (uint32_t)flags;
  return (int)callRemote(op);
}

int select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset,
              struct timeval *timeout) {
  stub_op_t *op = getNewOp(&stub_ctx);
  op->op_code = SOCK_OP_SELECT;
  op->op_p[0].dir = OP_PRMDIR_NONE;
  op->op_p[0].val = (uint32_t)maxfdp1;
  op->op_p[1].dir = OP_PRMDIR_IN|OP_PRMDIR_OUT;
  op->op_p[1].val = (uint32_t)readset;
  op->op_p[1].size = sizeof (fd_set);
  op->op_p[2].dir = OP_PRMDIR_IN|OP_PRMDIR_OUT;
  op->op_p[2].val = (uint32_t)writeset;
  op->op_p[2].size = sizeof (fd_set);
  op->op_p[3].dir = OP_PRMDIR_IN|OP_PRMDIR_OUT;
  op->op_p[3].val = (uint32_t)exceptset;
  op->op_p[3].size = sizeof (fd_set);
  op->op_p[4].dir = OP_PRMDIR_IN;
  op->op_p[4].val = (uint32_t)timeout;
  op->op_p[4].size = sizeof (struct timeval);
  return (int)callRemote(op);
}

int bind(int s, const struct sockaddr *name, socklen_t namelen) {
  stub_op_t *op = getNewOp(&stub_ctx);
  op->op_code = SOCK_OP_BIND;
  op->op_p[0].dir = OP_PRMDIR_NONE;
  op->op_p[0].val = (uint32_t)s;
  op->op_p[1].dir = OP_PRMDIR_IN;
  op->op_p[1].val = (uint32_t)name;
  op->op_p[1].size = (uint32_t)namelen;
  op->op_p[2].dir = OP_PRMDIR_NONE;
  op->op_p[2].val = (uint32_t)namelen;
  return (int)callRemote(op);
}

int listen(int s, int backlog) {
  stub_op_t *op = getNewOp(&stub_ctx);
  op->op_code = SOCK_OP_LISTEN;
  op->op_p[0].dir = OP_PRMDIR_NONE;
  op->op_p[0].val = (uint32_t)s;
  op->op_p[1].dir = OP_PRMDIR_NONE;
  op->op_p[1].val = (uint32_t)backlog;
  return (int)callRemote(op);
}

#if 0
int accept(int s, struct sockaddr *addr, socklen_t *addrlen);
int shutdown(int s, int how);
int getpeername (int s, struct sockaddr *name, socklen_t *namelen);
int getsockname (int s, struct sockaddr *name, socklen_t *namelen);
int getsockopt (int s, int level, int optname, void *optval, socklen_t *optlen);
int setsockopt (int s, int level, int optname, const void *optval, socklen_t optlen);
int read(int s, void *mem, size_t len);
int recvfrom(int s, void *mem, size_t len, int flags,
      struct sockaddr *from, socklen_t *fromlen);
int sendmsg(int s, const struct msghdr *message, int flags);
int sendto(int s, const void *dataptr, size_t size, int flags,
    const struct sockaddr *to, socklen_t tolen);
int write(int s, const void *dataptr, size_t size);
int writev(int s, const struct iovec *iov, int iovcnt);
int ioctl(int s, long cmd, void *argp);
int fcntl(int s, int cmd, int val);
#endif

/*
 * Ascii internet address interpretation routine.
 * The value returned is in network order.
 */
in_addr_t inet_addr(const char *cp) {
  struct in_addr val;

  if (inet_aton(cp, &val))
    return val.s_addr;
  return INADDR_NONE;
}

/*
 * Check whether "cp" is a valid ascii representation
 * of an Internet address and convert to a binary address.
 * Returns 1 if the address is valid, 0 if not.
 * This replaces inet_addr, the return value from which
 * cannot distinguish between failure and a local broadcast address.
 */

int inet_aton(const char *cp, struct in_addr *addr) {
  uint32_t val, base, n;
  char c;
  uint32_t parts[4], *pp = parts;

  for (;;) {
    /*
     * Collect number up to '.'.
     * Values are specified as for C:
     * 0x=hex, 0=octal, other=decimal.
     */
    val = 0; base = 10;
    if (*cp == '0') {
      if (*++cp == 'x' || *cp == 'X')
        base = 16, cp++;
      else
        base = 8;
    }
    while ((c = *cp) != '\0') {
      if (isascii(c) && isdigit(c)) {
        val = (val * base) + (c - '0');
        cp++;
        continue;
      }
      if (base == 16 && isascii(c) && isxdigit(c)) {
        val = (val << 4) +
          (c + 10 - (islower(c) ? 'a' : 'A'));
        cp++;
        continue;
      }
      break;
    }
    if (*cp == '.') {
      /*
       * Internet format:
       *  a.b.c.d
       *  a.b.c (with c treated as 16-bits)
       *  a.b (with b treated as 24 bits)
       */
      if (pp >= parts + 3 || val > 0xff)
        return 0;
      *pp++ = val, cp++;
    } else
      break;
  }
  /*
   * Check for trailing characters.
   */
  if (*cp && (!isascii(*cp) || !isspace(*cp)))
    return 0;
  /*
   * Make the address according to
   * the number of parts specified.
   */
  n = pp - parts + 1;
  switch (n) {

  case 1:       /* a -- 32 bits */
    break;

  case 2:       /* a.b -- 8.24 bits */
    if (val > 0xffffff)
      return 0;
    val |= parts[0] << 24;
    break;

  case 3:       /* a.b.c -- 8.8.16 bits */
    if (val > 0xffff)
      return 0;
    val |= (parts[0] << 24) | (parts[1] << 16);
    break;

  case 4:       /* a.b.c.d -- 8.8.8.8 bits */
    if (val > 0xff)
      return 0;
    val |= (parts[0] << 24) | (parts[1] << 16) | (parts[2] << 8);
    break;
  }
  if (addr)
    addr->s_addr = htonl(val);
  return 1;
}
