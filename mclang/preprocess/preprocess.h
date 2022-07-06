
#ifndef __PREPROCESS_H__
#define __PREPROCESS_H__

#include "preprocess/codetext.h"
#include <fstream>
#include <string>
#include <vector>

#define MAX_INCLUDE_DEPTH 128

/**
 * Wrapper around input file stream objects, used to keep track of information
 * like line and column numbers
 */
class InpStream {

public:

    /**
     * Constructor
     * @param filename The file to be read
     */
    InpStream(std::string filename);

    /**
     * Destructor
     */
    ~InpStream();

    /**
     * Get a character from the stream
     * @return The character read
     * @post `stream`, `line` and `col` are changed accordingly
     */
    char get();

    /**
     * Peek to see the next character that will be read with get
     * @return The character to be read next
     */
    char peek();

    /**
     * Check if the end of the file has been reached or the stream has exited
     * @return Boolean indicating that reading can continue and there are still
     * characters to be read
     */
    bool good();

    // Raw input filestream
    std::ifstream stream;

private:

    // Filename, for convenience
    std::string filename;

    // Current line and column of the stream
    unsigned int line, col;

};

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

    /**
     * Process a given file
     * @param filename The file to process
     * @post The output is appended in `out`, if needed
     * @note Processes included files recursively
     */
    void processFile(std::string filename);

private:

    /**
     * Command type, or just raw text
     */
    enum PrepLineType {
        LINE_TXT, LINE_INCL
    };

    /**
     * Used internally when reading lines
     * Content is just the entire line when dealing with raw text, or anything
     * that comes after the statement when dealing with a preprocessor statement
     */
    struct PrepLine {
        PrepLineType type = LINE_TXT;
        std::string content = "";
    };

    /**
     * Read in a preprocessor code block
     * @param fs The current input filestream
     * @post The output is appended in `out`, if needed
     */
    void readCodeBlock(InpStream &fs);

    /**
     * Read in a single line. Depending on wether it is raw text or a
     * preprocessor statement, an action is taken
     * @param fs The current input filestream
     * @note Skips over the endline at the end
     */
    PrepLine readLine(InpStream &fs);

    // The line that has been read up to this point
    std::string curRead;

    // Current include depth: number of includes that are above the current file
    // being read
    unsigned int includeDepth;

    // Output switch
    bool doOutput;

    // Output
    CodeText out;

};

#endif