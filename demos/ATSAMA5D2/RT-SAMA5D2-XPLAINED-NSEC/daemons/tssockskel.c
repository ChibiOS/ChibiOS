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
 * @file    tssockskel.c
 * @brief   Sockets skeleton daemon for trusted clients.
 *
 */

#include "ch.h"
#include "chobjfifos.h"
#include "tsclient.h"
#include "tsdaemonskels.h"
#include "tssockskel.h"
#include <string.h>

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
static skel_ctx_t skel_ctx;

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/**
 * @name    Sockets API skeletons.
 * @{
 */

/**
 * @brief int socket(int domain, int type, int protocol)
 */
static void l_socket(skel_req_t *skreqp) {
  int result;

  /* call the api exposed by the TCP/IP stack.*/
  result = socket((int)skreqp->stub_op_p[0],
                  (int)skreqp->stub_op_p[1],
                  (int)skreqp->stub_op_p[2]);

  /* report the result and copy the 'out' parameters.*/
  returnToRemote(skreqp, result);
}

/**
 * @brief int connect(int s, const struct sockaddr *name, socklen_t namelen)
 */
static void l_connect(skel_req_t *skreqp) {
  int s, result, socklen;
  struct sockaddr sockaddr;

  s = (int)skreqp->stub_op_p[0];
  skreqp->stub_op_p[1] = (uint32_t)&sockaddr;
  socklen = (int)skreqp->stub_op_p[2];

  /* Call the stub service in order to copy the 'in' parameter
     sockaddr.*/
  paramsInFromRemote(skreqp);

  /* Call the api exposed by the TCP/IP stack.*/
  result = connect(s, &sockaddr, socklen);

  /* Report the result.*/
  returnToRemote(skreqp, result);
}

/**
 * @brief int close(int s)
 */
static void l_close(skel_req_t *skreqp) {
  int result;

  /* Call the api exposed by the TCP/IP stack.*/
  result = close((int)skreqp->stub_op_p[0]);

  /* report the result.*/
  returnToRemote(skreqp, result);
}


/**
 * @brief int recv(int s, void *mem, size_t len, int flags)
 */
static void l_recv(skel_req_t *skreqp) {
  int result;
  void *mem;
  size_t len;

  len = skreqp->stub_op_p[2];

  /* Allocate the space for the receive buffer.*/
  mem = chHeapAlloc(NULL, len);
  if (NULL == mem) {
    result = ENOMEM;
  } else {

    /* call the api exposed by the TCP/IP stack.*/
    result = recv((int)skreqp->stub_op_p[0], mem, len,
        (int)skreqp->stub_op_p[3]);
    skreqp->stub_op_p_sz[1] = result;
    skreqp->stub_op_p[1] = (uint32_t)mem;
  }

  /* report the result and copy 'out' parameter mem.*/
  returnToRemote(skreqp, result);
  if (NULL != mem)
    chHeapFree(mem);
}

/**
 * @brief int send(int s, const void *dataptr, size_t size, int flags)
 */
static void l_send(skel_req_t *skreqp) {
  int result;
  void *dataptr;
  size_t size;

  size = skreqp->stub_op_p[2];

  /* Allocate the space for the send buffer.*/
  dataptr = chHeapAlloc(NULL, size);
  if (NULL == dataptr) {
    result = ENOMEM;
  } else {
    skreqp->stub_op_p[1] = (uint32_t)dataptr;

    /* call the stub service in order to copy the
       'in' parameter dataptr.*/
    paramsInFromRemote(skreqp);

    /* call the api exposed by the TCP/IP stack.*/
    result = send((int)skreqp->stub_op_p[0], dataptr, size,
        (int)skreqp->stub_op_p[3]);
    chHeapFree(dataptr);
  }

  /* report the result.*/
  returnToRemote(skreqp, result);
}


/**
 * @brief int select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset,
 *                    struct timeval *timeout)
 */
