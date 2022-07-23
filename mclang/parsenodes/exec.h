
#ifndef __PARSENODE_EXEC_H__
#define __PARSENODE_EXEC_H__

#include "general/loc.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

class BCManager;
class CodeBlockNode;

class ExecNode : public ParseNode {

public:

    /**
     * Constructor
     * @param type The subcommand type
     * @param args The subcommand arguments
     * @param codeblock Code inside the statement block
     * @param loc The location of the parse node
     */
    ExecNode(std::string type, std::string args, CodeBlockNode *codeblock,
    Loc loc);

    /**
     * Destructor
     */
    virtual ~ExecNode() override;

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

    // Subcommand type
    std::string execType;

    // Subcommand arguments
    std::string execArgs;

    // Content of the statement block
    CodeBlockNode *codeblock;

};

#endif