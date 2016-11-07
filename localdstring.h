/* localdstring.h */

/* AUTHOR:  Chris Reid <spikeysnack@gmail.com> */
/* LICENSE: Free for all purposes              */
/* COPYRIGHT: 2016 - Chris Reid                */

/** local (stack allocated) dstrings 
 **
 **  Stack allocated objects are reclaimed when they go out of scope.
 **   This means you need not free them explicitly,
 **   and they take up no memory when the function exits.
 ** 
 **  It also means you can't send them into functions as 
 **   parameters, or return them from functions, or rely
 **   on their values externally to the current context.
 **
 **  In C this means the current function in the current file.
 ** 
 **  It is estimated ~80% of strings in a program are local
 **   to the function they are used in, so this make sense to 
 **   include them in dstrings as an option.
 ** 
 **  The structure is nearly identical to a regular dstring,
 **   except the d->str member is a fixed-length array of char
 **   instead of dstring's variable length array.
 **   This is more of a compiler requirement than a design decision;
 **   The compiler would not allow alloca to create a struct on the 
 **   stack of unknown size (probably a wise restriction).
 **
 **  The basic trick of accessing by the str pointer is used again
 **   and "Gee It Just Works" © so there you go. 
 ** 
 **  Just don't exceed LOCALMAX characters or the localdstring
 **   will be truncated to LOCALMAX. I set LOCALMAX to 256 because
 **   almost no sentence you might write is longer than that in a function,
 **   but you may set it to any multiple of 16 you deem appropriate for 
 **   your application.
 ** 
 **  The C macros in this file use a number of clever tricks designed to
 **   keep the allocation functions in the current scope and to avoid
 **   name collisions on repeated macro calls where variables are created.
 **   One technique is the __extension__ compiler directive which tells
 **   the compiler to turn off some pedantic warnings, coupled with an
 **   enclosing set of parentheses and curly brackets.
 ** 
 **  (__extension__ ({  ...code ... })) turns the enclosed code into
 **   an expression, and if you make sure the last code statement evaluates
 **   to a desired type that is what the expression "returns". 
 **  Another technique is to use a do{ ...code... }while(0) to create an
 **   enclosing scope within a macro. It also "returns" the last typed statement. 
 **  A third technique is to use multiple dependent macros to organize parameters and
 **   make macros friendlier or more in keeping with the api. Variadic macros
 **   use this a lot to simulate duck typing and roll  multiple functions into one.
 **
 **  It takes some mental gymnastics somtimes to understand why these work the
 **   way they do, and you can mess up using them in unexpected ways, but if you
 **   practice with them you will gain confidence and eventually the code will 
 **   flow from your fingers like a master mage's blessing of good fortune
 **   before the full moon festival night of drinking, feasting, and mating.
 **   ( Until then, type slowly and deliberately, checking yourwork, compiling 
 **     incrementally, getting each part right at each step. 
 **     The paradoxical mantra with C is, as always, "The hard way is the easy way.")
 **
 **/ 


#ifndef LOCALDSTRING_H
#define LOCALDSTRING_H

#include <alloca.h>  /* stack allocation */
#include <string.h>  /* printf family */
#include "dstring.h"


/* A type  alias same as dstring */ 
typedef dstring localdstring;
 
/* Most strings will be less than this big. */
#define LOCALMAX  256

/* This is similar to struct dst but of fixed size. */
typedef struct localdst localdst;

struct localdst
{
#ifdef ENV32BIT
  size_t fill[2]; // makes it same size
#endif
  size_t len;  // chars used
  size_t free; // chars unused
  char  str[LOCALMAX]; // actual c-style string
};


#ifndef strndupa
/* Return an alloca'd copy of at most N bytes of string.  
   This is primarliy for c-style strings                */ 
#define strndupa(S, N)                                  \
  (__extension__	                  		\
  ({							\
  const char* __old   = (S);				\
  size_t __len = strnlen (__old, (N));                  \
  char *__new = (char *) __builtin_alloca (__len + 1);  \
  memset(__new , 0, __len + 1);                         \
  (char *) memcpy (__new, __old, __len);                \
  __new[__len] = '\0';                                  \
  __new;                                                \
}))

#endif


#ifndef DSTRING_AUTO
#define DSTRING_AUTO
/* Generate a stack-allocated dstring with alloca from a c-style string 
   ( The trick here is to run the stack allocations in the calling scope )
   ( of the current context by doing it in the parameter list            )
   ( instead of inside the function. The function does no allocation;    )
   ( it merely organizes and assigns the newly allocated data.           )
 */
#define dstring_auto(cs) dstring_auto_impl( strlen(cs), strndupa(cs, strlen(cs) ) , (localdst*)alloca( sizeof(localdst)) )

#define dstring_auto_null() dstring_auto("")

static inline localdstring dstring_auto_impl( size_t sz, cstring cs, localdst* lds);

static inline localdstring dstring_auto_impl( size_t sz, cstring cs, localdst* lds)
{
  memset(lds , 0, sizeof(localdst) );
  
  if (sz)
    {    
      memcpy(lds->str ,cs , sz); 
      lds->len = sz;
      lds->free = LOCALMAX - sz;
    }
  else
    {
      lds->len = 0;
      lds->free = LOCALMAX;      
    }
  
  return lds->str; 
} // dstring_auto_impl
#endif




/* 
   creates a stack-based dstring that releases its memory 
   when it goes out of local scope. 
   Do not pass localdst as a parameter to a function 
   or try to return one from a function.
   
   localdstring lds = DSLOCAL("A LOCAL STRING");

*/

#define DSLOCAL(cs) (__extension__ ({                        \
 size_t sz  = strlen(cs);                                    \
 localdst* _ldst = (localdst*)alloca (sizeof (localdst));    \
 memset(_ldst, 0, sizeof (localdst) );                       \
 memcpy(_ldst->str, cs, sz       );                          \
 _ldst->len = sz; _ldst->free = LOCALMAX - sz;               \
 _ldst->str;                                                 \
 }))



/* 
   Use a format string (as in snprintf ) 
   and return a stack-allocated localstring 
   
   localdstring lds = 
    DSLOCAL_FMT( "%d = three , pi ~= %08f" , 3 , 3.14159 );
*/

#define DSLOCAL_FMT(...)	( __extension__ ({	         	\
  size_t _dst_len = snprintf(NULL, 0, __VA_ARGS__) + 1  ;		\
  if (_dst_len > LOCALMAX) _dst_len = LOCALMAX;                         \
  cstring _cs = (cstring) alloca ( _dst_len );                          \
  memset( _cs , 0 , _dst_len);                                          \
  snprintf( _cs, _dst_len, __VA_ARGS__ );	                        \
  DSLOCAL(_cs);                                                         \
}))


/* concatenate localdstrings into one new one */
#define DSLOCAL_CONCAT(...)   ( __extension__ ({                \
  localdstring _lds = DSLOCAL("");   			        \
  do { size_t n = 0;                                            \
  dstring dsarr[] = { __VA_ARGS__ , NULL };                     \
  while (dsarr[n] != NULL)                                      \
  { _lds = DSLOCAL_FMT( "%s%s", _lds, dsarr[n] ); n++;}         \
  }while(0);							\
  _lds;                                                         \
}))




/* scan data from a localdstring into a varargs list of variables 
   according to a format string 
   ( This really is useless.  Perhaps it needs .. something.    )
  
*/
#define DSLOCAL_SCAN( DSL , FMT, ...) \
  (__extension__ ({ sscanf(DSL, FMT, __VA_ARGS__); }))


#endif //LOCALDSTRING_H


/*END*/








