
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
    copyContextStack();
    // Add function definition to context
    man.funcs.push(this);
    // If there are no parameters, the function should be generated beforehand
    // with its original name. If it has parameters it should be generated with
    // default constant values, to check for syntax errors
    if (params.empty()) {
        bytecode({});
    } else {
        std::vector<std::string> constValues;
        for (const Param &param : params) {
            if (param.type.isConst) {
                if (param.type == Type("const str"))
                    constValues.push_back("");
                else 
                    constValues.push_back("0");
            }
        }
        bytecode(constValues);
    }
}

std::string FuncNode::bytecode(std::vector<std::string> constValues) {
    // TODO: Implement predefined functions, then account for recursive calls of
    // constant values (put some limit on it)
    // Check if this function was already generated with the given constants
    std::string callname;
    if (findAlias(constValues, callname))
        return callname;
    // Functions without params will receive original name, other will get
    // some random name
    bcman->addFunc(params.empty() ? name : "");
    callname = bcman->topFunc()->name;
    // Add alias of function that will be generated right now
    aliases.push_back({callname, constValues});
    // Temporary set current context stack aside
    ContextStack curCtx = bcman->ctx;
    bcman->ctx = ctxStore;
    // Add instructions to set constant variable values
    for (const std::pair<std::string, std::string> &val :
    bcman->ctx.getConstValues())
        bcman->write(BCInstr(INSTR_SET, val.first, val.second));
    bcman->ctx.push();
    initGlobalVars();
    initParams(constValues);
    codeblock->bytecode(*bcman);
    bcman->ctx.pop();
    bcman->popFunc();
    bcman->ctx = curCtx;
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
    for (FuncNode *func : bcman->funcs.getFuncs())
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
    for (const Var &var : bcman->ctx.getVars())
        bcman->write(BCInstr(INSTR_ADDI, var.name, "0"));
}

void FuncNode::initParams(std::vector<std::string> constValues) {
    // Add parameters to context and copy them to the correct variables
    unsigned int constCount = 0;
    for (unsigned int i = 0; i < params.size(); i++) {
        if (params[i].type.isConst) {
            bcman->write(BCInstr(INSTR_SET, params[i].name,
            constValues[constCount]));
            bcman->ctx.setConst(params[i].name, constValues[constCount]);
            constCount++;
        } else {
            bcman->write(BCInstr(INSTR_COPY, params[i].name, "__param"
            + std::to_string(i)));
        }
        Var var(Type(), "??");
        if (bcman->ctx.findVar(params[i].name, var))
            MCLError(1, "Parameter name is already defined somewhere else.",
            loc);
        bcman->ctx.pushVar(Var(params[i].type, params[i].name));
    }
}

void FuncNode::copyContextStack() {
    ctxStore = bcman->ctx;
}