#include <parse/lexer.h>


namespace Frost::Parse{


TokenStream& Lexer::lex(){

    m_tokens.push(Token::create());

    return m_tokens;
}

}