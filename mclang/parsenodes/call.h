
#ifndef __PARSENODE_CALL_H__
#define __PARSENODE_CALL_H__

#include "bcgen/bcgen.h"
#include "parsenodes/parsenode.h"
#include <string>
#include <vector>

class CallNode : public ParseNode {

public:

    /**
     * Constructor
     * @param fname The name of the function to call
     * @param props General parse node properties
     */
    CallNode(std::string fname, ParseNodeProps props);

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
    virtual void bytecode(BCManager &man) const override;

private:

    // The name of the function to call
    std::string fname;

};

#endif