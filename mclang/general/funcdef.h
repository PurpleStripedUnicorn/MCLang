
#ifndef __FUNCDEF_H__
#define __FUNCDEF_H__

#include <string>
#include <vector>

/**
 * Function parameter with type and parameter name
 */
struct Param {
    Param(std::string type, std::string name);
    std::string type, name;
};

/**
 * Function definition, with name and parameter types/names
 * Constant parameters are separate from normal parameters
 */
struct FuncDef {
    FuncDef(std::string name, std::vector<Param> params, std::vector<Param>
    constParams);
    std::string name;
    std::vector<Param> params, constParams;
};

#endif