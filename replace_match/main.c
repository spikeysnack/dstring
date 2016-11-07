#include <stdlib.h>
#include <stdio.h>
#include "dstring.h"
#include "dstringarray.h"
#include "replace_match.h"

typedef struct
{
  char* old;
  char* new;
}replace;




int main(int argc, char** argv , char** env)
{

  dstringarray lyrics = NULL;
  dstring filename = new_dstring("Lazarus.txt");
  dstring author  = new_dstring("David Bowie");
  dstring title   = new_dstring("Lazarus");
  
  lyrics = read_lines_from_file(filename );

  if ( ! lyrics)
    {
      fprintf(stderr, "read failed\n");
     exit(-1);
    }
  else
    {
      dstringarray_for_each(lyrics, P );

      fprintf( stdout , "%s\n" , title );

      fprintf( stdout , "%s\n" , author );
    }


 dstringarray dsa  = NULL;
 size_t count = COUNT(lyrics);
 size_t i = 0;
 
//dstring replace_word( const dstring in , const cstring match, const cstring replacement );

 dsa = new_dstringarray(count);

const replace a =     { "I'm "  , "David's "  };
const replace b =     { "I've " , "David's "  };
const replace c =     { "I'll " , "David's "  };
const replace d =     { "I "    , "David "    };
const replace e =     { " me"   , " David"    };

 for(i = 0; i< count; i++)
   {
     dsa->arr[i] = replace_word( lyrics->arr[i] , a.old, a.new );

     dsa->arr[i] = replace_word( dsa->arr[i]    , b.old, b.new );
     dsa->arr[i] = replace_word( dsa->arr[i]    , c.old, c.new );
     dsa->arr[i] = replace_word( dsa->arr[i]    , d.old, d.new );
     dsa->arr[i] = replace_word( dsa->arr[i]    , e.old, e.new );
  }
 
 // dstringarray_for_each(dsa , DSDEBUG);

 DSARRAY_PRINT(dsa);
 // clean up
 DSARRAY_FREE(dsa);
 DSARRAY_FREE(lyrics);
 DSFREE_ALL( title, author, filename );


  return 0;
}

/*end*/
