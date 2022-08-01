
#include "compiler/argread.h"
#include "compiler/compiler.h"
#include "errorhandle/handle.h"
#include <string>

ArgReader::ArgReader(Compiler *comp) : comp(comp), curIndex(0), argc(0),
argv(nullptr), hasGivenFilename(false) {

}

ArgReader::~ArgReader() {

}

void ArgReader::read(int argc, char *argv[]) {
    curIndex = 0;
    this->argc = argc;
    this->argv = argv;
    hasGivenFilename = false;
    while (curIndex < argc)
        readCur();
    if (!hasGivenFilename)
        MCLError(1, "No filename given.");
}

std::string ArgReader::cur() const {
    if (curIndex >= argc)
        return "";
    return std::string(argv[curIndex]);
}

void ArgReader::next() {
    curIndex++;
}

void ArgReader::readCur() {
    if (cur().size() >= 1 && cur()[0] == '-') {
        if (cur().size() >= 2 && cur()[1] == '-')
            readWordArg();
        else
            readLetterArg();
    } else {
        readMainArg();
    }
}

void ArgReader::readMainArg() {
    comp->filename = cur();
    next();
}