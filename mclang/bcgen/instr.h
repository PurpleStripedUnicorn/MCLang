
#ifndef __BCGEN_INSTR_H__
#define __BCGEN_INSTR_H__

#include <string>

enum BCInstrType {
    // Error type
    INSTR_ERR,
    // Normal command: arg1 = command
    INSTR_CMD,
    // Execute-statement with function call: arg1 = type + args, arg2 = function
    // to call
    INSTR_EXEC_CALL,
    // Set a variable to a certain value: arg1 = variable name, arg2 = value
    INSTR_SET,
    // Push a value of a variable to the stack: arg1 = variable name
    INSTR_PUSH,
    // Pop the top value from the stack
    INSTR_POP,
    // Get the top value on the stack, does not modify the stack: arg1 =
    // variable to put the value in
    INSTR_TOP
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