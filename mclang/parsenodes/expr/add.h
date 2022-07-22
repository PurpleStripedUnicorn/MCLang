
#ifndef __PARSENODE_ADD_H__
#define __PARSENODE_ADD_H__

#include "parsenodes/expr/expr.h"
#include <vector>

class BCManager;

class AddNode : public ExprNode {

public:

    /**
     * Constructor
     * @param left The left child node of the expression
     * @param right The right child node of the expression
     * @param props General parse node properties
     */
    AddNode(ParseNode *left, ParseNode *right, ParseNodeProps props);

    /**
     * Destructor
     */
    virtual ~AddNode() override;

    /**
     * Generate bytecode for this parse node
     * @param man The main bytecode manager
     */
    virtual void bytecode(BCManager &man) const override;

};

#endif