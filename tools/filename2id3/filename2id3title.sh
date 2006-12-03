#!/bin/bash

CONTENT=`ls $1`

# set $IFS to end-of-line
ORIGIFS=$IFS
IFS=`echo -en "\n\b"`


for i in $CONTENT; do
TITLE=`echo $i | sed s/^.*.[0-9].-.// | sed s/.mp3$//`
/usr/bin/id3tool -t "$TITLE" $i
done

# set $IFS back
IFS=$ORIGIFS

