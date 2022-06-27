
#ifndef __BCCONVERT_H__
#define __BCCONVERT_H__

#include "bcgen/bcgen.h"
#include "bcgen/instr.h"
#include "compiler/compiler.h"
#include "errorhandle/handle.h"
#include <string>
#include <vector>

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
     * Convert an inserted command to a raw command (trivial)
     * @param instr The instruction to convert, assumed to be of the command
     * type
     */
    std::string convertCmd(BCInstr instr) const;

    /**
     * Convert a custom execute call to a raw command
     * @param instr The instruction to convert, assumed to be of the exec-call
     * type
     */
    std::string convertExecCall(BCInstr instr) const;

    // The compiler component
    Compiler *comp;

    // Pointer to the list of bytecode functions
    std::vector<BCFunc *> *data;

};

#endif