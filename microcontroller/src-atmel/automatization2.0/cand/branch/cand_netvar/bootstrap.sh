#!/bin/bash

aclocal
automake -a -c -f
autoconf
./configure
make
