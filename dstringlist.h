/* dstringlist.h */

/* AUTHOR:  Chris Reid <spikeysnack@gmail.com> */
/* LICENSE: Free for all purposes              */
/* COPYRIGHT: 2015- Chris Reid                 */

/*
 * Put dstrings into a double linked list.
 *
 * The format is borrowed directly from the 
 * linux kernel implementation of linked lists
 * which is nearly atomic, ultra simple, and 
 * transparent. The driver code is in a 
 * separate file dstring_llist.h which
 * is generic and can be repurposed
 * for any kind of struct.
 * 
 * A doubly-linked list is made by embedding 
 * the list_head structure into the struct along 
 * with the object(s). 
 * Then one node is initialized as the head of the list
 * with its next and prev pointers pointing to itself. 
 * As nodes are added linked-list style
 * the last node's next points to the head in
 * circular fashion. 
 *
 * To traverse the list you simply get next 
 * until you come around to the list head again 
 * and stop there by checking (next != head ).
 *
 * (note) append_nodelete adds the dstring into 
 * the end of the list with the no_delete member set.
 * In this case the dstring is not deallocated when
 * the list is freed, but its list entry is.
 * Otherwise the dstring is freed along with the list.
 */


#ifndef  DSTRINGLIST_H
#define  DSTRINGLIST_H

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

#ifndef DSTRING_H
#include "dstring.h"
#endif

#ifndef DSTRING_LLIST_H
#include "dstring_llist.h"
#endif



/**********************************************************************/
/*  UTILS & MACROS  */
/********************/
#define _INLINE_ static __inline__

#define P(D)     if(D) printf ("%s" , D )
#define PN(D)    if(D) printf ("%s\n" , D )
#define SZ(D)    if(D) printf ("%zu : %zu : (%zu)" , dstlen(D) , dstfree(D), dst_mem(D)-16 )

#define PD(D)    do { P(D); SZ(D); }while(0) 


/**********************************************************************/



/************dstringlist structure defintion **********/

/* a traditional linux kernel-type 
 * double linked list with data payload
 */
struct dst_list
{
  struct list_head list;
  dstring ds;
  int no_delete;
};

// a type alias 
typedef struct dst_list* dstringlist;



/*****************function defintions******************/

_INLINE_ bool is_empty_dstringlist(dstringlist ptr);

_INLINE_ void print_dstringlist( dstringlist dsl);

_INLINE_ void append_dstring( dstringlist ptr, dstring dst);

_INLINE_ void append_dslist( dstringlist ptr, dstringlist ap);

_INLINE_ void append_dstring_nodelete( dstringlist ptr, dstring dst);

_INLINE_ void append_string( dstringlist ptr, const char* str, size_t len);

_INLINE_ dstringlist dstringlist_from_dstring( dstring ds , const char* delim);

_INLINE_ dstring find_dstring( dstringlist ptr, dstring ds );





/* simple printout of dstrings in a row with a space between */

_INLINE_ void print_dstringlist( dstringlist dsl)
{
  if ( (!dsl)  || is_empty_dstringlist(dsl) ) return;

  dstringlist iter = dsl;

  list_for_each_entry(iter, &(dsl->list), list)
    {
      P( iter->ds  ); P(" ");
    }
} // print_dstringlist




/* concatenate localdstrings into one new one */


_INLINE_ dstringlist dstringlist_copy( dstringlist ptr);

_INLINE_ dstring dstringlist_join( dstringlist ptr, dstring ds, const cstring sep );

_INLINE_ void del_dstring( dstringlist ptr, dstring ds);

_INLINE_ void dstringlist_free( dstringlist ptr);

/*  MACROS */

#define DSLISTFREE(DSL)  (dstringlist_free(DSL), (DSL) = NULL)

#define DSLIST_CREATE() ( __extension__ ({                                \
   dstringlist DSL = (dstringlist)calloc(1, sizeof(struct dst_list));     \
    INIT_LIST_HEAD(&DSL->list);                                           \
DSL; }))


#define  DSARGLISTEND  ((dstring) NULL-1)

