#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
struct type{
const char* identifier;
uint32_t size;
};
struct slice{
void* data;
uint32_t size;
struct type type;
};
int32_t x = 99;
struct vec{
uint32_t x;
uint32_t y;
uint32_t z;
};
void main(){
x = 1+3*3;
printf("x=%d\n",x);
struct vec v;
v.x = 99;
printf("v.x=%d\n",v.x);
struct slice s;
}
