#include <ncurses.h>
#include <fstream>

#include "movement.h"
#include "file_handling.h"

using namespace std;

void save(int x, int y, bool& saved, vector<vector<char>>& data) {
	move(0, 0);
	clrtoeol();

	addstr("saving...");
	
	ofstream test;
	test.open("test.txt");

	for (auto i: data) {
		for (auto j: i) {
			test << (char) j;
		}
		test << "\n";
	}
	
	test.close();
	
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
