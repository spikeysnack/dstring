/* dhtest.c*/

/* test rig for creating dsdictionaries */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <search.h>
#include "dstring.h"
#include "dstringarray.h"


typedef struct _DSH_ENTRY
{
  dstring key;
  dstring data;
} dsh_entry;


/* a standard array of c-style strings */
cstring data[] = { "alpha", "bravo", "charlie", "delta",
		 "echo", "foxtrot", "golf", "hotel", "india", "juliet",
		 "kilo", "lima", "mike", "november", "oscar", "papa",
		 "quebec", "romeo", "sierra", "tango", "uniform",
		 "victor", "whisky", "x-ray", "yankee", "zulu"
};

  //  dsa = dstring_array_from_cstring_array(data);

#define INT2VOIDP(i) (void*)(uintptr_t)(i) /* quiets compiler warnings  and is more expressive*/

/*
#ifdef __USE_GNU
// Data type for reentrant functions. 
struct hsearch_data
{
  struct _ENTRY *table;
  unsigned int size;
  unsigned int filled;
};
*/

#define HASHTABLE struct hsearch_data

int main(int argc , char** argv, char** env)
{

/* an array of dstrings  (26)*/
  dstringarray dsa1 = DSARRAY( "alpha", "bravo", "charlie", "delta",
			      "echo", "foxtrot", "golf", "hotel", "india", "juliet",
			      "kilo", "lima", "mike", "november", "oscar", "papa",
			      "quebec", "romeo", "sierra", "tango", "uniform",
			      "victor", "whisky", "x-ray", "yankee", "zulu"
			      );

/* another array of dstrings (27)*/
  dstringarray  dsa2 = DSARRAY( "aardvark" , "bear" , "cat", "dog", "elephant", "fox", "gnu", "horse",
				"ibix", "jackass" , "koala", "llama" , "macoque", "nematode", "ocelot" ,
				"picachu" , "quiqui" , "rat", "snake", "tarantula", "urdu", "velociraptor", 
				"wapiti", "xanthian", "yak", "zephyr", "unknown");

 

  /*
  DSARRAY_PRINT(dsa1);
  DSARRAY_PRINT(dsa2);
  */

  HASHTABLE* htab = NULL;
  ENTRY e, *ep = NULL;
  int i, result;

  //dynamically create an empty hash table struct 
  htab=calloc(1,sizeof(HASHTABLE));

  //dynamically create a single table of 30 elements 
  result=hcreate_r( 30, htab );

  //  hcreate(30);

  // add array 1 to hashtable with array2 as keys   

  for (i = 0; i < COUNT(dsa2); i++) 
    {
      e.key = DSTRING_AT(dsa2, i);

      /* data is just an integer here, 
	 but could be a pointer to any data or struct */
      e.data = DSTRING_AT(dsa1, i); 
      
      result = hsearch_r(e, ENTER, &ep, htab);
      //    ep = hsearch(e, ENTER);

      /* there should be no failures */
      if (ep == NULL) 
	{
	  fprintf(stderr, "entry failed\n");
	  exit(EXIT_FAILURE);
	}
    } // for

  for (i = 0; i < COUNT(dsa2); i++) 
    {
      /* print two entries from the table, and
	 show that two are not in the table */
      e.key = DSTRING_AT(dsa2, i);

      result = hsearch_r(e, FIND, &ep, htab);
      
      printf("%9.9s -> %9.9s:%s\n", e.key,
	     ep ? ep->key : "NULL", ep ? (dstring)(ep->data) : 0);
    } //for

  //clean up
  hdestroy_r(htab);

  DSARRAY_FREE(dsa2);
  DSARRAY_FREE(dsa1);

  exit(EXIT_SUCCESS);
}


/*END*/

