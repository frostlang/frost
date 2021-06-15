#include <debug.h>

#include <iostream>

namespace Frost{

DebugStream default_dbg_stream;
DebugStream default_info_stream;

DebugStream& operator<<(DebugStream& ds, Debugable& debugable){

    std::cout << debugable.debug();
    return ds;

}

DebugStream& operator<<(DebugStream& ds, Debugable&& debugable){

    std::cout << debugable.debug();
    return ds;

}

DebugStream& operator<<(DebugStream& ds, Debugable* debugable){

    std::cout << debugable->debug();
    return ds;

}

DebugStream& operator<<(DebugStream& ds, const char* s){
    std::cout << s;
    return ds;
}

DebugStream& operator<<(DebugStream& ds, std::string s){
    std::cout << s;
    return ds;
}

DebugStream& operator<<(DebugStream& ds, s32 s){
    std::cout << s;
    return ds;
}

DebugStream& operator<<(DebugStream& ds, char s){
    std::cout << s;
    return ds;
}

DebugStream& operator<<(DebugStream& ds, u1 s){
    std::cout << s;
    return ds;
}

}