
#include "bcgen/bcgen.h"
#include "bcgen/instr.h"
#include "parsenodes/call.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

CallNode::CallNode(std::string fname, ParseNodeProps props) :
ParseNode(PNODE_CALL, props), fname(fname) {

}

CallNode::~CallNode() {
    
}

std::vector<ParseNode *> CallNode::children() const {
    return {};
}

void CallNode::bytecode(BCManager &man) const {
    // TODO: Add check if given function actually exists, implement function
    // arguments, etc.
    std::vector<std::string> varList;
    man.varManager.getVarNames(varList);
    for (unsigned int i = 0; i < varList.size(); i++)
        man.write(BCInstr(INSTR_PUSH, varList[i]));
    man.write(BCInstr(INSTR_CALL, fname));
    if (varList.size() > 0) {
        for (unsigned int i = 0; i < varList.size(); i++) {
            man.write(BCInstr(INSTR_TOP, varList[varList.size() - i - 1]));
            man.write(BCInstr(INSTR_POP));
        }
    }
}