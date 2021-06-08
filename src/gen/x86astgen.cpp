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
    dbg() << "name = " <<name<<"\n";
    dbg() << "op0 = " << op0 << "\n";
    dbg() << "op1 = " << op1 << "\n";
    dbg() << "op2 = " << op2 << "\n";
    for(auto& instruction_encoding : instruction_lookup_table){
        dbg() << "checking...\n";
        dbg() << "name = " <<instruction_encoding.name() <<"\n";
        dbg() << "op0 = " << instruction_encoding.op0() << "\n";
        dbg() << "op1 = " << instruction_encoding.op1() << "\n";
        dbg() << "op2 = " << instruction_encoding.op2() << "\n";
        if(instruction_encoding.name()==name
        && instruction_encoding.op0()==op0
        && instruction_encoding.op1()==op1
        && instruction_encoding.op2()==op2){
            dbg() << "found instruction! " << name << "\n";
            return instruction_encoding;
        }
    }
    return {};
}

void X86ASTGenerator::gen(){
    dbg() << "gen!\n";
    visit(m_ast, BuildContext::create());
}


void X86ASTGenerator::emit(const char* instr){
    
}


Register X86ASTGenerator::alloc_reg(BuildContext ctx){
    return Register(Register::Type::AH);
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

Optional<Operand> X86ASTGenerator::visit(Parse::AST* ast, BuildContext ctx){
    switch(ast->type()){
        case Parse::AST::Type::PROGRAM:{
            return visit(static_cast<Parse::ProgramAST*>(ast), ctx);
        } case Parse::AST::Type::BIN:{
            return visit(static_cast<Parse::BinOpAST*>(ast), ctx);
        } case Parse::AST::Type::LITERAL:{
            return visit(static_cast<Parse::LiteralAST*>(ast), ctx);
        }
    }
    return Optional<Operand>();
}

Optional<Operand> X86ASTGenerator::visit(Parse::ProgramAST* program_ast, BuildContext ctx){
    for(auto& ast : program_ast->statements()){
        visit(ast, ctx);
    }
    return Optional<Operand>();
}

// example of a literal ast
Optional<Operand> X86ASTGenerator::visit(Parse::BinOpAST* bin_op_ast, BuildContext ctx){
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
                Register(Register::Type::AH)
            );


            Operand rhs = visit(bin_op_ast->rhs(), ctx).data();

           
            auto instruction = Instruction::create("add", lhs, rhs, Operand::create());

            dbg() << "instruction = " << instruction.to_asm() << "\n";



            break;
        }
    }

    return Optional<Operand>();
    
}

// example of a literal ast
Optional<Operand> X86ASTGenerator::visit(Parse::LiteralAST* literal_ast, BuildContext ctx){    
    // first get the encoding
    auto encoding = op_encoding_from_type(literal_ast->lit_type());
    // then create the operand
    auto op = Operand::create(encoding, std::stoi(literal_ast->token().value().data()));
    return Optional<Operand>(op);
}


}