function ltgt()
{
    OLDIFS=$IFS
    IFS="$(printf '\n')" 

    mapfile thetxt < "$1"

    for line  in ${thetxt[@]}
    do
	line=${line//</&lt;}
	line=${line//>/&gt;}
	echo "$line"
    done
    IFS=$OLDIFS
}      



