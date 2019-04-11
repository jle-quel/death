#!/bin/bash

################################################################################
### STATIC FUNCTIONS
################################################################################

function debug()
{
	cd /tmp
	echo "int main(void) { }" > main.c

	gcc main.c -o dyn_c
	clang main.c -o exe_c
	cp dyn_c test/
	cp exe_c test2/

	clang main.c -c -o exe_o
	gcc main.c -c -o dyn_o
	cp dyn_o test/
	cp exe_o test2/

	cat dyn_c| head -c 12328 > /tmp/test/stripped
	chmod 744 /tmp/test/stripped

	cd
}

function real()
{
	cp /bin/ls /bin/date /bin/echo /bin/ping /bin/true /bin/false /tmp/test/
	cp /bin/sh /bin/zsh  /bin/bash /bin/ifind /usr/sbin/ifconfig /bin/vim /bin/nano /tmp/test2/
}

function error()
{
	cp /bin/zdump /usr/sbin/ebtables-save /tmp/test/
	cp /sbin/jffs2reader /usr/bin/pnmnoraw /tmp/test2/
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
	err|e)
		error	;;
	*)
		usage	;;
esac
