
#ifndef __BCGEN_FUNCMAN_H__
#define __BCGEN_FUNCMAN_H__

#include "general/types.h"
#include <string>
#include <vector>

class FuncNode;

/**
 * Manages function definitions
 */
class FuncManager {

public:

    /**
     * Constructor
     * @note Starts with no functions in `funcs`
     */
    FuncManager();

    /**
     * Destructor
     */
    ~FuncManager();

    /**
     * Get a list of all functions
     * @return A vector of pointers to function parse nodes
     */
    std::vector<FuncNode *> getFuncs() const;

    /**
     * Add a function to the function list
     * @param func A pointer to the function parse node
     * @post Function pointer is added at the end of `funcs`
     */
    void push(FuncNode *func);

    /**
     * Find a function with name and parameter types
     * @param name The name of the function
     * @param types A list of parameter types
     * @return A pointer to the function parse node, or null if nothing is found
     */
    FuncNode *find(std::string name, std::vector<Type> types);

private:

    // Pointers to function parse nodes
    std::vector<FuncNode *> funcs;

};

#endif