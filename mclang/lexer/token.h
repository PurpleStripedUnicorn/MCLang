
#ifndef __LEXER_TOKEN_H__
#define __LEXER_TOKEN_H__

#include "general/loc.h"
#include <string>

enum TokenType {
    // Error type
    TOK_ERRTYPE,
    // Empty token type, purely internal
    TOK_EMPTY,
    // Basic command insertion
    TOK_CMD,
    // Punctuation
    TOK_SEMICOL, TOK_COMMA,
    // Normal word
    TOK_WORD,
    // Typenames and other special words
    TOK_TYPENAME, TOK_CONST, TOK_NAMESPACE,
    // Braces and stuff
    TOK_LBRACE, TOK_RBRACE, TOK_LCBRACE, TOK_RCBRACE,
    // Mathematics and operators
    TOK_ADD, TOK_SUB, TOK_DIV, TOK_MUL, TOK_MOD,
    // Assignment
    TOK_ASSIGN,
    // Assignment operators, have to be in the same order as normal operators!
    TOK_ASSIGN_ADD, TOK_ASSIGN_SUB, TOK_ASSIGN_DIV, TOK_ASSIGN_MUL,
    TOK_ASSIGN_MOD,
    // Comparison operators
    TOK_LT, TOK_LTE, TOK_GT, TOK_GTE, TOK_EQ,
    // Logical operators
    TOK_NOT, TOK_AND, TOK_OR,
    // Strings, numbers and other constants
    TOK_NUM, TOK_STR, TOK_TRUE, TOK_FALSE,
    // Execute-like statements, similar to if-statements, but only take strings
    // inside the round brackets
    TOK_EXEC_STMT,
    // If-statements
    TOK_IF, TOK_ELSEIF, TOK_ELSE
};

struct Token {
    Token();
    Token(TokenType type);
    Token(TokenType type, std::string content);
    Token(TokenType type, std::string content, Loc loc);
    TokenType type;
    std::string content;
    Loc loc;
};

#endif