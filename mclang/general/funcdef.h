
#ifndef __FUNCDEF_H__
#define __FUNCDEF_H__

#include "general/var.h"
#include <string>
#include <vector>

/**
 * Function definition, with name and parameter types/names
 */
struct FuncDef {
    FuncDef(std::string name, std::vector<Var> params);
    std::string name;
    std::vector<Var> params;
};

#endif