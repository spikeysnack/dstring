#!/usr/bin/env bash

# array of filenames
flist=("1.txt" "5.txt" "9.txt" "D.txt" "H.txt" "L.txt" "P.tx"         \
    "2.txt" "6.txt" "A.txt" "E.txt" "I.txt" "M.txt" "Q.txt" "test.h" \
    "3.txt" "7.txt" "B.txt" "F.txt" "J.txt" "N.txt" "R.tx"           \
    "This is a long one.txt" "4.txt" "8.txt " "C.txt" "G.txt"        \
    "K.txt" "O.txt" "S.tx" "T" "U" "V" "W" "X" "Y" "Z")

mkdir files.dir
cd files.dir

for name in "${flist[@]}"
do
 touch  "$name"
# echo -e " [$name] \n" 
done

ls

../rename -f txt TXT *.txt

ls

../rename -f TXT txt *.TXT
 

for name in "${flist[@]}"
do
rm "$name"
done

cd ..
rm -r files.dir

#END


