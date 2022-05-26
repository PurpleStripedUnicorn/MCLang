
#ifndef __PARSENODE_FUNC_H__
#define __PARSENODE_FUNC_H__

#include "parsenodes/codeblock.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

class FuncNode : public ParseNode {

public:

    /**
     * Constructor
     * @param name Name of the function
     * @param codeblock Code inside the function
     */
    FuncNode(std::string name, CodeBlockNode *codeblock);

    /**
     * Destructor
     */
    virtual ~FuncNode() override;

    /**
     * Get the children of this parse node
     * @return A vector with pointers to the child nodes
     */
    virtual std::vector<ParseNode *> children() const override;

private:

    // Name of the function
    std::string name;

    // Content of the function
    CodeBlockNode *codeblock;

};

#endif