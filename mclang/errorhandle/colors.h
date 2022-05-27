
#ifndef __ERRORHANDLE_COLORS_H__
#define __ERRORHANDLE_COLORS_H__

#include <map>
#include <string>

const std::map<std::string, std::string> colorNames = {
    {"black", "30"},
    {"red", "31"},
    {"green", "32"},
    {"yellow", "33"},
    {"blue", "34"},
    {"magenta", "35"},
    {"cyan", "36"},
    {"white", "37"},
    {"bold", "1"}
};

/**
 * Output text in the given color to an output stream
 * @param os The output stream to send the colored text to
 * @param txt The text to output
 * @param color The color names of the text, see above names. Provided separated
 * by spaces to allow combinations like "red bold"
 */
void colorText(std::ostream &os, const std::string &txt, std::string color);

#endif