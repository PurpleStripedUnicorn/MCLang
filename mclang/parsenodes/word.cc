
#include "bcgen/bcgen.h"
#include "errorhandle/handle.h"
#include "general/loc.h"
#include "parsenodes/parsenode.h"
#include "parsenodes/word.h"
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
    // TODO: implement const vars, etc.
    if (varType != Type("int") && varType != Type("bool"))
        MCLError(1, "Accessing inaccessible variable type \"" + varType.str()
        + "\".", loc);
    // Return the variable value to "__res"
    man.write(BCInstr(INSTR_COPY, "__res", content));
    man.ret.type = varType;
    man.ret.value = "__res";
}

std::string WordNode::getContent() const {
    return content;
}