
#ifndef __PARSER_H__
#define __PARSER_H__

#include "general/funcdef.h"
#include "general/types.h"
#include "general/var.h"
#include "lexer/token.h"
#include <vector>

class Compiler;
class ParseNode;

class Parser {

public:

    /**
     * Constructor
     * @param comp The main compiler component
     */
    Parser(Compiler *comp);

    /**
     * Destructor
     * @note Destroys the generated trees (if any exist)
     */
    ~Parser();

    /**
     * Construct a parse tree from the stored tokens
     * @post Parse tree output pointer is stored in `out` variable
     * @warning Do not delete output tree pointer! This will be deleted on
     * destruction of the parser object!
     */
    void genTree();

    /**
     * Get (a pointer to) the generated parse tree
     * @return A pointer to the generated parse tree
     * @note Tree has to be generated with the `genTree` function
     */
    ParseNode *getTree();

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
     * Move to the previous token
     * @post `curIndex` is decremented
     * @warning Does nothing if `curIndex` is equal to 0!
     */
    void prev();

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
     * Read in a function definition or the definition of a global variable
     * Both start the same, which is why this function exists
     * @return A pointer to the generated parse node
     */
    ParseNode *readInDef();

    /**
     * Read in the end of a global variable definition, after the variable type
     * and the variable name
     * @param type The typename that was already read
     * @param lastLoc The location of the typename token
     * @return A pointer to the generated parse node
     */
    ParseNode *readInGlobalVar(Type type, Loc lastLoc);

    /**
     * Read in the end of a function definition, after the return type and the
     * function name
     * @param type The return type that was already read
     * @param funcName The function name that was already read
     * @param lastLoc The location of the typename token
     * @return A pointer to the generated parse node
     */
    ParseNode *readInFunc(Type type, std::string funcName, Loc lastLoc);

    /**
     * Read in a function parameter, a type followed by a parameter name
     * @return A variable object for the parameter
     */
    Param readInFuncParam();

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

    /**
     * Read in a "line" of code, which can be a command, statement, etc.
     * @return A pointer to the generated parse node
     */
    ParseNode *readInLine();

    /**
     * Read in an execute-like statement
     * @return A pointer to the generated parse node
     */
    ParseNode *readInExec();

    /**
     * Read in an if-statement
     * @return A pointer to the generated parse node
     */
    ParseNode *readInIf();

    /**
     * Read in an expression
     * @return A pointer to the generated parse node
     */
    ParseNode *readInExpr();

    /**
     * Read in an assignment
     * @return A pointer to the genrated parse node
     * @note Can also just return an operation lower in the order of operators
     */
    ParseNode *readInAssign();

    /**
     * Read in a sum
     * @return A pointer to the genrated parse node
     * @note Can also just return an operation lower in the order of operators
     */
    ParseNode *readInSum();

    /**
     * Read in a product (also includes modulo)
     * @return A pointer to the genrated parse node
     * @note Can also just return an operation lower in the order of operators
     */
    ParseNode *readInProd();

    /**
     * Read in a term in a product, which can be a number, word, function call,
     * etc.
     * @return A pointer to the generated parse node
     */
    ParseNode *readInTerm();

    /**
     * Read in a function call, part of an expression
     * @return A pointer to the generated parse node
     * @note Can also just return an operation lower in the order of operators,
     * such as a word node, which is not listed separately. Number node is also
     * not listed separately
     */
    ParseNode *readInCall();

    /**
     * Read in a namespace statement
     * @return A pointer to the generated parse node
     */
    ParseNode *readInNamespace();

    /**
     * Read in a variable initialization
     * @return A pointer to the generated parse node
     */
    ParseNode *readInVarInit();

    /**
     * Read in a (variable/function) type, which can contain const
     */
    Type readInType();

    // Compiler component
    Compiler *comp;

    // Vector of input tokens
    const std::vector<Token> *toks;

    // Current index of the token being read
    unsigned int curIndex;

    // Pointer to output parse tree
    ParseNode *out;

};

#endif