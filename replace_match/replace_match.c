/*replace_match.c*/



#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "dstring.h"
#include "dstringlist.h"
#include "dstringarray.h"
#include "replace_match.h"

#define HEREDOC(...) #__VA_ARGS__
ssize_t getline(char **lineptr, size_t *n, FILE *stream);


/* open a file and read lines from the whole file
 * store them in a dstringarray and return that.
 */
dstringarray read_lines_from_file( const cstring filename)
{
  FILE* fp;
  dstring ds = NULL;
  dstringlist dsl = NULL; /* a dstringlist as this is for an example */
  dstringarray dsa = NULL;
  size_t numlines = 0;

  if(! filename) 
    return NULL;

  dsl = new_dslist();

  fp = fopen(filename, "r");

  if (fp == NULL)
    return NULL;

  do 
    {
      ds = read_a_line(fp);
      append_dstring_nodelete( dsl, ds); // we will free the list but not delete the dstrings
      numlines++;
    }while(ds);



  fclose(fp);

  dstringlist iter = dsl;

  //  fprintf(stderr, "numlines:\t%zu\n", numlines);

  dsa = new_dstring_array( numlines );

  list_for_each_entry(iter, &(dsl->list), list)
    {
      dstring_add(dsa, iter->ds);
    }

  DSLISTFREE(dsl); // list freed but dstrings not.
  
  //  DSARRAY_PRINT(dsa);

  return dsa;
} 


/* read 1 line from an open file pointer
 * and return it as a dstring 
 */
dstring read_a_line(FILE* fp )
{
  cstring line = NULL;
  dstring dline = NULL;
  size_t  len  = 80;
  ssize_t read = 0; // must be signed to hold -1

  if (fp == NULL)
    return NULL;

  if ( (read = getline(&line, &len, fp)) != -1)  
    {
      dline = new_dstring(line);
      free(line);
    }
    /*
    fprintf(stdout, "Retrieved line of length %zu :\n", read);
    fprintf(stdout, "%s", line);
    DSDEBUG(dline);
    */ 
    
  if (dstlen(dline) > 0)
    {
    return  dline;
    }
  else
    {
      DSFREE(dline);
      return NULL;
    }
}




bool write_lines_to_file(const cstring filename, dstringarray lines)
{
  FILE* fp;
  size_t numlines = 0;
  size_t counter = 0;

  if (! filename) 
    return false;

  numlines = COUNT(lines);

  fp = fopen(filename, "a");

  if (!fp)
    return false;

  for ( counter = 0; counter < numlines ;counter++) 
    {
      if ( ! write_a_line(fp, DSTRING_AT(lines, counter)) ) 
	return false;
    }


  fclose(fp);  // close the file

  return true;
}


bool write_a_line(FILE* fp, dstring line)
{
  int err = 0;
  if (!fp)
    return false;

  if(line)
    { 
      err = fprintf( fp, "%s\n", line );   // write to file
      fflush(fp);           
      if (err < 0) return false; //failed
    }

  return true;
}




/*END*/

