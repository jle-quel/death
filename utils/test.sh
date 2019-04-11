#!/bin/bash

RESET="\e[39m"
RED="\e[31m"
GREEN="\e[32m"

COUNT=0
TOTAL=0

for filename in /tmp/test/* /tmp/test2/* ; do
	printf "$filename -> "
	RESULT=$(xxd $filename | head | grep "PWN")
	if [[ "$RESULT" =~ "PWN" ]] ; then
		((COUNT+=1))
		printf "$RED INFECTED $RESET\n"
	else
		printf "$GREEN CLEAN $RESET\n"
	fi
	((TOTAL+=1))
done

printf "\n$COUNT/$TOTAL\n"
