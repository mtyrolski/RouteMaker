#!/bin/bash

if [ $# != 1 ];
then
	echo "required args: program name and path";
	exit;
fi

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'
PROG=routemaker
TESTDIR=$1
CORRECT=0
WRONG=0

SAVE_IFS=$IFS
IFS=$(echo -en "\n")


for f in $TESTDIR/*.in; do

	OUTF="$(mktemp)";
    echo "Saving temporary results to $OUTF";
    echo -n "Testing $f ...";
	./$PROG < $f 1>$OUTF.out 2>$OUTF.err
	EXIT_CODE=$?

	diff ${f%in}out $OUTF.out &>/dev/null
	OUT_DIFF=$?

	diff ${f%in}err $OUTF.err &>/dev/null
	ERR_DIFF=$?
	if [ $OUT_DIFF -eq 0 -a $ERR_DIFF -eq 0 -a $EXIT_CODE -eq 0 ];
	then
		echo -e " ${GREEN}OK${NC}";
		((CORRECT+=1));
	else
		if [ $OUT_DIFF -ne 0 ];
		then
		    echo ""
		    echo -e "-------${RED}wrong output${NC}";	
		    echo "----------Diff in .out = $OUT_DIFF"
		    diff ${f%in}out $OUTF.out
		fi
		if [ $ERR_DIFF -ne 0 ];
		then
			echo ""
		    echo -e "-------${RED}wrong diagnostic output${NC}";
		    echo "----------Diff in .err = $OUT_DIFF"s
		    diff ${f%in}err $OUTF.err
		fi
		((WRONG+=1));
	fi
	echo ""
	echo ""
	rm -f $OUTF.out $OUTF $OUTF.err
done;

IFS=${SAVE_IFS}

echo -e "Passed ${GREEN}$CORRECT${NC} tests, failed ${RED}$WRONG${NC} tests."
