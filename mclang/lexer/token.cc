
#include "lexer/token.h"

Token::Token() : type(TOK_ERRTYPE), content("") {
    
}

Token::Token(TokenType type) : type(type), content("") {

}

Token::Token(TokenType type, std::string content) : type(type),
content(content) {
    
}