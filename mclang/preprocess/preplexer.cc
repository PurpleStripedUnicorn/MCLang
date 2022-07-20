
#include "errorhandle/handle.h"
#include "preprocess/preplexer.h"
#include "preprocess/preptoken.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>

PrepLexer::PrepLexer(std::string &input, std::string filename,
std::vector<PrepToken> &out) : input(input), loc({filename, 1, 1}), curIndex(0),
out(out) {
    out.clear();
}

PrepLexer::~PrepLexer() {

}

char PrepLexer::cur() const {
    if (atEnd())
        return ' ';
    return input[curIndex];
}

void PrepLexer::next() {
    // Escaped line endings are ignored here!
    // Also '\r' is ignored
    if (cur() == '\n')
        loc.line++, loc.col = 1;
    else
        loc.col++;
    curIndex++;
    ignoreEnds();
}

void PrepLexer::ignoreEnds() {
    while (cur() == '\r' || cur() == '\\') {
        if (cur() == '\r')
            curIndex++, loc.col++;
        else if (cur() == '\\') {
            if (curIndex + 1 <= input.size() && (input[curIndex + 1] == '\n'
            || input[curIndex + 1] == '\r')) {
                if (input[curIndex + 1] == '\r' && curIndex + 2 <= input.size()
                && input[curIndex + 2] == '\n')
                    curIndex++;
                curIndex += 2, loc.line++, loc.col = 1;
            } else {
                break;
            }
        }
    }
}

bool PrepLexer::atEnd() const {
    return curIndex >= input.size();
}

void PrepLexer::lex() {
    // Possible line ending escaped at the start of the file!
    ignoreEnds();
    bool atLineStart = true;
    while (!atEnd()) {
        if (cur() == '\n')
            out.push_back(PrepToken(PTOK_ENDL, loc)), atLineStart = true, next();
        else if (cur() == ' ' || cur() == '\t')
            next();
        else if (atLineStart && cur() == '/')
            readCmd();
        else if (('a' <= cur() && cur() <= 'z') || ('A' <= cur() && cur() <=
        'Z') || cur() == '_')
            readIdent();
        else if (cur() == '#')
            readPrepStmt();
        else if (cur() == '<' && out.size() >= 1 && out.back().type ==
        PTOK_PREP_STMT && out.back().content == "include")
            readInclLib();
        else if ('0' <= cur() && cur() <= '9')
            readNumber();
        else if (cur() == '"')
            readString();
        else if (checkPunctSymbols())
            ;
        else
            MCLError(1, "Unrecognized token.", loc.line, loc.col);
    }
}

std::vector<PrepToken> &PrepLexer::getOutput() const {
    return out;
}

void PrepLexer::readCmd() {
    // Skip the '/'
    next();
    std::string content = "";
    while (!atEnd() && cur() != '\n')
        content.push_back(cur()), next();
    if (content.size() == 0)
        MCLError(1, "Empty command.", loc.line, loc.col);
    if (content.substr(0, 9) == "function ")
        MCLError(0, "Inserted functions can create undefined behaviour!",
        loc.line, loc.col);
    out.push_back(PrepToken(PTOK_CMD, content, loc));
}

bool PrepLexer::isAlphaNumUS() const {
    return ('a' <= cur() && cur() <= 'z') || ('A' <= cur() && cur() <= 'Z')
    || ('0' <= cur() && cur() <= '9') || cur() == '_';
}

void PrepLexer::readIdent() {
    std::string content = "";
    while (!atEnd() && isAlphaNumUS())
        content.push_back(cur()), next();
    out.push_back(PrepToken(PTOK_IDENT, content, loc));
}

void PrepLexer::readPrepStmt() {
    // Skip the '#'
    next();
    std::string content = "";
    while (!atEnd() && isAlphaNumUS())
        content.push_back(cur()), next();
    out.push_back(PrepToken(PTOK_PREP_STMT, content, loc));
}

void PrepLexer::readNumber() {
    std::string content = "";
    while (!atEnd() && '0' <= cur() && cur() <= '9')
        content.push_back(cur()), next();
    out.push_back(PrepToken(PTOK_NUM, content, loc));
}

void PrepLexer::readString() {
    // Skip the first '"'
    next();
    std::string content = "";
    while (!atEnd() && cur() != '"') {
        if (cur() == '\\') {
            char esc = readEscapeChar();
            if (esc != '\n')
                content.push_back(esc);
        } else {
            content.push_back(cur()), next();
        }
    }
    // Skip the last '"'
    next();
    out.push_back(PrepToken(PTOK_STR, content, loc));
}

char PrepLexer::readEscapeChar() {
    // Skip the '\\'
    next();
    if (escapeChars.count(cur()) == 0)
        return cur();
    return escapeChars.find(cur())->second;
}

void PrepLexer::readInclLib() {
    // Skip the '<'
    next();
    std::string content = "";
    while (!atEnd() && isAlphaNumUS())
        content.push_back(cur()), next();
    if (cur() != '>')
        MCLError(1, "Invalid include", loc.line, loc.col);
    // Skip the '>'
    next();
}

bool PrepLexer::checkPunctSymbols() {
    std::string content = "";
    while (punctSymbols.count(content + cur()))
        content.push_back(cur()), next();
    if (content.size() == 0)
        return false;
    out.push_back(PrepToken(PTOK_PUNCT, content, loc));
}