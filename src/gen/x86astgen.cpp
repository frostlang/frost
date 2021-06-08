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

void X86ASTGenerator::gen(){}


void X86ASTGenerator::emit(const char* instr){
    
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

// example of a literal ast
void X86ASTGenerator::visit(Parse::BinOpAST* bin_op_ast){
    
    switch(bin_op_ast->op()){
        case Parse::BinOpAST::Op::PLUS: {


            //
            // x86 encoding for add is 
            // add rax, 1
            //
            // so, we need to take the lhs and rhs operands. and lookup the instruction that uses those operandencodings with the add instr



            // 1. get the lhs and rhs Operands

            Operand lhs;
            Operand rhs;
            Operand nil;

            Instruction::create("add", lhs, rhs, nil);




            break;
        }
    }
    
}

// example of a literal ast
void X86ASTGenerator::visit(Parse::LiteralAST* literal_ast){
    
    // first get the encoding
    OperandEncoding encoding = op_encoding_from_type(literal_ast->type());
    
    // then construct an Operand value
    //Operand operand = Operand::create(encoding, literal_ast->token().value());
    
}


}