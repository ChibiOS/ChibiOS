/*
    ChibiOS - Copyright (C) 2006..2022 Giovanni Di Sirio

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
#include "paths.h"

#include "sglob.h"

#define SHELL_MAX_LINE_LENGTH       128
#define SHELL_MAX_ARGUMENTS         20
#define SHELL_PROMPT_STR            "> "
#define SHELL_NEWLINE_STR           "\r\n"
#define SHELL_WELCOME_STR           "ChibiOS/SB Mini Shell"
#define SHELL_DEFAULT_PATH          "/bin"
#define SHELL_EXECUTABLE_EXTENSION  ".elf"

static const char *prompt;
static char pathbuf[1024];

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

static void shell_usage(const char *s) {

  shell_error("usage: ");
  shell_error(s);
  shell_error(SHELL_NEWLINE_STR);
}

bool shell_getline(char *line, size_t size) {
  char *p = line;

  while (true) {
    char c;

    if (read(STDIN_FILENO, &c, 1) == 0)
      return true;

    if ((c == 4) && (p == line)) {
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

static void cmd_cd(int argc, char *argv[]) {
  int ret;

  if (argc != 2) {
    shell_usage("cd <path>");
    return;
  }

  ret = chdir(argv[1]);
  if (ret == -1) {
    shell_error(argv[1]);
    shell_error(": No such file or directory" SHELL_NEWLINE_STR);
  }
}

static void cmd_pwd(int argc, char *argv[]) {
  char *p;

  (void)argv;

  if (argc != 1) {
    shell_usage("pwd");
    return;
  }

  p = getcwd(pathbuf, sizeof pathbuf);
  if (p != NULL) {
    shell_write(pathbuf);
    shell_write(SHELL_NEWLINE_STR);
  }
}

static void cmd_echo(int argc, char *argv[]) {
  int i;

  for (i = 1; i < argc; i++) {
    shell_write(argv[i]);
    shell_write(" ");
  }
  shell_write(SHELL_NEWLINE_STR);
}

static void cmd_env(int argc, char *argv[]) {
  extern char **environ;
  char **pp;

  (void)argv;

  if (argc != 1) {
    shell_usage("env");
    return;
  }

  pp = environ;
  while (*pp != NULL) {
    shell_write(*pp++);
    shell_write(SHELL_NEWLINE_STR);
  }
}

static void cmd_exit(int argc, char *argv[]) {
  msg_t msg;

  if (argc == 1) {
    msg = 0;
  }
  else if (argc == 2) {
    msg = atoi(argv[1]);
  }
  else {
    shell_usage("exit [n]");
    return;
  }

  _exit(msg);
}

static void cmd_mkdir(int argc, char *argv[]) {
  int ret;

  if (argc != 2) {
    shell_usage("mkdir <dirname>");
    return;
  }

  ret = mkdir(argv[1], 0777);
  if (ret == -1) {
    shell_error(argv[1]);
    shell_error(": Creation failed" SHELL_NEWLINE_STR);
  }
}

static void cmd_mv(int argc, char *argv[]) {
  int ret;

  if (argc != 3) {
    shell_usage("mv <old> <new>");
    return;
  }

  ret = rename(argv[1], argv[2]);
  if (ret == -1) {
    shell_error(argv[1]);
    shell_error(": No such file or directory" SHELL_NEWLINE_STR);
  }
}

static void cmd_path(int argc, char *argv[]) {
  char *s;

  (void)argv;

  if (argc != 1) {
    shell_usage("path");
    return;
  }

  s = getenv("PATH");
  if (s != NULL) {
    shell_write(s);
    shell_write(SHELL_NEWLINE_STR);
  }
}

static void cmd_rm(int argc, char *argv[]) {
  int ret;

  if (argc != 2) {
    shell_usage("rm <filename>");
    return;
  }

  ret = unlink(argv[1]);
  if (ret == -1) {
    shell_error(argv[1]);
    shell_error(": No such file or directory" SHELL_NEWLINE_STR);
  }
}

static void cmd_rmdir(int argc, char *argv[]) {
  int ret;

  if (argc != 2) {
    shell_usage("rmdir <dirname>");
    return;
  }

  ret = rmdir(argv[1]);
  if (ret == -1) {
    shell_error(argv[1]);
    shell_error(": Remove failed" SHELL_NEWLINE_STR);
  }
}

static bool shell_execute(int argc, char *argv[]) {
  extern int runelf(int argc, char *argv[], char *envp[]);
  char *fname = argv[0];
  int i, ret;

  static const struct {
    const char *name;
    void (*cmdf)(int argc, char *argv[]);
  } builtins[] = {
    {"cd",      cmd_cd},
    {"echo",    cmd_echo},
    {"env",     cmd_env},
    {"exit",    cmd_exit},
    {"mkdir",   cmd_mkdir},
    {"mv",      cmd_mv},
    {"path",    cmd_path},
    {"pwd",     cmd_pwd},
    {"rm",      cmd_rm},
    {"rmdir",   cmd_rmdir},
    {NULL,      NULL}
  };

  i = 0;
  while (builtins[i].name != NULL) {
    if (strcmp(builtins[i].name, fname) == 0) {
      builtins[i].cmdf(argc, argv);
      return false;
    }
    i++;
  }

  if (index(fname, '/') != NULL) {
    /* It is a path, executing as-is without scanning the PATH variable.*/
     ret = runelf(argc, argv, environ);
    if (ret != -1) {
      return false;
    }
  }
  else {
    char *p;

    p = getenv("PATH");
    if (p == NULL) {
      p = SHELL_DEFAULT_PATH;
    }

    /* Searching for executable.*/
    while (true) {
      size_t n;

      /* Getting next path, exit if there is an empty entry.*/
      n = strcspn(p, ":");
      if (n == 0U) {
        errno = ENOENT;
        break;
      }

      /* Error if the path is too long.*/
      if (n >= sizeof pathbuf) {
        errno = ERANGE;
        break;
      }

      /* Non absolute paths are ignored.*/
      if (*p == '/') {
        /* Building combined path, error on path buffer overflow.*/
        memmove(pathbuf, p, n);
        pathbuf[n] = '\0';
        if (path_append(pathbuf, fname, sizeof pathbuf) == 0U) {
          errno = ERANGE;
          break;
        }

        /* Enforcing an executable file extension, there is no eXecute
           attribute to handle.*/
        if (path_add_extension(pathbuf,
                               SHELL_EXECUTABLE_EXTENSION,
                               sizeof pathbuf) == 0U) {
          errno = ERANGE;
          break;
        }

        /* Trying to execute from, this path.*/
        argv[0] = pathbuf;
        ret = runelf(argc, argv, environ);
        argv[0] = fname;
        if (ret != -1) {
          return false;
        }
        if (errno != ENOENT) {
          break;
        }
      }

      /* On the next path, if any.*/
      p += n;
      if (*p == '\0') {
        errno = ENOENT;
        break;
      }
      p++;
    }
  }

  return true;
}

