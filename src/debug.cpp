#include <debug.h>

#include <iostream>

namespace Frost{

DebugStream default_dbg_stream;

DebugStream& operator<<(DebugStream& ds, Debugable& debugable){

    std::cout << debugable.debug();
    return ds;

}

DebugStream& operator<<(DebugStream& ds, const char* s){
    std::cout << s;
    return ds;
}

DebugStream& operator<<(DebugStream& ds, s32 s){
    std::cout << s;
    return ds;
}

}