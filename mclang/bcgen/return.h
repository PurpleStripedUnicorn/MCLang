
#ifndef __BCGEN_RETURN_H__
#define __BCGEN_RETURN_H__

#include "general/types.h"
#include <string>

/**
 * Return object, contains type and value, which can be a variable name or
 * constant value
 */
struct Return {
    Type type;
    std::string value;
};

#endif