#include <vector>
#include <string>
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

class Instruction{
public:
    static Instruction create(std::string name, u8 op, 
        OperandEncoding operand_encoding_0,
        OperandEncoding operand_encoding_1,
        OperandEncoding operand_encoding_2){
        Instruction i;
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




//
// NOTE this class takes the AST and generates 
//
class X86ASTGenerator{// : ASTGenerator{

public:
    static X86ASTGenerator create(Parse::AST* ast){
        X86ASTGenerator x;
        x.m_ast = ast;
        return x;
    }
    
    void gen();

    void emit(const char*);

private:
    Parse::AST* m_ast;
};

}