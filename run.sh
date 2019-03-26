#!/bin/bash

mkdir -p /tmp/test /tmp/test2
rm -rf /tmp/test/* /tmp/test2/*

cp /bin/ls /bin/date /bin/echo /bin/ping /bin/true /bin/false /tmp/test/
cp /bin/sh /bin/zsh  /bin/bash /bin/ifind /usr/sbin/ifconfig /tmp/test2/
