Assignment Part 2

Task - No matter what the login be, print Ownz_U! on the screen.

I have found the stack base pointer address using inline assembly code and then found the offset for the password using gdb. 
I then subtracted the address the offset to get the address of password each time. 

I overflow the name stack space and populate the pwd stack address at the place of evil stack address and then push Ownz_U! at the pwd stack address. 
Thus, after login and password, evil address is invoked but that contains the address of pwd and that contains Ownz_U! which is printed on the screen.

Steps to run the attack

1) make clean
2) make attack
3) ./findAddress - This command finds the stack base address and subtracts the offset from it and writes the addr of pwd in a file named "PasswordAddr"
4) ./badbuf < PasswordAddr

REFERENCES

StackOverflow
Wiki

