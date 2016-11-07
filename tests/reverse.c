/*reverse.c */

/* reverse an array of strings, line by line */

/** to compile:
 *  gcc -O3  -mtune=native -foptimize-strlen -o reverse reverse.c -l dstring 
 *  gcc -O0 -ggdb3  -o reverse reverse.c -l dstring 
*/


/**************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dstring.h"
#include "dstringarray.h"


/*******************************/

void revprint (const cstring cs);
dstringarray dstringarray_from_cstringarray( const cstring csa[]);
void print_cstring_array(const cstring csa[] );
void print_dstringarray(const dstringarray dsa );

dstringarray reverse_array(dstringarray dsa);

/********************************************************/
const cstring cs[] = { 
                   "---------- Ice and Fire ------------",
		   "                                    ",
		   "fire, in end will world the say Some",
		   "ice. in say Some                    ",
		   "desire of tasted I've what From     ",
		   "fire. favor who those with hold I   ",
		   "                                    ",
		   "... elided paragraph last ...       ",
		   "                                    ",
		   "Frost Robert -----------------------",
		   NULL
                 };


dstringarray dstringarray_from_cstringarray( const cstring csa[])
{
  size_t n = 0;
  dstringarray dsa;

  while ( cs[n++] );

  dsa = NEW_DSARRAY(n);  

  n = 0;
while( csa[n] )
    {
      dstring_add( dsa, new_dstring(csa[n]) );
      n++;
    }
 return dsa;
}



/* print a string words forwards, 
   but order of words backwards */
/******************************/
void revprint (cstring cs) 
{
  dstringarray dsa1 = dstringarray_from_cstring(cs, " ");

  //  dstringarray dsa1 = dstringarray_from_dstring(new_dstring(cs), " ");
  size_t count = COUNT(dsa1);
    
  for( ; count ; count--)
    {
      fprintf(stdout, "%s ", DSTRING_AT(dsa1, count-1) );
    }

  DSARRAY_FREE(dsa1);
} // revprint 





/*******************************************/
/* print an array of strings , one per line*/
void print_cstring_array(const cstring csa[] )
{
  size_t i = 0;
  while ( csa[i] )
    {
      fprintf(stdout, "%s\n", csa[i] ); 
      i++;
    }

}


/**********************************************/
/* print a dstringarray of dstrings , one per line*/
void print_dstringarray(const dstringarray dsa )
{
  size_t i= 0 , j = COUNT(dsa);
  while ( i < j  )
    {
      fprintf(stdout, "%s\n", DSTRING_AT(dsa, i) ); 
      i++;
    }

}




/*******************************************/
int main (int argc, char** argv, char** env ) 
{

  print_cstring_array(cs);

  size_t count = 0;
  size_t i     = 0;

  while ( cs[count++] );

  dstringarray t = dstringarray_from_cstringarray(cs);

  //  DSARRAY_PRINT(t);
  

  for ( ; i < count; i++)
    {
      revprint( DSTRING_AT(t, i) );
      fprintf(stdout, "\n");
    }

  fflush(stdout);

  DSARRAY_FREE(t);

  return 0;
} // main



