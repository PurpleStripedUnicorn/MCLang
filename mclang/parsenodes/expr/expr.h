
#ifndef __PARSENODE_EXPR_H__
#define __PARSENODE_EXPR_H__

#include "parsenodes/parsenode.h"
#include <vector>

class BCManager;

class ExprNode : public ParseNode {

public:

    /**
     * Constructor
     * @param type The type of the expression node
     * @param left The left child node of the expression, can be the only one
     * @param right The right child node of the expression, can be omitted for
     * no right child
     * @param props General parse node properties
     */
    ExprNode(ParseNodeType type, ParseNodeProps props,  ParseNode *left,
    ParseNode *right = NULL);

    /**
     * Destructor
     */
    virtual ~ExprNode() override;

    /**
     * Get the children of this parse node
     * @return A vector with pointers to the child nodes
     */
    virtual std::vector<ParseNode *> children() const override;

    /**
     * Generate bytecode for this parse node
     * @param man The main bytecode manager
     */
    virtual void bytecode(BCManager &man) const = 0;

protected:

    // The children of the parse node
    ParseNode *left, *right;

};

#endif