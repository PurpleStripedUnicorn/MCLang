
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
        std::cout << "Stopped reading before EOF" << std::endl;
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
        std::cout << "Expected a different token type! Located at line "
        << cur().loc.line << "[" << cur().loc.col << "]" << std::endl;
}

ParseNode *Parser::readInProgram() {
    std::vector<ParseNode *> childNodes;
    while (accept(TOK_TYPENAME))
        childNodes.push_back(readInFunc());
    return new ProgramNode(childNodes);
}

ParseNode *Parser::readInFunc() {
    expect(TOK_TYPENAME);
    if (cur().content != "void")
        std::cout << "Invalid typename for function, has to be 'void'"
        << std::endl;
    next();
    expect(TOK_WORD);
    std::string name = cur().content;
    next();
    expect(TOK_LBRACE), next();
    expect(TOK_RBRACE), next();
    expect(TOK_LCBRACE), next();
    CodeBlockNode *codeblock = (CodeBlockNode *)readInCodeBlock();
    expect(TOK_RCBRACE), next();
    return new FuncNode(name, codeblock);
}

ParseNode *Parser::readInCodeBlock() {
    std::vector<ParseNode *> childNodes;
    while (!accept(TOK_RCBRACE))
        childNodes.push_back(readInCmd());
    return new CodeBlockNode(childNodes);
}

ParseNode *Parser::readInCmd() {
    expect(TOK_CMD);
    ParseNode *out = new CmdNode(cur().content);
    next();
    return out;
}
