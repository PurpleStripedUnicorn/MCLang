
#ifndef __BCGEN_FUNCMAN_H__
#define __BCGEN_FUNCMAN_H__

#include "general/types.h"
#include <map>
#include <string>
#include <vector>

class BCManager;

/**
 * Function manager
 * Keeps track of defined and generated functions, and can generate bytecode to
 * call or generate functions
 */
class FuncManager {

public:

    /**
     * Constructor
     * @param bcman The main bytecode manager
     */
    FuncManager(BCManager &bcman);

    /**
     * Destructor
     */
    ~FuncManager();

private:

    // Main bytecode manager
    BCManager &bcman;

    /**
     * Function alias
     * This is a bytecode function that can be called, that has the correct
     * const values filled in inside the code
     */
    struct FuncAlias {
        std::string callname;
        std::vector<std::string> constValues;
    };

    /**
     * Function parameter
     * Contains the type and the parameter name
     */
    struct FuncParam {
        Type type;
        std::string name;
    };

    /**
     * Registered function type, contains information about aliases and input
     * types
     */
    struct Func {
        std::string name;
        std::vector<FuncParam> params;
        std::vector<FuncAlias> aliases;
    };

    // List of registered functions, also containing aliases
    std::map<std::string, std::vector<Func>> funcs;

};

#endif