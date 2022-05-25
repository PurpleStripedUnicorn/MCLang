
#ifndef __LEXER_H__
#define __LEXER_H__

#include <string>

class Lexer {

public:

    /**
     * Constructor
     * @param inp Input string to be tokenized
     */
    Lexer(std::string inp);

private:

    // String to be tokenized
    std::string txt;

};

#endif