
#include "lexer/token.h"
#include <string>

Token::Token() : type(TOK_ERRTYPE), content("") {
    
}

Token::Token(TokenType type) : type(type), content("") {

}

Token::Token(TokenType type, std::string content) : type(type),
content(content) {
    
}

Token::Token(TokenType type, std::string content, Loc loc) : type(type),
content(content), loc(loc) {
    
}