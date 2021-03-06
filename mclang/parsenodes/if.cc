
#include "bcgen/bcgen.h"
#include "bcgen/instr.h"
#include "compiler/compiler.h"
#include "errorhandle/handle.h"
#include "general/loc.h"
#include "general/types.h"
#include "parsenodes/codeblock.h"
#include "parsenodes/if.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

IfNode::IfNode(std::vector<ParseNode *> args, std::vector<CodeBlockNode *>
codeblocks, Loc loc) : ParseNode(PNODE_IF, loc), ifArgs(args),
codeblocks(codeblocks) {

}

std::vector<ParseNode *> IfNode::children() const {
    std::vector<ParseNode *> out = ifArgs;
    for (unsigned int i = 0; i < codeblocks.size(); i++)
        out.push_back(codeblocks[i]);
    return out;
}

IfNode::~IfNode() {
    for (unsigned int i = 0; i < codeblocks.size(); i++)
        delete codeblocks[i];
}

void IfNode::bytecode(BCManager &man) {
    std::string tmpId = man.tmp.reserve();
    man.write(BCInstr(INSTR_SET, tmpId, "0"));
    for (unsigned int i = 0; i < codeblocks.size(); i++) {
        man.addFunc();
        std::string fname = man.topFunc()->name;
        codeblocks[i]->bytecode(man);
        man.write(BCInstr(INSTR_SET, tmpId, "1"));
        man.popFunc();
        std::string cond = "";
        if (i < ifArgs.size()) {
            ifArgs[i]->bytecode(man);
            if (man.ret.type != Type("const str"))
                MCLError(1, "If-statement input is not of type \"const str\".",
                loc);
            cond.append("if " + man.ret.value + " ");
        }
        cond.append("if score " + tmpId + " " + man.comp->scoreboardName
        + " matches 0");
        man.write(BCInstr(INSTR_EXEC_CALL, cond, fname));
    }
    man.tmp.free(tmpId);
    man.ret.type = Type("void");
    man.ret.value = "";
}

bool IfNode::hasElse() const {
    return ifArgs.size() < codeblocks.size();
}