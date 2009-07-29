#!/bin/bash

rm slide-*.{ppm,bmp}

echo "pdftoppm()..."
pdftoppm -r 200 main.pdf slide

echo "converting to .bmp..."
for f in `ls --color=never *.ppm` ; do
	convert $f `echo $f | sed -r 's/\.ppm/.bmp/'`
done
