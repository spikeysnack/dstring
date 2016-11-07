#!/bin/bash 

# prepends a file to another file


if [ $# -lt 2 ] ; then
    echo "prepend <file1> <file 2>"
    exit
fi

# prepends $1 to $2, replaces $2

cp "$2" /tmp/2
cat "$1" > /tmp/1 
cat "$2" >> /tmp/1 && mv /tmp/1 "$2"

#end






