
#ifndef __PARSENODE_CODEBLOCK_H__
#define __PARSENODE_CODEBLOCK_H__

#include "general/loc.h"
#include "parsenodes/parsenode.h"
#include <vector>

class BCManager;

class CodeBlockNode : public ParseNode {

public:

    /**
     * Constructor
     * @param childNodes The lines of code inside the code block
     * @param loc The location of the parse node
     */
    CodeBlockNode(std::vector<ParseNode *> childNodes, Loc loc);

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
    virtual void bytecode(BCManager &man) override;

private:

    // Lines of code inside the code block
    std::vector<ParseNode *> childNodes;

};

#endif