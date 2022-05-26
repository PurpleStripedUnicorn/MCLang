
#include "parsenodes/parsenode.h"

ParseNode::ParseNode(ParseNodeType type, ParseNodeProps props) : props(props),
type(type) {

}

ParseNode::~ParseNode() {

}

ParseNodeType ParseNode::getType() const {
    return type;
}