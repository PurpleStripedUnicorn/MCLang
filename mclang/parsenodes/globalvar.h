
#ifndef __PARSENODE_GLOBALVAR_H__
#define __PARSENODE_GLOBALVAR_H__

#include "parsenodes/parsenode.h"
#include <vector>
#include <string>

class BCManager;

class GlobalVarNode : public ParseNode {

public:

    /**
     * Constructor
     * @param varType The variable type to make the variable
     * @param varName The name of the global variable
     * @param props General parse node properties
     * @note Global variables cannot be initialized with a value, instead they
     * will start at value zero
     */
    GlobalVarNode(std::string varType, std::string varName,
    ParseNodeProps props);

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
    virtual void bytecode(BCManager &man) const override;

protected:

    // The type of the global variable
    std::string varType;

    // THe name of the global variable
    std::string varName;

};

#endif