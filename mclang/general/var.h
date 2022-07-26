
#ifndef __VARIABLE_H__
#define __VARIABLE_H__

#include "general/types.h"
#include <string>

/**
 * A variable with type and possible constant value
 */
struct Var {
    Var(Type type, std::string name);
    Type type;
    std::string name;
};

/**
 * A constant variable, with value attached
 */
struct ConstVar : public Var {
    ConstVar(Type type, std::string name, std::string value);
    std::string value;
};

#endif