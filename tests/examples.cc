/**
 * @warning This script is only to be executed by someone with exprience with
 * programming, execution of this script may be dangerous on some systems!
 */
#include <iostream>
#include <filesystem>
#include <set>
#include <string>
#include <vector>

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

#ifdef _WIN32
std::string colored(std::string txt, std::string color) {
    (void)color;
    return txt;
}
#else
std::string colored(std::string txt, std::string color) {
    if (color == "red")
        return "\033[0;31m" + txt + "\033[0m";
    if (color == "green")
        return "\033[0;32m" + txt + "\033[0m";
    return txt;
}
#endif

void printFileResult(std::string name, bool hasFailed) {
    std::cout << (hasFailed ? colored("FAILURE:", "red") : colored("SUCCESS:",
    "green")) << "    " << name << std::endl;
}

int main () {
    unsigned int success = 0, failed = 0;
    std::vector<std::string> failedList;
    std::cout << std::endl;
    for (const std::string &path : getFileNames()) {
        bool isInvalid = path.find("invalid") != std::string::npos;
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
    std::string color = failed > 0 ? "red" : "green";
    std::cout << std::endl
    << colored("---------------------------------------------", color)
    << std::endl << std::endl
    << colored(std::to_string(failed) + " failures, " + std::to_string(success)
    + " successes", color) << std::endl;
    if (failed > 0) {
        std::cout << std::endl << colored("List of failures:", color)
        << std::endl;
        for (unsigned int i = 0; i < failedList.size(); i++)
            std::cout << "    " << colored(failedList[i], color) << std::endl;
    }
    std::cout << std::endl
    << colored("---------------------------------------------", color)
    << std::endl << std::endl;
    return failed > 0 ? 1 : 0;
}