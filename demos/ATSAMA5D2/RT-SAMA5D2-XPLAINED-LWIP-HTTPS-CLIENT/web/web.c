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

/*
 * This file is a modified version of the lwIP web server demo. The original
 * author is unknown because the file didn't contain any license information.
 */

/**
 * @file web.c
 * @brief HTTP client code.
 * @{
 */
#include <ctype.h>

#include "ch.h"

#include "lwip/netdb.h"
#include "lwip/sockets.h"

#include <string.h>

#include "wolfssl_chibios.h"
#include "web.h"
#include "chprintf.h"

extern unsigned char client_cert[];
extern unsigned int client_cert_len;

/* HTTP request */
static const char *REQUEST = "GET "CONFIG_RESOURCE" HTTP/1.0\r\n"
                             "Host: "CONFIG_WEBSITE"\r\n"
                             "User-Agent: SAMA5D2\r\n"
                             "\r\n";

static char recv_buf[1000];

struct addrinfo hints = {
  .ai_family = AF_INET,
  .ai_socktype = SOCK_STREAM,
  };

static struct addrinfo *res;

static WOLFSSL_CTX* ctx;
static WOLFSSL* ssl;
static WOLFSSL_METHOD* method;

/*
 * @brief HTTPS client functions.
 */
int https_client(void) {

  /* Resolve the IP of the target website */
  int result = getaddrinfo(CONFIG_WEBSITE, WEB_PORT, &hints, &res);

  if((result != 0) || (res == NULL)) {
    chprintf((BaseSequentialStream *)&SD1, "Unable to resolve IP for target website %s\n\r", CONFIG_WEBSITE);
    return 0;
  }

  chprintf((BaseSequentialStream *)&SD1,"Target website's IP resolved\n\r");

  /* create a new socket */
  int s = socket(AF_INET, SOCK_STREAM, 0);

  if(s < 0) {
    chprintf((BaseSequentialStream *)&SD1, "Unable to allocate a new socket\n\r");
    return 0;
  }

  chprintf((BaseSequentialStream *)&SD1, "Socket allocated, id=%d\n\r", s);

  /* connect to the specified server */
  result = connect(s, res->ai_addr, res->ai_addrlen);
  if(result != 0) {
    chprintf((BaseSequentialStream *)&SD1, "Unable to connect to the target website\n\r");
    close(s);
    return 0;
  }

  chprintf((BaseSequentialStream *)&SD1, "Connected to the target website\n\r");

  /* initialize wolfssl library */
  wolfSSL_Init();
  method = wolfTLSv1_2_client_method(); /* use TLS v1.2 */

  /* make new ssl context */
  if ((ctx = wolfSSL_CTX_new(method)) == NULL) {
    chprintf((BaseSequentialStream *)&SD1, "wolfSSL_CTX_new error\n\r");
    close(s);
    return 0;
  }

  wolfSSL_SetIORecv(ctx, wolfssl_recv_cb) ;
  wolfSSL_SetIOSend(ctx, wolfssl_send_cb) ;

  /* Add cert to ctx */
  int e = 0;

  e = wolfSSL_CTX_load_verify_buffer(ctx, client_cert, client_cert_len, SSL_FILETYPE_ASN1);
  if (e != SSL_SUCCESS) {
    chprintf((BaseSequentialStream *)&SD1, "Error loading client certs\n\r");
    close(s);
    return 0;
  }

  /* TODO: delete this line */
  wolfSSL_CTX_set_verify(ctx, SSL_VERIFY_NONE, 0);

  /* make new wolfSSL struct */
  if ((ssl = wolfSSL_new(ctx)) == NULL) {
    chprintf((BaseSequentialStream *)&SD1, "wolfSSL_new error\n\r");
    close(s);
    return 0;
  }

  /* Connect wolfssl to the socket, server, then send message */
  e = wolfSSL_set_fd(ssl, s);
  if (e != SSL_SUCCESS) {
    chprintf((BaseSequentialStream *)&SD1, "wolfSSL_set_fd error\n\r");
    close(s);
    return 0;
  }

  e = wolfSSL_connect(ssl);
  if (e != SSL_SUCCESS) {
    chprintf((BaseSequentialStream *)&SD1, "wolfSSL_connect error\n\r");
    close(s);
    return 0;
  }

  /* send the request */
  result = wolfSSL_write(ssl, REQUEST, strlen(REQUEST));
  if(result < 0) {
    chprintf((BaseSequentialStream *)&SD1, "Unable to send the HTTP request\r\n");
    close(s);
    return 0;
  }
  chprintf((BaseSequentialStream *)&SD1, "HTTPS request sent\n\r");

  /* print the response */
  chprintf((BaseSequentialStream *)&SD1, "HTTPS response:\n\r");
  chprintf((BaseSequentialStream *)&SD1,"--------------------------------------------------------------------------------\n\r");
  int r;
  do {
    memset(recv_buf, 0, sizeof(recv_buf));
    r = wolfSSL_read(ssl, recv_buf, sizeof(recv_buf) - 1);
    chprintf((BaseSequentialStream *)&SD1,"%s\n\r",recv_buf);
  } while(r > 0);
  chprintf((BaseSequentialStream *)&SD1, "--------------------------------------------------------------------------------\n\r");

  /* frees all data before client termination */
  wolfSSL_free(ssl);
  wolfSSL_CTX_free(ctx);
  wolfSSL_Cleanup();

  lwip_freeaddrinfo(res);
  close(s);
  chprintf((BaseSequentialStream *)&SD1, "Socket closed\n\r");

  return 1;
}

/** @} */
