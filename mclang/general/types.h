
#ifndef __TYPES_H__
#define __TYPES_H__

/**
 * Basic type
 */
enum BaseType {
    TYPE_VOID, TYPE_INT, TYPE_BOOL, TYPE_STR
};

/**
 * Extended type, which also contains info about being const
 */
struct Type {
    bool operator==(const Type &other) const;
    bool operator!=(const Type &other) const;
    BaseType base;
    bool isConst;
    // Easy-to-access types
    const static Type tVoid, tInt, tBool, tConstInt, tConstBool, tConstStr;
};

#endif