# Dstrings  
##	  A Dynamic Strings Library

##### Programmer: [Chris Reid](mailto:spikeysnack@gmail.com)
##### Year	    2016
##### Version     1.6


### INTRO

  Hello and welcome.
                                                                                                                                                             
  dstrings are dynamic *(runtime-allocated)* strings 
  that carry their used and free size with them.
 
  In memory they look like this:   
```C

    [ --------------- struct dst ----------------- ]   
     ____________________________________....  ____    
    | len | free | str                        |'\0'|   
    |_____|______|_______________________.... |____|   
                 ^ dstring                           <figure 1>  
```
  In C code they look like this:
```C

    struct dst   
        {   
            size_t len;   
            size_t free;   
            char[] str;   
        };
   
```

  They are addressed by the char* member of their struct ('str' above)
  so that they behave exactly like regular c-style char strings.  
  The structure size is insignificant in memory usage compared 
  to the potential size of the string data. 
  But having the size and free data  greatly speeds up operations 
  on the strings such as appending, truncating, resizing, and editing. 
  
  When operating on a dstring as a memory object you need to 
  address its struct address and its size_t members, 
  but otherwise you use it just like another string. 
  C sees a pointer to a char array (char*) 
  and libc functions know exactly what to do with it, 
  ignoring the 2 size_t variables before the (char*).  
  However, an extra element of safety is introduced 
  by always knowing exactly the size and free memory available 
  in the dstring, and never exceeding it, 
  or reallocating before a buffer overrun occurs.


#### LICENSE
	Free for all C uses. No guarantees implied, or given.
	The file dstring_llist.h is based upon the linux kernel
	linked list code and is GPL freeware. 
	

#### INSTALLATION

	Installation is by default into /usr/local/lib
	and /usr/local/include and requires sudo priveleges.
	Alternatively you may put libdstring.so.1 and its
	link libdstring.so and the associated headers
	dstring.h, localdstring.h, dstring_llist.h,
	dstringlist.h, and dstringarray.h
	in devel locations of your choosing. 

	dstring_llist.h ,dstringlist.h, and dstringarray.h  
	are optional, but useful.

	Use a standard 'make' or 'make all' or "make install" command.
	 

    Steps:

	-- run the shell script build.sh 
	   This chooses and runs the Makefile for
	   your OS. (BSD or GNU)

	-- make all 

	-- put "#include dstring.h"  in your C program.

	   add dstring.o to your compilation string.
	   
	   or

	-- make libdstring.so (default)

	-- put libdstring.so in a dir recognized by your compiler chain	
	   or use rpath  (relative path)   
	   example in Makefile: 
	   	      		LIBINCLUDES =$(PWD)  
				-L -Wl,-rpath=$(LIBINCLUDES)/
  
	-- put dstring.h in a dir recognized by your compiler chain	
           or the current dir

	-- put "#include dstring.h"  in your C program.

	-- add -ldstring to your compilation command.
	
	-- compile and run
	
	-- libdstring.so does not know about dstringlists.
	   For those you need to include dstringlist.h and dstring_llist.h, as well.

	-- Example programs are provided for testing and educational purposes.
`

#### IMPLEMENTATION
   
  As in figure 1 above, the layout is simple, yet some explanation is
  necessary to avoid common mistakes in programs.

  The psuedo-type dstring is simply and alias for (char*).

  It is accepted as a c-style string by all standard C string functions.

  To avoid confusions in the dstring library the alias cstring is used for (char*) 
  to indicate a regular pointer to an  array of chars and not a dstring.
     

  Wrapping the character data is the following structure:
 ```C

    struct dst  
        {   
	    size_t len;     // number of chars used   
   	    size_t free;    // number of chars not used but available   
   	    char   str[];   // actual c-style string   
  	};  
   
 ```
 
 For consistency, speed, and accuracy, it is mandatory that

      1. all memory allocations occur at multiples of 16 bytes.
         Strings can be of any length, always terminated with '\0'.
	 d->len + d->free will always be a multiple of 16.
	  
      2. all memory operations on dstrings (and thus on dst structures)
         occur only on the whole dst struct and never on the str part
         only.

      3. changing a dstring requires a recalculation of its size variables.
    	      

#### NORMAL USAGE

Pretty simple, normal object-style C programming:
```C
   
    dstring ds = new_dstring_init( "a 16 char string" , 32 );  // 16 bytes used , 16 bytes available

    printf( "Use just like a char*: %s \n", ds );

    printf( "dstring is %zu chars long with %zu free chars of space left.\n", dstlen(ds), dstfree(ds) );  
             /* %zu and %zi are the GNU printf format directives for  
 	     (unsigned) [size_t], and 
	     (signed)   [ssize_t] */
   
    DSFREE(ds); // A macro that nulls the pointer as well as frees the memory   

