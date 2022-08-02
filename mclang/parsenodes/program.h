
#ifndef __PARSENODE_PROGRAM_H__
#define __PARSENODE_PROGRAM_H__

#define GEN_RECURION_LIMIT 100

#include "general/loc.h"
#include "parsenodes/parsenode.h"
#include <string>
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

    /**
     * The bytecode of functions is generated in two steps:
     * 1. The bytecode of the functions with void return type and no parameters
     *    is generated
     * 2. All functions that still need to be generated because it was required
     *    for generation of other functions, are generated. This is done
     *    repeatedly until there are no more functions left to generate or a cap
     *    for the amount of loops is reached
     * @param man The main bytecode manager
     */
    void generateFunctions(BCManager &man) const;

    /**
     * Show an error for reaching the generation recursion limit
     * @param man The main bytecode manager
     */
    void recursionError(BCManager &man) const;

    // Child nodes of the main program
    std::vector<ParseNode *> childNodes;

};

#endif