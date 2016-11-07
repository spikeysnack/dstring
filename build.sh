#!/bin/sh
# build.sh
# builds for linux or bsd

#/* AUTHOR:  Chris Reid <spikeysnack@gmail.com> */#
#/* LICENSE: Free for all purposes              */#
#/* COPYRIGHT: 2015- Chris Reid                 */#


ARCH=$(uname)
MAKE=$(which make)
MAKEFILE="Makefile"

# pick makefile
if [ ! -f "Makefile" ]
then

    if [ "X$ARCH" = "XLinux" ] ; then
	MAKEFILE=Makefile.GNU
    elif [ "X$ARCH" = "XFreeBSD" ] ; then       
	MAKEFILE=Makefile.BSD
    elif [ "X$ARCH" = "XOpenBSD" ] ; then       
	MAKEFILE=Makefile.BSD
    fi
    
    ln -s "${MAKEFILE}" Makefile
fi


#find a C compiler

DEFAULT_CC=$(which cc 2>/dev/null)

GCC=$(which gcc 2>/dev/null)

CLANG=$(which clang 2>/dev/null)

PCC=$(which pcc 2>/dev/null)


if [ ! -z "$DEFAULT_CC" ]
    then
    CC="${DEFAULT_CC}"
elif [ ! -z  "$CLANG" ]
    then
    CC="${CLANG}"
elif [ ! -z "$GCC" ]
    then
    CC=$"{GCC}"
elif [ ! -z "$PCC" ]
    then
    CC=$"{PCC}"
else
    echo "NEED TO SET CC as compiler in MAKEFILE"
    exit 1
fi


export CC="${CC}"

#do make 
if [ $# -eq 0 ] ; then
      ${MAKE} -f ${MAKEFILE} clean all  #default 
else
    ${MAKE} -f ${MAKEFILE} "$*"
fi



