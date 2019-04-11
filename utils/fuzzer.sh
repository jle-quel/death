#!/bin/bash

cd /tmp

echo "int main(void) { }" > main.c
gcc main.c -o a.out
LEN=680
COUNT=0

while [ $COUNT -lt $LEN ] ; do
	echo $COUNT
	gcc main.c -o /tmp/test/corrupted.out
	gcc main.c -o /tmp/test2/corrupted.out
	dd if=/dev/urandom count=4 bs=1 seek=$COUNT of=/tmp/test/corrupted.out conv=notrunc status=none
	dd if=/dev/urandom count=4 bs=1 seek=$COUNT of=/tmp/test2/corrupted.out conv=notrunc status=none
	/home/vagrant/Documents/war/war
	if [[ $? != 0 ]]; then
		python -c "print hex($COUNT)"
		echo "ERR"
		break
	fi
	COUNT=$[$COUNT+4]
done

cd
