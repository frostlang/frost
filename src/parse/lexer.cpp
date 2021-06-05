#include <parse/lexer.h>
#include <cstring>


namespace Frost::Parse{

u1 is_num(char c){
    return c>='0' && c <= '9';
}

u1 is_alpha(char c){
    return c>='a' && c <= 'z';
}

void Lexer::skip_whitespace(){
    char c;
    while(true){
        c = peek();
        if(c==' '||c=='\t'||c=='\n'||c=='\r')
            next();
        else
            return;
    }
}

TokenStream& Lexer::lex(){

    dbg() << "lexing...\n";

    while(peek()!='\0'){

        skip_whitespace();
        char c = peek();

        switch(c){
            case '{': m_tokens.push(Token::create(TokenType::LCURLY)); next(); break;
            case '}': m_tokens.push(Token::create(TokenType::RCURLY)); next(); break;
            case '(': m_tokens.push(Token::create(TokenType::LPAREN)); next(); break;
            case ')': m_tokens.push(Token::create(TokenType::RPAREN)); next(); break;
            case ']': m_tokens.push(Token::create(TokenType::LBRACKET)); next(); break;
            case '[': m_tokens.push(Token::create(TokenType::RBRACKET)); next(); break;
            case '"': m_tokens.push(Token::create(TokenType::QUOTE)); next(); break;
            case '\'': m_tokens.push(Token::create(TokenType::APOSTROPHE)); next(); break;
            case '^': m_tokens.push(Token::create(TokenType::ARROW)); next(); break;
            case '*': m_tokens.push(Token::create(TokenType::STAR)); next(); break;
            case ';': m_tokens.push(Token::create(TokenType::SEMICOLON)); next(); break;
            case '+': {
                switch(peek(1)){
                    case '+': m_tokens.push(Token::create(TokenType::INCREMENT)); next(); break;
                    default: m_tokens.push(Token::create(TokenType::PLUS)); break;
                }
                next(); 
                break;
            }
            case '-': {
                switch(peek(1)){
                    case '-': m_tokens.push(Token::create(TokenType::DECREMENT)); next(); break;
                    default: m_tokens.push(Token::create(TokenType::MINUS)); break;
                }
                next(); 
                break;
            }
            case '/': {
                switch(peek(1)){
                    case '/': m_tokens.push(Token::create(TokenType::REMAINDER)); next(); break;
                    default: m_tokens.push(Token::create(TokenType::DIV)); break;
                }
                next(); 
                break;
            }
            case '&': {
                switch(peek()){
                    case '&': m_tokens.push(Token::create(TokenType::LAND)); next(); break;
                    default: m_tokens.push(Token::create(TokenType::AMPERSAND)); break;
                }
                next(); 
                break;
            }
            default: {
                if(is_num(c)){
                    number();
                    break;
                }else if(is_alpha(c)){
                    alpha();
                    break;
                }
            }
        }

    }
    return m_tokens;
}

void Lexer::number(){
    u32 start = m_index;
    u32 offset = 1;
    char c = next();
    while(is_num(peek())){
        next();
        offset++;
    }
    auto identifier_value = std::string_view(m_unit->source()).substr(start, offset);
    m_tokens.push(Token::create(TokenType::NUMBER, identifier_value));
}

u1 Lexer::match(const char* keyword){
    for(u32 i=0;i<strlen(keyword);i++){
        //dbg() << "checking " << peek(i) << " against " << keyword[i] << "\n";
        if(peek(i)!=keyword[i])
            return false;
    }
    return true;
}

void Lexer::alpha(){

    // first check for keywords
    switch(peek()){
        case 'a':{
            if(match("and")){
                m_tokens.push(Token::create(TokenType::LAND)); next(strlen("and")); return;
            }else if(match("any")){
                m_tokens.push(Token::create(TokenType::ANY)); next(strlen("any")); return;
            }
        }
        case 'b':{
            if(match("break")){
                m_tokens.push(Token::create(TokenType::BREAK)); next(strlen("break")); return;
            }
            break;
        }
        case 'c':{
            if(match("continue")){
                m_tokens.push(Token::create(TokenType::BREAK)); next(strlen("continue")); return;
            }
            break;
        }
        case 'f':{
            if(match("fn")){
                m_tokens.push(Token::create(TokenType::FN)); next(strlen("fn")); return;
            }else if(match("f32")){
                m_tokens.push(Token::create(TokenType::F32)); next(strlen("f32")); return;
            }else if(match("f64")){
                m_tokens.push(Token::create(TokenType::F32)); next(strlen("f64")); return;
            }else if(match("for")) {
                m_tokens.push(Token::create(TokenType::FOR)); next(strlen("for")); return;
            }else if(match("false")) {
                m_tokens.push(Token::create(TokenType::FALSE)); next(strlen("false")); return;
            }
            break;
        }
        case 'i':{
            if(match("interface")){
                m_tokens.push(Token::create(TokenType::INTERFACE)); next(strlen("interface")); return;
            }
        }
        case 'm':{
            if(match("match")){
                m_tokens.push(Token::create(TokenType::MATCH)); next(strlen("match")); return;
            }
            break;
        }
        case 'o':{
            if(match("or")){
                m_tokens.push(Token::create(TokenType::LOR)); next(strlen("or")); return;
            }
        }
        case 's':{
            if(match("struct")){
                m_tokens.push(Token::create(TokenType::STRUCT)); next(strlen("struct")); return;
            }else if(match("s8")){
                m_tokens.push(Token::create(TokenType::S8)); next(strlen("s8")); return;
            }else if(match("s16")){
                m_tokens.push(Token::create(TokenType::S16)); next(strlen("s16")); return;
            }else if(match("s32")){
                m_tokens.push(Token::create(TokenType::S32)); next(strlen("s32")); return;
            }
            break;
        }
        case 't':{
            if(match("true")){
                m_tokens.push(Token::create(TokenType::TRUE)); next(strlen("true")); return;
            }
            break;
        }
        case 'u':{
            if(match("u0")){
                m_tokens.push(Token::create(TokenType::U0)); next(strlen("u0")); return;
            }else if(match("u8")){
                m_tokens.push(Token::create(TokenType::U8)); next(strlen("u8")); return;
            }else if(match("u16")){
                m_tokens.push(Token::create(TokenType::U16)); next(strlen("u16")); return;
            }else if(match("u32")){
                m_tokens.push(Token::create(TokenType::U32)); next(strlen("u32")); return;
            }
            break;
        }
    }

    u32 start = m_index;
    u32 offset = 1;
    char c = next();
    while(is_alpha(peek())){
        next();
        offset++;
    }
    auto identifier_value = std::string_view(m_unit->source()).substr(start, offset);
    m_tokens.push(Token::create(TokenType::IDENTIFIER, identifier_value));
}

char Lexer::current(){
    return m_unit->source()[m_index];
}
char Lexer::peek(u32 ahead){
    return m_unit->source()[m_index+ahead];
}
char Lexer::next(u32 ahead){
    char c = m_unit->source()[m_index];
    m_index+=ahead;
    return c;
}
}