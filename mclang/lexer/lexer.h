
#ifndef __LEXER_H__
#define __LEXER_H__

#include "lexer/token.h"
#include <iostream>
#include <string>
#include <vector>

class Lexer {

public:

    /**
     * Constructor
     * @param inp Input string to be tokenized
     */
    Lexer(std::string inp);

    /**
     * Move to the next character
     * @post `curIndex` is incremented
     */
    void next();

    /**
     * Check if the lexer is at the end of the input string
     * @return Boolean indicating if the current index is past the end of the
     * input string
     */
    bool atEnd() const;

    /**
     * Get the current character
     * @return The character at the `curIndex` position in `txt`
     */
    char cur() const;

    /**
     * Read in the stored string and convert it to tokens
     * @return A vector of tokens generated from the `txt` string
     */
    std::vector<Token> readIn();

    /**
     * Read in one token
     * @param tok Reference to a token object where the found token will be
     * placed, if any is found
     * @return Boolean indicating if a token could be read
     */
    bool readInToken(Token &tok);

    /**
     * Read in a word, typename, etc.
     * @param tok Reference to a token object where the fond token will be
     * placed, if any is found
     * @return Boolean indicating if a token could be read
     */
    bool readInWord(Token &tok);

private:

    // String to be tokenized
    std::string txt;

    // Index of the current character being read
    unsigned int curIndex;

    // Indicates wether we're still reading the start of a line (excluding
    // spaces/tabs)
    bool atLineStart;

};

#endif