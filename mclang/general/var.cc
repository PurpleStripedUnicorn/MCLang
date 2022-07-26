
#include "general/var.h"

Var::Var(std::string type, std::string name) : type(type), name(name) {

}

ConstVar::ConstVar(std::string type, std::string name, std::string value) :
Var(type, name), value(value) {
    
}