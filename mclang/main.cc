
#include "lexer/lexer.h"
#include "lexer/debug.h"
#include "lexer/token.h"
#include <iostream>
#include <vector>

int main () {
    std::string inp = "int main () {\n  /say hello\n}";
    Lexer lex(inp);
    std::vector<Token> out = lex.readIn();
    std::cout << lexerDebugTable(out) << std::endl;
    return 0;
}