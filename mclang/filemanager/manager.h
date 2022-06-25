
#ifndef __FILEMANAGER_H__
#define __FILEMANAGER_H__

struct CmdFunc;

#include <dirent.h>
#include <filesystem>
#include <fstream>
#include <string>
#include <sys/stat.h>
#include <vector>

#ifdef _WIN32
    #define MKDIR_FAIL_CODE 0
    #define DIRSEP std::string("\\")
#else
    #define MKDIR_FAIL_CODE -1
    #define DIRSEP std::string("/")
#endif

class FileManager {

public:

    /**
     * Constructor
     * @param root The target directory root folder. Does not have to exist yet,
     * but parent folder does have to exist
     * @param ns The namespace of the datapack
     * @post Target folder is created
     */
    FileManager(std::string root, std::string ns);

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

};

#endif