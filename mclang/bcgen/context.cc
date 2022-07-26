
#include "bcgen/context.h"
#include "general/funcdef.h"
#include "general/types.h"
#include "general/var.h"
#include <string>
#include <vector>

Context::Context(ContextType type, Context *prev) : type(type), prev(prev) {

}

Context::~Context() {
    
}

Context *Context::getPrev() const {
    return prev;
}

bool Context::findVar(std::string name, Type &result) const {
    for (const Var &var : vars) {
        if (var.name == name) {
            result = var.type;
            return true;
        }
    }
    return false;
}

bool Context::findVarAll(std::string name, Type &result) const {
    bool cur = findVar(name, result);
    if (cur)
        return true;
    if (prev != nullptr)
        return prev->findVarAll(name, result);
    return false;
}

bool Context::findFunc(std::string name, std::vector<Type> types,
FuncDef &result) const {
    for (const FuncDef &func : funcs) {
        if (func.name == name && func.params.size() == types.size()) {
            bool success = true;
            for (unsigned int i = 0; i < func.params.size(); i++) {
                if ((Type)func.params[i].type != types[i]) {
                    success = false;
                    break;
                }
            }
            if (success) {
                result = func;
                return true;
            }
        }
    }
    return false;
}

bool Context::findFuncAll(std::string name, std::vector<Type> types,
FuncDef &result) const {
    bool cur = findFunc(name, types, result);
    if (cur)
        return true;
    if (prev != nullptr)
        return prev->findFuncAll(name, types, result);
    return false;
}

void Context::addVar(Var var) {
    vars.push_back(var);
}

void Context::setConst(std::string name, std::string value) {
    constValues[name] = value;
}

void Context::addFunc(FuncDef func) {
    funcs.push_back(func);
}

const std::vector<Var> &Context::getVars() const {
    return vars;
}

std::vector<Var> Context::getLocalVars() const {
    std::vector<Var> out;
    Context *cur = (Context *)this;
    while (cur != nullptr) {
        for (const Var &var : cur->getVars())
            if (!var.type.isConst)
                out.push_back(var);
        if (cur->getType() == CTX_FUNC)
            break;
        cur = cur->getPrev();
    }
    return out;
}

ContextType Context::getType() const {
    return type;
}

ContextStack::ContextStack() : topContext(new Context(CTX_GLOBAL)) {

}

ContextStack::~ContextStack() {
    Context *cur = topContext;
    while (cur != nullptr) {
        Context *tmp = cur->getPrev();
        delete cur;
        cur = tmp;
    }
}

void ContextStack::pushContext(ContextType type) {
    Context *newTop = new Context(type, topContext);
    topContext = newTop;
}

void ContextStack::popContext() {
    Context *newTop = topContext->getPrev();
    delete topContext;
    topContext = newTop;
}

void ContextStack::addVar(Var var) {
    topContext->addVar(var);
}

void ContextStack::setConst(std::string name, std::string value) {
    topContext->setConst(name, value);
}

void ContextStack::addFunc(FuncDef func) {
    topContext->addFunc(func);
}

std::vector<Var> ContextStack::getLocalVars() const {
    return topContext->getLocalVars();
}