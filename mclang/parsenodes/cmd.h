
#ifndef __PARSENODE_CMD_H__
#define __PARSENODE_CMD_H__

#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

class CmdNode : public ParseNode {

public:

    /**
     * Constructor
     * @param cmd A string with the command
     */
    CmdNode(std::string cmd);

    /**
     * Destructor
     */
    virtual ~CmdNode() override;

    /**
     * Get the children of this parse node
     * @return A vector with pointers to the child nodes
     */
    virtual std::vector<ParseNode *> children() const override;

private:

    // The command as a string, without '/'
    std::string cmd;

};

#endif