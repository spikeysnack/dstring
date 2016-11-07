#!/bin/make
# GNU MAKEFILE

#/* AUTHOR:  Chris Reid <spikeysnack@gmail.com> */#
#/* LICENSE: Free for all purposes              */#
#/* COPYRIGHT: 2015- Chris Reid                 */#

DEBUG=1

#CC= gcc
#CC= clang

DEFINES= -D DSTRING
#BASE_CFLAGS= -m32 -fstrict-aliasing -fpic  -Wno-unused-function $(DEFINES) 
BASE_CFLAGS=  -fstrict-aliasing -fpic  -Wno-unused-function --optimize-strlen $(DEFINES) 


DEBUG_CFLAGS= -ggdb3  -O0 -Wall -Werror  -fno-omit-frame-pointer -Wno-unused-function

INCLUDES= -I..
LIBINCLUDES= -L..

# rpath tells the linker where to find the library 
LIBPATH= -Wl,-rpath=$(LIBINCLUDES) 
LIBS= -ldstring 

STATIC_LIBS= -l ../libdstring.a 

DEFINES += -D USE_SMALL_WORDS

ifdef DEBUG
DEFINES += -D DEBUG
CFLAGS= $(BASE_CFLAGS)  $(DEBUG_CFLAGS)
else
CFLAGS= $(BASE_CFLAGS)
DEFINES += -D NDEBUG  #turns off assert
endif

ifdef C_SRC
OBJS += ${C_SRC:.c=.o}
endif

BINARIES =  dstringtest localtest reverse listtest arraytest replace_test replace_test2 empty_test memtest

all:	$(BINARIES)	


run:	$(BINARIES)	
	./dstringtest
	./localtest
	./reverse
	./memtest
	./listtest
	./arraytest
	./replace_test
	./replace_test2 "This is a funny little sentence."  "a funny little" "actually a good" 
	./empty_test  ""
	./empty_test  "X"
	./empty_test  ''
	./empty_test  " "


testmain.o:	../dstring.o testmain.c
	$(CC) $(CFLAGS)	  $(INCLUDES)  -c testmain.c

dstringtest:	../libdstring.so testmain.o
	$(CC) $(CFLAGS)	  $(INCLUDES)   -o dstringtest  testmain.o    $(LIBINCLUDES) $(LIBPATH)  $(LIBS)

listtest: ../libdstring.so listtest.o 
	$(CC) $(CFLAGS)	  $(INCLUDES)   -o listtest    listtest.o     $(LIBINCLUDES)  $(LIBPATH)  $(LIBS)

localtest: ../libdstring.so localtest.o 
	$(CC) $(CFLAGS)	  $(INCLUDES)   -o localtest    localtest.o   $(LIBINCLUDES)  $(LIBPATH)  $(LIBS)

reverse: ../libdstring.so reverse.o 
	$(CC) $(CFLAGS)	  $(INCLUDES)   -o reverse    reverse.o       $(LIBINCLUDES)  $(LIBPATH)  $(LIBS)

arraytest:	../libdstring.so arraytest.o
	$(CC) $(CFLAGS)	  $(INCLUDES)   -o arraytest     arraytest.o  $(LIBINCLUDES)  $(LIBPATH)  $(LIBS)

memtest:	../libdstring.so memtest.o
	$(CC) $(CFLAGS)	  $(INCLUDES)   -o memtest     memtest.o      $(LIBINCLUDES)  $(LIBPATH)  $(LIBS)

# replace_test:	replace_test.o ../dstring_utils.o
# 	$(CC) $(CFLAGS)	  $(INCLUDES)   -o replace_test  replace_test.o   ../dstring_utils.o $(LIBINCLUDES)  $(LIBPATH)  $(LIBS) 

# replace_test2:	replace_test2.o ../dstring_utils.o
# 	$(CC) $(CFLAGS)	  $(INCLUDES)  -o replace_test2   replace_test2.o  ../dstring_utils.o $(LIBINCLUDES)  $(LIBPATH)  $(LIBS) 

# empty_test:	empty_test.o ../dstring_utils.o
# 	$(CC) $(CFLAGS)   $(INCLUDES) -o empty_test	empty_test.o	../dstring_utils.o	$(LIBINCLUDES)	$(LIBPATH)	$(LIBS) 

replace_test:	replace_test.o 
	$(CC) $(CFLAGS)	  $(INCLUDES)  -o replace_test  replace_test.o	$(LIBINCLUDES)  $(LIBPATH)  $(LIBS) 

replace_test2:	replace_test2.o 
	$(CC) $(CFLAGS)	  $(INCLUDES)  -o replace_test2 replace_test2.o	$(LIBINCLUDES)  $(LIBPATH)  $(LIBS) 

empty_test:	empty_test.o 
	$(CC) $(CFLAGS)   $(INCLUDES)  -o empty_test	empty_test.o	$(LIBINCLUDES)	$(LIBPATH)	$(LIBS) 

arraytest.o:	arraytest.c
	$(CC) $(CFLAGS)   $(INCLUDES)   -c arraytest.c  

listtest.o:  listtest.c
	$(CC) $(CFLAGS)	  $(INCLUDES)   -c listtest.c  

localtest.o:  localtest.c
	$(CC) $(CFLAGS)	  $(INCLUDES)   -c localtest.c  

reverse.o:  reverse.c
	$(CC) $(CFLAGS)	  $(INCLUDES)   -c reverse.c  

memtest.o:  memtest.c 
	$(CC) $(CFLAGS)   $(INCLUDES)   -c memtest.c

replace_test.o:	replace_test.c 
	$(CC) $(CFLAGS)   $(INCLUDES)   -c replace_test.c  

replace_test2.o:	replace_test2.c 
	$(CC) $(CFLAGS)   $(INCLUDES)   -c replace_test2.c  

empty_test.o:	empty_test.c 
	$(CC) $(CFLAGS)   $(INCLUDES)   -c empty_test.c  

clean:
	rm -f  *.o  *.a *~ \#* core $(BINARIES) 

