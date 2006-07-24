#!/bin/bash
privoxypid=`cat var/run/privoxy`
torpid=`cat var/run/tor`
echo "Killing Privoxy.."
killall privoxy
  if [ $? == 0 ]; then
	echo "DONE"
  else
	echo "FAILED"
  fi

echo "Killing Tor..."
killall tor
  if [ $? == 0 ]; then
	echo "DONE"
  else
	echo "FAILED"
  fi

echo "Killing Dillo..."
killall dillo
  if [ $? == 0 ]; then
	echo "DONE"
  else
	echo "FAILED"
  fi
