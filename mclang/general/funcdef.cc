
#include "general/funcdef.h"
#include <string>
#include <vector>

Param::Param(Type type, std::string name) : type(type), name(name) {

}

FuncDef::FuncDef(std::string name, std::vector<Param> params) : name(name),
params(params) {

}