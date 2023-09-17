/* recho.c */

/* test program for basic tutorial */

#include <stdlib.h>
#include <stdio.h>
#include "dstring.h"


int main ( int argc, char** argv, char** env)
{
  dstring bw = NULL;

  size_t i = 0;

  if (argc > 1 )
    {      
      bw = DSBLANK(100); // an empty dstring

      for ( i=argc ; i >0 ; i--)
	{
	  bw = dstring_append( bw, argv[i] ); //append the args into the string (last to first)
	  bw = dstring_append( bw, " " );    // append a space to them
	}

      fprintf(stdout, "%s\n" , bw); // output

      
      DSFREE(bw); //clean up
    } // if
  else
    {
      /* no args so do nothing */
    }

  return 0;
}

/*END*/

