#ifndef FILE_HANDLING_EDEX
#define FILE_HANDLING_EDEX

#include <vector>


void save(int x, int y, bool& saved, std::vector<std::vector<char>>& data);

void unsave(int x, int y, bool& saved);


#endif
