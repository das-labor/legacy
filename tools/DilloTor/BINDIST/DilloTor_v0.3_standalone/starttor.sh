#!/bin/bash
curdir=`dirname $0`
echo "Running in $curdir"
echo "Starting Privoxy..."
cd $curdir
echo `pwd`
$curdir/sbin/privoxy --pidfile $curdir/var/run/privoxy --config $curdir/etc/privoxy/config
echo "Starting Tor..."
$curdir/bin/tor -f $curdir/etc/tor/torrc --PidFile $curdir/var/run/tor --DataDirectory $curdir/lib/tor &
echo "Starting Dillo..."
$curdir/bin/dillo
