
#ifndef __PREPTOKEN_H__
#define __PREPTOKEN_H__

#include "general/loc.h"
#include <string>

/**
 * Preprocessor token type
 * These types are refined later by the "main lexer"
 */
enum PrepTokenType {
    // Error type, only used internally
    PTOK_ERR,
    // Empty token type, purely internal
    PTOK_EMPTY,
    // Basic command insertion
    PTOK_CMD,
    // Identifier, later converted to words, namespace def's, etc.
    PTOK_IDENT,
    // Operator/puctuator, e.g. +, -, ++, *=, ;, etc.
    PTOK_PUNCT,
    // String literals
    PTOK_STR,
    // Number literals
    PTOK_NUM,
    // Preprocessor statement
    PTOK_PREP_STMT,
    // Standard library include (so with syntax `<library>`)
    PTOK_INCL_LIB,
    // End of line, used to determine the ends of preprocessor statements
    PTOK_ENDL
};

/**
 * Preprocessor token
 * Later converted to actual token by the "main lexer"
 */
struct PrepToken {
    PrepToken(PrepTokenType type, Loc loc);
    PrepToken(PrepTokenType type, std::string content, Loc loc);
    PrepTokenType type;
    std::string content;
    Loc loc;
};

#endif