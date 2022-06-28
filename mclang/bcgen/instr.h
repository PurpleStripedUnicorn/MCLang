
#ifndef __BCGEN_INSTR_H__
#define __BCGEN_INSTR_H__

#include <string>

enum BCInstrType {
    // Error type
    INSTR_ERR,
    // Normal command
    INSTR_CMD,
    // Execute-statement with function call: arg1 = type + args, arg2 = function
    // to call
    INSTR_EXEC_CALL,
    // Set a variable to a certain value: arg1 = variable name, arg2 = value
    INSTR_SET
};

struct BCInstr {
    BCInstr();
    BCInstr(BCInstrType type);
    BCInstr(BCInstrType type, std::string arg1);
    BCInstr(BCInstrType type, std::string arg1, std::string arg2);
    BCInstrType type;
    std::string arg1, arg2;
};

#endif