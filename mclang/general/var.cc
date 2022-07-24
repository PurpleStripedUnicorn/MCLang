
#include "general/var.h"

Var::Var(std::string type, std::string name) : type(type), name(name),
isConst(false), constValue("") {

}

Var::Var(std::string type, std::string name, std::string constValue) :
type(type), name(name), isConst(true), constValue(constValue) {
    
}