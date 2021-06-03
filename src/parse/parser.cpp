#include <vector>
#include <parse/parser.h>
#include <parse/ast.h>


namespace Frost::Parse{


AST* Parser::parse(){

    dbg() << "parsing...\n";

    std::vector<AST*> statements;

    while(!m_tokens->end()){

        auto t = m_tokens->next();

        dbg() << "token="<<t<<"\n";
        
        switch(t.type()){

            case TokenType::IDENTIFIER: {
                statements.push_back(identifier());
                break;
            }

            case TokenType::LCURLY: {
                statements.push_back(block());
                break;
            }

            case TokenType::UNKNOWN: {
                dbg() << "unknown token type whils't parsing!\n";
                break;
            }

            default: {
                dbg() << "um...\n";
                break;
            }
        }

        break;
    }

    return new ProgramAST(ProgramAST::create(statements));
}

AST* Parser::identifier(){
    return 0;
}

AST* Parser::block(){

    std::vector<AST*> statements;

    return new BlockAST(BlockAST::create(statements));
}

}