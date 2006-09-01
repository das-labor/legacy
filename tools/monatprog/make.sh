#!/bin/sh

fmt -s -w 78 Monatspr.tex > tmp
mv tmp Monatspr.tex

latex Monatspr.tex
dvips -t landscape Monatspr.dvi -o Monatspr.ps
ps2pdf14 Monatspr.ps
rm Monatspr.aux Monatspr.dvi Monatspr.log Monatspr.ps

