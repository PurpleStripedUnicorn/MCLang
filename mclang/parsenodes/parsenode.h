
#ifndef __PARSENODE_H__
#define __PARSENODE_H__

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

protected:

    // Node type
    ParseNodeType type;

};

#endif