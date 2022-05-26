
#include "bcgen.h"

BCFunc::BCFunc(std::string name) : name(name) {

}

BCManager::BCManager() : uniqueFuncId(1) {

}

BCManager::~BCManager() {

}

std::vector<BCFunc> BCManager::getBytecode() const {
    return funcList;
}

BCFunc *BCManager::curFunc() const {
    if (funcStack.size() == 0)
        return NULL;
    return funcStack.back();
}

void BCManager::write(BCInstr instr) {
    if (funcStack.size() == 0)
        return;
    funcStack.back()->instrList.push_back(instr);
}

void BCManager::addFunc(std::string name) {
    if (name == "")
        name = std::to_string(uniqueFuncId++);
    funcList.push_back(BCFunc(name));
    funcStack.push_back(&funcList.back());
}

void BCManager::popFunc() {
    if (funcStack.size() > 0)
        funcStack.pop_back();
}