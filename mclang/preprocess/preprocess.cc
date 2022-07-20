
#include "errorhandle/handle.h"
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

bool Preprocessor::atTokenListEnd() const {
    return readIndexStack.back() >= tokenStack.back().size();
}

PrepToken Preprocessor::curToken() const {
    return tokenStack.back()[readIndexStack.back()];
}

void Preprocessor::nextToken() {
    readIndexStack.back()++;
}

void Preprocessor::readProgram() {
    while (!atTokenListEnd()) {
        PrepToken cur = curToken();
        out.push_back(cur);
        nextToken();
    }
}