
#ifndef __PREPROCESS_H__
#define __PREPROCESS_H__

#include "preprocess/preplexer.h"
#include "preprocess/preptoken.h"
#include <fstream>
#include <map>
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
    inline bool atEnd() const;

    /**
     * Get the current top token iterator
     * @note When past the end of file returns a `PTOK_EOF`
     * @return The token currently being read
     */
    inline PrepToken cur() const;

    /**
     * Move to the next item in the current token list
     * @post Index on the top of the `readIndexStack` has been incremented
     */
    inline void next();

    /**
     * Expect a certain type of token, when not matched an error is shown
     * @param type The type expected
     */
    void expect(PrepTokenType type);

    /**
     * Expect a certain token with both type and content matching, when not
     * matched an error is shown
     * @param type The type expected
     * @param content The token content expected
     */
    void expect(PrepTokenType type, std::string content);

    /**
     * Write a preprocessor token to the output, if `doOutput` is true
     * @param tok The token to write
     * @post The output token is put at the end of `out`
     */
    void write(PrepToken tok);

    /**
     * Read in and convert a program
     * Assumes that the `tokenStack` and `tokenIter` are not empty
     * @post The output tokens are put in `out`
     */
    void readProgram();

    /**
     * Read in a preprocessor code block, so this can be inside preprocessor
     * if-statements for example
     * @post The output tokens are put in `out`
     */
    void readCodeBlock();

    /**
     * Read in an identifier, this is handled separately because of `#define`
     * statements
     * @post The output tokens are put in `out`
     */
    void readIdent();

    /**
     * Read in a preprocessor statement
     * @post The output tokens are put in `out`
     */
    void readPrepStmt();

    /**
     * Read in an include preprocessor statement
     * @post The output tokens are put in `out`
     */
    void readInclude();

    /**
     * Convert an identifier if it has been defined using `#define`
     * @param tok The tokens which will be replaced if it is defined
     * @return A vector of preprocessor tokens that replaces the identifier
     */
    std::vector<PrepToken> convertIdent(PrepToken tok) const;

    /**
     * Read in a define preprocessor statement
     * @post The output tokens are put in `out`
     */
    void readDefine();

    /**
     * Get the file name of the current file being read
     * @return The file name as a string
     */
    inline std::string curFilename() const;

    // Current include depth: number of includes that are above the current file
    // being read
    unsigned int includeDepth;

    // Enable/disable writing to the output
    bool doOutput;

    // Currently tracked definitions
    std::map<std::string, std::vector<PrepToken>> defs;

    // Stack of vectors used to store lexed files
    std::vector<std::vector<PrepToken>> tokenStack;

    // Stack of indexes indicating the items on the token stack being currently
    // read
    std::vector<unsigned int> readIndexStack;

    // Output
    std::vector<PrepToken> out;

    // File currently being read
    std::string curFile;

};

#endif