
#include "bcgen/context.h"

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
    // TODO: implement
}

bool Context::findVarAll(std::string name, Type &result) {
    // TODO: implement
}

bool Context::findFunc(std::string name, std::vector<Type> types,
FuncDef &result) const {
    // TODO: implement
}

bool Context::findFuncAll(std::string name, std::vector<Type> types,
FuncDef &result) const {
    // TODO: implement
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