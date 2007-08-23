#!/bin/bash
# resize-images.sh
# to resize images in a given directory tree

newsize=1440

echo "resizing all images in subdirectories of $1"
cd $1

for dir in `ls ./`; do 
	echo $dir
	cd $dir
	for ext in "jpg jpeg png gif"
	do 
		for img in `ls *.$ext`
			do
				if [ `identify -format "%w" $img` -gt $newsize ]; then
					echo converting $img
				  	convert -resize $newsize $img $img
				fi
			done
	done
done
