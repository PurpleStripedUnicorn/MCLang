
#ifndef __PARSER_DEBUG_H__
#define __PARSER_DEBUG_H__

#include <string>

class ParseNode;

const std::string parseNodeNames[] = {
    "ERR",
    "PROGRAM",
    "FUNC",
    "CMD",
    "CODEBLOCK",
    "EXEC",
    "IF",
    "CALL",
    "NAMESPACE"
};

/**
 * Returns a string which shows the parse tree in simplified form
 * @param root Root node of the tree
 * @param pad The left padding to add, 1 padding is 2 spaces
 * @return A string containing the tree
 */
std::string parserDebugTree(ParseNode *root, unsigned int pad = 0);

#endif