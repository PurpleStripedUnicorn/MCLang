
#ifndef __PREPLEXER_H__
#define __PREPLEXER_H__

#include <string>
#include <vector>

struct PrepToken;

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
    PrepLexer(std::string &input, std::string filename,
    std::vector<PrepToken> *out);

    /**
     * Destructor
     */
    ~PrepLexer();

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

private:

    // A pointer to the input string to read
    std::string &input;

    // Current location and filename
    struct {
        std::string filename;
        unsigned int line, col;
    } loc;

    // The current index of the character being read
    unsigned int curIndex;

    // Pointer to where the output should be put
    std::vector<PrepToken> *out;

};

#endif