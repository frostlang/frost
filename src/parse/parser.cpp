#include <vector>
#include <parse/parser.h>
#include <parse/ast.h>


namespace Frost::Parse{


AST* Parser::parse(){
    try {
        dbg() << "parsing...\n";

        std::vector<AST*> statements;

        while(!m_tokens->end()){
            statements.push_back(statement());
            break;
        }
        
        return new ProgramAST(ProgramAST::create(statements));
    
    }catch(ParseException& e){
        dbg() << "PANIC=parse exception\n";
    }

    return new ErrorAST(ErrorAST::create());
    
}

Type Parser::parse_type(){

    auto t = Type::create(TypeType::U0);


    switch(m_tokens->peek().type()){
        case TokenType::ARROW: {

            t.set_type(TypeType::POINTER);

            // now parse the inner type
            break;
        }
    }

    return t;
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
        case TokenType::IF:{
            return ifstmt();
            break;
        }
        case TokenType::LCURLY: {
            return block();
            break;
        }

        case TokenType::UNKNOWN: {
            dbg() << "unknown token type whils't parsing!\n";
            break;
        }
        default: return expression();
    }
    return new ErrorAST(ErrorAST::create());
}
AST* Parser::ifstmt(){
    m_tokens->consume(TokenType::IF);
    IfAST if_ast = IfAST::create();
    if_ast.set_if_cond(expression());
    if_ast.set_if_body(statement());

    if(m_tokens->consume(TokenType::ELSE).has()){
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
    return lor();
}


AST* Parser::lor(){
    auto higher_precedence = land();
    if(m_tokens->consume(TokenType::LOR).has()){
        dbg() << "LOR!\n";
        auto rhs = lor();
        return new LOrAST(LOrAST::create(higher_precedence, rhs));

    }
    return higher_precedence;
}
AST* Parser::land(){
    auto higher_precedence = bor();
    if(m_tokens->consume(TokenType::LAND).has()){
        dbg() << "LAND!\n";
        auto rhs = land();
        return new LAndAST(LAndAST::create(higher_precedence, rhs));

    }
    return higher_precedence;
}
AST* Parser::bor(){
    auto higher_precedence = band();
    if(m_tokens->consume(TokenType::BOR).has()){
        dbg() << "BOR!\n";
        auto rhs = bor();
        return new BOrAST(BOrAST::create(higher_precedence, rhs));

    }
    return higher_precedence;
}
AST* Parser::band(){
    auto higher_precedence = eq();
    if(m_tokens->consume(TokenType::AMPERSAND).has()){
        dbg() << "BAND!\n";
        auto rhs = band();
        return new BAndAST(BAndAST::create(higher_precedence, rhs));

    }
    return higher_precedence;
}
AST* Parser::eq(){
    return single();
    return 0;}
AST* Parser::cmp(){return 0;}
AST* Parser::shift(){return 0;}
AST* Parser::pm(){return 0;}
AST* Parser::mdmr(){return 0;}
AST* Parser::un(){return 0;}
AST* Parser::cast(){return 0;}


AST* Parser::block(){

    std::vector<AST*> statements;

    if(!m_tokens->consume(TokenType::LCURLY).has())
        panic("expected { for opening block expression");
    
    while(!m_tokens->expect(TokenType::RCURLY)){
        statements.push_back(statement());
    }

    if(!m_tokens->consume(TokenType::RCURLY).has())
        panic("expected } for closing block expression");

    return new BlockAST(BlockAST::create(statements));
}

AST* Parser::group(){

    if(!m_tokens->consume(TokenType::LPAREN).has())
        panic("expected ( for opening group expression");

    expression();

    if(!m_tokens->consume(TokenType::RPAREN).has())
        panic("expected ) closing for group expression");
    
    return 0;

}

//
// (){}
//
AST* Parser::fn(){

    return 0;

}

AST* Parser::single(){

    switch(m_tokens->peek().type()){
        case TokenType::IDENTIFIER: return identifier();
        case TokenType::NUMBER: return num();
        case TokenType::QUOTE: return string();
    }
    return 0;
}

AST* Parser::identifier(){
    
    // parse an identifier (variable)
    if(!m_tokens->expect(TokenType::IDENTIFIER)){}
    auto& token = m_tokens->next();

    return new VariableAST(VariableAST::create(token));
}

AST* Parser::string(){
    // parse an identifier (variable)
    if(!m_tokens->expect(TokenType::QUOTE)){}
    auto& opening_string = m_tokens->next();

    // expect an identifier
    if(!m_tokens->expect(TokenType::IDENTIFIER)){}
    auto& token = m_tokens->next();

    // ensure we have the closing string (")
    if(m_tokens->expect(opening_string.type())){}
    m_tokens->next();

    return new LiteralAST(LiteralAST::create(token));
}

AST* Parser::num(){

    // parse a number literal
    if(!m_tokens->expect(TokenType::NUMBER)){}
    auto& token = m_tokens->next();

    return new LiteralAST(LiteralAST::create(token));
}

}