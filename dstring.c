/* dstring.c */

/* AUTHOR:  Chris Reid <spikeysnack@gmail.com> */
/* LICENSE: Free for all purposes              */
/* COPYRIGHT: 2015- Chris Reid                 */



/* this files is compiled into 
   dstring.o and libdstring.so */


#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include "dstring.h"


//static bool fastappend = NO;


/**************** dstring utils ************************/ 


/* compare dstrings  */
int dst_cmp(const dstring d1, const dstring d2) 
{
  size_t s1, s2, min;
  int cmp;
  
  if (  d1 && !d2 ) return  1;
  if ( !d1 &&  d2 ) return -1;
  if ( !d1 && !d2 ) return  0;


  s1 = dstlen(d1);
  s2 = dstlen(d2);

  min = (s1 < s2) ? s1 : s2;

  cmp = memcmp(d1, d2, min); // check up to shorter string length.

  if (cmp == 0)  //check len
    {
      if ( s1 > s2 ) return  1;
      if ( s2 > s1 ) return -1;
    }

  return cmp;
} // dst_cmp




/* longer dstring check 
 use dstcheck(ds) for quick checks */

int dstring_check(const dstring ds)
{
  DSTPTR(ds);

  if (!ds ) return 1; // degenerate case 

  /* first quick check */
  if (! dstcheck(ds) ) return 0;


  if ( strlen(ds) != d->len )
    {
      fprintf(stderr , "ERROR  dstring_check  strlen(ds):%zu != d->len:%zu \n", strlen(ds) ,d->len );
      return 0;
    }

  if ( ds[d->len] != '\0' ) 
    {
      fprintf(stderr , "ERROR  dstring_check  unterminated string or memory corruption at %p\n", &ds[d->len] );
      return 0;
    }   

  return 1;
} // dstring_check





/**************** create/free dstrings ************************/ 


/* create a dstring with or without an initial c-style string*/
dstring dstring_new_init(const void *p, size_t len) 
{
  
  dstptr d = NULL;
  size_t sz = 0;
  size_t psz = 0;

  //  assert( len);
  
  if (p)
    {
      psz = strlen(p);      
  
      if (psz > len) len = psz;
    }
  else
    {
      if (!len) len = 16;
    }


  if (len > DST_MAX ) len = DST_MAX;
  /*
#ifdef ENV64BIT 
  sz = sizeof(*d) + len + 1;
#endif

#ifdef ENV32BIT
  sz = sizeof(*d) + 8 + len + 1;
#endif
  */
  sz = sizeof(*d) + len + 1;
 
  if ( sz%16 ) sz += ( 16 - (sz%16) ); 

  d = calloc( sz, sizeof(char) );

  if (!d) 
    {
      fprintf( stderr, "CALLOC ERROR [new_dstring_init] (calloc failed) attempt to allocate %zu bytes \n", sz );
      exit(-1);
    }

  if (p) 
    {      
      if (psz) memcpy(d->str, p, psz);
      d->str[psz] = '\0'; //safety 
      d->len = psz;
      d->free =  sz - (sizeof (*d) + d->len);
    }
  else 
    {
      d->len = 0;
      d->free =  sz - sizeof(*d);
    }
  
  return (dstring) d->str;
} // new_dstring_init



/* a dstring struct with no char data and size 0 */
dstring  emptydstring(void) 
{ 
  return new_dstring_init( "" , 16 ); 
} // emptydstring


/* dstring copy of a c-style string  */
dstring  new_dstring(const cstring cs) 
{
  size_t slen = 0;

  if ( cs )
    {
      slen = strlen(cs);
      return new_dstring_init(cs, slen);
    }
  else 
    return emptydstring();
} // new_dstring



/* copy a dstring */
dstring dstring_copy(dstring ds)
{ 
  assert(ds); // throw error if null
  return new_dstring_init(ds , dstlen(ds) );
} // dstring_copy
#define dsdup   dstring_copy



