
#include "bcgen/bcgen.h"
#include "errorhandle/handle.h"
#include "general/loc.h"
#include "parsenodes/parsenode.h"
#include "parsenodes/word.h"
#include <map>
#include <string>
#include <vector>

WordNode::WordNode(std::string content, Loc loc) :
ParseNode(PNODE_WORD, loc), content(content) {

}

WordNode::~WordNode() {

}

std::vector<ParseNode *> WordNode::children() const {
    return {};
}

void WordNode::bytecode(BCManager &man) {
    Type varType;
    if (!wasInitialized(man, varType))
        MCLError(1, "Accessing uninitialized variable \"" + content + "\"",
        loc);
    if (varType == Type("int") || varType == Type("bool")) {
        // Return the variable value to "0res"
        man.write(BCInstr(INSTR_COPY, "0res", content));
        man.ret.type = varType;
        man.ret.value = "0res";
    } else if (varType == Type("const int") || varType == Type("const bool")
    || varType == Type("const str")) {
        man.ret.type = varType;
        man.ret.value = findConstValue(man);
    } else {
        MCLError(1, "Accessing inaccessible variable type \"" + varType.str()
        + "\".", loc);
    }
}

std::string WordNode::getContent() const {
    return content;
}

std::string WordNode::findConstValue(BCManager &man) const {
    return man.ctx.getConstValue(content);
}

bool WordNode::wasInitialized(BCManager &man, Type &varType) const {
    Var var(Type("int"), "??");
    bool result = man.ctx.findVar(content, var);
    if (!result)
        return false;
    varType = var.type;
    return true;
}