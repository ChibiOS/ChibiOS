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

#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "sbuser.h"
#include "paths.h"

#include "dirent.h"
#include "sglob.h"

#define SHELL_HISTORY_DEPTH         8
#define SHELL_MAX_LINE_LENGTH       128
#define SHELL_MAX_ARGUMENTS         20
#define SHELL_PROMPT_STR            "> "
#define SHELL_NEWLINE_STR           "\r\n"
#define SHELL_WELCOME_STR           "ChibiOS/SB Mini Shell"
#define SHELL_DEFAULT_PATH          "/bin"
#define SHELL_EXECUTABLE_EXTENSION  ".elf"

#define CTRL(c) (char)((c) - 0x40)

static struct {
  const char        *prompt;
  char              *history_head;
  char              *history_current;
  char              history_buffer[SHELL_HISTORY_DEPTH][SHELL_MAX_LINE_LENGTH];
  char              pathbuf[1024];
} state;

static void shell_putc(char c) {

  (void) write(STDOUT_FILENO, &c, 1);
}

static void shell_write(const char *s) {
  size_t n = strlen(s);

  (void) write(STDOUT_FILENO, s, n);
}

static void shell_writeln(const char *s) {
  size_t n = strlen(s);

  (void) write(STDOUT_FILENO, s, n);
  (void) write(STDOUT_FILENO, SHELL_NEWLINE_STR, 2);
}

static void shell_error(const char *s) {
  size_t n = strlen(s);

  (void) write(STDERR_FILENO, s, n);
}

static void shell_errorln(const char *s) {
  size_t n = strlen(s);

  (void) write(STDERR_FILENO, s, n);
  (void) write(STDERR_FILENO, SHELL_NEWLINE_STR, 2);
}

static void shell_usage(const char *s) {

  shell_error("usage: ");
  shell_errorln(s);
}

static void shell_save_history(char *line) {

  strcpy(state.history_head, line);
  state.history_head += SHELL_MAX_LINE_LENGTH;
  if (state.history_head >= state.history_buffer[SHELL_HISTORY_DEPTH]) {
    state.history_head = state.history_buffer[0];
  }
}

static size_t shell_get_history_prev(char *line) {
  size_t len;
  char *p;

  p = state.history_current - SHELL_MAX_LINE_LENGTH;
    if (p < state.history_buffer[0]) {
      p = state.history_buffer[SHELL_HISTORY_DEPTH - 1];
  }
  if ((len = strlen(p)) > (size_t)0) {
    state.history_current = p;
  }
  strcpy(line, p);

  return len;
}

static size_t shell_get_history_next(char *line) {
  size_t len;
  char *p;

  p = state.history_current + SHELL_MAX_LINE_LENGTH;
    if (p > state.history_buffer[SHELL_HISTORY_DEPTH - 1]) {
      p = state.history_buffer[0];
  }
  if ((len = strlen(p)) > (size_t)0) {
    state.history_current = p;
  }
  strcpy(line, p);

  return len;
}

static void shell_reset_line(void) {

  shell_write("\r");
  shell_write(state.prompt);
  shell_write("\033[K");
}

static bool shell_getline(char *line, size_t size) {
  char *p = line;
  int seq;

  state.history_current = state.history_head;
  seq = 0;;
  while (true) {
    char c;

    if (read(STDIN_FILENO, &c, 1) == 0)
      return true;

    /* Escape sequences decoding.*/
    switch (seq) {
    case 0:
      if (c == 27) {
        seq = 1;
        continue;
      }
      break;
    case 1:
      if (c == '[') {
        seq = 2;
        continue;
      }
      if (c == 27) {
        /* ESC+ESC clears the line.*/
        continue;
      }
      seq = 0;
      break;
    case 2:
      seq = 0;
      if (c == 'A') {
        /* Cursor up.*/
        size_t len = shell_get_history_prev(line);

        if (len > (size_t)0) {
          shell_reset_line();
          shell_write(line);
          p = line + len;
        }
        continue;
      }
      if (c == 'B') {
        /* Cursor down.*/
        size_t len = shell_get_history_next(line);

        if (len > (size_t)0) {
          shell_reset_line();
          shell_write(line);
          p = line + len;
        }
        continue;
      }
    }

    if ((c == CTRL('D')) && (p == line)) {
      return true;
    }

    if ((c == CTRL('H')) || (c == 127)) {
      if (p != line) {
        shell_write("\010 \010");
        p--;
      }
      continue;
    }
    if (c == '\r') {
      shell_write(SHELL_NEWLINE_STR);
      *p = 0;
      if (strlen(line) != 0) {
        shell_save_history(line);
      }
      return false;
    }

    if (c < ' ')
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
    shell_errorln(": No such file or directory");
  }
}

