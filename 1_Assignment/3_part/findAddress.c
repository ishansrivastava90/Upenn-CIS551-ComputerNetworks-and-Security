#include<stdio.h>

void main(){
 unsigned long int basePointer;
 __asm__("movq %%rbp, %0" : "=r"(basePointer));

 basePointer = basePointer - 0xa0;

 FILE * f = fopen("PasswordAddr","w");
 fprintf(f,"%p",basePointer);
 fclose(f);
}
