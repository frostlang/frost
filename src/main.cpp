#include <debug.h>
#include <parse/token.h>

int main(){
    using namespace Frost;

    Parse::Token t;
    dbg() << "hello world!" << " ... " << t << "\n"; 

    return 0;
}