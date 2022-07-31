
#include "bcgen/funcman.h"
#include "general/types.h"
#include "parsenodes/func.h"
#include <string>
#include <vector>

FuncManager::FuncManager() {

}

FuncManager::~FuncManager() {

}

void FuncManager::push(FuncNode *func) {
    funcs.push_back(func);
}

FuncNode *FuncManager::find(std::string name, std::vector<Type> types) {
    for (FuncNode *func : funcs)
        if (func->getName() == name && func->acceptTypes(types))
            return func;
    return nullptr;
}