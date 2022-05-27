
#ifndef __LEXER_TOKEN_H__
#define __LEXER_TOKEN_H__

#include <string>

enum TokenType {
    // Error type
    TOK_ERRTYPE,
    // Empty token type, purely internal
    TOK_EMPTY,
    // Basic command insertion
    TOK_CMD,
    // Normal word
    TOK_WORD,
    // Typenames and other special words
    TOK_TYPENAME,
    // Braces and stuff
    TOK_LBRACE, TOK_RBRACE, TOK_LCBRACE, TOK_RCBRACE,
    // Mathematics and operators
    TOK_ADD, TOK_SUB, TOK_DIV, TOK_MUL, TOK_MOD,
    // Assignment
    TOK_ASSIGN,
    // Assignment operators, have to be in the same order as normal operators!
    TOK_ASSIGN_ADD, TOK_ASSIGN_SUB, TOK_ASSIGN_DIV, TOK_ASSIGN_MUL,
    TOK_ASSIGN_MOD,
    // Strings, numbers and other constants
    TOK_NUM, TOK_STR, TOK_TRUE, TOK_FALSE,
    // Comparisons
    TOK_EQ
};

struct Token {
    Token();
    Token(TokenType type);
    Token(TokenType type, std::string content);
    TokenType type;
    std::string content;
    struct {
        unsigned int line, col;
    } loc;
};

#endif