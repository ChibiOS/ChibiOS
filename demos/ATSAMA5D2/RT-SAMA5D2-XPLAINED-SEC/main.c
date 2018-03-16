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
#include "chtssi.h"
#include "proxies/tssockstub.h"
#include "rt_test_root.h"
#include "oslib_test_root.h"
#include "chprintf.h"
#include <string.h>

#define SERVER_PORT_NUM 8080
#define SERVER_IP_ADDRESS "192.168.1.76"

void tcpexample(int port) {
  int socket_fd;
  struct sockaddr_in ra;

  int recv_data; char data_buffer[80];
  struct fd_set rset;
  struct timeval tm = {10, 0};

  /*
   * Creates an TCP socket, i.e. a SOCK_STREAM, with Internet Protocol Family,
   * i.e. PF_INET. Protocol family and Address family are related. For example
   * PF_INET Protocol Family and AF_INET family are coupled.
   */
  socket_fd = socket(PF_INET, SOCK_STREAM, 0);

  if ( socket_fd < 0 ) {
    chprintf((BaseSequentialStream *)&SD1, "socket call failed.\r\n");
    return;
  }

  FD_ZERO(&rset);

  /* Connects to server ip-address.*/
  memset(&ra, 0, sizeof(struct sockaddr_in));
  ra.sin_family = AF_INET;
  ra.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS);
  ra.sin_port = htons(port);


  if (connect(socket_fd, (struct sockaddr *)&ra,
      sizeof (struct sockaddr_in)) < 0) {
    chprintf((BaseSequentialStream *)&SD1, "Connect failed.\r\n");
    close(socket_fd);
    return;
  }
  while (true) {
    chsnprintf(data_buffer, sizeof data_buffer,
        "Sending this message on port %d.\r\n", port);
    if (send(socket_fd, data_buffer, strlen(data_buffer), 0) < 0) {
      chprintf((BaseSequentialStream *)&SD1, "Send failed.\r\n");
      close(socket_fd);
      return;
    }
    do {
      FD_SET(socket_fd, &rset);
      recv_data = select(socket_fd+1, &rset, 0, 0, &tm);
      if (recv_data < 0) {
        chprintf((BaseSequentialStream *)&SD1, "Select failed.\r\n");
        close(socket_fd);
        return;
      }
    } while (recv_data == 0);
    recv_data = recv(socket_fd, data_buffer, sizeof data_buffer, 0);
    if (recv_data < 0) {
      chprintf((BaseSequentialStream *)&SD1, "Recv failed.\r\n");
      close(socket_fd);
      return;
    }
    data_buffer[recv_data] = '\0';
    chprintf((BaseSequentialStream *)&SD1, "Received data: %s.\r\n", data_buffer);
  }
  close(socket_fd);
}

/*
 * Two threads that run the same tcp example on two
 * different ports.
 */
static THD_WORKING_AREA(waThreadTcp1, 512);
static THD_WORKING_AREA(waThreadTcp2, 512);
static THD_FUNCTION(ThreadTcp, arg) {
  int port  = (int)arg;

  tsWaitStubSkelReady();
  chThdSleepMilliseconds(5000);
  while (true) {
    tcpexample(port);
    chThdSleepMilliseconds(250);
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
  chThdCreateStatic(waThreadTcp1, sizeof waThreadTcp1, NORMALPRIO+10, ThreadTcp, (void *)8080);
  chThdCreateStatic(waThreadTcp2, sizeof waThreadTcp2, NORMALPRIO+10, ThreadTcp, (void *)8081);

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
