
#ifndef __PARSENODE_NUM_H__
#define __PARSENODE_NUM_H__

#include "general/loc.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

class BCManager;

class NumNode : public ParseNode {

public:

    /**
     * Constructor
     * @param content The number content
     * @param loc The location of the parse node
     */
    NumNode(std::string content, Loc loc);

    /**
     * Destructor
     */
    virtual ~NumNode() override;

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
     * Get the content of the number
     * @return The content as a string
     */
    std::string getContent() const;

protected:

    // The number content
    std::string content;

};

#endif