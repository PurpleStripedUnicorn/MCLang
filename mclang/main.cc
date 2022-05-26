
#include "bcgen/bcgen.h"
#include "bcgen/debug.h"
#include "lexer/lexer.h"
#include "lexer/debug.h"
#include "lexer/token.h"
#include "parsenodes/parsenode.h"
#include "parser/debug.h"
#include "parser/parser.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

bool debugMode = false;
std::string fname = "";

void readInLetterArg(char letter) {
    if (letter == 'd') {
        // Debugging mode
        debugMode = true;
    } else if (letter == 'h') {
        // Help page
        std::cout << std::endl << " -- MCLang compiler -- " << std::endl
        << std::endl << "-d     " << "Use debugging tools. Debug info will " <<
        "be dumped in files in the current working directory."
        << std::endl << "-h     " << "Show this page."
        << std::endl << std::endl;
        exit(0);
    } else {
        std::cerr << "Invalid option '-" << letter << "' given" << std::endl;
        exit(1);
    }
}

void readInArgs(int argc, char *argv[]) {
    for (int i = 1; i < argc; i++) {
        std::string cur = argv[i];
        // detect arguments like '-d' (for debugging)
        if (cur[0] == '-')
            readInLetterArg(cur[1]);
        else
            fname = cur;
    }
}

int main(int argc, char *argv[]) {
    debugMode = false, fname = "";
    // Read in the arguments provided via the command line
    readInArgs(argc, argv);
    if (fname == "") {
        std::cerr << "No filename given." << std::endl;
        return 1;
    }
    std::cout << fname << std::endl;
    // Read the contents of the given file
    std::ifstream inpFile(fname);
    std::string inp;
    if (inpFile.is_open()) {
        std::ostringstream ss;
        ss << inpFile.rdbuf();
        inp = ss.str();
    } else {
        std::cerr << "Unable to open file '" << fname << "'." << std::endl;
        return 1;
    }
    inpFile.close();
    // Lexer
    Lexer lex(inp);
    std::vector<Token> lexOut = lex.readIn();
    if (debugMode) {
        std::ofstream out("mcl_lexer.debug");
        out << lexerDebugTable(lexOut);
        out.close();
    }
    // Parser
    Parser pars(lexOut);
    ParseNode *parsOut = pars.genTree();
    if (debugMode) {
        std::ofstream out("mcl_parser.debug");
        out << parserDebugTree(parsOut);
        out.close();
    }
    // Bytecode generator
    BCManager bcman;
    parsOut->bytecode(bcman);
    if (debugMode) {
        std::ofstream out("mcl_bcgen.debug");
        out << bcgenInstrList(bcman.getBytecode());
        out.close();
    }
    if (!debugMode)
        std::cout << "Currently nothing happens without debug mode."
        << std::endl;
    return 0;
}