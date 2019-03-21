#!/bin/bash

mkdir -p /tmp/test /tmp/test2
rm -rf /tmp/test/* /tmp/test2/*
cp /bin/ls /bin/date /bin/echo /tmp/test/
cp /bin/zsh /bin/sh /bin/bash /tmp/test2/
