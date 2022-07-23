
#include "general/loc.h"
#include "lexer/token.h"
#include <string>

Token::Token() : type(TOK_ERRTYPE), content(""), loc(Loc::unknown) {
    
}

Token::Token(TokenType type) : type(type), content(""), loc(Loc::unknown) {

}

Token::Token(TokenType type, std::string content) : type(type),
content(content), loc(Loc::unknown) {
    
}

Token::Token(TokenType type, std::string content, Loc loc) : type(type),
content(content), loc(loc) {
    
}