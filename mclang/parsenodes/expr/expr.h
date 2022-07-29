
#ifndef __PARSENODE_EXPR_H__
#define __PARSENODE_EXPR_H__

#include "general/loc.h"
#include "parsenodes/parsenode.h"
#include <vector>

class BCManager;

class ExprNode : public ParseNode {

public:

    /**
     * Constructor
     * @param type The type of the expression node
     * @param loc The location of the parse node
     * @param left The left child node of the expression, can be the only one
     * @param right The right child node of the expression, can be omitted for
     * no right child
     */
    ExprNode(ParseNodeType type, Loc loc,  ParseNode *left,
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
    virtual void bytecode(BCManager &man) = 0;

protected:

    // The children of the parse node
    ParseNode *left, *right;

};

#endif