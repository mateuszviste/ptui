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

#include <dos.h>

#include "ptui.h"  /* include self for control */

static unsigned char far *vmem; /* video memory pointer (beginning of page 0) */
static int term_width = 0, term_height = 0;
static int cursor_start = 0, cursor_end = 0; /* remember the cursor's shape */
static unsigned short videomode = 0;

/* returns 1 if color is supported, 0 otherwise */
int ptui_hascolor(void) {
  if (videomode == 7) return(0); /* MDA/herc mode */
  return(1);
}

/* inits the UI subsystem */
int ptui_init(void) {
  union REGS regs;
  regs.h.ah = 0x0F;  /* get current video mode */
  int86(0x10, &regs, &regs);
  videomode = regs.h.al;
  term_width = regs.h.ah; /* int10,F provides number of columns in AH */
  /* read screen length from BIOS at 0040:0084 */
  term_height = (*(unsigned char far *) MK_FP(0x40, 0x84)) + 1;
  if (term_height < 10) term_height = 25; /* assume 25 rows if weird value */
  /* select the correct VRAM address */
  if (videomode == 7) { /* MDA/HERC mode */
    vmem = MK_FP(0xB000, 0); /* B000:0000 video memory addess */
  } else {
    vmem = MK_FP(0xB800, 0); /* B800:0000 video memory address */
  }
  /* get cursor shape */
  regs.h.ah = 3;
  regs.h.bh = 0;
  int86(0x10, &regs, &regs);
  cursor_start = regs.h.ch;
  cursor_end = regs.h.cl;
  return(0);
}

void ptui_close(void) {
}

static void cursor_set(int startscanline, int endscanline) {
  union REGS regs;
  regs.h.ah = 0x01;
  regs.h.al = videomode; /* RBIL says some BIOSes require video mode in AL */
  regs.h.ch = startscanline;
  regs.h.cl = endscanline;
  int86(0x10, &regs, &regs);
}

int ptui_getrowcount(void) {
  return(term_height);
}


int ptui_getcolcount(void) {
  return(term_width);
}


void ptui_cls(void) {
  union REGS regs;
  regs.x.ax = 0x0600;  /* Scroll window up, entire window */
  regs.h.bh = 0x07;    /* Attribute to write to screen */
  regs.h.bl = 0;
  regs.x.cx = 0x0000;  /* Upper left */
  regs.h.dh = term_height - 1;
  regs.h.dl = term_width - 1; /* Lower right */
  int86(0x10, &regs, &regs);
  ptui_locate(0, 0);
}


void ptui_puts(const char *str) {
  union REGS regs;
  /* display the string one character at a time */
  while (*str != 0) {
    regs.h.ah = 0x02;
    regs.h.dl = *str;
    int86(0x21, &regs, &regs);
    str++;
  }
  /* write a CR/LF pair to screen */
  regs.h.ah = 0x02; /* DOS 1+ - WRITE CHARACTER TO STDOUT */
  regs.h.dl = '\r';
  int86(0x21, &regs, &regs);
  regs.h.ah = 0x02; /* DOS 1+ - WRITE CHARACTER TO STDOUT */
  regs.h.dl = '\n';
  int86(0x21, &regs, &regs);
}


void ptui_locate(int column, int row) {
  union REGS regs;
  regs.h.ah = 0x02;
  regs.h.bh = 0;
  regs.h.dh = row;
  regs.h.dl = column;
  int86(0x10, &regs, &regs);
}


void ptui_putchar(int c, int attr, int x, int y) {
  unsigned char far *p;
  p = vmem + ((y * term_width + x) << 1);
  *p++ = c;
  *p = attr;
}


int ptui_getkey(void) {
  union REGS regs;
  regs.h.ah = 0x08;
  int86(0x21, &regs, &regs);
  if (regs.h.al != 0) return(regs.h.al);
  /* extended key, read again */
  regs.h.ah = 0x08;
  int86(0x21, &regs, &regs);
  return(0x100 | regs.h.al);
}


int ptui_kbhit(void) {
  union REGS regs;
  regs.h.ah = 0x0b; /* DOS 1+ - GET STDIN STATUS */
  int86(0x21, &regs, &regs);
  return(regs.h.al);
}


void ptui_cursor_show(void) {
  if (cursor_start == 0) return;
  cursor_set(cursor_start, cursor_end); /* unhide the cursor */
}


void ptui_cursor_hide(void) {
  cursor_set(0x2F, 0x0E); /* hide the cursor */
  /* the 'start position' of cursor_set() is a bitfield:
   *
   * Bit(s)  Description     (Table 00013)
   *  7      should be zero
   *  6,5    cursor blink (00=normal, 01=invisible)
   *  4-0    topmost scan line containing cursor
   */
}


void ptui_refresh(void) {
  /* nothing here, we draw directly to video memory already */
}
