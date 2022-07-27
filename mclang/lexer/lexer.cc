
#include "compiler/compiler.h"
#include "errorhandle/handle.h"
#include "lexer/lexer.h"
#include "lexer/token.h"
#include "preprocess/preprocess.h"
#include "preprocess/preptoken.h"
#include <map>
#include <string>
#include <vector>

Lexer::Lexer(Compiler *comp) : comp(comp), input(comp->prep->getOutput()),
curIndex(0) {

}

void Lexer::readIn() {
    curIndex = 0;
    readTokens.clear();
    Token tok;
    while (!atEnd()) {
        Loc lastLoc = cur().loc;
        if (readInToken(tok)) {
            if (tok.type != TOK_EMPTY) {
                tok.loc = lastLoc;
                readTokens.push_back(tok);
            }
        // Token not recognized
        } else {
            MCLError(1, "Could not recognize token.", lastLoc);
        }
    }
}

std::vector<Token> *Lexer::tokens() const {
    return (std::vector<Token> *)&readTokens;
}

inline void Lexer::next() {
    curIndex++;
}

inline bool Lexer::atEnd() const {
    return curIndex >= input.size();
}

PrepToken Lexer::cur() const {
    if (atEnd()) {
        Loc lastLoc = Loc::unknown;
        if (input.size() > 0)
            lastLoc = input.back().loc;
        return PrepToken(PTOK_EOF, lastLoc);
    }
    return input[curIndex];
}

bool Lexer::readInToken(Token &tok) {
    if (cur().type == PTOK_EMPTY || cur().type == PTOK_EOF || cur().type ==
    PTOK_ENDL) {
        tok = Token(TOK_EMPTY), next();
        return true;
    }
    if (cur().type == PTOK_CMD) {
        tok = Token(TOK_CMD, cur().content), next();
        return true;
    }
    if (cur().type == PTOK_IDENT) {
        tok = convertIdent(cur().content), next();
        return true;
    }
    if (cur().type == PTOK_PUNCT) {
        tok = convertPunct(cur().content), next();
        return true;
    }
    if (cur().type == PTOK_STR) {
        tok = Token(TOK_STR, cur().content), next();
        return true;
    }
    if (cur().type == PTOK_NUM) {
        tok = Token(TOK_NUM, cur().content), next();
        return true;
    }
    return false;
}

inline Token &Lexer::lastRead() const {
    return (Token &)readTokens[readTokens.size() - 1];
}

Token Lexer::convertIdent(std::string ident) {
    TokenType tt = TOK_WORD;
    if (ident == "int" || ident == "void" || ident == "bool")
        tt = TOK_TYPENAME;
    if (ident == "const")
        tt = TOK_CONST;
    if (ident == "namespace")
        tt = TOK_NAMESPACE;
    if (ident == "if")
        tt = TOK_IF;
    if (readTokens.size() > 0 && ident == "if" && lastRead().type == TOK_ELSE) {
        tt = TOK_ELSEIF;
        readTokens.pop_back();
    }
    if (ident == "else")
        tt = TOK_ELSE;
    for (unsigned int i = 0; i < sizeof(execNames) / sizeof(execNames[0]); i++)
        if (ident == execNames[i])
            tt = TOK_EXEC_STMT;
    return Token(tt, ident);
}

Token Lexer::convertPunct(std::string punct) {
    if (punctTable.count(punct) == 0)
        MCLError(1, "Unexpected error reading punctuation", cur().loc);
    return Token(punctTable.find(punct)->second);
}