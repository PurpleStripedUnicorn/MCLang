
#ifndef __PARSENODE_H__
#define __PARSENODE_H__

#include "bcgen/bcgen.h"
#include <vector>

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
    PNODE_IF
};

/**
 * This type is used to store properties of parse nodes, without taking up many
 * function arguments
 */
struct ParseNodeProps {
    struct {
        unsigned int line = 0, col = 0;
    } loc;
};

class ParseNode {

public:

    /**
     * Constructor
     * @param type The node type
     */
    ParseNode(ParseNodeType type, ParseNodeProps props);

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

    // General parse node properties, such as location
    ParseNodeProps props;

protected:

    // Node type
    ParseNodeType type;

};

#endif