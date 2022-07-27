
#include "bcgen/bcgen.h"
#include "errorhandle/handle.h"
#include "general/funcdef.h"
#include "general/loc.h"
#include "general/types.h"
#include "general/var.h"
#include "parsenodes/codeblock.h"
#include "parsenodes/func.h"
#include "parsenodes/parsenode.h"
#include <map>
#include <string>
#include <vector>

FuncNode::FuncNode(Type retType, std::string name, std::vector<Param> params,
CodeBlockNode *codeblock, Loc loc) : ParseNode(PNODE_FUNC, loc),
retType(retType), name(name), params(params), codeblock(codeblock) {

}

std::vector<ParseNode *> FuncNode::children() const {
    return {(ParseNode *)codeblock};
}

FuncNode::~FuncNode() {
    delete codeblock;
}

void FuncNode::bytecode(BCManager &man) {
    // TODO: Implement non-void functions
    if (retType != Type("void"))
        MCLError(1, "Non-void functions are not supported", loc);
    // TODO: Implement functions that accept constant values
    for (const Param &param : params)
        if (param.type.isConst)
            MCLError(1, "Function cannot take constant parameters.", loc);
    // Functions without params will receive original name, other will get
    // some random name
    if (params.empty())
        man.addFunc(name);
    else
        man.addFunc();
    // Add instructions to set constant variable values
    for (std::pair<std::string, std::string> constVal :
    man.ctx.getConstValues())
        man.write(BCInstr(INSTR_SET, constVal.first, constVal.second));
    // Get the input types
    std::vector<Type> paramTypes;
    for (const Param &param : params)
        paramTypes.push_back(param.type);
    FuncDef tmp;
    if (man.ctx.findFuncAll(name, paramTypes, tmp))
        MCLError(1, "Function with same name and parameter types was already "
        "defined", loc);
    // Add function definition to context
    man.ctx.addFunc(FuncDef(name, params));
    man.ctx.addFuncAlias(name, paramTypes, man.topFunc()->name);
    // Initilize global variables at zero if they weren't already (by adding 0)
    man.write(BCInstr(INSTR_SET, "__zero", "0"));
    std::vector<Var> globalVars = man.ctx.getGlobalVars();
    for (unsigned int i = 0; i < globalVars.size(); i++)
        man.write(BCInstr(INSTR_ADD, globalVars[i].name, "__zero"));
    man.ctx.pushContext(CTX_FUNC);
    // Add parameters to context and copy them to the correct variables
    for (unsigned int i = 0; i < params.size(); i++) {
        man.write(BCInstr(INSTR_COPY, params[i].name, "__param"
        + std::to_string(i)));
        Type tmp;
        if (man.ctx.findVar(params[i].name, tmp))
            MCLError(1, "Parameter name is already defined somewhere else.",
            loc);
        man.ctx.addVar(Var(params[i].type, params[i].name));
    }
    codeblock->bytecode(man);
    man.ctx.popContext();
    man.popFunc();
}