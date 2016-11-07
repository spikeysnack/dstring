/* replace_test2.c */

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

void replace_test( int argc, char** argv);
void space_test();
void append_test();
void array_test();
void array2_test();
void scan_test();



#define P(S)   printf("(%s)\n", S )
#define PDS(DS)   printf("(%s) %zu :%zu \n", DS, dstlen(DS), dstfree(DS) )

void verheader( dstring ver )
{
  system("clear");
  printf("=============================================\n");
  printf("dstring version:\t %s\n", ver );  
  printf("=============================================\n\n");
}


int main(int argc, char **argv, char** env )
{

  if (argc != 4)
    {
      fprintf( stderr, "usage:\t %s <string>  <match>  <replacement>\n", argv[0] );
    }
  else
    {     
      verheader( dstring_version() );

      P("==REPLACE TEST==");      
      replace_test( argc,  argv);
      askforkey("press any key");

      verheader( dstring_version() );
      P("==SPACE TEST==");
      space_test();
      askforkey("press any key");
      
      verheader( dstring_version() );
      P("==APPEND TEST==");
      append_test();
      askforkey("press any key"); 

      verheader( dstring_version() );
      P("==ARRAY TEST==");
      array_test();
      askforkey("press any key");

      verheader( dstring_version() );
      P("==SCAN TEST==");
      scan_test();
    }


  exit(EXIT_SUCCESS);
} /* main */






void replace_test( int argc, char** argv)
{
  dstring input;
  cstring match; 
  cstring replacement;
  dstring replaced = NULL;
  dstring test     = NULL;
  dstring tc1       = NULL;
  dstring tc2       = NULL;
  dstring tc3       = NULL;

  dstring uc       = NULL;
  dstring ac       = NULL;

  assert( argv[1] && argv[2] && argv[3] );

  printf("[replace_word]\n");
  
  input = new_dstring(argv[1]);
  match = argv[2];
  replacement = argv[3];
  
  replaced = replace_word( input , match, replacement ); //works
   
  printf( "[%s] ==> [%s]\n\n", argv[1] , replaced );
   
  test = dstring_copy (replaced );

  DSFREE(replaced);
  DSFREE(input);
      
  printf("[title_case]\t");
  tc1 = title_case( test);
  printf("[%s] \n", tc1 );
  
  printf("[uncapitalize]\t");
  uc = uncapitalize( tc1);
  printf( "[%s] \n", uc );
  
  printf("[all_caps]\t");
  ac = all_caps( uc);
  printf( "[%s] \n", ac );
  
  
  printf(" [is_sentence]\t");

  if (is_sentence(tc1) )
    printf( "%s ==> IS a sentence. \n", tc1 );
  else
    printf( "%s ==> NOT a sentence. \n", tc1 );
  
  printf("[title_case]\t");
  tc2 = title_case(uc);
  
printf( "[%s] \n", tc2 );
    

  printf("[title_case]\t");

  tc3 = title_case( "is it not time for a change to our lives of an age bygone by any means and trial?" );

  printf( "[%s]\n\n", tc3 );

  
  DSFREE(tc1);
  DSFREE(tc2);
  DSFREE(tc3);

  DSFREE(ac);
  DSFREE(uc);
  DSFREE(test);
  DSFREE(replaced);
  
} //replace_test


