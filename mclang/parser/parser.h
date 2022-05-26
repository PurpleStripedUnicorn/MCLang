
#ifndef __PARSER_H__
#define __PARSER_H__

#include "lexer/token.h"
#include "parsenodes/cmd.h"
#include "parsenodes/codeblock.h"
#include "parsenodes/func.h"
#include "parsenodes/parsenode.h"
#include "parsenodes/program.h"
#include <iostream>
#include <string>
#include <vector>

class Parser {

public:

    /**
     * Constructor
     * @param toks Vector of input tokens
     */
    Parser(const std::vector<Token> &toks);

    /**
     * Destructor
     * @note Destroys the generated trees (if any exist)
     */
    ~Parser();

    /**
     * Construct a parse tree from the stored tokens
     * @return A pointer to the generated parse tree root node
     * @post Parse tree output pointer is stored in `out` variable
     * @warning Do not delete output tree pointer! This will be deleted on
     * destruction of the parser object!
     */
    ParseNode *genTree();

private:

    /**
     * Get the current token being read
     * @return The token in `toks[curIndex]`
     */
    Token cur() const;

    /**
     * Move to the next token
     * @post `curIndex` is incremented
     */
    void next();

    /**
     * Check if the current token is of the given type
     * @param type The token type
     * @return Boolean indicating if the types match
     */
    bool accept(TokenType type) const;

    /**
     * Indicate that a certain token type is expected, shows an error if the
     * type does not match with the current token type
     * @param type The token type
     */
    void expect(TokenType type);

    /**
     * Read in an entire program
     * @return A pointer to the generated parse node
     */
    ParseNode *readInProgram();

    /**
     * Read in a function definition
     * @return A pointer to the generated parse node
     */
    ParseNode *readInFunc();

    /**
     * Read in a code block
     * @return A pointer to the generated parse node
     */
    ParseNode *readInCodeBlock();

    /**
     * Read in an inserted command
     * @return A pointer to the generated parse node
     */
    ParseNode *readInCmd();

    // Vector of input tokens
    const std::vector<Token> &toks;

    // Current index of the token being read
    unsigned int curIndex;

    // Pointer to output parse tree
    ParseNode *out;

};

#endif