#define localcheck(DS)  (  ((dst_mem(DS) % 16) != 0) &&  ((dst_mem(DS) % 31) == 0 ) )


/* Free a dstring. release its memory. */
void dstring_free(dstring ds)
{

  if ( ! ds ) return;  

  if ( localcheck(ds) ) 
    {
      fprintf( stderr, "ERROR [dstring_free] localdstring cannot be freed. it will free automatically at end of function.\n" );
      return;
    } 
  
  dstring_clear(ds);

  free( ds - sizeof(struct dst) );
  ds = NULL;
} // dstring_free


/* clear the data but keep the memory and the struct 
 * this just zeroes out the first byte and keeps track
 * of the bytes in the dst->free.
 */
void dstring_clear(dstring ds) 
{
  if (!ds ) return;
  DSTPTR(ds);
  d->free =  d->free + d->len;
  d->len = 0;
  memset(d->str , 0, d->free ); // zero out string memory 
  d->str[d->free] = '\0';
}



/* clear the free area only */
void dstring_clear_free(dstring ds) 
{
   if (!ds ) return;
  DSTPTR(ds);

  void* p = ds + ((d->len + 1) * sizeof (char));
 
 if (d->free)  memset( p , 0, d->free-1 ); // zero out string memory  
} // dstring_clear_free



/* return copy of the first n letters of ds */
dstring dstring_truncate( dstring ds, size_t n )
{
  dstring dscopy = dstring_copy(ds);

  if (n < dstlen(ds) ) 
    {
      dscopy[n] = '\0';
      dscopy = dstring_minimize_mem(dscopy);
    }
  
  return dscopy;
} // dstring_truncate
#define dsndup  dstring_truncate



/* copy of ds from n to m letters  
* if ds = "ABCDEFGHIJ" then 
* dstring_slice( ds, 5, 10) returns  
* "EFGHIJ" (6 letters) -> ds[4]--ds[9]
* Use dstlen(ds) for a slice 
* start to end of the string
* if you  don't know the exact length. 
*/ 

dstring dstring_slice( dstring ds, size_t start, size_t end )
{
  dstring dscopy;
  size_t dlen, sz;
  
  if (!ds) return NULL;

  dlen = dstlen(ds);
 
  if (end >= dlen) end = dlen;

  if (start == 0) start = 1; // 1 is first letter, not index
  
  if ( (end <= start) || ( start >= dlen) )
    {
      fprintf( stderr, "ERROR start (%zu) after end (%zu)Logic error\n", start, end );
      exit(-1);
    }

  sz = end - start;

  dscopy = dstring_new_init( ds+(start-1) , sz );   
  dscopy = dstring_truncate( dscopy, end );
  
  return dscopy;
} // dstring_slice
/*alias*/
#define dstring_substr dstring_slice 



/* checks the cstring in the dstring and updates
 * the used and free count */
void dstring_minimize(dstring ds) 
{
  size_t slen = 0;
  //  size_t diff = 0;
  size_t total = 0;
  if (!ds ) return;

  DSTPTR(ds);  
  total = dstlen(ds) + dstfree(ds);

  slen = strlen(ds) + 1;
  d->len = slen;
  d->free  = total - d->len;


  if ( ! dstcheck(ds) ) 
    {
      fprintf(stderr , "ERROR -- [dstring_minimize] quickcheck failed. %zu : %zu \n", dstlen(ds) , dstfree(ds) );
      exit(-1);
    }

    dstring_clear_free(ds); 
}    // dstring_minimize






/* reduces dst->len to len or strlen( dsr->str)
   whichever is longer. adds it to free. no memory reduction */
dstring dstring_shorten( dstring ds, size_t len )
{
  size_t strsize;
  size_t diff;

  if (! ds ) goto fin;
  DSTPTR(ds);  

  if (len > d->len )   goto fin; // illogical 

  strsize = strlen(ds);

  if (len < strsize ) len = strsize; // safety
    
  diff = d->len - len;

  d->len = len;

  d->free = d->free + diff ;

 fin:
  return ds;

} // dstring_shorten



