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

#include "ch.h"
#include "hal.h"
#include <ctype.h>

#include "chtssi.h"
#include "proxies/tssockstub.h"
#include "wolfssl_chibios.h"
#include "rt_test_root.h"
#include "oslib_test_root.h"
#include "chprintf.h"
//#include <stdio.h>
#include <string.h>

#define CONFIG_RESOURCE    "/"
#define CONFIG_WEBSITE     "www.howsmyssl.com"

#define WEB_PORT           "443"

/* HTTP request */
static const char *REQUEST = "GET "CONFIG_RESOURCE" HTTP/1.0\r\n"
                             "Host: "CONFIG_WEBSITE"\r\n"
                             "User-Agent: SAMA5D2\r\n"
                             "\r\n";

#define SERVER_IP_ADDRESS "104.196.190.195"
#define SERVER_PORT_NUM   443

#define MAX_SSL_BUF 2000
static uint8_t ssl_recv_buffer[MAX_SSL_BUF];

static struct sockaddr_in ra;
static WOLFSSL_CTX* ctx;
static WOLFSSL* ssl;
static WOLFSSL_METHOD* method;

int httpsexample(void) {

  /* create a new socket */
  int s = socket(AF_INET, SOCK_STREAM, 0);

  if(s < 0) {
    chprintf((BaseSequentialStream *)&SD1, "Unable to allocate a new socket\n\r");
    return 0;
  }

  chprintf((BaseSequentialStream *)&SD1, "Socket allocated, id=%d\n\r", s);

  memset(&ra, 0, sizeof(struct sockaddr_in));
  ra.sin_len = sizeof(ra);
  ra.sin_family = AF_INET;
  ra.sin_port = htons(SERVER_PORT_NUM);
  ra.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);

  /* connect to the specified server */
  int result = connect(s, (struct sockaddr *)&ra,
                   sizeof (struct sockaddr_in));
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
#if 0
  e = wolfSSL_CTX_load_verify_buffer(ctx, client_cert, client_cert_len, SSL_FILETYPE_ASN1);
  if (e != SSL_SUCCESS) {
    chprintf((BaseSequentialStream *)&SD1, "Error loading client certs\n\r");
    close(s);
    return 0;
  }
#endif
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
  chprintf((BaseSequentialStream *)&SD1, "HTTP request sent\n\r");

  /* print the response */
  chprintf((BaseSequentialStream *)&SD1, "HTTP response:\n\r");
  chprintf((BaseSequentialStream *)&SD1,"--------------------------------------------------------------------------------\n\r");
  int r;
  do {
    memset(ssl_recv_buffer, 0, MAX_SSL_BUF);
    r = wolfSSL_read(ssl, ssl_recv_buffer, MAX_SSL_BUF - 1);
    chprintf((BaseSequentialStream *)&SD1,"%s\n\r",ssl_recv_buffer);
  } while(r > 0);
  chprintf((BaseSequentialStream *)&SD1, "--------------------------------------------------------------------------------\n\r");

  /* frees all data before client termination */
  wolfSSL_free(ssl);
  wolfSSL_CTX_free(ctx);
  wolfSSL_Cleanup();

//  freeaddrinfo(res);
  close(s);
  chprintf((BaseSequentialStream *)&SD1, "Socket closed\n\r");

  return 1;
}

/*
 * One thread that run the https example.
 */
static THD_WORKING_AREA(waThreadHttps, 2048);
static THD_FUNCTION(ThreadHttps, arg) {
  (void)arg;

  tsWaitStubSkelReady();
  chThdSleepMilliseconds(5000);
  while (true) {
    httpsexample();
    chThdSleepMilliseconds(500);
  }
}

/*
 * LED blinker thread, times are in milliseconds.
 */
static THD_WORKING_AREA(waThread1, 512);
static THD_FUNCTION(Thread1, arg) {

  (void)arg;
  chRegSetThreadName("blinker");

  while (true) {
    palToggleLine(LINE_LED_BLUE);
    chThdSleepMilliseconds(80);
    palToggleLine(LINE_LED_BLUE);
    chThdSleepMilliseconds(120);
    palToggleLine(LINE_LED_BLUE);
    chThdSleepMilliseconds(120);
    palToggleLine(LINE_LED_BLUE);
    chThdSleepMilliseconds(120);
    palToggleLine(LINE_LED_BLUE);
    chThdSleepMilliseconds(160);
    palToggleLine(LINE_LED_BLUE);
    chThdSleepMilliseconds(600);
  }
}

static const SerialConfig sdcfg = {
  115200,
  0,
  UART_MR_PAR_NO
};

/*
 * Application entry point.
 */
int main(void) {

  /*
   * System initializations.
   * - HAL initialization, this also initializes the configured device drivers
   *   and performs the board-specific initializations.
   * - Kernel initialization, the main() function becomes a thread and the
   *   RTOS is active.
   *   The foreign interrupts are disabled up to the trampoline in the non secure world
   */
  halInit();
  chSysInit();

  /*
   * Activates the serial driver 0 using the driver default configuration.
   */
  sdStart(&SD1, &sdcfg);

  /*
   * Creates the blinker thread (and any other ancillary thread).
   */
  chThdCreateStatic(waThread1, sizeof waThread1, NORMALPRIO, Thread1, NULL);
  chThdCreateStatic(waThreadHttps, sizeof waThreadHttps, NORMALPRIO+1, ThreadHttps, NULL);

  /*
   * System initializations.
   * - TSSI initialization, the trusted services are created and started.
   *   Lastly, the thread "main" becomes the non secure execution environment.
   */
  tssiInit();
  /*
   * It never goes here
   */
}
