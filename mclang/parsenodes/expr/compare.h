
#ifndef __PARSENODE_COMPARE_H__
#define __PARSENODE_COMPARE_H__

#include "bcgen/return.h"
#include "general/loc.h"
#include "parsenodes/expr/expr.h"
#include "parsenodes/parsenode.h"

class BCManager;

/**
 * Comparison parse node, for <, <=, >, >= and ==
 */
class CompareNode : public ExprNode {

public:

    /**
     * Constructor
     * @param type The logic node type
     * @param left The left child node of the expression
     * @param right The right child node of the expression
     * @param loc The location of the parse node
     */
    CompareNode(ParseNodeType type, ParseNode *left, ParseNode *right, Loc loc);

    /**
     * Destructor
     */
    virtual ~CompareNode() override;

    /**
     * Generate bytecode for this parse node
     * @param man The main bytecode manager
     */
    virtual void bytecode(BCManager &man) override;

private:

    // Store the returns for showing error messages
    Return retLeft, retRight;

    /**
     * Throw an error for unsupported return types
     * @note Assumes `retLeft` and `retRight` are correct
     */
    void invalidTypeError() const;

    /**
     * Get the bytecode instruction that belongs to this comparison node
     * @return The bytecode instruction type
     */
    BCInstrType getInstrType() const;

};

#endif