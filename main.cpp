#include <vector>
#include <ncurses.h>
#include <iostream>
#include <string>
#include <locale.h>
#include <span> 

#define TAB_WIDTH 4

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
	vector<char> tab_spaces = {};

	for (int i=0; i<TAB_WIDTH; i++){
		tab_spaces.push_back(32);
	}

	setlocale(LC_ALL, "");
	
	vector<vector<char>> data;

	int data_x = 0;
	int data_y = 0;

	data.push_back({});

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

			data[data_y].insert(data[data_y].begin() + data_x, k);

			data_x++;
			x++; 

			if (saved) {
				unsave(x, y, saved);
			}
		}

		if (k == 9) {
			for (int i = 0; i<TAB_WIDTH; i++) {
				data[data_y].insert(data[data_y].begin() + data_x, 32);
				addch(32);

				data_x++;
				x++;
			}
			move(y,x);
			if (saved) {
				unsave(x, y, saved);
			}
		}

		if (k == KEY_BACKSPACE) {
			if (x > 0) {
				#ifndef __INTELLISENSE__ 
				std::span<char> to_check(data[data_y]);
				#endif

				
			}

			if (saved) {
				unsave(x, y, saved);
			}

		}

		if (k == KEY_ENTER || k == 10 || k == 13) {
			data.insert(data.begin() + data_y + 1, {});

			data_y++;
			y++;

			data_x = 0;
			x = 0;
			move(y, x);
			if (saved) {
				unsave(x,  y, saved);
			}

		}

		if (k == KEY_UP) {
			if (y > 1) {
				y--;
				data_y--;

				if (x >= data[data_y].size()) {
					data_x = data[data_y].size()-1;
					x = data_x;
				}

				move(y,x);
			}
		}

		if (k == KEY_RIGHT){

			if (x+1 < data[data_y].size()) {
				x++;
				data_x++;
				move(y,x);
			}
			
		}

		if (k == KEY_DOWN) {
			if (data_y+1 < data.size()) {
				y++;
				data_y++;
				move(y,x);
			}
		}

		if (k == KEY_LEFT) {
			if (x > 0) {
				data_x--;
				x--;
				move(y,x);
			}
		}
	}

	endwin();
	return 0;
}
