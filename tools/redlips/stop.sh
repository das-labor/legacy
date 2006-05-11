#!/bin/sh
iptables -D INPUT -p tcp --dport 1234 -j QUEUE
