
#ifndef __CONTEXT_H__
#define __CONTEXT_H__

#include "general/funcdef.h"
#include "general/types.h"
#include "general/var.h"
#include <map>
#include <string>
#include <vector>

/**
 * Type for contexts, with different meanings
 * BASIC: Basic type without functionality
 * BLOCK: For code blocks, like in if-statements, etc.
 * FUNC: For entire functions
 * GLOBAL: Global context should only be used once
 */
enum ContextType {
    CTX_BASIC, CTX_BLOCK, CTX_FUNC, CTX_GLOBAL
};

/**
 * A context in a context stack
 * A context contains function definitions, variable definitions, constant
 * values, etc.
 */
class Context {

public:

    /**
     * Constructor
     * @param funcCtx Indicates if this is a function context
     */
    Context(ContextType type, Context *prev = nullptr);

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
    bool findVar(std::string name, Type &result) const;

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
    bool findVarAll(std::string name, Type &result) const;

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

    /**
     * Add a variable to this context
     * @param var The variable to add
     * @note There is no extra check to see if this variable already exists
     */
    void addVar(Var var);

    /**
     * Set the value of a constant, in the current context
     * @param name Constant variable name
     * @param value The value to assign to the constant
     */
    void setConst(std::string name, std::string value);

    /**
     * Add a function to the current context
     * @param func The function to add
     */
    void addFunc(FuncDef func);

    /**
     * Get all variables defined in this context
     * @return A reference to the vector containing the variables
     */
    const std::vector<Var> &getVars() const;

    /**
     * Get all non-constant variables in the current function context
     * What this means is that the stack will be traversed until a function
     * context is hit, and all non-constant variables will be collected on the
     * way
     * @param traverse Turn travering the stack on or off
     * @return A vector containing the variables
     */
    std::vector<Var> getLocalVars(bool traverse = true) const;

    /**
     * Get the type of this context
     * @return The type of the context
     */
    ContextType getType() const;

    /**
     * Add an alias to the function with the given name and typenames
     * @param name The name of the function
     * @param types A vector of argument types
     * @param alias The alias to add to the function
     * @note Assumes that the given function with argument types exists
     */
    void addFuncAlias(std::string name, std::vector<Type> types, FuncAlias alias
    );

private:

    // Context type
    ContextType type;

    // Previous context
    Context *prev;

    // Variables in the current context
    std::vector<Var> vars;

    // Values of constants
    std::map<std::string, std::string> constValues;

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
     * Add an empty context at the top of the context stack
     * @param type The context type
     * @post `topContext` is changed to the new context on the stack
     */
    void pushContext(ContextType type);

    /**
     * Remove the top context from the context stack
     * @post `topContext` is changed to the previous context on the stack
     */
    void popContext();

    /**
     * Add a variable to the top context
     * @param var The variable to add
     * @note There is no extra check to see if this variable already exists
     */
    void addVar(Var var);

    /**
     * Set the value of a constant, in the top context
     * @param name Constant variable name
     * @param value The value to assign to the constant
     */
    void setConst(std::string name, std::string value);

    /**
     * Add a function to the top context
     * @param func The function to add
     */
    void addFunc(FuncDef func);

    /**
     * Get all local variables in this function, this can be used for stack
     * operations when calling functions
     * @return A vector containing the variables
     */
    std::vector<Var> getLocalVars() const;

    /**
     * Get all global variables in the context stack. Searches for contexts with
     * the GLOBAL type
     * @return A vector containing the variables
     */
    std::vector<Var> getGlobalVars() const;

    /**
     * Check if there is a variable (const or not) in the top context with the
     * given name, if so get this variable's type
     * @param name The name of the variable
     * @param result The variable type will be put here
     * @return Boolean indicating if the variable is in this context
     */
    bool findVar(std::string name, Type &result) const;

    /**
     * Check if there is a variable (const or not) in the top context or any
     * context below that one with the given name, if so get this variable's
     * type
     * @param name The name of the variable
     * @param result The variable type will be put here
     * @return Boolean indicating if the variable is in this context
     * @note Puts the first variable it finds in `result`, searching downwards
     * through the stack
     */
    bool findVarAll(std::string name, Type &result) const;

    /**
     * Check if there is a function definition with the given name and the given
     * parameter types in the top context, if so get this function definition
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
     * parameter types in the top context or any context below that one, if
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

    /**
     * Create a unique variable starting with "__tmp" that has not been
     * registered in any context in the context stack
     * @param type The variable type to create
     * @return The created variable
     */
    Var makeUniqueVar(Type type);

    /**
     * Add an alias to the function with the given name and typenames
     * @param name The name of the function
     * @param types A vector of argument types
     * @param alias The alias to add to the function
     * @note Assumes that the given function with argument types exists
     */
    void addFuncAlias(std::string name, std::vector<Type> types, FuncAlias alias
    );

private:

    Context *topContext;

};

#endif