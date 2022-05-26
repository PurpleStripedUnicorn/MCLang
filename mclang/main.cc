
#include "bcgen/bcgen.h"
#include "bcgen/debug.h"
#include "lexer/lexer.h"
#include "lexer/debug.h"
#include "lexer/token.h"
#include "parsenodes/parsenode.h"
#include "parser/debug.h"
#include "parser/parser.h"
#include <iostream>
#include <vector>

int main () {
    std::string inp = "void main () {\n  /say hello\n/say hi!\n}";
    // Lexer
    Lexer lex(inp);
    std::vector<Token> lexOut = lex.readIn();
    std::cout << lexerDebugTable(lexOut) << std::endl << std::endl;
    // Parser
    Parser pars(lexOut);
    ParseNode *parsOut = pars.genTree();
    std::cout << parserDebugTree(parsOut) << std::endl << std::endl;
    // Bytecode generator
    BCManager bcman;
    parsOut->bytecode(bcman);
    std::cout << bcgenInstrList(bcman.getBytecode()) << std::endl;
    return 0;
}