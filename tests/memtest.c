/* memtest.c */


/* AUTHOR:  Chris Reid <spikeysnack@gmail.com> */
/* LICENSE: Free for all purposes              */
/* COPYRIGHT: 2015- Chris Reid                 */

#include <stdio.h>
#include "dstring.h"

struct kst
{
  size_t size;
  size_t free;
  char str[1024];
};

struct kst kst_pool = {0, 1024 , {0} };

dstring dkst = (dstring)kst_pool.str;
 



/**********************************************************************/
/*  UTILS & MACROS  */
/********************/
#define P(D)     if(D) printf ("%s" , D )
#define PN(D)    if(D) printf ("%s\n" , D )
#define SZ(D)    if(D) printf ("%zu : %zu : (%zu), [%zu]" , dstlen(D) , dstfree(D), dst_mem(D)-sizeof(struct dst) , dst_mem(D) )
#define PD(D)    do { P(D); P(" "); SZ(D); PN(""); }while(0) 

/**********************************************************************/





int main ( int argc, char** argv , char** env)
{


  system("clear");

  PD(dstring_version() );

  PN("********************************************");
  localdstring ver = DSLOCAL_FMT( "dstring library version ((%s))", dstring_version() );
  PD(ver);
  PN("********************************************\n");


  printf("size of struct dst: %zu \n" , sizeof(struct dst) ); 

  if ( "" != NULL )   printf("\"\" != 0   %zu\n", sizeof("") ); 

  if ( strcmp("" , "\0" )) printf("same\n" ); 
  else printf("not the same\n" ); 


  dstring zero = new_dstring("");

  if ( zero ) 
    {
      printf(" zero is not null \t" );
      PD(zero);
    }

  if ( dst_cmp(zero , "" )) printf("zero and \"\" same\n" ); 
  else printf("zero and \"\" not the same\n" ); 


  dstring a = new_dstring_init("ABCDE" , 16 );
  dstring b = new_dstring_init("FGHIJ" , 16 );
  dstring c = new_dstring_init("KLMNO" , 16 );
  dstring d = new_dstring_init("PQRST" , 16 );

  dstptr da = (dstptr) (a - sizeof(*a) );
  printf("size of dstptr of a: %zu \n" , sizeof(*da) ); 

  DSDEBUG(a);

  PN("append 4 items : dstrings, nulls,  and cstrings into a dstring ==>" );  
  dstring e = dstring_append_multiple(4, a , b , c, d , "MORE", NULL, "SOME MORE EVEN", "THE END");
    PD(e);

    PN("clear it but keep the memory for use later:");

    dstring_clear(e);
    PD(e);

    PN("append 8 items : dstrings, nulls,  and cstrings into a dstring ==>" );  
    e = dstring_append_multiple(8, a , b , c, d , "MORE", NULL, "SOME MORE EVEN", "THE END");


  PD(e);
  P("PRINT_ALL:\t");
  PRINT_ALL(a,b,c,d,e);

  PN("");

  PN("free all dstrings ( null ones are ignored)");


     DSFREE_ALL(a,b,c,d,e);



    //int j =   PP_NARG( a, b, c, d, e);
  
    //printf("j = %d\n ", j);

  PN("should be empty now");

  PRINT_ALL(a,b,c,d,e); //nothing should print. the pointers are still there, though.
  PN("");

  localdstring lds = DSLOCAL_FMT( " A LOCAL DSTRING Frees automatically when %s ends", __FUNCTION__ );

  PD(lds);

  DSFREE_ALL();

  return 0;
}

/*END*/
