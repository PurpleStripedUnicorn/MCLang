
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

void CallNode::bytecode(BCManager &man) {
    // Put local variables on stack
    std::vector<Var> varList = man.ctx.getLocalVars();
    for (unsigned int i = 0; i < varList.size(); i++)
        man.write(BCInstr(INSTR_PUSH, varList[i].name));
    // Evaluate child nodes and put values in parameter variables, also keep
    // track of output types of parameters
    std::vector<Type> inpTypes;
    for (unsigned int i = 0; i < params.size(); i++) {
        params[i]->bytecode(man);
        man.write(BCInstr(INSTR_COPY, "__param" + std::to_string(i),
        man.ret.value));
        inpTypes.push_back(man.ret.type);
    }
    FuncDef funcDef;
    // Check if function exists
    if (!man.ctx.findFuncAll(fname, inpTypes, funcDef)) {
        std::string errTxt = "Function \"" + fname + "\" with argument types (";
        for (unsigned int i = 0; i < inpTypes.size(); i++) {
            if (i != 0)
                errTxt.append(", ");
            errTxt.append(inpTypes[i].str());
        }
        errTxt.append(") not defined");
        MCLError(1, errTxt, loc);
    }
    // TODO: Implement return types other than void
    // TODO: Implement constant parameters
    // Actual function call
    if (funcDef.aliases.size() == 0)
        MCLError(1, "Unexpected error, could not generate function call "
        "bytecode.", loc);
    man.write(BCInstr(INSTR_CALL, funcDef.aliases[0].name));
    // Take local variables off the stack
    if (varList.size() > 0) {
        for (unsigned int i = 0; i < varList.size(); i++) {
            man.write(BCInstr(INSTR_TOP, varList[varList.size() - i - 1].name));
            man.write(BCInstr(INSTR_POP));
        }
    }
    man.ret.type = Type("void");
    man.ret.value = "";
}