static void cmd_pwd(int argc, char *argv[]) {
  char *p;

  (void)argv;

  if (argc != 1) {
    shell_usage("pwd");
    return;
  }

  p = getcwd(state.pathbuf, sizeof state.pathbuf);
  if (p != NULL) {
    shell_writeln(state.pathbuf);
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
    shell_writeln(*pp++);
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

#if 0
static void cmd_dir(int argc, char *argv[]) {
  DIR *dirp;
  struct dirent *dep;

  if (argc > 2) {
    shell_usage("dir [path]");
    return;
  }

  if (argc == 1) {
    dirp = opendir(".");
  }
  else {
    dirp = opendir(argv[1]);
  }
  if (dirp == NULL) {
    shell_error(argv[1]);
    shell_errorln(": No such file or directory");
    return;
  }

  while ((dep = readdir(dirp)) != NULL) {
    shell_write(dep->d_name);
    shell_write(" ");
  }
  shell_write(SHELL_NEWLINE_STR);

  closedir(dirp);
}
#endif

static void cmd_mkdir(int argc, char *argv[]) {
  int ret;

  if (argc != 2) {
    shell_usage("mkdir <dirname>");
    return;
  }

  ret = mkdir(argv[1], 0777);
  if (ret == -1) {
    shell_error(argv[1]);
    shell_errorln(": Creation failed");
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
    shell_errorln(": No such file or directory");
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
    shell_writeln(s);
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
    shell_errorln(": No such file or directory");
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
    shell_errorln(": Remove failed");
  }
}

static void cmd_help(int argc, char *argv[]);

static const struct builtins {
  const char *name;
  void (*cmdf)(int argc, char *argv[]);
} builtins[] = {
  {"cd",      cmd_cd},
//  {"dir",     cmd_dir},
  {"echo",    cmd_echo},
  {"env",     cmd_env},
  {"exit",    cmd_exit},
  {"help",    cmd_help},
  {"mkdir",   cmd_mkdir},
  {"mv",      cmd_mv},
  {"path",    cmd_path},
  {"pwd",     cmd_pwd},
  {"rm",      cmd_rm},
  {"rmdir",   cmd_rmdir},
  {NULL,      NULL}
};

static void cmd_help(int argc, char *argv[]) {
  const struct builtins *bip = builtins;

  (void)argv;

  if (argc != 1) {
    shell_usage("help");
    return;
  }

  while (bip->name != NULL) {
    shell_write(bip->name);
    shell_write(" ");
    bip++;
  }
  shell_write(SHELL_NEWLINE_STR);
}

static bool shell_execute(int argc, char *argv[]) {
  extern int runelf(int argc, char *argv[], char *envp[]);
  char *fname = argv[0];
  int i, ret;

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
      if (n >= sizeof state.pathbuf) {
        errno = ERANGE;
        break;
      }

      /* Non absolute paths are ignored.*/
      if (*p == '/') {
        /* Building combined path, error on path buffer overflow.*/
        memmove(state.pathbuf, p, n);
        state.pathbuf[n] = '\0';
        if (path_append(state.pathbuf, fname, sizeof state.pathbuf) == 0U) {
          errno = ERANGE;
          break;
        }

        /* Enforcing an executable file extension, there is no eXecute
           attribute to handle.*/
        if (path_add_extension(state.pathbuf,
                               SHELL_EXECUTABLE_EXTENSION,
                               sizeof state.pathbuf) == 0U) {
          errno = ERANGE;
          break;
        }

        /* Trying to execute from, this path.*/
        argv[0] = state.pathbuf;
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

  state.prompt = getenv("PROMPT");
  if (state.prompt == NULL) {
    state.prompt = SHELL_PROMPT_STR;
  }

  /* Welcome.*/
  shell_writeln(SHELL_NEWLINE_STR SHELL_WELCOME_STR);

  state.history_head = state.history_buffer[0];
  while (true) {
    int i, n, ret;
    char line[SHELL_MAX_LINE_LENGTH];
    char *args[SHELL_MAX_ARGUMENTS + 1];
    char *ap, *tokp;
    sglob_t sglob;

    /* Prompt.*/
    shell_write(state.prompt);

    /* Reading input line.*/
    if (shell_getline(line, SHELL_MAX_LINE_LENGTH)) {
      shell_writeln("exit");
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
        shell_errorln("too many arguments");
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
      shell_errorln(": command not found");
    }

    /* Freeing memory allocated during processing.*/
    sglob_free(&sglob);
    continue;

outofmem:
    shell_errorln("msh: out of memory");
    sglob_free(&sglob);
  }
}
