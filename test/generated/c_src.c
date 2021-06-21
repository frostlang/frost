#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
void test_mangled_identifier(){
}
void main(){
}
void(*x)()  = &test_mangled_identifier;
