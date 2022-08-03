
#ifndef __PARSENODE_LOGIC_H__
#define __PARSENODE_LOGIC_H__

#include "bcgen/instr.h"
#include "bcgen/return.h"
#include "general/loc.h"
#include "parsenodes/expr/expr.h"
#include "parsenodes/parsenode.h"
#include <map>
#include <string>
#include <vector>

class BCManager;

/**
 * Arithmatic parse node, which is used for all arithmatic operations, such as
 * addition, modulo, etc.
 * @note This is not used when using for example "+="
 */
class LogicNode : public ExprNode {

public:

    /**
     * Constructor
     * @param type The logic node type
     * @param left The left child node of the expression
     * @param right The right child node of the expression, should be null for a
     * NOT node
     * @param loc The location of the parse node
     */
    LogicNode(ParseNodeType type, ParseNode *left, ParseNode *right, Loc loc);

    /**
     * Destructor
     */
    virtual ~LogicNode() override;

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
     * Generate bytecode for this parsenode assuming it is a NOT operator
     * @param man The main bytecode manager
     */
    void bytecodeNot(BCManager &man);

    /**
     * Generate bytecode for this parsenode assuming it is an AND or OR operator
     * @param man The main bytecode manager
     */
    void bytecodeAndOr(BCManager &man);

    /**
     * Generate bytecode for this parsenode assuming it is an AND operator
     * @param man The main bytecode manager
     */
    void bytecodeAnd(BCManager &man);

    /**
     * Generate bytecode for this parsenode assuming it is an OR operator
     * @param man The main bytecode manager
     */
    void bytecodeOr(BCManager &man);

};

#endif