
#include "parser/parser.h"

Parser::Parser(Compiler *comp) : toks(comp->lexer->tokens()), curIndex(0), out(NULL) {

}

Parser::~Parser() {
    delete out;
}

ParseNode *Parser::genTree() {
    curIndex = 0;
    delete out;
    ParseNode *tmp = readInProgram();
    if (curIndex != toks->size())
        MCLError(1, "Stopped reading before EOF.", cur().loc.line,
        cur().loc.col);
    out = tmp;
    return tmp;
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

void Parser::curLoc(unsigned int &line, unsigned int &col) const {
    line = cur().loc.line, col = cur().loc.col;
}
