
#include "bcgen/bcgen.h"
#include "bcgen/context.h"
#include "bcgen/instr.h"
#include "errorhandle/handle.h"
#include "general/loc.h"
#include "parsenodes/expr/arith.h"
#include "parsenodes/expr/expr.h"
#include "parsenodes/parsenode.h"

ArithNode::ArithNode(ParseNodeType type, ParseNode *left, ParseNode *right,
Loc loc) : ExprNode(type, loc, left, right) {

}

ArithNode::~ArithNode() {
    
}

void ArithNode::bytecode(BCManager &man) const {
    Return retLeft, retRight;
    right->bytecode(man);
    if (man.ret.type != Type::tInt)
        MCLError(1, "Arithmatic not allowed on non-int type", loc);
    man.ctx.pushContext(CTX_BASIC);
    Var tmpVar = man.ctx.makeUniqueVar(Type::tInt);
    man.write(BCInstr(INSTR_COPY, tmpVar.name, man.ret.value));
    left->bytecode(man);
    if (man.ret.type != Type::tInt)
        MCLError(1, "Arithmatic not allowed on non-int type", loc);
    if (instrTypeTable.count(getType()) == 0)
        MCLError(1, "Unexpected error while converting arithmatic");
    BCInstrType instrType = instrTypeTable.find(getType())->second;
    man.write(BCInstr(instrType, man.ret.value, tmpVar.name));
    man.ctx.popContext();
    man.ret.type = Type::tInt;
}