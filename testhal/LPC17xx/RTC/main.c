/*
    ChibiOS/RT - Copyright (C) 2006-2013 Giovanni Di Sirio

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
This structure is used to hold the values representing a calendar time.
It contains the following members, with the meanings as shown.

int tm_sec       seconds after minute [0-61] (61 allows for 2 leap-seconds)
int tm_min       minutes after hour [0-59]
int tm_hour      hours after midnight [0-23]
int tm_mday      day of the month [1-31]
int tm_mon       month of year [0-11]
int tm_year      current year-1900
int tm_wday      days since Sunday [0-6]
int tm_yday      days since January 1st [0-365]
int tm_isdst     daylight savings indicator (1 = yes, 0 = no, -1 = unknown)
*/

#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "ch.h"
#include "hal.h"

#include "shell.h"
#include "chprintf.h"
#include "chrtclib.h"

static RTCAlarm alarmspec;
static time_t unix_time;

/* libc stub */
int _getpid(void) {return 1;}
/* libc stub */
void _exit(int i) {(void)i;}
/* libc stub */
#include <errno.h>
#undef errno
extern int errno;
int _kill(int pid, int sig) {
  (void)pid;
  (void)sig;
  errno = EINVAL;
  return -1;
}


/* sleep indicator thread */
static WORKING_AREA(blinkWA, 128);
static msg_t blink_thd(void *arg){
  (void)arg;
  while (TRUE) {
    chThdSleepMilliseconds(100);
    palTogglePad(GPIO0, GPIO0_LED2_RED);
  }
  return 0;
}

static void wakeup_cb(RTCDriver *rtcp, rtcevent_t event) {

  (void)rtcp;

  if (event == RTC_EVENT_ALARM) {

  }
}

/* Wake-up from Deep-sleep mode with rtc alarm (must be set first) */
/* Before going sleep disconnect debugger !!! User manual page 758 */
static void func_sleep(void) {

  /* Deep sleep-mode configuration */
  LPC_SC->PCON &= ~((1UL << 1) | (1UL << 0));       /* Clear PM0 and PM1 bit for deep sleep mode.*/
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;                /* Deep sleep mode */

  __WFI();

  NVIC_SystemReset();
}

/* Wake-up from Deep power-down with wake-up pin or rtc alarm (must be set first) */
/* Before going power down disconnect debugger !!! User manual page 758 */
static void func_pwrdown(void) {

  LPC_SC->PCON |= (1UL << 1) | (1UL << 0);          /* Set PM0 and PM1 bit for deep power-down mode.*/
  SCB->SCR |= SCB_SCR_SLEEPDEEP_Msk;                /* Deep sleep mode. */

  __WFI();
}

static void cmd_pwrdown(BaseSequentialStream *chp, int argc, char *argv[]){
  (void)argv;

  if (argc > 0) {
    chprintf(chp, "Usage: pwrdown\r\n");
    return;
  }
  chprintf(chp, "Going to power down.\r\n");

  chThdSleepMilliseconds(200);

  func_pwrdown();

}

static void cmd_sleep(BaseSequentialStream *chp, int argc, char *argv[]){
  (void)argv;
  if (argc > 0) {
    chprintf(chp, "Usage: sleep\r\n");
    return;
  }
  chprintf(chp, "Going to sleep.\r\n");

  chThdSleepMilliseconds(200);

  /* going to anabiosis */
  func_sleep();
}

/*
 *
 */
