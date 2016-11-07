/* do_all.h*/

/** a macro rig that is a hybrid statically/dynamicially
* allocated set of objects for things like lists and arrays.
*
* Theory of Operation
* I.   A single macro creates a container for the objects 
*       and becomes essentially an atomic operation, even though
*       it may have many steps.
*
* II.  The container creation macro is embedded into a 
*       variable argument-length macro that instantiates
*       entries for the container and adds them into it.
*
* III. A do-while(0) single-iteration loop encapsulates the
*       static creation of a C-primitive array that holds the
*       arguments that the CPP symbol __VA_ARGS__ expands to.
*
* IV.  A sentinel other than NULL is chosen as an array end-marker
*       (because NULL is a valid input) in this example (NULL-1).
*
* V.   A static array is allocated local to the do/while block
*       with NULL as arg[0] , __VA_ARGS__ as arg[1] ..arg[n],
*       and ARGLISTEND as the end-marker. The 2 ## before the
*       __VA_ARGS__ tells the macro processor to "eat" the comma
*       before it if there are no arguments preceding it. This 
*       allows for 0 arguments to the macro if needed, and suppresses
*       argument errors from the compiler.
*
* VI.  The arguments are peeled off the internal array
*       by starting at arg[1] and stopping at ARGLISTEND. 
*       inserting the arguments  into the container with an appropriate
*       inline function and possibly verified at that time.
*       The function returns should be void 
*       or ignorable like int or bool. The static array memory
*       is recovered automatically at block exit.
*
* VII. A single statement at the very end makes the macro
*       "output" the container's pointer. This is a gcc thing
*       but has been adopted by most compilers.
*       
* VIII. The entire macro "function" is wrapped with 
*        ( __extension__ ({  ... })) to suppress pedantic
*        warnings and to tell the compiler that the code inside
*        is all one block. 
*
* IX.  Usage is convenient: 
*        Example:
*
*	        stringlist sl1 = SLIST();
*		stringlist sl2 = SLIST( "A" , "B", "C", "D", E");
*	        stringlist sl3 = SLIST( name, address, phone );
*
* X.  Destruction macros can operate the same way, freeing
*      each member of the container 
*      and then the container itself. 
*
*/

#include "list.h" /* linux kernel-linked list */



typedef struct st_list
{
  char* str;
  struct list head list;
}string;

#define SLIST_CREATE() ( __extension__ ({                                \
   stringlist SL = (stringlist)calloc(1, sizeof(struct st_list));       \
    INIT_LIST_HEAD(&SL->list);                                          \
SL; }))


#define  SARGLISTEND  ((string) NULL-1)

#define SLIST(...)   ( __extension__ ({                            \
  stringlist _sl =  SLIST_CREATE();				   \
  do { size_t n = 1;						   \
	string sarr[] = { NULL, ##__VA_ARGS__ , SARGLISTEND };	   \
  while ( sarr[n] != SARGLISTEND )                                 \
	{                                                          \
         if ( is_string(sarr[n]) ){ append_string(_sl, sarr[n]);}  \
         else {append_string(_sl, new_string(sarr[n]));}	   \
	 n++;                                                      \
        }							   \
  }while(0);                                                       \
  _sl;                                                             \
}))


/* these make it "look right" when coding. 
 * slist s = new_stringlist( str1 , str2 .. .);
 */
#define new_slist  SLIST
#define new_stringlist  SLIST



