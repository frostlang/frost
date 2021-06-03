#include <parse/lexer.h>


namespace Frost::Parse{


TokenStream& Lexer::lex(){

    dbg() << "lexing...\n";

    while(peek()!='\0'){

        char c = next();
        
        dbg() << "next char = " << c << "\n";

        switch(c){
            case '{': m_tokens.push(Token::create(TokenType::LCURLY)); break;
            case '}': m_tokens.push(Token::create(TokenType::RCURLY)); break;
            case '(': m_tokens.push(Token::create(TokenType::LPAREN)); break;
            case ')': m_tokens.push(Token::create(TokenType::RPAREN)); break;
            case ']': m_tokens.push(Token::create(TokenType::LBRACKET)); break;
            case '[': m_tokens.push(Token::create(TokenType::RBRACKET)); break;
            case '"': m_tokens.push(Token::create(TokenType::QUOTE)); break;
            case '\'': m_tokens.push(Token::create(TokenType::APOSTROPHE)); break;
            case '^': m_tokens.push(Token::create(TokenType::ARROW)); break;
            case '*': m_tokens.push(Token::create(TokenType::STAR)); break;
            case '+': {
                switch(peek()){
                    case '+': m_tokens.push(Token::create(TokenType::INCREMENT)); next(); break;
                    default: m_tokens.push(Token::create(TokenType::PLUS)); break;
                }
            }
            case '-': {
                switch(peek()){
                    case '-': m_tokens.push(Token::create(TokenType::DECREMENT)); next(); break;
                    default: m_tokens.push(Token::create(TokenType::MINUS)); break;
                }
            }
            case '/': {
                switch(peek()){
                    case '/': m_tokens.push(Token::create(TokenType::REMAINDER)); next(); break;
                    default: m_tokens.push(Token::create(TokenType::DIV)); break;
                }
            }
            case '&': {
                switch(peek()){
                    case '&': m_tokens.push(Token::create(TokenType::LAND)); next(); break;
                    default: m_tokens.push(Token::create(TokenType::AMPERSAND)); break;
                }
            }
        }

    }
    return m_tokens;
}

char Lexer::current(){
    return m_unit->source()[m_index];
}
char Lexer::peek(u32 ahead){
    return m_unit->source()[m_index+ahead];
}
char Lexer::next(){
    return m_unit->source()[m_index++];
}
}