/* increases dst->len to len up to len + free;
 * whichever is shorter. free is decreased by same amount. 
 * no memory add unless free < len
 */
dstring dstring_lengthen( dstring ds, size_t len )
{

  if ( !(ds && len) ) goto fin;

  len = (len %16)?  len  : len + (16 - (len%16));

  DSTPTR(ds);  

  if ( len > d->free )
    {  
      ds = dstring_addmem( ds , len );
      
      if ( !ds )
	{
	  fprintf( stderr, "LENGTHEN FAILED\n");
	  exit(-1);
	}

    }



  dstring_clear_free(ds);       


  d->free = d->free - len;
  
  d->len  = d->len  + len;


 fin:
  return ds;

} // dstring_lengthen



/* makes the dstring larger 
 * without changing the string inside. 
 * adds only to d->free.
 * always at least doubles d->free.
 */

dstring dstring_addmem(dstring ds, size_t addlen) 
{
  dstptr newd = {0};
  size_t oldmem;
  size_t newlen;
  size_t alloc;

  if ( (!ds) || (!addlen) ) goto fin;

  DSTPTR(ds);  
 
  oldmem = dst_mem(ds);
  newlen = d->len + d->free + addlen;
  alloc = newlen + (sizeof *newd) +1;

  if (alloc%16) alloc += (16 - (alloc%16)); //align 16

  if ( alloc > DST_MAX )  alloc = DST_MAX; // dstring limit  

  newd =  realloc( d, alloc  ); // get more mem

  if (newd == NULL)
    {
      fprintf(stderr, "ERROR [dstring_admem]  (realloc failed) from %zu to %zu \n", oldmem , alloc  );
      exit(-1);
      goto fin;
    }
  else
    {
      d = newd;
      d->free = alloc - ((sizeof *d) + d->len); 
      memset( (d->str+d->len+1), 0, d->free-1);
      ds = d->str;
    }
  
 fin:
  return ds;
}// dstring_addmem 




/* set memory size to minimum necessary to house the 
 * string and the null at the end. leaves no free mem. 
 */
dstring dstring_minimize_mem(dstring ds) 
{
  size_t strsize;
  size_t oldmemsize;
  size_t alloc;
  dstptr dtmp = NULL;


  if (! ds) goto fin;
  
  strsize = strlen(ds);
  
  DSTPTR(ds);    
  
  oldmemsize = dst_mem(ds);
  
  alloc = (sizeof *d) + strsize + 1;

  if ( alloc%16 ) alloc += (16 - (alloc%16)); //align 16

  dtmp = realloc(d , alloc ); /* releasing memory should never fail , btw. */
  
  if (dtmp == NULL)
    {
      fprintf( stderr, "DEBUG [dstring_minimize_mem (realloc failed) from %zu to %zu !\n", oldmemsize , alloc );
      goto fin;
    }
  else
    {
      d = dtmp;
      ds = d->str;
      d->len=strsize;
      d->free = alloc - (sizeof(*d) + d->len);
      memset (d->str+d->len+1 , 0, d->free);
    }

  
 fin:
  return ds;
} // dstring_minimize_mem
 


/* tries to adjust the size of the string by sz,
 * but no more than len + free. 
 * Use addmem instead to get more memory space.
 */
void dstring_free_to_len( dstring ds, size_t sz )
{
  size_t diff;

  if (! ds ) return;

  DSTPTR(ds);  

  diff = sz;

  if ( diff >= d->free) diff = d->free;
  
  d->len += diff;
  d->free -= diff;
     
} // dstring_free_to_len



/* returns a dstring changed larger or adjusted up by newlen
 * bytes. If more is requested than len+free, memory is added
 * to free, then transferred to len. Otherwise len is increased
 * and free is decreased by newlen.
 * You get back the string adjusted so that it can be assigned
 * a string of at least newlen bytes.
 */
