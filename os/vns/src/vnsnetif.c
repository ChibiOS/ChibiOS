/*
    ChibiOS - Copyright (C) 2006-2026 Giovanni Di Sirio.

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
 * @file        vnsnetif.c
 * @brief       Generated VNS base classes. source.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  VNSNETIF
 * @{
 */

#include "vns.h"

/*===========================================================================*/
/* Module local definitions.                                                 */
/*===========================================================================*/

/*===========================================================================*/
/* Module local macros.                                                      */
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

/*===========================================================================*/
/* Module local functions.                                                   */
/*===========================================================================*/

/*===========================================================================*/
/* Module exported functions.                                                */
/*===========================================================================*/

/*===========================================================================*/
/* Module class "vns_socket_c" methods.                                      */
/*===========================================================================*/

/**
 * @name        Methods implementations of vns_socket_c
 * @{
 */
/**
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p vns_socket_c instance to be
 *                              initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @return                      A new reference to the object.
 */
void *__vnssck_objinit_impl(void *ip, const void *vmt) {
  vns_socket_c *self = (vns_socket_c *)ip;

  /* Initialization of the ancestors-defined parts.*/
  __bo_objinit_impl(self, vmt);

  /* No initialization code.*/

  return self;
}

/**
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p vns_socket_c instance to be
 *                              disposed.
 */
