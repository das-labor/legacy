#!/bin/sh
#
# tor    The Onion Router
#
# Startup/shutdown script for tor. This is a wrapper around torctl;
# torctl does the actual work in a relatively system-independent, or at least
# distribution-independent, way, and this script deals with fitting the
# whole thing into the conventions of the particular system at hand.
# This particular script is written for Red Hat/Fedora Linux, and may
# also work on Mandrake, but not SuSE.
#
# These next couple of lines "declare" tor for the "chkconfig" program,
# originally from SGI, used on Red Hat/Fedora and probably elsewhere.
#
# chkconfig: 2345 90 10
# description: Onion Router - A low-latency anonymous proxy
#

# Library functions
if [ -f /etc/rc.d/init.d/functions ]; then
   . /etc/rc.d/init.d/functions
elif [ -f /etc/init.d/functions ]; then
   . /etc/init.d/functions
fi

# Increase open file descriptors a reasonable amount
ulimit -n 8192

TORCTL=/data/Data/projekte/DilloTor/tor-0.1.1.23/binary/bin/torctl

# torctl will use these environment variables
TORUSER=_tor
export TORUSER
TORGROUP=_tor
export TORGROUP

if [ -x /bin/su ] ; then
    SUPROG=/bin/su
elif [ -x /sbin/su ] ; then
    SUPROG=/sbin/su
elif [ -x /usr/bin/su ] ; then
    SUPROG=/usr/bin/su
elif [ -x /usr/sbin/su ] ; then
    SUPROG=/usr/sbin/su
else
    SUPROG=/bin/su
fi

case "$1" in

    start)
    action $"Starting tor:" $TORCTL start
    RETVAL=$?
    ;;

    stop)
    action $"Stopping tor:" $TORCTL stop
    RETVAL=$?
    ;;

    restart)
    action $"Restarting tor:" $TORCTL restart
    RETVAL=$?
    ;;

    reload)
    action $"Reloading tor:" $TORCTL reload
    RETVAL=$?
    ;;

    status)
    $TORCTL status
    RETVAL=$?
    ;;

    *)
    echo "Usage: $0 (start|stop|restart|reload|status)"
    RETVAL=1
esac

exit $RETVAL
