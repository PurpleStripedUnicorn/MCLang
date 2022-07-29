
#ifndef __PARSENODE_WORD_H__
#define __PARSENODE_WORD_H__

#include "general/loc.h"
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
    virtual void bytecode(BCManager &man) override;

    /**
     * Get the content of the word
     * @return The content as a string
     */
    std::string getContent() const;

protected:

    /**
     * Find a the value of the constant with the current variable's name
     * @param man The main bytecode manager
     * @return A string containing the value, or an empty string if nothing was
     * found
     */
    std::string findConstValue(BCManager &man) const;

    /**
     * Check if the variable is initialized
     * @param man The main bytecode manager
     * @param varType The variable type will be put here
     * @return A boolean indicating if the variable was initialized
     */
    bool wasInitialized(BCManager &man, Type &varType) const;

    // The word content
    std::string content;

};

#endif