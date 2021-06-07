#include <vector>
#include <string>
#include <string_view>
#include <gen/generator.h>
#include <gen/frostir.h>
#include <parse/ast.h>

namespace Frost::Gen{

using namespace Parse;

class Register{
    enum class Type{
        EAX,
        EBX,
    };
};

class OperandEncoding{
public:
    enum class EncodingType{
        IMM,
        REG,
        MEM
    };

    // e.g. add rax would be _64 & REG
    enum class Size{
        ANY,
        _8,
        _16,
        _32,
        _64
    };
    static OperandEncoding create(){
        OperandEncoding o;
        return o;
    }
    static OperandEncoding create(EncodingType encoding_type, Size size){
        OperandEncoding o;
        o.m_encoding_type = encoding_type;
        o.m_size = size;
        return o;
    }
private:
    EncodingType m_encoding_type;
    Size m_size;
};

class InstructionEncoding{
public:
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
private:
    std::string m_name;
    u8 m_op;
    OperandEncoding m_operand_encoding[3]; // can have 3 operands
};

// this represents an x86 instruction operand
// e.g. 8, RAX, [RDI] etc
class Operand{
public:
    static Operand create(OperandEncoding encoding, std::string_view value){
        Operand o;
        o.m_encoding = encoding;
        o.m_value = value;
        return o;
    }
    OperandEncoding& encoding(){
        return m_encoding;
    }
    std::string_view& value(){
        return m_value;
    }
private:
    OperandEncoding m_encoding;
    std::string_view m_value;
};

extern InstructionEncoding lookup_instr(
    std::string name,
    OperandEncoding op0,
    OperandEncoding op1,
    OperandEncoding op2
);


class Instruction{
public:
    static Instruction create(
        std::string name, 
        Operand op0,
        Operand op1,
        Operand op2
        ){
        Instruction i;
        i.m_encoding = lookup_instr(name, op0.encoding(), op1.encoding(), op2.encoding());
        i.m_op0 = op0;
        i.m_op1 = op1;
        i.m_op2 = op2;
        return i;
    }
private:
    Position m_position;
    InstructionEncoding m_encoding;
    Operand m_op0;
    Operand m_op1;
    Operand m_op2;
};



//
// NOTE this class takes the AST and generates 
//
class X86ASTGenerator{// : ASTGenerator{

public:
    static X86ASTGenerator create(Parse::AST* ast){
        X86ASTGenerator x;
        x.m_ast = ast;

        // this defines the add instruction that takes the form add reg8 imm8 (adds 8 bit immediate to 8 bit register)
        Gen::InstructionEncoding::create("add", 04,
            Gen::OperandEncoding::create(Gen::OperandEncoding::EncodingType::REG, Gen::OperandEncoding::Size::_8),
            Gen::OperandEncoding::create(Gen::OperandEncoding::EncodingType::IMM, Gen::OperandEncoding::Size::_8),
            Gen::OperandEncoding::create()
        );
        // 16 & 32 bit add use the same op code
        Gen::InstructionEncoding::create("add", 05,
            Gen::OperandEncoding::create(Gen::OperandEncoding::EncodingType::REG, Gen::OperandEncoding::Size::_16),
            Gen::OperandEncoding::create(Gen::OperandEncoding::EncodingType::IMM, Gen::OperandEncoding::Size::_16),
            Gen::OperandEncoding::create()
        );
        Gen::InstructionEncoding::create("add", 05,
            Gen::OperandEncoding::create(Gen::OperandEncoding::EncodingType::REG, Gen::OperandEncoding::Size::_32),
            Gen::OperandEncoding::create(Gen::OperandEncoding::EncodingType::IMM, Gen::OperandEncoding::Size::_32),
            Gen::OperandEncoding::create()
        );

        // add imm 8 to register or memory
        Gen::InstructionEncoding::create("add", 80,
            Gen::OperandEncoding::create(Gen::OperandEncoding::EncodingType::REG, Gen::OperandEncoding::Size::_32),
            Gen::OperandEncoding::create(Gen::OperandEncoding::EncodingType::IMM, Gen::OperandEncoding::Size::_8),
            Gen::OperandEncoding::create()
        );
        Gen::InstructionEncoding::create("add", 80,
            Gen::OperandEncoding::create(Gen::OperandEncoding::EncodingType::MEM, Gen::OperandEncoding::Size::_32),
            Gen::OperandEncoding::create(Gen::OperandEncoding::EncodingType::IMM, Gen::OperandEncoding::Size::_8),
            Gen::OperandEncoding::create()
        );

        return x;
    }
    
    void gen();

    void emit(const char*);
    
    OperandEncoding op_encoding_from_type(Type type);

    void visit(Parse::BinOpAST* bin_op_ast);
    void visit(Parse::LiteralAST* literal_ast);

private:
    Parse::AST* m_ast;
};
}