
#ifndef __PREPTOKEN_H__
#define __PREPTOKEN_H__

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
    PTOK_IDEN,
    // Operator/puctuator, e.g. +, -, ++, *=, ;, etc.
    PTOK_PUNCT,
    // String literals
    PTOK_STR,
    // Number literals
    PTOK_NUM
};

/**
 * Preprocessor token
 * Later converted to actual token by the "main lexer"
 */
struct PrepToken {
    PrepToken(PrepTokenType type);
    PrepToken(PrepTokenType type, std::string content);
    PrepTokenType type;
    std::string content;
};

#endif