dstring dstring_resize ( dstring ds , size_t newlen )
{
#ifdef __SSIZE_T_TYPE
  ssize_t diff; // need signed 
#else
  long diff;
#endif

  dstring dtmp   = NULL;
  
  if (! ds ) goto fin;

  DSTPTR(ds);  

  diff = newlen - ( d->len + d->free );  // total diff (+-)
  
  if ( diff > 0 ) // need more memory
    {
      dtmp = dstring_addmem( ds, diff  );
      if (dtmp == NULL ) // realloc could fail
	{
	  return ds;
	}
      else
	{
	  ds = dtmp;	  
	  dstring_free_to_len( ds , ( newlen - d->len) ); // transfer from free	
	}
    }
  else // have enough mem already
    {
      if (newlen > d->len) 
	{
	  ds = dstring_lengthen( ds , (newlen - d->len) );
        }

      if (newlen < d->len )
	{
	  ds = dstring_shorten( ds, (d->len - newlen) );
	}
    }// else

  fin:
  return ds;

} // dstring_resize


/* append a string to the new dstring. 
 *
 * (A note on the implementation of this function).
 * 
 * Even though the dstring is specified as
 * const, it is mutable because throughout
 * the function it is not reassigned to another
 * memory location.
 * The memory location returned may change however. 
 * The idea is that what is returned
 * is always either the same block if there is
 * space in the dstring for the append, 
 * and an entirely new,  copied block if not. This way, the
 * variable ds is not subject to reallocation outside
 * its context of origin, which can and does
 * lead to problems at least when you try to free
 * the allocated memory blocks later. I have tried 
 * over ten different append strategies, and only two
 * would not leak or crash : 
 * 1. always copying: 
 *    memory intensive
 *    slow on large dstrings ( minutes!) 
 *    kernel would kill any append over ~236K chars
 * 2. full pre-allocation prior to appends:
 *    only good for 100% known sizes at compile time.
 *    super fast but memory hog.
 *
 * So I incorporated parts of both along with a 
 * progressive memory allocation strategy. 
 *
 * Each time there is not enough free dstring memory, 
 * append copies into a string with its size 
 * shifted right 1 place:
 * (16) =0x0010 --> (256) =0x0100 --> (4096) =0x1000 --> (65536)  =0x10000 etc.
 * (32) =0x0020 --> (512) =0x0200 --> (8192) =0x2000 --> (131072) =0x20000 etc.
 *
 * This limits the number of calls to calloc to O(logN)
 * of course based on the idea that appends will result
 * in linear size increases.  
 * The memory expansion algorithm I settled upon is
 * controlled here by setting the growth factor constant
 * as the amount of bytes to shift right. 
 * 1 as the default growth factor requires 4 allocation
 * calls for 100,000 appends of up to 8 characters,
 * starting with 64k dstrings memory. Excess free space
 * is freed with a single call to dstring_minimize_mem
 * after all appends in the loop are done.
 *
 *
 * Return a new dstring appended if space needs allocating 
 * or the same one appended if there is already enough. 
 */
#define GROWFACTOR 1 // should be enough to limit # of mallocs to O(log n)

dstring  dstring_fast_append( const dstring ds, const cstring str )
{
  dstring appended = NULL;
  size_t slen = 0;
  size_t dlen = 0;
  size_t dfree = 0;
  size_t dmem = 0;
  size_t newmem = 0;
  
  //NULL ptr
  if (!ds)
    {
      appended = new_dstring(str);
      goto fin;
    }

  // bad input
  if ( ! is_dstring(ds) )
    {
      fprintf(stderr , "ERROR [dstring_fast_append] ds in not a dstring\n");
      appended = NULL;
      goto fin;
    }

  if (! str )
    {
      //      appended = dstring_copy(ds);
      appended = ds;
      goto fin;
    }


  if (! appended) appended = ds;


  slen = strlen(str);

  dlen  = dstlen(appended);

  dfree = dstfree(appended);

  dmem = dst_mem(appended);

  newmem = dmem << GROWFACTOR;

  newmem = (newmem%16)? newmem : newmem + ( 16 - (newmem%16));

  //  fprintf( stderr, "DEBUG %s  %zu:%zu [%zu] (%zu) \n", appended, dlen, dfree , dmem , newmem);

  if ( dfree < (slen+1) )
    {
      appended= dstring_addmem( appended , newmem );
      dfree = dstfree(appended);
    }

  
  if ( dfree >= slen)
    {


 
      memcpy(appended+dlen , str , slen);
  
      dstptr d = (dstptr) (appended - sizeof(*d));

      d->len  += slen;
      d->free -= slen;
    }


 fin:
  return appended;  
} // dstring_fast_append


