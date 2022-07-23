
#include "bcgen/bcgen.h"
#include "bcgen/instr.h"
#include "compiler/compiler.h"
#include "parsenodes/codeblock.h"
#include "parsenodes/if.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

IfNode::IfNode(std::vector<std::string> args, std::vector<CodeBlockNode *>
codeblocks, Loc loc) : ParseNode(PNODE_IF, loc), ifArgs(args),
codeblocks(codeblocks) {

}

std::vector<ParseNode *> IfNode::children() const {
    std::vector<ParseNode *> out;
    for (unsigned int i = 0; i < codeblocks.size(); i++)
        out.push_back(codeblocks[i]);
    return out;
}

IfNode::~IfNode() {
    for (unsigned int i = 0; i < codeblocks.size(); i++)
        delete codeblocks[i];
}

void IfNode::bytecode(BCManager &man) const {
    std::string tmpId = man.varManager.getUniqueVar();
    man.write(BCInstr(INSTR_SET, tmpId, "0"));
    for (unsigned int i = 0; i < codeblocks.size(); i++) {
        man.addFunc();
        std::string fname = man.topFunc()->name;
        codeblocks[i]->bytecode(man);
        man.write(BCInstr(INSTR_SET, tmpId, "1"));
        man.popFunc();
        std::string cond = "";
        if (i < ifArgs.size())
            cond.append("if " + ifArgs[i] + " ");
        cond.append("if score " + tmpId + " " + man.comp->scoreboardName
        + " matches 0");
        man.write(BCInstr(INSTR_EXEC_CALL, cond, fname));
    }
}

bool IfNode::hasElse() const {
    return ifArgs.size() < codeblocks.size();
}