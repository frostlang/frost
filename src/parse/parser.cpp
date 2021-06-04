#include <vector>
#include <parse/parser.h>
#include <parse/ast.h>


namespace Frost::Parse{


AST* Parser::parse(){

    try {
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
    
    }catch(ParseException& e){
        dbg() << "PANIC=parse exception\n";
    }

    return new ErrorAST(ErrorAST::create());
    
}

AST* Parser::statement(){
    switch(m_tokens->peek().type()){
        case TokenType::BREAK:{
            return new BreakAST(BreakAST::create());
        }
        case TokenType::CONTINUE:{
            return new ContinueAST(ContinueAST::create());
        }
        case TokenType::RETURN:{
            break;
        }
    }
    return 0;
}
AST* Parser::ifstmt(){

    IfAST if_ast = IfAST::create();
    if_ast.set_if_cond(expression());
    if_ast.set_if_body(statement());

    if(m_tokens->expect(TokenType::ELSE)){
        m_tokens->next();
        if_ast.set_else_body(statement());
    }

    return new IfAST(if_ast);
}

AST* Parser::forloop(){

    if(!m_tokens->expect(TokenType::FOR)){

    }

    //
    // the types of for are
    // single value = for x {}
    // 

    return 0;

}



//
// x : pub const U32 = 123;   ->   public constant U32
// x : mut U32 = 999;         ->   private mutable U32
// x : U32 999;               ->   private constant U32
//
//
//
//
//
//
AST* Parser::define(){

    if(!m_tokens->expect(TokenType::IDENTIFIER)){
        // TODO
    }
    auto& identifier = m_tokens->next();

    if(!m_tokens->expect(TokenType::COLON)){
        // TODO
    }

    return 0;


}

AST* Parser::expression(){
    return 0;
}

AST* Parser::identifier(){
    return 0;
}

AST* Parser::block(){

    std::vector<AST*> statements;

    if(!m_tokens->expect(TokenType::LCURLY))
        panic("expected { for opening block expression");
    m_tokens->next();
    
    while(!m_tokens->expect(TokenType::RCURLY)){
        statements.push_back(statement());
    }

    if(!m_tokens->expect(TokenType::RCURLY))
        panic("expected } for closing block expression");
    m_tokens->next();

    return new BlockAST(BlockAST::create(statements));
}

AST* Parser::group(){

    if(!m_tokens->expect(TokenType::LPAREN))
        panic("expected ( for opening group expression");
    m_tokens->next();

    expression();

    if(!m_tokens->expect(TokenType::RPAREN))
        panic("expected ) closing for group expression");
    m_tokens->next();
    
    return 0;

}


}