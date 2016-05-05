clean:
	rm -rf a.out
	rm -rf findAddress
	rm -rf badbuf
	rm -rf PasswordAddr
	clear

attack:
	gcc -o findAddress findAddress.c
	gcc -fno-stack-protector -o badbuf badbuf.c -z execstack
