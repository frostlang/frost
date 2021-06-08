#include <gen/x86astgen.h>

namespace Frost::Gen{




std::vector<InstructionEncoding> instruction_lookup_table;


void register_instr(InstructionEncoding instruction_encoding){
    instruction_lookup_table.push_back(instruction_encoding);
}

InstructionEncoding lookup_instr(
    std::string name,
    OperandEncoding op0,
    OperandEncoding op1,
    OperandEncoding op2
){
    /*dbg() << "name = " <<name<<"\n";
    dbg() << "op0 = " << op0 << "\n";
    dbg() << "op1 = " << op1 << "\n";
    dbg() << "op2 = " << op2 << "\n";*/
    for(auto& instruction_encoding : instruction_lookup_table){
        /*dbg() << "checking...\n";
        dbg() << "name = " <<instruction_encoding.name() <<"\n";
        dbg() << "op0 = " << instruction_encoding.op0() << "\n";
        dbg() << "op1 = " << instruction_encoding.op1() << "\n";
        dbg() << "op2 = " << instruction_encoding.op2() << "\n";*/
        if(instruction_encoding.name()==name
        && instruction_encoding.op0()==op0
        && instruction_encoding.op1()==op1
        && instruction_encoding.op2()==op2){
            return instruction_encoding;
        }
    }
    return {};
}

void X86ASTGenerator::gen(){
    dbg() << "gen!\n";
    BuildContext ctx = BuildContext::create();
    visit(m_ast, ctx);
    dbg() << ctx.block();
}


void X86ASTGenerator::emit(const char* instr){
    
}

Register BuildContext::alloc_reg(OperandEncoding::Size size){
    return Register((Register::Type)m_used_registers++);
}


OperandEncoding X86ASTGenerator::op_encoding_from_type(Type type){
    switch(type.type()){
        case Type::Storage::U8:
        case Type::Storage::S8:{
            return OperandEncoding::create(OperandEncoding::EncodingType::IMM, OperandEncoding::Size::_8);
        }
        case Type::Storage::U16:
        case Type::Storage::S16:{
             return OperandEncoding::create(OperandEncoding::EncodingType::IMM, OperandEncoding::Size::_16);
        }
        case Type::Storage::U32:
        case Type::Storage::S32:{
             return OperandEncoding::create(OperandEncoding::EncodingType::IMM, OperandEncoding::Size::_32);
        }
    }
    return OperandEncoding::create(OperandEncoding::EncodingType::IMM, OperandEncoding::Size::ANY);
}

Optional<Operand> X86ASTGenerator::visit(Parse::AST* ast, BuildContext& ctx){
    switch(ast->type()){
        case Parse::AST::Type::PROGRAM:{
            return visit(static_cast<Parse::ProgramAST*>(ast), ctx);
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
    for(auto& ast : program_ast->statements()){
        visit(ast, ctx);
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
    OperandEncoding encoding = op_encoding_from_type(variable_ast->var_type());
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
    // first get the encoding
    auto encoding = op_encoding_from_type(literal_ast->lit_type());
    // then create the operand
    auto op = Operand::create(encoding, std::stoi(literal_ast->token().value().data()));
    return Optional<Operand>(op);
}


}