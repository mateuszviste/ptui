/*
 * PTUI stands for "Portable Terminal UI". It is an ANSI C library that
 * provides simple terminal-handling routines that can operate on Linux,
 * Windows and DOS.
 *
 * Copyright (C) 2013-2020 Mateusz Viste
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 */

#include <conio.h>
#include <pc.h>    /* ScreenRows() */

#include "ptui.h"  /* include self for control */


int ptui_hascolor(void) {
  if (ScreenMode() == 7) return(0);
  return(1);
}


/* inits the UI subsystem */
int ptui_init(void) {
  return(0);
}


void ptui_close(void) {
}


int ptui_getrowcount(void) {
 return(ScreenRows());
}


int ptui_getcolcount(void) {
 return(ScreenCols());
}


void ptui_cls(void) {
  clrscr();
}


void ptui_puts(const char *str) {
  cprintf("%s\r\n", str);
}


void ptui_locate(int x, int y) {
  ScreenSetCursor(y, x);
}


void ptui_putchar(unsigned int c, int attr, int x, int y) {
  if (c > 255) c = '.';
  ScreenPutChar(c, attr, x, y);
}


int ptui_getkey(void) {
  return(getkey());
}


int ptui_kbhit(void) {
  return(kbhit());
}


void ptui_cursor_show(void) {
  _setcursortype(_NORMALCURSOR);
}


void ptui_cursor_hide(void) {
  _setcursortype(_NOCURSOR);
}


void ptui_refresh(void) {
  /* nothing here, we draw directly to video memory already */
}
