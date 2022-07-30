
#ifndef __PARSENODE_BOOl_H__
#define __PARSENODE_BOOL_H__

#include "general/loc.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

class BCManager;

class BoolNode : public ParseNode {

public:

    /**
     * Constructor
     * @param content The boolean content
     * @param loc The location of the parse node
     */
    BoolNode(std::string content, Loc loc);

    /**
     * Destructor
     */
    virtual ~BoolNode() override;

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
     * Get the content of the boolean
     * @return The content as a string
     */
    std::string getContent() const;

protected:

    // The number content
    std::string content;

};

#endif