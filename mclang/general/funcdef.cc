
#include "general/funcdef.h"
#include <string>
#include <vector>

Param::Param(Type type, std::string name) : type(type), name(name) {

}

FuncAlias::FuncAlias() : name("??"), constValues({}) {

}

FuncAlias::FuncAlias(std::string name) : name(name), constValues({}) {

}

FuncAlias::FuncAlias(std::string name, std::vector<std::string> constValues) :
name(name), constValues(constValues) {

}

FuncDef::FuncDef() : name("??"), params({}) {
    
}

FuncDef::FuncDef(std::string name, std::vector<Param> params) : name(name),
params(params) {

}

const FuncDef FuncDef::unknown = FuncDef("??", {});