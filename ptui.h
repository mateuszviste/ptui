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

#ifndef ptui_h_sentinel
#define ptui_h_sentinel

/* inits the UI subsystem, 0 on success, non-zero otherwise
 * flags may accept following (possibly OR-ed) flags:
 *   PTUI_ENABLE_MOUSE    - enable mouse support if available */
int ptui_init(int flags);

/* returns 1 if terminal has color capability, 0 otherwise
 * this must be called only AFTER ptui_init() */
int ptui_hascolor(void);

void ptui_close(void);

/* returns the number of rows of current text mode */
int ptui_getrowcount(void);

/* returns the number of columns of current text mode */
int ptui_getcolcount(void);

/* clear the screen */
void ptui_cls(void);

/* print a string on screen, and go to next line */
void ptui_puts(const char *str);

/* Set the position (zero-based) of the cursor on screen */
void ptui_locate(int x, int y);

/* Put a char directly on screen, without playing with the cursor. Coordinates are zero-based.
 * This may be a "wide" (unicode) codepoint on platforms that support unicode. */
void ptui_putchar(int c, int attr, int x, int y);

/* waits for a key to be pressed and returns it. ALT+keys have 0x100 added to
 * them. this may also report a "PTUI_MOUSE" key in case of a mouse click,
 * in such case call ptui_getmouse() to fetch the details about last click) */
int ptui_getkey(void);

/* returns 0 if no key is awaiting in the keyboard buffer, non-zero otherwise */
int ptui_kbhit(void);

/* if mouse support is enabled, set mouse cursor to be:
 * status == 0    -> hidden
 * statis != 0    -> not hidden (default)
 * NOTE: this works only on the MSDOS platform */
void ptui_mouseshow(int status);

/* fetches the coordinates of the last mouse click. returns -1 if nothing is
 * pending. */
int ptui_getmouse(unsigned int *x, unsigned int *y);

/* makes the cursor visible */
void ptui_cursor_show(void);

/* hides the cursor */
void ptui_cursor_hide(void);

/* tell the UI library to render the screen (ignored on platforms that perform immediate rendering) */
void ptui_refresh(void);


/* some public definitions used by PTUI */

#define PTUI_ENABLE_MOUSE 1 /* may be passed to ptui_init() */
#define PTUI_MOUSE 0x200   /* returned by ptui_getkey() to advertise a mouse event */

#endif
