/* dstring_utils.c */

/* AUTHOR:  Chris Reid <spikeysnack@gmail.com> */
/* LICENSE: Free for all purposes              */
/* COPYRIGHT: 2015- Chris Reid                 */


#include <stddef.h>
#include <stdio.h>
#include <stdarg.h>
#include <unistd.h>
#include <malloc.h>
#include <string.h>

#include "dstring_utils.h"


#ifdef USE_SMALL_WORDS
/* from : New York Times Manual of Style */

char* small_words[] =
  {
    " A ", " An ", " And ", " As ", " At ",
    " But ", " By ", " En " , " For ", " If ", " In ", " Is " ,
    " Of ", " On ", " Or ", " The ", " To ", " Via ", " Vs " , NULL
  };

#endif





/* private functions 
 *  C functions not in the .h files are callable but not advertised.
 *  Usage is for special cases and internal utilities not normally
 *  used by the application programmer.
 */



/*********************************************************/
/* public functions */
/*********************************************************/

/* Check if the word is surrounded by spaces. 
 * YES means the word is surrounded 
 * on both sides by a space character.
 * note: uses cstrings because of no modification.
 * cstrings or dstrings are the same to the compiler:
 * (char*) but I us different names to remind me that
 * I am dealing with separate objects. 
*/
bool check_spaces( const dstring in , const cstring match )
{
  bool OK        = NO;
  cstring at     = NULL;
  cstring after  = NULL;
  cstring before = NULL;
 
  if ( (!match) ||  (!in) )   goto fin;

  /* cstrings positioned at, after and before in */

  at = strstr( in, match);

  if ( (at) && (at > in) )
    {
      before = at - 1;
      after  = at + 1;
      
      if ( (at[0] != ' ' ) && (before[0] == ' ') && (after[0] == ' ') )  
	OK = YES; 
    }
  
 fin:
  return OK;
} // check_spaces



/* check for capital first letter and period at end */
bool is_sentence(const dstring in)
{

  enum  
  { 
    PERIOD    = '.', 
    QUESTION  = '?', 
    EXCLMARK  = '!'
  };
  
  bool quoted   = NO;
  bool sentence = NO;
  int len       = 0;
  int start     = 0;
  int end       = 0;

  if (!in) goto fin;

  len = dstlen(in);
  quoted = is_quoted(in);

  /* handle double-quoted sentence */
  if (! quoted ) 
    { start = 0; end = len -1; }
  else 
    { start = 1; end = len -2; }

  /* check capitalization of the first char */
  if ( (in[start]  < 'A') 
       && (in[start]  > 'Z')) 
    goto fin;

  /* check end char */
  if (     (in[end] == PERIOD) 
	   || (in[end] == QUESTION) 
	   || (in[end] == EXCLMARK) )
    sentence = YES;
  
 fin:      
  return sentence;
} // is_sentence



/* check if string is surrounded  by double-quotes */
bool is_quoted(const dstring in)
{
  int len;

  len = dstlen(in);

  if (( in[0] == '\"') && ( in[len-1] == '\"') )
    return YES;
  else
    return NO;
} // is_quoted



/* returns a copy of a string with 
 * double-quotes around it, 
 * unless it is already quoted then a straight copy */
dstring quote( const dstring in)
{
  int len;
  dstring out, dscopy;

  if (!in) return new_dstring_init( "\"\"" , 16);

  len = dstlen(in); 

  dscopy = dstring_copy(in);

  if (is_quoted(dscopy) ) return dscopy; // already quoted  
  
  out = new_dstring_init( "\"" , len+2 );
  
  strncat( out+1 , dscopy , len );

  out[len+1] = '\"';
  out[len+2] = '\0';

  dstring_minimize(out);

  return out;  
} //quote



/* returns a copy of a string with
 * the quotes removed  or a copy if
 * it is not quoted */
dstring unquote( const dstring _in)
{
  int i, ilen, cpylen;
  dstring dscopy ;

  if (! is_quoted(_in) ) return dstring_copy(_in);

  ilen = dstlen(_in);
  
  cpylen = ilen - 2;
  
  dscopy = new_dstring_init("" , cpylen );
   
  for (i = 0; i < cpylen; i++)
    {
      dscopy[i] = _in[i+1];
    }
  
  dscopy[cpylen] = '\0';
  
  dstring_minimize(dscopy); //safety
      
  return dscopy;
} //unquote




