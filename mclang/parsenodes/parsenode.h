
#ifndef __PARSENODE_H__
#define __PARSENODE_H__

#include "general/loc.h"
#include <vector>

class BCManager;

enum ParseNodeType {
    // Error type
    PNODE_ERRTYPE,
    // Main program node
    PNODE_PROGRAM,
    // Function definition
    PNODE_FUNC,
    // Inserted commands
    PNODE_CMD,
    // Blocks of code, to group commands
    PNODE_CODEBLOCK,
    // Execute-statement
    PNODE_EXEC_STMT,
    // If-statement
    PNODE_IF,
    // Function call
    PNODE_CALL,
    // Namespace setting
    PNODE_NAMESPACE,
    // Expressions
    PNODE_ADD, PNODE_SUB, PNODE_MUL, PNODE_DIV, PNODE_MOD, PNODE_ASSIGN_ADD,
    PNODE_ASSIGN_SUB, PNODE_ASSIGN_MUL, PNODE_ASSIGN_DIV, PNODE_ASSIGN_MOD,
    PNODE_ASSIGN,
    // Variable initialization
    PNODE_VARINIT,
    // Global variable
    PNODE_GLOBALVAR,
    // Variable name/word
    PNODE_WORD, PNODE_NUM
};

class ParseNode {

public:

    /**
     * Constructor
     * @param type The node type
     * @param loc The location of the parse node
     */
    ParseNode(ParseNodeType type, Loc loc);

    /**
     * Destructor
     */
    virtual ~ParseNode();

    /**
     * Get the children of this parse node
     * @return A vector with pointers to the child nodes
     */
    virtual std::vector<ParseNode *> children() const = 0;

    /**
     * Get the type of the parse node
     * @return The type of this parse node
     */
    ParseNodeType getType() const;

    /**
     * Generate bytecode for this parse node
     * @param man The main bytecode manager
     */
    virtual void bytecode(BCManager &man) const = 0;

    /**
     * Get the location of the parse node
     * @return The location as a "Loc" type
     */
    Loc getLoc() const;

protected:

    // Node type
    ParseNodeType type;

    // The location of the parse node
    Loc loc;

};

#endif