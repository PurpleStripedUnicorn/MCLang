
#include "bcgen/bcgen.h"
#include "bcgen/context.h"
#include "errorhandle/handle.h"
#include "general/funcdef.h"
#include "general/loc.h"
#include "general/types.h"
#include "general/var.h"
#include "parsenodes/codeblock.h"
#include "parsenodes/func.h"
#include "parsenodes/parsenode.h"
#include <map>
#include <string>
#include <vector>

FuncNode::FuncNode(Type retType, std::string name, std::vector<Param> params,
CodeBlockNode *codeblock, Loc loc) : ParseNode(PNODE_FUNC, loc),
retType(retType), name(name), params(params), codeblock(codeblock) {

}

std::vector<ParseNode *> FuncNode::children() const {
    return {(ParseNode *)codeblock};
}

FuncNode::~FuncNode() {
    delete codeblock;
}

void FuncNode::bytecode(BCManager &man) {
    bcman = &man;
    // TODO: Implement non-void functions
    if (retType != Type("void"))
        MCLError(1, "Non-void functions are not supported", loc);
    if (hasNameConflict())
        MCLError(1, "Function with same name and parameter types was already "
        "defined", loc);
    // Add function definition to context
    man.ctx.back().funcs.push_back(this);
    // If there are no parameters, the function should be generated beforehand
    if (params.empty())
        bytecode({});
}

std::string FuncNode::bytecode(std::vector<std::string> constValues) {
    // Check if this function was already generated with the given constants
    std::string callname;
    if (findAlias(constValues, callname))
        return callname;
    // Add instructions to set constant variable values
    for (const Context &ctx : bcman->ctx)
        for (const std::pair<std::string, std::string> &val : ctx.constValues)
            bcman->write(BCInstr(INSTR_SET, val.first, val.second));
    // Functions without params will receive original name, other will get
    // some random name
    bcman->addFunc(params.empty() ? name : "");
    callname = bcman->topFunc()->name;
    // Add alias of function that will be generated right now
    aliases.push_back({bcman->topFunc()->name, constValues});
    bcman->ctx.push_back(Context());
    initGlobalVars();
    initParams(constValues);
    codeblock->bytecode(*bcman);
    bcman->ctx.pop_back();
    bcman->popFunc();
    return callname;
}

Type FuncNode::getReturnType() const {
    return retType;
}

std::string FuncNode::getName() const {
    return name;
}

std::vector<Type> FuncNode::getParamTypes() const {
    std::vector<Type> out;
    for (const Param &param : params)
        out.push_back(param.type);
    return out;
}

bool FuncNode::acceptTypes(std::vector<Type> types) const {
    if (types.size() != params.size())
        return false;
    for (unsigned int i = 0; i < types.size(); i++)
        if (!(types[i] >= params[i].type))
            return false;
    return true;
}

bool FuncNode::findAlias(std::vector<std::string> constValues, std::string
&result) const {
    for (const FuncAlias &alias : aliases) {
        if (alias.constValues == constValues) {
            result = alias.callname;
            return true;
        }
    }
    return false;
}

bool FuncNode::hasNameConflict() const {
    for (Context &ctx : bcman->ctx)
        for (FuncNode *func : ctx.funcs)
            if (hasNameConflict(func))
                return true;
    return false;
}

bool FuncNode::hasNameConflict(FuncNode *other) const {
    if (other->getName() != name)
        return false;
    std::vector<Type> otherTypes = other->getParamTypes();
    if (otherTypes.size() != params.size())
        return false;
    for (unsigned int i = 0; i < otherTypes.size(); i++)
        if (otherTypes[i] != params[i].type)
            return false;
    return true;
}

void FuncNode::initGlobalVars() {
    for (const Context &ctx : bcman->ctx)
        for (const Var &var : ctx.vars)
            bcman->write(BCInstr(INSTR_ADDI, var.name, "0"));
}

void FuncNode::initParams(std::vector<std::string> constValues) {
    // Add parameters to context and copy them to the correct variables
    unsigned int constCount = 0;
    for (unsigned int i = 0; i < params.size(); i++) {
        if (params[i].type.isConst) {
            bcman->write(BCInstr(INSTR_SET, params[i].name,
            constValues[constCount]));
            constCount++;
            bcman->ctx.back().constValues[params[i].name] =
            constValues[constCount];
        } else {
            bcman->write(BCInstr(INSTR_COPY, params[i].name, "__param"
            + std::to_string(i)));
        }
        Type tmp;
        for (const Context &ctx : bcman->ctx)
            for (const Var &var : ctx.vars)
                if (var.name == params[i].name)
                    MCLError(1, "Parameter name is already defined somewhere "
                    "else.", loc);
        bcman->ctx.back().vars.push_back(Var(params[i].type, params[i].name));
    }
}