
#include "bcgen/context.h"
#include "general/funcdef.h"
#include "general/types.h"
#include "general/var.h"

Context::Context() : prev(nullptr) {

}

Context::Context(Context *prev) : prev(prev) {

}

Context::~Context() {
    
}

Context *Context::getPrev() const {
    return prev;
}

bool Context::findVar(std::string name, Type &result) {
    for (Var &var : vars) {
        if (var.name == name) {
            result = var.type;
            return true;
        }
    }
    return false;
}

bool Context::findVarAll(std::string name, Type &result) {
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

ContextStack::ContextStack() : topContext(new Context()) {

}

ContextStack::~ContextStack() {
    Context *cur = topContext;
    while (cur != nullptr) {
        Context *tmp = cur->getPrev();
        delete cur;
        cur = tmp;
    }
}

void ContextStack::pushContext() {
    Context *newTop = new Context(topContext);
    topContext = newTop;
}

void ContextStack::popContext() {
    Context *newTop = topContext->getPrev();
    delete topContext;
    topContext = newTop;
}