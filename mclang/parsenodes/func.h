
#ifndef __PARSENODE_FUNC_H__
#define __PARSENODE_FUNC_H__

#include "general/loc.h"
#include "general/types.h"
#include "general/var.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

class BCManager;
class CodeBlockNode;

/**
 * Function alias
 * Generated function given constant values as inputs
 */
struct FuncAlias {
    std::string callname;
    std::vector<std::string> constValues;
};

class FuncNode : public ParseNode {

public:

    /**
     * Constructor
     * @param retType THe return type of the function
     * @param name Name of the function
     * @param params The parameters of the function
     * @param codeblock Code inside the function
     * @param loc The location of the parse node
     */
    FuncNode(Type retType, std::string name, std::vector<Param> params,
    CodeBlockNode *codeblock, Loc loc);

    /**
     * Destructor
     */
    virtual ~FuncNode() override;

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

    /**
     * Generate the bytecode for this function, with the given constants filled
     * in
     * @param constValues The values of the constant arguments
     * @return The name of the function to call
     * @post Stores the name of the generated bytecode function given the
     * constant values in `aliases`
     * @note If the bytecode for this set of constants was already generated, no
     * new bytecode will be generated
     */
    std::string bytecode(std::vector<std::string> constValues);

    /**
     * Get the return type of the function
     * @return The return type
     */
    Type getReturnType() const;

    /**
     * Get the name of the function
     * @return The name of the function
     */
    std::string getName() const;

    /**
     * The the parameter types of this function
     * @return A vector containing the parameter types
     */
    std::vector<Type> getParamTypes() const;

private:

    /**
     * Check if a function with the given constants filled in has already been
     * generated
     * @param constValues The constant values to look for
     * @param result Puts the call name of the function in the result, if any is
     * found
     * @return A boolean indicating if a function was found
     */
    bool findAlias(std::vector<std::string> constValues, std::string &result)
    const;

    /**
     * Check if the function has already been defined with the same parameter
     * types (ignoring const)
     * @return Boolean indicating if another function was found
     */
    bool hasNameConflict() const;

    /**
     * Check if the function has a name conflict with the given other function
     * @return Boolean indicating if there is a conflict
     */
    bool hasNameConflict(FuncNode *other) const;

    /**
     * Initialize global variables by adding 0 to them
     */
    void initGlobalVars();

    /**
     * Initialize parameters by copying them to their correct variables
     * @param constValues The constant values to initialize
     */
    void initParams(std::vector<std::string> constValues);

    // Return type of the function
    Type retType;

    // Name of the function
    std::string name;

    // Function parameters
    std::vector<Param> params;

    // Content of the function
    CodeBlockNode *codeblock;

    // Generated functions with filled in constants
    std::vector<FuncAlias> aliases;

    // Pointer to the main bytecode manager, will be assigned once `bytecode` is
    // called
    BCManager *bcman;

};

#endif