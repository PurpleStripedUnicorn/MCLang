
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

std::vector<Var> Context::getLocalVars(bool traverse) const {
    std::vector<Var> out;
    for (const Var &var : getVars())
        if (!var.type.isConst)
            out.push_back(var);
    if (traverse && getType() != CTX_FUNC) {
        std::vector<Var> res = getPrev()->getLocalVars(true);
        out.insert(out.end(), res.begin(), res.end());
    }
    return out;
}

ContextType Context::getType() const {
    return type;
}

void Context::addFuncAlias(std::string name, std::vector<Type> types,
FuncAlias alias) {
    for (FuncDef &func : funcs) {
        if (func.name == name && types.size() == func.params.size()) {
            bool found = true;
            for (unsigned int i = 0; i < types.size(); i++) {
                if (types[i] != func.params[i].type) {
                    found = false;
                    break;
                }
            }
            if (found) {
                func.aliases.push_back(alias);
                return;
            }
        }
    }
    prev->addFuncAlias(name, types, alias);
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

std::vector<Var> ContextStack::getGlobalVars() const {
    std::vector<Var> out;
    Context *cur = topContext;
    while (cur != nullptr) {
        if (cur->getType() == CTX_GLOBAL) {
            std::vector<Var> tmp = cur->getLocalVars(false);
            out.insert(out.end(), tmp.begin(), tmp.end());
        }
        cur = cur->getPrev();
    }
    return out;
}

bool ContextStack::findVar(std::string name, Type &result) const {
    return topContext->findVar(name, result);
}

bool ContextStack::findVarAll(std::string name, Type &result) const {
    return topContext->findVarAll(name, result);
}

bool ContextStack::findFunc(std::string name, std::vector<Type> types,
FuncDef &result) const {
    return topContext->findFunc(name, types, result);
}

bool ContextStack::findFuncAll(std::string name, std::vector<Type> types,
FuncDef &result) const {
    return topContext->findFuncAll(name, types, result);
}

Var ContextStack::makeUniqueVar(Type type) {
    unsigned int i = 0;
    Type tmp;
    while (findVar("__tmp" + std::to_string(i), tmp))
        i++;
    addVar(Var(type, "__tmp" + std::to_string(i)));
    return Var(type, "__tmp" + std::to_string(i));
}

void ContextStack::addFuncAlias(std::string name, std::vector<Type> types,
FuncAlias alias) {
    topContext->addFuncAlias(name, types, alias);
}