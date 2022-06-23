
#ifndef __MANAGER_H__
#define __MANAGER_H__

#include <string>

/**
 * The compile manager stores compiler argument values, and keeps track of
 * things that need to be communicated between components
 */
class CompileManager {

public:

    /**
     * Constructor
     */
    CompileManager();

    /**
     * Destructor
     */
    ~CompileManager();

    // Namespace to use in the output
    std::string ns;

private:

};

#endif