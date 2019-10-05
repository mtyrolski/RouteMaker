#!/usr/bin/env bash
if [ $# -le 1 ] || [ ! -e "$1" ];
then
	exit 1;
fi

re='^[0-9]+$'
DIR=$1
flag=0
shift
while test ${#} -gt 0
do
    if ! [[ $1 =~ $re ]] ; 
    then
        flag=1
        break
    fi
    if [ $1 -lt 1 ] || [ $1 -gt 999 ]
    then
        flag=1
        break
    fi
    while IFS='' read -r line || [[ -n "$line" ]]; 
    do
        if [ -z "$line" ]
        then
            continue
        fi
        IFS=';' read -r -a array <<< "$line"
        array_size=${#array[@]}
        if [ $1 == ${array[0]} ];
        then
            sum=0
            for ((i = 2 ; i < $array_size ; i+=3 )); 
            do 
                sum=$((sum+${array[i]})); 
            done
        echo "$1;$sum"
        break
        fi
    
    done < $DIR
  shift
done
exit $flag