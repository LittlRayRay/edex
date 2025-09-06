#ifndef BUFFER_EDEX
#define BUFFER_EDEX

#include <ncurses.h>

#include <vector>
#include <string>

class Buffer {
    private:
        std::vector<std::vector<char>> data;

    public:
        int height, width, start_x, start_y;
        std::string filename;

        Buffer(int height, int width, std::string filename);

        void insert_line(size_t idy);
        void insert_char(size_t idx, size_t idy, char c);
        void delete_line(size_t idy);
        void delete_char(size_t idx, size_t idy);

        void render_data (WINDOW &scr, size_t delta, int cur_x, int cur_y);

};

#endif