```

#### ADVANCED USAGE
	 
   To get at the characters in a dstring, just access them like
   regular c-strings.  `mydstring[4]` returns the 5th char of mydstring.

   The emptydstring() function returns  a dst structure with
   len = 0 , free=16, and str = "" (not NULL); Thus it passes
   consistency checks and is ready for a string to be appended.
          
   The utilities dstlen, dstfree, is_dstring are for general use.
   dstlen should be faster than strlen, but strlen works too, and 
   is often optimized by the compiler. 
   dst_mem returns the total size of the dst struct to help against
   comitting buffer overflows.

   Sharing dstrings -- The dstring type is a pointer type,
   so any dstring can be assigned to another dstring variable.
   Make sure to dettach pointers after all their accesses are done 
   except for one used to free the memory -- 
   this is just normal good practice in C. 
   dstrings have memory garbage in them before initializaton 
   so be sure to assign them to NULL and instantiate them before use.

### localdstrings

   Local dstrings -- stack-allocated local dstrings are provided
   for short-term use inside functions. 80% of strings in a 
   program are temporary and used read-only. localdstrings are
   automatically recycled at the return of the function when it
   exits. They must not be used as return values or as parameters
   to functions, or garbage and segmentation faults will result.
   Inside the function that they are instantiated in they retain
   integrity and may be used just as normal  locally statically-allocated
   c-style strings. They use the alloca functions provided by
   gcc and clang and others. tcc and pcc may not be able to 
   take advantage of local dstrings. Definitions for localdstrings
   are located in localdstring.h .

   If many concatenations (building long dstrings) are to be made,
   use a large buffer on creation to speed up processing and to reduce
   the number of internal reallocation (realloca0 calls. 
   When the concatenations are finished (or whenever optimally convienent), 
   call dstring_minimize_mem to release the free memory in the dstring.
   (see note in dstring.c at the dstring_append function.)   
   ##### Example
   
```C

    dstring longdstring = NULL;   
    longdstring = new_dstring_init("", 32768);  // 32K to start   
    longdstring = dstring_append(longdstring, "append this" );    
    ... (large number of appends ...)   

    dstring_minimize_mem( longdstring);  // free unused mem   

```
   Use the macros DSFREE and (DSLISTFREE for lists, DSARRAY_FREE for arrays) 
   to assure that dangling pointers are avoided.
   
#### DSTRINGLISTS
   dstringlists are an implementation of double-linked lists
   taken directly from the linux kernel implementation.
   A little study of the code in dstring_llist.h and dstringlist.h
   and the included test programs should suffice to utilize
   these powerful list facilites, although arrays are better for
   indexed access to containers of dstrings. All normal linked-list
   operations are provided.
 
#### DSTRINGARRAYS
   dstringarrays are simple structures of an array of dstring pointers
   and a size element. 
   Additions and deletions are quick and consist of assigning 
   and deassigning pointers.
   Generally you would not resize a dstringarray such as one would 
   with a linked list. Some utiltiy functions are provided.

#### DSTRING UTILS
   An example string manipulation utlity is provided: dstring_utils.
   It is both a string api and a program for working with strings. 
   It provides some string functions missing from stdlib that
   are traditionally re-written by every programmer for every
   program. (Why?) Most are part of other newer languages'
   base apis by now anyway, like find, find & replace, 
   remove a word from string, quote & unquote, chomp ,slice,truncate, 
   trim, pad etc. I encourage creativity in 
   discovering new and useful functions to round out the 
   functionalities.

#### ALIASES & MACROS
   C is flexible. Extremely. So is my brain. Most of these 
   functions start with the noun dstring, then an underscore,
   then a verb indicating the intent of the function. 
   Accordingly, 40% of the time I reverse them. 
   dstring_copy becomes copy_dstring in my head,
   new_dstring_init becomes dstring_new_init (oops! I just did it again!)
   So the function names are aliases with #defines so the compiler
   writes it correctly, and I can go on. 
   The other aliases are MACROs to make programming with libdstring
   easier and more intuitive. 
 
#### DSSCANF

  Parsing a dstring to extract variable data out of it into
  ints and substrings and floats, etc. is possible with the
  dsscanf utility. It takes the same format as libc sscanf
  and indeed gcc and clang will parse the format string
  for correctness at compile time just as if it were a sscanf 
  function. There is a catch, however. Just like sscanf, 
  dsscanf will seg fault if you send it an output parameter
  with insufficient memory to hold the data you are trying
  to stuff into it. Most of the time it will be another dstring
  that is too small or unitialized or just a pointer with no
  memory attached. Other problems could be bad values like reading
  a negative integer into a size_t var or placeing an  8-bit value 
  into a 16-bit numerical value not initialized to zero first, 
  leaving the upper 8 bits full of garbage.   
  Sit Programmis Cauti.
   
#### DSLOCAL
  
  DSLOCAL is a macro to generate localdstrings that are
  automatically destroyed at the exit of their local 
  function scope. They are allocated on the stack by
  the C runtime and so may be faster and more easy to use.

#### DSLOCAL_FMT

  The DSLOCAL_FMT macro is functionally like the printf family: 
  it  creates a localdstring either from a static string 
  in quotes or with a printf format string and variables. 
  gcc and clan check the format string forsyntax just as if it were for printf. 
  There is a defined MAX size of 256 bytes to save memory for localdstrings, 
  but that is easily changed  by editing localdstring.h
  and changing the LOCALMAX definition to a larger multiple of 16 
  if, for some reason, you need more than 256 chars for a single 
  local string  inside of a function.

  Example:
```C
    void f()   
        {   
            localdstring My_Name = DSLOCAL("Christopher");   

	    localdstring ld = DSLOCAL_FMT( "RULE %d: %s never returns %s", 1 , My_Name ,  "localdstrings" );   
                                       |                               |     |               |             
                                       printf format                   int   dstring         c-style string   

        }  /* (My_Name and ld are autmatically de-allocated when f returns) /*/   

