
#ifndef __BCGEN_INSTR_H__
#define __BCGEN_INSTR_H__

#include <string>

/**
 * Bytecode instruction type
 * @warning Changing the order of these instructions may break things in the
 * bytecode converter!
 */
enum BCInstrType {
    // Error type
    INSTR_ERR,
    // Normal command: arg1 = command
    INSTR_CMD,
    // Execute-statement with function call: arg1 = type + args, arg2 = function
    // to call
    INSTR_EXEC_CALL,
    // Call a function: arg1 = function name
    INSTR_CALL,
    // Set a variable to a certain value: arg1 = variable name, arg2 = value
    INSTR_SET,
    // Push a value of a variable to the stack: arg1 = variable name
    INSTR_PUSH,
    // Pop the top value from the stack
    INSTR_POP,
    // Get the top value on the stack, does not modify the stack: arg1 =
    // variable to put the value in
    INSTR_TOP,
    // Copy value from arg2 into arg1
    INSTR_COPY,
    // Arithmatic, applies operation to two arguments and then puts result in
    // arg1
    INSTR_ADD, INSTR_SUB, INSTR_MUL, INSTR_DIV, INSTR_MOD,
    // Arithmatic with the second argument being a number
    INSTR_ADDI, INSTR_SUBI, INSTR_MULI, INSTR_DIVI, INSTR_MODI,
    // Comparison operators, applies operation to two arguments (arg1 = left,
    // arg2 = right) and then puts the result in arg1 (either 0 or 1)
    INSTR_LT, INSTR_LTE, INSTR_GT, INSTR_GTE, INSTR_EQ, INSTR_NEQ,
    // Not operation, if arg1 is not 0, it becomes 0. If it is 0 it becomes 1
    INSTR_NOT,
    // Other logical operations, interpret every value other than 0 as true and
    // result is a 0 or 1
    INSTR_AND, INSTR_OR
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