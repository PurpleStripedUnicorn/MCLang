
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
#include "preprocess/debug.h"
#include "preprocess/preprocess.h"
#include <fstream>
#include <map>
#include <string>

Compiler::Compiler() : filename(""), ns("dp"), outputFolder("out_datapack"),
debugMode(false), fileOutput(true), scoreboardName("mclang"),
mcVersion("latest"), description(""), prep(nullptr), lexer(nullptr),
parser(nullptr), bcMan(nullptr), bcConvert(nullptr) {
    
}

Compiler::~Compiler() {
    delete prep;
    delete lexer;
    delete parser;
    delete bcMan;
    delete bcConvert;
}

void Compiler::compile() {
    runPreprocessor();
    runLexer();
    runParser();
    runBCGenerator();
    runBCConverter();
    runFileOutput();
}

void Compiler::runPreprocessor() {
    prep = new Preprocessor();
    prep->processFile(filename);
    if (debugMode) {
        std::ofstream out("mcl_prep.debug");
        out << prepDebugTable(&prep->getOutput());
        out.close();
    }
}

void Compiler::runLexer() {
    lexer = new Lexer(this);
    lexer->readIn();
    if (debugMode) {
        std::ofstream out("mcl_lexer.debug");
        out << lexerDebugTable(lexer->tokens());
        out.close();
    }
}

void Compiler::runParser() {
    parser = new Parser(this);
    parser->genTree();
    if (debugMode) {
        std::ofstream out("mcl_parser.debug");
        out << parserDebugTree(parser->getTree());
        out.close();
    }
}

void Compiler::runBCGenerator() {
    bcMan = new BCManager(this);
    bcMan->generate();
    if (debugMode) {
        std::ofstream out("mcl_bcgen.debug");
        out << bcgenInstrList(bcMan->getBytecode());
        out.close();
    }
}

void Compiler::runBCConverter() {
    bcConvert = new BCConverter(this);
    cmds = bcConvert->getRawCommands();
    if (debugMode) {
        std::ofstream out("mcl_cmds.debug");
        out << bcconvertCmdList(cmds);
        out.close();
    }
}

void Compiler::runFileOutput() {
    if (fileOutput) {
        FileManager fm(this);
        fm.genDatapack(cmds);
    }
}