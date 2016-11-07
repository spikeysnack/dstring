/**********************************************************************/
/* localtest.c */

/* AUTHOR:  Chris Reid <spikeysnack@gmail.com> */
/* LICENSE: Free for all purposes              */
/* COPYRIGHT: 2015- Chris Reid                 */


/* run some tests on localdstrings */

/* headers */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "dstring.h"
#include "key_utils.h"
// #include "localdstring.h" // now included with dstring.h


/**********************************************************************/
/*  UTILS & MACROS  */
/********************/
#define P(D)     if(D) printf ("%s" , D )
#define PN(D)    if(D) printf ("%s\n" , D )
#define SZ(D)    if(D) printf (" %zu : %zu : (%zu) " , dstlen(D) , dstfree(D), dst_mem(D) )
#define PD(D)    do { P(D);  SZ(D); PN("");}while(0) 
#define LPD(D)   do { P(D); LSZ(D); PN("");}while(0) 

/**********************************************************************/

typedef void(*testf)(void);


void  localtest1( const dstring ds );
void  localtest2(void);
void  localtest3(void);
void  localtest4(void);
void  localtest5(const cstring cs);
void  localtest6(void);




/********************************/
void localtest1( const dstring ds )
{


  localdstring local_s          = NULL;
  localdstring local_alphabet   = NULL;
  localdstring local_numbers    = NULL;
  localdstring local_formatted  = NULL;
  cstring      numbers          = "0123456789";

  PN("[test 1]");


  local_s = DSLOCAL(ds );
  local_alphabet = DSLOCAL( "abcdefghijklmonpqrstuvwxyz" );  
  local_numbers = DSLOCAL(numbers);
  local_formatted = DSLOCAL_FMT("[<{%s %s %08f>]" , "PI is close to", ":\t", 3.14159 );

  printf( "%s\t %s \t%s\n", local_s, local_alphabet, local_numbers);

  PD(local_s);  

  PD(local_alphabet);  

  PD(local_numbers);  

  PD(local_formatted);    
  
  /* don;t need to free these */
} // localtest


/*******************/
void localtest2(void)
{
  int result = 0;

  float float_from_dstring = 0.0;

  PN("[test 2]");

  /* don't need to free this */
  localdstring from_floats = DSLOCAL_FMT("Pi approximation:\t %08f \n", 234.7899797 / 74.741222 );

  PD(from_floats);

  result =  sscanf( from_floats, "%*[^\t]%08f\n", &float_from_dstring ); 
  
  if (result) fprintf( stdout , "The localdstring value scanned is %08f \n" , float_from_dstring );

  /* don't need to free this */
  localdstring lds = DSLOCAL("3.14159");

  PD(lds);

  fprintf( stdout , "The localdstring value is %s \n" , lds );

  /* this works */
  DSCLEAR(lds);

  PD(lds);
  fprintf( stdout , "The localdstring value is %s \n" , lds );

    /* this works too*/
  result =  dsprintf( lds, 9, "%08f\n", float_from_dstring ); 
 

 if (result) 
   fprintf( stdout , "The localdstring value is %s \n" , lds );


} // localtest2



void localtest3(void)
{
  int result = 0;

  float float_from_dstring = 0.0;

  localdstring from_float;   /* don't need to free this */

  PN("[test 3]");

  from_float = DSLOCAL_FMT("Pi approximation:\t%08f", 234.789979 / 74.751222 );

  PD(from_float);

  result =  DSLOCAL_SCAN( from_float, "%*[^\t]%08f\n", &float_from_dstring ); 

  //  result =  sscanf( from_float, "%*[^\t]%08f\n", &float_from_dstring ); 
  
  if (result) fprintf( stdout , "The float value scanned is %08f\n" , float_from_dstring );

  if (result) fprintf( stdout , "The localdstring is %s\n" , from_float);



} // localtest3


void localtest4(void)
{

  localdstring A, B;   /* don't need to free this */

  PN("[test 4]");

  A = DSLOCAL("A  local string");
  B = DSLOCAL("B  local string");

  P("[4]\tA\t"); PD(A);
  P("[4]\tB\t"); PD(B);


} // localtest4


void localtest5(const cstring cs)
{
  localdstring A, B;   /* don't need to free this */

  PN("[test 5]");
  A = dstring_auto(cs);
  B = dstring_auto("still in scope, not overwiting A");

  P("[5]\tA\t"); PD(A);
  P("[5]\tB\t"); PD(B);

  if (dst_cmp(A, B)== 0 )
    fprintf( stderr, "ERROR AA and BB are the same!\n" );

  if (A == B)
    fprintf( stderr, "ERROR A and B point to same memory! (AA overwritten on stack) BAD\n" );

} // localtest5


void localtest6(void)
{
  localdstring CC, DD, EE;

  PN("test 6");

  CC  = dstring_auto("[***************************************************************]");
  DD  = dstring_auto( strndupa(CC, dstlen(CC)) );
  EE  = dstring_auto_null();
 

  
  P("6 CC\t"); PD(CC);
  P("6 DD\t"); PD(DD);
  P("6 EE\t"); PD(EE);


  if (dst_cmp(CC, DD) != 0 )
    fprintf( stderr, "6 ERROR CC and DD are NOT the same!\n" );

  if (CC == DD)
    fprintf( stderr, "6 ERROR CC and DD point to same memory! (AA overwritten on stack) BAD\n" );


  if ( dstcheck(EE) )
    PN("6 EE dstcheck passes");


  if (  dstring_check(EE) )
    PN("6 EE dstring_check passes");



  EE = DSLOCAL_CONCAT( DD , CC);
  P("6 EE\t"); PD(EE);
  
 
  EE = DSLOCAL_CONCAT( EE , "...TEH END" );
  P("6 EE\t"); PD(EE);

  localdstring FF = DSLOCAL_CONCAT( "0" , "1",  "2", "3", "6", "7", "8", "9", "A", "B", "C", "D", "E", "F" );

  P("6 FF\t"); PD(FF);

}//localtest6


int main ( int argc, char** argv , char** env)
{

  dstring alphabet = NULL;

  system("clear");
  PN(dstring_version() );

  /* create a regular dstring */	
  alphabet = dstring_new_init(  "ABCDEFGHIJKLMNOPQRSTUVWXYZ", 160 );   

  localtest1(alphabet);
  /* free regular dstring */
  DSFREE(alphabet); 


  /* don;t need to free these */
  localtest2();

  localtest3();

  localtest4();

 localtest5("local test #5");

 localtest6();
  

  return 0;
}





/*END*/
