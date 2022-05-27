
#ifndef __LEXER__DEBUG_H__
#define __LEXER__DEBUG_H__

#include "lexer/token.h"
#include <string>
#include <vector>

const std::string tokenTypeNames[] = {
    "ERR",
    "EMPTY",
    "CMD",
    "WORD",
    "TYPENAME",
    "(", ")", "{", "}",
    "+", "-", "/", "*", "%",
    "=",
    "+=", "-=", "/=", "*=", "%=",
    "NUM", "STR", "TRUE", "FALSE"
};

/**
 * Returns a string which is table with debug information of a vector of tokens
 * @param toks Token vector to generate the table from
 * @return A string containing the table
 */
std::string lexerDebugTable(std::vector<Token> toks);

#endif