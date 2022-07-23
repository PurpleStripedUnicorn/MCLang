
#include "general/loc.h"
#include "parsenodes/parsenode.h"

ParseNode::ParseNode(ParseNodeType type, Loc loc) : type(type), loc(loc) {

}

ParseNode::~ParseNode() {

}

ParseNodeType ParseNode::getType() const {
    return type;
}

Loc ParseNode::getLoc() const {
    return loc;
}
