
#ifndef __PARSENODE_PROGRAM_H__
#define __PARSENODE_PROGRAM_H__

#include "bcgen/bcgen.h"
#include "bcgen/instr.h"
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

    /**
     * Generate bytecode for this parse node
     * @param man The main bytecode manager
     */
    virtual void bytecode(BCManager &man) const override;

private:

    // Child nodes of the main program
    std::vector<ParseNode *> childNodes;

};

#endif