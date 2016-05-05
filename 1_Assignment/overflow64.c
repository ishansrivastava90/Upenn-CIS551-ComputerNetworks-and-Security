/*
 * U. Penn CIS551 program to generate (and illustrate) 
 * buffer overflows caused by bad code and evil inputs.
 *
 * It's probably best compiled with -DSTACK so you can see what
 * the stack looks like from inside get_hex(), the buggy routine.
 *
 * Normally, get_hex() returns to the point where it is called from
 * main and the stack pointer is restored, main() prints "No problem!",
 * and exits. Life is good.
 *
 * The goal is to get the return address to be the address of problem();
 * this can be done from the keyboard with zero cleverness when
 * compiled with -DSTACK, more digging and work otherwise.
 */

#include <stdio.h>
extern void exit();
void problem()
{
  printf("Problem :-(\n" );
  exit(1);
}

void get_hex( long *buf )
{
  long l;
#ifdef STACK
  long *p=&l;
  printf("&l: 0x%lx\n", &l );
  for( l=0; l<16; l++ )
    {
      printf("p: 0x%lx, *p: 0x%lx\n", p, *p );
      ++p;
    }
#endif
  buf = &l;
  while( scanf("%lx", buf ) > 0 )
    {
          buf++;
    }
}
main()
{
  long buf = 0xFEEDDEADBEEFF00D;

#ifdef STACK
  printf("&buf: 0x%lx\n", &buf );
  printf("&get_hex(): 0x%lx\n", &get_hex );
  printf("&problem: 0x%lx\n", &problem );
  printf("&main: 0x%lx\n", &main );
#endif

  get_hex( &buf );
  printf("No problem!\n" );
  exit(0);
}

