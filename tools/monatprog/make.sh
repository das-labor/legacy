#!/bin/sh

latex Monatspr.tex
dvips -t landscape Monatspr.dvi -o Monatspr.ps
ps2pdf14 Monatspr.ps
rm Monatspr.aux Monatspr.dvi Monatspr.log Monatspr.ps

