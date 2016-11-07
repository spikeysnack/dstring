/* testlist.c */

/* AUTHOR:  Chris Reid <spikeysnack@gmail.com> */
/* LICENSE: Free for all purposes              */
/* COPYRIGHT: 2015- Chris Reid                 */


#include <stdio.h>
#include <stdlib.h>
#include "dstring.h"
#include "dstringlist.h"


/********************************************/
/* function prototypes */

dstring dstring_test( size_t sz);
void copy_test( dstringlist* dsl);
dstringlist* get_list( dstring ds);
void print_list(dstringlist* dsl);


/********************************************/
/* PRINT MACROS */
#define PN(D)    if(D) printf ("%s\n" , D )

#define sz(D)    if(D) printf ("%zu : %zu : (%zu)" , dstlen(D) , dstfree(D), dst_mem(D)-16 )

#ifdef DEBUG
    #define P(D)  printf("\"%s\" : %zu  : %zu : %zu  (%zu) " , D , dstlen(D) , dstfree(D) , dst_mem(D)-16 , dst_mem(D) )
#else
    #define P(D)  if(D) printf ("%s" , D )
#endif

//#define LOOPS 100    /* tiny test */
//#define LOOPS 1000    /* small test */
#define LOOPS 10000   /* medium  test */
//#define LOOPS 100000  /* massive test */
//#define LOOPS 1024*1024   /* kernel will kill it */


/********************************************/


int main(int argc, char** argv, char** env)
{

  /* create a big list of dstrings */
  printf("%d loops :\n", LOOPS);
  dstring  bigdst = dstring_test( LOOPS );
  P(bigdst);  
  printf("%d loops done\n", LOOPS);

  printf("%d member list :\n", LOOPS);
 
 dstringlist* d_list = get_list(bigdst);

  print_list( d_list);  

  printf("\n%d member list :\n", LOOPS);


  askforkey("press any key");

/*************************************************/

  dstring A = new_dstring_init("I FOUND IT" , 32 );
  
  append_dstring_nodelete( d_list , A);

  dstring B=find_dstring( d_list , A);

  A = NULL; //safety -- but make sure there are no further refs to A

  if (B)   
    { 
      PN(B);
      del_dstring(d_list, B);
    } 

  dstring C = find_dstring( d_list , B); // if not found C will be NULL

  B = NULL; //safety -- but make sure there are no further refs to B

  if (C)  
    {
      PN(C);
    }   
  else 
    { 
      PN( "NOT FOUND"); 
    }



askforkey("press any key");
  PN( "DSTRINGLIST COPY TEST");  

  copy_test(d_list);

askforkey("press any key");
  /* cleanup memory */






  PN( "freeing memory");
  

  DSLISTFREE(d_list); // A  (& B & C too!) are NOT freed here because append_nodelete ;   


  DSFREE(C);     
  
  DSFREE(bigdst);  //safety -- NULLS bigdst variable as well as frees memory     

  PN( "memory freed");  
  


 return 0;
}

/**********************************************************************/

void copy_test( dstringlist* dsl)
{
  int different = 0;
  dstringlist* L;
  dstringlist* iter , *iter2;
  
  NEW_DSLIST(L);
  
  L = dstringlist_copy(dsl);

  printf( "PRINT_LIST L\n");

  print_list(L);

  iter2 = L;      

  /* need parens below (MACRO)*/
  list_for_each_entry(iter, &(dsl->list), list)
    {
      iter2 = list_entry((*iter2).list.next, dstringlist, list);      

      if ( dst_cmp(iter->ds , iter2->ds) != 0 )
	{
	  different = 1;
	  fprintf( stderr, "OOPS different.  [%s] != [%s] \n" , iter->ds , iter2->ds );
	}
    }


  if ( ! different ) printf("\nCOPY TEST PASSED. LIST AND COPY  HAVE IDENTICAL SIZE AND DSTRINGS.\n");

  DSLISTFREE(L);

} // copy_test





dstringlist* get_list( dstring ds)
{
  dstringlist* dsl; 
  /*
  dsl = calloc(1, sizeof(struct dst_list) );

  INIT_LIST_HEAD(&(dsl->list) );
  */
  NEW_DSLIST(dsl);

  (*dsl) = dstringlist_from_dstring( dsl, ds, " ");  // copies d

  return dsl;
} // get_list





void print_list( dstringlist* dsl)
{
 dstringlist* iter;

  /* need parens below (MACRO)*/
  list_for_each_entry(iter, &(dsl->list), list)
    {
      P( iter->ds  ); 
    }
}




dstring dstring_test( size_t sz)
{
  dstring dsbig = NULL;
  int i ;
  char numstr[8] = {0};

  dsbig = new_dstring_init("", sz * 8 );

  printf("dstring_append(dsbig, numstr)  (%zu times )\n", sz);
  
  for (i = 0 ; i<= sz ; i++)
   {     
     snprintf( numstr, 8, "%d ", i );     
     //     printf("%d  %s\n", i, numstr );

     if (! (i % 50000) ) printf( "%d\n", i);

     dsbig = dstring_append( dsbig, numstr );
   }


 // PN(dsbig);
 return dsbig;

}



void askforkey("press any key")
{
  char c ='\0';
  printf("<press any key>\n");
  //ungetc(c , stdin);
  c = getchar();
  //c = getc(stdin);
  if (c == '\0' ) return;

}



/*END*/
