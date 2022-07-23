
#include "errorhandle/colors.h"
#include "errorhandle/handle.h"
#include "general/loc.h"
#include <iostream>
#include <string>

MCLError::MCLError(unsigned int level, std::string txt, unsigned int line,
unsigned int col) : level(level), txt(txt), loc(Loc(line, col)) {
    print();
}

MCLError::MCLError(unsigned int level, std::string txt, Loc loc) : level(level),
txt(txt), loc(loc) {
    print();
}

void MCLError::print() const {
    std::string locText = loc.filename;
    if (loc.line == 0)
        locText += ":??";
    else
        locText += ":" + std::to_string(loc.line);
    if (loc.col != 0 && loc.line != 0)
        locText += ":" + std::to_string(loc.col);
    colorText(std::cerr, locText + ": ", "white bold");
    if (level == 0)
        colorText(std::cerr, "warning:", "magenta bold");
    else
        colorText(std::cerr, "error:", "red bold");
    std::cerr << std::endl << "    " << txt << std::endl;
    if (level == 1)
        exit(1);
}