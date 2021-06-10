#include <gen/x86_64.h>

namespace Frost::Gen::X86_64{

std::vector<InstructionEncoding> instruction_lookup_table = {
    #define INSTR(name, op, op0_type, op0_size, op1_type, op1_size) \
            InstructionEncoding::create(name, op, \
                OperandEncoding::create(OperandEncoding::EncodingType::op0_type, OperandEncoding::Size::op0_size), \
                OperandEncoding::create(OperandEncoding::EncodingType::op1_type, OperandEncoding::Size::op1_size), \
                OperandEncoding::create() \
            )


        INSTR("add", 0, MEM, _8, REG, _8),
        INSTR("add", 0, REG, _8, REG, _8),
        INSTR("add", 1, MEM, _16, REG, _16),
        INSTR("add", 1, REG, _16, REG, _16),
        INSTR("add", 4, REG, _8, IMM, _8),
        INSTR("add", 5, REG, _16, IMM, _16),
        INSTR("add", 5, REG, _32, IMM, _32),
        INSTR("mov", 88, REG, _8, MEM, _8),
        INSTR("mov", 88, REG, _8, MEM, _16),
        INSTR("mov", 88, REG, _8, MEM, _32),
        INSTR("mov", 0xC6, MEM, _8, IMM, _8)
};

void register_instruction(InstructionEncoding instruction_encoding){
    instruction_lookup_table.push_back(instruction_encoding);
}

Register BuildContext::alloc_reg(OperandEncoding::Size size){
    return Register((Register::Type)m_used_registers++);
}

OperandEncoding OperandEncoding::create(Frost::Type type, OperandEncoding::EncodingType encoding_type){
    switch(type.type()){
        case Type::Storage::U8:
        case Type::Storage::S8:{
            return OperandEncoding::create(encoding_type, OperandEncoding::Size::_8);
        }
        case Type::Storage::U16:
        case Type::Storage::S16:{
             return OperandEncoding::create(encoding_type, OperandEncoding::Size::_16);
        }
        case Type::Storage::U32:
        case Type::Storage::S32:{
             return OperandEncoding::create(encoding_type, OperandEncoding::Size::_32);
        }
    }
    return OperandEncoding::create(encoding_type, OperandEncoding::Size::ANY);
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
}