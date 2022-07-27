
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
    if (!man.ctx.findVarAll(content, varType))
        MCLError(1, "Accessing uninitialized variable \"" + content + "\"",
        loc);
    if (varType == Type("int") || varType == Type("bool")) {
        // Return the variable value to "__res"
        man.write(BCInstr(INSTR_COPY, "__res", content));
        man.ret.type = varType;
        man.ret.value = "__res";
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
    for (const std::pair<std::string, std::string> &val :
    man.ctx.getConstValues())
        if (val.first == content)
            return val.second;
    return "";
}