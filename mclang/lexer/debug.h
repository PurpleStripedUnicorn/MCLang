
#ifndef __LEXER__DEBUG_H__
#define __LEXER__DEBUG_H__

#include <string>
#include <vector>

struct Token;

const std::string tokenTypeNames[] = {
    "ERR",
    "EMPTY",
    "CMD",
    ";", ",",
    "WORD",
    "TYPENAME", "CONST", "NAMESPACE",
    "(", ")", "{", "}",
    "+", "-", "/", "*", "%",
    "=",
    "+=", "-=", "/=", "*=", "%=",
    "<", "<=", ">", ">=", "==", "!=",
    "!", "&&", "||",
    "NUM", "STR", "TRUE", "FALSE",
    "EXEC",
    "IF", "ELSEIF", "ELSE"
};

/**
 * Returns a string which is table with debug information of a vector of tokens
 * @param toks Token vector to generate the table from
 * @return A string containing the table
 */
std::string lexerDebugTable(std::vector<Token> *toks);

#endif