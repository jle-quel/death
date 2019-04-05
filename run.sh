#!/bin/bash

################################################################################
### STATIC FUNCTIONS
################################################################################

function debug()
{
	cd /tmp
	echo "int main(void) { }" > main.c
	gcc main.c -o dyn
	clang main.c -o exe
	cp dyn test/
	cp exe test2/
	cd
}

function real()
{
	cp /bin/ls /bin/date /bin/echo /bin/ping /bin/true /bin/false /tmp/test/
	cp /bin/sh /bin/zsh  /bin/bash /bin/ifind /usr/sbin/ifconfig /tmp/test2/
}

function usage()
{
	echo "usage: ./run.sh [debug | real]"
	exit 1
}

################################################################################
### PUBLIC FUNCTION
################################################################################

mkdir -p /tmp/test /tmp/test2
rm -rf /tmp/test/* /tmp/test2/*

case "$1" in
	debug|d)
		debug	;;
	real|r)
		real	;;
	*)
		usage	;;
esac
