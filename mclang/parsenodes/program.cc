
#include "bcgen/bcgen.h"
#include "compiler/compiler.h"
#include "errorhandle/handle.h"
#include "parsenodes/namespace.h"
#include "parsenodes/parsenode.h"
#include "parsenodes/program.h"
#include <vector>

ProgramNode::ProgramNode(std::vector<ParseNode *> childNodes,
ParseNodeProps props) : ParseNode(PNODE_PROGRAM, props), childNodes(childNodes)
{

}

ProgramNode::~ProgramNode() {
    for (unsigned int i = 0; i < childNodes.size(); i++)
        delete childNodes[i];
}

std::vector<ParseNode *> ProgramNode::children() const {
    return childNodes;
}

void ProgramNode::bytecode(BCManager &man) const {
    applyGlobalSettings(man);
    for (unsigned int i = 0; i < childNodes.size(); i++)
        childNodes[i]->bytecode(man);
}

void ProgramNode::applyGlobalSettings(BCManager &man) const {
    // Search for namespace setting override
    bool foundNamespace = false;
    for (unsigned int i = 0; i < childNodes.size(); i++) {
        if (childNodes[i]->getType() == PNODE_NAMESPACE) {
            if (foundNamespace)
                MCLError(0, "Namespace set multiple times, only last setting "
                "is used", childNodes[i]->props.loc.line,
                childNodes[i]->props.loc.col);
            man.comp->ns = ((NSNode *)childNodes[i])->getName();
            foundNamespace = true;
        }
    }
}