#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
typedef struct {
uint32_t x;
uint32_t y;
} vec;
void test(){
printf("hello world!\n");
}
void main(){
uint32_t x = 1;
x = 99;
x = x*43;
printf("x = %d",x);
}
