#include <ncurses.h>
#include "movement.h"

// if the displayed coords uses zero indexing or not
#define ZERO_INDEX_LOCATOR true

void b_move(int y, int x) {
	move(0, 20);
	clrtoeol();
	char str[1000];

	if (ZERO_INDEX_LOCATOR) {
		sprintf(str, "r:%d, c:%d", y-1, x);
	} else {
		sprintf(str, "r:%d, c:%d", y, x+1);
	}
	addstr(str);

	move(y,x);
}

void wb_move(WINDOW &scr, int y, int x) {
	wmove(&scr, 0, 20);
	wclrtoeol(&scr);
	char str[1000];

	if (ZERO_INDEX_LOCATOR) {
		sprintf(str, "r:%d, c:%d", y-1, x);
	} else {
		sprintf(str, "r:%d, c:%d", y, x+1);
	}
	waddstr(&scr, str);

	wmove(&scr, y,x);
}

void inc(int& y, int& x, const int increment, const int maxx) {
	if (increment > 0) {
		if (x + increment < maxx){
			x += increment;
		} else {
			x = (x+increment) % maxx;
			y += 1;
		}
	} else if (increment < 0) {
		if (x + increment >= 0) {
			x += increment;
		} else if (y > 0){
			x = maxx-1;
			y -= 1;
		}
	}
}
