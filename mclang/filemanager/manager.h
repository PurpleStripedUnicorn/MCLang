
#ifndef __FILEMANAGER_H__
#define __FILEMANAGER_H__

#include "bcconvert/bcconvert.h"
#include <map>
#include <string>
#include <vector>

class Compiler;

/**
 * List of minecraft versions and their respective pack formats
 */
std::map<std::string, int> packVersions = {
    {"latest", 10},
    {"1.19", 10},
    {"1.18.2", 9},
    {"1.18.1", 8},
    {"1.18", 8},
    {"1.17.1", 7},
    {"1.17", 7},
};

class FileManager {

public:

    /**
     * Constructor
     * @param comp The main compiler component
     * @post Target folder is created
     */
    FileManager(Compiler *comp);

    /**
     * Destructor
     */
    ~FileManager();

    /**
     * Generate the datapack inside the created target folder
     * @param cmds List of functions containing the raw commands to be written
     * to the files
     */
    void genDatapack(const std::vector<CmdFunc> &cmds) const;

private:

    /**
     * Create a function file and write the given commands to it
     * @param func Function object containing the name of the function and the
     * command content
     */
    void genFunctionFile(const CmdFunc &func) const;

    /**
     * Get the pack format for the output datapack, based on the version given
     * by the compiler
     */
    int getPackFormat() const;

    /**
     * Create the "pack.mcmeta" file associated to this datapack
     */
    void genPackFile() const;

    /**
     * Delete the datapack that is currently in the output folder. If there is
     * no "pack.mcmeta" file present, nothing will be deleted. Will show an
     * error if there is not "pack.mcmeta" but the folder does exist
     */
    void deletePrevPack() const;

    /**
     * Create the folder structure to store all files in
     */
    void genFolderStructure() const;

    /**
     * Create a subfolder of the root folder
     * @param path The path relative to the root output folder
     * @post Given folder is created
     */
    void createSubFolder(std::string path) const;

    /**
     * Create a folder based on a path relative to current working directory
     * @param path The path to the folder to be created
     * @post Given folder is created
     */
    void createFolder(std::string path) const;

    // Target directory folder
    std::string root;

    // The namespace of the datapack
    std::string ns;

    // The compiler component
    Compiler *comp;

};

#endif