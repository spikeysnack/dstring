/*dstringtest.c*/

/* AUTHOR:  Chris Reid <spikeysnack@gmail.com> */
/* LICENSE: Free for all purposes              */
/* COPYRIGHT: 2015- Chris Reid                 */

/* test the basic functionality of dstrings and
   their utilities.
*/


#include <stdio.h>
#include "dstring.h"
#include "key_utils.h"

#define LOOPS 10000


/* PRINT MACROS */
#define P(D)    if(D) printf ("[%s] \n" , D ) ;
#define PD(D)   printf ("[%s] : %zi  : %zi : (%zi) \n" , D , dstlen(D) , dstfree(D) , dst_mem(D) )
#define sz(D)   printf ("%zi : %zi \n" , dstlen(D) , dstfree(D) )

typedef void (*testf)(void);
 
void verheader( void );
void basic_test( void );
void append_test( void );
void large_append_test( void );
void fast_append_test( void );
void memory_test( void );
void copy_test( void );
void max_test( void );
int main (int argc, char** argv, char** env);


void verheader( void )
{
  dstring ver = dstring_version();
  system("clear");
  printf("=============================================\n");
  printf("dstring version:\t %s\n", ver );
  printf("=============================================\n\n");
}



/* the first function run*/

int main (int argc, char** argv, char** env)
{

  verheader();
  basic_test();
  askforkey("press any key");  

  verheader();
  append_test();
  askforkey("press any key");  

  verheader();
  large_append_test();
  askforkey("press any key");  

  verheader();
  fast_append_test();
  askforkey("press any key");  

  verheader();
  copy_test();
  askforkey("press any key");  

  max_test();

 return 0;
} //main


void basic_test()
{

  P("==== BASIC TESTS =========");

  /*********** ds ***********/
  /* dynamically create a dstring of 100 bytes */

  P("new_dstring_init(  (char*), 1000) \n" );

  dstring ds = new_dstring_init("A brand new string" , 1000 );

  PD(ds);

  //  DSTPTR(ds);

  printf("dstring_clear(ds) \n");
  dstring_clear(ds);
  PD(ds);

  printf("dstring_free(ds) \n");
  DSFREE(ds);
  

} // basic_test



void append_test()
{
  dstring ds1 = NULL;
  dstring ds2 = NULL;

  P("==== BASIC APPEND TEST ====");

  ds1 = new_dstring( "ABCD" );
  PD(ds1);
  
  printf("dstring_append ( ds1, EFGHIJKLM)  \n" );

  ds2 = dstring_append ( ds1, "EFGHIJKLM" );

  PD(ds2);

  DSFREE(ds1);
  DSFREE(ds2);

} // append_test



void large_append_test()
{

 dstring ds = NULL;
 dstring tmp = NULL;

  P("==== LARGE APPEND TEST ====");

  int i ;
  char numstr[8] = {0};
  //  dstring tmp;
  
  for (i = 0 ; i<= LOOPS ; i++)
    {
      snprintf( numstr, 8, "%d, ", i );

      tmp = ds;

      ds = dstring_append( tmp, numstr);

      if (! dstcheck(tmp) ) DSDEBUG(tmp);

      DSFREE(tmp);

      memset(numstr, 0, 8); //clear it
      //      if(tmp)  dsmem = tmp;	
    }

  
  PD(ds);

  DSFREE(ds);
} // large_append_test




void fast_append_test()
{
  dstring ds = NULL;

  P("==== FAST APPEND TEST ====");

  P("10,000 times ");


  int i ;
  char numstr[8] = {0};
  //  dstring tmp;
  
  for (i = 0 ; i<= LOOPS ; i++)
    {
      snprintf( numstr, 8, "%d, ", i );

      ds = dstring_fast_append( ds, numstr);
      
      memset(numstr, 0, 8); //clear it
      //      if(tmp)  dsmem = tmp;	
    }

  
  PD(ds);

  DSFREE(ds);

} // append_test



void memory_test()
{
  P("==== MEMORY TESTS ====");

  dstring dsmem = new_dstring("Basic Memory Test 1");
  PD(dsmem);

  printf("dstring_lengthen( dsmem, 100 )\n");
  dsmem =  dstring_lengthen ( dsmem, 100 );
  PD(dsmem);
  
  printf("dstring_minimize(dsmem)\n\n");
  dstring_minimize( dsmem);
  PD(dsmem);
  
  printf("dstring_lengthen( dsmem, 100 )\n");
  dsmem =  dstring_lengthen ( dsmem, 100 );
  PD(dsmem);
  
  printf("dstring_resize(dsmem, 1000 )\n\n");
  dsmem = dstring_resize( dsmem, 1000 );
  PD(dsmem);
  
  printf("dstring_minimize_mem(dsmem)\n\n");
  dstring_minimize_mem( dsmem);
  PD(dsmem);

}


void copy_test()
{
  P("==== COPY TEST ====");

  dstring ds = new_dstring(" One to be copied and stuff");

  printf("dstring_copy(ds)\n\n");

  dstring dscpy = dstring_copy(ds);

  if ( ! dst_cmp( ds , dscpy) )
    printf(" [dst_cmp] DSTRINGS ARE IDENTICAL\n");
  else
    printf(" [dst_cmp] STRINGS ARE DIFFERENT\n");

  DSFREE(ds);

  P(dscpy);
 
  DSFREE(dscpy);

} // copy_test


void max_test()
{
  
  dstring ds;
  int check;

  P("==== MAX SIZE TEST ====");

  printf( "dstring DST_MAX:\t%zi\n" , DST_MAX ); 
  printf( "dstring_new_len:\t %zi \n" , (ssize_t)50000000 ); 
 
 ds = dstring_new_init( "SUPER LONG DSTRING"   , 50000000 );

 check = dstring_check(ds);
 printf( "string size: %zd\t memsize: %zi  check=%d \n", dstlen(ds), dst_mem(ds) , check ); 
 
 P(ds);
 
 DSFREE(ds);

} // max_test


/*END*/
