
#ifndef __BCCONVERT_H__
#define __BCCONVERT_H__

#include <string>
#include <vector>

#define STACK_CREATE "execute unless data storage mclang:stack stack run data "\
"modify storage mclang:stack stack set value []"

struct BCFunc;
struct BCInstr;
class Compiler;

/**
 * Raw commands are stored in these functions
 */
struct CmdFunc {
    CmdFunc(std::string name);
    std::string name;
    std::vector<std::string> cmdList;
};

/**
 * Manager object that converts bytecode to raw commands
 */
class BCConverter {

public:

    /**
     * Constructor
     * @param data A pointer to the list of functions with contained in them
     * bytecode instructions
     * @param comp The compiler component
     * @note Data can be retrieved by using the compiler component
     */
    BCConverter(Compiler *comp);

    /**
     * Destructor
     */
    ~BCConverter();

    /**
     * Convert the bytecode to commands and get the result
     * @return A vector with functions containing the raw commands
     */
    std::vector<CmdFunc> getRawCommands();

private:

    /**
     * Convert a given bytecode function to raw commands
     * @param func The function to convert
     * @return A vector of strings which contain the commands
     */
    std::vector<std::string> convertFunc(const BCFunc &func);

    /**
     * Convert a single bytecode instruction to raw commands
     * @param instr The instruction to convert
     * @return A vector of strings which contain the commends
     */
    std::vector<std::string> convertInstr(const BCInstr &instr);

    /**
     * Convert a raw inserted command, also checks if the command calls a
     * function, to show a warning
     * @param instr The instruction to convert, assumed to be of the inserted
     * command type
     * @return A vector of strings containing the output commands
     */
    std::vector<std::string> convertCmd(BCInstr instr) const;

    /**
     * Convert a custom execute call to a raw command
     * @param instr The instruction to convert, assumed to be of the exec-call
     * type
     * @return A vector of strings containing the output commands
     */
    std::vector<std::string> convertExecCall(BCInstr instr) const;

    /**
     * Convert a variable assignment
     * @param instr The instruction to convert, assumed to be of the "set"
     * type
     * @return A vector of strings containing the output commands
     */
    std::vector<std::string> convertSet(BCInstr instr) const;

    /**
     * Convert a stack operation instruction
     * @param instr The instruction to convert, assumed to be a stack operation
     * instruction
     * @return A vector of strings containing raw commands
     */
    std::vector<std::string> convertStackOp(BCInstr instr) const;

    // The compiler component
    Compiler *comp;

    // Pointer to the list of bytecode functions
    std::vector<BCFunc *> *data;

};

#endif