#include <vector>
#include <parse/parser.h>
#include <parse/ast.h>


namespace Frost::Parse{


AST* Parser::parse(){
    try {
        dbg() << "parsing...\n";

        std::vector<AST*> statements;

        while(!m_tokens->end()){
            statements.push_back(statement({}));
            break;
        }
        
        return new ProgramAST(ProgramAST::create(statements));
    
    }catch(ParseException& e){
        dbg() << "PANIC=parse exception\n";
    }

    return new ErrorAST(ErrorAST::create());
    
}

AST* Parser::statement(ParseContext){
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
        case TokenType::FOR:{
            return forloop({});
        }
        case TokenType::IF:{
            return ifstmt({});
        }
        case TokenType::LCURLY: {
            return block({});
            break;
        }
        case TokenType::IDENTIFIER:{
            // this could potentially be a decl, or a variable
            return identifier({});
        }
        case TokenType::UNKNOWN: {
            dbg() << "unknown token type whils't parsing!\n";
            break;
        }
        default: return expression({});
    }
    return new ErrorAST(ErrorAST::create());
}
AST* Parser::ifstmt(ParseContext){
    m_tokens->consume(TokenType::IF);
    IfAST if_ast = IfAST::create();
    if_ast.set_if_cond(expression({}));
    if_ast.set_if_body(statement({}));

    if(m_tokens->consume(TokenType::ELSE).has()){
        if_ast.set_else_body(statement({}));
    }

    return new IfAST(if_ast);
}

AST* Parser::forloop(ParseContext){

    if(!m_tokens->expect(TokenType::FOR)){

    }

    //
    // the types of for are
    // single value = for x {}
    // 

    return 0;

}

// this means we have seen an identifier...
// it could be a decl or a variable
AST* Parser::identifier(ParseContext ctx){
    
    // dealing with a decleration here!
    if(m_tokens->peek(1).type()==TokenType::COLON){
        return decl(ctx);
    }else{
        // else we are dealing with an expr e.g. x + 33;
        return expression(ctx);
    }
}

