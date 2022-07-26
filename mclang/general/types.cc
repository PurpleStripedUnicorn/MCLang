
#include "general/types.h"

const Type Type::tVoid = {TYPE_VOID, false},
Type::tInt = {TYPE_INT, false},
Type::tBool = {TYPE_BOOL, false},
Type::tConstInt = {TYPE_INT, true},
Type::tConstBool = {TYPE_BOOL, true},
Type::tConstStr = {TYPE_STR, true};