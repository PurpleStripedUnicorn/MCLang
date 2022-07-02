
#ifndef __PARSENODE_FUNC_H__
#define __PARSENODE_FUNC_H__

#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

class BCManager;
class CodeBlockNode;

class FuncNode : public ParseNode {

public:

    /**
     * Constructor
     * @param name Name of the function
     * @param codeblock Code inside the function
     */
    FuncNode(std::string name, CodeBlockNode *codeblock, ParseNodeProps props);

    /**
     * Destructor
     */
    virtual ~FuncNode() override;

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

    // Name of the function
    std::string name;

    // Content of the function
    CodeBlockNode *codeblock;

};

#endif