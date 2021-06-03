#pragma once

#include <debug.h>

#define ASSERT(expression)                                                \
    if(!expression){                                                      \
        dbg() << "ASSERTION FAILED: file="<<__FILE__<<" line="<<__LINE__; \
        exit(0);                                                          \
    }
