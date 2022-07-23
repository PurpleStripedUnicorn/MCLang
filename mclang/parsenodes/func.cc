
#include "bcgen/bcgen.h"
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
    // Initilize global variables at zero if they weren't already (by adding 0)
    man.write(BCInstr(INSTR_SET, "__zero", "0"));
    std::vector<std::string> globalVars;
    man.varManager.getVarNames(globalVars);
    for (unsigned int i = 0; i < globalVars.size(); i++)
        man.write(BCInstr(INSTR_ADD, globalVars[i], "__zero"));
    man.varManager.addContext();
    codeblock->bytecode(man);
    man.varManager.popContext();
    man.popFunc();
}