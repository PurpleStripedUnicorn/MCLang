
#ifndef __PARSENODE_PROGRAM_H__
#define __PARSENODE_PROGRAM_H__

#include "parsenodes/parsenode.h"
#include <vector>

class ProgramNode : public ParseNode {

public:

    /**
     * Constructor
     */
    ProgramNode(std::vector<ParseNode *> childNodes);

    /**
     * Destructor
     */
    virtual ~ProgramNode() override;

    /**
     * Get the children of this parse node
     * @return A vector with pointers to the child nodes
     */
    virtual std::vector<ParseNode *> children() const override;

private:

    // Child nodes of the main program
    std::vector<ParseNode *> childNodes;

};

#endif