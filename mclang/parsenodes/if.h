
#ifndef __PARSENODE_IF_H__
#define __PARSENODE_IF_H__

#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

class BCManager;
class CodeBlockNode;

class IfNode : public ParseNode {

public:

    /**
     * Constructor
     * @param args A list of if-statement arguments, if there is an else at the
     * end this list should have one less element than `codeblocks`, otherwise
     * it should have the same amount
     * @param codeblocks Code inside the statement blocks in the if-statements
     * @param loc The location of the parse node
     */
    IfNode(std::vector<std::string> args, std::vector<CodeBlockNode *>
    codeblocks, Loc loc);

    /**
     * Destructor
     */
    virtual ~IfNode() override;

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

    /**
     * Check if this if-statement has an else-statement at the end or not
     * @return A boolean indicating if there is an else-statement at the end
     */
    bool hasElse() const;

private:

    // If-statement arguments
    std::vector<std::string> ifArgs;

    // Content of the statement blocks
    std::vector<CodeBlockNode *> codeblocks;

};

#endif