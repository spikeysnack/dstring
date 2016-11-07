#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* clang    -O1 -o nullptr nullptr.c */

/* shows strlen is unsafe for null string pointers */

/* this fixes it nicely, tho */ 
#define strlen(s) (s==NULL)?0:strlen(s)

size_t strlen(const char *s);

size_t (*oldstrlen)(const char *);


int main(int argc, char** argv , char** env)
{
  char* j; 
  void* v = NULL;
  void* v2 = 0;
  char* c = NULL;
  char* c2 = "";
  char* five = "ABCDE";

  oldstrlen = &strlen;  /* this captures the original strlen() */ 
  
  if ( argv[1] && (strcmp( argv[1] , "default" ) == 0) )
    {
      fprintf(stderr, "USING DEFAULT STRLEN\n"  );

      printf(" j:\t%zu\n" , oldstrlen(j) );
    }
  else
    {
      printf(" j:\t%zu\n" , strlen(j) );      
    }


  printf("void* v (NULL) :\t%zu\n" , strlen(v) );
  printf("void* v2 (0)  :\t%zu\n", strlen(v2));
  printf("char*  c (NULL) :\t%zu\n" , strlen(c) );
  printf("char* c2 (\"\") :\t%zu\n", strlen(c2));
  printf("char* five (ABCDE) :\t%zu\n", strlen(five));

  return 0;
}



