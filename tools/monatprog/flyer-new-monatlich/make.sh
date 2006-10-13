#!/bin/sh

NAME="veranstaltung"

fmt -s -w 78 $NAME.tex > tmp
mv tmp $NAME.tex

latex $NAME.tex
dvips -t landscape $NAME.dvi -o $NAME.ps
ps2pdf14 $NAME.ps
rm $NAME.aux $NAME.dvi $NAME.log $NAME.ps

gv $NAME.pdf

