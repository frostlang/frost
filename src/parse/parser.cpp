#include <parse/parser.h>
#include <parse/ast.h>

namespace Frost::Parse{


AST* Parser::parse(){
    return new ProgramAST();
}


}