#ifndef UTIL_H
#define UTIL_H

#include <ncurses.h>

extern bool logStarted;

char *wdynamicGetStr(WINDOW *window);
void wcprintw(WINDOW *window, int cury, int curx, attr_t attr, short colorpair, const char *fmt, ...);

#endif