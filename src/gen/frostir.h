#pragma once

#include <vector>
#include <types.h>

namespace Frost::Gen{

//
// intermediate code representation
//
// each instruction is 3 bytes (opcode, param1, param2)
//
// NOP
// ADD p1 p2
//
#pragma pack(1)
struct FrostIR {

    enum class Type : u32{
        NOP,

        ADD,
        SUB,
        MUL,
        DIV,
        MOD,
        REM,

        GOTO
    };

    Type m_type;
    u32 m_param1;
    u32 m_param2;    

};

}