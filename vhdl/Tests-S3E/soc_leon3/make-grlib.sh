#!/bin/sh

PREFIX=grlib
LIBPATH=" -Pghdlwork"
GHDL=ghdl
GHDLOPTS="-Pghdlwork --workdir=ghdlwork"

mkdir -p ghdlwork
echo > grlib.prj


for lib in `cat $PREFIX/libs.txt`; do 
	if [ -e $PREFIX/$lib/dirs.txt ]; then
		libname=`echo $lib | sed '/\// s%.*/%%'`
		echo "===== Library: $libname ====="
		for dir in `cat $PREFIX/$lib/dirs.txt`; do
			if [ -e $PREFIX/$lib/$dir/vhdlsyn.txt ]; then
				for fname in `cat $PREFIX/$lib/$dir/vhdlsyn.txt`; do
					file="$PREFIX/$lib/$dir/$fname"
					if [ -e $file ]; then
						echo "$file"
						echo "$vhdl $libname $file" >> grlib.prj
						$GHDL -a $GHDLOPTS --work=$libname $file
					else
						echo "!! $file not found";
					fi
				done
			fi

			if [ -e $PREFIX/$lib/$dir/vhdlsim.txt ]; then
				for fname in `cat $PREFIX/$lib/$dir/vhdlsim.txt`; do
					file="$PREFIX/$lib/$dir/$fname"
					if [ -e $file ]; then
						echo "$file"
						$GHDL -a $GHDLOPTS --work=$libname $file
					else
						echo "!! $file not found";
					fi
				done
			fi
		done
	else 
		echo "!! $PREFIX/$lib/dirs.txt not found"
	fi
done

