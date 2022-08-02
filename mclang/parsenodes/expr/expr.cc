
#include "general/loc.h"
#include "parsenodes/expr/expr.h"
#include "parsenodes/parsenode.h"
#include <vector>

ExprNode::ExprNode(ParseNodeType type, Loc loc, ParseNode *left,
ParseNode *right) : ParseNode(type, loc), left(left), right(right) {

}

ExprNode::~ExprNode() {
    delete left;
    delete right;
}

std::vector<ParseNode *> ExprNode::children() const {
    if (right == nullptr)
        return {left};
    return {left, right};
}