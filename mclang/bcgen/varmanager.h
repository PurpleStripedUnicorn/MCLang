
#ifndef __VARMANAGER_H__
#define __VARMANAGER_H__

#include <string>
#include <vector>

/**
 * Variable context used for managing variables defined in a stack of contexts
 */
class VarContext {

public:

    /**
     * Constructor
     * @note initially there are no variables in this context
     */
    VarContext();

    /**
     * Destructor
     */
    ~VarContext();

    /**
     * Add a variable to the context
     * @param name The variable name
     * @post Given variable is added to `vars`
     */
    void addVar(std::string name);

    /**
     * Check if a variable has already been defined in this context
     * @param name The variable name to check for
     * @return A boolean indicating if the given name is in the `vars` vector
     */
    bool hasVar(std::string name) const;

    /**
     * Append all variable names in this context to a given vector
     * @param list The vector to append the variable names to
     */
    void appendVarNames(std::vector<std::string> &list) const;

private:

    // Variables stored in this context
    std::vector<std::string> vars;

};

/**
 * Manager object for variables which can be assigned in different contexts
 * This class is used by the bytecode manager
 */
class VarManager {

public:

    /**
     * Constructor
     * @note Starts with one empty context on the context stack
     */
    VarManager();

    /**
     * Destructor
     */
    ~VarManager();

    /**
     * Add an empty context at the top of the context stack
     */
    void addContext();

    /**
     * Remove the top context from the context stack
     * @note If the context stack if empty, nothing happens
     */
    void popContext();

    /**
     * Add a variable to the current cotext
     * @param name The variable name
     * @pre `contextStack` is not empty
     */
    void addVar(std::string name);

    /**
     * Check if a variable is defined in the current context, meaning any
     * context above this one as well
     * @param name The variable name to search for
     * @return Boolean indicating if the variable is found
     */
    bool hasVar(std::string name) const;

    /**
     * Create a unique variable name that does not appear on the context stack,
     * it will have a name in the form "__tmp[ID]"
     * @return The variable name
     * @post The variable is added to the top context on the context stack
     */
    std::string getUniqueVar();

    /**
     * Generate a list of all of the variables that are in the context stack
     * @param list The variable to put the list of the names of variables in
     */
    void getVarNames(std::vector<std::string> &list) const;

private:

    // Context stack
    std::vector<VarContext> contextStack;

};

#endif