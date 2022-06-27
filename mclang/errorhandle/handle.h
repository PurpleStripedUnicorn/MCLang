
#ifndef __ERRORHANDLE_H__
#define __ERRORHANDLE_H__

#include <string>

#define LEFT_PAD 3
#define ERRTXT_PAD 10
#define LINETXT_PAD 15
#define TXT_PAD 60

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
    struct {
        unsigned int line, col;
    } loc;

};

#endif