#include <vector>
#include <string>
#include <string_view>
#include <sstream>
#include <gen/generator.h>
#include <gen/frostir.h>
#include <parse/ast.h>
#include <variant>
#include <debug.h>

namespace Frost::Gen{

using namespace Parse;

class Register : public Debugable{
public:
    enum class Type{
        // 8 bit general purpose registers
        AH,
        AL,
        BH,
        CH,
        CL,
        DH,
        DL,

        // 16 bit general purpose registers
        AX,
        BX,
        CX,
        DX,

        // 32 bit general purpose registers
        EAX,
        EBX,
        ECX,
        EDX,

        CS,
        DS,
        ES,
        FS,
        GS,
        SS,

        ESI,
        EDI,
        EBP,
        EIP,
        ESP
    };
    Register(){}
    Register(Type type){
        m_type = type;
    }
    std::string debug() override {
        const char* lookup[] = {
        // 8 bit general purpose registers
        "AH","AL","BH","BH","CH","CL","DH","DL",
        // 16 bit general purpose registers
        "AX","BX","CX","DX",
        // 32 bit general purpose registers
        "EAX","EBX","ECX","EDX",
        "CS","DS","ES","FS","GS","SS",
        "ESI","EDI","EBP","EIP","ESP"
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
    u8 m_op;
    OperandEncoding m_operand_encoding[3]; // can have 3 operands
};

// this represents an x86 instruction operand
// e.g. 8, RAX, [RDI] etc
class Operand : public Debugable{
public:
    static Operand create(){
        Operand o;
        o.m_encoding = OperandEncoding::create();
        o.m_value = 0;
        return o;
    }
    static Operand create(OperandEncoding encoding, std::variant<Register, s32> value){
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
        }
        return "";
    }
    std::string debug(){
        std::stringstream ss;
        
        if(m_encoding.type()==OperandEncoding::EncodingType::REG){
            ss << std::get<Register>(m_value).debug();
        }else if(m_encoding.type()==OperandEncoding::EncodingType::IMM){
            ss << std::get<s32>(m_value);
        }

        return ss.str();
    }
    OperandEncoding& encoding(){
        return m_encoding;
    }
private:
    OperandEncoding m_encoding;
    std::variant<Register, s32> m_value;
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
    std::string debug() override{
        std::stringstream ss;
        ss << "instruction : " << (u32)m_encoding.op() << " : " << m_encoding.name() << " " << m_op0.debug() << " " << m_op1.debug() << " " << m_op2.debug();
        return ss.str();
    }
    std::string to_asm(){
        std::stringstream ss;
        ss << m_encoding.name() << " " << m_op0.to_asm() << " " << m_op1.to_asm() << " " << m_op2.to_asm();
        return ss.str();
    }
private:
    Position m_position;
    InstructionEncoding m_encoding;
    Operand m_op0;
    Operand m_op1;
    Operand m_op2;
};



class BuildContext{
public:
    static BuildContext create(){
        BuildContext b;
        return b;
    }
private:
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
        register_instr(Gen::InstructionEncoding::create("add", 4,
            Gen::OperandEncoding::create(Gen::OperandEncoding::EncodingType::REG, Gen::OperandEncoding::Size::_8),
            Gen::OperandEncoding::create(Gen::OperandEncoding::EncodingType::IMM, Gen::OperandEncoding::Size::_8),
            Gen::OperandEncoding::create()
        ));
        // 16 & 32 bit add use the same op code
        register_instr(Gen::InstructionEncoding::create("add", 5,
            Gen::OperandEncoding::create(Gen::OperandEncoding::EncodingType::REG, Gen::OperandEncoding::Size::_16),
            Gen::OperandEncoding::create(Gen::OperandEncoding::EncodingType::IMM, Gen::OperandEncoding::Size::_16),
            Gen::OperandEncoding::create()
        ));
        register_instr(Gen::InstructionEncoding::create("add", 5,
            Gen::OperandEncoding::create(Gen::OperandEncoding::EncodingType::REG, Gen::OperandEncoding::Size::_32),
            Gen::OperandEncoding::create(Gen::OperandEncoding::EncodingType::IMM, Gen::OperandEncoding::Size::_32),
            Gen::OperandEncoding::create()
        ));

        // add imm 8 to register or memory
        register_instr(Gen::InstructionEncoding::create("add", 80,
            Gen::OperandEncoding::create(Gen::OperandEncoding::EncodingType::REG, Gen::OperandEncoding::Size::_32),
            Gen::OperandEncoding::create(Gen::OperandEncoding::EncodingType::IMM, Gen::OperandEncoding::Size::_8),
            Gen::OperandEncoding::create()
        ));
        register_instr(Gen::InstructionEncoding::create("add", 80,
            Gen::OperandEncoding::create(Gen::OperandEncoding::EncodingType::MEM, Gen::OperandEncoding::Size::_32),
            Gen::OperandEncoding::create(Gen::OperandEncoding::EncodingType::IMM, Gen::OperandEncoding::Size::_8),
            Gen::OperandEncoding::create()
        ));

        return x;
    }
    
    void gen();

    void emit(const char*);
    
    OperandEncoding op_encoding_from_type(Type type);
    Register alloc_reg(BuildContext);

    Optional<Operand> visit(Parse::AST*, BuildContext);
    Optional<Operand> visit(Parse::ProgramAST*, BuildContext);
    Optional<Operand> visit(Parse::BinOpAST*, BuildContext);
    Optional<Operand> visit(Parse::LiteralAST*, BuildContext);

private:
    Parse::AST* m_ast;
};
}