/* carriage returns and newline characters
 * are removed throughout the whole string.
 * The null char "\0" at the end is retained. 
 */

dstring chomp( const dstring in)
{
  int i, j, ilen;

  dstring chomped = NULL;

  if (!in) 
    {
      chomped = emptydstring();
      goto fin;
    }
  
  i     = 0;
  j     = 0;

  ilen  = dstlen(in);

  chomped  = new_dstring_init("" , dstlen(in) );

  while ( i < ilen  )
    {
      if ( (in[i] != '\r') &&  (in[i] != '\n') && (in[i] != '\0') )
	{
	  chomped[j++] = in[i];
	}
      i++;
    }
  
  chomped[j] = '\0';

  dstring_minimize(chomped); //recalculate len & free

 fin:
  return chomped;

} // chomp


dstring ltrim( const dstring in, const cstring match)
{
  cstring p;
  size_t mlen,count;
  bool dotrim; 

  dotrim = YES;
  p = in;
  mlen = strlen(match);
  count = 0;
  
  while(count < mlen)
    {
    if ( p[count] != match[count]) 
      {
	dotrim = NO;	  
      }
    count++;
    }
  
  if(dotrim)
    return new_dstring(in+mlen);
  else
    return dstring_copy(in);
} // ltrim



dstring rtrim( const cstring in, const cstring match)
{
  cstring p = NULL;

  cstring m = NULL;

  size_t ilen, mlen, i;

  bool dotrim= YES; /* default*/

  dstring dscopy = new_dstring(in);
  
  ilen = dstlen(dscopy);
  mlen = strlen(match);
    
  p = dscopy + (ilen-1);    //end of string
  m = match  + (mlen-1);    //end of string
  
  i = mlen;
  
  while( i )  
    {
      if ( *(p--) != *(m--) )  
	{
	  dotrim = NO; break;
	} 
      i--;
    }
    
  if(dotrim)
    return dstring_slice( in , 0 , ilen - mlen );
  else
    return dscopy;
} // rtrim


dstring trim( const dstring in, const cstring match)
{
  dstring trimmed =  rtrim( ltrim( in, match), match );

  return trimmed;
}





/* pad string(right) with n ' ' (space) characters */
dstring pad( const cstring _in, int n )
{
  int i, ilen, len;
  dstring padded;

  if (!n) return dstring_copy(_in);

  ilen = strlen(_in);

  len = ilen + n ;
 
  padded = new_dstring_init( _in , len );
  
  for ( i = ilen; i < len ; i++) 
    padded[i] = ' ';

  padded[len] = '\0';

  dstring_minimize(padded);
  
  return padded;
} // pad



/* pads (right) a string with a printable character  n times */
dstring pad_char( const cstring _in,  char c, int n )
{
  enum { TAB=0x09 , SPACE=0x20, DEL=0x7F };

  int i = 0, ilen = 0 , len = 0;
  dstring padded = NULL;

  ilen= strlen(_in);

  /* printable character range */

  if (  (c == TAB) || ((c >= SPACE ) && (c < DEL)) )
    {
      
      len = ilen + n;
      
      padded = new_dstring_init( _in , len);
      
      for ( i = ilen; i < len ; i++) padded[i] = c;
      
      padded[len] = '\0';      
    }
  else
    {
      fprintf( stderr, " ERROR [pad_char] non-printable character (%d) -- using spaces instead \n" , c);

      DSFREE(padded);

      return pad( _in, n ); 
    }
  
  return padded;
} //pad_char




/* Remove runs of spaces of two or more.
 * The string is scanned until all 
 * double spaces are removed.
 */ 
