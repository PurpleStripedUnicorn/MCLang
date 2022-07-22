
#include "errorhandle/handle.h"
#include "preprocess/preplexer.h"
#include "preprocess/preptoken.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>

PrepLexer::PrepLexer(const std::string &input, std::string filename,
std::vector<PrepToken> &out) : input(input), loc({filename, 1, 1}), curIndex(0),
out(out) {
    out.clear();
}

PrepLexer::~PrepLexer() {

}

void PrepLexer::lex() {
    // Possible line ending escaped at the start of the file!
    ignoreEnds();
    atLineStart = true;
    while (!atEnd()) {
        if (cur() == '\n')
            out.push_back(PrepToken(PTOK_ENDL, loc)), next();
        else if (cur() == ' ' || cur() == '\t')
            next();
        else if (cur() == '/')
            readSlash();
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

char PrepLexer::cur() const {
    if (atEnd())
        return ' ';
    return input[curIndex];
}

void PrepLexer::next() {
    // Escaped line endings are ignored here!
    // Also '\r' is ignored
    if (cur() != ' ' && cur() != '\t')
        atLineStart = false;
    if (cur() == '\n')
        loc.line++, loc.col = 1, atLineStart = true;
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

void PrepLexer::readSlash() {
    // Skip the '/'
    bool tmpLineStart = atLineStart;
    next();
    if (atEnd()) {
        out.push_back(PrepToken(PTOK_PUNCT, "/", loc));
        return;
    }
    if (cur() == '/')
        next(), readSingleComment();
    else if (cur() == '*')
        next(), readMultiComment();
    else if (tmpLineStart)
        readCmd();
    else
        // NOTE: This will always return true because of the prefix
        checkPunctSymbols("/");
}

void PrepLexer::readSingleComment() {
    while (!atEnd() && cur() != '\n')
        next();
    // Also skip over the line ending
    if (!atEnd() && cur() == '\n')
        next();
}

void PrepLexer::readMultiComment() {
    // Number of characters into "*/" that have been read
    unsigned int readStop = 0;
    while (!atEnd() && readStop < 2) {
        if (cur() == '*')
            readStop = 1;
        else if (readStop == 1 && cur() == '/')
            readStop = 2;
        else
            readStop = 0;
        next();
    }
}

void PrepLexer::readCmd() {
    std::string content = "";
    while (!atEnd() && cur() != '\n')
        content.push_back(cur()), next();
    if (content.size() == 0)
        MCLError(1, "Empty command.", loc.line, loc.col);
    if (content.substr(0, 9) == "function ")
        MCLError(0, "Inserted functions can create undefined behaviour!",
        loc.line, loc.col);
    // Ignore single-line comments
    if (content[0] != '/')
        out.push_back(PrepToken(PTOK_CMD, content, loc));
}

inline bool PrepLexer::isAlphaNumUS() const {
    return ('a' <= cur() && cur() <= 'z') || ('A' <= cur() && cur() <= 'Z')
    || ('0' <= cur() && cur() <= '9') || cur() == '_';
}

void PrepLexer::readIdent() {
    std::string content = "";
    while (!atEnd() && isAlphaNumUS())
        content.push_back(cur()), next();
    // Because we want to treat the point after the identifier in a "#define"
    // statement as the start of a new line, we have an extra check here
    if (out.size() >= 1 && out.back().type == PTOK_PREP_STMT
    && out.back().content == "define")
        atLineStart = true;
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
    out.push_back(PrepToken(PTOK_INCL_LIB, content, loc));
}

bool PrepLexer::checkPunctSymbols(std::string prefix) {
    std::string content = prefix;
    while (punctSymbols.count(content + cur()))
        content.push_back(cur()), next();
    if (content.size() == 0)
        return false;
    out.push_back(PrepToken(PTOK_PUNCT, content, loc));
    return true;
}