
#include "bcgen/bcgen.h"
#include "bcgen/instr.h"
#include "errorhandle/handle.h"
#include "general/loc.h"
#include "general/types.h"
#include "parsenodes/cmd.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

CmdNode::CmdNode(std::string cmd, Loc loc) : ParseNode(PNODE_CMD, loc), cmd(cmd)
{

}

CmdNode::~CmdNode() {
    
}

std::vector<ParseNode *> CmdNode::children() const {
    return {};
}

void CmdNode::bytecode(BCManager &man) {
    replaceConstants(man);
    if (primaryCmd() == "function")
        MCLError(0, "Raw function call insertion has undefined behaviour, use "
        "normal function call instead!", loc);
    if (primaryCmd() == "execute")
        MCLError(0, "Raw usage of execute can replaced by execute-statements.",
        loc);
    if (!isKnownCmd())
        MCLError(0, "The given command \"" + primaryCmd() + "\" is not known "
        "to be a valid command.", loc);
    man.write(BCInstr(INSTR_CMD, processedCmd));
    man.ret.type = Type("void");
    man.ret.value = "";
}

bool CmdNode::isKnownCmd() const {
    return knownCmds.count(primaryCmd()) > 0;
}

std::string CmdNode::primaryCmd() const {
    std::string out;
    unsigned int i = 0;
    while (i < cmd.size() && processedCmd[i] != ' ')
        out.push_back(processedCmd[i]), i++;
    return out;
}

void CmdNode::replaceConstants(const BCManager &man) {
    std::string out = "";
    unsigned int i = 0;
    while (i < cmd.size()) {
        if (i + 1 < cmd.size() && cmd[i] == '{' && cmd[i + 1] == '{')
            i += 2, out.append(readInConstant(man, i));
        else
            out.push_back(cmd[i]), i++;
    }
    processedCmd = out;
}

std::string CmdNode::readInConstant(const BCManager &man, unsigned int &i)
const {
    std::string constName = "";
    while (i < cmd.size() && isAlphaNumUS(cmd[i]))
        constName.push_back(cmd[i]), i++;
    if (i >= cmd.size() || cmd[i] != '}')
        return constName;
    i++;
    if (i >= cmd.size() || cmd[i] != '}')
        return constName + "}";
    i++;
    // Search for constant name definition
    std::string value = man.ctx.getConstValue(constName);
    if (value == "")
        return constName + "}}";
    return value;
}

bool CmdNode::isAlphaNumUS(char c) const {
    return c == '_' || ('A' <= c && c <= 'Z') || ('a' <= c && c <= 'z');
}