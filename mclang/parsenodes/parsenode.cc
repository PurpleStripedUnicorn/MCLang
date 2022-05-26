
#include "parsenodes/parsenode.h"

ParseNode::ParseNode(ParseNodeType type) : type(type), loc({0, 0}) {

}

ParseNode::~ParseNode() {

}

ParseNodeType ParseNode::getType() const {
    return type;
}

void ParseNode::setLoc(unsigned int line, unsigned int col) {
    loc.line = line, loc.col = col;
}