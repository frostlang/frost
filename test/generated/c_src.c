#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
struct type {
const char* name;
};
struct type vec_type = {"vec"};
struct vec{
uint32_t x;
uint32_t y;
};
void main(){
struct vec a;
struct vec b;
struct vec c;
a.x = 3*9999;
printf("a.x=%d\n",a.x);
printf("vec name = %s\n",vec_type.name);
}
