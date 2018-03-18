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
#include "tssockstub.h"
#include <string.h>
#include <ctype.h>

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

#define METHOD_MAX_PARAMS   6
#define STUB_MAX_OPS        32

#define OP_PRMDIR_NONE      0
#define OP_PRMDIR_IN        1
#define OP_PRMDIR_OUT       2

/*===========================================================================*/
/* Module exported variables.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module local types.                                                       */
/*===========================================================================*/
typedef struct stub_op stub_op_t;
typedef enum {FREE=0, CALLING, PENDING} op_state_t;

typedef struct stub_param {
  uint32_t  dir;
  uint32_t  val;
  uint32_t  size;
} stub_parm_t;

typedef struct stub_op {
  uint32_t            op_code;  /* e.g. connect, recv, sendv, close, etc.*/
  op_state_t          op_state; /* calling, pending, free.*/
  stub_parm_t         op_p[METHOD_MAX_PARAMS];
  thread_reference_t  op_wthdp; /* TS internal client thread (the caller).*/
} stub_op_t;

/*===========================================================================*/
/* Module local variables.                                                   */
/*===========================================================================*/

static objects_fifo_t ops_fifo;
static msg_t ops_msgs[STUB_MAX_OPS];
static struct stub_op ops[STUB_MAX_OPS] = {0};
static bool tsSkelIsReady = false;

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

static bool isOpValid(stub_op_t *op)
{
  if ((op < &ops[0]) || (op >= &ops[STUB_MAX_OPS]))
    return FALSE;
  if (((char *)op - (char *)&ops[0]) % sizeof ops[0])
    return FALSE;
  return TRUE;
}

/**
 * @brief   Implement an a call to a NSEC function.
 * @details It activates the channel between the stubs service and
 *          the skels daemon running in the nsec world.
 *          To do it, it uses an event to signal the skels
 *          daemon that a new op request is ready to be executed.
 *          Behind the scenes, the skels daemon will then gets the op, calling
 *          the stubs service via smc. The daemon executes it and then calls
 *          the stubs service again to post the result and to wake up the
 *          calling thread of this function.
 *
 * @param[in] op    the 'remote' method description.
 *
 * @return          the return value of 'remote' method.
 */
static uint32_t callRemote(stub_op_t *op) {
  uint32_t r;

  chSysLock();
  chFifoSendObjectI(&ops_fifo, op);
  chEvtBroadcastFlagsI(&tsEventSource, EVT_F_SOCK_NEW_OP);
  chThdSuspendS(&op->op_wthdp);
  chSysUnlock();
  r = op->op_code;
  chFifoReturnObject(&ops_fifo, op);
  return r;
}

