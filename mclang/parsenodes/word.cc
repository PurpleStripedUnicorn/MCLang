
#include "bcgen/bcgen.h"
#include "errorhandle/handle.h"
#include "parsenodes/parsenode.h"
#include "parsenodes/word.h"
#include <string>
#include <vector>

WordNode::WordNode(std::string content, ParseNodeProps props) :
ParseNode(PNODE_WORD, props) {

}

WordNode::~WordNode() {

}

std::vector<ParseNode *> WordNode::children() const {
    return {};
}

void WordNode::bytecode(BCManager &man) const {
    // Check if the variable is actually defined
    if (!man.varManager.hasVar(content))
        MCLError(1, "Accessing uninitialized variable \"" + content + "\"",
        props.loc.line, props.loc.col);
    // Return the variable value to "__res"
    man.write(BCInstr(INSTR_COPY, "__res", content));
}

std::string WordNode::getContent() const {
    return content;
}