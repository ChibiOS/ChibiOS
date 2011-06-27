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

#define	MAXDIG		11

static char *itoa(char *p, int num, unsigned radix) {
  int i;
  char *q;

  q = p + MAXDIG;
  do {
    i = (int)(num % radix);
    i += '0';
    if (i > '9')
    i += 'A' - '0' - 10;
    *--q = i;
  } while ((num /= radix) != 0);

  i = (int)(p + MAXDIG - q);
  do
    *p++ = *q++;
  while (--i);

  return p;
}

#ifndef NO_LONGD

static char *ltoa(char *p, long num, unsigned radix) {
  int i;
  char *q;

  q = p + MAXDIG;
  do {
    i = (int)(num % radix);
    i += '0';
    if (i > '9')
    i += 'A' - '0' - 10;
    *--q = i;
  } while ((num /= radix) != 0);

  i = (int)(p + MAXDIG - q);
  do
    *p++ = *q++;
  while (--i);

  return p;
}
#endif

void chprintf(BaseChannel *chp, const char *fmt, ...) {
  va_list ap;
  char buf[MAXDIG + 1];
  char *p, *s;
  int c, i, width, ndigit, ndfnd, ljust, zfill;
#ifndef NO_LONGD
  int lflag;
  long l;
#endif

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
    ljust = 0;
    if (*fmt == '-') {
      fmt++;
      ljust++;
    }
    zfill = ' ';
    if (*fmt == '0') {
      fmt++;
      zfill = '0';
    }
    for (width = 0;;) {
      c = *fmt++;
      if (c >= '0' && c <= '9')
        c -= '0';
      else if (c == '*')
        c = va_arg(ap, int);
      else
        break;
      width *= 10;
      width += c;
    }
    ndfnd = 0;
    ndigit = 0;
    if (c == '.') {
      for (;;) {
        c = *fmt++;
        if (c >= '0' && c <= '9')
          c -= '0';
        else if (c == '*')
          c = va_arg(ap, int);
        else
          break;
        ndigit *= 10;
        ndigit += c;
        ndfnd++;
      }
    }
#ifndef NO_LONGD
    lflag = 0;
#endif
    if (c == 'l' || c == 'L') {
#ifndef NO_LONGD
      lflag++;
#endif
      if (*fmt)
        c = *fmt++;
    }
    switch (c) {
    case 'X':
#ifndef NO_LONGD
      lflag++;
#endif
    case 'x':
      c = 16;
      goto oxu;
    case 'U':
#ifndef NO_LONGD
      lflag++;
#endif
    case 'u':
      c = 10;
      goto oxu;
    case 'O':
#ifndef NO_LONGD
      lflag++;
#endif
    case 'o':
      c = 8;
      oxu:
#ifndef NO_LONGD
      if (lflag) {
        p = ltoa(p, va_arg(ap, long), c);
        break;
      }
#endif
      p = itoa(p, va_arg(ap, int), c);
      break;
    case 'D':
#ifndef NO_LONGD
      lflag++;
#endif
    case 'd':
#ifndef NO_LONGD
      if (lflag) {
        if ((l = va_arg(ap, long)) < 0) {
          *p++ = '-';
          l = -l;
        }
        p = ltoa(p, l, 10);
        break;
      }
#endif
      if ((i = va_arg(ap, int)) < 0) {
        *p++ = '-';
        i = -i;
      }
      p = itoa(p, i, 10);
      break;
/*    case 'e':
    case 'f':
    case 'g':
      zfill = ' ';
      *p++ = '?';
      break;*/
    case 'c':
      zfill = ' ';
      *p++ = va_arg(ap, int);
      break;
    case 's':
      zfill = ' ';
      if ((s = va_arg(ap, char *)) == 0)
        s = "(null)";
      if (ndigit == 0)
        ndigit = 32767;
      for (p = s; *p && --ndigit >= 0; p++)
        ;
      break;
    default:
      *p++ = c;
      break;
    }
    i = (int)(p - s);
    if ((width -= i) < 0)
      width = 0;
    if (ljust == 0)
      width = -width;
    if (width < 0) {
      if (*s == '-' && zfill == '0') {
        chIOPut(chp, (uint8_t)*s++);
        i--;
      }
      do
        chIOPut(chp, (uint8_t)zfill);
      while (++width != 0);
    }
    while (--i >= 0)
      chIOPut(chp, (uint8_t)*s++);

    while (width) {
      chIOPut(chp, (uint8_t)zfill);
      width--;
    }
  }
}
