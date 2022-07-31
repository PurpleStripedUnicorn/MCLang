
#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "general/funcdef.h"
#include "general/types.h"
#include "general/var.h"
#include <map>
#include <string>
#include <vector>

class FuncNode;

/**
 * A context in a context stack
 * A context contains variable definitions and constant values
 */
struct Context {
    std::vector<Var> vars;
    std::map<std::string, std::string> constValues;
};

/**
 * Context stack
 * Has useful operations that can be performed on the current contexts
 */
class ContextStack {

public:

    /**
     * Constructor
     * @note `stack` is initialized with one context
     */
    ContextStack();

    /**
     * Destructor
     */
    ~ContextStack();

    /**
     * Add an empty context at the top of the context stack
     * @post An empty context is added at the end of `stack`
     */
    void push();

    /**
     * Remove the top context
     * @post Top context is removed from `stack`
     * @note If the context stack is empty, nothing happens
     */
    void pop();

    /**
     * Check if the context stack is empty
     * @return A boolean indicating if `stack` is empty
     */
    bool empty() const;

    /**
     * Get a list of all of the variables in the stack currently
     * @return The list as a vector of Vars
     */
    std::vector<Var> getVars() const;

    /**
     * Get a list of variables in the top context of the context stack
     * @return The list as a vector of Vars
     * @note If the context stack if empty, returns an empty vector
     */
    std::vector<Var> getLocalVars() const;

    /**
     * Find a specific variable
     * @param name The name of the variable
     * @param result The found variable is put in this parameter
     * @return A boolean indicating if a variable was found
     */
    bool findVar(std::string name, Var &result) const;

    /**
     * Get the value of a specific constant
     * @param name The name of the constant
     * @return The value of the constant as a string if stored, otherwise ""
     */
    std::string getConstValue(std::string name) const;

    /**
     * Get all constant values in the current contexts
     * @return A map with all of the constant names and their values
     */
    std::map<std::string, std::string> getConstValues() const;

    /**
     * Get constant values in the top context
     * @return A map with the constant names and their values
     */
    std::map<std::string, std::string> getLocalConstValues() const;

    /**
     * Add a variable to the current top context
     * @param var The variable to add to the context
     * @pre The `stack` is not empty
     */
    void pushVar(Var var);

    /**
     * Set the value of a constant in the top context
     * @param name The name of the constant
     * @param value The constant value
     * @pre The `stack` is not empty
     */
    void setConst(std::string name, std::string value);

private:

    // Actual context stack
    std::vector<Context> stack;

};

#endif