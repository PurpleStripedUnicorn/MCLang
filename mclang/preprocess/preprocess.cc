
#include "errorhandle/handle.h"
#include "general/loc.h"
#include "preprocess/preplexer.h"
#include "preprocess/preprocess.h"
#include "preprocess/preptoken.h"
#include <fstream>
#include <sstream>
#include <string>

Preprocessor::Preprocessor() : includeDepth(0), doOutput(true) {

}

Preprocessor::~Preprocessor() {

}

void Preprocessor::processFile(std::string filename) {
    std::ifstream file(filename);
    if (!file.is_open())
        MCLError(1, "Could not open given file \"" + filename + "\"");
    std::stringstream contentStream;
    contentStream << file.rdbuf();
    std::string content = contentStream.str();
    tokenStack.push_back(std::vector<PrepToken>());
    readIndexStack.push_back(0);
    PrepLexer lex(content, filename, tokenStack.back());
    lex.lex();
    readProgram();
    tokenStack.pop_back();
    readIndexStack.pop_back();
}

std::vector<PrepToken> &Preprocessor::getOutput() const {
    return (std::vector<PrepToken> &)out;
}

std::vector<PrepToken> &Preprocessor::curTokenList() const {
    return (std::vector<PrepToken> &)tokenStack.back();
}

inline bool Preprocessor::atEnd() const {
    return readIndexStack.back() >= curTokenList().size();
}

inline PrepToken Preprocessor::cur() const {
    if (atEnd()) {
        Loc lastLoc = tokenStack.back().back().loc;
        return PrepToken(PTOK_EOF, lastLoc);
    }
    return tokenStack.back()[readIndexStack.back()];
}

inline void Preprocessor::next() {
    readIndexStack.back()++;
}

void Preprocessor::expect(PrepTokenType type) {
    if (cur().type != type)
        MCLError(1, "Unexpected token.", cur().loc.line, cur().loc.col);
}

void Preprocessor::expect(PrepTokenType type, std::string content) {
    if (cur().type != type || cur().content != content)
        MCLError(1, "Unexpected token.", cur().loc.line, cur().loc.col);
}

void Preprocessor::write(PrepToken tok) {
    if (doOutput)
        out.push_back(tok);
}

void Preprocessor::readProgram() {
    doOutput = true;
    readCodeBlock();
    if (!atEnd())
        MCLError(1, "Stopped reading before EOF.", cur().loc.line,
        cur().loc.col);
}

void Preprocessor::readCodeBlock() {
    while (!atEnd()) {
        if (cur().type == PTOK_IDENT)
            readIdent();
        else if (cur().type == PTOK_PREP_STMT)
            readPrepStmt();
        else if (cur().type == PTOK_INCL_LIB)
            MCLError(1, "Unexpected include library.", cur().loc.line,
            cur().loc.col);
        else
            write(cur()), next();
    }
}

void Preprocessor::readIdent() {
    expect(PTOK_IDENT);
    std::vector<PrepToken> tmp = convertIdent(cur());
    for (unsigned int i = 0; i < tmp.size(); i++)
        write(tmp[i]);
    next();
}

void Preprocessor::readPrepStmt() {
    expect(PTOK_PREP_STMT);
    std::string stmt = cur().content;
    if (stmt == "include")
        readInclude();
    else if (stmt == "define")
        readDefine();
    else
        MCLError(1, "Unrecognized statement \"" + stmt + "\".", cur().loc.line,
        cur().loc.col);
}

void Preprocessor::readInclude() {
    expect(PTOK_PREP_STMT, "include"), next();
    if (cur().type == PTOK_STR) {
        processFile(cur().content);
    } else if (cur().type == PTOK_INCL_LIB) {
        // TODO: implement
        MCLError(1, "Feature not implemented.", cur().loc.line, cur().loc.col);
    } else {
        MCLError(1, "Expected include file.", cur().loc.line, cur().loc.col);
    }
    // Skip the include file text
    next();
    expect(PTOK_ENDL), next();
}

std::vector<PrepToken> Preprocessor::convertIdent(PrepToken tok) const {
    if (tok.type == PTOK_IDENT && defs.count(tok.content) > 0)
        return defs.find(tok.content)->second;
    return {tok};
}

void Preprocessor::readDefine() {
    expect(PTOK_PREP_STMT, "define"), next();
    // Indentifier to define
    expect(PTOK_IDENT);
    std::string name = cur().content;
    next();
    std::vector<PrepToken> replace;
    while (!atEnd() && cur().type != PTOK_ENDL) {
        std::vector<PrepToken> tmp = convertIdent(cur());
        replace.insert(replace.end(), tmp.begin(), tmp.end());
    }
    if (replace.empty())
        MCLError(1, "Empty definition", cur().loc.line, cur().loc.col);
    defs.insert({name, replace});
    // Skip end of line
    next();
}