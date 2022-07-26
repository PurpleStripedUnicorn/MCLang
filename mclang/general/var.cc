
#include "general/var.h"

Var::Var(Type type, std::string name) : type(type), name(name) {

}

ConstVar::ConstVar(Type type, std::string name, std::string value) :
Var(type, name), value(value) {
    
}