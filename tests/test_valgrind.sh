#!/bin/bash

PROG=routemaker
TESTDIR=$1

LEAKEXITCODE=42
VALGRINDFLAGS="--error-exitcode=$LEAKEXITCODE --leak-check=full \
--show-leak-kinds=all --errors-for-leak-kinds=all -q"

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'
CORRECT=0
WRONG=0

for f in $TESTDIR/*.in; do

    echo  "Testing... $f"
	OUTF="$(mktemp)";

	valgrind $VALGRINDFLAGS ./$PROG < $f 1>$OUTF.out 2>$OUTF.err
	EXIT_CODE=$?
	if [ $EXIT_CODE -ne 0 ];
    then
    	echo -e "-------${RED}WRONG exit code: $EXIT_CODE ${NC}";
		((WRONG+=1));
		cat $OUTF.err
    else
        echo -e "-------${GREEN}GOOD exit code: $EXIT_CODE ${NC}";
		((CORRECT+=1));
    fi
        echo ""

	rm -f $OUTF $OUTF.out $OUTF.err;

done;

echo -e "Passed ${GREEN}$CORRECT${NC} tests, failed ${RED}$WRONG${NC} tests."
