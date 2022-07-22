
#include "bcgen/bcgen.h"
#include "bcgen/instr.h"
#include "errorhandle/handle.h"
#include "parsenodes/expr/arith.h"
#include "parsenodes/expr/expr.h"
#include "parsenodes/parsenode.h"

ArithNode::ArithNode(ParseNodeType type, ParseNode *left, ParseNode *right, ParseNodeProps props) :
ExprNode(type, props, left, right) {

}

ArithNode::~ArithNode() {
    
}

void ArithNode::bytecode(BCManager &man) const {
    // Puts the result in "__res"
    left->bytecode(man);
    man.varManager.addContext();
    std::string tmpVar = man.varManager.getUniqueVar();
    man.write(BCInstr(INSTR_COPY, tmpVar, "__res"));
    right->bytecode(man);
    if (instrTypeTable.count(getType()) == 0)
        MCLError(1, "Unexpected error while converting arithmatic");
    BCInstrType instrType = instrTypeTable.find(getType())->second;
    man.write(BCInstr(instrType, "__res", tmpVar));
    man.varManager.popContext();
}