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
#include <stdlib.h>

#include "sbuser.h"

#define SHELL_MAX_LINE_LENGTH       128
#define SHELL_MAX_ARGUMENTS         20
#define SHELL_PROMPT_STR            "> "
#define SHELL_NEWLINE_STR           "\r\n"
#define SHELL_WELCOME_STR           "ChibiOS/SB shell"

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

static char *fetch_argument(char **pp) {
  char *p, *ap;

  /* Skipping white space.*/
  ap = *pp;
  ap += strspn(ap, " \t");

  if (*ap == '\0') {
    return  NULL;
  }

  if (*ap == '"') {
    /* If an argument starts with a double quote then its delimiter is another
       quote.*/
    ap++;
    p = strpbrk(ap, "\"");
  }
  else {
    /* The delimiter is white space.*/
    p = strpbrk(ap, " \t");
  }

  if (p != NULL) {
    /* Replacing the delimiter with a zero.*/
    *p++ = '\0';
  }
  else {
    /* Final one, pointing on the final zero.*/
    p = ap + strlen(ap);
  }
  *pp = p;

  return ap;
}

static int cmd_exit(int argc, char *argv[]) {

  (void)argc;
  (void)argv;

  sbExit(0);

  return 0;
}

static int shell_execute(int argc, char *argv[]) {
  int i;

  static const struct {
    const char *name;
    int (*cmdf)(int argc, char *argv[]);
  } builtins[] = {
    {"exit",    cmd_exit},
    {NULL,      NULL}
  };

  i = 0;
  while (builtins[i].name != NULL) {
    if (strcmp(builtins[i].name, argv[0]) == 0) {
      return builtins[i].cmdf(argc, argv);
    }
    i++;
  }

  return -1;
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
  char *ap, *tokp;
  int i;

  (void)argc;
  (void)argv;
  (void)envp;

  while (true) {
    /* Welcome.*/
    shell_write(SHELL_WELCOME_STR SHELL_NEWLINE_STR);

    /* Prompt.*/
    shell_write(SHELL_PROMPT_STR);

    /* Reading input line.*/
    if (shell_getline(line, SHELL_MAX_LINE_LENGTH)) {
      shell_write(SHELL_NEWLINE_STR);
      shell_write("logout");
      break;
    }

    /* Parsing arguments.*/
    tokp = line;
    i = 0;
    while ((ap = fetch_argument(&tokp)) != NULL) {
      if (i < SHELL_MAX_ARGUMENTS) {
        args[i++] = ap;
      }
      else {
        i = 0;
        shell_error("too many arguments" SHELL_NEWLINE_STR);
        break;
      }
    }
    args[i] = NULL;

    shell_execute(i, args);
  }
}
