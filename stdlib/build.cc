/**
 * This file is used to generate C++ code that stores the contents of the ".mcl"
 * files in this folder in a string, this C++ should then be compiled by the
 * makefile to use in the final built program
 * NOTE: This script is only made for Linux, not Windows!
 */

#include <filesystem>
#include <iostream>
#include <fstream>
#include <set>
#include <string>

#define RAW_DELIM "!--!DELIM!--!"

/**
 * Get the filenames of the ".mcl" and ".mclang" files in the "stdlib" folder
 * (recursively searches through directories)
 */
std::set<std::string> getFileNames(std::string path = "stdlib") {
    std::set<std::string> out;
    for (auto const &entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_regular_file()) {
            std::string ext = std::filesystem::path(entry).extension().string();
            if (ext == ".mcl" || ext == ".mclang")
                out.insert(entry.path().string());
        }
        if (entry.is_directory()) {
            std::set<std::string> sub = getFileNames(entry.path().string());
            out.insert(sub.begin(), sub.end());
        }
    }
    return out;
}

/**
 * Write the contents of a file as a string to the given output filestream
 * NOTE: Make sure that input filenames are valid variable names in C++
 */
void addFileToOutput(std::ofstream &fs, std::string filename) {
    fs << "{\"" << std::filesystem::path(filename).stem().string() << "\", R\""
    << RAW_DELIM << "(";
    std::ifstream inp(filename);
    std::string line;
    if (inp.is_open()) {
        while (std::getline(inp, line))
            fs << line << "\n";
    } else {
        std::cerr << "Could not build MCLang standard library." << std::endl;
        exit(1);
    }
    inp.close();
    fs << ")" << RAW_DELIM << "\"}," << std::endl;
}

int main() {
    std::ofstream fs("mclang/stdlib/stdlib.h");
    fs << "#ifndef __STDLIB_RAW_FILE_H__" << std::endl;
    fs << "#define __STDLIB_RAW_FILE_H__" << std::endl;
    fs << "#include <string>" << std::endl << "#include <map>" << std::endl
    << "const std::map<std::string, std::string> stdlibContent = {"
    << std::endl;
    for (std::string filename : getFileNames())
        addFileToOutput(fs, filename);
    fs << "};" << std::endl;
    fs << "#endif" << std::endl;
    fs.close();
    return 0;
}