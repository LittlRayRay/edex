#include <vector>
#include <ncurses.h>
#include <locale.h>
#include <math.h>

#include "movement.h"
#include "file_handling.h"
#include "debug_edex.h"

#define TAB_WIDTH 4

using namespace std;

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

	pair<int, int> visibile {0, 0};

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
				save(x, y, saved, data);
			}
		}

		if ((k >= 32 && k <= 126) || k == 9) {
			data[data_y].insert(data[data_y].begin() + data_x, k);
			clrtoeol();
			int track_x = x;
			int track_y = y;
			for (int i = data_x; i < data[data_y].size(); i++) {
				addch(data[data_y][i]);
				if (data[data_y][i] == '\t') {
					inc(track_y, track_x, TAB_WIDTH, maxx);
				} else {
					inc(track_y, track_x, 1, maxx);
				}
				move(track_y, track_x);
			}
			data_x++;
			if (k == 9) {
				inc(y, x, TAB_WIDTH, maxx);
			} else {
				inc(y, x, 1, maxx);
			}
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
			} else if (x >= 0 && data_x > 0){
				data_x--;

				clrtoeol();
				y--;
				if (data[data_y][data_x] == '\t') {
					inc(y, x, -TAB_WIDTH, maxx);
				} else {
					inc(y, x, -1, maxx);
				}
				y++;
				move(y,x);
				delch();

				data[data_y].erase(data[data_y].begin() + data_x);


				for (int i = data_x; i < data[data_y].size(); i++) {
					if (data[data_y][i] == '\t') {
						inc(y, x, TAB_WIDTH, maxx);
						move(y,x);
					} else {
						addch(data[data_y][i]);
					}
				}

				b_move(y,x);
			}

			if (saved) {
				unsave(x, y, saved);
			}

		}

		if (k == KEY_ENTER || k == 10 || k == 13) {
			clrtoeol();

			vector<char> empty;
			data.insert(data.begin() + data_y + 1, empty);
			data_x--;

			for (int i = data_x; i < data[data_y].size(); i++) {
				data[data_y+1].push_back(data[data_y][i]);
			}

			for (int i = data_x; i < data[data_y].size(); i++) {
				data[data_y].pop_back();
			}


			data_y++;
			y++;

			data_x = 0;
			x = 0;
			move(y, x);
			
			for (int i = 0; i < data[data_y].size(); i++) {
				addch(data[data_y][data_x]);
				data_x++;
			}

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

			if (data_x < data[data_y].size()) {
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
				data_y++;

				// how many lines that y was from the original
				int difference = data_x / maxx;

				data_x = min(data_x, (int)data[data_y].size());
				x = data_x % maxx;

				y = y - difference + data_x / maxx + 1;
				b_move(y,x);
			}
		}

		if (k == KEY_LEFT) {
			if (data_x > 0) {
				data_x--;
				if (x > 0) {
					x--;
				} else {
					x = 0;
					y--;
				}
				b_move(y,x);
			}
		}
	}

	endwin();
	return 0;
}
