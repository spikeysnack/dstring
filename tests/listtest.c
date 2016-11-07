/* listtest.c */

/* AUTHOR:  Chris Reid <spikeysnack@gmail.com> */
/* LICENSE: Free for all purposes              */
/* COPYRIGHT: 2015- Chris Reid                 */


/* run some tests on linked lists of dstrings */

/* headers */
#include <stdio.h>
#include <stdlib.h>
#include "dstring.h"
#include "dstringlist.h"
#include "key_utils.h"


/********************************************/
/* PRINT MACROS */

//#define LOOPS 100     /* tiny    test (basic logic)*/
//#define LOOPS 1000    /* small   test */
#define LOOPS 10000   /* medium  test (real app size)*/
//#define LOOPS 50000   /* large   test*/
//#define LOOPS 100000  /* massive test */
//#define LOOPS 1024*1024 /*stress test (kernel will probably kill it) */
/********************************************/


/**********************************************************************/
/*  UTILS & MACROS  */
/********************/
#define P(D)     if(D) printf ("%s" , D )
#define PN(D)    if(D) printf ("%s\n" , D )
#define SZ(D)    if(D) printf ("%zu : %zu : (%zu)" , dstlen(D) , dstfree(D), dst_mem(D)-16 )
#define PD(D)    do { P(D); SZ(D); }while(0) 

/**********************************************************************/


/********************************************/
/* function prototypes */
/********************************************/

int main(int argc, char** argv, char** env);
void verheader( dstring ver );

dstring loops_test( size_t sz);
void    copy_test( dstringlist dsl);
void    find_test( dstringlist dsl );
void    join_test(dstringlist dsl);

dstringlist get_list( dstring ds);
void print_list(dstringlist dsl);


void verheader( dstring ver )
{
  system("clear");
  printf("=============================================\n");
  printf("dstring version:\t %s\n", ver );
  printf("=============================================\n\n");
}

/********************************************/

/* the first function run */
int main(int argc, char** argv, char** env)
{
  
  dstringlist dsl_1 = NULL;
  dstringlist dsl_2 = NULL;

  dstringlist firstlist = NULL;

  

  dstring ver = dstring_version();
  verheader(ver);

  PN("==== BASIC LIST TESTS ====");

  firstlist= new_dstringlist();
  print_list( firstlist);
  DSLISTFREE(firstlist);

  dsl_1 = new_dslist( "one" , "two",  "three");
  dsl_2 = new_dslist( "DOG" , "CAT",  "FERRET", "HORSE" );

  PN("list 1");
  print_list( dsl_1);
  P("\n");
  fflush(stdout);

  PN("appending four five six to list1:");
  append_dstring(dsl_1, new_dstring("four"));
  append_dstring(dsl_1, new_dstring("five"));
  append_dstring(dsl_1, new_dstring("six"));

  append_dstring_multi(dsl_1, new_dstring("six") , new_dstring("seven") , new_dstring("eight"));

  P("\n");
  fflush(stdout);

  PN("list 1");
  print_list( dsl_1);
  P("\n");
  fflush(stdout);

  PN("list 2");
  print_list( dsl_2);
  P("\n");
  fflush(stdout);

  append_dslist( dsl_1 , dsl_2);  
  PN("list 1");
  print_list( dsl_1);
  P("\n");

  PN("list 2");
  print_list( dsl_2);
  P("\n");

  PN("Freeing lists");
  DSLISTFREE(dsl_2);
  DSLISTFREE(dsl_1);

  if ( dsl_2 )   
    { PN("list2 not free!");}
  else 
    {PN("list2 freed OK");}

  if ( dsl_1 )   
    {PN("list1 not free!");}
  else 
    {PN("list1 freed OK");}

  /************************************/


  askforkey("press any key");
  verheader(ver);

  P("==== LOOPS TEST ====");
  /* create a big list of dstrings */
  printf("%d loops :\n", LOOPS);

  askforkey("press any key");
  dstring  bigdst = loops_test( LOOPS );
  P(bigdst);  
  printf("%d loops done\n", LOOPS);
  askforkey("press any key");
/********************************************/



  verheader(ver);
  PN("==== LIST FROM DSTRING TEST ====");

  dstringlist d_list = get_list(bigdst);
  print_list( d_list);  
  printf("\n%d member list :\n", LOOPS);
exit(0);

  askforkey("press any key");
/*********************************************/
  verheader(ver);
  PN("==== FIND TEST ====");
  find_test(d_list);
  askforkey("press any key");

/*********************************************/
  verheader(ver);
  PN( "==== DSTRINGLIST COPY TEST ====");  
  copy_test(d_list);
  askforkey("press any key");

/*********************************************/
  verheader(ver);
  PN( "==== JOIN TEST ===");  

  join_test(d_list);
  askforkey("press any key");

/*********************************************/
  verheader(ver);
  PN( "==== FREE ALLOCATED MEMORY ====");  
  DSLISTFREE(d_list); // A  (& B & C too!) are NOT freed here because append_nodelete ;   
  DSFREE(bigdst);  //safety -- NULLS bigdst variable as well as frees memory     
 
  PN( "DONE");    
  return 0;
} // main

