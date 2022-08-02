
#include "bcgen/bcgen.h"
#include "bcgen/instr.h"
#include "errorhandle/handle.h"
#include "general/loc.h"
#include "general/types.h"
#include "parsenodes/codeblock.h"
#include "parsenodes/exec.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

ExecNode::ExecNode(std::string type, ParseNode *args, CodeBlockNode *codeblock,
Loc loc) : ParseNode(PNODE_EXEC_STMT, loc), execType(type),
execArgs(args), codeblock(codeblock) {

}

std::vector<ParseNode *> ExecNode::children() const {
    return {(ParseNode *)codeblock};
}

ExecNode::~ExecNode() {
    delete codeblock;
}

void ExecNode::bytecode(BCManager &man) {
    man.addFunc();
    std::string fname = man.topFunc()->name;
    codeblock->bytecode(man);
    man.popFunc();
    execArgs->bytecode(man);
    if (man.ret.type != Type("const str"))
        MCLError(1, "Exec-statement input is not of type \"const str\".", loc);
    man.write(BCInstr(INSTR_EXEC_CALL, execType + " " + man.ret.value, fname));
    man.ret.type = Type("void");
    man.ret.value = "";
}