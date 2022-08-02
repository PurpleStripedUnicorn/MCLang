
#include "bcgen/bcgen.h"
#include "compiler/compiler.h"
#include "errorhandle/handle.h"
#include "general/loc.h"
#include "general/types.h"
#include "parsenodes/func.h"
#include "parsenodes/namespace.h"
#include "parsenodes/parsenode.h"
#include "parsenodes/program.h"
#include <vector>

ProgramNode::ProgramNode(std::vector<ParseNode *> childNodes, Loc loc) :
ParseNode(PNODE_PROGRAM, loc), childNodes(childNodes)
{

}

ProgramNode::~ProgramNode() {
    for (unsigned int i = 0; i < childNodes.size(); i++)
        delete childNodes[i];
}

std::vector<ParseNode *> ProgramNode::children() const {
    return childNodes;
}

void ProgramNode::bytecode(BCManager &man) {
    applyGlobalSettings(man);
    registerGlobalVars(man);
    registerFunctions(man);
    generateFunctions(man);
    man.ret = {Type("void"), ""};
}

void ProgramNode::applyGlobalSettings(BCManager &man) const {
    // Search for namespace setting override
    bool foundNamespace = false;
    for (unsigned int i = 0; i < childNodes.size(); i++) {
        if (childNodes[i]->getType() == PNODE_NAMESPACE) {
            if (foundNamespace)
                MCLError(0, "Namespace set multiple times, only last setting "
                "is used", childNodes[i]->getLoc());
            man.comp->ns = ((NSNode *)childNodes[i])->getName();
            foundNamespace = true;
        }
    }
}

void ProgramNode::registerGlobalVars(BCManager &man) const {
    for (ParseNode *node : childNodes)
        if (node->getType() == PNODE_GLOBALVAR)
            node->bytecode(man);
}

void ProgramNode::registerFunctions(BCManager &man) const {
    for (ParseNode *node : childNodes) {
        if (node->getType() == PNODE_FUNC) {
            ((FuncNode *)node)->checkNameConflicts(man);
            man.funcs.push((FuncNode *)node);
        }
    }
}

void ProgramNode::generateFunctions(BCManager &man) const {
    for (FuncNode *func : man.funcs.getFuncs())
        func->addGenerationEntry(man, func->defaultConstValues());
    bool foundUngenerated = true;
    unsigned int i = 0;
    while (foundUngenerated && i < GEN_RECURION_LIMIT) {
        foundUngenerated = false;
        for (FuncNode *func : man.funcs.getFuncs())
            if (func->hasUngeneratedEntries())
                func->generateEntries(man), foundUngenerated = true;
        i++;
    }
    if (foundUngenerated)
        recursionError(man);
}

void ProgramNode::recursionError(BCManager &man) const {
    std::string funcList = "";
    for (FuncNode *func : man.funcs.getFuncs())
        if (func->hasUngeneratedEntries())
            funcList += std::string(funcList == "" ? "" : ", ") + "\""
            + func->getName() + "\"";
    MCLError(1, "Generation recursion limit of " + std::to_string(
    GEN_RECURION_LIMIT) + " reached. Function(s) " + funcList + " could not be "
    "generated!");
}