/* rename.h */

/* help and options */

/* AUTHOR:  Chris Reid <spikeysnack@gmail.com> */
/* LICENSE: Free for all purposes              */
/* COPYRIGHT: 2015- Chris Reid                 */


#ifndef RENAME_H
#define RENAME_H

#include <stdio.h>
#include <unistd.h>
#include "dstring.h"

/* maybe stdbool is gone */
#ifndef __bool_true_false_are_defined
typedef enum {false = 0, true = 1} bool;

/* from objc */
static const bool NO    = false;
static const bool YES   = true;
#endif

#define eprintf(...)   fprintf( stderr, __VA_ARGS__ )

/* 
 * define some boolean flags (global variables)
*/
extern bool all_matches;
extern bool force_rename;
extern bool quiet_rename;

void usage(FILE* f);
void help(FILE* f);
void version(FILE* f);
int options ( int argc, char** argv);



/* usage message */
void usage(FILE* f)
{
  version(f);
  fprintf( f, "usage: rename  <match> <replacement>  <filename1> <filename2> ...\n");
  fprintf( f, "options:\n\t -a replace all matches in filename, (not just first match) \n");
  fprintf( f, "\t -f force rename (non-interactive)  \n");
  fprintf( f, "\t -h help \n");
  fprintf( f, "\t -q quiet \n");
  fprintf( f, "\t -v version \n");
  
  fprintf ( f,"\tnote: (*) processes all files in current dir\n");
}


/* help message */
void help(FILE* f)
{
  fprintf( f, "renames  a matching string in  a list of filenames.\n" ); 
  usage(f);
  fprintf( f, "\n\tEXAMPLE:\t to change \'doug\' in dougdog.txt  to good \n" ); 
  fprintf( f, "\t rename doug good dougdog.txt \n" );
  fprintf( f, "\t ==> gooddog.txt \n" ); 
}


/* version info */
void version(FILE* f)
{

#ifndef RENAME_VERSION
#error RENAME_VERSION is not Defined yet
#endif
  fprintf( f, "%s version:\t %s  date:\t%s\n", "rename",  RENAME_VERSION, RENAME_DATE );

}

/* 
 * commandline  options 
 * see getopt (3) manpage 
*/

/*  
 * options   
 * (a)ll  
 * (f)orce 
 * (h)elp 
 * (q)uiet  
 * (v)ersion 
*/

extern int optind; // <index of last option -- see unistd.h>

int options ( int argc, char** argv)
{
  int opt = 0;

  //  size_t index = 0;

  while ((opt = getopt(argc, argv, "afhqv")) != -1) 
    {
      switch (opt) {
	
      case 'a':
	all_matches = true;
	break;

      case 'f':
	force_rename = true;
	break;

      case 'h':
	help(stdout);
	exit(EXIT_SUCCESS);
	break;
	
      case 'q':
	quiet_rename = true;
	break;

      case 'v':
	version(stdout);
	exit(EXIT_SUCCESS);
	break;
	
      default: /* '?' */
	usage(stderr);
	exit(EXIT_FAILURE);
	
      }// switch(opt)
      
    }//while(opt)
  
  /*
    for (index = optind; index < argc; index++)
      fprintf (stderr, "Non-option argument %s\n", argv[index]);
  */
  return optind;

}/* options */


#endif

/*END*/
