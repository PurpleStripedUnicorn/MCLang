
#ifndef __PARSENODE_PROGRAM_H__
#define __PARSENODE_PROGRAM_H__

#include "general/loc.h"
#include "parsenodes/parsenode.h"
#include <vector>

class BCManager;

class ProgramNode : public ParseNode {

public:

    /**
     * Constructor
     * @param childNodes The main components of the program (functions, global
     * vars, etc.)
     * @param loc The location of the parse node
     */
    ProgramNode(std::vector<ParseNode *> childNodes, Loc loc);

    /**
     * Destructor
     */
    virtual ~ProgramNode() override;

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

private:

    /**
     * Search for global settings in the parse tree and apply them to the
     * compiler. A global setting for example is the namespace
     * @param man The main bytecode manager
     * @post Compiler component has settings applied
     */
    void applyGlobalSettings(BCManager &man) const;

    /**
     * Register all global variables (both constant and non-constant)
     * @param man The main bytecode manager
     * @post Global variables are added in the bytecode manager
     */
    void registerGlobalVars(BCManager &man) const;

    /**
     * Register all functions
     * @param man The main bytecode manager
     * @post Functions are registered in the bytecode manager
     */
    void registerFunctions(BCManager &man) const;

    // Child nodes of the main program
    std::vector<ParseNode *> childNodes;

};

#endif