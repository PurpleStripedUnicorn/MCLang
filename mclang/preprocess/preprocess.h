
#ifndef __PREPROCESS_H__
#define __PREPROCESS_H__

#include <string>
#include <vector>

#define MAX_INCLUDE_DEPTH 128

class Preprocessor {

public:

    /**
     * Constructor
     */
    Preprocessor();

    /**
     * Destructor
     */
    ~Preprocessor();

private:

    // The line that has been read up to this point
    std::string curRead;

    // Current include depth: number of includes that are above the current file
    // being read
    unsigned int includeDepth;
    

};

#endif