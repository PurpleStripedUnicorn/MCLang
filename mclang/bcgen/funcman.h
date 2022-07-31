
#ifndef __BCGEN_FUNCMAN_H__
#define __BCGEN_FUNCMAN_H__

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
     * Add a function to the function list
     * @param func A pointer to the function parse node
     * @post Function pointer is added at the end of `funcs`
     */
    void push(FuncNode *func);

private:

    // Pointers to function parse nodes
    std::vector<FuncNode *> funcs;

};

#endif