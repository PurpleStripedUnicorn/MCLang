
#include "parsenodes/exec.h"

ExecNode::ExecNode(std::string type, std::string args, CodeBlockNode *codeblock,
ParseNodeProps props) : ParseNode(PNODE_EXEC_STMT, props), execType(type),
execArgs(args), codeblock(codeblock) {

}

std::vector<ParseNode *> ExecNode::children() const {
    return {(ParseNode *)codeblock};
}

ExecNode::~ExecNode() {
    delete codeblock;
}

void ExecNode::bytecode(BCManager &man) const {
    man.addFunc();
    std::string fname = man.topFunc()->name;
    codeblock->bytecode(man);
    man.popFunc();
    man.write(BCInstr(INSTR_EXEC_CALL, execType + " " + execArgs, fname));
}