dstring rm_dblspaces( const cstring _in ) 
{
  int i=0, j=0, len=0;
  dstring dscopy = NULL;
  bool found = false;

  if(! _in) return NULL;

  dscopy = new_dstring( _in );

  len=dstlen(dscopy);

 floop:
  found = NO;
  
  for(i=0; i < len; i++)  //traverse the string
    {
    
      if( (dscopy[i]== ' ' ) && (dscopy[i+1]== ' ' )  ) //check for double space	
	{ 
	  found = YES;
	  for (j = i ; j < len ; j++)
	    {
	      dscopy[j]= dscopy[j+1];  // shorten string length 
	    }
	  len--;
	}//if
          
    }//for

  if (found) goto floop;
  

  dstring_minimize(dscopy);

  dstring_clear_free(dscopy);

  return dscopy;    
} //rm_dblspaces



/* returns a copy of a  dstring cut off at n chars 
 * ( truncate is a file length function in unistd.h
 * so truncate_string  will have to serve as a function name.) */
dstring truncate_string( const cstring _in, int n )
{

  dstring dscopy;

  dscopy = new_dstring(_in);
  
  dscopy[n] = '\0'; //truncate at  n

  dstring_minimize(dscopy);

  dstring_clear_free(dscopy);

  return dscopy;
} // truncate_string



/* returns a copy of a string with every letter capitalized */
dstring all_caps( const cstring in)
{
  dstring ac = NULL;
  int aclen = 0;
  char a = 'a', A = 'A';
  int gap = A - a;
  int i = 0;
  
  if(!in) return NULL;
  
  ac = new_dstring(in);

  aclen = dstlen(ac);
  
  for ( i = 0 ; i < aclen; i++)
    if ( (ac[i] >= 'a') && (ac[i] <= 'z') )  
      ac[i] += gap; 
  
  return ac;
} // all_caps



/* Uncapitalise every letter in string */
dstring uncapitalize( const cstring in )
{
  dstring uc = NULL;
  int uclen = 0;
  char a = 'a',  A = 'A';
  int gap = A - a;
  int i = 0;
  
  if(!in) return NULL;

  uc = new_dstring(in);

  uclen = dstlen(uc);

  for ( i = 0 ; i < uclen; i++)
    {
      if ( (uc[i] >= 'A') && (uc[i] <= 'Z') )  uc[i] -= gap; 
    }
  
  return uc;
} // uncapitalize


dstring sentence_case( const cstring in)
{
  dstring uc = NULL;
  dstring sc = NULL;
  char a = 'a';
  char A = 'A';
  int gap = A - a;
  
  uc = uncapitalize(in);
  sc = dstring_copy(uc);

  if ( (uc[0] >= 'a') && (uc[0] <='z') )  sc[0] += gap; 

  DSFREE(uc);
  return sc;
}



/* Capitalise every word in the string.
 * If USE_SMALL_WORDS is defined then 
 * the words in the small_words list 
 * are not capitalized.
 */
dstring title_case( const cstring in)
{
  dstring tc = NULL;
  dstring tmp = NULL;
  int tclen = 0;
  char a = 'a',  A = 'A';
  int gap = A - a;
  int i = 0;
  
  if(!in) return NULL;
  
  tc = new_dstring(in);

  tmp = tc;

  tc = uncapitalize(tc);

  DSFREE(tmp);

  tclen = dstlen(tc); 

  if ( (tc[0] >= 'a') && (tc[0] <= 'z') ) tc[0]+=gap; 

  for ( i = 0 ; i < tclen; i++)
    {
      if ( (tc[i] == ' ') && (tc[i+1] >= 'a') && (tc[i+1] <= 'z') )  
	tc[i+1] += gap; 
    }
  
  tc[tclen] = '\0';
  
#ifdef USE_SMALL_WORDS
  i = 0;
  cstring small = NULL;
  cstring found = NULL;
  
  while( (small = small_words[i++])  )
    {      
      found = strstr(tc, small);
      if (found ) found[1] -= gap; // skip ' '
    }//while
#endif

      
  return tc;

} // title_case


/* a string is added to the beginning of a string */
dstring prepend ( const dstring _in , const dstring _beg )
{
  dstring prepended;
  prepended = dstring_append( _beg , _in);
  return prepended;  
} // prepend



/* A string is added to the end of a string 
 * A new string is returned. */
dstring append ( const dstring _in , const dstring _end )
{
  return dstring_append(_in, _end);
} // append



