#!/bin/bash

n=`date  +"backup__%F_%X"`

mkdir $n;
cp *.c *.h *.S *.txt *.ps *.hex Makefile Doxyfile $n


