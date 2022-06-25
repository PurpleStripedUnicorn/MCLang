
#ifndef __BCGEN_H__
#define __BCGEN_H__

#include "bcgen/instr.h"
#include "compiler/compiler.h"
#include <string>
#include <vector>

/**
 * Instructions are stored in functions which can then call each other
 */
struct BCFunc {
    BCFunc(std::string name);
    std::string name;
    std::vector<BCInstr> instrList;
};

/**
 * Manager object to keep track of stuff while generating bytecode, which is
 * done by the parse node objects. This object also stores the generated
 * bytecode
 */
class BCManager {

public:

    /**
     * Constructor
     * @param comp The compiler component
     */
    BCManager(Compiler *comp);

    /**
     * Destructor
     */
    ~BCManager();

    /**
     * Generate bytecode
     * @note This function assumes that the parser generated a parse tree, which
     * is used in this function
     * @post Generated bytecode is stored in `funcList`
     */
    void generate();

    /**
     * Get all of the generated bytecode
     * @return A vector containing the functions which contain bytecode
     * instructions
     */
    std::vector<BCFunc> getBytecode() const;

    /**
     * Get a pointer to the function that is currently being written to
     * @return A pointer to the top function on the `funcStack`
     */
    BCFunc *curFunc() const;

    /**
     * Write an instruction to the current function
     * @param instr The instruction to be appended
     * @post An instruction is appended to the top function of the `funcStack`
     */
    void write(BCInstr instr);

    /**
     * Create a new function and add it to the function stack
     * @param name The name of the function (optional)
     * @note If no name is given, a unique number will be assigned
     */
    void addFunc(std::string name = "");

    /**
     * Remove the top function from the function stack
     * @post The top element of the `funcStack` is removed, if there is any
     * @note The function will still be present in the `funcList`
     */
    void popFunc();

    /**
     * Get a pointer to the top function on the function stack
     * @return Top element of `funcStack`
     */
    BCFunc *topFunc() const;

private:

    // The compiler component
    Compiler *comp;

    // List of functions that are generated
    std::vector<BCFunc *> funcList;

    // Function stack, which helps with adding instructions to the right
    // functions
    std::vector<BCFunc *> funcStack;

    // Counter to keep track of unique IDs/names assigned to functions
    unsigned int uniqueFuncId;

};

#endif