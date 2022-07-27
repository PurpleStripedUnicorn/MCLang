
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
struct Context {
    Context(ContextType type, Context *prev = nullptr);
    ContextType type;
    Context *prev;
    std::vector<Var> vars;
    std::map<std::string, std::string> constValues;
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
     * Add a variable to the uppermost block/function/global context, skipping
     * basic contexts
     * @param var The variable to add
     * @note There is no extra check to see if this variable already exists
     * @note If there are only basic contexts, nothing happens
     */
    void addBlockVar(Var var);

    /**
     * Set the value of a constant, in the uppermost block context, like with
     * `addBlockVar`
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
     * the GLOBAL type, stops when one is found
     * @return A vector containing the variables
     * @note If no global context is found, an empty vector is returned
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

    /**
     * Get all constant definitions in all currently registered contexts
     * @return A map containing all of the constant definitions
     * @note Assumes that there are no duplicate constant names
     */
    std::map<std::string, std::string> getConstValues() const;

    /**
     * Print out the context stack in debug form
     */
    void print() const;

private:

    /**
     * Find a variable in a given context
     * @param name The name of the variable to search for
     * @param result The type of the variable will be put here if found
     * @param ctx The context to search in
     * @return A boolean indicating if the variable was found
     */
    bool findVarCtx(std::string name, Type &result, Context *ctx) const;

    /**
     * Find a function with given parameter types in a given context
     * @param name The name of the variable to search for
     * @param types The parameter types of the function
     * @param result The function definition will be put here if found
     * @param ctx The context to search in
     * @return A boolean indicating if the variable was found
     */
    bool findFuncCtx(std::string name, std::vector<Type> types,
    FuncDef *&result, Context *ctx) const;

    /**
     * Retrieve the uppermost block context
     * @return A pointer to the uppermost context that is not of the BASIC type,
     * null if none can be found
     */
    Context *topBlockCtx() const;

    Context *topContext;

};

#endif