/* numtest.c */

/* AUTHOR:  Chris Reid <spikeysnack@gmail.com> */
/* LICENSE: Free for all purposes              */
/* COPYRIGHT: 2015- Chris Reid                 */

#include <stddef.h>
#include <stdio.h>
#include <stdint.h>
#include <limits.h>

/* check maximums of some integer types */

int main ( int argc, char** argv, char** env)
{

  size_t sz;
  ssize_t ssz, mssz;
  unsigned long ul;
  signed long sl;

  sz = SIZE_MAX;

  ssz = SSIZE_MAX;
  mssz = - SSIZE_MAX;

  ul = ULONG_MAX;
  sl = LONG_MAX;


  


  printf( "size_t:  %zu (%zu bytes)\n", sz , sizeof(sz) );

  printf( "ssize_t %zi (%zu bytes)\n", ssz ,sizeof(ssz) );

  printf( "ssize_t mssz %zi (%zu bytes) \n", mssz, sizeof(mssz));

  printf( "ul %lu  (%zu bytes)\n", ul, sizeof(ul) );

  printf( "sl %li (%zu bytes) \n", sl, sizeof(sl) );


  printf( " left shift test \n");

    size_t j  = 1;
    ssize_t k = 1;

  /* check shifting possibilities */
  for ( sz = 0; sz < 65; sz++)
    {
      printf( "U %zu :  %zu \t" , sz ,  j << sz );
      printf( "S %zi :  %zi \n" , sz , -k << sz );
    }



 return 0;
}
