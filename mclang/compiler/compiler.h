
#ifndef __COMPILER_H__
#define __COMPILER_H__

class Lexer;
class Parser;
class BCManager;
class BCConverter;
class FileManager;
class Preprocessor;

#include <map>
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

    /**
     * Add an entry to the settings
     * @param name The name of the setting
     * @param value The value of the setting
     * @note Overrides any (possibly) already set value
     */
    void setSetting(std::string name, std::string value);

    /**
     * Find an entry in the settings
     * @param name The name of the setting
     * @return A string containing the value of the setting, if the settings
     * with the given name does not exist, return ""
     */
    std::string getSetting(std::string name) const;

    /**
     * Remove an entry from the settings
     * @param name The name of the setting
     * @note If a setting with the given name does not exist, nothing happens
     */
    void removeSetting(std::string name);

    // Input filename
    std::string filename;

    // Namespace to use in the output
    std::string ns;

    // Datpack output folder
    std::string outputFolder;

    // Debug mode switch
    bool debugMode;

    // File output enable/disable
    bool fileOutput;

    // Scoreboard objective name
    std::string scoreboardName;

    // Minecraft version
    std::string mcVersion;

    // Datapack description
    std::string description;

    // NOTE: These components may still may null values when the compilation is
    // not done
    // Preprocessor compoenent
    Preprocessor *prep;
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

    // Settings are used by components for specific behaviour
    std::map<std::string, std::string> settings;

};

#endif