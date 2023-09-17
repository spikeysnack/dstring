/* dstringarray.h */

/* AUTHOR:  Chris Reid <spikeysnack@gmail.com> */
/* LICENSE: Free for all purposes              */
/* COPYRIGHT: 2015 - Chris Reid                */


/* In keeping with the modality of dstrings, 
 * a dstringarray is dynamically allocated
 * and carries its size with it. 
 * This is nothing special; just an array
 * of pointers wrapped in a structure 
 * with a size data member.
 * Good programming practice dictates, however,
 * that the size of an array should not grow
 * after creation. That could be a bad thing.
 * Adding and removing dstring pointers 
 * and accessing them as an array is what
 * this stucture is all about.
 * Copying of dstringarrays should not
 * be an automatic function, so it is
 * not provided here. You are free to 
 * write that one if you need it; it should
 * take about 10 minutes to write, 3 hours
 * to debug, then 45 minutes to change your
 * mind and just copy the dstrings you want
 * into another array.
*/

/* includes and compiler directives */
#include <stdarg.h>
#include <string.h>
#include "dstring.h"

#ifndef DSTRINGARRAY_H
#define DSTRINGARRAY_H


/** 
 API HELPERS 
                       -- these are safer to use and easier to remember.

 NEW_DSARRAY(sz)         allocate a new empty dstringarray of size sz
 
 DSTRING_AT(DSA,I)       return the dstring at index (or null if not there)
 
 DSTRING_FIND(DSA,STR)   match a string and return its dstring (or null)

 ARRAY_CONTAINS(DSA,STR) true/false dstring is in array
 
 DSARRAY_FREE(DSA)       removes and deletes all dstrings and the array
                         and nulls out all main variables
	   
 DSARRAY_CLEAR(DSA)      removes and nulls dstring pointers from array for reuse

 COUNT(DSA)              returns number of occupied slots   

 ARRAY_EMPTY(DSA)        true/false all slots empty

 FULL(DSA)               true/false all slots full  

 ARRAY_CHECK(DSA)        consistency check of array and dstrings inside

*/



/* internal array structure */
struct dst_array
{
size_t size;
dstring arr[];
};


typedef struct dst_array* dstringarray;

/*end marker */
#define ARRAY_END  (NULL-1)

/* function prototypes  */
#define _INLINE_ static __inline__


_INLINE_ dstringarray new_dstring_array( size_t sz );

_INLINE_ dstringarray dstring_array_init( size_t sz, dstring ds1, ... );

_INLINE_ dstringarray dstringarray_from_dstring( const dstring ds , cstring delim );

_INLINE_ dstringarray dstring_array_from_cstring_array( const cstring* carray );

_INLINE_ void dstring_add( dstringarray dsa , dstring ds );

_INLINE_ void dstring_set( dstringarray dsa , dstring ds, size_t index); 

_INLINE_ void dstring_unset( dstringarray dsa,  size_t index);

_INLINE_ dstring dstringarray_index( const dstringarray dsa , unsigned int index);

_INLINE_ dstring dstringarray_find ( dstringarray dsa , dstring ds );

_INLINE_ dstring dstringarray_match_len ( dstringarray dsa , const char* str, size_t len );

_INLINE_ int dstringarray_cull ( dstringarray dsa );

/* utility macros */
#define DSARRAY_PRINT(DSA) do { int n;                      \
    for(n=0; n<DSA->size; n++) {                            \
      printf("%d:",n);                                      \
      if (DSA->arr[n]) { printf("\t%s", DSA->arr[n]);	    \
      if ( ! rindex( DSA->arr[n], '\n') ) printf("\n"); }   \
    }    						    \
  }while(0)


#define PRINT_DSARRAY DSARRAY_PRINT

/* perform a function on all members of array */
#define dstringarray_for_each(dsa, F)		\
  do{ int n;                                    \
    for(n=0; n< dsa->size; n++)                 \
      { F(dsa->arr[n]);}                        \
  }while(0)

#define for_each_dstringarray dstringarray_for_each
 

#define  DSARGLISTEND  ((dstring) NULL-1)

#define DSARRAY(...)   ( __extension__ ({                                   \
  dstringarray _dsa = NULL;                                                 \
  do { size_t n = 0;							    \
      dstring dsarr[] = { __VA_ARGS__ , DSARGLISTEND };  	            \
      while ( dsarr[n] != DSARGLISTEND ){n++;}		          	    \
      _dsa = dstring_array_init(n  , __VA_ARGS__ )    ;                     \
     }while(0);                                                             \
   _dsa;                                                                    \
}))


/* frees all dstrings and then itself and nulls out its reference */
#define DSARRAY_FREE(DSA)  \
  do { int n; \
       for (n=DSA->size-1; n>=0; n--) {if(DSA->arr[n]) DSFREE(DSA->arr[n]);} \
       free(DSA); DSA = NULL; \
     } while(0)



/*macro empties pointers in array but frees no memory  (no delete dstrings)*/
#define DSARRAY_CLEAR(DSA)  do { int n; \
 for (n=DSA->size-1; n>=0; n-- ) { DSA->arr[n] = NULL;} \
  } while(0)


