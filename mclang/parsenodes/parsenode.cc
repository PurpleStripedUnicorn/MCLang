
#include "parsenodes/parsenode.h"

ParseNode::ParseNode(ParseNodeType type) : type(type) {

}

ParseNode::~ParseNode() {

}

ParseNodeType ParseNode::getType() const {
    return type;
}