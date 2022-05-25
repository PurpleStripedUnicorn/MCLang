
#include "lexer/token.h"

Token::Token(TokenType type) : type(type) {

}

Token::Token(TokenType type, std::string content) : type(type),
content(content) {
    
}