/* inserts a string in the middle of another string 
 * at pos letters in. The string is split and the 
 * end part is attached after the inserted string */

dstring insert( const dstring _in, const cstring _s , int pos )
{
  dstring ins = NULL;
  dstring s   = NULL;
  size_t ilen = 0;
  size_t slen = 0;
  size_t len = 0;
  int i = 0;

  if (_s) 
    s = new_dstring(_s);
  else
    return dstring_copy(_in);

  ilen = dstlen(_in);
  slen = dstlen(s);

  len = ilen + slen;
  
  if ( pos > len ) return dstring_append( _in, s);

  ins = dstring_new_init( "" , len );

  for(i = 0; i < pos ; i++)
    {
      ins[i] = _in[i];
    }
  
  for(i = pos; i < pos+slen ; i++)
    {
      ins[i] = s[i-pos];
    }

  for(i = pos+slen ; i < len ; i++)
    {
      ins[i] = _in[i-slen];
    }

  
  ins[len] = '\0';
  
  dstring_minimize(ins);
  
  return ins;

} // insert 




/* Like insert above, 
 * but inserts spaces 
 * on both sides of the word. 
 */
dstring insert_word( const dstring _in, const cstring _word, int pos )
{
  int i;
  size_t ilen, wlen, len;
  dstring ins;

  ilen = dstlen(_in);

  //  fprintf( stderr, "DEBUG [insert_word] in = %s , %zu %zu [%zu]\n", _in , dstlen(_in) , dstfree(_in) , dst_mem(_in) );

  if ( is_dstring(_word) ) wlen = dstlen(_word);
  else wlen = strlen(_word);

  len = ilen + wlen + 2;

  ins = dstring_new_init("" , len );
  
  if ( pos > ilen) pos = ilen;

  /* copy first part */
  for(i = 0; i < pos ; i++)
    {
      ins[i] = _in[i];
    }
  
  // add space 
  ins[pos] = ' ';
  
  // insert word 
  for(i = pos+1; i < wlen+pos+1 ; i++)
    {
      ins[i] = _word[i-(pos+1)];
    }
  //add space
  ins[wlen+pos+1] = ' ';

  // copy second part 
  for(i = pos+wlen+2 ; i < len ; i++)
    {
      ins[i] = _in[i-(pos+1) ];
    }

  //  ins[len] = '\0';
  
  //  fprintf( stderr, "DEBUG [insert_word] %s , %zu %zu [%zu]\n", ins , dstlen(ins) , dstfree(ins) , dst_mem(ins) );

  dstring_minimize(ins);

  //  fprintf( stderr, "DEBUG2 [instert_word] %s , %zu %zu [%zu]\n", ins , dstlen(ins) , dstfree(ins) , dst_mem(ins) );
  
  return ins;

} //insert_word




/* removes a word from a string and resizes to proper size.
 * if the string is a sentence, and the words we want to remove 
 * is at the end, we check for commas and periods
 * and chop them out.
 * there is already a remove funtion in stdio.h that deletes 
 * files so remove_word wil have to do.
 */

dstring remove_word( const dstring _in, const cstring _match )
{
  enum { PERIOD='.', SPACE =' ', COMMA=',' };

  dstring in      = {0};
  dstring found   = {0};
  dstring out     =  {0};
  dstring match   = new_dstring(_match);
  size_t  matchlen = 0;
  dstring bad     = {0};

  dstring before = {0};
  dstring after  = {0};

  bool sentence = false;
  bool quoted   = false;

  if(  (!_in) || (!_match) ) return NULL;
  
  matchlen = dstlen(match);
  quoted   = is_quoted(_in);
  sentence = is_sentence(_in);

  if (quoted )
      in = unquote( _in);
  else
    in = dstring_copy(_in);

  found = find( in, match);

  if (found) 
    {
            
      if ( found[matchlen] == COMMA )
	{
	  match = append(match, ",");
	} 

      if ( dstlen(found) == dstlen(in) ) 
	{
	  out = dstring_after ( in, match);
	  out = ltrim(out, " ");

	}
      else
	{
	  before = dstring_before( in, match);
	  after = ltrim( dstring_after ( in, match), " ");
	  out = append(before, after);
	}

      if ( sentence )
	{
	  bad = find( out, ",.");

	  if (bad) out = replace_word( out, ",.", ".");

	  bad = find( out, " .");

	  if (bad) out =replace_word( out, " .", ".");

	}

      if (quoted) out = quote( out);
      
      return out;      
    }
  else  /* match not found */
    {
      return in;
    }


}// remove_word */





