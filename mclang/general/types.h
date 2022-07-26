
#ifndef __TYPES_H__
#define __TYPES_H__

#include <map>
#include <string>

/**
 * Basic type
 */
enum BaseType {
    TYPE_VOID, TYPE_INT, TYPE_BOOL, TYPE_STR
};

// Base type conversion to string
const std::map<BaseType, std::string> baseTable = {
    {TYPE_VOID, "void"},
    {TYPE_INT, "int"},
    {TYPE_BOOL, "bool"},
    {TYPE_STR, "str"}
};

/**
 * Extended type, which also contains info about being const
 */
struct Type {
    Type();
    Type(std::string type);
    bool operator==(const Type &other) const;
    bool operator!=(const Type &other) const;
    std::string str() const;
    BaseType base;
    bool isConst;
};

#endif