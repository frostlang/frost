#include <gen/frostir.h>
#include <gen/generator.h>
#include <parse/ast.h>

namespace Frost::Gen{


class FrostIRGenerator : public Generator {

public:
    static FrostIRGenerator create(Parse::AST* ast){
        FrostIRGenerator f;
        f.m_ast = ast;
        return f;
    }
private:
    Parse::AST* m_ast;
};

}