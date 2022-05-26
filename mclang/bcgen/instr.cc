
#include "bcgen/instr.h"

BCInstr::BCInstr() : type(INSTR_ERR), arg1(""), arg2("") {

}

BCInstr::BCInstr(BCInstrType type) : type(type), arg1(""), arg2("") {

}

BCInstr::BCInstr(BCInstrType type, std::string arg1) : type(type), arg1(arg1),
arg2("") {

}

BCInstr::BCInstr(BCInstrType type, std::string arg1, std::string arg2) :
type(type), arg1(arg1), arg2(arg2) {

}