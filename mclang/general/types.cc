
#include "general/types.h"

bool Type::operator==(const Type &other) const {
    return base == other.base && isConst == other.isConst;
}

bool Type::operator!=(const Type &other) const {
    return !operator==(other);
}

const Type Type::tVoid = {TYPE_VOID, false},
Type::tInt = {TYPE_INT, false},
Type::tBool = {TYPE_BOOL, false},
Type::tConstInt = {TYPE_INT, true},
Type::tConstBool = {TYPE_BOOL, true},
Type::tConstStr = {TYPE_STR, true};