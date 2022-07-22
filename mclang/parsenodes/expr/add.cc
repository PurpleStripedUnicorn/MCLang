
#include "bcgen/bcgen.h"
#include "parsenodes/expr/add.h"
#include "parsenodes/expr/expr.h"
#include "parsenodes/parsenode.h"

AddNode::AddNode(ParseNode *left, ParseNode *right, ParseNodeProps props) :
ExprNode(PNODE_ADD, props, left, right) {

}

AddNode::~AddNode() {
    
}

void AddNode::bytecode(BCManager &man) const {
    // Puts the result in "__res"
    left->bytecode(man);
    man.varManager.addContext();
    std::string tmpVar = man.varManager.getUniqueVar();
    man.write(BCInstr(INSTR_COPY, tmpVar, "__res"));
    right->bytecode(man);
    man.write(BCInstr(INSTR_ADD, "__res", tmpVar));
    man.varManager.popContext();
}