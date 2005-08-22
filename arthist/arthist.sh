#!/bin/bash
case "$1" in
start)
    log_begin_msg "Starting up arthist..."
    start-stop-daemon --chdir /usr/local/arthist-0.3.1/ -c nobody --exec /usr/local/arthist-0.3.1/arthist --start -b
    log_end_msg $?
    ;;
stop)
    log_begin_msg "Shutting down arthist..."
    killall arthist
    log_end_msg $?
    ;;
*)
    log_success_msg "Usage: /etc/init.d/arthist {start|stop}"
    exit 1
    ;;
esac

exit 0
