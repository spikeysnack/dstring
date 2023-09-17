/* rename.c */

/* AUTHOR:  Chris Reid <spikeysnack@gmail.com> */
/* LICENSE: Free for all purposes              */
/* COPYRIGHT: 2015- Chris Reid                 */


#define RENAME_VERSION "1.8"
#define RENAME_DATE "16 Sep 2023"

#include <stdio.h>

#include "dstring.h"
#include "dstringarray.h"
#include "file_utils.h"
#include "rename.h"


/* some options for renaming. See options.h    */

bool all_matches;
bool force_rename;
bool quiet_rename;
size_t files_renamed;



bool rename_file ( const char* filename, const char* newname );
size_t rename_files_ds( dstringarray dsa , dstring match , dstring replace ); 
bool valid_filename ( const char* filename );
size_t longest_name(dstringarray dsa);
dstring format_for_length( dstring a , dstring b, size_t len );

int main(int argc, char **argv, char** env );



int main(int argc, char **argv, char** env )
{
  int arg_index = 0;
  size_t argsz  = 0;
  int i         = 0;
  dstring match   = NULL;
  dstring replace = NULL;
  dstringarray dsa_filenames = NULL;
  files_renamed = 0;

  if (argc < 2) 
    {
      usage(stderr);
      exit(-1);
    }
  
  /* set options and return index arg */
  arg_index = options(argc, argv ); 

  match   = new_dstring_init( argv[arg_index] ,   64 );

  replace = new_dstring_init( argv[arg_index+1],  64 );


  /* some basic safety bails */

  if (dst_cmp(match, replace) == 0 )
    {
      fprintf( stderr, "ERROR -- match and replacement are the same.\n" );
      usage(stderr);
      exit(-1);
    }


  if (file_exists(match) )
    {
      fprintf(stderr, "ERROR -- possible missing match argument\n" );
      fprintf(stderr, "ERROR -- match [%s] is a file name -- not overwriting. exiting\n" , match );     
      usage(stderr);
      exit(-1);
   }

  if (file_exists(replace) )
   {
      fprintf(stderr, "ERROR -- possible missing replacement argument\n" );
      fprintf(stderr, "ERROR -- replacement [%s] is a file name -- not overwriting. exiting\n" , replace );
      usage(stderr);
      exit(-1);
    }


  /* OK args are good */

  argsz = argc - (arg_index +2 ); // #arguments - #options
  
  dsa_filenames = new_dstringarray( argsz );

  i = (arg_index + 2); //skip match and replace strings


  /* create a dstring array of filenames  from cmdline */
  while ( argv[i] )
    {
      dstring_add( dsa_filenames , new_dstring_init(argv[i], strlen(argv[i])*2  ) );
      i++;
    }

  //  DSARRAY_PRINT(dsa_filenames);

  /* try to rename the files here */  
  files_renamed =   rename_files_ds( dsa_filenames , match, replace );


  DSARRAY_FREE(dsa_filenames);


  if (! quiet_rename) 
    {
      if( files_renamed )
	fprintf(stderr, "Renamed %zu files.\n", files_renamed);
      else
	{
	  fprintf(stderr, "no files renamed. \n");
	}
    }

  exit(EXIT_SUCCESS);
} /* main */



size_t rename_files_ds( dstringarray dsa , dstring match , dstring replace )
{
  dstring input    = NULL;
  dstring replaced = NULL;
  size_t  j = 0, k = 0;
  bool success     = NO;
  size_t renamed   = 0;
  size_t longest   = 0;
  dstringarray changed_names = NULL;

  if (! quiet_rename) changed_names = NEW_DSARRAY( 2 * dsa->size);

  for (j = 0 ; j < dsa->size ; j++)       
    {
      input = dstring_copy(dsa->arr[j]);
      
      replaced = replace_word( input, match, replace ); //works
  
      if (all_matches) 
	{
	  while (strstr( match , replaced ) )
	    {
	      replaced = replace_word( replaced, match, replace);	  
	    }
	}


      // validate computed filename 
      if ( ! valid_filename(replaced) )
	{
	  break;
	}

      
      if ( dst_cmp( replaced, input) != 0 ) // change
	{
	  if (! quiet_rename) 	//build a list of changed names
	    {
	      dstring_add(changed_names , dstring_copy(input) );
	      dstring_add(changed_names , dstring_copy(replaced) );
	    }
	  
	  if (! file_exists(replaced) )
	    {
	      success = rename_file( input, replaced );
	      if (success) renamed++;
	    }
	  else
	    {
	      fprintf(stderr, "ERROR: file [%s] already exists -- not overwriting. exiting.\n", replaced );
	      break;
	    }
	}
      
      DSFREE(replaced);
      DSFREE(input);
      
    } // for
  

  if (! quiet_rename) 
    {
      longest = longest_name(changed_names);    
  
      k = COUNT(changed_names);
  
      for( j = 0; j < k; j+=2)
	printf( "%s\n", format_for_length(changed_names->arr[j], changed_names->arr[j+1], longest ) );

      DSARRAY_FREE(changed_names);
    }
  
  
  return renamed;
} // rename_files_ds





size_t longest_name(dstringarray dsa)
{
  size_t i = 0;
  size_t j = 1;
  size_t k = 0;

  for( i = 0; i < dsa->size; i++ )
    {

      k = dstlen(dsa->arr[i]);

      if ( j < k) j = k; 
    } 
  
  return j;
}




dstring format_for_length( dstring a , dstring b, size_t len )
{

  size_t a_sz  = dstlen(a);
  size_t b_sz  = dstlen(b);
  size_t center = len + 3 ;  

  size_t l = (a_sz > b_sz)? a_sz : b_sz;
  
  dstring spaces = DSBLANK(center - l);

  memset(spaces, ' ', center - l );

  dstring_minimize(spaces);
  
  dstring arrow = dstring_new_init(" ==> ", 16);

  dstring f = DSBLANK( 2 * (center +l)  );
  
  //  f = dstring_append(f, spaces );  
  f = dstring_append(f, a );  
  f = dstring_append(f, spaces );
  f = dstring_append(f, arrow );
  f = dstring_append(f, spaces );
  f = dstring_append(f, b );

  DSFREE(arrow);
  DSFREE(spaces);

  return f;
}






/*END*/
