#include <gen/x86astgen.h>

namespace Frost::Gen::X86{




void X86ASTGenerator::gen(){
    dbg() << "gen!\n";
    X86::BuildContext ctx = X86::BuildContext::create();
    visit(m_ast, ctx);
    dbg() << ctx.block();
}


void X86ASTGenerator::emit(const char* instr){
    
}

Optional<X86::Operand> X86ASTGenerator::visit(Parse::AST* ast, X86::BuildContext& ctx){
    switch(ast->type()){
        case Parse::AST::Type::PROGRAM:{
            return visit(static_cast<Parse::ProgramAST*>(ast), ctx);
        }case Parse::AST::Type::DECL:{
            return visit(static_cast<Parse::DeclAST*>(ast), ctx);
        }case Parse::AST::Type::RETURN:{
            return visit(static_cast<Parse::ReturnAST*>(ast), ctx);
        } case Parse::AST::Type::BIN:{
            return visit(static_cast<Parse::BinOpAST*>(ast), ctx);
        } case Parse::AST::Type::VARIABLE:{
            return visit(static_cast<Parse::VariableAST*>(ast), ctx);
        } case Parse::AST::Type::LITERAL:{
            return visit(static_cast<Parse::LiteralAST*>(ast), ctx);
        }
    }
    return Optional<X86::Operand>();
}

Optional<X86::Operand> X86ASTGenerator::visit(Parse::ProgramAST* program_ast, X86::BuildContext& ctx){
    // create the main fn
    ctx.block().push(X86::Instruction::create("main:"));
    for(auto& ast : program_ast->statements()){
        visit(ast, ctx);
    }
    return Optional<X86::Operand>();
}

Optional<X86::Operand> X86ASTGenerator::visit(Parse::ReturnAST* return_ast, X86::BuildContext& ctx){
    ctx.block().push(X86::Instruction::create("ret"));
    return Optional<X86::Operand>();
}

Optional<X86::Operand> X86ASTGenerator::visit(Parse::IfAST* if_ast, X86::BuildContext& ctx){
    ctx.block().push(X86::Instruction::create("if_start:"));
    ctx.block().push(X86::Instruction::create("if_end:"));
    return Optional<X86::Operand>();
}

Optional<X86::Operand> X86ASTGenerator::visit(Parse::DeclAST* decl_ast, X86::BuildContext& ctx){
    dbg() << "generating decl!\n";

    auto stack_offset = ctx.alloc_stack();
    std::stringstream ss;
    ss << "[ebp-" << stack_offset << "]";
    auto operand = Operand::create(
        OperandEncoding::create(decl_ast->lit_type(), OperandEncoding::EncodingType::MEM),
        ss.str()
    );

    dbg() << "decl operand before = " << operand << "\n";
    
    m_sym_table.put(s(decl_ast->token().value()), operand);

/*
    if(ctx.scope()==X86::BuildContext::Scope::GLOBAL){
        //
    }else if(ctx.scope()==X86::BuildContext::Scope::FN){
        // get an offset on the stack
        auto stack_offset = ctx.alloc_stack();
    }
*/
    return Optional<X86::Operand>(operand);
}

// example of a literal ast
Optional<X86::Operand> X86ASTGenerator::visit(Parse::BinOpAST* bin_op_ast, X86::BuildContext& ctx){
    dbg() << "generating bin op!\n";
    switch(bin_op_ast->op()){
        case Parse::BinOpAST::Op::PLUS: {


            //
            // x86 encoding for add is 
            // add rax, 1
            //
            // so, we need to take the lhs and rhs operands. and lookup the instruction that uses those operandencodings with the add instr



            // 1. get the lhs and rhs Operands

            X86::Operand lhs = X86::Operand::create(
                X86::OperandEncoding::create(X86::OperandEncoding::EncodingType::REG, X86::OperandEncoding::Size::_8),
                ctx.alloc_reg(X86::OperandEncoding::Size::_8)
            );

            lhs = visit(bin_op_ast->lhs(), ctx).data();
            X86::Operand rhs = visit(bin_op_ast->rhs(), ctx).data();
           
            auto add = X86::Instruction::create("add", lhs, rhs);

            ctx.block().push(add);



            break;
        }
    }

    return Optional<X86::Operand>();
    
}

Optional<X86::Operand> X86ASTGenerator::visit(Parse::VariableAST* variable_ast, X86::BuildContext& ctx){    
    // first get the encoding
    X86::OperandEncoding encoding = X86::OperandEncoding::create(variable_ast->var_type(), OperandEncoding::EncodingType::REG);

    // then find a register to put the variable in
    X86::Register reg = ctx.alloc_reg(encoding.size());

    Operand var_location = m_sym_table.get(s(variable_ast->token().value())).data();

    X86::Operand mov_lhs = X86::Operand::create(encoding, reg);
    X86::Instruction mov = X86::Instruction::create("mov", mov_lhs, var_location);
    ctx.block().push(mov);

    // finally return the register the variable is now in
    return Optional<X86::Operand>(X86::Operand::create(encoding, reg));
}



Optional<X86::Operand> X86ASTGenerator::visit(Parse::LiteralAST* literal_ast, X86::BuildContext& ctx){    

    // so... the problem with this is that we need to know if we are within a nested operation
    // e.g. 1 + 2 + 3
    // otherwise ^ would generate
    // 
    // add al 1
    // 2   3  0
    //
    // the 2 is the first paramater and so should be stored in a register

    // first get the encoding
    auto encoding = X86::OperandEncoding::create(literal_ast->lit_type(), OperandEncoding::EncodingType::IMM);
    // then create the operand
    auto op = X86::Operand::create(encoding, std::stoi(literal_ast->token().value().data()));
    return Optional<X86::Operand>(op);
}


}