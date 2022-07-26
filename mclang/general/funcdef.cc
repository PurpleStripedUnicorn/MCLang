
#include "general/funcdef.h"
#include <string>
#include <vector>

Param::Param(Type type, std::string name) : type(type), name(name) {

}

FuncDef::FuncDef() : name("??"), params({}) {
    
}

FuncDef::FuncDef(std::string name, std::vector<Param> params) : name(name),
params(params) {

}

const FuncDef FuncDef::unknown = FuncDef("??", {});