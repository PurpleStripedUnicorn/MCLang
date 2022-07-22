
#include "compiler/compiler.h"
#include "errorhandle/handle.h"
#include "lexer/debug.h"
#include "lexer/lexer.h"
#include "lexer/token.h"
#include "parsenodes/call.h"
#include "parsenodes/cmd.h"
#include "parsenodes/codeblock.h"
#include "parsenodes/exec.h"
#include "parsenodes/expr/arith.h"
#include "parsenodes/expr/assign.h"
#include "parsenodes/expr/expr.h"
#include "parsenodes/expr/num.h"
#include "parsenodes/func.h"
#include "parsenodes/if.h"
#include "parsenodes/namespace.h"
#include "parsenodes/parsenode.h"
#include "parsenodes/program.h"
#include "parsenodes/varinit.h"
#include "parsenodes/word.h"
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
    while (true) {
        if (accept(TOK_TYPENAME))
            childNodes.push_back(readInFunc());
        else if (accept(TOK_NAMESPACE))
            childNodes.push_back(readInNamespace());
        else
            break;
    }
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
    if (accept(TOK_TYPENAME))
        return readInVarInit();
    // If there are no special tokens found, try to read an expression, and then
    // a semicolon
    ParseNode *expr = readInExpr();
    expect(TOK_SEMICOL), next();
    return expr;
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

ParseNode *Parser::readInExpr() {
    return readInAssign();
}

ParseNode *Parser::readInAssign() {
    // Assignment is right associative
    unsigned int line, col;
    curLoc(line, col);
    ParseNode *left = readInSum();
    if (accept(TOK_ASSIGN)) {
        next();
        ParseNode *right = readInAssign();
        if (left->getType() != PNODE_WORD)
            MCLError(1, "Left side of assignment is not a variable name", line,
            col);
        return new AssignNode(((WordNode *)left)->getContent(), right, {.loc =
        {line, col}});
    }
    return left;
}

ParseNode *Parser::readInSum() {
    // Sums are left associative
    unsigned int line, col;
    curLoc(line, col);
    ParseNode *cur = readInProd();
    while (accept(TOK_ADD) || accept(TOK_SUB)) {
        ParseNodeType ptype = PNODE_ADD;
        if (accept(TOK_SUB))
            ptype = PNODE_SUB;
        next();
        ParseNode *right = readInProd();
        cur = new ArithNode(ptype, cur, right, {.loc = {line, col}});
    }
    return cur;
}

ParseNode *Parser::readInProd() {
    // Sums are left associative
    unsigned int line, col;
    curLoc(line, col);
    ParseNode *cur = readInCall();
    while (accept(TOK_MUL) || accept(TOK_DIV) || accept(TOK_MOD)) {
        ParseNodeType ptype = PNODE_MUL;
        if (accept(TOK_DIV))
            ptype = PNODE_DIV;
        if (accept(TOK_MOD))
            ptype = PNODE_MOD;
        next();
        ParseNode *right = readInCall();
        cur = new ArithNode(ptype, cur, right, {.loc = {line, col}});
    }
    return cur;
}

ParseNode *Parser::readInCall() {
    unsigned int line, col;
    curLoc(line, col);
    // NOTE: This should be changed later to ensure proper order of operations
    if (accept(TOK_NUM)) {
        std::string content = cur().content;
        next();
        return new NumNode(content, {.loc = {line, col}});
    }
    expect(TOK_WORD);
    std::string fname = cur().content;
    next();
    if (accept(TOK_LBRACE)) {
        next();
        expect(TOK_RBRACE), next();
        return new CallNode(fname, {.loc = {line, col}});
    }
    return new WordNode(fname, {.loc = {line, col}});
}

ParseNode *Parser::readInNamespace() {
    unsigned int line, col;
    curLoc(line, col);
    expect(TOK_NAMESPACE), next();
    expect(TOK_WORD);
    std::string nsName = cur().content;
    next();
    expect(TOK_SEMICOL), next();
    return new NSNode(nsName, {.loc = {line, col}});
}

ParseNode *Parser::readInVarInit() {
    unsigned int line, col;
    curLoc(line, col);
    expect(TOK_TYPENAME);
    std::string varType = cur().content;
    next();
    ParseNode *childExpr = readInExpr();
    expect(TOK_SEMICOL), next();
    return new VarInitNode(varType, childExpr, {.loc = {line, col}});
}

void Parser::curLoc(unsigned int &line, unsigned int &col) const {
    line = cur().loc.line, col = cur().loc.col;
}
