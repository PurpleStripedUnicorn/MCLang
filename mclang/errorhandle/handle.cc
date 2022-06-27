
#include "errorhandle/colors.h"
#include "errorhandle/handle.h"
#include <iostream>
#include <string>

MCLError::MCLError(unsigned int level, std::string txt, unsigned int line,
unsigned int col) : level(level), txt(txt), loc({line, col}) {
    print();
}

void MCLError::print() const {
    std::cerr << std::endl;
    for (unsigned int i = 0; i < LEFT_PAD; i++)
        std::cerr << " ";
    unsigned int errtxtSize = 0;
    if (level == 0) {
        colorText(std::cerr, "warning:", "magenta bold");
        errtxtSize = 8;
    } else {
        colorText(std::cerr, "error:", "red bold");
        errtxtSize = 6;
    }
    for (unsigned int i = 0; i + errtxtSize < ERRTXT_PAD; i++)
        std::cerr << " ";
    std::string lineTxt = "";
    if (loc.line != 0 || loc.col != 0)
        lineTxt = "Line " + std::to_string(loc.line) + "["
        + std::to_string(loc.col) + "]";
    while (lineTxt.size() < LINETXT_PAD)
        lineTxt.push_back(' ');
    std::cerr << lineTxt;
    std::cerr << txt.substr(0, TXT_PAD);
    for (unsigned int i = TXT_PAD; i < txt.size(); i += TXT_PAD) {
        std::cerr << std::endl;
        for (unsigned int j = 0; j < LEFT_PAD + ERRTXT_PAD + LINETXT_PAD; j++)
            std::cerr << " ";
        std::cerr << txt.substr(i, TXT_PAD);
    }
    std::cerr << std::endl << std::endl;
    if (level == 1)
        exit(1);
}