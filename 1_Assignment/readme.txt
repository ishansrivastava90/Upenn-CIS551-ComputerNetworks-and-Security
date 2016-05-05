Readme

This is a example of buffer over flow attack. I changed the array size to 12 to simplify since the concept remains the same. On viewing the stack using GDB, it was seen that 16 bytes were allocated for password and then 16 bytes for the name. After the name, the pointer to evil string was stored. Since 16 bytes were allocated for the name, I took a 16 byte login and appended it with the address of the good string in place of the evil string. Thus, the stack got overwritten and in place of evil string pointer , pointer for good string was placed. Hence, even when the name and password didn't match, it printed "Welcome to the machine"

Login used - iiiiiiiiiiiiiiii\xe4\x07\x40\x00

