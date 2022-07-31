
#include <map>
#include <vector>
#include <string>
#include "bcgen/context.h"
#include "general/var.h"

ContextStack::ContextStack() : stack({Context()}) {

}

ContextStack::~ContextStack() {

}

void ContextStack::push() {
    stack.push_back(Context());
}

void ContextStack::pop() {
    if (stack.size() > 0)
        stack.pop_back();
}

bool ContextStack::empty() const {
    return stack.empty();
}

std::vector<Context> &ContextStack::getStack() {
    return stack;
}

std::vector<Var> ContextStack::getVars() const {
    std::vector<Var> out;
    for (const Context &ctx : stack)
        out.insert(out.end(), ctx.vars.begin(), ctx.vars.end());
    return out;
}

std::vector<Var> ContextStack::getLocalVars() const {
    if (stack.empty())
        return {};
    return stack.back().vars;
}

bool ContextStack::findVar(std::string name, Var &result) const {
    for (const Context &ctx : stack) {
        for (const Var &var : ctx.vars) {
            if (var.name == name) {
                result = var;
                return true;
            }
        }
    }
    return false;
}

std::string ContextStack::getConstValue(std::string name) const {
    for (const Context &ctx : stack)
        if (ctx.constValues.count(name) > 0)
            return ctx.constValues.find(name)->second;
    return "";
}

std::map<std::string, std::string> ContextStack::getConstValues() const {
    std::map<std::string, std::string> out;
    for (const Context &ctx : stack)
        out.insert(ctx.constValues.begin(), ctx.constValues.end());
    return out;
}

std::map<std::string, std::string> ContextStack::getLocalConstValues() const {
    if (stack.empty())
        return {};
    return stack.back().constValues;
}

void ContextStack::pushVar(Var var) {
    stack.back().vars.push_back(var);
}

void ContextStack::setConst(std::string name, std::string value) {
    stack.back().constValues[name] = value;
}