//
// types can have the following form:
// mut U32
// const U32
// mut ^U32
// mut []U32
// mut ()
Optional<Type> Parser::type(){

    //
    // todo
    // optimise this by casting to u8 and then checking > and < than TYPE and LPAREN etc
    //
    if(!(
        m_tokens->expect(TokenType::TYPE)
        ||m_tokens->expect(TokenType::ANY)
        ||m_tokens->expect(TokenType::U0)
        ||m_tokens->expect(TokenType::U1)
        ||m_tokens->expect(TokenType::U8)
        ||m_tokens->expect(TokenType::S8)
        ||m_tokens->expect(TokenType::U16)
        ||m_tokens->expect(TokenType::S16)
        ||m_tokens->expect(TokenType::U32)
        ||m_tokens->expect(TokenType::S32)
        ||m_tokens->expect(TokenType::F32)
        ||m_tokens->expect(TokenType::U64)
        ||m_tokens->expect(TokenType::S64)
        ||m_tokens->expect(TokenType::F64)
        ||m_tokens->expect(TokenType::MUT)
        ||m_tokens->expect(TokenType::CONST)
        ||m_tokens->expect(TokenType::LBRACKET)
        ||m_tokens->expect(TokenType::ARROW)
        ||m_tokens->expect(TokenType::LPAREN)
        
    )) return Optional<Type>();

    Type t = Type::create(Type::Storage::U0);

    if(m_tokens->consume(TokenType::MUT).has()){
        t.set_mut(MutableType::MUT);
    }
    else if(m_tokens->consume(TokenType::CONST).has()){
        t.set_mut(MutableType::CONST);
    }

    // tuple :)
    if(m_tokens->consume(TokenType::LPAREN).has()){
        while(!m_tokens->consume(TokenType::RPAREN).has()){
            auto inner = type();
            if(inner.has())
                t.inner_types().push_back(inner.data());
            else
                dbg() << "um... this is an error?\n";
        }
    }

    // array
    if(m_tokens->consume(TokenType::LBRACKET).has()){
        if(m_tokens->consume(TokenType::RBRACKET).has()){
            t.set_type(Type::Storage::SLICE);
            auto inner = type();
            if(inner.has())
                t.inner_types().push_back(inner.data());  
            else
                dbg() << "um... this is an error?\n";
        }else{
            // sized array
            auto size = m_tokens->consume(TokenType::NUMBER);
            t.set_type(Type::Storage::ARRAY);
            m_tokens->consume(TokenType::RBRACKET);
        }
    }

    if(auto n = m_tokens->consume(TokenType::U0); n.has()){
        t.set_type(Type::Storage::U0);
    }else if(auto n = m_tokens->consume(TokenType::U1); n.has()){
        t.set_type(Type::Storage::U1);
    }else if(auto n = m_tokens->consume(TokenType::U8); n.has()){
        t.set_type(Type::Storage::U8);
    }else if(auto n = m_tokens->consume(TokenType::S8); n.has()){
        t.set_type(Type::Storage::S8);
    }


    return Optional<Type>();
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
AST* Parser::decl(ParseContext){

    // get the identifier
    auto& identifier = m_tokens->consume(TokenType::IDENTIFIER).data();


    // now consume the :
    if(!m_tokens->consume(TokenType::COLON).has()){
        return new ErrorAST(ErrorAST::create());
    }

    // now look for the type information
    if(auto t = type(); t.has()){
        // has a type
    }

    if(!m_tokens->consume(TokenType::SEMICOLON).has()){
        // has a value
    }

    dbg() << "done decl!\n";

    return 0;

}

AST* Parser::expression(ParseContext){
    return lor({});
}


AST* Parser::lor(ParseContext){
    auto higher_precedence = land({});
    if(m_tokens->consume(TokenType::LOR).has()){
        dbg() << "LOR!\n";
        auto rhs = lor({});
        return new BinOpAST(BinOpAST::create(BinOpAST::Op::LOR, higher_precedence, rhs));

    }
    return higher_precedence;
}
AST* Parser::land(ParseContext){
    auto higher_precedence = bor({});
    if(m_tokens->consume(TokenType::LAND).has()){
        dbg() << "LAND!\n";
        auto rhs = land({});
        return new BinOpAST(BinOpAST::create(BinOpAST::Op::LAND, higher_precedence, rhs));

    }
    return higher_precedence;
}
AST* Parser::bor(ParseContext){
    auto higher_precedence = band({});
    if(m_tokens->consume(TokenType::BOR).has()){
        dbg() << "BOR!\n";
        auto rhs = bor({});
        return new BinOpAST(BinOpAST::create(BinOpAST::Op::BOR, higher_precedence, rhs));

    }
    return higher_precedence;
}
AST* Parser::band(ParseContext){
    auto higher_precedence = eq({});
    if(m_tokens->consume(TokenType::AMPERSAND).has()){
        dbg() << "BAND!\n";
        auto rhs = band({});
        return new BinOpAST(BinOpAST::create(BinOpAST::Op::BAND, higher_precedence, rhs));

    }
    return higher_precedence;
}
AST* Parser::eq(ParseContext){
    auto higher_precedence = cmp({});
    if(m_tokens->expect(TokenType::EQUALS) || m_tokens->expect(TokenType::NEQUALS)){
        auto op = m_tokens->next();
        auto rhs = eq({});
        auto bin_op_type = (op.type()==TokenType::EQUALS) ? BinOpAST::Op::EQ : BinOpAST::Op::NEQ;
        return new BinOpAST(BinOpAST::create(bin_op_type, higher_precedence, rhs));
    }
    return higher_precedence;
}

AST* Parser::cmp(ParseContext){return mdmr({});}
AST* Parser::shift(ParseContext){return 0;}
AST* Parser::pm(ParseContext){return 0;}
AST* Parser::mdmr(ParseContext ctx){
    auto higher_precedence = single(ctx);
    if(m_tokens->expect(TokenType::PLUS)){
        auto op = m_tokens->next();
        auto rhs = mdmr({});
        auto bin_op_type = BinOpAST::Op::PLUS;
        return new BinOpAST(BinOpAST::create(bin_op_type, higher_precedence, rhs));
    }
    return higher_precedence;
}
AST* Parser::un(ParseContext){return 0;}
AST* Parser::cast(ParseContext){return 0;}


AST* Parser::block(ParseContext){

    std::vector<AST*> statements;

    if(!m_tokens->consume(TokenType::LCURLY).has())
        panic("expected { for opening block expression");
    
    while(!m_tokens->expect(TokenType::RCURLY)){
        statements.push_back(statement({}));
    }

    if(!m_tokens->consume(TokenType::RCURLY).has())
        panic("expected } for closing block expression");

    return new BlockAST(BlockAST::create(statements));
}

AST* Parser::group(ParseContext){

    if(!m_tokens->consume(TokenType::LPAREN).has())
        panic("expected ( for opening group expression");

    expression({});

    if(!m_tokens->consume(TokenType::RPAREN).has())
        panic("expected ) closing for group expression");
    
    return 0;

}

//
// function literals can take the 2 basic forms:
// (){}
// {}
// () u32 {}
// u32 {}
AST* Parser::fn(ParseContext){

    // parse paramaters
    std::vector<AST*> param_decls;
    u1 has_params = m_tokens->consume(TokenType::LPAREN).has();
    while(has_params && !m_tokens->consume(TokenType::RPAREN).has()){
        param_decls.push_back(decl({}));
        m_tokens->consume(TokenType::COMMA);

    }
    
    // at this point we may be expecting a return type

    // if epect(type) || expect(identifier)... possible return type?

    // parse the body
    AST* body = statement({});



    return 0;

}

AST* Parser::single(ParseContext){

    switch(m_tokens->peek().type()){
        case TokenType::IDENTIFIER: return var({});
        case TokenType::NUMBER: return num({});
        case TokenType::QUOTE: return string({});
    }
    return 0;
}

AST* Parser::var(ParseContext){
    auto& token = m_tokens->consume(TokenType::IDENTIFIER).data();
    return new VariableAST(VariableAST::create(token));
}

AST* Parser::string(ParseContext){
    // parse an identifier (variable)
    if(!m_tokens->expect(TokenType::QUOTE)){}
    auto& opening_string = m_tokens->next();

    // expect an identifier
    if(!m_tokens->expect(TokenType::IDENTIFIER)){}
    auto& token = m_tokens->next();

    // ensure we have the closing string (")
    if(m_tokens->expect(opening_string.type())){}
    m_tokens->next();

    return new LiteralAST(LiteralAST::create(token, Frost::Type(Frost::Type::Storage::POINTER)));
}

AST* Parser::num(ParseContext){

    // parse a number literal
    if(!m_tokens->expect(TokenType::NUMBER)){}
    auto& token = m_tokens->next();

    return new LiteralAST(LiteralAST::create(token, Frost::Type(Frost::Type::Storage::U8)));
}

}