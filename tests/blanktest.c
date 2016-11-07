#include <stdio.h>

int main (int argc, char**argv , char** env)
{

  char* b = "";
  char* c = "\0";
  char* d = '\0';


  if ( "" )
    printf( "(\"\") emptystring is true\n");
  else
    printf( "(\"\") emptystring is false\n");



  if ( b )
    printf( "(b) emptystring is true\n");
  else
    printf( "(b) emptystring is false\n");



  if ( "\0" )
    printf( " (\"\\0\") nullchar is true\n");
  else
    printf( " (\"\\0\") nullchar is false\n");

  if ( c )
    printf( " (c) nullchar is true\n");
  else
    printf( " (c) nullchar is false\n");


  if ( '\0' )
    printf( "\'\' is true\n");
  else
    printf( "\'\' is false\n");


  if ( d )
    printf( "(d) is true\n");
  else
    printf( "(d) is false\n");









  return 0;
}
