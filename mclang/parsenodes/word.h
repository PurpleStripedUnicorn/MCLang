
#ifndef __PARSENODE_WORD_H__
#define __PARSENODE_WORD_H__

#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

class BCManager;

class WordNode : public ParseNode {

public:

    /**
     * Constructor
     * @param content The word content
     * @param loc The location of the parse node
     */
    WordNode(std::string content, Loc loc);

    /**
     * Destructor
     */
    virtual ~WordNode() override;

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
     * Get the content of the word
     * @return The content as a string
     */
    std::string getContent() const;

protected:

    // The word content
    std::string content;

};

#endif