/* get number of occupied entries */
_INLINE_ size_t dstringarray_count(dstringarray dsa )
{
  int i;
  size_t n;  

  if(!dsa) return 0;

  n = 0;
  for(i = dsa->size-1; i>=0; i--) {if(dsa->arr[i]) n++;}
  return n;  
}
#define COUNT(DSA)  dstringarray_count(DSA)



/* is it empty?*/
_INLINE_ size_t dstringarray_empty(dstringarray dsa )
{
  return COUNT(dsa)  == 0;
}
#define ARRAY_EMPTY(DSA)  dstringarray_empty(DSA)


/* is it full?*/
_INLINE_ int dstarray_is_full(dstringarray dsa )
{
  return  dstringarray_count(dsa) == dsa->size ;
}
#define FULL(DSA)  dstarray_is_full(DSA)

/* is it corrupted? */
_INLINE_ int dst_array_check(const dstringarray dsa )
{
  dstringarray p;
  size_t realsize;
  int i;

  if (!dsa) return 0;

  p = dsa;

  if (p->arr[dsa->size] != ARRAY_END ) return 0; //check bound

  for (i = p->size-1; i >=0; i --)
    {
      if(p->arr[i]) 
	{
	  if ( ! dstcheck(p->arr[i]) )
	    { 
	      fprintf(stderr, "ERROR dst_array_check  dstring at index %d fails check\n" , i);
	      return 0;
	    }
	}
    }

  realsize = 0;

  /* actual count of array*/
  while (dsa->arr[realsize] != ARRAY_END )
    {
      realsize++;
    }
    
    if( realsize != dsa->size ) return 0;

    return 1; //all checks OK

} // dst_array_check
#define ARRAY_CHECK(DSA)  dst_array_check(dsa)


/* functions */


/* allocate an array of dstring pointers with a size */

_INLINE_ dstringarray new_dstring_array( size_t sz )
{
  dstringarray da;
  
  da = calloc( 1 , sizeof(struct dst_array) + ((sz+1) * sizeof(dstring))  );  //allocate struct
  
  da->size = sz;
  da->arr[sz] = ARRAY_END;
  return da;
}
#define new_dstringarray(SZ)  new_dstring_array(SZ)
#define NEW_DSARRAY(SZ) new_dstring_array(SZ)


/* create a dstringarray from a c-style string array*/
_INLINE_ dstringarray dstring_array_from_cstring_array( const cstring* carray )
{
  size_t ca_sz = 0;
  size_t i = 0;
  dstringarray dsa = NULL;
  
  if (! carray ) return NULL;

  while( carray[ca_sz] ) ca_sz++;

  dsa = new_dstring_array( ca_sz );
  
  for (i = 0; i < ca_sz; i++)
    {
      dstring_add( dsa, new_dstring( carray[i] ) );
    }
  
  return dsa;
}  // dstring_array_init



/* varargs multi init contructor */
_INLINE_ dstringarray dstring_array_init( size_t sz, dstring ds1, ... )
{
  dstringarray new_dsa = new_dstring_array( sz );

  va_list ap;

  size_t i;

  va_start(ap, ds1);

  new_dsa->arr[0] = new_dstring(ds1);
  
  for (i = 1; i < sz; i++)
    {
      if ( new_dsa->arr[i] != ARRAY_END ) new_dsa->arr[i] = new_dstring( va_arg(ap, dstring) );
      else break;
    }      

  va_end(ap);

  return new_dsa;
}  // dstring_array_init
#define new_dstringarray_init  dstring_array_init
#define dstringarray_new_init dstring_array_init


/* add dstring to first open slot */

_INLINE_ void dstring_add( dstringarray dsa , dstring ds )
{
  size_t sz, i;
  sz = dsa->size;
  for( i = 0; i < sz ; i++)
    {
      
      if (! dsa->arr[i] )
	{
	  dsa->arr[i] = ds;
	  break;
	}    
    } 
  
} // dstring_add

 
_INLINE_ void dstring_set( dstringarray dsa , dstring ds, size_t index) 
{
  if(!dsa) return;

  if (index <  dsa->size)
    {
	  if (! dsa->arr[index] ) 
	    {
	      dsa->arr[index] = ds;
	    }
	  else
	    {
	      fprintf( stderr, "WARNING -- overwriting dstring  entry %zu (%s)\n" , index ,dsa->arr[index] );
	      fprintf( stderr, "FIX -- use dstring_unset( dstrintarray , index) first.\n" );
	      dsa->arr[index] = ds;
	    }
    }
  else
    {
      	      fprintf( stderr, "WARNING -- %zu exceeds size of array (%zu):\t nothing done.\n" , index ,dsa->size );
    }
} // dstring_set



/* index is cleared . (0) is valid index */
_INLINE_ void dstring_unset( dstringarray dsa,  size_t index) 
{
  if(!dsa) return;

  if (index <  dsa->size)
    {
     dsa->arr[index] = NULL;
    }
}// dsring_unset





/* return the dstring at index (possibly null) */

