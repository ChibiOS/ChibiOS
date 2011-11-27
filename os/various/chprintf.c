/*
 ChibiOS/RT - Copyright (C) 2006,2007,2008,2009,2010,
 2011 Giovanni Di Sirio.

 This file is part of ChibiOS/RT.

 ChibiOS/RT is free software; you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation; either version 3 of the License, or
 (at your option) any later version.

 ChibiOS/RT is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <stdarg.h>

#include "ch.h"

#define MAX_FILLER 11

static char *ltoa(char *p, long num, unsigned radix) {
  int i;
  char *q;

  q = p + MAX_FILLER;
  do {
    i = (int)(num % radix);
    i += '0';
    if (i > '9')
      i += 'A' - '0' - 10;
    *--q = i;
  } while ((num /= radix) != 0);

  i = (int)(p + MAX_FILLER - q);
  do
    *p++ = *q++;
  while (--i);

  return p;
}

/**
 * @brief   System formatted output function.
 * @details This function implements a minimal @p printf() like functionality
 *          with output on a @p BaseChannel.
 *          The general parameters format is: %[.][width|*][l|L]p.
 *          The following parameter types (p) are supported:
 *          - <b>x</b> hexadecimal integer.
 *          - <b>X</b> hexadecimal long.
 *          - <b>o</b> octal integer.
 *          - <b>O</b> octal long.
 *          - <b>d</b> decimal signed integer.
 *          - <b>D</b> decimal signed long.
 *          - <b>u</b> decimal unsigned integer.
 *          - <b>U</b> decimal unsigned long.
 *          - <b>c</b> character.
 *          - <b>s</b> string.
 *          .
 * @note    Floating point types are not implemented, this function is meant
 *          as a system utility and not a full implementation.
 *
 * @param[in] chp       pointer to a @p BaseChannel implementing object
 * @param[in] fmt       formatting string
 */
void chprintf(BaseChannel *chp, const char *fmt, ...) {
  va_list ap;
  char buf[MAX_FILLER + 1];
  char *p, *s, c, filler;
  int i, n, width;
  bool_t lflag, ljust;
  long l;

  va_start(ap, fmt);
  while (TRUE) {
    c = *fmt++;
    if (c == 0) {
      va_end(ap);
      return;
    }
    if (c != '%') {
      chIOPut(chp, (uint8_t)c);
      continue;
    }
    p = buf;
    s = buf;
    ljust = FALSE;
    if (*fmt == '-') {
      fmt++;
      ljust = TRUE;
    }
    filler = ' ';
    if (*fmt == '.') {
      fmt++;
      filler = '0';
    }
    for (width = 0;;) {
      c = *fmt++;
      if (c >= '0' && c <= '9')
        c -= '0';
      else if (c == '*')
        c = va_arg(ap, int);
      else
        break;
      width = width * 10 + c;
    }
    n = 0;
    if (c == '.') {
      for (;;) {
        c = *fmt++;
        if (c >= '0' && c <= '9')
          c -= '0';
        else if (c == '*')
          c = va_arg(ap, int);
        else
          break;
        n *= 10;
        n += c;
      }
    }
    lflag = FALSE;
    if (c == 'l' || c == 'L') {
      lflag++;
      if (*fmt)
        c = *fmt++;
    }
    switch (c) {
    case 'X':
      lflag = TRUE;
    case 'x':
      c = 16;
      goto oxu;
    case 'U':
      lflag = TRUE;
    case 'u':
      c = 10;
      goto oxu;
    case 'O':
      lflag = TRUE;
    case 'o':
      c = 8;
oxu:
      if (lflag)
        l = va_arg(ap, long);
      else
        l = va_arg(ap, int);
      p = ltoa(p, l, c);
      break;
    case 'D':
      lflag = TRUE;
    case 'd':
      if (lflag)
        l = va_arg(ap, long);
      else
        l = va_arg(ap, int);
      if (l < 0) {
        *p++ = '-';
        l = -l;
      }
      p = ltoa(p, l, 10);
      break;
    case 'c':
      filler = ' ';
      *p++ = va_arg(ap, int);
      break;
    case 's':
      filler = ' ';
      if ((s = va_arg(ap, char *)) == 0)
        s = "(null)";
      if (n == 0)
        n = 32767;
      for (p = s; *p && --n >= 0; p++)
        ;
      break;
    default:
      *p++ = c;
      break;
    }
    i = (int)(p - s);
    if ((width -= i) < 0)
      width = 0;
    if (ljust == FALSE)
      width = -width;
    if (width < 0) {
      if (*s == '-' && filler == '0') {
        chIOPut(chp, (uint8_t)*s++);
        i--;
      }
      do
        chIOPut(chp, (uint8_t)filler);
      while (++width != 0);
    }
    while (--i >= 0)
      chIOPut(chp, (uint8_t)*s++);

    while (width) {
      chIOPut(chp, (uint8_t)filler);
      width--;
    }
  }
}
