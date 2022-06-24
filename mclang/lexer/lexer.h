
#ifndef __LEXER_H__
#define __LEXER_H__

#include "compiler/compiler.h"
#include "errorhandle/handle.h"
#include "lexer/token.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>

// Two-character tokens list, ordered by first character and then by second
// character, a space character means it is also a single character
const std::map<char, std::map<char, TokenType>> twoLetterTokens = {
    {'(', {{' ', TOK_LBRACE}}},
    {')', {{' ', TOK_RBRACE}}},
    {'{', {{' ', TOK_LCBRACE}}},
    {'}', {{' ', TOK_RCBRACE}}},
    {'+', {{' ', TOK_ADD}, {'=', TOK_ASSIGN_ADD}}},
    {'-', {{' ', TOK_SUB}, {'=', TOK_ASSIGN_SUB}}},
    {'/', {{' ', TOK_DIV}, {'=', TOK_ASSIGN_DIV}}},
    {'*', {{' ', TOK_MUL}, {'=', TOK_ASSIGN_MUL}}},
    {'=', {{' ', TOK_ASSIGN}, {'=', TOK_EQ}}}
};

// Execute-statement indicators, i.e. positioned, as, at, etc.
// Note that if-statements are special and not listed here! The "store"
// subcommand is not supported
const std::string execNames[] = {
    "align", "anchored", "as", "at", "facing", "in", "positioned", "rotated",
    "unless"
};

class Lexer {

public:

    /**
     * Constructor
     * @param comp Main compiler component
     */
    Lexer(Compiler *comp);

    /**
     * Read in the stored string and convert it to tokens
     * @post The result is stored in the `readTokens` vector
     */
    void readIn();

    /**
     * Get the read tokens
     * @pre `readIn` function has been run before
     * @return A pointer to a vector of tokens, stored in `readTokens`
     */
    std::vector<Token> *tokens() const;

private:

    /**
     * Move to the next character
     * @post `curIndex` is incremented, `curLoc.col` is incremented
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
     * Read in one token
     * @param tok Reference to a token object where the found token will be
     * placed, if any is found
     * @return Boolean indicating if a token could be read
     */
    bool readInToken(Token &tok);

    /**
     * Read in a word, typename, etc.
     * @param tok Reference to a token object where the found token will be
     * placed, if any is found
     * @return Boolean indicating if a token could be read
     */
    bool readInWord(Token &tok);

    /**
     * Read in a word, typename, etc.
     * @param tok Reference to a token object where the found token will be
     * placed, if any is found
     * @return Boolean indicating if a token could be read
     */
    bool readInCmd(Token &tok);

    /**
     * Read in a string literal
     * @param tok Reference to a token object where the found token will be
     * placed, if any is found
     * @return Boolean indicating if a token could be read
     */
    bool readInString(Token &tok);

    /**
     * Convert an escaped character to its escaped version
     * @param inp The input character
     * @return The escaped character that the input character represents. If
     * none can be found, the input character is returned
     */
    char convertEscapeChar(char inp);

    /**
     * Get the last read token. This can be used to edit the last token
     * @return A reference to the last token that was read
     */
    Token &lastRead() const;

    // Compiler component
    Compiler *comp;

    // String to be tokenized
    std::string txt;

    // Index of the current character being read
    unsigned int curIndex;

    // Indicates wether we're still reading the start of a line (excluding
    // spaces/tabs)
    bool atLineStart;

    // Vector of currently read tokens
    std::vector<Token> readTokens;

    // Current location where we are reading, line and column (both start
    // counting from 1)
    struct {
        unsigned int line, col;
    } curLoc;

};

#endif