#define DSLIST(...)   ( __extension__ ({                       \
  dstringlist _dsl =  DSLIST_CREATE();					        \
  do { size_t n = 1;										    \
	dstring dsarr[] = { NULL, ##__VA_ARGS__ , DSARGLISTEND };	\
  while ( dsarr[n] != DSARGLISTEND )                            \
	{                                                           \
      if ( is_dstring(dsarr[n]) ){ append_dstring(_dsl, dsarr[n]);}     \
      else {append_dstring(_dsl, new_dstring(dsarr[n]));}				\
	  n++;                                                              \
    }								                            \
  }while(0);                                                    \
  _dsl;                                                         \
}))

#define new_dslist  DSLIST
#define new_dstringlist  DSLIST




/***************function implementations***************/

/* add dstring to list (deletable) */
_INLINE_ void append_dstring( dstringlist ptr, dstring ds)
{
  dstringlist dsl = NULL;

  if ((!ptr) || (!ds) )
	{
	  //	  fprintf(stderr, "append_dstring: dstringlist.h line 112:  nullpointer\n");
	  return;
	} 

	dsl = new_dslist();

  if(!dsl) 
    {
    perror("calloc on dls failed");
    exit(1);
    }
  
  dsl->ds = ds;
  dsl->no_delete = 0;

  list_add_tail( &(dsl->list), &(ptr->list) );
} // append_dstring

#define append_dstring_multi( L , ... )  ( __extension__ ({     \
  do { size_t n = 1;										    \
	dstring dsarr[] = { NULL, ##__VA_ARGS__ , DSARGLISTEND };	\
    while ( dsarr[n] != DSARGLISTEND )                          \
	  {  append_dstring( L, dsarr[n]);  n++; }					\
  }while(0);                                                    \
}))   


/* add dstring to list (non-deletable) */
_INLINE_ void append_dstring_nodelete( dstringlist ptr, dstring ds)
{
  dstringlist dsl;

  if ((!ptr) || (!ds) ) return;

  dsl = new_dslist();

  if(!dsl) 
    {
    perror("calloc");
    exit(1);
    }
  
  dsl->ds = ds;
  dsl->no_delete = 1; // will not be freed when list is freed

  list_add_tail( &(dsl->list), &(ptr->list) );
} // append_dstring_nodelete


_INLINE_ void append_dslist( dstringlist ptr, dstringlist ap)
{
  if (!(ap && ptr) ) return;

  list_splice_tail_init( &(ap->list), &(ptr->list) );

}


/* append anonymous dstring to list */
_INLINE_ void append_string( dstringlist ptr, const char* str, size_t len)
{
  dstringlist dsl;

  if ((!ptr) || (!str) || (!len) ) return;

  dsl = new_dslist();

  if(!dsl) 
    {
    perror("calloc");
    exit(1);
    }
  
  dsl->ds = dstring_new_init( str, len ) ;

  dsl->no_delete = 0;

  list_add_tail( &(dsl->list), &(ptr->list) );

} // append_string



/* create a linked list of dstrings 
 * from a dstring separated by delim 
 */
_INLINE_ dstringlist dstringlist_from_dstring( dstring ds , const char* delim)
{
  dstringlist dsl = NULL;
  dstring tmp = NULL;
  char c = 0;
  char* str, *startp, *endp;
  size_t ilen, count, entries, strsz;  
  
  dsl = new_dslist();

  if (! delim ) delim = " ";
  
  if (! ds) goto fin;

  /* use a copy */
  tmp = dstring_copy(ds);
  tmp =  dstring_minimize_mem(tmp);

  /* count items */
  ilen = dstlen(tmp);
  entries = 0;
  count   = 0;

  while( count < ilen )
	{
	  c = tmp[count];
	  if ( c == delim[0]) { entries++; tmp[count] = '\0';}    // replace all delim with null chars
	  count++;	  
	}

  /* string pointers */
  startp  = (char*)tmp;
  endp = (char*)tmp;
  count = 0;

  while ( count++ <= entries )
	{
	  startp = endp;

	  while(*endp != '\0') endp++;
	  
	  strsz = endp - startp;

	  if( strsz)
		{
		  /* allocate memory for new string */
		  str = calloc( strsz , sizeof(char) ); 
	  
		  /* copy string data */
		  str = strncpy( str, startp , strsz );
		  
		  /* safety */
		  str[strsz] = '\0'; 

		  /* add items to list */
		  append_dstring( dsl, new_dstring(str) );

		  free(str);
		  /* skip null char */
		  endp++;
		} // if
	  
	} // while
	  
  dstring_free(tmp);
  
 fin:
  return dsl;

} // dstringlist_from_dstring


