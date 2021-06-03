#pragma once

#include <string>
#include <types.h>

namespace Frost{

/*
Represents an abstract class that implements debug()s
*/
class Debugable{
public:
virtual std::string debug() = 0;
};

class DebugStream{

public:
    static DebugStream create(){
        return {};
    }
    
    friend DebugStream& operator<<(DebugStream& ds, Debugable& debugable);
    friend DebugStream& operator<<(DebugStream& ds, const char* s);
    friend DebugStream& operator<<(DebugStream& ds, s32 s);

};

extern DebugStream default_dbg_stream;
inline DebugStream& dbg(){
    return default_dbg_stream;
}
extern DebugStream& operator<<(DebugStream& ds, Debugable& debugable);
extern DebugStream& operator<<(DebugStream& ds, const char* s);
extern DebugStream& operator<<(DebugStream& ds, s32 s);

}