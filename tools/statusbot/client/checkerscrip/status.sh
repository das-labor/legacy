#!/bin/bash


# wenn investigate, dann selber versuchen rauszubekommen
STATUS='investigate'

HOST_TO_PING="10.0.1.3"

#STATUS='online'
#STATUS='offline'

#
# der rueckgabewert dieses scripts setzt den status des statusbots
# auf www.das-labor.org
#
# dieses script wird vom status-client etwa alle 30 sekunden aufgerufen und 
# wartet dann auf dessen terminierung. der status unknown wird vom 
# jabberclient auf www.das-labor.org selbstaendig gesetzt wenn er 
# die verbindung zum client verlohren hat
#

if [ x${STATUS} == x'online' ]; then
        exit 0
fi

if [ x${STATUS} == x'offline' ]; then
        exit 1
fi

if [ x${STATUS} == x'investigate' ]; then
        ping -c 2 ${HOST_TO_PING} 2>&1
        ec=$?
        if [ $ec == 0 ];then
                exit 0
        fi
        exit 1
fi

