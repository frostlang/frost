#include <gen/x86astgen.h>

namespace Frost::Gen::X86_64{




void X86ASTGenerator::gen(){
    X86_64::BuildContext ctx = X86_64::BuildContext::create();
    visit(m_ast, ctx);
    dbg() << ctx.block();
}


void X86ASTGenerator::emit(const char* instr){
    
}

Optional<X86_64::Operand> X86ASTGenerator::visit(Parse::AST* ast, X86_64::BuildContext& ctx){
    switch(ast->type()){
        case Parse::AST::Type::PROGRAM:{
            return visit(static_cast<Parse::ProgramAST*>(ast), ctx);
        }case Parse::AST::Type::BLOCK:{
            return visit(static_cast<Parse::BlockAST*>(ast), ctx);
        }case Parse::AST::Type::EXPR_STMT:{
            return visit(static_cast<Parse::ExprStmtAST*>(ast), ctx);
        }case Parse::AST::Type::DECL:{
            return visit(static_cast<Parse::DeclAST*>(ast), ctx);
        }case Parse::AST::Type::ASSIGN:{
            return visit(static_cast<Parse::AssignAST*>(ast), ctx);
        }case Parse::AST::Type::RETURN:{
            return visit(static_cast<Parse::ReturnAST*>(ast), ctx);
        } case Parse::AST::Type::BIN:{
            return visit(static_cast<Parse::BinOpAST*>(ast), ctx);
        } case Parse::AST::Type::VARIABLE:{
            return visit(static_cast<Parse::VariableAST*>(ast), ctx);
        } case Parse::AST::Type::LITERAL:{
            return visit(static_cast<Parse::LiteralAST*>(ast), ctx);
        } case Parse::AST::Type::FN:{
            return visit(static_cast<Parse::FnAST*>(ast), ctx);
        }
    }
    return Optional<X86_64::Operand>();
}

Optional<X86_64::Operand> X86ASTGenerator::visit(Parse::ProgramAST* program_ast, X86_64::BuildContext& ctx){
    // create the main fn
    ctx.block().push(X86_64::Instruction::create("global main"));
    ctx.block().push(X86_64::Instruction::create("main:"));
    ctx.block().push(X86_64::Instruction::create("ret"));
    for(auto& ast : program_ast->statements()){
        visit(ast, ctx);
    }
    return Optional<X86_64::Operand>();
}
Optional<X86_64::Operand> X86ASTGenerator::visit(Parse::BlockAST* block_ast, X86_64::BuildContext& ctx){
    // create the main fn
    for(auto& ast : block_ast->statements()){
        visit(ast, ctx);
    }
    return Optional<X86_64::Operand>();
}
Optional<X86_64::Operand> X86ASTGenerator::visit(Parse::ReturnAST* return_ast, X86_64::BuildContext& ctx){
    ctx.block().push(X86_64::Instruction::create("ret"));
    return Optional<X86_64::Operand>();
}

Optional<X86_64::Operand> X86ASTGenerator::visit(Parse::IfAST* if_ast, X86_64::BuildContext& ctx){
    ctx.block().push(X86_64::Instruction::create("if_start:"));
    ctx.block().push(X86_64::Instruction::create("if_end:"));
    return Optional<X86_64::Operand>();
}

Optional<X86_64::Operand> X86ASTGenerator::visit(Parse::DeclAST* decl_ast, X86_64::BuildContext& ctx){

    auto stack_offset = ctx.alloc_stack();
    //std::stringstream ss;
    //ss << "[rbp-" << stack_offset << "]";
    auto operand = Operand::create(
        OperandEncoding::create(decl_ast->lit_type(), OperandEncoding::EncodingType::MEM),
        MemoryLocation(
            MemoryLocation::Value(MemoryLocation::Value(Register(Register::Type::RBP))),
            MemoryLocation::Value(MemoryLocation::Value(stack_offset))
            )
    );

    m_sym_table.put(s(decl_ast->token().value()), operand);

    if(decl_ast->value()){
        // do a move!
        auto rhs = visit(decl_ast->value(), ctx).data();
        ctx.block().push(Instruction::create("mov", operand, rhs));
    }

/*
    if(ctx.scope()==X86::BuildContext::Scope::GLOBAL){
        //
    }else if(ctx.scope()==X86::BuildContext::Scope::FN){
        // get an offset on the stack
        auto stack_offset = ctx.alloc_stack();
    }
*/
    return Optional<X86_64::Operand>(operand);
}




Optional<X86_64::Operand> X86ASTGenerator::visit(Parse::ExprStmtAST* expr_stmt_ast, X86_64::BuildContext& ctx){
    return visit(expr_stmt_ast->expr(), ctx);
}


Optional<X86_64::Operand> X86ASTGenerator::visit(Parse::AssignAST* assign_ast, X86_64::BuildContext& ctx){

    // TODO
    // we might want to make the ctx a non reference so we can create new ones when moving on
    // when getting the lhs, we want it's location not its value
    ctx.value_type()=BuildContext::ValueType::LOCATION;
    auto mov_lhs = visit(assign_ast->lhs(), ctx);
    
    ctx.value_type()=BuildContext::ValueType::VALUE;
    ASSERT(mov_lhs.has());
    auto mov_rhs = visit(assign_ast->rhs(), ctx);
    ASSERT(mov_rhs.has());
    X86_64::Instruction mov = X86_64::Instruction::create("mov", mov_lhs.data(), mov_rhs.data());
    ctx.block().push(mov);

    return Optional<X86_64::Operand>();
}


// example of a literal ast
Optional<X86_64::Operand> X86ASTGenerator::visit(Parse::BinOpAST* bin_op_ast, X86_64::BuildContext& ctx){
    switch(bin_op_ast->op()){
        case Parse::BinOpAST::Op::PLUS: {


            //
            // x86 encoding for add is 
            // add rax, 1
            //
            // so, we need to take the lhs and rhs operands. and lookup the instruction that uses those operandencodings with the add instr



            // 1. get the lhs and rhs Operands

            X86_64::Operand lhs = X86_64::Operand::create(
                X86_64::OperandEncoding::create(X86_64::OperandEncoding::EncodingType::REG, X86_64::OperandEncoding::Size::_8),
                ctx.alloc_reg(X86_64::OperandEncoding::Size::_8)
            );

            lhs = visit(bin_op_ast->lhs(), ctx).data();
            X86_64::Operand rhs = visit(bin_op_ast->rhs(), ctx).data();
           
            auto add = X86_64::Instruction::create("add", lhs, rhs);

            ctx.block().push(add);



            break;
        }
    }

    return Optional<X86_64::Operand>();
    
}

Optional<X86_64::Operand> X86ASTGenerator::visit(Parse::VariableAST* variable_ast, X86_64::BuildContext& ctx){    
    
    
    if(ctx.value_type()==BuildContext::ValueType::VALUE){
        
        // first get the encoding
        X86_64::OperandEncoding encoding = X86_64::OperandEncoding::create(variable_ast->var_type(), OperandEncoding::EncodingType::REG);

        // then find a register to put the variable in
        X86_64::Register reg = ctx.alloc_reg(encoding.size());

        Operand var_location = m_sym_table.get(s(variable_ast->token().value())).data();

        X86_64::Operand mov_lhs = X86_64::Operand::create(encoding, reg);
        X86_64::Instruction mov = X86_64::Instruction::create("mov", mov_lhs, var_location);
        ctx.block().push(mov);

        // finally return the register the variable is now in
        return Optional<X86_64::Operand>(X86_64::Operand::create(encoding, reg));
    
    
    }else if(ctx.value_type()==BuildContext::ValueType::LOCATION){
        return m_sym_table.get(s(variable_ast->token().value())).data();
    }
    return Optional<X86_64::Operand>();
}



Optional<X86_64::Operand> X86ASTGenerator::visit(Parse::LiteralAST* literal_ast, X86_64::BuildContext& ctx){    

    // so... the problem with this is that we need to know if we are within a nested operation
    // e.g. 1 + 2 + 3
    // otherwise ^ would generate
    // 
    // add al 1
    // 2   3  0
    //
    // the 2 is the first paramater and so should be stored in a register

    // first get the encoding
    auto encoding = X86_64::OperandEncoding::create(literal_ast->lit_type(), OperandEncoding::EncodingType::IMM);
    // then create the operand
    auto op = X86_64::Operand::create(encoding, std::stoi(literal_ast->token().value().data()));
    return Optional<X86_64::Operand>(op);
}


Optional<X86_64::Operand> X86ASTGenerator::visit(Parse::FnAST* fn_ast, X86_64::BuildContext& ctx){
    ctx.block().push(Instruction::create("function:"));
    visit(fn_ast->body(), ctx);
    ctx.block().push(Instruction::create("ret"));
    return Optional<X86_64::Operand>();
}
}