
#ifndef __PREPROCESS_H__
#define __PREPROCESS_H__

#include "preprocess/preplexer.h"
#include "preprocess/preptoken.h"
#include <fstream>
#include <string>
#include <vector>

#define MAX_INCLUDE_DEPTH 128

/**
 * Main preprocessor, uses the preprocessor lexer internaly
 */
class Preprocessor {

public:

    /**
     * Constructor
     */
    Preprocessor();

    /**
     * Destructor
     */
    ~Preprocessor();

    /**
     * Process a given file
     * @param filename The file to process
     * @post The output is appended in `out`, if needed
     * @note Processes included files recursively
     */
    void processFile(std::string filename);

    /**
     * Get the generated output
     * @return A reference to `out`
     */
    std::vector<PrepToken> &getOutput() const;

private:

    /**
     * Get the current top vector of token stack
     * @return Reference to last item in `tokenStack`
     */
    std::vector<PrepToken> &curTokenList() const;

    /**
     * Check if we are currently reading past the end of the current token list
     * @return Boolean indicating the result
     */
    bool atTokenListEnd() const;

    /**
     * Get the current top token iterator
     * @note Assumes that `atTokenListEnd()` returns false
     * @return The token currently being read
     */
    PrepToken curToken() const;

    /**
     * Move to the next item in the current token list
     * @post Index on the top of the `readIndexStack` has been incremented
     */
    void nextToken();

    /**
     * Read in and convert a program
     * Assumes that the `tokenStack` and `tokenIter` are not empty
     * @post The output tokens are put in `out`
     */
    void readProgram();

    // Current include depth: number of includes that are above the current file
    // being read
    unsigned int includeDepth;

    // Output switch
    bool doOutput;

    // Stack of vectors used to store lexed files
    std::vector<std::vector<PrepToken>> tokenStack;

    // Stack of indexes indicating the items on the token stack being currently
    // read
    std::vector<unsigned int> readIndexStack;

    // Output
    std::vector<PrepToken> out;

};

#endif