void space_test()
{
  dstring trouble =  new_dstring( "This string  has  some         overly-spaced         words.");
  dstring  notrouble;


  notrouble = rm_dblspaces(trouble) ;
  
  if ( notrouble )
    {
      notrouble = replace_word( notrouble, "some" , "no");
      printf("%s ==> %s \n", trouble, notrouble);
    } 


      printf( "[chomp]\t" );
      dstring cr = new_dstring("this string\n has \r  a bunch of \n\r  carriage-\rreturns\n");
      printf( "[%s]\n", cr );
      cr = chomp(cr);
      printf( "[%s]\n", cr );


      dstring astr = new_dstring("First Part"); 
      dstring bstr = new_dstring("Second Part"); 
      dstring cstr = new_dstring("Third Part"); 

      printf( "[%s]\n", "[prepend]" );
      dstring ba = dstring_append(bstr , astr);
      printf( "ba:\t[%s]\n", ba );

      printf( "[%s]\n", "[append]" );

      dstring ab =  dstring_append(astr , bstr);

      printf( "ab:\t[%s]\n", ab );

      dstring abc =  dstring_append(ab , cstr);

      printf( "abc:\t[%s]\n", abc );
      
      printf( "[%s]\n", "[insert]" );

      dstring ins = insert( abc, "{inserted text}", 16 );

      printf( "ins:\t[%s]\n", ins );

      printf( "[%s]\n", "[insert word]" );

      dstring insw = insert_word( abc, "{inserted words}", 16 );

      printf( "insw:\t[%s]\n", insw );


      DSFREE_ALL( astr, bstr, cstr, ab , ba, abc, ins, insw );

} //space_test



void append_test()
{
  int i = 0;
  int ret = 0;

  dstring faststr = NULL;
  dstring numstr = NULL;
  int numchars = 0;  

  printf( "[%s]\n", "large memory fast append" );
  
  i = 0;

  numstr  = DSBLANK(16); 
  numchars = dstfree(numstr);
  
  for ( i = 0; i < LOOPS ; i++)
    {
      
      ret= dsprintf( numstr, numchars, "%d " , i+1 ); 

      if (!ret) fprintf(stderr , "ERROR dsprintf failed on %d" , i+1); 
      
      if ( ! (i % 10000 ) ) 
	{
	  printf( "%d appends\n" , i); 
	  fflush(stdout);
	} 
      


      faststr = dstring_fast_append( faststr, numstr );
      
      if (faststr == NULL ) 
	{
	  printf("%d %s failed\n", i, numstr);
	  break;
	}
      
      dstring_clear(numstr);
      
    }//for
  
  askforkey("press any key");
  
  printf( "faststr:\t[%s]\n", faststr );
  askforkey("press any key");

       printf( "\n" );
       printf( "\n-----------------------------\n" );
       printf( "dstarray6\n" );    
       dstringarray dstarray6 = dstringarray_from_dstring( faststr , " " );
       DSARRAY_PRINT( dstarray6 );
       
       printf( "\n-----------------------------\n" );

       askforkey("press any key");

       printf( "dstarray7\n" );
      
       dstringarray dstarray7 = new_dstring_array ( 2000 - 1000);
       int k ;
       for (k = 1000 ; k < 2001 ; k++)
	 {
	   dstring_set( dstarray7 , dstarray6->arr[k], (k-1000) );
	 } 

       DSARRAY_PRINT( dstarray7  );
       printf( "\n-----------------------------\n" );

       printf( "\n" );

} // append_test



void array_test()
{

  dstring longstring = new_dstring("This is a sentence consisting of words, and phrases. Another Sentence, starts right here."); 

  printf( "dstring longstring:\t %s \n", longstring );
    


  printf( "[%s]\n", "dstring_array_from _dstring (longstring, \" \")" );
  
  dstringarray dsa = dstringarray_from_dstring( longstring, " " );
  
  DSARRAY_PRINT( dsa  );
     
  printf( "\n" );
  
  DSARRAY_FREE(dsa);

} // array_test



void scan_test()
{
  dstring ds;
  int i;
  float f;
  char c;
  size_t ret;

  ds = DSBLANK(32);

  ret = dsprintf( ds , dstfree(ds) , "%d. %c = %f" , 8, 'J', 1.046 );
   
  printf( "ds= [%s]\n",  ds); 

  ret = dsscanf( ds , "%d. %c = %f" , &i, &c , &f );

  if ( ret)  printf( "i= %d   c= %c   f= %f\n", i, c , f );

}
 


/*END*/