/*
 * Application entry point.
 */
int main(int argc, char *argv[], char *envp[]) {

#if 0
  /* Enable for RAM debug.*/
  asm volatile ("bkpt");
#endif

  (void)argc;
  (void)argv;
  (void)envp;

  prompt = getenv("PROMPT");
  if (prompt == NULL) {
    prompt = SHELL_PROMPT_STR;
  }

  /* Welcome.*/
  shell_write(SHELL_NEWLINE_STR SHELL_WELCOME_STR SHELL_NEWLINE_STR);

  while (true) {
    int i, n, ret;
    char line[SHELL_MAX_LINE_LENGTH];
    char *args[SHELL_MAX_ARGUMENTS + 1];
    char *ap, *tokp;
    sglob_t sglob;

    /* Prompt.*/
    shell_write(prompt);

    /* Reading input line.*/
    if (shell_getline(line, SHELL_MAX_LINE_LENGTH)) {
      shell_write("exit" SHELL_NEWLINE_STR);
      break;
    }

    /* Fetching arguments.*/
    tokp = line;
    n = 0;
    while ((ap = fetch_argument(&tokp)) != NULL) {
      if (n < SHELL_MAX_ARGUMENTS) {
        args[n++] = ap;
      }
      else {
        n = 0;
        shell_error("too many arguments" SHELL_NEWLINE_STR);
        break;
      }
    }
    args[n] = NULL;

    /* Special case for empty lines.*/
    if (n == 0) {
      continue;
    }

    /* Adding the command name as-is.*/
    sglob_init(&sglob);
    ret = sglob_add(&sglob, args[0]);
    if (ret == SGLOB_NOSPACE) {
      goto outofmem;
    }

    /* Arguments processing except those starting with a "-" which are
       options, those are added without processing.*/
    for (i = 1; i < n; i++) {
      /* Accumulating arguments expansion into the "glob".*/
      if (*args[i] != '-') {
        ret = sglob_match(&sglob, args[i]);
      }
      else {
        ret = sglob_add(&sglob, args[i]);
      }
      if (ret == SGLOB_NOSPACE) {
        goto outofmem;
      }
    }

    /* Building the full arguments array.*/
    ret = sglob_build(&sglob, 0);
    if (ret == SGLOB_NOSPACE) {
      goto outofmem;
    }

    /* Executing the glob.*/
    if (shell_execute(sglob.sgl_pathc, sglob.sgl_pathv)) {
      shell_error("msh: ");
      shell_error(args[0]);
      shell_error(": command not found" SHELL_NEWLINE_STR);
    }

    /* Freeing memory allocated during processing.*/
    sglob_free(&sglob);
    continue;

outofmem:
    shell_error("msh: out of memory" SHELL_NEWLINE_STR);
    sglob_free(&sglob);
  }
}
