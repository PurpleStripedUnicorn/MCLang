
#ifndef __PREPLEXER_H__
#define __PREPLEXER_H__

#include "general/loc.h"
#include <map>
#include <set>
#include <string>
#include <vector>

struct PrepToken;

// NOTE: Arithmatic operators should also be in this list when implemented
// NOTE: For any symbols containing two or more characters, EVERY suffix of this
// symbol should also be a symbol!
const std::set<std::string> punctSymbols = {
    ";",
    "(", ")",
    "{", "}",
    "+", "+=",
    "-", "-=",
    "*", "*=",
    "/", "/=",
    "%", "%=",
    "=", "=="
};

// Table for escape characters that do not simply translate to themselves
const std::map<char, char> escapeChars = {
    {'n', '\n'},
    {'t', '\t'}
};

/**
 * Converts raw text into preprocessor tokens, ignores newlines if they are
 * preceded by "\"
 */
class PrepLexer {

public:

    /**
     * Constructor
     * @param input A pointer to the input string
     * @param filename The name of the file being read
     * @param out A pointer to a vector where the output should be put, is
     * cleared first
     * @note The pointer to the input string should not change between
     * construction and destruction of this object! The string contents should
     * also not change!
     */
    PrepLexer(const std::string &input, std::string filename,
    std::vector<PrepToken> &out);

    /**
     * Destructor
     */
    ~PrepLexer();

    /**
     * Run the lexer
     * @post The output is stored in `out`
     */
    void lex();

    /**
     * Get the lexer output
     * @return A reference to the output of the lexer
     */
    std::vector<PrepToken> &getOutput() const;

private:

    /**
     * Get the current character being read
     * @return The character at `input[curIndex]`
     * @note Returns ' ' if `atEnd()` is true
     */
    char cur() const;

    /**
     * Move to the next character
     * @post `curIndex` is incremented and line and column numbers are changed
     * accordingly
     */
    void next();

    /**
     * Ignore escaped line endings and '\r'
     * @note Does not use the `next()` function!
     */
    void ignoreEnds();

    /**
     * Check if the current index is past the length of the input string
     * @return Boolean indicating if `input.size() <= curIndex`
     */
    bool atEnd() const;

    /**
     * Read in a slash, which can become a command, a punctuation symbol, or a
     * comment (borth single-line and multi-line)
     * @post Location is changed accordingly
     */
    void readSlash();

    /**
     * Skip over a single-line comment, note that "//" has already been read
     * @post Location is changed accordingly
     */
    void readSingleComment();

    /**
     * Skip over a multi-line comment, note that "/""*" has already been read
     * @post Location is changed accordingly
     */
    void readMultiComment();

    /**
     * Read in a basic inserted command, note that the '/' should have already
     * been read
     * @post Location is changed accordingly
     */
    void readCmd();

    /**
     * Check if the current character is a letter, a number or an underscore
     * @return Boolean indicating the result
     */
    inline bool isAlphaNumUS() const;

    /**
     * Read in an identifier, which is a sequence of letters, numbers and
     * underscores, where the first character cannot be a number
     * @post Location is changed accordingly
     */
    void readIdent();

    /**
     * Read in a preprocessor statement
     * @post Location is changed accordingly
     */
    void readPrepStmt();

    /**
     * Read in a number
     * @post Location is changed accordingly
     */
    void readNumber();

    /**
     * Read in a string literal
     * @post Location is changed accordingly
     */
    void readString();

    /**
     * Read in an escaped character inside a string
     * @return The read escaped character, translated
     * @post Location is chanegd accordingly
     */
    char readEscapeChar();

    /**
     * Read in an include library, that is, using '<' and '>'
     * @post Location is changed accordingly
     */
    void readInclLib();

    /**
     * Check for punctuation symbols and read them in
     * @param prefix Punctuation symbols already read that should be taken into
     * account as well
     * @return A boolean indicating if a punctuation symbol was actually read
     * @post Location is changed accordingly
     */
    bool checkPunctSymbols(std::string prefix = "");

    // A pointer to the input string to read
    const std::string &input;

    // Current location and filename
    Loc loc;

    // The current index of the character being read
    unsigned int curIndex;

    // Pointer to where the output should be put
    std::vector<PrepToken> &out;

    // Are we at the start of a line (not counting whitespace)
    bool atLineStart;

};

#endif