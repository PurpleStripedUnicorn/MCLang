
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
 * Defined aliases with inserted constant values
 */
struct FuncAlias {  
    FuncAlias();
    FuncAlias(std::string name);
    FuncAlias(std::string name, std::vector<std::string> constValues);
    std::string name;
    std::vector<std::string> constValues;
};

/**
 * Function definition, with name and parameter types/names
 */
struct FuncDef {
    FuncDef();
    FuncDef(std::string name, std::vector<Param> params);
    Type returnType;
    const static FuncDef unknown;
    std::string name;
    std::vector<Param> params;
    std::vector<FuncAlias> aliases;
};

#endif