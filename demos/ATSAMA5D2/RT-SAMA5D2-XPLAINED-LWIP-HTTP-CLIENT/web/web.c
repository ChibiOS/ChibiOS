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

#include "ch.h"

#include "lwip/netdb.h"
#include "lwip/sockets.h"

#include <string.h>

#include "web.h"
#include "chprintf.h"

/* HTTP request */
static const char *REQUEST = "GET "CONFIG_RESOURCE" HTTP/1.0\r\n"
                             "Host: "CONFIG_WEBSITE"\r\n"
                             "User-Agent: SAMA5D2\r\n"
                             "\r\n";

static char recv_buf[100];

struct addrinfo hints = {
  .ai_family = AF_INET,
  .ai_socktype = SOCK_STREAM,
  };

static struct addrinfo *res;

/*
 * @brief HTTP client functions.
 */
int http_client(void) {

  /* Resolve the IP of the target website */
  int result = getaddrinfo(CONFIG_WEBSITE, "80", &hints, &res);

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

  /* send the request */
  result = write(s, REQUEST, strlen(REQUEST));
  if(result < 0) {
    chprintf((BaseSequentialStream *)&SD1, "Unable to send the HTTP request\r\n");
    close(s);
    return 0;
  }
  chprintf((BaseSequentialStream *)&SD1, "HTTP request sent\n\r");

  /* print the response */
  chprintf((BaseSequentialStream *)&SD1, "HTTP response:\n\r");
  chprintf((BaseSequentialStream *)&SD1,"--------------------------------------------------------------------------------\n\r");
  int r;
  do {
    memset(recv_buf, 0, sizeof(recv_buf));
    r = read(s, recv_buf, sizeof(recv_buf) - 1);
    chprintf((BaseSequentialStream *)&SD1,"%s\n\r",recv_buf);
  } while(r > 0);
  chprintf((BaseSequentialStream *)&SD1, "--------------------------------------------------------------------------------\n\r");

  lwip_freeaddrinfo(res);
  close(s);
  chprintf((BaseSequentialStream *)&SD1, "Socket closed\n\r");

  return 1;
}

/** @} */
