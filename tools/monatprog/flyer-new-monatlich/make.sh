#!/bin/sh

NAME="veranstaltung"

cat head.tex > $NAME.tex
python do.py >> $NAME.tex
cat foot.tex >> $NAME.tex

latex $NAME.tex
dvips -t landscape $NAME.dvi -o $NAME.ps
ps2pdf14 $NAME.ps
rm $NAME.aux $NAME.dvi $NAME.log $NAME.ps

evince $NAME.pdf

