
#include "bcgen/bcgen.h"
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
    for (unsigned int i = 0; i < childNodes.size(); i++)
        childNodes[i]->bytecode(man);
}