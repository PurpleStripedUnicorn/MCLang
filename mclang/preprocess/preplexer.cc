
#include "preprocess/preplexer.h"
#include "preprocess/preptoken.h"
#include <string>
#include <vector>

PrepLexer::PrepLexer(std::string &input, std::string filename,
std::vector<PrepToken> *out) : input(input), loc({filename, 1, 1}), curIndex(0),
out(out) {
    out->clear();
    // Possible line ending escaped at the start of the file!
    ignoreEnds();
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