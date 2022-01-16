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

#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "sbuser.h"

#define SHELL_MAX_LINE_LENGTH       128
#define SHELL_MAX_ARGUMENTS         20
#define SHELL_PROMPT_STR            "> "
#define SHELL_NEWLINE_STR           "\r\n"

static void shell_write(const char *s) {
  size_t n = strlen(s);

  (void) write(STDOUT_FILENO, s, n);
}

static void shell_error(const char *s) {
  size_t n = strlen(s);

  (void) write(STDERR_FILENO, s, n);
}

static void shell_putc(char c) {

  (void) write(STDOUT_FILENO, &c, 1);
}

bool shell_getline(char *line, size_t size) {
  char *p = line;

  while (true) {
    char c;

    if (read(STDIN_FILENO, &c, 1) == 0)
      return true;

    if (c == 4) {
      shell_putc('^');
      shell_putc('D');
      return true;
    }

    if ((c == 8) || (c == 127)) {
      if (p != line) {
        shell_putc(8);
        shell_putc(' ');
        shell_putc(8);
        p--;
      }
      continue;
    }
    if (c == '\r') {
      shell_write(SHELL_NEWLINE_STR);
      *p = 0;
      return false;
    }

    if (c < 0x20)
      continue;

    if (p < line + size - 1) {
      shell_putc(c);
      *p++ = c;
    }
  }
}

static char *fetch_argument(char *p) {
  char *dp;

  /* Skipping white space.*/
  p += strspn(p, " \t");

  if (*p == '\0') {
    dp = p;
    p = NULL;
  }
  else if (*p == '"') {
    /* If an argument starts with a double quote then its delimiter is another
       quote.*/
    p++;
    dp = strpbrk(p++, "\"");
  }
  else {
    /* The delimiter is white space.*/
    dp = strpbrk(p++, " \t");
  }

  /* Replacing the delimiter with a zero.*/
  *dp = '\0';

  return p;
}

/*
 * Application entry point.
 */
int main(int argc, char *argv[], char *envp[]) {
#if 0
  char *s;
  int i = 1;

  printf("argc: %d\r\n", argc);
  printf("argv: ");
  while ((s = *argv++) != NULL) {
    printf("%s", s);
  }
  printf("\r\n");
  printf("envp: ");
  while ((s = *envp++) != NULL) {
    printf("%s", s);
  }
  printf("\r\n");

  while (i <= 10) {
    printf("#1 Hello World (%u)!!\r\n", i++);
    sbSleepMilliseconds(500);
  }

  return i;
#endif
  char line[SHELL_MAX_LINE_LENGTH];
  char *args[SHELL_MAX_ARGUMENTS + 1];
  char *p;
  int i, j;

  while (true) {
    /* Prompt.*/
    shell_write(SHELL_PROMPT_STR);

    /* Reading input line.*/
    if (shell_getline(line, SHELL_MAX_LINE_LENGTH)) {
      shell_write(SHELL_NEWLINE_STR);
      shell_write("logout");
      break;
    }

    /* Parsing arguments.*/
    p = line;
    i = 0;
    while ((p = fetch_argument(p)) != NULL) {
      if (i < SHELL_MAX_ARGUMENTS) {
        args[i++] = p;
      }
      else {
        i = 0;
        shell_error("too many argu" SHELL_NEWLINE_STR);
        break;
      }
    }
    args[i] = NULL;

    j = 0;
    while ((p = args[j++]) != NULL) {
      shell_write(p);
      shell_write(SHELL_NEWLINE_STR);
    }
  }
}
