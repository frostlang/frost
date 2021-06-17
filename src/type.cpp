#include <type.h>

namespace Frost{

const char* type_debug[] = {
    "UNKNOWN",

    "ANY",
    
    "U0",
    "U1",
    "U8",
    "S8",
    "U16",
    "S16",
    "U32",
    "S32",
    "F32",

    "POINTER",
    "STRUCT",
    "INTERFACE",
    "TUPLE",
    "SLICE",
    "ARRAY",
    "FN"
};

}