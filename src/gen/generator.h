#pragma once

#include <parse/ast.h>

namespace Frost::Gen{

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