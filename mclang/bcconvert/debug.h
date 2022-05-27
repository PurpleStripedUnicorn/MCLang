
#ifndef __BCCONVERT_DEBUG_H__
#define __BCCONVERT_DEBUG_H__

#include "bcconvert/bcconvert.h"
#include <string>
#include <vector>

/**
 * Returns a string with the raw commands in the given functions
 * @param funcList Vector containing the functions, which contain the
 * raw commands
 * @return A string containing the list
 */
std::string bcconvertCmdList(const std::vector<CmdFunc> &funcList);

#endif