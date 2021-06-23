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
void main(){
uint32_t x;
printf(":)\n");
}
