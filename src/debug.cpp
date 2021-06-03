#include <debug.h>

#include <iostream>

namespace Frost{

DebugStream default_dbg_stream;


DebugStream& operator<<(DebugStream& ds, const char* s){
    std::cout << s;
    return ds;
}

}