/* always returns a new dstring */
dstring dstring_append( const dstring ds, const cstring str )
{
  dstring dscopy;

  if (!ds) 
    dscopy = emptydstring();
  else  
    dscopy = dstring_copy(ds);

  return dstring_fast_append( dscopy , str);
} // dstring_append


dstring dstring_append_multiple( int n, dstring dsa, const dstring dsb, ...)
{
  int i;

  dstring appended = new_dstring_init(dsa, dstlen(dsa) * 5);
  dstring tmp = NULL;
  dstring end = new_dstring(dsb);

  appended = dstring_append( appended, end );

  va_list args;
  va_start(args, dsb);

  for(i = n-2  ; i > 0 ; i-- )
    { 
      end = va_arg(args, dstring);
      tmp = new_dstring(end);
      if ( tmp)  appended = dstring_append( appended, tmp );
      tmp = NULL;
    }

  va_end(args);

  appended = dstring_minimize_mem( appended );

  return appended;
}


/* append a dstring to another dstring*/
dstring  dstring_append_dstring( dstring dsa, const dstring dsb )
{
  if (!dsb ) 
    return dsa;
  else  
    return dstring_append( dsa , dsb );

} // dstring_append_dstring




/* dsscanf reads from variables into a dstring,
   either makeing a new one (1024 bytes) or
   limited to the dstlen of an existing one.
*/

#define DSSCAN_MAX 1024

#if defined(__GNUC__)

inline __attribute__ ((__gnu_inline__)) int dsscanf (dstring ds, const cstring fmt, ...)
  __attribute__ ((format (scanf, 2, 3))); 


inline  __attribute__ ((__gnu_inline__)) int dsscanf( dstring ds , cstring fmt, ...)
{
  int result = 0;
  va_list args;

  if (!ds) ds =  DSBLANK(DSSCAN_MAX);

  if  (!fmt) goto fin;

  va_start(args, fmt);
  
  result = vsscanf( ds, fmt, args);
  
  va_end(args);
  
  dstring_minimize_mem(ds);

 fin:
  return result;
}
#endif // #if defined(__GNUC__)


/* An  snprintf  wrapper */
/* reads values from variables into a dstring.
 
 * NOTE ( you MUST run dstring_minimize 
 * on any dstrings used as output arguments 
 * to update their string length and free chars sizes.)
*/

#if defined(__GNUC__) 

inline __attribute__ ((__gnu_inline__)) int dsprintf ( dstring ds, size_t sz, const cstring fmt, ...)
  __attribute__ ((format (printf, 3, 4))); 


inline __attribute__ ((__gnu_inline__)) int dsprintf( dstring ds , size_t sz, cstring fmt, ...)
{
  int result = 0;
  va_list args;

  
  if (!ds) 
  {
    ds = DSBLANK(sz+1);
  }
  else
    {
      dstring_clear(ds);

      size_t dlen = dstlen(ds) + dstfree(ds);

	if (dlen < sz) 
	  fprintf(stderr, " WARNING [dsprintf] %zu bytes exceeds lenght of dstring [%zu]\n" , sz, dlen );     
    }


 if (!fmt) goto fin;

  va_start(args, fmt);

  result = vsnprintf( ds, sz, fmt, args);

  va_end(args);

  dstring_minimize(ds);
fin:
  return result;
}

#endif // dsprintf (clang)



/*END*/

