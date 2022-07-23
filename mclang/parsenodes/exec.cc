
#include "bcgen/bcgen.h"
#include "bcgen/instr.h"
#include "parsenodes/codeblock.h"
#include "parsenodes/exec.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

ExecNode::ExecNode(std::string type, std::string args, CodeBlockNode *codeblock,
Loc loc) : ParseNode(PNODE_EXEC_STMT, loc), execType(type),
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