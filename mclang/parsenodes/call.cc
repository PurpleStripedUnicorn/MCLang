
#include "bcgen/bcgen.h"
#include "bcgen/instr.h"
#include "errorhandle/handle.h"
#include "general/funcdef.h"
#include "general/loc.h"
#include "parsenodes/call.h"
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

void CallNode::bytecode(BCManager &man) const {
    // Check if function exists
    if (!man.hasFuncDef(fname))
        MCLError(1, "Called function not defined", loc);
    FuncDef fdef = man.getFuncDef(fname);
    // Put local variables on stack
    std::vector<std::string> varList;
    man.varManager.getVarNames(varList);
    for (unsigned int i = 0; i < varList.size(); i++)
        man.write(BCInstr(INSTR_PUSH, varList[i]));
    // TODO: Check parameter types, implement return types, etc...
    // Check if number of parameters matches (should later be replaced)
    if (fdef.params.size() != params.size())
        MCLError(1, "Called function with incorrect number of arguments", loc);
    // Evaluate child nodes and put values in parameter variables
    for (unsigned int i = 0; i < params.size(); i++) {
        params[i]->bytecode(man);
        man.write(BCInstr(INSTR_COPY, "__param" + std::to_string(i), "__res"));
    }
    // Actual function call
    man.write(BCInstr(INSTR_CALL, fname));
    // Take local variables off the stack
    if (varList.size() > 0) {
        for (unsigned int i = 0; i < varList.size(); i++) {
            man.write(BCInstr(INSTR_TOP, varList[varList.size() - i - 1]));
            man.write(BCInstr(INSTR_POP));
        }
    }
}