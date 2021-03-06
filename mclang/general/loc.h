
#ifndef __LOC_H__
#define __LOC_H__

#include <string>

/**
 * Standard location indicator, containing filename, line number and column
 * number
 */
struct Loc {
    Loc(unsigned int line = 0, unsigned int col = 0);
    Loc(std::string filename, unsigned int line, unsigned int col);
    const static Loc unknown;
    const static std::string nofile;
    std::string filename;
    unsigned int line, col;
};

#endif