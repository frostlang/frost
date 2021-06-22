#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
uint8_t x;
void lambda_0(){
printf("hello y\n");
}
void lambda_1(){
printf("hello z\n");
}
void main(){
void(*y)()  = &lambda_0;
void(*z)()  = &lambda_1;
y();
}
