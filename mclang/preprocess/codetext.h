
#ifndef __PREPROCESS_CODETEXT_H__
#define __PREPROCESS_CODETEXT_H__

#include <string>
#include <vector>

/**
 * Code lines are put in a chain where the next line can be retrieved from the
 * current one
 */
struct CodeLine {
    CodeLine(std::string filename, unsigned int line, unsigned int col,
    std::string txt);
    std::string filename;
    unsigned int line = 0, col = 0;
    std::string txt;
    CodeLine *next;
};

struct CodeText {
    CodeText();
    ~CodeText();
    void append(const CodeLine &line);
    CodeLine *first = NULL, *last = NULL;
};

#endif