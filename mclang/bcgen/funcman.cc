
#include "bcgen/funcman.h"
#include <vector>

FuncManager::FuncManager() {

}

FuncManager::~FuncManager() {

}

void FuncManager::push(FuncNode *func) {
    funcs.push_back(func);
}