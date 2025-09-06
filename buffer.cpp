#include <vector>
#include <string>
#include <ncurses.h>
#include "movement.h"

using namespace std;

class Buffer {
	private:
		vector<vector<char>> data;

	public:
		int height, width;
        int start_x, start_y;
		string filename;
        int tab_size;

        Buffer(int height, int width, string filename, int tab_size) {
            height = height;
            width = width;
            start_x = 0;
            start_y = 1;
            filename = filename;
            tab_size = tab_size;

            init_pair(1, COLOR_WHITE, COLOR_CYAN);
        }		

        void insert_line(size_t idy) {
            data.insert(data.begin() + idy, vector<char>{});
        }

        void insert_char(size_t idx, size_t idy, char c) {
            data[idy].insert(data[idy].begin() + idx, c);
        }
        
        void delete_line(size_t idy) {
            data.erase(data.begin() + idy);
        }

        void delete_char(size_t idx, size_t idy) {
            data[idy].erase(data[idy].begin() + idx);
        }

        void render_data(WINDOW &scr, size_t delta, int cur_x, int cur_y) {
            int curr_x = start_x;
            int curr_y = start_y;

            bool finished = false;
            
            // assume wraparound
            for (int i = delta; i < data.size() && !finished; i++) {
                wb_move(scr, curr_y, curr_x);
                // add numbering on left side  
                attron(COLOR_PAIR(1));
                const chtype ch = i + '0';
                
                if (ch == 9) {
                    for (int i = 0; i < tab_size; i++) {
                        // add tab_size spaces for each tab
                        waddch(&scr, 32);
                        curr_x++;

                        if (curr_x >= scr._maxx) {
                            curr_x = start_x;
                            curr_y++;
                            if (curr_y >= scr._maxy) finished = true;
                        }
                    }
                } else {
                    waddch(&scr, ch);

                    curr_x++;
                    
                    attroff(COLOR_PAIR(1));
                    
                    for (int j = 0; j < data[i].size() && !finished; j++) {
                                           
                        wmove(&scr, curr_y, curr_x);  
                        waddch(&scr, data[i][j]);
                        
                        curr_x++; 

                        if (curr_x >= scr._maxx) {
                            curr_x = start_x;
                            curr_y++;
                            if (curr_y >= scr._maxy) finished = true;
                        }
                    }

                }

            }

            wb_move(scr, cur_y, cur_x);
        }
};
