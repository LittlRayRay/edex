#include <vector>
#include <ncurses.h>
#include <iostream>
#include <string>
#include <locale.h>
#include <math.h>

#define TAB_WIDTH 4
#define ZERO_INDEX_LOCATOR true 

using namespace std;

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

void save(int x, int y, bool& saved) {
	move(0, 0);
	clrtoeol();

	addstr("saved");

	move(0, 0);
	chgat(-1, A_NORMAL, 0, NULL);
	b_move(y, x);

	refresh();
	saved = true;
}

void unsave(int x, int y, bool& saved) {
	move(0, 0);
	clrtoeol();

	addstr("unsaved");

	move(0, 0);
	chgat(-1, A_BLINK | A_ITALIC, 0, NULL);
	b_move(y,x);

	refresh();
	saved = false;
}

bool check_all_spaces(vector<vector<char>>& data, int& data_y, int& data_x) {
	bool spaces = true;

	for (int i = 0; i < data_x; i++) {
		if (data[data_y][i] != 32) {
			spaces = false;
			break;
		}
	}

	return spaces;
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
	int maxy, maxx;
		
	while (!close) {
		int k = getch();

		getmaxyx(screen, maxy, maxx);
		getyx(screen, y, x);	

		debug_clear(y,x,screen);

		if (k == KEY_F(1)){
			close = true;
		}

		if (k == KEY_F(2)) {
			if (!saved) {
				save(x, y, saved);
			}
		}

		if ((k >= 32 && k <= 126) || k == 9) {
			if (k == 9) {
				k = '\t';
				for (int i = 0; i < TAB_WIDTH; i++){
					if (x < maxx-1) {
						x++;
					} else {
						y++;
						x = 0; 
					}
				}
			} else {
				if (x < maxx-1) {
					x++;
				} else {
					y++;
					x = 0;
				}
			}
			data[data_y].insert(data[data_y].begin() + data_x, k);
			clrtoeol();
			int track_x = x;
			int track_y = y;
			for (int i = data_x; i < data[data_y].size(); i++) {
				if (data[data_y][i] == '\t') {
					if (track_x + TAB_WIDTH < maxx) {
						track_x += TAB_WIDTH;
					} else {
						track_x = (TAB_WIDTH + track_x) % maxx;
						track_y += 1;
					}
				} else {
					addch(data[data_y][i]);
					if (track_x < maxx-1) {
						track_x += 1;
					} else {
						track_x = 0;
						track_y += 1;
					}
				}
				move(track_y, track_x);
			}
			data_x++;
			b_move(y, x);
			if (saved) {
				unsave(x, y, saved);
			}
		}

		if (k == KEY_BACKSPACE) {

			// 3 cases; 
			// x = 0, data_x  = 0;
			// x = 0, data_x > 0;
			// x > 0, data_x > 0;
			
			if (x == 0 && data_x == 0 && data_y > 0) {
				data.erase(data.begin() + data_y); 	
				data_y--;
				y--;
				x = data[data_y].size();
				data_x = x;

				b_move(y,x);
			} else if (x == 0 && data_x > 0){
				data_x--;
				
				if (data[data_y][data_x] == '\t') {
					for (int i = 0; i<TAB_WIDTH; i++) {
						if ( x > 0) {
							x--;
						} else {
							x=maxx-1;
							y--;
						}
					}
				} else {
					y--;
					x=maxx-1;
				}

				b_move(y,x);
				delch();
				data[data_y].erase(data[data_y].begin() + data_x);
			} else if (x > 0 && data_x > 0){
				data_x--;
				if (data[data_y][data_x] == '\t') {
					for (int i = 0; i<TAB_WIDTH; i++) {
						if ( x > 0) {
							x--;
						} else {
							x=maxx-1;
							y--;
						}
					}
				} else {
					x--;
				}
				
				b_move(y,x);
				delch();
				data[data_y].erase(data[data_y].begin() + data_x);
			}

			if (saved) {
				unsave(x, y, saved);
			}

		}

		if (k == KEY_ENTER || k == 10 || k == 13) {
			vector<char> empty;
			data.insert(data.begin() + data_y + 1, empty);

			data_y++;
			y++;

			data_x = 0;
			x = 0;
			b_move(y, x);
			if (saved) {
				unsave(x,  y, saved);
			}
		}

		if (k == KEY_UP) {
			if (y > 1) {
				y--;
				data_y--;

				if (x >= data[data_y].size()) {
					data_x = data[data_y].size();
					x = data_x;
				}

				b_move(y,x);
			}
		}

		if (k == KEY_RIGHT){

			if (x < data[data_y].size()) {
				if (x+1 < maxx) {
					x++;
				} else {
					x = 0;
					y++;
				}
				data_x++;
				b_move(y,x);
			}
			
		}

		if (k == KEY_DOWN) {
			if (data_y+1 < data.size()) {
				y++;
				data_y++;

				if (x >= data[data_y].size()) {
					data_x = data[data_y].size();
					x = data_x;
				}

				b_move(y,x);
			}
		}

		if (k == KEY_LEFT) {
			if (x > 0) {
				data_x--;
				x--;
				b_move(y,x);
			}
		}
	}

	endwin();
	return 0;
}
