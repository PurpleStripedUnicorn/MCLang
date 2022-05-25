
#include <string>

enum TokenType {
    // Error type
    TOK_ERRTYPE,
    // Basic command insertion
    TOK_CMD,
    // Normal word
    TOK_WORD,
    // Typenames and other special words
    TOK_TYPENAME,
    // Braces and stuff
    TOK_LBRACE, TOK_RBRACE, TOK_LCBRACE, TOK_RCBRACE
};

struct Token {
    Token();
    Token(TokenType type);
    Token(TokenType type, std::string content);
    TokenType type;
    std::string content;
};