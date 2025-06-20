#ifndef DEBUG_EDEX
#define DEBUG_EDEX

#include <ncurses.h>

void debug_msg(int y, int x, const char* dbmsg, WINDOW* screen);
void debug_clear(int y, int x, WINDOW* screen);

#endif