void __vnssck_dispose_impl(void *ip) {
  vns_socket_c *self = (vns_socket_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __bo_dispose_impl(self);
}

/**
 * @brief       Implementation of method @p vnssckConnect().
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p vns_socket_c instance.
 * @param[in]     addr          Pointer to the remote endpoint address.
 * @param[in]     timeout       Maximum time allowed for the operation to
 *                              complete.
 * @return                      The operation result.
 */
msg_t __vnssck_connects_impl(void *ip, const vns_address_t *addr,
                             sysinterval_t timeout) {
  vns_socket_c *self = (vns_socket_c *)ip;

  (void)addr;
  (void)timeout;

  return CH_RET_ENOSYS;
}

/**
 * @brief       Implementation of method @p vnssckBind().
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p vns_socket_c instance.
 * @param[in]     addr          Pointer to the local endpoint address.
 * @return                      The operation result.
 */
msg_t __vnssck_binds_impl(void *ip, const vns_address_t *addr) {
  vns_socket_c *self = (vns_socket_c *)ip;

  (void)addr;

  return CH_RET_ENOSYS;
}

/**
 * @brief       Implementation of method @p vnssckListen().
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p vns_socket_c instance.
 * @param[in]     backlog       Maximum number of pending incoming connections.
 * @return                      The operation result.
 */
msg_t __vnssck_listens_impl(void *ip, unsigned backlog) {
  vns_socket_c *self = (vns_socket_c *)ip;

  (void)backlog;

  return CH_RET_ENOSYS;
}

/**
 * @brief       Implementation of method @p vnssckAccept().
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p vns_socket_c instance.
 * @param[out]    spp           Pointer to the variable that will receive the
 *                              accepted socket object.
 * @param[in]     timeout       Maximum time allowed for the operation to
 *                              complete.
 * @return                      The operation result.
 */
msg_t __vnssck_accepts_impl(void *ip, vns_socket_c **spp,
                            sysinterval_t timeout) {
  vns_socket_c *self = (vns_socket_c *)ip;

  (void)spp;
  (void)timeout;

  return CH_RET_ENOSYS;
}

/**
 * @brief       Implementation of method @p vnssckSend().
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p vns_socket_c instance.
 * @param[in]     buf           Pointer to the data buffer to be transmitted.
 * @param[in]     n             Number of bytes to transmit.
 * @param[in]     timeout       Maximum time allowed for the operation to
 *                              complete.
 * @return                      The number of bytes sent, or a negative error
 *                              code.
 */
ssize_t __vnssck_sends_impl(void *ip, const void *buf, size_t n,
                            sysinterval_t timeout) {
  vns_socket_c *self = (vns_socket_c *)ip;

  (void)buf;
  (void)n;
  (void)timeout;

  return CH_RET_ENOSYS;
}

/**
 * @brief       Implementation of method @p vnssckReceive().
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p vns_socket_c instance.
 * @param[out]    buf           Pointer to the destination buffer.
 * @param[in]     n             Maximum number of bytes to receive.
 * @param[in]     timeout       Maximum time allowed for the operation to
 *                              complete.
 * @return                      The number of bytes received, or a negative
 *                              error code.
 */
ssize_t __vnssck_receives_impl(void *ip, void *buf, size_t n,
                               sysinterval_t timeout) {
  vns_socket_c *self = (vns_socket_c *)ip;

  (void)buf;
  (void)n;
  (void)timeout;

  return CH_RET_ENOSYS;
}

/**
 * @brief       Implementation of method @p vnssckSendTo().
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p vns_socket_c instance.
 * @param[in]     buf           Pointer to the data buffer to be transmitted.
 * @param[in]     n             Number of bytes to transmit.
 * @param[in]     addr          Pointer to the destination endpoint address.
 * @param[in]     timeout       Maximum time allowed for the operation to
 *                              complete.
 * @return                      The number of bytes sent, or a negative error
 *                              code.
 */
ssize_t __vnssck_sendsto_impl(void *ip, const void *buf, size_t n,
                              const vns_address_t *addr, sysinterval_t timeout) {
  vns_socket_c *self = (vns_socket_c *)ip;

  (void)buf;
  (void)n;
  (void)addr;
  (void)timeout;

  return CH_RET_ENOSYS;
}

/**
 * @brief       Implementation of method @p vnssckReceiveFrom().
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p vns_socket_c instance.
 * @param[out]    buf           Pointer to the destination buffer.
 * @param[in]     n             Maximum number of bytes to receive.
 * @param[out]    addr          Pointer to the variable that will receive the
 *                              source endpoint address.
 * @param[in]     timeout       Maximum time allowed for the operation to
 *                              complete.
 * @return                      The number of bytes received, or a negative
 *                              error code.
 */
ssize_t __vnssck_receivesfrom_impl(void *ip, void *buf, size_t n,
                                   vns_address_t *addr, sysinterval_t timeout) {
  vns_socket_c *self = (vns_socket_c *)ip;

  (void)buf;
  (void)n;
  (void)addr;
  (void)timeout;

  return CH_RET_ENOSYS;
}

/**
 * @brief       Implementation of method @p vnssckClose().
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p vns_socket_c instance.
 * @return                      The operation result.
 */
msg_t __vnssck_closes_impl(void *ip) {
  vns_socket_c *self = (vns_socket_c *)ip;

  return CH_RET_ENOSYS;
}
/** @} */

/*===========================================================================*/
/* Module class "vns_netif_c" methods.                                       */
/*===========================================================================*/

/**
 * @name        Methods implementations of vns_netif_c
 * @{
 */
/**
 * @brief       Implementation of object creation.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[out]    ip            Pointer to a @p vns_netif_c instance to be
 *                              initialized.
 * @param[in]     vmt           VMT pointer for the new object.
 * @return                      A new reference to the object.
 */
void *__vnsif_objinit_impl(void *ip, const void *vmt) {
  vns_netif_c *self = (vns_netif_c *)ip;

  /* Initialization of the ancestors-defined parts.*/
  __bo_objinit_impl(self, vmt);

  /* No initialization code.*/

  return self;
}

/**
 * @brief       Implementation of object finalization.
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p vns_netif_c instance to be
 *                              disposed.
 */
void __vnsif_dispose_impl(void *ip) {
  vns_netif_c *self = (vns_netif_c *)ip;

  /* No finalization code.*/
  (void)self;

  /* Finalization of the ancestors-defined parts.*/
  __bo_dispose_impl(self);
}

/**
 * @brief       Implementation of method @p vnsifCreateSocket().
 * @note        This function is meant to be used by derived classes.
 *
 * @param[in,out] ip            Pointer to a @p vns_netif_c instance.
 * @param[in]     domain        Address family of the socket, for example @p
 *                              AF_INET.
 * @param[in]     type          Communication type of the socket, for example
 *                              stream or datagram.
 * @param[out]    spp           Pointer to the variable that will receive the
 *                              created socket object.
 * @return                      The operation result.
 */
msg_t __vnsif_creates_impl(void *ip, vns_domain_t domain, vns_socktype_t type,
                           vns_socket_c **spp) {
  vns_netif_c *self = (vns_netif_c *)ip;

  (void)domain;
  (void)type;
  (void)spp;

  return CH_RET_ENOSYS;
}
/** @} */

/** @} */