/* The word is searched for a match,
 * and the match is replaced with the new string.
 * The string is split and the end is reattached
 * after the insert, with according lengthening
 * or shortening.
 * 
 */
dstring replace_word( const dstring in , const cstring _match, const cstring _replacement )
{

  int ilen=0, mlen=0,  flen=0;
  dstring found       = NULL;
  dstring last        = NULL;
  dstring match       = NULL;
  dstring replacement = NULL;
  dstring replaced    = NULL;
  cstring result      = NULL;

  if (!in) return NULL;

  if (dstlen(in) == 0 ) return NULL;

  if ( (! _match) || (strlen(_match) == 0 ) )
    return dstring_copy(in);
  else
    match  = new_dstring(_match);

  if ( strlen(_replacement) == 0  )
      return remove_word( in , match );
  else
      replacement = new_dstring(_replacement);
  
  ilen = dstlen(in);

  if ( is_dstring(match) ) 
    mlen = dstlen(match);
  else 
    mlen = strlen(match);

  /* do actual search */
  result = strstr( in, match);

  if ( result )
    {
      found = new_dstring ( result );

      flen = dstlen(found);
      
      last = new_dstring( (found + mlen) ); // just past the match

      replaced = dstring_truncate( in , ilen - flen );       

      replaced = dstring_append( replaced , replacement );

      replaced = dstring_append( replaced , last );
      DSFREE(last);
      DSFREE(found);
    }
  else  // not found
    {
      replaced = dstring_copy(in);
    }

  DSFREE(match);

  return replaced;
  
} // replace_word    


/* we might  not have this anymore in glib so here it is once again for the win */

#ifndef memmem
 
static inline void *memmem(const void *haystack, size_t hlen, const void *needle, size_t nlen)
{
  int needle_first;
  const void *p = haystack;
  size_t plen = hlen;

  if (!nlen)
    return NULL;

  needle_first = *(unsigned char *)needle;

  while (plen >= nlen && (p = memchr(p, needle_first, plen - nlen + 1)))
    {
      if (!memcmp(p, needle, nlen))
	return (void *)p;

      p++;
      plen = hlen - (p - haystack);
    }

  return NULL;
}
#endif

/***************************************************/
/* find a substring and return it */

dstring find( const dstring ds , const cstring match)
{
  size_t mlen   =    0;
  cstring found = NULL;
  dstring dsfnd = NULL;

  if (match)
    {
      mlen = strlen(match); 
      found = memmem( ds, dstlen(ds) , match, mlen );
    }
  
  if ( found )  dsfnd = dstring_new_init( found , mlen+1);

  return dsfnd;  
}

/* find a substring and return the rest of the  string after it */
dstring dstring_after (const dstring ds , const cstring match )
{
  size_t dlen     =    0;
  size_t mlen     =    0;
  size_t sublen   =    0;
  dstring found   = NULL; 
  dstring dsafter = NULL;

  dlen = dstlen(ds);
  mlen = strlen(match);

  if ( mlen > dlen) goto fin;

  found = find(ds , match);
  
  if( found == ds ) return dstring_copy(ds);

  if( found )
    {
      sublen = (found - ds) - mlen;
      dsafter = new_dstring_init( found+mlen , sublen+1); 
    }
  
  dstring_minimize_mem(dsafter);

 fin:
  return dsafter;  
}



/* find a substring and return the substring before it */
dstring dstring_before (const dstring ds , const cstring match )
{
  dstring found = NULL; 
  dstring dsbefore = NULL;

  found = find(ds , match);

  dsbefore = rtrim( ds , found);
  
  dstring_minimize_mem(dsbefore);

  return dsbefore;  
}



/***************************************************/

/*END*/
