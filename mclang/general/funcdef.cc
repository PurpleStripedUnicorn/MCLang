
#include "general/funcdef.h"
#include "general/var.h"
#include <string>
#include <vector>

FuncDef::FuncDef(std::string name, std::vector<Var> params) : name(name),
params(params) {

}