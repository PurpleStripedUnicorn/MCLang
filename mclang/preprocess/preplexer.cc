
#include "preprocess/preplexer.h"
#include "preprocess/preptoken.h"
#include <string>
#include <vector>

PrepLexer::PrepLexer(std::string *input, std::string filename,
std::vector<PrepToken> *out) : input(input), loc({filename, 1, 1}), curIndex(0),
out(out) {
    out->clear();
}

PrepLexer::~PrepLexer() {

}