_INLINE_ dstring dstringarray_index( const dstringarray dsa , unsigned int index)
{

  if( (dsa) &&  (index < dsa->size) )
    return dsa->arr[index];
  else
    return NULL;
} // dstringarray_index

#define DSTRING_AT(DSA,I) dstringarray_index( DSA, I)



/* partial string match  (only string compare) */
_INLINE_ dstring dstringarray_match_len ( dstringarray dsa , const char* str, size_t len )
{

  size_t sz, i, slen;
  
  if ((!dsa) || (!str) || (!len)) return NULL;

  slen = strlen(str);
  
  if (len > slen) len = slen;
  
  sz = dsa->size;
  
  for( i = 0; i < sz ; i++)
    {     
      if (strncmp( str, dsa->arr[i], len ) ) return dsa->arr[i];
    } 
  
  return NULL;  
  
} // dstringarray_match_len

/*exact match */
#define dstringarray_match(DSA,STR)  stringarray_match_len( DSA, STR, UINT_MAX) 

#define DSTRING_FIND(DSA,STR) stringarray_match_len( DSA, STR, UINT_MAX)

/* true/false array contains dstring */
#define ARRAY_CONTAIN(DSA,STR)  (dstringarray_match(DSA,STR) != NULL)



/*remove duplicate pointers from array 
* returns new count of array.
*    note copies of dstrings stay 
*    as pointers do not match
*/

_INLINE_ int dstringarray_cull ( dstringarray dsa )
{
  int i, j;
  i = j = 0;

  if( (!dsa) || (COUNT(dsa) < 2) ) goto fin;

  for(i = dsa->size-1; i >=0 ;i--)
    {      
      if (!dsa->arr[i]) continue; //skip null

      for(j = i; j >= 0; j--)
	{
	  if (!dsa->arr[j]) continue; //skip null

	  if ( (i!=j) && (dsa->arr[i] == dsa->arr[j]) )
	    {
	      dsa->arr[j] = NULL; //remove dup
	    }
	} //for j
    } //for i


 fin:  
  return COUNT(dsa);
} // dstringarray_cull




/*remove duplicate dstrings from array 
* returns new count of array.
* copies of dstrings & pointers are deleted.
*/

_INLINE_ int dstringarray_cull_copies ( dstringarray dsa )
{
  int i, j;
  i = j = 0;

  if( (!dsa) || (COUNT(dsa) < 2) ) goto fin;

  for(i = dsa->size-1; i >=0 ;i--)
    {      
      if (!dsa->arr[i]) continue; //skip null

      for(j = i; j >= 0; j--)
	{
	  if (!dsa->arr[j]) continue; //skip null
	  
	  if ( (i!=j) && (dsa->arr[i] == dsa->arr[j])  )
	    {
	      dsa->arr[j] = NULL; //remove dup
	    }

	  if ( (i!=j) && (dst_cmp(dsa->arr[i],dsa->arr[j]) == 0) )
	    {
	      DSFREE(dsa->arr[j]); //remove copy
	    }
	  
	} //for j
    } //for i

  
 fin:  
  return COUNT(dsa);
} // dstringarray_cull


#define dstringarray_from_cstring(cs, delim)  dstringarray_from_dstring( new_dstring(cs) , delim)

/* create an array of dstrings 
 * from a dstring separated by delim 
 */
_INLINE_ dstringarray dstringarray_from_dstring( const dstring ds , cstring delim )
{
  dstringarray dsa = NULL;
  dstring tmp = NULL;
  dstring dnew = NULL;
  char c = '\0';
  char* str, *startp, *endp;
  size_t ilen, count, entries, strsz; 
  size_t arraysize = 0;

  if (! ds) goto fin;

  if (! delim ) delim = " ";
  
  /* use a copy */
  tmp = dstring_copy(ds);
  tmp =  dstring_minimize_mem(tmp);

  /* count items */
  ilen = dstlen(tmp);
  entries = 0;
  count   = 0;


  // Edge case: the delim is last readable char (like a .)
  if(ds[ilen-1] == delim[0] ){ ds[ilen-1] = '\0'; }

  
  while( count < ilen)
	{
	  c = tmp[count];
	  if ( c == delim[0]) { entries++; tmp[count] = '\0';}    // replace all delim with null chars
	  count++;	  
	}

  arraysize = entries+1;
     
  dsa = new_dstring_array(arraysize);

  /* string pointers */
  startp  = (char*)tmp;
  endp = (char*)tmp;
  count = 0;

  while ( count++ <= arraysize )
	{
	  startp = endp;

	  while(*endp != '\0') endp++;
	  
	  strsz = endp - startp;
	  
	  str = startp;
	  
	  	  
	  if( strsz > 0)
	    {
	      
	      dnew = dstring_new_init(str, strsz);
	      /* add items to array */
	      dstring_add( dsa , dnew );		    
	      /* skip null char */
	      endp++;
	    } // if
	  dnew = NULL;   
	} // while
	  

  dstring_free(tmp);
  
 fin:
  return dsa;

} // dstringarray_from_dstring



#endif // DSTRINGARRAY_H


/*END*/
