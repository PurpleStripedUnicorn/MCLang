
#include "bcgen/context.h"
#include "general/funcdef.h"
#include "general/types.h"
#include "general/var.h"
#include <iostream>
#include <map>
#include <string>
#include <vector>

Context::Context(ContextType type, Context *prev) : type(type), prev(prev) {

}

ContextStack::ContextStack() : topContext(new Context(CTX_GLOBAL)) {

}

ContextStack::~ContextStack() {
    Context *cur = topContext;
    while (cur != nullptr) {
        Context *tmp = cur->prev;
        delete cur;
        cur = tmp;
    }
}

void ContextStack::pushContext(ContextType type) {
    Context *newTop = new Context(type, topContext);
    topContext = newTop;
}

void ContextStack::popContext() {
    Context *newTop = topContext->prev;
    delete topContext;
    topContext = newTop;
}

void ContextStack::addVar(Var var) {
    topContext->vars.push_back(var);
}

void ContextStack::addBlockVar(Var var) {
    Context *cur = topContext;
    while (cur != nullptr) {
        if (cur->type == CTX_BLOCK || cur->type == CTX_FUNC || cur->type ==
        CTX_GLOBAL) {
            cur->vars.push_back(var);
            return;
        }
        cur = cur->prev;
    }
}

void ContextStack::setConst(std::string name, std::string value) {
    topContext->constValues.insert({name, value});
}

void ContextStack::addFunc(FuncDef func) {
    topContext->funcs.push_back(func);
}

std::vector<Var> ContextStack::getLocalVars() const {
    std::vector<Var> out;
    Context *cur = topContext;
    while (cur != nullptr) {
        out.insert(out.end(), cur->vars.begin(), cur->vars.end());
        if (cur->type == CTX_FUNC)
            break;
        cur = cur->prev;
    }
    return out;
}

std::vector<Var> ContextStack::getGlobalVars() const {
    Context *cur = topContext;
    while (cur != nullptr) {
        if (cur->type == CTX_GLOBAL)
            return cur->vars;
        cur = cur->prev;
    }
    return {};
}

bool ContextStack::findVar(std::string name, Type &result) const {
    return findVarCtx(name, result, topContext);
}

bool ContextStack::findVarAll(std::string name, Type &result) const {
    Context *cur = topContext;
    while (cur != nullptr) {
        if (findVarCtx(name, result, cur))
            return true;
        cur = cur->prev;
    }
    return false;
}

bool ContextStack::findFunc(std::string name, std::vector<Type> types,
FuncDef &result) const {
    FuncDef *res;
    bool out = findFuncCtx(name, types, res, topContext);
    result = *res;
    return out;
}

bool ContextStack::findFuncAll(std::string name, std::vector<Type> types,
FuncDef &result) const {
    Context *cur = topContext;
    while (cur != nullptr) {
        FuncDef *res;
        if (findFuncCtx(name, types, res, cur)) {
            result = *res;
            return true;
        }
        cur = cur->prev;
    }
    return false;
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
    Context *cur = topContext;
    FuncDef *func;
    while (cur != nullptr) {
        if (findFuncCtx(name, types, func, cur)) {
            func->aliases.push_back(alias);
            break;
        }
        cur = cur->prev;
    }
}

std::map<std::string, std::string> ContextStack::getConstValues() const {
    std::map<std::string, std::string> out;
    Context *cur = topContext;
    while (cur != nullptr) {
        out.insert(cur->constValues.begin(), cur->constValues.end());
        cur = cur->prev;
    }
    return out;
}

void ContextStack::print() const {
    Context *cur = topContext;
    while (cur != nullptr) {
        std::cout << std::endl << "--- ";
        if (cur->type == CTX_BASIC)
            std::cout << "BASIC";
        if (cur->type == CTX_BLOCK)
            std::cout << "BLOCK";
        if (cur->type == CTX_FUNC)
            std::cout << "FUNC";
        if (cur->type == CTX_GLOBAL)
            std::cout << "GLOBAL";
        std::cout << " ---" << std::endl << "Variables: ";
        for (const Var &var : cur->vars)
            std::cout << var.name << " ";
        std::cout << std::endl << "Constant values: ";
        for (const std::pair<std::string, std::string> &val : cur->constValues)
            std::cout << "(" << val.first << ", " << val.second << ") ";
        std::cout << std::endl << "Functions: ";
        for (const FuncDef &func : cur->funcs) {
            std::cout << func.name << " (";
            for (const FuncAlias &alias : func.aliases)
                std::cout << (&alias == &*func.aliases.begin() ? "" : ", ")
                << alias.name;
            std::cout << ") ";
        }
        std::cout << std::endl;
        cur = cur->prev;
    }
    std::cout << std::endl;
}

bool ContextStack::findVarCtx(std::string name, Type &result, Context *ctx)
const {
    if (ctx == nullptr)
        return false;
    for (const Var &var : ctx->vars) {
        if (var.name == name) {
            result = var.type;
            return true;
        }
    }
    return false;
}

bool ContextStack::findFuncCtx(std::string name, std::vector<Type> types,
FuncDef *&result, Context *ctx) const {
    if (ctx == nullptr)
        return false;
    for (const FuncDef &func : ctx->funcs) {
        if (func.name == name && func.params.size() == types.size()) {
            bool success = true;
            for (unsigned int i = 0; i < types.size(); i++)
                if (!(types[i] >= func.params[i].type))
                    success = false;
            if (success) {
                result = (FuncDef *)&func;
                return true;
            }
        }
    }
    return false;
}