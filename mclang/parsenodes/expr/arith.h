
#ifndef __PARSENODE_ARITH_H__
#define __PARSENODE_ARITH_H__

#include "bcgen/instr.h"
#include "general/loc.h"
#include "parsenodes/expr/expr.h"
#include "parsenodes/parsenode.h"
#include <map>
#include <vector>

class BCManager;

// Table to find which instruction should be used per type of arithmatic node
const std::map<ParseNodeType, BCInstrType> instrTypeTable = {
    {PNODE_ADD, INSTR_ADD},
    {PNODE_SUB, INSTR_SUB},
    {PNODE_MUL, INSTR_MUL},
    {PNODE_DIV, INSTR_DIV},
    {PNODE_MOD, INSTR_MOD}
};

/**
 * Arithmatic parse node, which is used for all arithmatic operations, such as
 * addition, modulo, etc.
 * @note This is not used when using for example "+="
 */
class ArithNode : public ExprNode {

public:

    /**
     * Constructor
     * @param type The arithmatic node type
     * @param left The left child node of the expression
     * @param right The right child node of the expression
     * @param loc The location of the parse node
     */
    ArithNode(ParseNodeType type, ParseNode *left, ParseNode *right, Loc loc);

    /**
     * Destructor
     */
    virtual ~ArithNode() override;

    /**
     * Generate bytecode for this parse node
     * @param man The main bytecode manager
     */
    virtual void bytecode(BCManager &man) const override;

};

#endif