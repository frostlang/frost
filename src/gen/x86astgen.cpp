#include <gen/x86astgen.h>

namespace Frost::Gen{


using namespace X86;


void X86ASTGenerator::gen(){
    dbg() << "gen!\n";
    BuildContext ctx = BuildContext::create();
    visit(m_ast, ctx);
    dbg() << ctx.block();
}


void X86ASTGenerator::emit(const char* instr){
    
}

Optional<Operand> X86ASTGenerator::visit(Parse::AST* ast, BuildContext& ctx){
    switch(ast->type()){
        case Parse::AST::Type::PROGRAM:{
            return visit(static_cast<Parse::ProgramAST*>(ast), ctx);
        }case Parse::AST::Type::DECL:{
            return visit(static_cast<Parse::DeclAST*>(ast), ctx);
        } case Parse::AST::Type::BIN:{
            return visit(static_cast<Parse::BinOpAST*>(ast), ctx);
        } case Parse::AST::Type::VARIABLE:{
            return visit(static_cast<Parse::VariableAST*>(ast), ctx);
        } case Parse::AST::Type::LITERAL:{
            return visit(static_cast<Parse::LiteralAST*>(ast), ctx);
        }
    }
    return Optional<Operand>();
}

Optional<Operand> X86ASTGenerator::visit(Parse::ProgramAST* program_ast, BuildContext& ctx){
    // create the main fn
    ctx.block().push(Instruction::create("main:"));
    for(auto& ast : program_ast->statements()){
        visit(ast, ctx);
    }
    return Optional<Operand>();
}

Optional<Operand> X86ASTGenerator::visit(Parse::DeclAST* decl_ast, BuildContext& ctx){

    if(ctx.scope()==BuildContext::Scope::GLOBAL){
        //
    }else if(ctx.scope()==BuildContext::Scope::FN){
        // get an offset on the stack
        auto stack_offset = ctx.alloc_stack();
    }

    return Optional<Operand>();
}

// example of a literal ast
Optional<Operand> X86ASTGenerator::visit(Parse::BinOpAST* bin_op_ast, BuildContext& ctx){
    dbg() << "generating bin op!\n";
    switch(bin_op_ast->op()){
        case Parse::BinOpAST::Op::PLUS: {


            //
            // x86 encoding for add is 
            // add rax, 1
            //
            // so, we need to take the lhs and rhs operands. and lookup the instruction that uses those operandencodings with the add instr



            // 1. get the lhs and rhs Operands

            Operand lhs = Operand::create(
                OperandEncoding::create(OperandEncoding::EncodingType::REG, OperandEncoding::Size::_8),
                ctx.alloc_reg(OperandEncoding::Size::_8)
            );

            lhs = visit(bin_op_ast->lhs(), ctx).data();
            Operand rhs = visit(bin_op_ast->rhs(), ctx).data();
           
            auto add = Instruction::create("add", lhs, rhs, Operand::create());

            ctx.block().push(add);



            break;
        }
    }

    return Optional<Operand>();
    
}

Optional<Operand> X86ASTGenerator::visit(Parse::VariableAST* variable_ast, BuildContext& ctx){    
    // first get the encoding
    OperandEncoding encoding = OperandEncoding::create(variable_ast->var_type());
    encoding.set_type(OperandEncoding::EncodingType::REG);

    // then find a register to put the variable in
    Register reg = ctx.alloc_reg(encoding.size());


    // now we move the variable into the register
    // todo this is wrong we need to find the offset of the variable on the stack...
    auto stack_ptr = ctx.stack_ptr();
    std::stringstream ss;
    ss << "[rbp-" << ctx.stack_ptr() << "]";

    Operand mov_lhs = Operand::create(encoding, reg);
    Operand mov_rhs = Operand::create(OperandEncoding::create(OperandEncoding::EncodingType::MEM, encoding.size()), ss.str());
    // create a mov instruction to put the variable in the register
    // emit("mov")
    Instruction mov = Instruction::create("mov", mov_lhs, mov_rhs, Operand::create());
    ctx.block().push(mov);

    // finally return the register the variable is now in
    return Optional<Operand>(Operand::create(encoding, reg));
}



Optional<Operand> X86ASTGenerator::visit(Parse::LiteralAST* literal_ast, BuildContext& ctx){    

    // so... the problem with this is that we need to know if we are within a nested operation
    // e.g. 1 + 2 + 3
    // otherwise ^ would generate
    // 
    // add al 1
    // 2   3  0
    //
    // the 2 is the first paramater and so should be stored in a register

    // first get the encoding
    auto encoding = OperandEncoding::create(literal_ast->lit_type());
    // then create the operand
    auto op = Operand::create(encoding, std::stoi(literal_ast->token().value().data()));
    return Optional<Operand>(op);
}


}