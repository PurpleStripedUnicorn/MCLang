
#ifndef __PARSENODE_NS_H__
#define __PARSENODE_NS_H__

#include "general/loc.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

class BCManager;

class NSNode : public ParseNode {

public:

    /**
     * Constructor
     * @param nsName The name of the namespace
     * @param loc The location of the parse node
     */
    NSNode(std::string nsName, Loc loc);

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
    virtual void bytecode(BCManager &man) override;

    /**
     * Get the namespace name
     * @return The namespace name
     */
    std::string getName() const;

private:

    // The name of the function to call
    std::string nsName;

};

#endif