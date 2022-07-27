
#ifndef __PARSENODE_ASSIGN_H__
#define __PARSENODE_ASSIGN_H__

#include "general/loc.h"
#include "parsenodes/expr/expr.h"
#include <string>
#include <vector>

class BCManager;

class AssignNode : public ExprNode {

public:

    /**
     * Constructor
     * @param varName The variable name at the left of the expression
     * @param expr The expression to assign to the variable
     * @param loc The location of the parse node
     */
    AssignNode(std::string varName, ParseNode *expr, Loc loc);

    /**
     * Destructor
     */
    virtual ~AssignNode() override;

    /**
     * Generate bytecode for this parse node
     * @param man The main bytecode manager
     */
    virtual void bytecode(BCManager &man) const override;

    /**
     * Get the name of the variable being assigned
     * @return The name as a string
     */
    std::string getVarName() const;

    /**
     * Get the assigned expression parse node
     * @return The pointer stored in `left`
     */
    ParseNode *getExpr() const;

private:

    // The name of the variable
    std::string varName;

    // NOTE: Expression can be accessed with `left` pointer

};

#endif