static void cmd_alarm(BaseSequentialStream *chp, int argc, char *argv[]){
  (void)argv;
  struct tm timp;

  if (argc < 1) {
    goto ERROR;
  }

  if ((argc == 1) && (strcmp(argv[0], "get") == 0)){
    rtcGetAlarm(&RTCD1, 0, &alarmspec);
    timp.tm_sec = alarmspec.alsec;
    timp.tm_min = alarmspec.almin;
    timp.tm_hour = alarmspec.alhour;
    timp.tm_mday = alarmspec.aldom;
    timp.tm_mon = alarmspec.almonth - 1;
    timp.tm_wday = alarmspec.aldow;
    timp.tm_year = alarmspec.alyear - 1900;
    timp.tm_yday = alarmspec.aldoy - 1;
    chprintf(chp, "%D%s",mktime(&timp)," - alarm in seconds\r\n");
    return;
  }

  if ((argc == 2) && (strcmp(argv[0], "set") == 0)){
    unix_time = (uint32_t)atol(argv[1]);
    localtime_r(&unix_time, &timp);
    alarmspec.alsec = timp.tm_sec;
    alarmspec.almin = timp.tm_min;
    alarmspec.alhour = timp.tm_hour;
    alarmspec.aldom = timp.tm_mday;
    alarmspec.almonth = timp.tm_mon + 1;
    alarmspec.aldow = timp.tm_wday;
    alarmspec.alyear = timp.tm_year + 1900;
    alarmspec.aldoy = timp.tm_yday + 1;
    rtcSetAlarm(&RTCD1, 0, &alarmspec);
    rtcSetCallback(&RTCD1, wakeup_cb);
    return;
  }
  else{
    goto ERROR;
  }

ERROR:
  chprintf(chp, "Usage: alarm get\r\n");
  chprintf(chp, "       alarm set N\r\n");
  chprintf(chp, "where N is alarm time in seconds\r\n");
}

/*
 *
 */
static void cmd_date(BaseSequentialStream *chp, int argc, char *argv[]){
  (void)argv;
  struct tm timp;

  if (argc == 0) {
    goto ERROR;
  }

  if ((argc == 1) && (strcmp(argv[0], "get") == 0)){
    unix_time = rtcGetTimeUnixSec(&RTCD1);

    if (unix_time == -1){
      chprintf(chp, "incorrect time in RTC cell\r\n");
    }
    else{
      chprintf(chp, "%D%s",unix_time," - unix time\r\n");
      rtcGetTimeTm(&RTCD1, &timp);
      chprintf(chp, "%s%s",asctime(&timp)," - formatted time string\r\n");
    }
    return;
  }

  if ((argc == 2) && (strcmp(argv[0], "set") == 0)){
    unix_time = atol(argv[1]);
    if (unix_time > 0){
      rtcSetTimeUnixSec(&RTCD1, unix_time);
      return;
    }
    else{
      goto ERROR;
    }
  }
  else{
    goto ERROR;
  }

ERROR:
  chprintf(chp, "Usage: date get\r\n");
  chprintf(chp, "       date set N\r\n");
  chprintf(chp, "where N is time in seconds sins Unix epoch\r\n");
  chprintf(chp, "you can get current N value from unix console by the command\r\n");
  chprintf(chp, "%s", "date +\%s\r\n");
  return;
}

static const ShellCommand commands[] = {
  {"alarm", cmd_alarm},
  {"date",  cmd_date},
  {"sleep", cmd_sleep},
  {"pwrdown", cmd_pwrdown},
  {NULL, NULL}
};

static const ShellConfig shell_cfg1 = {
  (BaseSequentialStream  *)&SD1,
  commands
};

BaseSequentialStream  * chp1 = (BaseSequentialStream  *)&SD1;

/**
 * Main function.
 */
int main(void){

  halInit();
  chSysInit();
  chThdCreateStatic(blinkWA, sizeof(blinkWA), NORMALPRIO, blink_thd, NULL);

  sdStart(&SD1, NULL);        /* Default is 38400-8-N-1.*/

  if (LPC_SC->PCON & (1UL << 11)) {
    chprintf(chp1, "Woke from Deep power-down\r\n");
    LPC_SC->PCON |= (1UL << 11);
  }

  if (LPC_SC->PCON & (1UL << 9)) {
    chprintf(chp1, "Woke from Deep-sleep mode\r\n");
    LPC_SC->PCON |= (1UL << 9);
  }

  /* Shell initialization.*/
  shellInit();
  static WORKING_AREA(waShell, 1024);
  shellCreateStatic(&shell_cfg1, waShell, sizeof(waShell), NORMALPRIO);

  /* wait until user do not want to test wakeup */
  while (TRUE){
    chThdSleepMilliseconds(200);
  }
  return 0;
}


