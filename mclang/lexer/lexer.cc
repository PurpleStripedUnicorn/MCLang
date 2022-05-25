
#include "lexer/lexer.h"

Lexer::Lexer(std::string inp) : txt(inp), curIndex(0), atLineStart(true) {

}

std::vector<Token> Lexer::readIn() {
    curIndex = 0, curLoc.line = 1, curLoc.col = 1;
    readTokens.clear();
    Token tok;
    while (!atEnd()) {
        unsigned int lastLine = curLoc.line, lastCol = curLoc.col;
        // Ignore whitespace
        if (cur() == ' ' || cur() == '\t') {
            next();
        } else if (cur() == '\n') {
            next();
            atLineStart = true;
            curLoc.line++;
            curLoc.col = 1;
        } else if (readInToken(tok)) {
            tok.loc.col = lastCol;
            tok.loc.line = lastLine;
            readTokens.push_back(tok);
            atLineStart = false;
        } else {
            // TODO: Add proper error handling
            std::cout << "Could not recognize token!" << std::endl;
            return readTokens;
        }
    }
    return readTokens;
}

void Lexer::next() {
    curIndex++, curLoc.col++;
}

bool Lexer::atEnd() const {
    return curIndex >= txt.size();
}

char Lexer::cur() const {
    if (atEnd())
        return '$';
    return txt[curIndex];
}

bool Lexer::readInToken(Token &tok) {
    // Simple tokens only contain one character
    if (simpleTokens.count(cur())) {
        tok = Token(simpleTokens.find(cur())->second);
        next();
        return true;
    }
    // '/' is special because it can be both division and the start of a command
    if (cur() == '/') {
        if (atLineStart)
            return readInCmd(tok);
        tok = Token(TOK_DIV);
        return true;
    }
    // Special tokens followed by '=', or just '=' by itself
    if (cur() == '=') {
        if (simpleTokens.count(lastRead().type)) {
            lastRead().type = simpleTokens.find(lastRead().type)->second;
            next();
            return true;
        }
        tok = Token(TOK_ASSIGN);
        return true;
    }
    if (('a' <= cur() && cur() <= 'z') || ('A' <= cur() && cur() <= 'Z')) {
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

bool Lexer::readInCmd(Token &tok) {
    // Skip the '/'
    next();
    std::string out = "";
    while (cur() != '\n') {
        out += cur();
        next();
    }
    // NOTE: the line ending needs to still be there when exiting this function
    atLineStart = false;
    tok = Token(TOK_CMD, out);
    return true;
}

Token &Lexer::lastRead() const {
    return (Token &)readTokens[readTokens.size() - 1];
}