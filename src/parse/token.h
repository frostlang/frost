#pragma once

#include <string_view>
#include <types.h>
#include <debug.h>

namespace Frost::Parse{

enum class TokenType : u8 {
    UNKNOWN
};

class Token : public Frost::Debugable{


public:

    std::string debug() override {
        return "Token!";
    }

    std::string_view& value(){
        return m_value;
    }

private:

    std::string_view m_value;

};

}