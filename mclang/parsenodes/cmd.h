
#ifndef __PARSENODE_CMD_H__
#define __PARSENODE_CMD_H__

#include "general/loc.h"
#include "parsenodes/parsenode.h"
#include <set>
#include <string>
#include <vector>

class BCManager;

/**
 * List of known minecraft commands
 * NOTE: Does not register version numbers!
 * NOTE: Only lists commands that need permission level 2 or lower
 */
std::set<std::string> knownCmds = {
    "advancement", "attribute", "bossbar", "clear", "clone", "data", "datapack",
    "defaultgamemode", "difficulty", "effect", "enchant", "execute",
    "experience", "fill", "forceload", "function", "gamemode", "gamerule",
    "give", "help", "item", "kill", "locate", "locatebiome", "loot", "msg",
    "particle", "place", "playsound", "recipe", "reload", "replaceitem", "say",
    "schedule", "scoreboard", "seed", "setblock", "setworldspawn", "spawnpoint",
    "spectate", "spreadplayers", "stopsound", "summon", "tag", "team",
    "teleport", "tell", "tellraw", "time", "title", "tm", "tp", "triger", "w",
    "weather", "worldborder", "xp"
};

class CmdNode : public ParseNode {

public:

    /**
     * Constructor
     * @param cmd A string with the command
     * @param loc The location of the parse node
     */
    CmdNode(std::string cmd, Loc loc);

    /**
     * Destructor
     */
    virtual ~CmdNode() override;

    /**
     * Get the children of this parse node
     * @return A vector with pointers to the child nodes
     */
    virtual std::vector<ParseNode *> children() const override;

    /**
     * Generate bytecode for this parse node
     * @param man The main bytecode manager
     */
    virtual void bytecode(BCManager &man) override;

private:

    /**
     * Check if the command is a known command
     * @return A boolean indicating if the commands in listed in `knownCmds`
     */
    bool isKnownCmd() const;

    /**
     * Get the primary command being executed
     * @return A string containing all of the characters in `cmd` before the
     * first space
     */
    std::string primaryCmd() const;

    // The command as a string, without '/'
    std::string cmd;

};

#endif