
#include "errorhandle/colors.h"
#include <map>
#include <string>

// Colors are only supported on Linux
#ifdef OS_LINUX

void colorText(std::ostream &os, const std::string &txt, std::string color) {
    std::string curColor = "";
    color.push_back(' ');
    for (unsigned int i = 0; i < color.size(); i++) {
        if (color[i] == ' ' && colorNames.count(curColor) > 0) {
            std::string ctxt = "\033[" + colorNames.find(curColor)->second;
            os << std::string("\033[") << ctxt << std::string("m");
            curColor = "";
        } else {
            curColor.push_back(color[i]);
        }
    }
    os << txt << std::string("\033[0m");
}

#else

void colorText(std::ostream &os, const std::string &txt, std::string color) {
    os << txt;
}

#endif