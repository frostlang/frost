#pragma once

#include <cstdint>

using u1 = bool;
using u8 = uint8_t;
using s8 = int8_t;
using u32 = uint32_t;
using s32 = int32_t;
using f32 = float;
using f64 = double;

template<typename T>
class Optional{
public:
    Optional(T data){
        m_has = true;
        m_data = data;
    }
    Optional(){
        m_has = false;
    }
    T& data(){
        return m_data;
    }
    u1 has(){
        return m_has;
    }
private:
    u1 m_has;
    T m_data;
};