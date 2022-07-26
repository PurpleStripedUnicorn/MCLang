
#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "general/funcdef.h"
#include "general/types.h"
#include "general/var.h"
#include <string>
#include <vector>

/**
 * A context in a context stack
 * A context contains function definitions, variable definitions, constant
 * values, etc.
 */
class Context {

public:

    /**
     * Constructor
     */
    Context();

    /**
     * Constructor
     * @param prev A pointer to the previous context on the stack
     */
    Context(Context *prev);

    /**
     * Destructor
     */
    ~Context();

    /**
     * Get the previous context on the stack
     * @return A pointer to the previous context
     */
    Context *getPrev() const;

    /**
     * Check if there is a variable (const or not) in this context with the
     * given name, if so get this variable's type
     * @param name The name of the variable
     * @param result The variable type will be put here
     * @return Boolean indicating if the variable is in this context
     */
    bool findVar(std::string name, Type &result);

    /**
     * Check if there is a variable (const or not) in this context or any
     * context below this one with the given name, if so get this variable's
     * type
     * @param name The name of the variable
     * @param result The variable type will be put here
     * @return Boolean indicating if the variable is in this context
     * @note Puts the first variable it finds in `result`, searching downwards
     * through the stack
     */
    bool findVarAll(std::string name, Type &result);

    /**
     * Check if there is a function definition with the given name and the given
     * parameter types in the current context, if so get this function
     * definition
     * @param name The function name
     * @param types A vector of argument types
     * @param result The found function definition will be put in this variable,
     * if any is found
     * @return A boolean indicating if there was a function found
     */
    bool findFunc(std::string name, std::vector<Type> types, FuncDef &result)
    const;

    /**
     * Check if there is a function definition with the given name and the given
     * parameter types in the current context or any context below this one, if
     * so get this function definition
     * @param name The function name
     * @param types A vector of argument types
     * @param result The found function definition will be put in this variable,
     * if any is found
     * @return A boolean indicating if there was a function found
     * @note Puts the first function it finds in `result`, searching downwards
     * through the stack
     */
    bool findFuncAll(std::string name, std::vector<Type> types, FuncDef &result)
    const;

private:

    // Previous context
    Context *prev;

    // Variables in the current context
    std::vector<Var> vars;

    // Global variables in the current context
    std::vector<ConstVar> constVars;

    // Function definitions in the current context
    std::vector<FuncDef> funcs;

};

/**
 * Context stack, to perform operations on contexts
 * Creates one context on creation
 */
class ContextStack {

public:

    /**
     * Constructor
     */
    ContextStack();

    /**
     * Destructor
     */
    ~ContextStack();

    /**
     * Add a context at the top of the context stack
     * @post `topContext` is changed to the new context on the stack
     */
    void pushContext();

    /**
     * Remove the top context from the context stack
     * @post `topContext` is changed to the previous context on the stack
     */
    void popContext();

private:

    Context *topContext;

};

#endif