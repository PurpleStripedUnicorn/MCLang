
#ifndef __PARSENODE_CALL_H__
#define __PARSENODE_CALL_H__

#include "general/loc.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

class BCManager;
class FuncNode;

class CallNode : public ParseNode {

public:

    /**
     * Constructor
     * @param fname The name of the function to call
     * @param params The parameters given to the function call
     * @param loc The location of the parse node
     */
    CallNode(std::string fname, std::vector<ParseNode *> params, Loc loc);

    /**
     * Destructor
     */
    virtual ~CallNode() override;

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
     * Find the function with the correct name and parameter types
     * @param man The main bytecode manager
     * @return A pointer to a function that has the same name and accepts the
     * given parameter types, or null if none is found
     */
    FuncNode *findFunc(BCManager &man) const;

    /**
     * Push local variables on the stack
     * @param man The main bytecode manager
     */
    void pushLocalVars(BCManager &man) const;

    /**
     * Retrieve local variables from the stack
     * @param man The main bytecode manager
     */
    void popLocalVars(BCManager &man) const;

    /**
     * Generate the bytecode for the child nodes and keep track of output types
     * @param man The main bytecode manager
     * @post Output types are put in `paramTypes`
     */
    void bytecodeChildren(BCManager &man);

    /**
     * Show an error for the called function not existing
     */
    void notFoundError();

    /**
     * Get the constant values that are passed to the function, when a constant
     * is also explicitly required. Uses `paramTypes` and `paramValues`
     */
    std::vector<std::string> getConstVals() const;

    // The name of the function to call
    std::string fname;

    // Passed function parameters
    std::vector<ParseNode *> params;

    // Parameter types will be put here after they have been found
    std::vector<Type> paramTypes;

    // Parameter return values will be stored here for use when determining
    // constant values
    std::vector<std::string> paramValues;

};

#endif