#include "demo.h"

/*
 * lookup -
 * returns the "value" part of a <name, value>
 * pair when given "name"; NULL if not found.
 */

char *
lookup( name )
char *name;
{
  struct sym_list *sp;

  sp = s_lookup( name );
  if( sp != (struct sym_list *) NULL )
    return( sp->s_sym.v_value );
  else
    return( (char *) NULL );
}

/*
 * insert()
 * inserts a new <name,value> pair into the associative memory,
 * which is maintained as a linked list pointed to by "Head".
 */

insert( name, value )
char *name, *value;
{
  struct sym_list *sp;

  sp = s_lookup( name );

  /* if it's already in the list, we won't
   * change the list structure. Otherwise,
   * we'll insert a new element at the beginning
   * of the list. 
   */

  if( sp == (struct sym_list * ) NULL )
    {
      sp = new_sym();
      if( sp == (struct sym_list *) NULL )
	{
	  printf( "Fatal error: no memory\n" );
	  exit( 1 );
	}
	
      sp->s_next = Head.s_next;
      Head.s_next = sp;
    }

  /* one way or another, "sp" now points to a valid entry. */

  sp->s_sym.v_name = strsave( name );
  sp->s_sym.v_value = strsave( value );
	
}

/*
 * returns a pointer to a copy of the 
 * character string pointed to by "str".
 */

char *
strsave( str )
char *str;
{
	char *p;

	p = (char *) malloc( strlen(str)+1 );
	if( p != (char *) NULL )
		strcpy( p, str );
	return( p );
}

/*
 * provides a pointer to a new sym_list structure
 */
 
struct sym_list *
new_sym()
{
  struct sym_list *p;

  p = (struct sym_list *) malloc( sizeof( struct sym_list ) );

  /* on failure, malloc() returns NULL,
   * which is our error return as well.
   */
  return( p );
	
}

/*
 * s_lookup:
 * finds a symlist structure on the list pointed to by
 * Head which has a v_name entry matching name.
 */

struct sym_list *
s_lookup( name )
char *name;
{
  struct sym_list *sp;


  for( sp = Head.s_next; sp != (struct sym_list *) NULL; sp = sp->s_next)
    {
      if( strcmp( sp->s_sym.v_name, name ) == 0 )
	return( sp );
    }

  return( (struct sym_list *) NULL );
}

/* save() -
 *  saves the set of main memory <name, value> pairs
 *  to the disk file named in the character string "file".
 *
 */

save( file )
char *file;
{
  FILE *filep;
  struct sym_list *sp;

  filep = fopen( file, "w" );

  if( filep == (FILE *) NULL )
    {
      fputs( "Fatal error. Cannot Save", stderr );
      exit( 1 );
    }

  for( sp = Head.s_next;
       sp != (struct sym_list *) NULL;
       sp = sp->s_next )
    {
      fprintf( filep, "%s%c%s",
	       sp->s_sym.v_name,
	       EQUALS,
	       sp->s_sym.v_value );
    }

  fclose( filep );
}

/*
 *
 * restore()
 *  - restores the associative memory with the contents
 *    of the data file named by "file". It expects
 *    to find the input file organized in the same
 *    fashion as the input to the associative memory system.
 */

void
restore( file )
char *file;
{
  FILE *db;
  char buf[BUFSIZE];

  db = fopen( file, "r" );

  if( db != (FILE *) NULL )
    {
      while( fgets( buf, BUFSIZE, db ) > 0 )
	{
	  char *ptr;
	      
	  if( (ptr = find_equals( buf ) ) > 0 )
	    {
	      *ptr = EOS;
	      insert( strsave( buf), strsave( ++ptr ));
	    }
	}
      fclose( db );
    }
  return;
}
