
#include "compiler/compiler.h"
#include <fstream>
#include <functional>
#include <iostream>
#include <sstream>
#include <string>

#define HELPPADLEFT 24
#define HELPPADRIGHT 100

bool debugMode;
bool fileOutput;
std::string fname;
std::string ns;
std::string outputName;
std::string scoreboardName;
std::string mcVersion;

/**************************
 * Command line arguments
 **************************/

/**
 * Object to store options for command line arguments
 */
struct CmdLineArg {
    char letter;
    std::string fullName;
    unsigned int argCount;
    const std::function<void(std::string)> handler;
    std::string description;
};

// Pre-define functions that have cross-dependence with argument list
void argHelpList(std::string args);

/**
 * Enable debug mode
 * @param args Arguments from the command line, not used
 */
void argDebugMode(std::string args) {
    std::string unused = args;
    debugMode = true;
}

/**
 * Set the output folder name
 * @param args The folder name to select, no spaces
 */
void argOutputFolder(std::string args) {
    outputName = args;
}

/**
 * Set the namespace
 * @param args The namespace as a string, no spaces
 * @note Assumes that the namespace is indeed a valid namespace name
 */
void argNamespace(std::string args) {
    ns = args;
}

/**
 * Disable file output
 * @param args Arguments from the command line, not used
 */
void argFileOutput(std::string args) {
    std::string unused = args;
    fileOutput = false;
}

/**
 * Set the scoreboard objective name being used
 * @param args The scoreboard objective name, no spaces
 * @note Assumes that the given name is indeed a valid scoreboard objective name
 */
void argScoreboardName(std::string args) {
    scoreboardName = args;
}

/**
 * Set the Minecraft version being used
 * @param args The version number
 */
void argVersion(std::string args) {
    mcVersion = args;
}

// List of all command line arguments, for easier use later in the program
// Use ' ' for no letter or "" for no full name
const CmdLineArg argList[] = {
    {'d', "debug", 0, argDebugMode, "Use debugging tools. Debug info will be "
    "dumped in files in the current working directory."},
    {'D', "disable-output", 0, argFileOutput, "Disable result output to the "
    "file system. Will even disable output when \"-o\" is given."},
    {'n', "namespace", 1, argNamespace, "Change the namespace of the output "
    "datapack, default is 'dp'"},
    {'h', "help", 0, argHelpList, "Show the help page."},
    {'o', "output", 1, argOutputFolder, "Set the output folder, default is "
    "'out_datapack'. Warning: this folder will be overwritten by this program! "
    "Be very careful when selecting an output folder!"},
    {' ', "scoreboard", 1, argScoreboardName, "Set the scoreboard objective "
    "used for variables. Default is \"mclang\"."},
    {'v', "version", 1, argVersion, "Set the Minecraft version to output as a "
    "datapack. Only supports normal versions after 1.17. Default is \"latest\""
    },
};

/**
 * Show the help page
 * @param args Arguments from the command line, not used
 */
void argHelpList(std::string args) {
    std::string unused = args;
    std::cout << std::endl << " -- MCLang compiler -- " << std::endl
    << std::endl;
    for (unsigned int i = 0; i < sizeof(argList) / sizeof(argList[0]); i++) {
        std::string cur = "";
        if (argList[i].letter != ' ')
            cur.push_back('-'), cur.push_back(argList[i].letter);
        if (argList[i].letter != ' ' && argList[i].fullName != "")
            cur.append(", ");
        if (argList[i].fullName != "")
            cur.append("--" + argList[i].fullName);
        while (cur.size() < HELPPADLEFT)
            cur.push_back(' ');
        cur.append(argList[i].description.substr(0, HELPPADRIGHT));
        for (unsigned int j = HELPPADRIGHT; j < argList[i].description.size();
        j += HELPPADRIGHT) {
            cur.push_back('\n');
            for (unsigned int k = 0; k < HELPPADLEFT; k++)
                cur.push_back(' ');
            cur.append(argList[i].description.substr(j, HELPPADRIGHT));
        }
        std::cout << cur << std::endl << std::endl;
    }
    exit(0);
}

void readInArgs(int argc, char *argv[]) {
    unsigned int leftToRead = 0;
    std::string curParams = "";
    unsigned int curArgType = 0;
    unsigned int i = 1;
    while (i < (unsigned int)argc) {
        std::string cur = argv[i];
        // Full-word command line argument
        if (cur.size() >= 2 && cur[0] == '-' && cur[1] == '-') {
            if (cur.size() == 2) {
                std::cerr << "Invalid command line argument." << std::endl;
                exit(1);
            }
            if (leftToRead > 0) {
                std::cerr << "Too few parameters to command line arguments "
                "given." << std::endl;
                exit(1);
            }
            std::string word = cur.substr(2);
            bool found = false;
            for (unsigned int j = 0; j < sizeof(argList) / sizeof(argList[0]);
            j++) {
                if (argList[j].fullName == word) {
                    found = true;
                    if (argList[j].argCount == 0) {
                        argList[j].handler("");
                    } else {
                        leftToRead = argList[j].argCount;
                        curArgType = j;
                    }
                }
            }
            if (!found) {
                std::cerr << "Unknown command line argument." << std::endl;
                exit(1);
            }
            i++;
        } else if (cur.size() >= 1 && cur[0] == '-') {
            // Single-letter command line argument
            if (leftToRead > 0) {
                std::cerr << "Too few parameters to command line arguments "
                "given." << std::endl;
                exit(1);
            }
            if (cur.size() != 2) {
                std::cerr << "Invalid command line arguments." << std::endl;
                exit(1);
            }
            char letter = cur[1];
            bool found = false;
            for (unsigned int j = 0; j < sizeof(argList) / sizeof(argList[0]);
            j++) {
                if (argList[j].letter == letter) {
                    found = true;
                    if (argList[j].argCount == 0) {
                        argList[j].handler("");
                    } else {
                        leftToRead = argList[j].argCount;
                        curArgType = j;
                    }
                }
            }
            if (!found) {
                std::cerr << "Unknown command line argument." << std::endl;
                exit(1);
            }
            i++;
        } else if (leftToRead > 1) {
            curParams.append(cur + " ");
            leftToRead--;
            i++;
        } else if (leftToRead == 1) {
            curParams.append(cur);
            argList[curArgType].handler(curParams);
            leftToRead = 0;
            i++;
        } else {
            if (fname == "") {
                fname = cur;
            } else {
                std::cerr << "Found unexpected argument." << std::endl;
                exit(1);
            }
            i++;
        }
    }
    if (leftToRead > 0) {
        std::cerr << "Too few parameters to command line arguments given."
        << std::endl;
        exit(1);
    }
}

int main(int argc, char *argv[]) {
    fileOutput = true, debugMode = false, fname = "", ns = "dp",
    outputName = "out_datapack", scoreboardName = "mclang",
    mcVersion = "latest";
    // Read in the arguments provided via the command line
    readInArgs(argc, argv);
    if (fname == "") {
        std::cerr << "No filename given." << std::endl;
        return 1;
    }
    // Compile the code inside the file
    Compiler comp;
    comp.filename = fname;
    comp.ns = ns;
    comp.outputFolder = outputName;
    comp.debugMode = debugMode;
    comp.fileOutput = fileOutput;
    comp.scoreboardName = scoreboardName;
    comp.mcVersion = mcVersion;
    comp.compile();
    return 0;
}