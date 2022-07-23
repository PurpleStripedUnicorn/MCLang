
#include "compiler/compiler.h"
#include "errorhandle/handle.h"
#include "general/loc.h"
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
#include "parsenodes/globalvar.h"
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
        MCLError(1, "Stopped reading before EOF.", cur().loc);
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
        cur().loc);
}

ParseNode *Parser::readInProgram() {
    Loc lastLoc = cur().loc;
    std::vector<ParseNode *> childNodes;
    while (true) {
        if (accept(TOK_TYPENAME))
            childNodes.push_back(readInFunc());
        else if (accept(TOK_NAMESPACE))
            childNodes.push_back(readInNamespace());
        else
            break;
    }
    return new ProgramNode(childNodes, lastLoc);
}

ParseNode *Parser::readInFunc() {
    Loc lastLoc = cur().loc;
    expect(TOK_TYPENAME);
    std::string type = cur().content;
    next();
    expect(TOK_WORD);
    std::string name = cur().content;
    next();
    // Function definition
    if (accept(TOK_LBRACE)) {
        if (type != "void")
            // TODO: Implement non-void functions
            MCLError(1, "Invalid return type '" + cur().content
            + "', needs to be 'void'.", cur().loc);
        expect(TOK_LBRACE), next();
        expect(TOK_RBRACE), next();
        expect(TOK_LCBRACE);
        CodeBlockNode *codeblock = (CodeBlockNode *)readInCodeBlock();
        return new FuncNode(name, codeblock, lastLoc);
    }
    // Global variable
    expect(TOK_SEMICOL), next();
    return new GlobalVarNode(type, name, lastLoc);
}

ParseNode *Parser::readInCodeBlock() {
    Loc lastLoc = cur().loc;
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
    return new CodeBlockNode(childNodes, lastLoc);
}

ParseNode *Parser::readInCmd() {
    Loc lastLoc = cur().loc;
    expect(TOK_CMD);
    ParseNode *out = new CmdNode(cur().content, lastLoc);
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
    Loc lastLoc = cur().loc;
    expect(TOK_EXEC_STMT);
    std::string execType = cur().content;
    next();
    expect(TOK_LBRACE), next();
    expect(TOK_STR);
    std::string args = cur().content;
    next();
    expect(TOK_RBRACE), next();
    CodeBlockNode *codeblock = (CodeBlockNode *)readInCodeBlock();
    return new ExecNode(execType, args, codeblock, lastLoc);
}

ParseNode *Parser::readInIf() {
    Loc lastLoc = cur().loc;
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
            cur().loc);
        }
        codeblocks.push_back((CodeBlockNode *)readInCodeBlock());
    } while ((accept(TOK_ELSEIF) || accept(TOK_ELSE)) && !foundElse);
    return new IfNode(ifArgs, codeblocks, lastLoc);
}

ParseNode *Parser::readInExpr() {
    return readInAssign();
}

ParseNode *Parser::readInAssign() {
    // Assignment is right associative
    Loc lastLoc = cur().loc;
    ParseNode *left = readInSum();
    if (accept(TOK_ASSIGN)) {
        next();
        ParseNode *right = readInAssign();
        if (left->getType() != PNODE_WORD)
            MCLError(1, "Left side of assignment is not a variable name",
            lastLoc);
        return new AssignNode(((WordNode *)left)->getContent(), right, lastLoc);
    }
    return left;
}

ParseNode *Parser::readInSum() {
    // Sums are left associative
    Loc lastLoc = cur().loc;
    ParseNode *cur = readInProd();
    while (accept(TOK_ADD) || accept(TOK_SUB)) {
        ParseNodeType ptype = PNODE_ADD;
        if (accept(TOK_SUB))
            ptype = PNODE_SUB;
        next();
        ParseNode *right = readInProd();
        cur = new ArithNode(ptype, cur, right, lastLoc);
    }
    return cur;
}

ParseNode *Parser::readInProd() {
    // Sums are left associative
    Loc lastLoc = cur().loc;
    ParseNode *cur = readInCall();
    while (accept(TOK_MUL) || accept(TOK_DIV) || accept(TOK_MOD)) {
        ParseNodeType ptype = PNODE_MUL;
        if (accept(TOK_DIV))
            ptype = PNODE_DIV;
        if (accept(TOK_MOD))
            ptype = PNODE_MOD;
        next();
        ParseNode *right = readInCall();
        cur = new ArithNode(ptype, cur, right, lastLoc);
    }
    return cur;
}

ParseNode *Parser::readInCall() {
    Loc lastLoc = cur().loc;
    // NOTE: This should be changed later to ensure proper order of operations
    if (accept(TOK_NUM)) {
        std::string content = cur().content;
        next();
        return new NumNode(content, lastLoc);
    }
    expect(TOK_WORD);
    std::string fname = cur().content;
    next();
    if (accept(TOK_LBRACE)) {
        next();
        expect(TOK_RBRACE), next();
        return new CallNode(fname, lastLoc);
    }
    return new WordNode(fname, lastLoc);
}

ParseNode *Parser::readInNamespace() {
    Loc lastLoc = cur().loc;
    expect(TOK_NAMESPACE), next();
    expect(TOK_WORD);
    std::string nsName = cur().content;
    next();
    expect(TOK_SEMICOL), next();
    return new NSNode(nsName, lastLoc);
}

ParseNode *Parser::readInVarInit() {
    Loc lastLoc = cur().loc;
    expect(TOK_TYPENAME);
    std::string varType = cur().content;
    next();
    ParseNode *childExpr = readInExpr();
    expect(TOK_SEMICOL), next();
    return new VarInitNode(varType, childExpr, lastLoc);
}
