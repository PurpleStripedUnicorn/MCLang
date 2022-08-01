
#include "bcgen/bcgen.h"
#include "compiler/compiler.h"
#include "errorhandle/handle.h"
#include "general/loc.h"
#include "general/types.h"
#include "parsenodes/namespace.h"
#include "parsenodes/parsenode.h"
#include "parsenodes/program.h"
#include <vector>

ProgramNode::ProgramNode(std::vector<ParseNode *> childNodes, Loc loc) :
ParseNode(PNODE_PROGRAM, loc), childNodes(childNodes)
{

}

ProgramNode::~ProgramNode() {
    for (unsigned int i = 0; i < childNodes.size(); i++)
        delete childNodes[i];
}

std::vector<ParseNode *> ProgramNode::children() const {
    return childNodes;
}

void ProgramNode::bytecode(BCManager &man) {
    applyGlobalSettings(man);
    registerGlobalVars(man);
    registerFunctions(man);
    man.ret = {Type("void"), ""};
    man.ret.type = Type("void");
    man.ret.value = "";
}

void ProgramNode::applyGlobalSettings(BCManager &man) const {
    // Search for namespace setting override
    bool foundNamespace = false;
    for (unsigned int i = 0; i < childNodes.size(); i++) {
        if (childNodes[i]->getType() == PNODE_NAMESPACE) {
            if (foundNamespace)
                MCLError(0, "Namespace set multiple times, only last setting "
                "is used", childNodes[i]->getLoc());
            man.comp->ns = ((NSNode *)childNodes[i])->getName();
            foundNamespace = true;
        }
    }
}

void ProgramNode::registerGlobalVars(BCManager &man) const {
    for (ParseNode *node : childNodes)
        if (node->getType() == PNODE_GLOBALVAR)
            node->bytecode(man);
}

void ProgramNode::registerFunctions(BCManager &man) const {
    for (ParseNode *node : childNodes)
        if (node->getType() == PNODE_FUNC)
            node->bytecode(man);
}