
#ifndef __PARSENODE_ARITH_H__
#define __PARSENODE_ARITH_H__

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
 * Entry in the table defined below
 */
struct ArithTableEntry {
    BCInstrType instrType, instrTypeI;
    std::string shortName;
};

// Table to find which instruction should be used per type of arithmatic node
const std::map<ParseNodeType, ArithTableEntry> arithTable = {
    {PNODE_ADD, {INSTR_ADD, INSTR_ADDI, "+"}},
    {PNODE_SUB, {INSTR_SUB, INSTR_SUBI, "-"}},
    {PNODE_MUL, {INSTR_MUL, INSTR_MULI, "*"}},
    {PNODE_DIV, {INSTR_DIV, INSTR_DIVI, "/"}},
    {PNODE_MOD, {INSTR_MOD, INSTR_MODI, "%"}}
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
     * Generate bytecode for this parse node, given the left expression returns
     * an "int"
     * @param man The main bytecode manager
     * @note Uses the `man.ret` member variable
     */
    void bytecodeInt(BCManager &man);

    /**
     * Generate bytecode for this parse node, given the left expression returns
     * a "const int"
     * @param man The main bytecode manager
     * @note Uses the `man.ret` member variable
     */
    void bytecodeConstInt(BCManager &man);

    /**
     * Generate bytecode for this parse node, given the left expression returns
     * a "const str"
     * @param man The main bytecode manager
     * @note Uses the `man.ret` member variable
     */
    void bytecodeConstStr(BCManager &man);

    /**
     * Evaluate the result of two constant integer inputs
     * @param numLeft Left value as a string
     * @param numRight Right value as a string
     * @return The resulting value, as a string
     */
    std::string evalConsts(std::string numLeft, std::string numRight) const;

};

#endif