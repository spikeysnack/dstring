/* dstring_utils.h */

/**  A collection of string utility functions
 **  not built-in to C  standard library
 **/


/* AUTHOR:  Chris Reid <spikeysnack@gmail.com> */
/* LICENSE: Free for all purposes              */
/* COPYRIGHT: 2015- Chris Reid                 */



/* Note: 
 * If the symbol USE_SMALL_WORDS is defined
 * -D USE_SMALL_WORDS on the compilation line
 * or here as a preprocessor directive
 * then small words such as a,an, the, of ,
 * will not be capitalized in title_case.
 */

//#define USE_SMALL_WORDS
//#define DEBUG_MALLOC


#define _GNU_SRC 

#ifndef DSTRING_UTILS_H
#define DSTRING_UTILS_H


#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <assert.h>
#include <errno.h>
#include <stdbool.h>

#include "dstring.h"


/***************************************************************************/
/* preliminaries / setups */


/* just so we can refer by name */
enum { TAB=9, SPACE=32, VTAB=11 };

#ifdef DEBUG
/* these are so common as to be almost givens */ 
#define MAX( m, n)  (m > n)? m:n
#define MIN( m, n)  (m > n)? n:m
#define ABS( x )  (x > 0)? (x) : -(x)

/* print function name , source line , var name , var value */
/* string debug */
#define SDEBUG(_s)  fprintf( stderr , "%s %d %s %s\n", __FUNCTION__, __LINE__, #_s ,  (_s) )
/* int debug */
#define IDEBUG(_i)  fprintf( stderr , "%s %d %s, %d\n", __FUNCTION__, __LINE__, #_i , (int) (_i) )
/* address debug */
#define ADEBUG(_a)  fprintf( stderr , "%s %d %s %p\n", __FUNCTION__, __LINE__, #_a, (void*) (_a) )

#endif



/***************************************************************/
/* function definitions */

bool    check_spaces( const dstring in , const dstring match );
bool    is_sentence(  const dstring in);
bool    is_quoted(    const dstring in);

dstring quote(   const dstring in);
dstring unquote( const dstring in);
dstring chomp( const dstring in);
dstring pad( const dstring _in, int n );
dstring pad_char( const dstring _in,  char c, int n );
dstring rm_dblspaces( const dstring in );
dstring truncate_string( const dstring _in , int n );

dstring find( const dstring ds , const cstring match);
dstring dstring_after( const dstring ds , const cstring match);
dstring dstring_before( const dstring ds , const cstring match);

dstring rtrim( const dstring in, const cstring match);
dstring ltrim( const dstring in, const cstring match);
dstring trim( const dstring in, const cstring match);

dstring all_caps( const dstring in);
dstring sentence_case( const dstring in);
dstring title_case( const dstring in);
dstring uncapitalize( const dstring in );


dstring insert( const dstring _in, const cstring _s, int pos );
dstring insert_word( const dstring _in, dstring _ins, int pos );
dstring remove_word( const dstring _in, const cstring _match );
dstring replace_word( const dstring in , const cstring match, const cstring replacement );

#endif // DSTRING_UTILS_H

/*END*/

