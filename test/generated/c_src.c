#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
uint8_t x;
void test_mangled_identifier(){
printf("hello world!\n");
}
void main(){
void(*x)()  = &test_mangled_identifier;
test_mangled_identifier();
}
