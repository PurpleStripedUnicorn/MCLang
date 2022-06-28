
#include "compiler/compiler.h"
#include "errorhandle/handle.h"
#include "lexer/debug.h"
#include "lexer/lexer.h"
#include "lexer/token.h"
#include "parsenodes/cmd.h"
#include "parsenodes/codeblock.h"
#include "parsenodes/exec.h"
#include "parsenodes/func.h"
#include "parsenodes/if.h"
#include "parsenodes/parsenode.h"
#include "parsenodes/program.h"
#include "parser/parser.h"
#include <iostream>
#include <string>
#include <vector>

Parser::Parser(Compiler *comp) : comp(comp), toks(NULL), curIndex(0), out(NULL)
{

}

Parser::~Parser() {
    delete out;
}

void Parser::genTree() {
    toks = comp->lexer->tokens();
    curIndex = 0;
    delete out;
    ParseNode *tmp = readInProgram();
    if (curIndex != toks->size())
        MCLError(1, "Stopped reading before EOF.", cur().loc.line,
        cur().loc.col);
    out = tmp;
}

ParseNode *Parser::getTree() {
    return out;
}

Token Parser::cur() const {
    if (curIndex >= toks->size())
        return Token(TOK_ERRTYPE);
    return (*toks)[curIndex];
}

void Parser::next() {
    curIndex++;
}

bool Parser::accept(TokenType type) const {
    return cur().type == type;
}

void Parser::expect(TokenType type) {
    if (!accept(type))
        MCLError(1, "Expected token '" + tokenTypeNames[(unsigned int)type]
        + "', instead got '" + tokenTypeNames[(unsigned int)cur().type] + "'.",
        cur().loc.line, cur().loc.col);
}

ParseNode *Parser::readInProgram() {
    unsigned int line, col;
    curLoc(line, col);
    std::vector<ParseNode *> childNodes;
    while (accept(TOK_TYPENAME))
        childNodes.push_back(readInFunc());
    return new ProgramNode(childNodes, {.loc = {line, col}});
}

ParseNode *Parser::readInFunc() {
    unsigned int line, col;
    curLoc(line, col);
    expect(TOK_TYPENAME);
    if (cur().content != "void")
        // TODO: Implement non-void functions
        MCLError(1, "Invalid return type '" + cur().content
        + "', needs to be 'void'.", cur().loc.line, cur().loc.col);
    next();
    expect(TOK_WORD);
    std::string name = cur().content;
    next();
    expect(TOK_LBRACE), next();
    expect(TOK_RBRACE), next();
    expect(TOK_LCBRACE);
    CodeBlockNode *codeblock = (CodeBlockNode *)readInCodeBlock();
    return new FuncNode(name, codeblock, {.loc = {line, col}});
}

ParseNode *Parser::readInCodeBlock() {
    unsigned int line, col;
    curLoc(line, col);
    std::vector<ParseNode *> childNodes;
    if (accept(TOK_LCBRACE)) {
        // Skip the '{'
        next();
        while (!accept(TOK_RCBRACE))
            childNodes.push_back(readInLine());
        // Skip the '}'
        next();
    } else {
        childNodes.push_back(readInLine());
    }
    return new CodeBlockNode(childNodes, {.loc = {line, col}});
}

ParseNode *Parser::readInCmd() {
    unsigned int line, col;
    curLoc(line, col);
    expect(TOK_CMD);
    ParseNode *out = new CmdNode(cur().content, {.loc = {line, col}});
    next();
    return out;
}

ParseNode *Parser::readInLine() {
    if (accept(TOK_CMD))
        return readInCmd();
    if (accept(TOK_EXEC_STMT))
        return readInExec();
    if (accept(TOK_IF))
        return readInIf();
    MCLError(1, "Invalid token type found.", cur().loc.line, cur().loc.col);
    return NULL;
}

ParseNode *Parser::readInExec() {
    unsigned int line, col;
    curLoc(line, col);
    expect(TOK_EXEC_STMT);
    std::string execType = cur().content;
    next();
    expect(TOK_LBRACE), next();
    expect(TOK_STR);
    std::string args = cur().content;
    next();
    expect(TOK_RBRACE), next();
    CodeBlockNode *codeblock = (CodeBlockNode *)readInCodeBlock();
    return new ExecNode(execType, args, codeblock, {.loc = {line, col}});
}

ParseNode *Parser::readInIf() {
    unsigned int line, col;
    curLoc(line, col);
    std::vector<std::string> ifArgs;
    std::vector<CodeBlockNode *> codeblocks;
    bool foundElse = false;
    do {
        // Skip the 'else'
        if (!accept(TOK_ELSE) && !accept(TOK_ELSEIF)) {
            // Expect an if-statement at the start
            expect(TOK_IF);
        }
        // "if", "else-if" or "else" statement
        if (accept(TOK_ELSEIF) || accept(TOK_IF)) {
            next();
            expect(TOK_LBRACE), next();
            expect(TOK_STR);
            ifArgs.push_back(cur().content);
            next();
            expect(TOK_RBRACE), next();
        } else if (accept(TOK_ELSE)) {
            next();
            foundElse = true;
        } else {
            MCLError(1, "Unexpected error unccured, invalid token found",
            cur().loc.line, cur().loc.col);
        }
        codeblocks.push_back((CodeBlockNode *)readInCodeBlock());
    } while ((accept(TOK_ELSEIF) || accept(TOK_ELSE)) && !foundElse);
    return new IfNode(ifArgs, codeblocks, {.loc = {line, col}});
}

void Parser::curLoc(unsigned int &line, unsigned int &col) const {
    line = cur().loc.line, col = cur().loc.col;
}
