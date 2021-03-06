#pragma once

#include <vector>
#include <string>
#include <string_view>
#include <sstream>
#include <gen/generator.h>
#include <parse/ast.h>
#include <variant>
#include <debug.h>
#include <iomanip>

namespace Frost::Gen::X86_64{



// TODO
// define a registerable target and platform system

class WindowsPlatform{
public:
    // map symbols = {"entry_point" : "_main:"}
};

class Register : public Debugable{
public:
    enum class Type : u8{
        // 8 bit general purpose registers
        AH,AL,BH,CH,CL,DH,DL,
        // 16 bit general purpose registers
        AX,BX,CX,DX,
        // 32 bit general purpose registers
        EAX,EBX,ECX,EDX,

        CS,DS,ES,FS,GS,SS,
        
        ESI,EDI,EBP,EIP,ESP,

        RBP
    };
    Register(){}
    Register(Type type){
        m_type = type;
    }
    std::string debug() override {
        const char* lookup[] = {
        // 8 bit general purpose registers
        "AH","AL","BH","CH","CL","DH","DL",
        // 16 bit general purpose registers
        "AX","BX","CX","DX",
        // 32 bit general purpose registers
        "EAX","EBX","ECX","EDX",
        "CS","DS","ES","FS","GS","SS",
        "ESI","EDI","EBP","EIP","ESP",

        "RBP"
        };
        return lookup[(int)m_type];
    }
    u1 operator==(Register other){
        return m_type==other.m_type;
    }
private:
    Type m_type;
};

class OperandEncoding : public Debugable{
public:
    enum class EncodingType : u8{
        IMM,
        REG,
        MEM
    };

    // e.g. add rax would be _64 & REG
    enum class Size : u8{
        ANY,
        _8,
        _16,
        _32,
        _64
    };
    static OperandEncoding create(){
        OperandEncoding o;
        o.m_encoding_type = EncodingType::IMM;
        o.m_size = Size::ANY;
        return o;
    }
    static OperandEncoding create(EncodingType encoding_type, Size size){
        OperandEncoding o;
        o.m_encoding_type = encoding_type;
        o.m_size = size;
        return o;
    }

    static OperandEncoding create(Frost::Type type, EncodingType encoding_type);

