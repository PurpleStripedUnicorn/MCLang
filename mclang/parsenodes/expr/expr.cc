
#include "parsenodes/expr/expr.h"
#include "parsenodes/parsenode.h"
#include <vector>

ExprNode::ExprNode(ParseNodeType type, ParseNodeProps props, ParseNode *left,
ParseNode *right) : ParseNode(type, props), left(left), right(right) {

}

ExprNode::~ExprNode() {
    delete left;
    delete right;
}

std::vector<ParseNode *> ExprNode::children() const {
    if (right == NULL)
        return {left};
    return {left, right};
}