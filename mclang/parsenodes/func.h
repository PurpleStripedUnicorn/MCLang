
#ifndef __PARSENODE_FUNC_H__
#define __PARSENODE_FUNC_H__

#include "general/loc.h"
#include "general/types.h"
#include "general/var.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

class BCManager;
class CodeBlockNode;

class FuncNode : public ParseNode {

public:

    /**
     * Constructor
     * @param retType THe return type of the function
     * @param name Name of the function
     * @param params The parameters of the function
     * @param codeblock Code inside the function
     * @param loc The location of the parse node
     */
    FuncNode(Type retType, std::string name, std::vector<Param> params,
    CodeBlockNode *codeblock, Loc loc);

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

    // Return type of the function
    Type retType;

    // Name of the function
    std::string name;

    // Function parameters
    std::vector<Param> params;

    // Content of the function
    CodeBlockNode *codeblock;

};

#endif