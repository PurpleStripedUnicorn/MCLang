
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
    if (arithTable.count(getType()) == 0)
        MCLError(1, "Unexpected error while converting arithmatic");
    left->bytecode(man);
    retLeft = man.ret;
    man.ctx.pushContext(CTX_BASIC);
    if (man.ret.type == Type("int"))
        bytecodeInt(man);
    else if (man.ret.type == Type("const int"))
        bytecodeConstInt(man);
    else if (man.ret.type == Type("const str"))
        bytecodeConstStr(man);
    else
        invalidTypeError();
    man.ctx.popContext();
}

void ArithNode::invalidTypeError() const {
    if (arithTable.count(getType()) == 0)
        MCLError(1, "Unexpected error while converting arithmatic");
    std::string shortName = arithTable.find(getType())->second.shortName;
    MCLError(1, "Operation \"" + shortName + "\" does not support operands of "
    "types \"" + retLeft.type.str() + "\" and \"" + retRight.type.str() + "\".",
    loc);
}

void ArithNode::bytecodeInt(BCManager &man) {
    Var tmpVar = man.ctx.makeUniqueVar(Type("int"));
    man.write(BCInstr(INSTR_COPY, tmpVar.name, man.ret.value));
    right->bytecode(man);
    retRight = man.ret;
    if (man.ret.type == Type("int")) {
        BCInstrType instrType = arithTable.find(getType())->second.instrType;
        man.write(BCInstr(instrType, man.ret.value, tmpVar.name));
        man.ret = {Type("int"), man.ret.value};
    } else if (man.ret.type == Type("const int")) {
        BCInstrType instrType = arithTable.find(getType())->second.instrTypeI;
        man.write(BCInstr(instrType, tmpVar.name, man.ret.value));
        man.ret = {Type("int"), tmpVar.name};
    } else {
        invalidTypeError();
    }
}

void ArithNode::bytecodeConstInt(BCManager &man) {
    right->bytecode(man);
    retRight = man.ret;
    if (man.ret.type == Type("int")) {
        BCInstrType instrType = arithTable.find(getType())->second.instrTypeI;
        man.write(BCInstr(instrType, man.ret.value, retRight.value));
        man.ret = {Type("int"), man.ret.value};
    } else if (man.ret.type == Type("const int")) {
        std::string val = std::to_string(std::stoi(retLeft.value)
        + std::stoi(man.ret.value));
        man.ret = {Type("const int"), val};
    } else {
        invalidTypeError();
    }
}

void ArithNode::bytecodeConstStr(BCManager &man) {
    right->bytecode(man);
    retRight = man.ret;
    if (man.ret.type == Type("const str")) {
        std::string val = retLeft.value + man.ret.value;
        man.ret = {Type("const str"), val};
    } else {
        invalidTypeError();
    }
}