static stub_op_t *getNewOp(void) {
  stub_op_t *op = chFifoTakeObjectTimeout(&ops_fifo, TIME_INFINITE);
  memset(op, 0, sizeof *op);
  op->op_state = CALLING;
  return op;
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief     The stubs service.
 * @details   And this is where the magic happens.
 */
THD_WORKING_AREA(waTsStubsService, 1024);
THD_FUNCTION(TsStubsService, tsstate) {
  ts_state_t *svcp = tsstate;
  skel_req_t *skrp;
  stub_op_t *op;
  msg_t r;
  int i;

  chFifoObjectInit(&ops_fifo, sizeof (stub_op_t), STUB_MAX_OPS,
      sizeof (uint8_t), ops, ops_msgs);
  for (;/* ever */;) {

    /* Wait a service request.*/
    (void)tssiWaitRequest(svcp);
    skrp = (skel_req_t *)TS_GET_DATA(svcp);
    r = SMC_SVC_OK;

    /* Process the request.*/
    if (TS_GET_DATALEN(svcp) != sizeof (skel_req_t)) {
      TS_SET_STATUS(svcp, SMC_SVC_INVALID);
      continue;
    }

    switch (skrp->req) {
    case SKEL_REQ_READY:
      tsSkelIsReady = true;
      break;

    case SKEL_REQ_GETOP:

      /* The nsec skeleton calls us to get a new op ready to be executed.*/
      if (chFifoReceiveObjectTimeout(&ops_fifo, (void **)&op, TIME_IMMEDIATE) ==
            MSG_TIMEOUT) {

        /* no op ready to be executed.*/
        r = SMC_SVC_NHND;
        break;
      }
      skrp->stub_op = (uint32_t)op;
      skrp->stub_op_code = op->op_code;

      /* Pass all the 'by value' arguments from stub to skel.*/
      for (i = 0; i < METHOD_MAX_PARAMS; ++i) {
        if (op->op_p[i].dir == OP_PRMDIR_NONE)
          skrp->stub_op_p[i] = op->op_p[i].val;
      }
      op->op_state = PENDING;
      break;

    case SKEL_REQ_CPYPRMS:

      /* The nsec skel calls us to get a copy of the 'in' parameters of
         the specified op.
         An 'in' parameter is an indirect argument, that is an argument
         the value of which is a pointer to a memory buffer, that
         must be copied in a non secure memory buffer.
         It represents data to be consumed by the callee.*/
      op = (stub_op_t *)skrp->stub_op;
      if (!isOpValid(op) || op->op_state != PENDING ||
            op->op_code != skrp->stub_op_code) {
        r = SMC_SVC_INVALID;
        break;
      }

      /* Copy all 'in' parameters.
         For each parameter check that the destination memory area
         is in the non secure memory arena.*/
      for (i = 0; i < METHOD_MAX_PARAMS; ++i) {
        if ((op->op_p[i].dir & OP_PRMDIR_IN) == 0)
          continue;
        if (!tsIsAddrSpaceValid((void *)skrp->stub_op_p[i], op->op_p[i].size)) {
          r = SMC_SVC_INVALID;
          break;
        }
        memcpy((void *)skrp->stub_op_p[i], (void *)op->op_p[i].val,
            op->op_p[i].size);
      }
      break;

    case SKEL_REQ_PUTRES:

      /* The nsec skel calls us to put a copy of the 'out' parameters of
         the specified op.
         An 'out' parameter is an indirect argument, that is an argument
         the value of which is a pointer to a memory buffer, that
         must be copied in a secure memory buffer.
         It represents data produced by the callee.*/
      op = (stub_op_t *)skrp->stub_op;
      if (!isOpValid(op) || op->op_state != PENDING ||
            op->op_code != skrp->stub_op_code) {
        r = SMC_SVC_INVALID;
        break;
      }

      /* Copy all 'out' parameters.
         For each parameter check that the source memory area
         is in the non secure memory arena, and that the size returned
         fits in the caller buffer size.*/
      for (i = 0; i < METHOD_MAX_PARAMS; ++i) {
        if ((op->op_p[i].dir & OP_PRMDIR_OUT) == 0)
          continue;
        if (!tsIsAddrSpaceValid((void *)skrp->stub_op_p[i], skrp->stub_op_p_sz[i])
              || (skrp->stub_op_p_sz[i] > op->op_p[i].size)) {
          r = SMC_SVC_INVALID;
          break;
        }
        memcpy((void *)op->op_p[i].val, (void *)skrp->stub_op_p[i],
            skrp->stub_op_p_sz[i]);
      }
      if (r != SMC_SVC_OK)
        break;

      /* Set the return value of the 'remote' callee method,
         and wake up the caller.*/
      op->op_code = skrp->stub_op_result;
      chThdResume(&op->op_wthdp, MSG_OK);
      break;

    default:
      r = SMC_SVC_INVALID;
      break;
    }

    /* Set the response.*/
    TS_SET_STATUS(svcp, r);
  }
}
/**
 * @brief     Is the skeletons daemon ready to operate?
 * @details   It is used at the startup to synchronize the
 *            stub service with the skeleton daemon.
 */
void tsWaitStubSkelReady(void) {
  while (!tsSkelIsReady) {
    chThdSleepMilliseconds(100);
  }
}

/**
 * @brief The sockets API.
 */
int socket(int domain, int type, int protocol) {
  stub_op_t *op = getNewOp();
  op->op_code = STUB_OP_SOCKET;
  op->op_p[0].dir = OP_PRMDIR_NONE;
  op->op_p[0].val = (uint32_t)domain;
  op->op_p[1].dir = OP_PRMDIR_NONE;
  op->op_p[1].val = (uint32_t)type;
  op->op_p[2].dir = OP_PRMDIR_NONE;
  op->op_p[2].val = (uint32_t)protocol;
  return (int)callRemote(op);
}

int connect(int s, const struct sockaddr *name, socklen_t namelen) {
  stub_op_t *op = getNewOp();
  op->op_code = STUB_OP_CONNECT;
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
  stub_op_t *op = getNewOp();
  op->op_code = STUB_OP_CLOSE;
  op->op_p[0].dir = OP_PRMDIR_NONE;
  op->op_p[0].val = (uint32_t)s;
  return (int)callRemote(op);
}

int recv(int s, void *mem, size_t len, int flags) {
  stub_op_t *op = getNewOp();
  op->op_code = STUB_OP_RECV;
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
  stub_op_t *op = getNewOp();
  op->op_code = STUB_OP_SEND;
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
  stub_op_t *op = getNewOp();
  op->op_code = STUB_OP_SELECT;
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
  stub_op_t *op = getNewOp();
  op->op_code = STUB_OP_BIND;
  op->op_p[0].dir = OP_PRMDIR_NONE;
  op->op_p[0].val = (uint32_t)s;
  op->op_p[1].dir = OP_PRMDIR_IN;
  op->op_p[1].val = (uint32_t)name;
  op->op_p[1].size = (uint32_t)namelen;
  op->op_p[2].dir = OP_PRMDIR_NONE;
  op->op_p[2].val = (uint32_t)namelen;
  return (int)callRemote(op);
}

#if 0
int accept(int s, struct sockaddr *addr, socklen_t *addrlen);
int shutdown(int s, int how);
int getpeername (int s, struct sockaddr *name, socklen_t *namelen);
int getsockname (int s, struct sockaddr *name, socklen_t *namelen);
int getsockopt (int s, int level, int optname, void *optval, socklen_t *optlen);
int setsockopt (int s, int level, int optname, const void *optval, socklen_t optlen);
int listen(int s, int backlog);
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
