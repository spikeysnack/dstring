/* changecase_options.h */

/* 
 * getopt options 
 * "man 3 getopt" 
*/

/* AUTHOR:  Chris Reid <spikeysnack@gmail.com> */
/* LICENSE: Free for all purposes              */
/* COPYRIGHT: 2015- Chris Reid                 */


#ifndef CHANGECASE_OPTIONS_H
#define CHANGECASE_OPTIONS_H

#include <unistd.h>
#include "dstring.h"
#include "changecase.h"
#include "changecase_help.h"


/* maybe stdbool is gone */
#ifndef __bool_true_false_are_defined
typedef enum {false = 0, true = 1} bool;

/* from objc */
static const bool NO    = false;
static const bool YES   = true;
#endif


extern casechange_t change;

extern bool change_suffix;
extern bool change_suffix_only;
extern bool force_rename;
extern bool quiet_rename;

/*  
 * options
 * (f)orce   
 * (h)elp
 * (l)ower  
 * (s)entence 
 * (t)itle 
 * (u)pper  
 * (v)ersion 
*/

extern int optind;

int changecase_options ( int argc, char** argv);

int changecase_options ( int argc, char** argv)
{
  int opt = 0;

  //  size_t index = 0;

  while ((opt = getopt(argc, argv, "fhlqstuvxX")) != -1) 
    {
      switch (opt) {

      case 'f':
	force_rename = YES;
	break;
	
      case 'h':
	change = NONE;
	changecase_help(stdout);
	exit(EXIT_SUCCESS);
	break;
	
      case 'l':
	change = LOWER;
	break;

      case 'q':
	quiet_rename = YES;
	break;

      case 's':
	change = SENTENCE;
	break;

      case 't':
	change = TITLE;
	break;

      case 'u':
	change = UPPER;
	break;

      case 'v':
	change = NONE;
	version(stdout);
	exit(EXIT_SUCCESS);
	break;
	
      case 'x':
	change_suffix=YES;
	break;

      case 'X':
	change_suffix_only=YES;
	break;

      default: /* '?' */
	change = NONE;
	usage(stderr);
	exit(EXIT_FAILURE);
	
      }// switch(opt)
      
    }//while(opt)
  
  /*
    for (index = optind; index < argc; index++)
      printf ("Non-option argument %s\n", argv[index]);
  */

  return optind;
}/* options */



#endif

/*END*/