/**********************************************************************/
/* test functions */
/**********************************************************************/

dstring loops_test( size_t sz)
{
  dstring ds = NULL;
  int i ;
  char numstr[8] = {0};

  ds = new_dstring_init("", sz * 8 );

  printf("dstring_append(dsbig, numstr)  (%zu times )\n", sz);
  
  for (i = 0 ; i<= sz ; i++)
   {     
     snprintf( numstr, 8, "%d ", i );     
     //     printf("%d  %s\n", i, numstr );
     if ( (i % 10000) == 0 ) printf( "%d\n", i);
     ds = dstring_append( ds, numstr );
   }

 return ds;
} // dstring_test

/**********************************************************************/

void find_test( dstringlist dsl )
{
  
 PN("FIND TEST");

 dstring A = new_dstring_init("I FOUND IT" , 16 );

 append_dstring_nodelete( dsl , A); 

 PN("searching list");
 dstring B=find_dstring( dsl , A);

 if (B)   
   { 
     P("found:\t");
     if (B == A)  
       { PN("B as pointer to A"); }
     else  
       { PN("B as copy of  A"  ); }
     
     PN(B);
     PN("deleting from list");
     del_dstring(dsl, B);
   } 
 
 PN("searching list");
 dstring C = find_dstring( dsl , B); // if not found C will be NULL
  
 if (C)  
   {  PN("found:\t"); PN(C); }   
 else 
   { PN( "NOT FOUND"); }

 B = NULL; //safety -- but make sure there are no further refs to A

 DSFREE(C);     
 DSFREE(B);     // harmless? or seg fault?
 DSFREE(A);     

} // find_test

/**********************************************************************/
void copy_test( dstringlist dsl)
{
  int different = 0;
  dstringlist L;
  dstringlist iter , iter2;
  
  L = new_dslist();
  
  L = dstringlist_copy(dsl);

  printf( "PRINT_LIST L\n");

  print_list(L);

  iter2 = L;      
/*
struct dst_list
{
  struct list_head list;
  dstring ds;
  int no_delete;
};
*/


  //  struct list_head* lhnext = &iter2->list.next;


  /* need parens below (MACRO)  */
  list_for_each_entry(iter, &(dsl->list), list)
    {
      iter2 = list_entry( iter2->list.next, struct dst_list, list);      

      if ( dst_cmp(iter->ds , iter2->ds) != 0 )
	{
	  different = 1;
	  fprintf( stderr, "OOPS different.  [%s] != [%s] \n" , iter->ds , iter2->ds );
	}
    }


  if ( ! different ) printf("\nCOPY TEST PASSED. LIST AND COPY  HAVE IDENTICAL SIZE AND DSTRINGS.\n");

  DSLISTFREE(L);

} // copy_test


dstringlist get_list( dstring ds)
{
  dstringlist dsl = NULL; 

  dsl = dstringlist_from_dstring( ds, " ");  // copies d

  return dsl;
} // get_list

/**********************************************************************/


void print_list( dstringlist dsl)
{
  dstringlist iter = dsl;

  /* need parens below (MACRO)*/
  list_for_each_entry(iter, &(dsl->list), list)
    {
      P( iter->ds  ); P(" ");
    }
} // print_list


/**********************************************************************/

void join_test(dstringlist dsl)
{

  localdstring msg = DSLOCAL("[join_test] joining a dstringlist with a delimiter (\"|--|\")" );
  
  PN(msg);
  askforkey("press any key");  

  dstring joined = new_dstring_init("", (1<<16) );  //64K
  PD(joined);

  joined = dstringlist_join( dsl, joined, "|--|" );

  PD(joined);
  PN(joined);

  DSFREE(joined);

  askforkey("press any key");  

} // join_test

/**********************************************************************/

/*END*/


