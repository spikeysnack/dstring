/* changecase help.h */

/* friendly messages */

#ifndef CHANGECASE_HELP_H
#define CHANGECASE_HELP_H

#include <stdio.h>

void usage(FILE* f);
void changecase_help(FILE* f);
void version(FILE* f);
void no_change(FILE* f);

/* usage message */
void usage(FILE* f)
{

  cstring usage_str = 
    "usage: changecase <option>  <file1> <file> ...\n"
    "options: \n"
    "\t -h    help \n"
    "\t -l    lower case\n"
    "\t -s    sentence case\n"
    "\t -t    title case\n"
    "\t -u    upper case\n"
    "\t -v    version \n"
    "\t -x    change file extension as well as name (default: NO ) \n"
    "\t -X    change suffix only (default: NO ) \n"
    "\tnote: (*) processes all files in current dir\n";

  version(f);
  fprintf( f , "%s\n" , usage_str );
}


void changecase_help(FILE* f)
{
  printf("changes the letter case of a  filename list of filenames.\n" ); 

  usage( f );

  cstring ex1 = 
  "\n\tEXAMPLE:\t to change \'cat.txt\' to CAT.txt \n"
  "\t changecase -u cat.txt\n";

  cstring ex2 = 
    "\n\tEXAMPLE:\t to change \'THIS IS A FILE.txt\' to title case \n"
    "\t changecase -t \"THIS IS A FILE.txt\" \n"
    " ==> \"This is a File.txt\" \n";

  fprintf( f , "%s\n" , ex1 );
  fprintf( f , "%s\n" , ex2 );
  
}


void version(FILE* f)
{

#ifndef CHANGECASE_VERSION
#error RENAME_VERSION is not Defined yet
#endif
  fprintf( f, "%s version:\t %s  date:\t%s\n", "rename",  CHANGECASE_VERSION, CHANGECASE_DATE );

}

void no_change(FILE* f)
{
  cstring errstr = "NO CHANGE OPTION SPECIFIED\n\n";
  fprintf( f , "%s\n" , errstr );
  usage( f );  
}


#endif

/*END*/
