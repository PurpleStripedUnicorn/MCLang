
#ifndef __PARSENODE_CODEBLOCK_H__
#define __PARSENODE_CODEBLOCK_H__

#include "bcgen/bcgen.h"
#include "bcgen/instr.h"
#include "parsenodes/parsenode.h"
#include <vector>

class CodeBlockNode : public ParseNode {

public:

    /**
     * Constructor
     * @param childNodes The lines of code inside the code block
     */
    CodeBlockNode(std::vector<ParseNode *> childNodes);

    /**
     * Destructor
     */
    virtual ~CodeBlockNode() override;

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

    // Lines of code inside the code block
    std::vector<ParseNode *> childNodes;

};

#endif