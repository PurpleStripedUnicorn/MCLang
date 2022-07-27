
#ifndef __PARSENODE_CALL_H__
#define __PARSENODE_CALL_H__

#include "general/loc.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

class BCManager;

class CallNode : public ParseNode {

public:

    /**
     * Constructor
     * @param fname The name of the function to call
     * @param params The parameters given to the function call
     * @param loc The location of the parse node
     */
    CallNode(std::string fname, std::vector<ParseNode *> params, Loc loc);

    /**
     * Destructor
     */
    virtual ~CallNode() override;

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

private:

    // The name of the function to call
    std::string fname;

    // Passed function parameters
    std::vector<ParseNode *> params;

};

#endif