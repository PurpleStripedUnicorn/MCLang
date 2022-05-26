
#include "parsenodes/codeblock.h"

CodeBlockNode::CodeBlockNode(std::vector<ParseNode *> childNodes) :
ParseNode(PNODE_CODEBLOCK), childNodes(childNodes) {

}

CodeBlockNode::~CodeBlockNode() {
    for (unsigned int i = 0; i < childNodes.size(); i++)
        delete childNodes[i];
}

std::vector<ParseNode *> CodeBlockNode::children() const {
    return childNodes;
}

void CodeBlockNode::bytecode(BCManager &man) const {
    for (unsigned int i = 0; i < childNodes.size(); i++)
        childNodes[i]->bytecode(man);
}