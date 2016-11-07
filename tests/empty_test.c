/* empty_test.c */

/* AUTHOR:  Chris Reid <spikeysnack@gmail.com> */
/* LICENSE: Free for all purposes              */
/* COPYRIGHT: 2015- Chris Reid                 */


#include <stdio.h>
#include "dstring.h"
#include "dstringarray.h"
//#include "dstring_utils.h"
#include "key_utils.h"

#define LOOPS 10000

extern dstring test_global;



//char* replace_word( char* in , char* match, char* replacement );
//char* replace_2( char* in, char* match, char* replacement );


void empty_test(  char* arg);




#define P(S)    printf("%s", S )
#define Pn(S)   printf("%s\n", S )
#define Pt(S)   printf("%s\t", S )
#define PDS(DS) printf("(%s) %zu :%zu \n", DS, dstlen(DS), dstfree(DS) )


void verheader( dstring ver )
{
  system("clear");
  printf("=============================================\n");
  printf("dstring version:\t %s\n", ver );  
  printf("=============================================\n\n");
}


int main(int argc, char **argv, char** env )
{

  if (argc == 1 ) 
    {
      fprintf( stderr, " ERROR No Arg.\n");
      fprintf( stderr, "\tusage:   empty_test  <arg>\n");
      exit(-1);
    }


  if (argc == 2)
    {
      verheader( dstring_version() );
      P("==EMPTY TEST==");      

      empty_test(  argv[1]);
      askforkey("press any key");
    }




  exit(EXIT_SUCCESS);
} /* main */





void empty_test( char* arg)
{

  cstring a  = NULL;
  dstring b  = NULL;

  if (!arg) 
    {
      fprintf(stderr, "ERROR  NO ARG \n" );
      exit(-1);
    }

  a = strdup( arg);
  b = new_dstring(arg);

  if ( is_empty( a) )
    fprintf(stderr, " a is empty [%s] \n" , a );
  else
    fprintf(stderr, " a is NOT empty [%s] \n" , a );


  if ( is_empty( b) )
    fprintf(stderr, " b is empty \n") , PDS(b) ;
  else
    fprintf(stderr, " b is NOT empty \n") , PDS(b) ;

} // empty_test



/*END*/
