
#ifndef __VARIABLE_H__
#define __VARIABLE_H__

#include <string>

/**
 * A variable with type and possible constant value
 */
struct Var {
    Var(std::string type, std::string name);
    Var(std::string type, std::string name, std::string constValue);
    std::string type, name;
    bool isConst;
    std::string constValue;
};

#endif