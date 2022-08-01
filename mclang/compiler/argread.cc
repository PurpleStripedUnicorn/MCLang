
#include "compiler/argread.h"
#include "compiler/compiler.h"
#include "errorhandle/handle.h"
#include <iostream>
#include <string>

ArgReader::ArgReader(Compiler *comp) : comp(comp), curIndex(0), argc(0),
argv(nullptr), hasGivenFilename(false) {

}

ArgReader::~ArgReader() {

}

void ArgReader::read(int argc, char *argv[]) {
    curIndex = 1;
    this->argc = argc;
    this->argv = argv;
    hasGivenFilename = false;
    while (curIndex < argc)
        readCur();
    if (!hasGivenFilename)
        MCLError(1, "No filename given.");
}

std::string ArgReader::execName() const {
    if (argc < 1)
        exit(1);
    return std::string(argv[0]);
}

void ArgReader::invalidError() const {
    MCLError(1, "Invalid command line arguments.");
}

std::string ArgReader::cur() const {
    if (curIndex >= argc)
        invalidError();
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
    hasGivenFilename = true;
    next();
}

void ArgReader::readLetterArg() {
    if (cur().size() != 2)
        invalidError();
    char letter = cur()[1];
    next();
    if (letter == 'c')
        readDescription();
    else if (letter == 'd')
        readDebug();
    else if (letter == 'D')
        readDisableOutput();
    else if (letter == 'n')
        readNamespace();
    else if (letter == 'h')
        readHelp();
    else if (letter == 'o')
        readOutput();
    else if (letter == 'v')
        readVersion();
}

void ArgReader::readWordArg() {
    if (cur().size() <= 2)
        invalidError();
    std::string word = cur().substr(2);
    next();
    if (word == "custom-description")
        readDescription();
    else if (word == "debug")
        readDebug();
    else if (word == "disable-output")
        readDisableOutput();
    else if (word == "namespace")
        readNamespace();
    else if (word == "help")
        readHelp();
    else if (word == "output")
        readOutput();
    else if (word == "scoreboard")
        readScoreboard();
    else if (word == "version")
        readVersion();
}

void ArgReader::readDescription() {
    comp->description = cur();
    next();
}

void ArgReader::readDebug() {
    comp->debugMode = true;
}

void ArgReader::readDisableOutput() {
    comp->fileOutput = false;
}

void ArgReader::readNamespace() {
    comp->ns = cur();
    next();
}

void ArgReader::readHelp() {
    std::cout << std::endl << "Command usage: " << execName()
    << " <input filename> [<arguments>]" << std::endl << std::endl;
    for (unsigned int i = 0; i < sizeof(argList) / sizeof(ArgHelpText); i++) {
        std::string cur = "";
        if (argList[i].letter != ' ')
            cur.push_back('-'), cur.push_back(argList[i].letter);
        if (argList[i].letter != ' ' && argList[i].word != "")
            cur.append(", ");
        if (argList[i].word != "")
            cur.append("--" + argList[i].word);
        while (cur.size() < HELPPADLEFT)
            cur.push_back(' ');
        cur.append(argList[i].description.substr(0, HELPPADRIGHT));
        for (unsigned int j = HELPPADRIGHT; j < argList[i].description.size();
        j += HELPPADRIGHT) {
            cur.push_back('\n');
            for (unsigned int k = 0; k < HELPPADLEFT; k++)
                cur.push_back(' ');
            cur.append(argList[i].description.substr(j, HELPPADRIGHT));
        }
        std::cout << cur << std::endl;
    }
    std::cout << std::endl;
    exit(0);
}

void ArgReader::readOutput() {
    comp->outputFolder = cur();
    next();
}

void ArgReader::readScoreboard() {
    comp->scoreboardName = cur();
    next();
}

void ArgReader::readVersion() {
    comp->mcVersion = cur();
    next();
}