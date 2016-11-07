/* file_utils.h */

/* 
 * some utlity options  for basic file operations
 */

/* utils.h */

/* 
 * some utlity options 
 * for basic operations
 */

/* AUTHOR:  Chris Reid <spikeysnack@gmail.com> */
/* LICENSE: Free for all purposes              */
/* COPYRIGHT: 2015- Chris Reid                 */


#ifndef FILE_UTILS_H
#define FILE_UTILS_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>


/* from changecase, rename */
extern bool change_suffix;
extern bool change_suffix_only;
extern bool all_matches;
extern bool force_rename;
extern bool quiet_rename;


#define BUFSIZE 256

bool file_exists(const char* filename);
bool valid_filename ( const char* filename );
bool rename_file ( const char* filename, const char* newname );
bool get_response();


/* this may be redundant */

bool get_response()
{
  char line[BUFSIZE] = {0};
  char p;

  fflush (stdin);

  if (  scanf ("%s", line) == EOF )
    return false;

  if (line == (char*)NULL )
    return false;
  else
    {
      p = line[0];
      if ( (p == 'Y') || ( p == 'y') )
        return true;
      else
        return false;
    }
}/* get_response */



/* file utils */
/* hack to check existence of a file */
bool file_exists(const char* filename)
{
  assert (filename != (char*) NULL );
  
  if ( access( filename , F_OK ) == 0 ) //check for existence
    return YES;
  else
    return NO;
}


/* check some aspects of filenames */
bool valid_filename ( const char* filename )
{
  char first = filename[0];
  char* ic = NULL;
  size_t icsz = 0;
  char* ob = NULL;
  size_t obsz = 0;
  char* tmpstr = NULL;

  if (! filename) return NO;
  
  const char*        dot            = ".";
  /*string: the period by itself is a psuedo alias in linux for the current directory */
  
  const char*        dotdot         = "..";
  /*string: two periods is a psuedo alias in linux for the parent directory */
  
 
  const char*        dbl_space      = "  ";
  /*string: double spaces in filenames cause problems with some programs */
   
  const char*        invalid_chars  = "/><|:&";
  /*string: filenames should not contain these chars */
  
  const char*        other_bad      = "-*!?;^+={}\"\'\\,";
  /*string: filename should not start with these chars */
  
  #define strlen(S) ( (S==NULL) ? 0 : strlen(S) )
  
  if ( (strlen(filename) == 1) && (strncmp ( filename , dot , 1  ) == 0 ) )
    {
      fprintf (stderr, "ERROR -- : . is a psuedo alias for the current directory.  exiting\n"  );
      return NO;
    }

  if ( (strlen(filename) == 2) && (strncmp(filename , dotdot , 2  )==0) )
    {
      fprintf (stderr, "ERROR -- : .. is a psuedo alias for the parent directory.  exiting\n"  );
      return NO;
    }

  if ( strstr ( filename , dbl_space ) )
    {
      fprintf (stderr, "ERROR -- [ %s ] would have a double space in the name.", filename );
      return NO;
    }

  tmpstr = (char*) filename; // not const

  ic =  (char*) invalid_chars;
  icsz = strlen(ic);

  ob =  (char*) other_bad;
  obsz = strlen(ob);

  int i, j, k, tl;

  tl = strlen(tmpstr);

  // invalid characters
  for( k = 0; k < tl; k++)
    {
      for(i = 0 ; i < icsz; i++)
	if (tmpstr[k] == ic[i])
	  {
	    fprintf (stderr, "ERROR -- [ %s ] contains invalid character [%c].  exiting. \n", filename, ic[0] );                                          
	    return NO;
	  }      

    }//while


  // other bad first characters
  for( j =0; j < obsz ; j++)
    {
      if ( first == ob[j] )
	{
	  fprintf (stderr, "ERROR -- [ %s ] starts with bad character [%c].  exiting. \n", filename, ob[0] );                                          
	  return NO;
	}
    }//for
      
  return YES;
} // valid_filename



/* boolean wrapper for stdio.h rename function */

bool rename_file ( const char* filename, const char* newname )
{
  int err;
  int ret = false;
  char* errstring;
  assert( newname != (char*) NULL );    
  errno = 0;

  if ( (file_exists( filename )) && (access(filename, W_OK) == 0) ) 
	{

      	  if (! force_rename )
	    {
              fprintf( stdout, "rename %s to %s (y/n)? ", filename , newname );
	      
	      if ( ! get_response() )  
		{
		  ret= false;
		  goto DONE;
		}
	    } 

	  if ( rename( filename, newname ) == 0 ) 
	    { 
	      ret = true;
	    }
	  else
	    {
	      
	      err = errno;
	      errno = 0;
	      errstring = strerror(err);
	      perror( "rename had an error:\t");
	      perror(errstring);
	      ret = false;
	    }
	  
	} /* file exists */
      else
	{
	  err = errno;
	  errno = 0;
	  errstring = strerror(err);
	  perror( " filename does not exist or cannot be accessed for writing\n");
	  perror(errstring);	  
	  ret = false; 
	} //top if

 DONE:      
  return ret;
}/*rename file*/
      

#endif // FILE_UTILS_H 



/*END*/
