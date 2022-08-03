
#ifndef __LEXER_H__
#define __LEXER_H__

#include "lexer/token.h"
#include "preprocess/preptoken.h"
#include <map>
#include <string>
#include <vector>

class Compiler;

const std::map<std::string, TokenType> punctTable = {
    {"(", TOK_LBRACE},
    {")", TOK_RBRACE},
    {"{", TOK_LCBRACE},
    {"}", TOK_RCBRACE},
    {";", TOK_SEMICOL},
    {",", TOK_COMMA},
    {"+", TOK_ADD}, {"+=", TOK_ASSIGN_ADD},
    {"-", TOK_SUB}, {"-=", TOK_ASSIGN_SUB},
    {"*", TOK_MUL}, {"*=", TOK_ASSIGN_MUL},
    {"/", TOK_DIV}, {"/=", TOK_ASSIGN_DIV},
    {"%", TOK_MOD}, {"%=", TOK_ASSIGN_MOD},
    {"=", TOK_ASSIGN}, {"==", TOK_EQ},
    {"<", TOK_LT}, {"<=", TOK_LTE},
    {">", TOK_GT}, {">=", TOK_GTE},
    {"&&", TOK_AND},
    {"||", TOK_OR}
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
     * Read in the stored input and convert it to tokens
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
     * Move to the next preprocessor token
     * @post `curIndex` is incremented
     */
    inline void next();

    /**
     * Check if the lexer is at the end of the input
     * @return Boolean indicating if the current index is past the end of the
     * input vector of preprocessor tokens
     */
    inline bool atEnd() const;

    /**
     * Get the current preprocessor token
     * @return The preprocessor token at the `curIndex` position in `input`
     */
    PrepToken cur() const;

    /**
     * Read in one token
     * @param tok Reference to a token object where the found token will be
     * placed, if any is found
     * @return Boolean indicating if a token could be read
     */
    bool readInToken(Token &tok);

    /**
     * Get the last read token. This can be used to edit the last token
     * @warning Assumes that at least one token has been read!
     * @return A reference to the last token that was read
     */
    inline Token &lastRead() const;

    /**
     * Convert an identifier to a token, can be a simple word, namespace
     * definition, etc.
     * @param ident The identifier content as a string
     * @return The token associated to the identifier
     * @note May change the `readTokens` vector in order to account for
     * accumulation of tokens being joined
     */
    Token convertIdent(std::string ident);

    /**
     * Convert punctuation to a token, e.g. ;, ++, etc.
     * @param punct The punctation content as a string
     * @return The token associated to the punctuation
     */
    Token convertPunct(std::string punct);

    // Compiler component
    Compiler *comp;

    // Preprocessor tokens to be converted to lexer tokens
    const std::vector<PrepToken> &input;

    // Index of the current character being read
    unsigned int curIndex;

    // Vector of currently read tokens
    std::vector<Token> readTokens;

};

#endif