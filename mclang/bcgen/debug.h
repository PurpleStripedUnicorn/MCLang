
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
    "M_ARITH_START",
    "ADD", "SUB", "MUL", "DIV", "MOD",
    "M_ARITH_END",
    "M_ARITHI_START",
    "ADDI", "SUBI", "MULI", "DIVI", "MODI",
    "M_ARITHI_END"
};

/**
 * Returns a string with the instructions in the given functions
 * @param funcList Vector containing the functions, which contain the
 * instructions
 * @return A string containing the list
 */
std::string bcgenInstrList(const std::vector<BCFunc *> *funcList);

#endif