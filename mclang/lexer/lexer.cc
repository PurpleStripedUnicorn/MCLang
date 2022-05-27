
#include "lexer/lexer.h"

Lexer::Lexer(std::string inp) : txt(inp), curIndex(0), atLineStart(true) {

}

std::vector<Token> Lexer::readIn() {
    curIndex = 0, curLoc.line = 1, curLoc.col = 1;
    readTokens.clear();
    Token tok;
    while (!atEnd()) {
        unsigned int lastLine = curLoc.line, lastCol = curLoc.col;
        // Ignore whitespace, atLineStart remains the same, because whitespace
        // is not counted for this
        if (cur() == ' ' || cur() == '\t') {
            next();
        // Keep track of the start of a line, for inserted commands
        // Also keeps track of line numbers
        } else if (cur() == '\n') {
            next();
            atLineStart = true;
            curLoc.line++;
            curLoc.col = 1;
        // Read in normal tokens
        } else if (readInToken(tok)) {
            if (tok.type != TOK_EMPTY) {
                tok.loc.col = lastCol;
                tok.loc.line = lastLine;
                readTokens.push_back(tok);
                atLineStart = false;
            }
        // Token not recognized
        } else {
            // TODO: Add proper error handling
            std::cout << "Could not recognize token at " << lastLine << "["
            << lastCol << "]." << std::endl;
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
    // '/' is special because it can be division, the start of a command or the
    // start of a comment
    if (cur() == '/') {
        next();
        // Start of a comment
        if (cur() == '/') {
            while (cur() != '\n' && !atEnd())
                next();
            tok = Token(TOK_EMPTY);
            return true;
        // Start of a multiline comment
        } else if (cur() == '*') {
            while (true) {
                next();
                while (cur() == '*') {
                    next();
                    if (cur() == '/') {
                        next();
                        tok = Token(TOK_EMPTY);
                        atLineStart = false;
                        return true;
                    }
                }
            }
            // Reached end of file without closing comment
            return false;
        // Start of a command
        } else if (atLineStart) {
            return readInCmd(tok);
        // Division assignment
        } else if (cur() == '=') {
            tok = Token(TOK_ASSIGN_DIV);
            next();
        } else {
            tok = Token(TOK_DIV);
            next();
        }
        return true;
    }
    // Checks for both one- and two-character tokens
    if (twoLetterTokens.count(cur())) {
        const std::map<char, TokenType> &curLook = twoLetterTokens.find(
        cur())->second;
        next();
        // Two characters
        if (curLook.count(cur())) {
            tok = Token(curLook.find(cur())->second);
            next();
            return true;
        }
        // One character
        if (curLook.count(' ')) {
            tok = Token(curLook.find(' ')->second);
            return true;
        }
        return false;
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
    // NOTE: The '/' has already been skipped
    std::string out = "";
    while (cur() != '\n') {
        out += cur();
        next();
    }
    // NOTE: The line ending needs to still be there when exiting this function
    atLineStart = false;
    tok = Token(TOK_CMD, out);
    return true;
}

Token &Lexer::lastRead() const {
    return (Token &)readTokens[readTokens.size() - 1];
}