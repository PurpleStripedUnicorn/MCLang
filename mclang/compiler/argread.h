
#ifndef __ARGREAD_H__
#define __ARGREAD_H__

#define HELPPADLEFT 25
#define HELPPADRIGHT 100

#include <string>
#include <vector>

class Compiler;

/**
 * Help text for a certain argument, containing the letter argument (' ' for no
 * letter argument), the word argument ("" for no word argument) and the
 * description of the argument
 */
struct ArgHelpText {
    char letter;
    std::string word;
    std::string description;
};

const ArgHelpText argList[] = {
    {'c', "custom-description", "Set the output datapack description"},
    {'d', "debug", "Use debugging tools. Debug info will be dumped in files in "
    "the current working directory."},
    {'D', "disable-output", "Disable result output to the file system. Will "
    "even disable output when \"-o\" is given."},
    {'n', "namespace", "Change the namespace of the output datapack, default "
    "is 'dp'"},
    {'h', "help", "Show the help page."},
    {'o', "output", "Set the output folder, default is 'out_datapack'. "
    "Warning: this folder will be overwritten by this program! Be very careful "
    "when selecting an output folder!"},
    {' ', "scoreboard", "Set the scoreboard objective used for variables. "
    "Default is \"mclang\"."},
    {'v', "version", "Set the Minecraft version to output as a datapack. Only "
    "supports normal versions after 1.17. Default is \"latest\""}
};

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
     * Get the executable filename
     * @return The (entered) executable filename as a string
     */
    std::string execName() const;

    /**
     * Show an error for invalid command line arguments
     */
    void invalidError() const;

    /**
     * Get the current command line argument as a string
     * @return `argv[curIndex]` converted to a string
     * @warning Shows an error if `curIndex` is larger than or equal to `argc`
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

    /**
     * Read in the "description" command line argument
     */
    void readDescription();

    /**
     * Read in the "debug" command line argument
     */
    void readDebug();

    /**
     * Read in the "disable output" command line argument
     */
    void readDisableOutput();

    /**
     * Read in the "namespace" command line argument
     */
    void readNamespace();

    /**
     * Read in the "help" command line argument
     */
    void readHelp();

    /**
     * Read in the "output" command line argument
     */
    void readOutput();

    /**
     * Read in the "scoreboard" command line argument
     */
    void readScoreboard();

    /**
     * Read in the "version" command line argument
     */
    void readVersion();

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