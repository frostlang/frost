#include <vector>
#include <parse/parser.h>
#include <parse/ast.h>


namespace Frost::Parse{


AST* Parser::parse(){
    try {
        std::vector<AST*> statements;

        // TODO
        // for some reason we only parse once...
        while(!m_tokens->end()){
            statements.push_back(statement({}));
        }
        
        return new ProgramAST(ProgramAST::create(statements));
    
    }catch(ParseException& e){
        dbg() << "PANIC=parse exception\n";
    }

    return new ErrorAST(ErrorAST::create());
    
}

AST* Parser::statement(ParseContext ctx){
    switch(m_tokens->peek().type()){
        case TokenType::BREAK:{
            return brk(ctx);
        }
        case TokenType::CONTINUE:{
            return ctn(ctx);
        }
        case TokenType::RETURN:{
            return ret(ctx);
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
        default: return expression_stmt({});
    }
    return new ErrorAST(ErrorAST::create());
}


AST* Parser::ret(ParseContext){
    m_tokens->consume(TokenType::RETURN);

    AST* expr = 0;
    // todo check if we are expecting a value
    return new ReturnAST(false, expr);
}
AST* Parser::brk(ParseContext){
    return 0;
}
AST* Parser::ctn(ParseContext){
    return 0;
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
        return expression_stmt(ctx);
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
        ||m_tokens->expect(TokenType::STRUCT)
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
        ||m_tokens->expect(TokenType::PUB)
        ||m_tokens->expect(TokenType::PRIV)
        ||m_tokens->expect(TokenType::MUT)
        ||m_tokens->expect(TokenType::CONST)
        ||m_tokens->expect(TokenType::LBRACKET)
        ||m_tokens->expect(TokenType::ARROW)
        ||m_tokens->expect(TokenType::LPAREN)
        ||m_tokens->expect(TokenType::FN)
        
    )) return Optional<Type>();

    Type t = Type::create(Type::Storage::U0);

    if(m_tokens->consume(TokenType::PUB).has()){
        t.set_access(AccessType::PUB);
    }else if(m_tokens->consume(TokenType::PRIV).has()){
        t.set_access(AccessType::PRIV);
    }

