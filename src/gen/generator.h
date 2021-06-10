#pragma once

#include <parse/ast.h>

namespace Frost::Gen{

class Arch : public Debugable{
public:
    enum class Type{
        X86_64
    };
    Arch(Type type) : m_type(type){}
    constexpr u1 is_supported(){
        return m_type==Type::X86_64;
    }

    std::string debug() override {
        const char* lookup[] = {
            "X86_64"
        };
        return lookup[(u8)m_type];
    }

    Type& type(){
        return m_type;
    }
private:
    Type m_type;
};

class Platform : public Debugable{
public:
    enum class Type{
        WINDOWS,
        LINUX,
        MACOS
    };
    Platform(Type type) : m_type(type){}
    constexpr u1 is_supported(){
        return m_type==Type::WINDOWS;
    }
    std::string debug() override {
        const char* lookup[] = {
            "WINDOWS",
            "LINUX",
            "MACOS"
        };
        return lookup[(u8)m_type];
    }

    Type& type(){
        return m_type;
    }
private:
    Type m_type;
};

class Target : public Debugable{
public:
    Target(Arch arch, Platform platform) : m_arch(arch), m_platform(platform){}
    constexpr u1 is_supported(){
        return m_arch.is_supported() && m_platform.is_supported();
    }
    std::string debug() override {
        return arch().debug().append(platform().debug());    
    }
    Arch& arch(){
        return m_arch;
    }
    Platform& platform(){
        return m_platform;
    }
private:
    Arch m_arch;
    Platform m_platform;
};



// represents an abstract code generator
class Generator{

public:

};


//class ASTGenerator : public Generator, Parse::ASTVisitor{
//public:
//private:
//};

class FIRGenerator : public Generator {};

}