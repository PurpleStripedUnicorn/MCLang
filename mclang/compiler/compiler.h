
#ifndef __COMPILER_H__
#define __COMPILER_H__

#include "bcgen/bcgen.h"
#include "bcgen/debug.h"
#include "bcconvert/bcconvert.h"
#include "bcconvert/debug.h"
#include "filemanager/manager.h"
#include "lexer/debug.h"
#include "lexer/lexer.h"
#include "parser/debug.h"
#include "parser/parser.h"
#include <string>

/**
 * Main compiler component, that handles all the subcomponents
 * Keeps track of certain things that need to be communicated between components
 */
class Compiler {

public:

    /**
     * Constructor
     */
    Compiler();

    /**
     * Destructor
     */
    ~Compiler();

    /**
     * Compile the stored input
     * @pre `input` variable is set
     * @post All components are now valid pointers
     */
    void compile();

    // Text input folder the compiler
    std::string input;

    // Namespace to use in the output
    std::string ns;

    // Output folder
    std::string outputFolder;

    // Debug mode switch
    bool debugMode;

    // NOTE: These component may still may null values when the compilation is
    // not done
    // Lexer component
    Lexer *lexer;
    // Parser component
    Parser *parser;
    // Bytecode generator manager component
    BCManager *bcMan;
    // Bytecode converter component
    BCConverter *bcConvert;
    // File manager component
    FileManager *fileMan;

private:

};

#endif