
#include "errorhandle/handle.h"
#include "preprocess/preprocess.h"
#include <string>
#include <fstream>

InpStream::InpStream(std::string filename) : stream(filename),
filename(filename), line(1), col(0) {

}

InpStream::~InpStream() {
    stream.close();
}

char InpStream::get() {
    char out = stream.get();
    if (out == '\r')
        out = stream.get();
    col++;
    if (out == '\n')
        line++, col = 0;
    return out;
}

char InpStream::peek() {
    char out;
    out = stream.peek();
    if (out == '\r') {
        stream.get();
        out = stream.peek();
    }
    return out;
}

bool InpStream::good() {
    return stream.good() && stream.is_open();
}

Preprocessor::Preprocessor() : curRead(""), includeDepth(0), doOutput(true) {

}

Preprocessor::~Preprocessor() {

}

void Preprocessor::processFile(std::string filename) {
    InpStream fs(filename);
    readCodeBlock(fs);
}

void Preprocessor::readCodeBlock(InpStream &fs) {
    
}

Preprocessor::PrepLine Preprocessor::readLine(InpStream &fs) {
    PrepLine out;
    char cur;
    bool atLineStart = true;
    bool isCmd = false;
    bool inCmdWord = false;
    std::string cmdWord = "";
    fs.stream.get(cur);
    while (fs.stream.good() && fs.stream.is_open() && cur != '\n') {
        out.content.push_back(cur);
        if (atLineStart) {
            if (cur == '#') {
                isCmd = true;
                inCmdWord = true;
            } else if (cur != ' ' && cur != '\t') {
                atLineStart = false;
            }
        }
        if (inCmdWord) {
            if (cur == ' ') {
                inCmdWord = false;
                out.content = "";
            } else {
                cmdWord.push_back(cur);
            }
        }
        fs.stream.get(cur);
    }
    if (cmdWord == "include")
        out.type = LINE_INCL;
    else
        MCLError(1, "Invalid preprocessor statement");
    
}