```

#### BUGS

   It's plain C, so the code is subject to bad input and especially
   null pointer exceptions. I have tried to make it null-pointer
   proof as much as nominally possible, but using a pointer before
   it has been initialized or after it has been freed still gums
   up the works royally in any C program. 
   Be carefull about assigning stray pointers at the beginning 
   and the end of the life or your dstrings, especially containers. 
   The size_t data is there to protect the programmer from mistakes 
   we all make, so use them. 
   Dropped pointers can leave massive memory leaks
   until the program is over or cause crashes. The library has
   been tested extensively for this, and internally it remains
   orthogonal and integral as regards to memory and pointers
   in almost all cases, even up to very large dstring lengths. 
   localdstrings ,if used correctly, always get cleaned up 
   automatically. gcc and clang do some wily optimizations
   but should not destroy data before the last access to a 
   dstring in the code execution path.

   Most crashes occur on two operations:
   writing to a pointer that is uninitialized or points to 
   some random unexpected memory location, often off-limits to
   the program; or writing past the boundaries of an array.

   An other time is when the contents of the string within
   the dstring contain errors, and a libc function like strlen --
   (a little utility with a big temper), segfaults on its own.
   
#### MODIFYING DSTRINGS
	  
   The dst struct is designed to be accessed by the string part
   and then pointer arithmetic is used to get a pointer handle
   to the structure itself. If the relationship memory-wise is
   altered, all hell will break loose and the program will crash, hard.
   A compromise is made between facility and memory security
   by casting a pointer out of the char* internally to the 
   address of the beginning of the struct.

   A compromise was struck for code-reuse by adding 8 bytes of
   filler to the struct for 32-bit compilers. This is wasted
   space, but it allows the dst struct to be uniform in size
   accross 64-bit and 32-bit machines, despite the difference
   in byte size of 4 bits for 32 and 8 bytes for 64 of the size_t
   unsigned integer. DSTRING_MAX has been set to a 1GB limit
   which is within both archetecture's size_t limit and most likeley
   a dstring will never need to be that big. (I hope!).
```C

    struct dst* d = (struct dst*) (ds - sizeof(*d));   

```  
 
   ( DSTPTR(ds); is a macro for the above, 
     because it is used so much internally.)

   This incantation gives the necessary information to the compiler and the
   tiny runtime facilities of the malloc/realloc/free infrastructure
   to properly address the struct without a prior reference to its 
   beginning in memory. This works because the char* array is at
   the tail end of the struct <figure 1>. 
   You then can access the struct's members by name 
   (d->len , d->free , d->str ). 


   Debugger's do not like this: it looks to their algorithms as
   if you are exceeding the bounds of objects, and they give some
   access errors if too many safeguards are turned on, but 
   actually you are not doing so. 
   The idea of addressing an object from an inside member 
   is novel, and not-at-all recommended unless it offers a large 
   benefit. 

   To examine a (struct dst) in a debugger such as gdb,
   address it like this: 

```gdb

    (gdb) p a   
    $1= (dstring) 0x603020 "ABCDE"   
    (gdb) p (struct dst*) (a-16)     
    $2 = (struct dst *) 0x603010     
    (gdb) p *(struct dst*) (a-16)    
    $3 = {len = 5, free = 27, str = 0x603020 "ABCDE"}   
    (gdb)   

```    
   In this case the benefit is that you can address
   and use the structs as an ordinary c-style strings and
   all the libc string ops work as usual.

   The (ds - sizeof(*d)) idiom works for both 32 and 64 bit
   architectures and should be scalable to other kinds of 
   structs such as binary-strings or wide_char strings or
   (unsigned char), where the specific case of (ds - 16) 
   would not. It is also possible to add data members after the
   char[] without penalty and access them from the struct,
   but caution need to be used because one could find 
   oneself in a little maze of twisting passages, all different.
      
#### IMPROVEMENTS

   There is always room for improvement in code. 
   Experiment; stumble; break; repair; leap forward. 
   Get better by doing, by impact.   
   I appreciate suggestions and/or criticisms from users and
   experts and welcome submissions. Flames go to /dev/null.
   happy coding!      

   ### Enjoy!    
   
