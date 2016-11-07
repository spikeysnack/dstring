#!/bin/make
# GNU MAKEFILE

#/* AUTHOR:  Chris Reid <spikeysnack@gmail.com> */#
#/* LICENSE: Free for all purposes              */#
#/* COPYRIGHT: 2015- Chris Reid                 */#

DEBUG=1

#CC= gcc
CC= clang

DEFINES= -D DSTRING
#BASE_CFLAGS= -m32 -fstrict-aliasing -fpic  -Wno-unused-function $(DEFINES) 
BASE_CFLAGS=   -fstrict-aliasing -fpic  -Wno-unused-function $(DEFINES) 
DEBUG_CFLAGS= -ggdb3  -Wall -Werror  -fno-omit-frame-pointer 

INCLUDES= -I..   # from Makefile in parent
 
LIBINCLUDES= -L.. 

# rpath tells the linker where to find the library 
LIBPATH= -Wl,-rpath=$(LIBINCLUDES) 
LIBS= -ldstring 

STATIC_LIBS= -l../libdstring.a 

ARCHIVE= ../dstring.tgz
INSTALLPATH= /usr/local
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

BINARIES= dic dhtest

.PHONY:	test clean

all:	$(BINARIES) test

test:	
	./dic
	./dhtest

dic:	dic.c
	$(CC) $(CFLAGS)   $(INCLUDES) -o dic dic.c $(LIBINCLUDES) $(LIBPATH)  $(LIBS)

dhtest:	dhtest.c
	$(CC) $(CFLAGS)   $(INCLUDES) -o dhtest dhtest.c $(LIBINCLUDES) $(LIBPATH)  $(LIBS)

clean:
	rm -f  *.o  *.a *~ \#* core $(BINARIES) 



