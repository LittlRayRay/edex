#include <ncurses.h>
#include <stdio.h>
#include <string>
#include <locale.h>

using namespace std;

void save(int x, int y, bool& saved) {
	move(0, 0);
	clrtoeol();

	addstr("saved");

	move(0, 0);
	chgat(-1, A_NORMAL, 0, NULL);
	move(y, x);

	refresh();
	saved = true;
}

void unsave(int x, int y, bool& saved) {
	move(0, 0);
	clrtoeol();

	addstr("unsaved");

	move(0, 0);
	chgat(-1, A_BLINK | A_ITALIC, 0, NULL);
	move(y,x);

	refresh();
	saved = false;
}

int main(int argc, char** argv) {
	const wchar_t block = U'\u2588';

	setlocale(LC_ALL, "");

	WINDOW* screen = initscr();

	bool close = false;
	bool saved = false;

	noecho();
	cbreak();
	keypad(screen, true);

	unsave(0, 1, saved);
	
	curs_set(1);

	int y, x;
		
	while (!close) {
		int k = getch();
		getyx(screen, y, x);	

		if (k == KEY_F(1)){
			close = true;
		}

		if (k == KEY_F(2)) {
			if (!saved) {
				save(x, y, saved);
			}
		}

		if (k >= 32 && k <= 126) {
			addch(k);
			x++; 
			if (saved) {
				unsave(x, y, saved);
			}
		}

		if (k == KEY_BACKSPACE) {
			if (x > 0) {
				move(y, x-1);
				delch();
			}

			if (saved) {
				unsave(x, y, saved);
			}

		}

		if (k == KEY_ENTER || k == 10 || k == 13) {
			y++;
			x = 0;
			move(y, x);
			if (saved) {
				unsave(x,  y, saved);
			}

		}

	}

	endwin();
	return 0;
}
