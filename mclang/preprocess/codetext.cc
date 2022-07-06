
#include "preprocess/codetext.h"

CodeLine::CodeLine(std::string filename, unsigned int line, unsigned int col,
std::string txt) : filename(filename), line(line), col(col), txt(txt),
next(NULL) {

}

CodeText::CodeText() {

}

CodeText::~CodeText() {
    CodeLine *cur = first;
    while (cur != NULL) {
        CodeLine *tmp = cur->next;
        delete cur;
        cur = tmp;
    }
}

void CodeText::append(const CodeLine &line) {
    CodeLine *cl = new CodeLine(line);
    if (first == NULL || last == NULL)
        first = last = cl;
    else
        last->next = cl, last = cl;
}