static void l_select(skel_req_t *skreqp) {
  int result;
  int maxfdpl;
  fd_set readset, writeset, exceptset;
  struct timeval timeout;

  maxfdpl = skreqp->stub_op_p[0];

  skreqp->stub_op_p_sz[1] = sizeof (fd_set);
  skreqp->stub_op_p_sz[2] = sizeof (fd_set);
  skreqp->stub_op_p_sz[3] = sizeof (fd_set);
  skreqp->stub_op_p[1] = (uint32_t)&readset;
  skreqp->stub_op_p[2] = (uint32_t)&writeset;
  skreqp->stub_op_p[3] = (uint32_t)&exceptset;
  skreqp->stub_op_p[4] = (uint32_t)&timeout;

  /* call the stub service in order to copy the
     'in' parameter readset, writeset, exceptset and timeout.*/
  paramsInFromRemote(skreqp);

  /* call the api exposed by the TCP/IP stack.*/
  result = select(maxfdpl, &readset, &writeset, &exceptset, &timeout);

  /* report the result and the parameters readset, writeset and exceptset.*/
  returnToRemote(skreqp, result);
}

/**
 * @brief bind(int s, const struct sockaddr *name, socklen_t namelen);
 */
static void l_bind(skel_req_t *skreqp) {
  int s, result, socklen;
  struct sockaddr sockaddr;

  s = (int)skreqp->stub_op_p[0];
  skreqp->stub_op_p[1] = (uint32_t)&sockaddr;
  socklen = (int)skreqp->stub_op_p[2];

  /* Call the stub service in order to copy the 'in' parameter
     sockaddr.*/
  paramsInFromRemote(skreqp);

  /* Call the api exposed by the TCP/IP stack.*/
  result = bind(s, &sockaddr, socklen);

  /* Report the result.*/
  returnToRemote(skreqp, result);
}

/**
 * @brief listen(int s, int backlog);
 */
static void l_listen(skel_req_t *skreqp) {
  int s, result, backlog;

  s = (int)skreqp->stub_op_p[0];
  backlog = (int)skreqp->stub_op_p[1];

  /* Call the api exposed by the TCP/IP stack.*/
  result = listen(s, backlog);

  /* Report the result.*/
  returnToRemote(skreqp, result);
}

/**
 * @brief   Socket Daemon. Dispatch a request to a local method.
 */
static THD_FUNCTION(TsSockSkelDaemon, arg) {
  objects_fifo_t *ofp = arg;
  skel_req_t *skreqp;

  for (;/* ever */;) {
    chFifoReceiveObjectTimeout(ofp, (void **)&skreqp,
        TIME_INFINITE);
    switch (skreqp->stub_op_code) {
    case SOCK_OP_SOCKET:
      l_socket(skreqp);
      break;
    case SOCK_OP_CONNECT:
      l_connect(skreqp);
      break;
    case SOCK_OP_CLOSE:
      l_close(skreqp);
      break;
    case SOCK_OP_RECV:
      l_recv(skreqp);
      break;
    case SOCK_OP_SEND:
      l_send(skreqp);
      break;
    case SOCK_OP_SELECT:
      l_select(skreqp);
      break;
    case SOCK_OP_BIND:
      l_bind(skreqp);
      break;
    case SOCK_OP_LISTEN:
      l_listen(skreqp);
      break;
    default:
      break;
    }
  }
}

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/**
 * @brief   Init the socket skeletons daemon objects and create the
 *          corresponding threads.
 */
void tsSocksSkelInit(void) {
  int i;
  skel_ctx_t *scp;

  scp = &skel_ctx;
  chFifoObjectInit(&scp->skel_req_fifo, sizeof (skel_req_t), N_MAX_SKEL_REQS,
      sizeof (uint8_t), scp->skel_reqs, scp->skel_req_msgs);
  scp->skel_eventflag = EVT_F_SOCK_NEW_OP;
  scp->stub_svc_name = SOCKS_SVC_NAME;

  for (i = 0; i < N_SOCKSKEL_THD; ++i)
    chThdCreateFromHeap(NULL, 2048, "TsSockSkelDaemonWrk", NORMALPRIO,
        TsSockSkelDaemon, &scp->skel_req_fifo);
  chThdCreateFromHeap(NULL, 2048, "TsSocksSkelDaemon", NORMALPRIO,
      TsSkelsDaemon, scp);
}

/** @} */
