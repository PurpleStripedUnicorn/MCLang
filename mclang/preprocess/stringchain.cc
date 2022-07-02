
#include "preprocess/stringchain.h"
#include <string>

#define MAX_NODE_SIZE 64

StringChain::StringChain() : strSize(0), first(NULL), last(NULL) {

}

StringChain::StringChain(const std::string &val) : strSize(0), first(NULL),
last(NULL) {
    append(val);
}

StringChain::~StringChain() {
    Node *cur = first;
    Node *prev;
    while (cur != NULL) {
        prev = cur;
        cur = cur->next;
        delete prev;
    }
}

void StringChain::append(char c) {
    strSize++;
    if (last->content.size() >= MAX_NODE_SIZE)
        createNode();
    last->content.push_back(c);
}

void StringChain::concatStrings(std::string &out) const {
    out = "";
    out.reserve(strSize);
    Node *cur = first;
    while (cur != NULL) {
        out.append(cur->content);
        cur = cur->next;
    }
}

void StringChain::append(const std::string &s) {
    for (unsigned int i = 0; i < s.size(); i++)
        append(s[i]);
}

void StringChain::createNode() {
    Node *newLast = new Node;
    newLast->next = NULL, newLast->content = "";
    last->next = newLast;
    last = newLast;
}