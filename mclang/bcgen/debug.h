
#ifndef __BCGEN_DEBUG_H__
#define __BCGEN_DEBUG_H__

#include <string>
#include <vector>

struct BCFunc;

const std::string instrNames[] = {
    "ERR",
    "CMD",
    "EXEC",
    "CALL",
    "SET",
    "PUSH",
    "POP",
    "TOP",
    "COPY",
    "ADD", "SUB", "MUL", "DIV", "MOD"
};

/**
 * Returns a string with the instructions in the given functions
 * @param funcList Vector containing the functions, which contain the
 * instructions
 * @return A string containing the list
 */
std::string bcgenInstrList(const std::vector<BCFunc *> *funcList);

#endif