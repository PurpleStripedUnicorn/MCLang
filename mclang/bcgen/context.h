
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
 * A context contains function definitions, variable definitions, constant
 * values, etc.
 */
struct Context {
    std::vector<Var> vars;
    std::map<std::string, std::string> constValues;
    std::vector<FuncNode *> funcs;
};

#endif