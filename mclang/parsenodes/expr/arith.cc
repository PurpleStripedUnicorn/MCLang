
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

void ArithNode::bytecode(BCManager &man) {
    right->bytecode(man);
    retLeft = man.ret;
    if (man.ret.type != Type("int"))
        MCLError(1, "Arithmatic not allowed on non-int type", loc);
    man.ctx.pushContext(CTX_BASIC);
    Var tmpVar = man.ctx.makeUniqueVar(Type("int"));
    man.write(BCInstr(INSTR_COPY, tmpVar.name, man.ret.value));
    left->bytecode(man);
    if (man.ret.type != Type("int"))
        MCLError(1, "Arithmatic not allowed on non-int type", loc);
    if (arithTable.count(getType()) == 0)
        MCLError(1, "Unexpected error while converting arithmatic");
    BCInstrType instrType = arithTable.find(getType())->second.instrType;
    man.write(BCInstr(instrType, man.ret.value, tmpVar.name));
    man.ctx.popContext();
    man.ret.type = Type("int");
}

void ArithNode::invalidTypeError() const {
    
}

void ArithNode::bytecodeInt(BCManager &man) {

}

void ArithNode::bytecodeConstInt(BCManager &man) {
    
}