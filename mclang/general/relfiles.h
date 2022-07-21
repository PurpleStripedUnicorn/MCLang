
#ifndef __RELFILES_H__
#define __RELFILES_H__

#include <string>

/**
 * Get an absolute file location from a relative include
 * @param base The file from which another file is included
 * @param ref The referenced file, which needs to be included
 * @return Absolute file location of `ref` as a string
 * @note Does not check if the resulting file actually exists
 * @note Converts '/' to file path separator
 */
std::string getReferencePath(std::string base, std::string ref);


#endif