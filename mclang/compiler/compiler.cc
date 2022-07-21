
#include "bcgen/bcgen.h"
#include "bcgen/debug.h"
#include "bcconvert/bcconvert.h"
#include "bcconvert/debug.h"
#include "compiler/compiler.h"
#include "filemanager/manager.h"
#include "lexer/debug.h"
#include "lexer/lexer.h"
#include "parser/debug.h"
#include "parser/parser.h"
#include "preprocess/preprocess.h"
#include <fstream>
#include <string>

Compiler::Compiler() : filename(""), ns("dp"), outputFolder("out_datapack"),
debugMode(false), fileOutput(true), scoreboardName("mclang"),
mcVersion("latest"), prep(NULL), lexer(NULL), parser(NULL), bcMan(NULL),
bcConvert(NULL), fileMan(NULL) {
    
}

Compiler::~Compiler() {
    delete prep;
    delete lexer;
    delete parser;
    delete bcMan;
    delete bcConvert;
    delete fileMan;
}

void Compiler::compile() {
    // Preprocessor
    prep = new Preprocessor();
    prep->processFile(filename);
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
    if (debugMode) {
        std::ofstream out("mcl_bcgen.debug");
        out << bcgenInstrList(bcMan->getBytecode());
        out.close();
    }
    // Bytecode converter
    bcConvert = new BCConverter(this);
    std::vector<CmdFunc> cmds = bcConvert->getRawCommands();
    if (debugMode) {
        std::ofstream out("mcl_cmds.debug");
        out << bcconvertCmdList(cmds);
        out.close();
    }
    // Create output files and folders
    if (fileOutput) {
        FileManager fm(this);
        fm.genDatapack(cmds);
    }
}