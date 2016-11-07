/*dsdictionary.h*/

/** a sample program that implements a simple dictionary
 *  using dstrings and linked lists.
 * A dictionary entry is just a 2-tuple of key:val 
 * with the list (itself a tuple of last:next) to 
 * make it listable.
 * todo: implement subset function to return mini-dic of only selected entries.
 *       cull function to eliminate duplicate identical entries
 *       embed dic name into list?
 *       num_entries? num_results?
 *       read/write files?
 *       partial val matches?  -->  dic val_contains(dic, cstring) 
 *       multi-key discrimination (database)
 */


#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dstring.h"
#include "dstringlist.h"


#define P(D)      if(D) printf ("%s" , D )
#define PN(D)     if(D) printf ("%s\n" , D )

typedef struct DSDIC_ENTRY
{
  struct list_head list;
  dstring key;
  dstring value;
}dsdic_entry;

typedef dsdic_entry* dic_entry;

typedef dsdic_entry* dsdictionary;

/* function defs */

static inline dic_entry new_dic_entry( const cstring k, const cstring v);

static inline void add_entry( dsdictionary dic , dic_entry entry);
static inline void del_entry( dic_entry entry);

static inline void delete_dic(dsdictionary dic);
static inline dsdictionary new_dic(void);
static inline dstring val_for_key(dsdictionary dic  , const cstring key);
static inline dstringlist all_vals_for_key( dsdictionary dic, const cstring key);
static inline void dump_dic(dsdictionary dic);

typedef unsigned long long ULL;

static inline ULL hash_key(const cstring key, ULL len, ULL hash);


/* dump the dic contents to the screen */
static inline void dump_dic(dsdictionary dic)
{
  dic_entry iter = NULL;

  list_for_each_entry(iter, &(dic->list), list)
    {
      P("{ ");
      P( iter->key  ); 
      P("\t:\t");
      P( iter->value  ); 
      PN(" }");
      
      }
}  // dump_dic


/* add a new key/value pair of strings to the dic 
 * strings are copied into new dstrings
 */
static inline dic_entry new_dic_entry( const cstring k, const cstring v)
{
  dic_entry dsde = (dic_entry) calloc(1, sizeof(dsdic_entry));
  INIT_LIST_HEAD(&(dsde->list));
  
  if ( k && v)
    {
      dsde->key   = new_dstring(k);
      dsde->value = new_dstring(v);
      return dsde;
    }
  else
    {
      free(dsde);
      return NULL;
    }
}




/* attach a dic_entry to the end of the dic */
static inline void add_entry( dsdictionary dic , dic_entry entry)
{
  list_add_tail( &(entry->list), &(dic->list)) ;
}


/* delete dic_entry  recover memory*/
static inline void del_entry( dic_entry entry)
{
  dic_entry tmp = entry;
  list_del( &(entry->list) );
  entry = NULL;
  
  DSFREE(tmp->key);
  DSFREE(tmp->value);
  free(tmp);
}


/* destroy dictionary 
 * delete all entries, 
 * recovering allocated memory
 */
static inline void delete_dic(dsdictionary dic)
{
  struct list_head *p = NULL, *n = NULL;      // iteration pointers
  dic_entry  tmp = NULL;                      // entry pointer

  list_for_each_safe(p, n, &(dic->list) )
    {
      tmp = list_entry(p, dsdic_entry, list); // get ptr to dsdic_entry struct;
      DSFREE(tmp->key);                       // release dstring
      DSFREE(tmp->value);                     // release dstring
      list_del(&tmp->list);                   // uncouple (remove) from list
      free(tmp);                              // release dsdic_entry struct
    }

#ifdef DEBUG
  if( ! list_empty( &(dic->list) ) )
    fprintf( stderr, "[del_dic] ERROR LIST NOT EMPTY \n" );
#endif
  
  free(dic);                                // free head (list frees too)

  dic = NULL;                               // clean pointer
} // delete_dic




/* create a new empty dictionary 
 * list head only (contains no key/val) 
 */
static inline dsdictionary new_dic(void)
{
  dsdictionary dic = (dsdictionary) calloc(1, sizeof(dsdic_entry));
  INIT_LIST_HEAD(&(dic->list));
  return dic;
}


/* return a new dstring for given val string
 * returns only first match
 */
static inline dstring val_for_key(dsdictionary dic  , const cstring key)
{
  dstring dkey   = NULL;
  dstring dval   = NULL;
  dstring found  = NULL;
  dic_entry iter = NULL;
 
  if ( (!dic) || (!key) ) return NULL;

  dkey = new_dstring(key);
    
  /* search keys (linear) until found or end reached*/
  list_for_each_entry(iter, &(dic->list), list)
    {
      found = iter->key;

      if ( dst_cmp( found , dkey) == 0)
	{
	  dval = iter->value;
	  break;
	}
      //      fprintf(stderr ," found {%s}\t{%s}\n", found, iter->value);    
      //      fprintf(stderr ," dval {%s}\n", dval);    
    }

  DSFREE(dkey);

  if( dval )  
    return new_dstring(dval);
  else
    return NULL;
} // val_for_key


/* return a new dstringlist of values for given key 
 * returns all matches as a dstringlist
 */
static inline dstringlist all_vals_for_key( dsdictionary dic, const cstring key)
{
  dstringlist dsresults_list = NULL;
  dstring dkey   = NULL;
  dstring dval   = NULL;
  dstring found  = NULL;
  dic_entry iter = NULL;
 
  if ( (!dic) || (!key) ) return NULL;

  dkey = new_dstring(key);

  dsresults_list = new_dslist();
    
  /* search keys (linear) until found or end reached*/
  list_for_each_entry(iter, &(dic->list), list)
    {
      found = iter->key;

      if ( dst_cmp( found , dkey) == 0)
	{
	  dval = iter->value;
	  append_dstring_nodelete( dsresults_list, dval);
	}
      //      fprintf(stderr ," found {%s}\t{%s}\n", found, iter->value);    
      //      fprintf(stderr ," dval {%s}\n", dval);    
    }

  DSFREE(dkey);

  if ( ! is_empty_dstringlist(dsresults_list) )
    return dsresults_list;
  else
    {
      DSLISTFREE(dsresults_list);
      return NULL;
    }

} //all_vals_for_key


// generate a hash for an entry
typedef unsigned long long ull;

static inline ULL hash_key(const cstring key, ULL len, ULL hash)
{
  ULL   i;
  
  for (i=0; i<len; ++i)
    {
      hash += key[i];

      hash += (hash << 10);

      hash ^= (hash >> 6);
    }

  hash += (hash << 3);

  hash ^= (hash >> 11);

  hash += (hash << 15);

  return hash;
} 



/*END*/
