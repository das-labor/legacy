#!/bin/sh
iptables -A INPUT -p tcp --dport 1234 -j QUEUE
perl redlips.pl
iptables -D INPUT -p tcp --dport 1234 -j QUEUE
