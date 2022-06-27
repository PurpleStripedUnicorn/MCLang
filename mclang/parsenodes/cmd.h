
#ifndef __PARSENODE_CMD_H__
#define __PARSENODE_CMD_H__

#include "bcgen/bcgen.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

class CmdNode : public ParseNode {

public:

    /**
     * Constructor
     * @param cmd A string with the command
     */
    CmdNode(std::string cmd, ParseNodeProps props);

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
    virtual void bytecode(BCManager &man) const override;

private:

    // The command as a string, without '/'
    std::string cmd;

};

#endif