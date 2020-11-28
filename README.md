# PTUI

PTUI stands for "Portable Terminal UI". It is an ANSI C library that provides
simple terminal-handling routines that can operate on Linux, Windows and DOS.

The prototype declarations (ptui.h) are common to all platforms, but source
files are not - each platform needs to build and link to the proper module:

ptui-curse.c   ncurses-based backend (Linux and Windows, requires libncurses)
ptui-dj.c      DJGPP driver (DOS, protected mode)
ptui-dos.c     real-time DOS driver (uses direct MDA/VGA hardware calls)

Project's homepage: https://github.com/mateuszviste/ptui


# How to use

Simple example:

    ptui_init();
    ptui_putchar('H', 0x17, 0, 0);
    ptui_putchar('e', 0x17, 1, 0);
    ptui_putchar('l', 0x17, 2, 0);
    ptui_putchar('l', 0x17, 3, 0);
    ptui_putchar('o', 0x17, 4, 0);
    ptui_getkey();  /* wait for a key */
    ptui_close();

Note about colors: each time a character needs to be output to the terminal, a
color attribute must be passed along. A color attribute is a single byte value
that encodes foreground color in its low nibble and background color in its
high nibble. For instance 0x17 would be white text on blue background, and
0x0e translates as yellow text on black background.


# Dependencies

On non-DOS platforms, this library requires ncursesw. One needs to ensure that
his program is linked to ncursesw (possibly tinfo, too, depending on your
distribution and packaging), and make sure that ncursesw prototypes are in the
include path. Typically, this would be done something like that:

    cc hello.c ptui-curse.c -lncursesw -ltinfo -I/usr/include/ncursesw


# Contact

If you'd like to get in contact with this library's author, you will find
instructions how to do so on his personal homepage: http://mateusz.viste.fr


# License

PTUI is published under the MIT license.

Copyright (C) 2013-2020 Mateusz Viste

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
