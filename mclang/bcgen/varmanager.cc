
#include "bcgen/varmanager.h"
#include <string>
#include <vector>

VarContext::VarContext() {

}

VarContext::~VarContext() {

}

void VarContext::addVar(std::string name) {
    vars.push_back(name);
}

bool VarContext::hasVar(std::string name) const {
    for (unsigned int i = 0; i < vars.size(); i++)
        if (vars[i] == name)
            return true;
    return false;
}

VarManager::VarManager() {
    addContext();
}

VarManager::~VarManager() {

}

void VarManager::addContext() {
    contextStack.push_back(VarContext());
}

void VarManager::popContext() {
    contextStack.pop_back();
}

void VarManager::addVar(std::string name) {
    contextStack.back().addVar(name);
}

bool VarManager::hasVar(std::string name) const {
    for (unsigned int i = 0; i < contextStack.size(); i++)
        if (contextStack[i].hasVar(name))
            return true;
    return false;
}