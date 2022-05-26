
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
    PNODE_CODEBLOCK
};

class ParseNode {

public:

    /**
     * Constructor
     * @param type The node type
     */
    ParseNode(ParseNodeType type);

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
     * Set the location of this node to the given line and column
     */
    void setLoc(unsigned int line, unsigned int col);

protected:

    // Node type
    ParseNodeType type;

    // Location of the node in the text
    struct {
        unsigned int line, col;
    } loc;

};

#endif