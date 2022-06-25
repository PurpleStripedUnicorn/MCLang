
#include "compiler/compiler.h"

#include "bcgen/bcgen.h"
#include "bcgen/debug.h"
#include "bcconvert/bcconvert.h"
#include "bcconvert/debug.h"
#include "filemanager/manager.h"
#include "lexer/debug.h"
#include "lexer/lexer.h"
#include "parser/debug.h"
#include "parser/parser.h"

Compiler::Compiler() : input(""), ns("dp"), outputFolder("out_datapack"),
debugMode(false), lexer(NULL), parser(NULL), bcMan(NULL), bcConvert(NULL),
fileMan(NULL) {
    
}

Compiler::~Compiler() {
    delete lexer;
    delete parser;
    delete bcMan;
    delete bcConvert;
    delete fileMan;
}

void Compiler::compile() {
    // Lexer
    lexer = new Lexer(this);
    lexer->readIn();
    if (debugMode) {
        std::ofstream out("mcl_lexer.debug");
        out << lexerDebugTable(lexer->tokens());
        out.close();
    }
    // Parser
    parser = new Parser(this);
    parser->genTree();
    if (debugMode) {
        std::ofstream out("mcl_parser.debug");
        out << parserDebugTree(parser->getTree());
        out.close();
    }
    // Bytecode generator
    bcMan = new BCManager(this);
    bcMan->generate();
    std::vector<BCFunc> bytecode = bcMan->getBytecode();
    if (debugMode) {
        std::ofstream out("mcl_bcgen.debug");
        out << bcgenInstrList(bcMan->getBytecode());
        out.close();
    }
    // Bytecode converter
    bcConvert = new BCConverter(&bytecode);
    std::vector<CmdFunc> cmds = bcConvert->getRawCommands();
    if (debugMode) {
        std::ofstream out("mcl_cmds.debug");
        out << bcconvertCmdList(cmds);
        out.close();
    }
    // Create output files and folders
    FileManager fm(outputFolder, ns);
    fm.genDatapack(cmds);
}