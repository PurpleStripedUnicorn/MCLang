
#ifndef __ARGREAD_H__
#define __ARGREAD_H__

#include <string>

class Compiler;

/**
 * Object to read in command line arguments given by "-" and "--" arguments and
 * possible values
 */
class ArgReader {

public:

    /**
     * Constructor
     * @param comp The main compiler component
     */
    ArgReader(Compiler *comp);

    /**
     * Destructor
     */
    ~ArgReader();

    /**
     * Read in an argument list
     * @param argc The number of arguments
     * @param argv The arguments
     * @post All settings are applied in 
     */
    void read(int argc, char *argv[]);

private:

    /**
     * Get the current command line argument as a string
     * @return `argv[curIndex]` converted to a string, returns an empty string
     * if `curIndex` is larger than or equal to `argc`
     */
    std::string cur() const;

    /**
     * Move to the next command line argument
     * @post `curIndex` is incremented
     */
    void next();

    /**
     * Read in the current argument, can also read in the main filename given
     */
    void readCur();

    /**
     * Read in the main compiler argument: the filename
     */
    void readMainArg();

    /**
     * Read in a single letter argument
     */
    void readLetterArg();

    /**
     * Read in a word size argument, so starting with "--"
     */
    void readWordArg();

    // The main compiler component
    Compiler *comp;

    // Current index of argument being read
    int curIndex;

    // Argument count
    int argc;

    // Pointer to arguments
    char **argv;

    // Indicates if a filename has been given so far
    bool hasGivenFilename;

};

#endif