    if(m_tokens->consume(TokenType::MUT).has()){
        t.set_mut(MutableType::MUT);
    }else if(m_tokens->consume(TokenType::CONST).has()){
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

    auto& next_type = m_tokens->next();
    switch(next_type.type()){
        case TokenType::U0: t.set_type(Type::Storage::U0); break;
        case TokenType::U1: t.set_type(Type::Storage::U1); break;
        case TokenType::U8: t.set_type(Type::Storage::U8); break;
        case TokenType::S8: t.set_type(Type::Storage::S8); break;
        case TokenType::U16: t.set_type(Type::Storage::U16); break;
        case TokenType::S16: t.set_type(Type::Storage::S16); break;
        case TokenType::U32: t.set_type(Type::Storage::U32); break;
        case TokenType::S32: t.set_type(Type::Storage::S32); break;
        case TokenType::TYPE: {
            t.set_type(Type::Storage::TYPE);
            break;
        }case TokenType::FN:{
            t.set_type(Type::Storage::FN);
            std::vector<Type> params;
            dbg() << "fn!\n";
            // now parse the fn params and return type
            if(m_tokens->consume(TokenType::LPAREN).has()){
                while(true){
                    auto next_param = type();
                    ASSERT(next_param.has());
                    t.inner_types().push_back(next_param.data());
                    if(m_tokens->consume(TokenType::RPAREN).has())
                        break;
                    m_tokens->consume(TokenType::COMMA);
                }
            }
            auto return_type = type();
            if(return_type.has()){
                t.inner_types().push_back(return_type.data());
            }
            for(auto& param : params)
                t.inner_types().push_back(param);
            break;
        }
        default:
            dbg() << "unknown type :(\n";
            break;
    }

    return Optional<Type>(t);
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
AST* Parser::decl(ParseContext ctx){
    // get the identifier
    auto& identifier = m_tokens->next();

    // now consume the :
    if(!m_tokens->consume(TokenType::COLON).has()){
        return new ErrorAST(ErrorAST::create());
    }

    auto t = type();

    u1 initialised = false;
    AST* initialiser = 0;
    if(m_tokens->consume(TokenType::ASSIGN).has()){
        initialised = true;
        initialiser=expression(ctx);
    }

    return new DeclAST(DeclAST::create(identifier,t.data(), initialised, initialiser));

}


AST* Parser::expression_stmt(ParseContext ctx){
    return new ExprStmtAST(assign(ctx));
}

AST* Parser::assign(ParseContext ctx){
    auto higher_precedence = expression(ctx);
    if(m_tokens->consume(TokenType::ASSIGN).has()){
        auto rhs = expression(ctx);
        return new AssignAST(higher_precedence, rhs);
    }
    return higher_precedence;
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
    auto higher_precedence = un(ctx);
    if(m_tokens->expect(TokenType::PLUS)){
        auto op = m_tokens->next();
        auto rhs = mdmr({});
        auto bin_op_type = BinOpAST::Op::PLUS;
        return new BinOpAST(BinOpAST::create(bin_op_type, higher_precedence, rhs));
    }
    return higher_precedence;
}
AST* Parser::un(ParseContext ctx){return cast(ctx);}
AST* Parser::cast(ParseContext ctx){return call(ctx);}
AST* Parser::call(ParseContext ctx){
    auto higher_precedence = single(ctx);
    if(m_tokens->consume(TokenType::LPAREN).has()){
        std::vector<AST*> args;
        if(!m_tokens->consume(TokenType::RPAREN).has()){
            // call ast
            while(true){
                auto next_arg = expression(ctx);
                args.push_back(next_arg);
                if(m_tokens->consume(TokenType::RPAREN).has())
                    break;
                m_tokens->consume(TokenType::COMMA);
            }
            m_tokens->consume(TokenType::RPAREN);
        }
        return new CallAST(higher_precedence, args);
    }
    return higher_precedence;
}


AST* Parser::block(ParseContext ctx){

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

//
// function literals can take the 2 basic forms:
// (){}
// {}
// () u32 {}
// u32 {}
AST* Parser::fn(ParseContext ctx){
dbg() << "fn!\n";
    m_tokens->consume(TokenType::LPAREN);
    m_tokens->consume(TokenType::RPAREN);
    /*// parse paramaters
    std::vector<AST*> param_decls;
    u1 has_params = m_tokens->consume(TokenType::LPAREN).has();
    while(has_params && !m_tokens->consume(TokenType::RPAREN).has()){
        param_decls.push_back(decl({}));
        m_tokens->consume(TokenType::COMMA);

    }*/
    
    // at this point we may be expecting a return type

    // if epect(type) || expect(identifier)... possible return type?

    // parse the body
    AST* body = statement(ctx);

    auto fn = FnAST(body);
    fn.mangled_identifier()="test_mangled_identifier";
    return new FnAST(fn);

}

AST* Parser::single(ParseContext ctx){
    switch(m_tokens->peek().type()){
        case TokenType::LPAREN: return group(ctx);
        case TokenType::IDENTIFIER: return var({});
        case TokenType::NUMBER: return num({});
        case TokenType::STRING: return string({});
        case TokenType::STRUCT: return strct({});
    }
    return 0;
}


AST* Parser::strct(ParseContext ctx){

    dbg() << "parsing struct\n";

    m_tokens->consume(TokenType::STRUCT);
    m_tokens->consume(TokenType::LCURLY);

    std::vector<AST*> decls;
    // a struct is simply a list of definitions
    if(!m_tokens->consume(TokenType::RCURLY).has()){
        // call ast
        while(true){
            auto next_decl = decl(ctx);
            decls.push_back(next_decl);
            if(m_tokens->consume(TokenType::RCURLY).has())
                break;
        }
        m_tokens->consume(TokenType::RPAREN);
    }

    return new StructAST(decls);
}

AST* Parser::group(ParseContext ctx){
    // TODO for now just do fn's
    return fn(ctx);
}
AST* Parser::var(ParseContext){
    // TODO bug
    // next has to be used here, if we use consume it just dies
    // basically we can't consume if we need the token value!
    auto& token = m_tokens->next();
    return new VariableAST(VariableAST::create(token));
}

AST* Parser::string(ParseContext){
    auto& string_token = m_tokens->next();
    return new StringAST(string_token);
}

AST* Parser::num(ParseContext){
    auto& token = m_tokens->next();
    return new LiteralAST(LiteralAST::create(token, Frost::Type(Frost::Type::Storage::U8)));
}

}