
#ifndef __BCGEN_DEBUG_H__
#define __BCGEN_DEBUG_H__

#include "bcgen/bcgen.h"
#include "bcgen/instr.h"
#include <string>
#include <vector>

const std::string instrNames[] = {
    "ERR",
    "CMD"
};

/**
 * Returns a string with the instructions in the given functions
 * @param funcList Vector containing the functions, which contain the
 * instructions
 * @return A string containing the list
 */
std::string bcgenInstrList(const std::vector<BCFunc> &funcList);

#endif