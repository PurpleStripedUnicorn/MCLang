
#ifndef __PARSENODE_GLOBALVAR_H__
#define __PARSENODE_GLOBALVAR_H__

#include "general/loc.h"
#include "general/types.h"
#include "parsenodes/parsenode.h"
#include <vector>
#include <string>

class BCManager;

class GlobalVarNode : public ParseNode {

public:

    /**
     * Constructor
     * @param varType The variable type to make the variable
     * @param childExpr The expression after the type definition
     * @param loc The location of the parse node
     * @note Global variables cannot be initialized with a value, instead they
     * will start at value zero
     */
    GlobalVarNode(Type varType, ParseNode *childExpr, Loc loc);

    /**
     * Destructor
     */
    virtual ~GlobalVarNode() override;

    /**
     * Get the children of this parse node
     * @return A vector with pointers to the child nodes
     */
    virtual std::vector<ParseNode *> children() const override;

    /**
     * Generate bytecode for this parse node
     * @param man The main bytecode manager
     */
    virtual void bytecode(BCManager &man) override;

protected:

    // The type of the global variable
    Type varType;

    // The expression after the type
    ParseNode *childExpr;

};

#endif