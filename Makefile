#!/bin/make
# GNU MAKEFILE

#/* AUTHOR:  Chris Reid <spikeysnack@gmail.com> */#
#/* LICENSE: Free for all purposes              */#
#/* COPYRIGHT: 2015- Chris Reid                 */#
#SHELL :=/bin/sh

DEBUG := 1

CC := $(shell which cc)

#CC= clang


DEFINES= -D DSTRING
#BASE_CFLAGS= -m32 -fstrict-aliasing -fpic  -Wno-unused-function $(DEFINES) 
BASE_CFLAGS=  -std=c2x -fstrict-aliasing -fpic  -Wno-unused-function $(DEFINES) 
DEBUG_CFLAGS= -ggdb3  -Wall -Werror  -fno-omit-frame-pointer  
INCLUDES= $(PWD)
LIBINCLUDES= $(PWD)
# rpath tells the linker where to find the library 
LIBPATH= -Wl,-rpath=$(LIBINCLUDES)/ 
LIBS= -ldstring 

STATIC_LIBS= -l $(PWD)/libdstring.a 

ARCHIVE= $(PWD)/dstring.tgz
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

BINARIES= libdstring.so  libdstring.a 

# test binaries are in the tests directory (make tests)
TEST_BINARIES= dstringtest listtest arraytest memtest replace_test1 replace_test2 empty_test


options:
	@echo  "Make Options:"
	@echo  "all 		(default) makes library and all test binaries"
	@echo  "tests 		makes all tests in tests directory"
	@echo  "lib     	makes shared library (default)"
	@echo  "static  	makes static version of dstring library  (libdstring.a)"
	@echo  "install 	installs library and headers (super user)"
	@echo  "uninstall 	removes library and headers"
	@echo  "dist  		makes a tarball (dstring.tgz)of all files in current dir"

help:	options

all:	$(BINARIES)


install:	lib
	sudo  cp libdstring.so.1 libdstring.so $(INSTALLPATH)/lib
	sudo  cp dstring.h /usr/local/include/
	sudo  cp localdstring.h /usr/local/include/
	sudo  cp dstring_llist.h dstringlist.h dstringarray.h $(INSTALLPATH)/include/
	@echo  "libdstring.so  installed to /usr/local/lib. headers to /usr/local/include."
	@echo  "to use:  include dstring.h and -l dstring in your compilation string"

uninstall:
	sudo  rm  $(INSTALLPATH)/lib/libdstring.* 
	@echo  "libdstring.so removed"
	sudo  rm  $(INSTALLPATH)/include/dstring* 
	@echo  "dstring  headers removed"


tests:	all 
	make -C tests all
#	make -C tests tests



dstringtest:	libdstring.so testmain.o
	$(CC) $(CFLAGS)   -o dstringtest  testmain.o  -L $(LIBINCLUDES) $(LIBPATH)  $(LIBS)

listtest: libdstring.so listtest.o 
	$(CC) $(CFLAGS)   -o listtest    listtest.o  -L $(LIBINCLUDES)  $(LIBPATH)  $(LIBS)

arraytest:	libdstring.so arraytest.o
	$(CC) $(CFLAGS)   -o arraytest     arraytest.o  -L $(LIBINCLUDES)  $(LIBPATH)  $(LIBS)

memtest:	libdstring.so memtest.o
	$(CC) $(CFLAGS)   -o memtest     memtest.o  -L $(LIBINCLUDES)  $(LIBPATH)  $(LIBS)

replace_test1:	replace_test.o dstring_utils.o
	$(CC) $(CFLAGS)   -o replace_test1   replace_test.o   dstring_utils.o -L $(LIBINCLUDES)  $(LIBPATH)  $(LIBS) 

replace_test2:	replace_test2.o dstring_utils.o
	$(CC) $(CFLAGS)   -o replace_test2   replace_test2.o   dstring_utils.o -L $(LIBINCLUDES)  $(LIBPATH)  $(LIBS) 

replace_match:	libdstring.so 
	cd replace_match && $(MAKE)

dsdictionary:	libdstring.so
	cd dsdictionary && $(MAKE)

changecase:	libdstring.so
	cd changecase && $(MAKE)

empty_test:	libdstring.so empty_test.o 
	$(CC) $(CFLAGS)   -o empty_test   empty_test.o  -L $(LIBINCLUDES)  $(LIBPATH)  $(LIBS) 

RENAME_DIR=rename

.PHONY: rename changecase dsdictionary dist clean install

rename:
	$(MAKE) -C $(RENAME_DIR)


rename_static:	rename.o dstring_utils.o static 
	$(CC) $(CFLAGS)  -static  -o rename   rename.o   dstring_utils.o   libdstring.a  

arraytest.o:	arraytest.c
	$(CC) $(CFLAGS)   -c arraytest.c  

listtest.o:  listtest.c  dstringlist.h
	$(CC) $(CFLAGS)   -c listtest.c  

memtest.o:  memtest.c  libdstring.so
	$(CC) $(CFLAGS)   -c memtest.c

empty_test.o:	empty_test.c dstring_utils.o libdstring.so
	$(CC) $(CFLAGS)   -c empty_test.c  

replace_test.o:	tests/replace_test.c dstring_utils.o libdstring.so
	$(CC) $(CFLAGS)   -c replace_test.c  

replace_test2.o:	replace_test2.c dstring_utils.o libdstring.so
	$(CC) $(CFLAGS)   -c replace_test2.c  

dstring_utils.o:	dstring_utils.h dstring_utils.c 
	$(CC) $(CFLAGS)   -c dstring_utils.c  

lib:	libdstring.so
	@echo  "libdstring.so created (link with -L .  -ldstring )"

# this is the shared lib
libdstring.so:	dstring.o dstring_utils.o
	$(CC) $(CFLAGS) -shared    -o libdstring.so.1  dstring.o dstring_utils.o
	ln -sf libdstring.so.1 libdstring.so


static:	libdstring.a

#if you need a static version of the library
libdstring.a:	dstring.o
	ar -cvq libdstring.a dstring.o
	@echo  "libdstring.a created ( add explicit path  (eg "/usr/local/lib/libdstring.a" to compile string )"

dstring.o:	dstring.h dstring.c
	$(CC) $(CFLAGS)  -c dstring.c


dist:	clean 
	if [ -f $(ARCHIVE) ] ; then	 \
		gunzip $(ARCHIVE) ;      \
		tar -uvf dstring.tar ;   \
		gzip  dstring.tar ;      \
		mv dstring.tar.gz dstring.tgz ; \
	else                             \
		tar -cvzf dstring.tgz ../dstring/* ; \
	fi;

clean:
	rm -f  *.o  *.a *~ \#* core $(BINARIES) libdstring.so* libdstring_plus.so*  
	make -C tests clean
	make -C rename clean
	make -C changecase clean
	make -C replace_match clean
	make -C dsdictionary clean


