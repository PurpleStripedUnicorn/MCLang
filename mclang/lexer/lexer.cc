
#include "lexer/lexer.h"

Lexer::Lexer(std::string inp) : txt(inp), curIndex(0), atLineStart(true) {

}

void Lexer::next() {
    curIndex++;
}

bool Lexer::atEnd() const {
    return curIndex >= txt.size();
}

char Lexer::cur() const {
    if (atEnd())
        return '$';
    return txt[curIndex];
}

std::vector<Token> Lexer::readIn() {
    curIndex = 0;
    Token tok;
    std::vector<Token> out;
    while (!atEnd()) {
        // Ignore whitespace
        if (cur() == ' ' || cur() == '\t') {
            ;
        } else if (cur() == '\n') {
            atLineStart = true;
        } else if (readInToken(tok)) {
            out.push_back(tok);
            atLineStart = false;
        } else {
            // TODO: Add proper error handling
            std::cout << "Could not recognize token!" << std::endl;
            return out;
        }
    }
    return out;
}

bool Lexer::readInToken(Token &tok) {
    if (cur() == '(') {
        tok = Token(TOK_LBRACE, "");
        return true;
    } else if (cur() == ')') {
        tok = Token(TOK_RBRACE, "");
        return true;
    } else if (cur() == '{') {
        tok = Token(TOK_LCBRACE, "");
        return true;
    } else if (cur() == '}') {
        tok = Token(TOK_RCBRACE, "");
        return true;
    } else if (cur() == '/' && atLineStart) {
        tok = Token(TOK_RCBRACE, "");
        return true;
    } else if (cur() == '}') {
        tok = Token(TOK_RCBRACE, "");
        return true;
    } else if (('a' <= cur() && cur() <= 'z') || ('A' <= cur() && cur() <= 'Z'))
    {
        return readInWord(tok);
    }
    return false;
}

bool Lexer::readInWord(Token &tok) {
    std::string word = "";
    while (('a' <= cur() && cur() <= 'z') || ('A' <= cur() && cur() <= 'Z')
    || cur() == '_' || ('0' <= cur() && cur() <= '9')) {
        word.push_back(cur());
        next();
    }
    if (word == "")
        return false;
    // Detect typenames and special words
    TokenType tt = TOK_WORD;
    if (word == "int" || word == "void")
        tt = TOK_TYPENAME;
    tok = Token(tt, word);
    return true;
}