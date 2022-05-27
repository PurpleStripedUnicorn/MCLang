
#include "parser/parser.h"

Parser::Parser(const std::vector<Token> &toks) : toks(toks), curIndex(0),
out(NULL) {

}

Parser::~Parser() {
    delete out;
}

ParseNode *Parser::genTree() {
    curIndex = 0;
    delete out;
    ParseNode *out = readInProgram();
    if (curIndex != toks.size())
        // TODO: Implement better error handling
        MCLError(1, "Stopped reading before EOF.", cur().loc.line,
        cur().loc.col);
    return out;
}

Token Parser::cur() const {
    if (curIndex >= toks.size())
        return Token(TOK_ERRTYPE);
    return toks[curIndex];
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
    expect(TOK_LCBRACE), next();
    CodeBlockNode *codeblock = (CodeBlockNode *)readInCodeBlock();
    expect(TOK_RCBRACE), next();
    return new FuncNode(name, codeblock, {.loc = {line, col}});
}

ParseNode *Parser::readInCodeBlock() {
    unsigned int line, col;
    curLoc(line, col);
    std::vector<ParseNode *> childNodes;
    while (!accept(TOK_RCBRACE))
        childNodes.push_back(readInCmd());
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

void Parser::curLoc(unsigned int &line, unsigned int &col) const {
    line = cur().loc.line, col = cur().loc.col;
}
