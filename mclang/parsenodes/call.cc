
#include "bcgen/bcgen.h"
#include "bcgen/context.h"
#include "bcgen/instr.h"
#include "errorhandle/handle.h"
#include "general/funcdef.h"
#include "general/loc.h"
#include "parsenodes/call.h"
#include "parsenodes/func.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

CallNode::CallNode(std::string fname, std::vector<ParseNode *> params, Loc loc)
: ParseNode(PNODE_CALL, loc), fname(fname), params(params) {

}

CallNode::~CallNode() {
    
}

std::vector<ParseNode *> CallNode::children() const {
    return params;
}

void CallNode::bytecode(BCManager &man) {
    pushLocalVars(man);
    bytecodeChildren(man);
    FuncNode *func = findFunc(man);
    if (func == nullptr)
        notFoundError();
    bytecodeConstParams(man, func);
    std::string callname = func->addGenerationEntry(man, getConstVals());
    man.write(BCInstr(INSTR_CALL, callname));
    popLocalVars(man);
    // TODO: Implement return types other than void
    man.ret = {Type("void"), ""};
}

FuncNode *CallNode::findFunc(BCManager &man) const {
    return man.funcs.find(fname, paramTypes);
}

void CallNode::pushLocalVars(BCManager &man) const {
    for (const Var &var : man.ctx.getLocalVars())
        man.write(BCInstr(INSTR_PUSH, var.name));
}

void CallNode::popLocalVars(BCManager &man) const {
    std::vector<Var> vars = man.ctx.getLocalVars();
    for (unsigned int i = 0; i < vars.size(); i++) {
        man.write(BCInstr(INSTR_TOP, vars[vars.size() - i - 1].name));
        man.write(BCInstr(INSTR_POP));
    }
}

void CallNode::bytecodeChildren(BCManager &man) {
    paramTypes.clear(), paramValues.clear();
    for (unsigned int i = 0; i < params.size(); i++) {
        params[i]->bytecode(man);
        if (!man.ret.type.isConst)
            man.write(BCInstr(INSTR_COPY, "__param" + std::to_string(i),
            man.ret.value));
        paramTypes.push_back(man.ret.type);
        paramValues.push_back(man.ret.value);
    }
}

void CallNode::bytecodeConstParams(BCManager &man, FuncNode *func) {
    // WARNING: Assumes that paramTypes, paramValues and funcParamTypes all have
    // the same size!
    std::vector<Type> funcParamTypes = func->getParamTypes();
    for (unsigned int i = 0; i < paramTypes.size(); i++)
        if (!funcParamTypes[i].isConst && paramTypes[i].isConst)
            man.write(BCInstr(INSTR_SET, "__param" + std::to_string(i),
            paramValues[i]));
}

void CallNode::notFoundError() {
    std::string errTxt = "Function \"" + fname + "\" with argument types (";
    bool first = true;
    for (const Type &ptype : paramTypes)
        errTxt.append((first ? "" : ", ") + ptype.str()), first = false;
    errTxt.append(") not defined");
    MCLError(1, errTxt, loc);
}

#include <iostream>
std::vector<std::string> CallNode::getConstVals() const {
    std::vector<std::string> out;
    for (unsigned int i = 0; i < paramTypes.size(); i++)
        if (paramTypes[i].isConst)
            out.push_back(paramValues[i]);
    return out;
}