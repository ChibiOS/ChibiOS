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
 * @file        vnsnetif.h
 * @brief       Generated VNS base classes. header.
 * @note        This is a generated file, do not edit directly.
 *
 * @addtogroup  VNSNETIF
 * @brief       VNS base classes.
 * @{
 */

#ifndef VNSNETIF_H
#define VNSNETIF_H


/*===========================================================================*/
/* Module constants.                                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Module pre-compile time settings.                                         */
/*===========================================================================*/

/*===========================================================================*/
/* Derived constants and error checks.                                       */
/*===========================================================================*/

/*===========================================================================*/
/* Module macros.                                                            */
/*===========================================================================*/

/*===========================================================================*/
/* Module data structures and types.                                         */
/*===========================================================================*/

/**
 * @class       vns_socket_c
 * @extends     base_object_c
 *
 * @brief       Common ancestor class of all socket classes.
 *
 * @name        Class @p vns_socket_c structures
 * @{
 */

/**
 * @brief       Type of a VNS socket class.
 */
typedef struct vns_socket vns_socket_c;

/**
 * @brief       Class @p vns_socket_c virtual methods table.
 */
struct vns_socket_vmt {
  /* From base_object_c.*/
  void (*dispose)(void *ip);
  /* From vns_socket_c.*/
  msg_t (*connects)(void *ip, const vns_address_t *addr, sysinterval_t timeout);
  msg_t (*binds)(void *ip, const vns_address_t *addr);
  msg_t (*listens)(void *ip, unsigned backlog);
  msg_t (*accepts)(void *ip, vns_socket_c **spp, sysinterval_t timeout);
  ssize_t (*sends)(void *ip, const void *buf, size_t n, sysinterval_t timeout);
  ssize_t (*receives)(void *ip, void *buf, size_t n, sysinterval_t timeout);
  ssize_t (*sendsto)(void *ip, const void *buf, size_t n, const vns_address_t *addr, sysinterval_t timeout);
  ssize_t (*receivesfrom)(void *ip, void *buf, size_t n, vns_address_t *addr, sysinterval_t timeout);
  msg_t (*closes)(void *ip);
};

/**
 * @brief       Structure representing a VNS socket class.
 */
struct vns_socket {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct vns_socket_vmt *vmt;
};
/** @} */

/**
 * @class       vns_netif_c
 * @extends     base_object_c
 *
 * @brief       Common ancestor class of all network interface classes.
 *
 * @name        Class @p vns_netif_c structures
 * @{
 */

/**
 * @brief       Type of a VNS netif class.
 */
typedef struct vns_netif vns_netif_c;

/**
 * @brief       Class @p vns_netif_c virtual methods table.
 */
struct vns_netif_vmt {
  /* From base_object_c.*/
  void (*dispose)(void *ip);
  /* From vns_netif_c.*/
  msg_t (*creates)(void *ip, vns_domain_t domain, vns_socktype_t type, vns_socket_c **spp);
};

/**
 * @brief       Structure representing a VNS netif class.
 */
struct vns_netif {
  /**
   * @brief       Virtual Methods Table.
   */
  const struct vns_netif_vmt *vmt;
};
/** @} */

/*===========================================================================*/
/* External declarations.                                                    */
/*===========================================================================*/

#ifdef __cplusplus
extern "C" {
#endif
  /* Methods of vns_socket_c.*/
  void *__vnssck_objinit_impl(void *ip, const void *vmt);
  void __vnssck_dispose_impl(void *ip);
  msg_t __vnssck_connects_impl(void *ip, const vns_address_t *addr,
                               sysinterval_t timeout);
  msg_t __vnssck_binds_impl(void *ip, const vns_address_t *addr);
  msg_t __vnssck_listens_impl(void *ip, unsigned backlog);
  msg_t __vnssck_accepts_impl(void *ip, vns_socket_c **spp,
                              sysinterval_t timeout);
  ssize_t __vnssck_sends_impl(void *ip, const void *buf, size_t n,
                              sysinterval_t timeout);
  ssize_t __vnssck_receives_impl(void *ip, void *buf, size_t n,
                                 sysinterval_t timeout);
  ssize_t __vnssck_sendsto_impl(void *ip, const void *buf, size_t n,
                                const vns_address_t *addr,
                                sysinterval_t timeout);
  ssize_t __vnssck_receivesfrom_impl(void *ip, void *buf, size_t n,
                                     vns_address_t *addr,
                                     sysinterval_t timeout);
  msg_t __vnssck_closes_impl(void *ip);
  /* Methods of vns_netif_c.*/
  void *__vnsif_objinit_impl(void *ip, const void *vmt);
  void __vnsif_dispose_impl(void *ip);
  msg_t __vnsif_creates_impl(void *ip, vns_domain_t domain,
                             vns_socktype_t type, vns_socket_c **spp);
#ifdef __cplusplus
}
#endif

/*===========================================================================*/
/* Module inline functions.                                                  */
/*===========================================================================*/

/**
 * @name        Virtual methods of vns_socket_c
 * @{
 */
/**
 * @brief       Connects the socket to a remote endpoint.
 *
 * @param[in,out] ip            Pointer to a @p vns_socket_c instance.
 * @param[in]     addr          Pointer to the remote endpoint address.
 * @param[in]     timeout       Maximum time allowed for the operation to
 *                              complete.
 * @return                      The operation result.
 *
 * @api
 */
CC_FORCE_INLINE
static inline msg_t vnssckConnect(void *ip, const vns_address_t *addr,
                                  sysinterval_t timeout) {
  vns_socket_c *self = (vns_socket_c *)ip;

  return self->vmt->connects(ip, addr, timeout);
}

/**
 * @brief       Binds the socket to a local endpoint.
 *
 * @param[in,out] ip            Pointer to a @p vns_socket_c instance.
 * @param[in]     addr          Pointer to the local endpoint address.
 * @return                      The operation result.
 *
 * @api
 */
CC_FORCE_INLINE
static inline msg_t vnssckBind(void *ip, const vns_address_t *addr) {
  vns_socket_c *self = (vns_socket_c *)ip;

  return self->vmt->binds(ip, addr);
}

/**
 * @brief       Marks the socket as a passive listening socket.
 *
 * @param[in,out] ip            Pointer to a @p vns_socket_c instance.
 * @param[in]     backlog       Maximum number of pending incoming connections.
 * @return                      The operation result.
 *
 * @api
 */
CC_FORCE_INLINE
static inline msg_t vnssckListen(void *ip, unsigned backlog) {
  vns_socket_c *self = (vns_socket_c *)ip;

  return self->vmt->listens(ip, backlog);
}

/**
 * @brief       Accepts an incoming connection and creates a new socket object.
 *
 * @param[in,out] ip            Pointer to a @p vns_socket_c instance.
 * @param[out]    spp           Pointer to the variable that will receive the
 *                              accepted socket object.
 * @param[in]     timeout       Maximum time allowed for the operation to
 *                              complete.
 * @return                      The operation result.
 *
 * @api
 */
CC_FORCE_INLINE
static inline msg_t vnssckAccept(void *ip, vns_socket_c **spp,
                                 sysinterval_t timeout) {
  vns_socket_c *self = (vns_socket_c *)ip;

  return self->vmt->accepts(ip, spp, timeout);
}

/**
 * @brief       Sends data through the socket.
 *
 * @param[in,out] ip            Pointer to a @p vns_socket_c instance.
 * @param[in]     buf           Pointer to the data buffer to be transmitted.
 * @param[in]     n             Number of bytes to transmit.
 * @param[in]     timeout       Maximum time allowed for the operation to
 *                              complete.
 * @return                      The number of bytes sent, or a negative error
 *                              code.
 *
 * @api
 */
CC_FORCE_INLINE
static inline ssize_t vnssckSend(void *ip, const void *buf, size_t n,
                                 sysinterval_t timeout) {
  vns_socket_c *self = (vns_socket_c *)ip;

  return self->vmt->sends(ip, buf, n, timeout);
}

/**
 * @brief       Receives data from the socket.
 *
 * @param[in,out] ip            Pointer to a @p vns_socket_c instance.
 * @param[out]    buf           Pointer to the destination buffer.
 * @param[in]     n             Maximum number of bytes to receive.
 * @param[in]     timeout       Maximum time allowed for the operation to
 *                              complete.
 * @return                      The number of bytes received, or a negative
 *                              error code.
 *
 * @api
 */
CC_FORCE_INLINE
static inline ssize_t vnssckReceive(void *ip, void *buf, size_t n,
                                    sysinterval_t timeout) {
  vns_socket_c *self = (vns_socket_c *)ip;

  return self->vmt->receives(ip, buf, n, timeout);
}

/**
 * @brief       Sends data to a specified remote endpoint.
 *
 * @param[in,out] ip            Pointer to a @p vns_socket_c instance.
 * @param[in]     buf           Pointer to the data buffer to be transmitted.
 * @param[in]     n             Number of bytes to transmit.
 * @param[in]     addr          Pointer to the destination endpoint address.
 * @param[in]     timeout       Maximum time allowed for the operation to
 *                              complete.
 * @return                      The number of bytes sent, or a negative error
 *                              code.
 *
 * @api
 */
CC_FORCE_INLINE
static inline ssize_t vnssckSendTo(void *ip, const void *buf, size_t n,
                                   const vns_address_t *addr,
                                   sysinterval_t timeout) {
  vns_socket_c *self = (vns_socket_c *)ip;

  return self->vmt->sendsto(ip, buf, n, addr, timeout);
}

/**
 * @brief       Receives data and returns the source endpoint address.
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
 *
 * @api
 */
CC_FORCE_INLINE
static inline ssize_t vnssckReceiveFrom(void *ip, void *buf, size_t n,
                                        vns_address_t *addr,
                                        sysinterval_t timeout) {
  vns_socket_c *self = (vns_socket_c *)ip;

  return self->vmt->receivesfrom(ip, buf, n, addr, timeout);
}

/**
 * @brief       Closes the socket endpoint.
 *
 * @param[in,out] ip            Pointer to a @p vns_socket_c instance.
 * @return                      The operation result.
 *
 * @api
 */
CC_FORCE_INLINE
static inline msg_t vnssckClose(void *ip) {
  vns_socket_c *self = (vns_socket_c *)ip;

  return self->vmt->closes(ip);
}
/** @} */

/**
 * @name        Virtual methods of vns_netif_c
 * @{
 */
/**
 * @brief       Creates a socket object bound to this interface.
 *
 * @param[in,out] ip            Pointer to a @p vns_netif_c instance.
 * @param[in]     domain        Address family of the socket, for example @p
 *                              AF_INET.
 * @param[in]     type          Communication type of the socket, for example
 *                              stream or datagram.
 * @param[out]    spp           Pointer to the variable that will receive the
 *                              created socket object.
 * @return                      The operation result.
 *
 * @api
 */
CC_FORCE_INLINE
static inline msg_t vnsifCreateSocket(void *ip, vns_domain_t domain,
                                      vns_socktype_t type, vns_socket_c **spp) {
  vns_netif_c *self = (vns_netif_c *)ip;

  return self->vmt->creates(ip, domain, type, spp);
}
/** @} */

#endif /* VNSNETIF_H */

/** @} */
