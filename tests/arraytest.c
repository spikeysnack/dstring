/*arraytest.c*/


/* AUTHOR:  Chris Reid <spikeysnack@gmail.com> */
/* LICENSE: Free for all purposes              */
/* COPYRIGHT: 2015- Chris Reid                 */


/* A quick test of dstring arrays and
 * their utilities 
 */


#include <stdio.h>
#include <stdlib.h>
#include "dstring.h"
#include "dstringarray.h"
#include "key_utils.h"


/* PRINT MACROS */
#ifdef DEBUG
#define P(D)     if(D) printf ("[%s] : %zi  : %zi : %zi  (%zi)\n" , D , dstlen(D) , dstfree(D) , dst_mem(D)-16 , dst_mem(D) )
#else
       #define P(D)    if(D) printf ("%s" , D )
#endif

#define PN(D)    if(D) P(D); printf ("\n")

#define sz(D)    if(D) printf ("%zi : %zi : (%zi)" , dstlen(D) , dstfree(D), dst_mem(D)-16 )


void split_test(void);
void macro_test();

int main(int argc, char** argv, char** env)
{
  int i;
  char numstr[6] = {0};
  dstring dsbig =NULL;

  dsbig = dstring_new_init("START" , 327680 );
  
  for (i = 0 ; i<= 100; i++)
    {
      snprintf( numstr, 6, "%d ", i );
      dsbig = dstring_append( dsbig, numstr);
      if (! is_dstring(dsbig))
	{
	  fprintf(stderr, "QUICKCHECK FAILED:\t %zi : %zi :%zi (%s)\n", dstlen(dsbig) , dstfree(dsbig) ,dst_mem(dsbig), numstr );
	  break;
	}      
    }// for
 
 dstringarray dsa = new_dstring_array( 10 );

 printf( " array size: %zu \n", dsa->size ); 

 dstring IG= dstring_new_init( "I GUESS" , 100 );

 dstring IGN= dstring_new_init( "(I GUESS NOT)" , 100 );


 dstring_add(dsa, IG);

 dstring_add(dsa, IGN);

 dstring_set( dsa , dsbig , 7 );

 dstring_set( dsa , new_dstring("four") , 4 );
 
 dsa->arr[8] = dsbig; // direct assignment  "DANGEROUS?"

 dstring_set( dsa , dsbig , 20 );

 dstring_set( dsa , dstring_copy(dsbig) , 4 );


 if ( ARRAY_CHECK(dsa) )
   {
     printf("ARRAY_CHECK Array is OK.\n");
   }
 else
   {
     printf("ARRAY_CHECK Array FAILED. can't go on.\n");
     exit(-1);
   }


 if ( FULL(dsa) )
   {
     printf("Array is full.\n");
   }
 else
   {
     size_t a = COUNT(dsa);
     printf("Array has %zu open slots out of %zu.\n", dsa->size - a , dsa->size );
   }

 size_t j;

 for (j = 0 ; j < dsa->size ; j++) printf( "%zi : %s \n", j , dsa->arr[j] );


 
 int c1 = COUNT(dsa);
 
printf("Culling array duplicate pointers .\n Array has %zu open slots out of %zu.\n", dsa->size - c1 , dsa->size );

 int c2 = dstringarray_cull(dsa);

  printf("Culled array.\n Array now has %zu open slots out of %zu.\n", dsa->size - c2 , dsa->size );


 printf("Culling array copies.\n Array has %zu open slots out of %zu.\n", dsa->size - c1 , dsa->size );
  int c3 = dstringarray_cull_copies(dsa);
 printf("Culled array.\n Array now has %zu open slots out of %zu.\n", dsa->size - c3 , dsa->size );

 for (j = 0 ; j < dsa->size ; j++) printf( "%zi : %s \n", j , dsa->arr[j] );



 DSARRAY_CLEAR(dsa); // set  all  pointers in array to null;

 printf("Array is cleared of all entries.\n");


 if ( ARRAY_EMPTY(dsa) )
   {
     printf("Array is empty.\n");
   }
 else
   {
     size_t a = COUNT(dsa);
     printf("Array has %zu open slots out of %zu.\n", dsa->size - a , dsa->size );
   }


 

 DSARRAY_PRINT(dsa);


 DSARRAY_FREE(dsa); /* Frees all dstrings in Array but there are none here after DSARRAY_CLEAR */

 DSFREE(IG);
 DSFREE(IGN);
 DSFREE(dsbig);


 split_test();

 macro_test();

  return 0;
}
 
/**********************************************************************/


 void split_test()
 {

   dstring test = new_dstring("This is to test the splitting function of dstringarrays, while counting their words.");
   
   dstringarray dsa;

   dsa = dstringarray_from_dstring( test, " ");

   DSARRAY_PRINT(dsa);

   DSARRAY_FREE(dsa);

   dsa = dstringarray_from_dstring( test, ",");
   DSARRAY_PRINT(dsa);

   DSARRAY_FREE(dsa);

   dsa = dstringarray_from_dstring( test, ".");
   DSARRAY_PRINT(dsa);
   DSARRAY_FREE(dsa);


   dsa = dstringarray_from_dstring( test, ":");
   DSARRAY_PRINT(dsa);
   DSARRAY_FREE(dsa);



 }

 void macro_test()
 {
   dstringarray dsa = NULL;
   dsa = DSARRAY("a", "b", "C", "D", "e", "f", "G" ); 

   DSARRAY_PRINT(dsa);                                                                                                                                                                        
   DSARRAY_FREE(dsa);
 }

/*END*/
