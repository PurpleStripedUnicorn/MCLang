
#ifndef __ERRORHANDLE_H__
#define __ERRORHANDLE_H__

#include "general/loc.h"
#include <string>

/**
 * Error object, shows the error/warning/note on creation
 */
class MCLError {

public:

    /**
     * Constructor
     * @param level The level of seriousness, a level of 0 means the program
     * will not be halted and indicates a "warning", level 1 is an "error"
     * @param txt The error text
     * @param line The line on which the error occured
     * @param col The column on which the error occured
     */
    MCLError(unsigned int level, std::string txt, unsigned int line = 0,
    unsigned int col = 0);

    /**
     * Constructor
     * @param level The level of seriousness, a level of 0 means the program
     * will not be halted and indicates a "warning", level 1 is an "error"
     * @param txt The error text
     * @param loc The location of error, which also includes file information
     */
    MCLError(unsigned int level, std::string txt, Loc loc);

private:

    /**
     * Print out the error on the screen
     */
    void print() const;

    // Level of seriousness
    unsigned int level;

    // Error text
    std::string txt;

    // Location of the error
    Loc loc;

};

#endif