
#include "bcgen/bcgen.h"
#include "errorhandle/handle.h"
#include "general/loc.h"
#include "general/types.h"
#include "parsenodes/expr/assign.h"
#include "parsenodes/parsenode.h"
#include "parsenodes/globalvar.h"
#include "parsenodes/word.h"
#include <string>

GlobalVarNode::GlobalVarNode(Type varType, ParseNode *childExpr, Loc loc)
: ParseNode(PNODE_GLOBALVAR, loc), varType(varType),
childExpr(childExpr) {

}

GlobalVarNode::~GlobalVarNode() {

}

std::vector<ParseNode *> GlobalVarNode::children() const {
    return {};
}

void GlobalVarNode::bytecode(BCManager &man) {
    // Get the global variable name
    std::string varName;
    if (varType.isConst) {
        if (childExpr->getType() != PNODE_ASSIGN)
            MCLError(1, "Constant global variable needs to be assigned.", loc);
        varName = ((AssignNode *)childExpr)->getVarName();
    } else {
        if (childExpr->getType() != PNODE_WORD)
            MCLError(1, "Expected global variable name without assignment "
            "value.", loc);
        varName = ((WordNode *)childExpr)->getContent();
    }
    // Check if the global variable name isn't already in use
    Type tmp;
    if (hasNameConflict(varName, man))
        MCLError(1, "Global variable \"" + varName + "\" already defined", loc);
    // Set constant variable value
    if (varType.isConst) {
        ((AssignNode *)childExpr)->getExpr()->bytecode(man);
        if (man.ret.type != varType)
            MCLError(1, "Assigning value of type \"" + man.ret.type.str()
            + "\" to global constant of type \"" + varType.str() + "\"", loc);
        // Return value contains the constant value here
        man.ctx.back().constValues.insert({varName, man.ret.value});
    }
    // Remember that this variable is now registered
    man.ctx.back().vars.push_back(Var(varType, varName));
    man.ret.type = Type("void");
    man.ret.value = "";
}

bool GlobalVarNode::hasNameConflict(std::string varName, BCManager &man) const {
    for (const Context &ctx : man.ctx)
        for (const Var &var : ctx.vars)
            if (var.name == varName)
                return true;
    return false;
}