    std::string debug(){
        std::stringstream ss;
        const char* type_lookup[] = {"IMM", "REG", "MEM"};
        const char* size_lookup[] = {"ANY", "8", "16", "32", "64"};
        ss << "operandencoding = "<< type_lookup[(int)m_encoding_type] << " " << size_lookup[(int) m_size];
        return ss.str();
    }
    u1 operator==(OperandEncoding other){
        return m_encoding_type==other.m_encoding_type && m_size==other.m_size;
    }
    OperandEncoding& set_type(EncodingType type){
        m_encoding_type = type;
        return *this;
    }
    EncodingType& type(){
        return m_encoding_type;
    }
    Size size(){
        return m_size;
    }
private:
    EncodingType m_encoding_type;
    Size m_size;
};

class InstructionEncoding : public Debugable{
public:
    enum class Type{
        DIRECTIVE,
        OP1,
        OP2,
        OP3
    };
    static InstructionEncoding create(std::string name, u8 op, 
        OperandEncoding operand_encoding_0,
        OperandEncoding operand_encoding_1,
        OperandEncoding operand_encoding_2){
        InstructionEncoding i;
        i.m_name = name;
        i.m_op = op;
        i.m_operand_encoding[0] = operand_encoding_0;
        i.m_operand_encoding[1] = operand_encoding_1;
        i.m_operand_encoding[2] = operand_encoding_2;
        return i;
    }
    u1 operator==(InstructionEncoding other){
        return m_name==other.m_name && m_op==other.m_op &&
            m_operand_encoding[0]==other.m_operand_encoding[0]
            &&m_operand_encoding[1]==other.m_operand_encoding[1]
            &&m_operand_encoding[2]==other.m_operand_encoding[2];
    }
    std::string debug() override {
        std::stringstream ss;
        ss << "instructionencoding name = "<<m_name
        << " op0 = " << m_operand_encoding[0].debug() 
        << " op1 = " << m_operand_encoding[1].debug()
        << " op2 = " << m_operand_encoding[2].debug();
        return ss.str();
    }
    std::string& name(){
        return m_name;
    }
    Type& type(){
        return m_type;
    }
    u8& op(){
        return m_op;
    }
    OperandEncoding& op0(){
        return m_operand_encoding[0];
    }
    OperandEncoding& op1(){
        return m_operand_encoding[1];
    }
    OperandEncoding& op2(){
        return m_operand_encoding[2];
    }
private:
    std::string m_name;
    Type m_type;
    u8 m_op = {0};
    OperandEncoding m_operand_encoding[3]; // can have 3 operands
};

// todo put this in the variant instead of std::string
// e.g. 
// mov [rax], 2
// mov [rax + 4], 5
// mov [eax + ebx], 8
class MemoryLocation : public Debugable{
public:
    enum class Type{
        SINGLE,
        DOUBLE
    };
    class Value{
    public:
        enum class Type{
            IMM,
            REG
        };
        Value(){}
        Value(s32 value) : m_type(Type::IMM), m_value(value){}
        Value(Register value) : m_type(Type::REG), m_value(value){}
        Type m_type;
        std::variant<s32, Register> m_value;
    };
    MemoryLocation(){}
    MemoryLocation(Value v1){
        m_offsets[0]=v1;
        m_type=Type::SINGLE;
    }
    MemoryLocation(Value v1, Value v2){
        m_offsets[0]=v1;
        m_offsets[1]=v2;
        m_type=Type::DOUBLE;
    }
    std::string debug() override {
        std::stringstream ss;
        ss << "[";
        if(m_offsets[0].m_type==Value::Type::REG){
            ss << std::get<Register>(m_offsets[0].m_value).debug();
        }else{
            ss << std::get<s32>(m_offsets[0].m_value);
        }
        if(m_type==MemoryLocation::Type::DOUBLE){
            if(m_offsets[1].m_type==Value::Type::REG){
                ss << std::get<Register>(m_offsets[1].m_value).debug();
            }else{
                ss << std::get<s32>(m_offsets[1].m_value);
            }
        }
        ss << "]";
        return ss.str();
    }
private:
    Type m_type;
    Value m_offsets[2];
};

// this represents an x86 instruction operand
// e.g. 8, RAX, [RDI], [rbp-8] etc
class Operand : public Debugable{
public:
    static Operand create(){
        Operand o;
        o.m_encoding = OperandEncoding::create();
        o.m_value = 0;
        return o;
    }
    static Operand create(OperandEncoding encoding, std::variant<Register, s32, MemoryLocation> value){
        Operand o;
        o.m_encoding = encoding;
        o.m_value = value;
        return o;
    }
    std::string to_asm(){
        if(m_encoding.type()==OperandEncoding::EncodingType::REG){
           return std::get<Register>(m_value).debug();
        }else if(m_encoding.type()==OperandEncoding::EncodingType::IMM){
           return std::to_string(std::get<s32>(m_value));
        }else if(m_encoding.type()==OperandEncoding::EncodingType::MEM){
           return std::get<MemoryLocation>(m_value).debug();
        }
        return "";
    }
    std::string debug(){
        std::stringstream ss;
        
        if(m_encoding.type()==OperandEncoding::EncodingType::REG){
            ss << std::get<Register>(m_value).debug();
        }else if(m_encoding.type()==OperandEncoding::EncodingType::IMM){
            ss << std::get<s32>(m_value);
        }else if(m_encoding.type()==OperandEncoding::EncodingType::MEM){
            ss << std::get<MemoryLocation>(m_value).debug();
        }

        return ss.str();
    }
    OperandEncoding& encoding(){
        return m_encoding;
    }

    
private:
    OperandEncoding m_encoding;
    std::variant<Register, s32, MemoryLocation> m_value;
};




extern std::vector<InstructionEncoding> instruction_lookup_table;
extern void register_instr(InstructionEncoding instruction_encoding);

extern InstructionEncoding lookup_instr(
    std::string name,
    OperandEncoding op0,
    OperandEncoding op1,
    OperandEncoding op2
);

class Instruction : public Debugable{
public:

