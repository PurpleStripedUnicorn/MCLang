
#ifndef __PARSENODE_CALL_H__
#define __PARSENODE_CALL_H__

#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

class BCManager;

class NSNode : public ParseNode {

public:

    /**
     * Constructor
     * @param nsName The name of the namespace
     * @param props General parse node properties
     */
    NSNode(std::string nsName, ParseNodeProps props);

    /**
     * Destructor
     */
    virtual ~NSNode() override;

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

    // The name of the function to call
    std::string nsName;

};

#endif