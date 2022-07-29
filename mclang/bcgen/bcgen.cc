
#include "bcgen/bcgen.h"
#include "bcgen/instr.h"
#include "compiler/compiler.h"
#include "parsenodes/parsenode.h"
#include "parser/parser.h"
#include <string>
#include <vector>

BCFunc::BCFunc(std::string name) : name(name) {

}

BCManager::BCManager(Compiler *comp) : ctx({Context()}), comp(comp),
uniqueFuncId(1) {

}

BCManager::~BCManager() {
    for (unsigned int i = 0; i < funcList.size(); i++)
        delete funcList[i];
}

void BCManager::generate() {
    comp->parser->getTree()->bytecode(*this);
}

const std::vector<BCFunc *> *BCManager::getBytecode() const {
    return &funcList;
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
    BCFunc *f = new BCFunc(name);
    funcList.push_back(f);
    funcStack.push_back(f);
}

void BCManager::popFunc() {
    if (funcStack.size() > 0)
        funcStack.pop_back();
}

BCFunc *BCManager::topFunc() const {
    return funcStack.back();
}