
#include "bcgen/bcgen.h"
#include "errorhandle/handle.h"
#include "general/funcdef.h"
#include "general/loc.h"
#include "general/var.h"
#include "parsenodes/codeblock.h"
#include "parsenodes/func.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

FuncNode::FuncNode(std::string name, std::vector<Var> params,
CodeBlockNode *codeblock, Loc loc) : ParseNode(PNODE_FUNC, loc), name(name),
params(params), codeblock(codeblock) {

}

std::vector<ParseNode *> FuncNode::children() const {
    return {(ParseNode *)codeblock};
}

FuncNode::~FuncNode() {
    delete codeblock;
}

void FuncNode::bytecode(BCManager &man) const {
    man.addFunc(name);
    // TODO: Change this later to allow functions with the same name
    if (man.hasFuncDef(name))
        MCLError(1, "Function with name \"" + name + "\" was already defined",
        loc);
    man.addFuncDef(FuncDef(name, params));
    // Initilize global variables at zero if they weren't already (by adding 0)
    man.write(BCInstr(INSTR_SET, "__zero", "0"));
    std::vector<std::string> globalVars;
    man.varManager.getVarNames(globalVars);
    for (unsigned int i = 0; i < globalVars.size(); i++)
        man.write(BCInstr(INSTR_ADD, globalVars[i], "__zero"));
    man.varManager.addContext();
    // Add parameters to context and copy them to the correct variables
    for (unsigned int i = 0; i < params.size(); i++) {
        man.write(BCInstr(INSTR_COPY, params[i].name, "__param"
        + std::to_string(i)));
        if (man.varManager.hasVar(params[i].name))
            MCLError(1, "Parameter name is already defined somewhere else.",
            loc);
        man.varManager.addVar(params[i].name);
    }
    codeblock->bytecode(man);
    man.varManager.popContext();
    man.popFunc();
}