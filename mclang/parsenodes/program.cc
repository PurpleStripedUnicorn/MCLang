
#include "parsenodes/program.h"

ProgramNode::ProgramNode(std::vector<ParseNode *> childNodes) :
ParseNode(PNODE_PROGRAM), childNodes(childNodes) {

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