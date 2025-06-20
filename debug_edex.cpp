#include "debug_edex.h"
#include "movement.h"
#include <ncurses.h>


void debug_msg(int y, int x, const char* dbmsg, WINDOW* screen){
	int rows = getmaxy(screen);

	move(rows-1, 0);
	clrtoeol();

	addstr(dbmsg);

	b_move(y, x);
}

void debug_clear(int y, int x, WINDOW* screen) {
	debug_msg(y, x, "", screen);
}
