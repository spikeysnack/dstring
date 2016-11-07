#!/usr/bin/env bash

[[ ! -x ./changecase ]] && echo "WOW. Can't run changecase. Build it?" && exit -1

[[ -d "files.dir" ]] && rm -rf "files.dir" 

#!/usr/bin/env bash

# array of filenames
flist=("1.txt" "5.txt" "9.txt" "D.txt" "H.txt" "L.txt" "P.tx"         \
    "2.txt" "6.txt" "A.txt" "E.txt" "I.txt" "M.txt" "Q.txt" "test.h" \
    "3.txt" "7.txt" "B.txt" "F.txt" "J.txt" "N.txt" "R.tx"           \
    "This is a long one.txt" "4.txt" "8.txt " "C.txt" "G.txt"        \
    "K.txt" "O.txt" "S.tx" "T" "U" "V" "W" "X" "Y" "Z")

FLIST_UPPER=("${flist[@]^^}")
flist_lower=("${flist[@],,}")
flist_sentence=("${flist[@]^}")


mkdir files.dir
cd files.dir

for name in "${flist[@]}"
do
 touch  "$name"
# echo -e " [$name] \n" 
done

ls -C
../changecase -l -f "${flist[@]}"

ls -C

../changecase -u -f  "${flist_lower[@]}"

ls -C

../changecase -t -f  "${FLIST_UPPER[@]}"

ls -C

#echo "FLIST : ${FLIST[@]}"

#echo "flist : ${flist[@]}"

#ls -C

#cd ..
#rm -r files.dir
#exit


#END


exit 0
