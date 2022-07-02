
#ifndef __STRINGCHAIN_H__
#define __STRINGCHAIN_H__

#include <string>

/**
 * A StringChain object creates a chain of strings using pointers. Each of these
 * strings can at most be of a certain maximum size. If they exceed this size
 * they will be split up. Content can be added at the end of the string and the
 * strings can be concatenated efficiently by reserving enough memory beforehand
 */
class StringChain {

public:

    /**
     * Constructor
     * @note Initializes as an entry string
     */
    StringChain();

    /**
     * Constructor
     * @param val Initialize with this value, is split up automatically
     */
    StringChain(const std::string &val);

    /**
     * Destructor
     */
    ~StringChain();

    /**
     * Add a character at the end
     * @param c The character to append
     */
    void append(char c);

    /**
     * Add a string at the end
     * @param s The string to append, is automatically split up if neccessary
     */
    void append(const std::string &s);

    /**
     * Join all strings stored in this chain together
     * @param out A reference to the string object where the result should 
     * put
     */
    void concatStrings(std::string &out) const;

private:

    /**
     * A node in the string chain, points to its successor and contains a string
     * of certain max size
     */
    struct Node {
        Node *next;
        std::string content;
    };

    /**
     * Create a new (empty) node at the end of the chain
     */
    void createNode();

    // The current amount of characters stored in this string chain
    unsigned int strSize;

    // First node in the chain
    Node *first;

    // Last node in the chain
    Node *last;

};

#endif