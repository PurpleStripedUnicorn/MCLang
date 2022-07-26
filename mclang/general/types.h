
#ifndef __TYPES_H__
#define __TYPES_H__

enum BaseType {
    TYPE_VOID, TYPE_INT, TYPE_BOOL
};

struct Type {
    BaseType base;
    bool isConst;
};

#endif