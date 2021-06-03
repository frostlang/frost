#include <parse/lexer.h>


namespace Frost::Parse{


TokenStream& Lexer::lex(){
    return m_tokens;
}

}