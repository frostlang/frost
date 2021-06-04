#pragma once

#include <vector>
#include <types.h>

namespace Frost{

enum class TypeType{
    ANY,

    U0,
    U1,
    U8,
    S8,
    U16,
    S16,
    U32,
    S32,
    F32,

    POINTER,
    STRUCT,
    INTERFACE,
    TUPLE,
    SLICE,
    FN
};

enum class AccessType{
    PUB,
    PRIV
};

enum class MutableType{
    MUT,
    CONST
};

enum class AssignableType{
    ASSIGNABLE,              // e.g. a variable 'x'
    UNASSIGNABLE             // e.g. a literal '123'
};

//
// Type::create(TypeType::U8)
// Type::create(TypeType::SLICE).set_inner(TypeType::U32)
//
//
class Type{
public:
    static Type create(TypeType type){
        Type t;
        t.m_type = type;
        t.m_access_type = AccessType::PRIV;
        t.m_mutable_type = MutableType::MUT;
        t.m_assignable_type = AssignableType::UNASSIGNABLE;
        return t;
    }
    Type& set_type(TypeType type){
        m_type = type;
        return *this;
    }
    TypeType& type(){
        return m_type;
    }
    Type& set_access(AccessType access){
        m_access_type = access;
        return *this;
    }
    AccessType& access(){
        return m_access_type;
    }
    Type& set_mut(MutableType mut){
        m_mutable_type = mut;
        return *this;
    }
    MutableType& mut(){
        return m_mutable_type;
    }
    Type& set_assignable(AssignableType assignable){
        m_assignable_type = assignable;
        return *this;
    }
    AssignableType& assignable(){
        return m_assignable_type;
    }
    Type& set_inner(std::vector<Type> inner){
        m_inner_types = inner;
        return *this;
    }
    std::vector<Type>& inner_types(){
        return m_inner_types;
    }
private:
    TypeType m_type;
    AccessType m_access_type;
    MutableType m_mutable_type;
    AssignableType m_assignable_type;
    std::vector<Type> m_inner_types;
};
}