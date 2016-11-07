/* key_utils.h */

/**
 **     Some utility options for basic key I/O  operations
 **   ( while technically part of dstring, this header file )
 **   ( is pure stdlib C. It can be used in any C program.  )
 **/

/* AUTHOR:  Chris Reid <spikeysnack@gmail.com> */
/* LICENSE: Free for all purposes              */
/* COPYRIGHT: 2015- Chris Reid                 */


#ifndef KEY_UTILS_H
#define KEY_UTILS_H


#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>


#define BUFSIZE 256


/* keypress utils */
void anykey(bool);
void pauseforkey(void);
char getkey( char* choice);


/**  waits for key press (any key) 
 **  if anykey is called with false 
 **  it waits for the enter key
 **/

/*  accept any key   */
/*********************/
void anykey(bool mode)
{

  struct termios info;
  
  tcgetattr(0, &info);          /* get current terminal attributes; fd 0 is stdin */
  
  if (mode) info.c_lflag &= ~ICANON;      /* disable canonical mode ( no return key) */
  else      info.c_lflag |=  ICANON;      /* enable canonical mode (return key needed)*/

  info.c_cc[VMIN] = 1;          /* wait for one keystroke  */
  info.c_cc[VTIME] = 0;         /* no timeout */
  tcsetattr(0, TCSANOW, &info); /* set immediately */
} //anykey



/* wait for a key */
/******************/
void pauseforkey()
{
  char c ='\0';

  anykey(true);  // set immediate mode

  c = getchar(); 

  anykey(false); // restore carriage ret  mode

  if (c == '\0' ) return;  // safety -- ignore null value
} //pauseforkey


/* wait for any key with a question*/
/******************/
void askforkey(char* ask)
{
  char c ='\0';

  if (strlen(ask) > 0 )
    fprintf(stdout,"<%s> ", ask);  

  anykey(true);  // set immediate mode

  c = getchar(); 

  anykey(false); // restore carriage ret  mode

  if (c == '\0' ) return;  // safety -- ignore null value
} //pauseforkey



/* returns a key press  
   with a prompt message
*/

/***********************/
char getkey( char* choice)
{
  char   c      ='\0';
  char*  fmt    = "<press %s>";
  size_t fmt_sz = strlen(fmt) + 1;
  char*  msg    = (char*)alloca(fmt_sz);
  int    strOK = 0;

  memset(msg, 0, fmt_sz);
  strOK = snprintf( msg, fmt_sz, fmt, choice); // printf syntax
  if ( strOK )
    fprintf(stdout, "%s\n", msg);
  else
    return (char) 0;



  anykey(true);  // set immediate mode

 cloop:
  c = getchar();  
  if (c == '\0' ) goto cloop;

  anykey(false); // restore carriage ret  mode

  return c;

} //getkey



#endif // KEYUTILS_H 



/*END*/
