
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
     * @param value The boolean value
     * @param loc The location of the parse node
     */
    BoolNode(bool value, Loc loc);

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
     * Get the boolean value
     * @return The value, as a C++ boolean
     */
    bool getValue() const;

protected:

    // The boolean value
    bool value;

};

#endif