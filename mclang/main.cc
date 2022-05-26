
#include "lexer/lexer.h"
#include "lexer/debug.h"
#include "lexer/token.h"
#include "parsenodes/parsenode.h"
#include "parser/debug.h"
#include "parser/parser.h"
#include <iostream>
#include <vector>

int main () {
    std::string inp = "void main () {\n  /say hello\n}";
    // Lexer
    Lexer lex(inp);
    std::vector<Token> lexOut = lex.readIn();
    std::cout << lexerDebugTable(lexOut) << std::endl << std::endl;
    // Parser
    Parser pars(lexOut);
    ParseNode *parsOut = pars.genTree();
    std::cout << parserDebugTree(parsOut) << std::endl;
    return 0;
}