
#ifndef __LEXER_H__
#define __LEXER_H__

#include "lexer/token.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>

// One-character tokens that can be read in easily
const std::map<char, TokenType> simpleTokens = {
    {'(', TOK_LBRACE},
    {')', TOK_RBRACE},
    {'{', TOK_LCBRACE},
    {'}', TOK_RCBRACE},
    {'+', TOK_ADD},
    {'-', TOK_SUB},
    // NOTE: Division is not present here because of commands starting with '/'
    {'*', TOK_MUL},
    {'%', TOK_MOD}
};

// Tokens that are followed by '=' and then mean something different
const std::map<TokenType, TokenType> eqTokens = {
    {TOK_ADD, TOK_ASSIGN_ADD},
    {TOK_SUB, TOK_ASSIGN_SUB},
    {TOK_DIV, TOK_ASSIGN_DIV},
    {TOK_MUL, TOK_ASSIGN_MUL},
    {TOK_ASSIGN, TOK_EQ}
};

class Lexer {

public:

    /**
     * Constructor
     * @param inp Input string to be tokenized
     */
    Lexer(std::string inp);

    /**
     * Read in the stored string and convert it to tokens
     * @return A vector of tokens generated from the `txt` string
     */
    std::vector<Token> readIn();

private:

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

    /**
     * Read in a word, typename, etc.
     * @param tok Reference to a token object where the fond token will be
     * placed, if any is found
     * @return Boolean indicating if a token could be read
     */
    bool readInCmd(Token &tok);

    /**
     * Get the last read token. This can be used to edit the last token
     * @return A reference to the last token that was read
     */
    Token &lastRead() const;

    // String to be tokenized
    std::string txt;

    // Index of the current character being read
    unsigned int curIndex;

    // Indicates wether we're still reading the start of a line (excluding
    // spaces/tabs)
    bool atLineStart;

    // Vector of currently read tokens
    std::vector<Token> readTokens;

};

#endif