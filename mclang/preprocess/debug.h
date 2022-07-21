
#ifndef __PREPROCESS_DEBUG_H__
#define __PREPROCESS_DEBUG_H__

#include <string>
#include <vector>

struct PrepToken;

// Preprocessor token type names
const std::string prepTokenNames[] = {
    "ERR",
    "EMPTY",
    "EOF",
    "CMD",
    "IDENT",
    "PUNCT",
    "STR",
    "NUM",
    "PREP_STMT",
    "INCL_LIB",
    "ENDL"
};

/**
 * Returns a string which is table with debug information of a vector of
 * preprocessor tokens
 * @param toks Token vector to generate the table from
 * @return A string containing the table
 */
std::string prepDebugTable(std::vector<PrepToken> *toks);

#endif