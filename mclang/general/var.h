
#ifndef __VARIABLE_H__
#define __VARIABLE_H__

#include <string>

/**
 * A variable with type and possible constant value
 */
struct Var {
    Var(std::string type, std::string name);
    std::string type, name;
};

/**
 * A constant variable, with value attached
 */
struct ConstVar : public Var {
    ConstVar(std::string type, std::string name, std::string value);
    std::string value;
};

#endif