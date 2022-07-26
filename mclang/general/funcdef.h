
#ifndef __FUNCDEF_H__
#define __FUNCDEF_H__

#include "general/types.h"
#include <string>
#include <vector>

/**
 * Function parameter with type and parameter name
 */
struct Param {
    Param(Type type, std::string name);
    Type type;
    std::string name;
};

/**
 * Function definition, with name and parameter types/names
 */
struct FuncDef {
    FuncDef(std::string name, std::vector<Param> params);
    Type returnType;
    std::string name;
    std::vector<Param> params;
};

#endif