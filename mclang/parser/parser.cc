
#include "compiler/compiler.h"
#include "errorhandle/handle.h"
#include "general/funcdef.h"
#include "general/loc.h"
#include "general/types.h"
#include "general/var.h"
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

void Parser::prev() {
    if (curIndex > 0)
        curIndex--;
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
        if (accept(TOK_TYPENAME) || accept(TOK_CONST))
            childNodes.push_back(readInDef());
        else if (accept(TOK_NAMESPACE))
            childNodes.push_back(readInNamespace());
        else
            break;
    }
    return new ProgramNode(childNodes, lastLoc);
}

ParseNode *Parser::readInDef() {
    Loc lastLoc = cur().loc;
    Type type = readInType();
    expect(TOK_WORD);
    std::string name = cur().content;
    next();
    if (accept(TOK_LBRACE))
        return readInFunc(type, name, lastLoc);
    return readInGlobalVar(type, lastLoc);
}

ParseNode *Parser::readInGlobalVar(Type type, Loc lastLoc)
{
    // To also read the name of the variable, we need to move one token back
    prev();
    ParseNode *childExpr = readInExpr();
    expect(TOK_SEMICOL), next();
    return new GlobalVarNode(type, childExpr, lastLoc);
}

ParseNode *Parser::readInFunc(Type type, std::string funcName, Loc lastLoc) {
    expect(TOK_LBRACE), next();
    std::vector<Param> params;
    if (accept(TOK_TYPENAME) || accept(TOK_CONST)) {
        params.push_back(readInFuncParam());
        while (accept(TOK_COMMA)) {
            next();
            params.push_back(readInFuncParam());
        }
    }
    expect(TOK_RBRACE), next();
    expect(TOK_LCBRACE);
    CodeBlockNode *codeblock = (CodeBlockNode *)readInCodeBlock();
    return new FuncNode(type, funcName, params, codeblock, lastLoc);
}

Param Parser::readInFuncParam() {
    Type type = readInType();
    expect(TOK_WORD);
    std::string name = cur().content;
    next();
    return Param(type, name);
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
    if (accept(TOK_TYPENAME) || accept(TOK_CONST))
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
    ParseNode *cur = readInTerm();
    while (accept(TOK_MUL) || accept(TOK_DIV) || accept(TOK_MOD)) {
        ParseNodeType ptype = PNODE_MUL;
        if (accept(TOK_DIV))
            ptype = PNODE_DIV;
        if (accept(TOK_MOD))
            ptype = PNODE_MOD;
        next();
        ParseNode *right = readInTerm();
        cur = new ArithNode(ptype, cur, right, lastLoc);
    }
    return cur;
}

ParseNode *Parser::readInTerm() {
    if (accept(TOK_NUM)) {
        Loc lastLoc = cur().loc;
        std::string content = cur().content;
        next();
        return new NumNode(content, lastLoc);
    }
    return readInCall();
}

ParseNode *Parser::readInCall() {
    Loc lastLoc = cur().loc;
    expect(TOK_WORD);
    std::string fname = cur().content;
    next();
    if (accept(TOK_LBRACE)) {
        next();
        std::vector<ParseNode *> params;
        if (!accept(TOK_RBRACE))
            params.push_back(readInExpr());
        while (!accept(TOK_RBRACE)) {
            expect(TOK_COMMA), next();
            params.push_back(readInExpr());
        }
        expect(TOK_RBRACE), next();
        return new CallNode(fname, params, lastLoc);
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
    Type varType = readInType();
    ParseNode *childExpr = readInExpr();
    expect(TOK_SEMICOL), next();
    return new VarInitNode(varType, childExpr, lastLoc);
}

Type Parser::readInType() {
    Type out;
    if (accept(TOK_CONST)) {
        out.isConst = true;
        next();
    } else {
        out.isConst = false;
    }
    expect(TOK_TYPENAME);
    if (cur().content == "void")
        out.base = TYPE_VOID;
    if (cur().content == "int")
        out.base = TYPE_INT;
    if (cur().content == "bool")
        out.base = TYPE_BOOL;
    if (cur().content == "str")
        out.base = TYPE_STR;
    next();
    return out;
}