    enum class InstructionType{
        DIRECTIVE,   // e.g. main:, db 5 100, etc... things that are not actually in the resulting binary
        INSTRUCTION  // actual x86_64 instructions
    };

    static Instruction create(std::string directive){
        Instruction i;
        i.m_encoding=InstructionEncoding();
        i.m_encoding.name()=directive;
        i.m_encoding.type()=InstructionEncoding::Type::DIRECTIVE;
        return i;
    }
    
    static Instruction create(
        std::string name, 
        Operand op0,
        Operand op1
        ){
        Instruction i;
        i.m_encoding = lookup_instr(name, op0.encoding(), op1.encoding(), OperandEncoding::create());
        i.m_encoding.type()=InstructionEncoding::Type::OP2;
        i.m_op0 = op0;
        i.m_op1 = op1;
        return i;
    }
    static Instruction create(
        std::string name, 
        Operand op0,
        Operand op1,
        Operand op2
        ){
        Instruction i;
        i.m_encoding = lookup_instr(name, op0.encoding(), op1.encoding(), op2.encoding());
        i.m_encoding.type()=InstructionEncoding::Type::OP3;
        i.m_op0 = op0;
        i.m_op1 = op1;
        i.m_op2 = op2;
        return i;
    }
    std::string debug() override{
        return to_asm();
    }
    std::string to_asm(){
        std::stringstream ss;
        ss << std::setfill('0') << std::setw(3) << (int)m_encoding.op()<<":";
        switch(m_encoding.type()){
            case InstructionEncoding::Type::DIRECTIVE:
                ss << m_encoding.name(); break;
            case InstructionEncoding::Type::OP1: 
                ss << m_encoding.name() << " " << m_op0.to_asm(); break;
            case InstructionEncoding::Type::OP2: 
                ss << m_encoding.name() << " " << m_op0.to_asm() << ", " << m_op1.to_asm(); break;
            case InstructionEncoding::Type::OP3: 
                ss << m_encoding.name() << " " << m_op0.to_asm() << ", " << m_op1.to_asm() << ", " << m_op2.to_asm(); break;
        }
        return ss.str();
    }
private:
    Parse::Position m_position;
    InstructionEncoding m_encoding;
    Operand m_op0;
    Operand m_op1;
    Operand m_op2;
};

class Block : public Debugable{
public:
    Block(){}
    Block& push(Instruction instruction){
        m_instructions.push_back(instruction);
        return *this;
    }
    std::string debug(){
        std::stringstream ss;
        ss << "block\n";
        for(auto& instruction : m_instructions){
            ss << instruction.debug() << "\n";
        }
        return ss.str();
    }
private:
    std::vector<Instruction> m_instructions;
};

class BuildContext{
public:
    enum class Scope{
        GLOBAL,
        FN
    };
    enum class ValueType{
        VALUE,
        LOCATION
    };

    static BuildContext create(){
        BuildContext b;
        b.m_scope = Scope::GLOBAL;
        b.m_stack_ptr=0;
        return b;
    }
    Scope& scope(){
        return m_scope;
    }
    Block& block(){
        return m_block;
    }
    s32& stack_ptr(){
        return m_stack_ptr;
    }
    s32 alloc_stack(){
        m_stack_ptr-=4;
        return m_stack_ptr;
    }
    ValueType& value_type(){
        return m_value_type;
    }
    Register alloc_reg(OperandEncoding::Size encoding_type);
private:
    Scope m_scope;
    u8 m_used_registers = 0;
    Block m_block;
    s32 m_stack_ptr = {0};
    ValueType m_value_type;
};


}