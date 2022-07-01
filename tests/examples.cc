/**
 * @warning This script is only to be executed by someone with exprience with
 * programming, execution of this script may be dangerous on some systems!
 */
#include <iostream>
#include <filesystem>
#include <set>
#include <string>
#include <vector>

#ifdef _WIN32
    #define DIRSEP "\\"
#else
    #define DIRSEP "/"
#endif

std::set<std::string> getFileNames(std::string path = "examples") {
    std::set<std::string> out;
    for (auto const &entry : std::filesystem::directory_iterator(path)) {
        if (entry.is_regular_file())
            out.insert(entry.path().string());
        if (entry.is_directory()) {
            std::set<std::string> sub = getFileNames(entry.path().string());
            out.insert(sub.begin(), sub.end());
        }
    }
    return out;
}

void printFileResult(std::string name, bool hasFailed) {
    std::cout << (hasFailed ? "\033[0;31mFAILURE:" : "\033[0;32mSUCCESS:")
    << "    \033[0m" << name << std::endl;
}

int main () {
    unsigned int success = 0, failed = 0;
    std::vector<std::string> failedList;
    std::cout << std::endl;
    for (const std::string &path : getFileNames()) {
        bool isInvalid = false;
        if (path.find("invalid") != std::string::npos)
            isInvalid = true;
#ifdef _WIN32
        if (system(("build\\main.exe -D " + path).c_str()) == 0) {
#else
        if (system(("build/main -D " + path).c_str()) == 0) {
#endif
            printFileResult(path, isInvalid);
            if (isInvalid)
                failed++, failedList.push_back(path);
            else
                success++;
        } else {
            printFileResult(path, !isInvalid);
            if (isInvalid)
                success++;
            else
                failed++, failedList.push_back(path);
        }
    }
    std::cout << std::endl << (failed > 0 ? "\033[0;31m" : "\033[0;32m")
    << "---------------------------------------------" << std::endl << std::endl
    << failed << " failures, " << success << " successes" << std::endl;
    if (failed > 0) {
        std::cout << std::endl << "List of failures:" << std::endl;
        for (unsigned int i = 0; i < failedList.size(); i++)
            std::cout << "    " << failedList[i] << std::endl;
    }
    std::cout << std::endl << "---------------------------------------------"
    << std::endl << std::endl << "\033[0m";
    return failed > 0 ? 1 : 0;
}