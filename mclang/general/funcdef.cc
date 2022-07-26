
#include "general/funcdef.h"
#include <string>
#include <vector>

Param::Param(Type type, std::string name) : type(type), name(name) {

}

FuncDef::FuncDef(std::string name, std::vector<Param> params, std::vector<Param>
constParams) : name(name), params(params), constParams(constParams) {

}