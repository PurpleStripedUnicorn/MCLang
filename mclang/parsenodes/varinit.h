
#ifndef __PARSENODE_VARINIT_H__
#define __PARSENODE_VARINIT_H__

#include "general/loc.h"
#include "parsenodes/parsenode.h"
#include <vector>
#include <string>

class BCManager;
class ExprNode;

class VarInitNode : public ParseNode {

public:

    /**
     * Constructor
     * @param varType The variable type to make the variable
     * @param childExpr Child node of the initialization, which should be an
     * assignment, or just the name of the variable
     * @param loc The location of the parse node
     */
    VarInitNode(std::string varType, ParseNode *childExpr, Loc loc);

    /**
     * Destructor
     */
    virtual ~VarInitNode() override;

    /**
     * Get the children of this parse node
     * @return A vector with pointers to the child nodes
     */
    virtual std::vector<ParseNode *> children() const override;

    /**
     * Generate bytecode for this parse node
     * @param man The main bytecode manager
     */
    virtual void bytecode(BCManager &man) const override;

protected:

    // The type of the initialized variable
    std::string varType;

    // The child expression of this node, which should be an assignment
    // expression
    ParseNode *childExpr;

};

#endif