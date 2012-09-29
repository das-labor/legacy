#!/bin/sh

GRAPH=$1

if [ ! -e $GRAPH-minutes.html ]; then
    echo "Could not fine $GRAPH-minutes.html"
    exit 1
fi

echo "!! WARNING !!"
echo 
echo "This script will overwrite"
echo "  $GRAPH-hours.html"
echo "  $GRAPH-days.html"
echo "  $GRAPH-weeks.html"
echo "  $GRAPH-months.html"
echo
echo "in 5 seconds. Press CTRL-C to abort..."
sleep 6

sed 's%start=-30m%start=-8h%'   < $GRAPH-minutes.html > $GRAPH-hours.html
sed 's%start=-30m%start=-8d%'   < $GRAPH-minutes.html > $GRAPH-days.html
sed 's%start=-30m%start=-60d%'  < $GRAPH-minutes.html > $GRAPH-weeks.html
sed 's%start=-30m%start=-400d%' < $GRAPH-minutes.html > $GRAPH-months.html

sed -i 's%meta http-equiv="refresh" content="30"%meta http-equiv="refresh" content="120"%'  $GRAPH-hours.html
sed -i 's%meta http-equiv="refresh" content="30"%meta http-equiv="refresh" content="3600"%' $GRAPH-days.html
sed -i 's%meta http-equiv="refresh" content="30"%meta http-equiv="refresh" content="3600"%' $GRAPH-weeks.html
sed -i 's%meta http-equiv="refresh" content="30"%meta http-equiv="refresh" content="3600"%' $GRAPH-months.html