/* return a dstring from a list 
 * separated by a separator string
 * such as " " or "." or "/".
 */
_INLINE_ dstring dstringlist_join( dstringlist ptr, dstring ds, const cstring sep )
{
  dstring dsep = NULL;
  dstringlist iter = NULL;
  int nosep = 0;

  
  if ((!ptr) || (!ds)) return NULL;
    
  if (sep) 
	dsep = new_dstring(sep);
  else 
	nosep = 1;
  
  /* free all deletable dstrings from list */
  list_for_each_entry(iter, &(*ptr).list, list)
	{
	  //      ds = dstring_fast_append( ds, iter->ds);
	  ds = dstring_append( ds, iter->ds);
	  
	  if ( (!nosep)
		   && (! list_last_entry( &(iter->list),  &(*ptr).list) ) )
		//              ds = dstring_fast_append(ds, dsep );      
		ds = dstring_append(ds, dsep );
	}
  

  dstring_minimize(ds); // clean up extra memory
  //  dstring_minimize_mem(dsl); // clean up extra memory


return ds;

} // dstringlist_join





/* remove all items from the dstringlist */
_INLINE_ void dstringlist_free( dstringlist ptr)
{
  struct list_head *p = NULL, *n = NULL;
  dstringlist  tmp = NULL;
  dstringlist  iter = ptr;

  if (!ptr) return;

  /* free all deletable dstrings from list */
  list_for_each_entry(iter, &(*ptr).list, list)
    {
      if (! iter->no_delete )
		{
		  dstring_free(iter->ds);	  
		}
	  iter->ds = NULL;
	  
    }    
  iter = NULL;


  list_for_each_safe(p, n, &(*ptr).list) 
	{
	  /* my points to each my_struct in the list */
	  tmp = list_entry(p, struct dst_list, list);
	  
	  list_del(&tmp->list);
	}

#ifdef DEBUG
  if( ! list_empty(&(*ptr).list) )
	fprintf( stderr, "[dstringlist_free]ERROR LIST NOT EMPTY \n" );
#endif

  free(ptr);

} //dstringlist_free


_INLINE_ bool is_empty_dstringlist(dstringlist ptr)
{
  return   list_empty(&(*ptr).list);
}


/* return dstring if it is in the list */
_INLINE_ dstring find_dstring( dstringlist ptr, dstring ds )
{

  dstringlist iter;
  dstring found = NULL;
  
  if( (!ptr) || (!ds) )  return NULL;

  /* clear all dstrings from list */
  list_for_each_entry(iter, &(*ptr).list, list)
    {
	  found = iter->ds;
	  if (found == ds ) return found; 
    }     

  return NULL;
} // find_dstring 


/* remove item from list */
_INLINE_ void del_dstring( dstringlist ptr, dstring ds)
{
  dstringlist iter;
  dstring found;

  if( (!ptr) || (!ds) )  return;

  /* check all dstrings in list for ds */
  list_for_each_entry(iter, &(*ptr).list, list)
    {
	  found = iter->ds;
	  if( found == ds ) list_del(&iter->list);	  
    }     

} // del_string




/* the nodelete flag gets copied too */
_INLINE_ dstringlist dstringlist_copy( dstringlist ptr)
{
  dstringlist dsl;
  dstringlist iter;
  dstring      dscpy;

  if(!ptr)  return NULL;
  
  dsl = calloc(1, sizeof(struct dst_list) );
  INIT_LIST_HEAD(&(dsl->list) );
  //dsl->ds = dstring_copy(ptr->ds);  

 /* check all dstrings in list for ds */
  list_for_each_entry(iter, &(*ptr).list, list)
    {
	  dscpy= dstring_copy(iter->ds);
 	  append_dstring( dsl, dscpy);
   }     

  return dsl;
} // dstringlist_copy


#endif // DSTRINGLIST_H


/*END*/



/* Local Variables: */
/* mode: c          */
/* mode: font-lock  */
/* tab-width:4      */
/* End:             */

