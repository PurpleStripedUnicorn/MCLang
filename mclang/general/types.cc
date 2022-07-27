
#include "general/types.h"
#include <map>
#include <string>

Type::Type() {

}

Type::Type(std::string type) {
    if (type.substr(0, 6) == "const ") {
        isConst = true;
        if (type.size() >= 6)
            type = type.substr(6);
    } else {
        isConst = false;
    }
    if (type == "void")
        base = TYPE_VOID;
    if (type == "int")
        base = TYPE_INT;
    if (type == "bool")
        base = TYPE_BOOL;
    if (type == "str")
        base = TYPE_STR;
}

bool Type::operator==(const Type &other) const {
    return base == other.base && isConst == other.isConst;
}

bool Type::operator!=(const Type &other) const {
    return !operator==(other);
}

bool Type::sameBase(const Type &other) const {
    return base == other.base;
}

std::string Type::str() const {
    std::string out = "";
    if (isConst)
        out.append("const ");
    out.append(baseTable.find(base)->second);
    return out;
}