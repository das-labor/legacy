#!/usr/bin/perl -w
#
# RewriteProxy.pl
# Version 0.01
# (c) Felix Groebert 2005
# http://groebert.org/felix/
#
#  [remote]<--->[router]<--->[client]
#
# note to myself:
# read byte wise - not line by line
# while (sysread($client, $byte, 1) == 1) { print STDOUT $byte; }
#
#
# Problems:
#
# 1) It seem kind of slow
#    - use fork after main loop
#    - event loop and no forks?
#
# 2) When rerouting traffic with my router's iptables change the destination host
#    to localhost for all tcp/80 packets, how can I then determine the original destination
#    host of that packet when ($client = $listen->accept) becomes true?
#
#    - SO_ORIGINAL_DST in getsockopt()
#     ---> embed c code :\
#
#

use IO::Socket;
use Net::hostent;
use strict;

# for logging use
sub logmsg { print "$0 [$$] [", scalar localtime, "] @_\n" }

# config
my $listen_addr = '10.0.0.3';
my $listen_port = 80;
my $remote_addr = '10.0.0.1'; 
my $remote_port = 80;

my ($client, $line, $listen, $remote, $childpid);

#setup

system("iptables -t nat -A PREROUTING -p tcp -d $remote_addr --dport $remote_port -j DNAT --to-destination $listen_addr:$listen_port");

#iptables -t nat -A PREROUTING -p tcp --dport 443 -d 217.114.210.119 -j DNAT --to-destination 127.0.0.1
#iptables -t nat -A PREROUTING -p tcp --dport 443 -s 192.168.0.3  -j DNAT --to-destination 192.168.0.99:443

$listen = new IO::Socket::INET ( 
                               Proto     => 'tcp',
                               LocalAddr => $listen_addr,
                               LocalPort => $listen_port,
                               Listen    => 20, # or SOMAXCONN
                               TYPE      => SOCK_STREAM,
                               Reuse     => 1 )
or die logmsg("cannot listen $!") unless $listen;

logmsg("accepting connections at $listen_addr:$listen_port and proxying for $remote_addr:$remote_port");

# Main Loop
while ($client = $listen->accept()) {
    logmsg("Connection from ".$client->peerhost);

    $remote = IO::Socket::INET->new( Proto => "tcp",
                                 PeerAddr => $remote_addr,
                                 PeerPort => $remote_port )
    or die logmsg("cannot connect remote $!");

    logmsg("remote connected $remote_addr:$remote_port");
    
    # split the program into two processes, identical twins
    die "can't fork: $!" unless defined($childpid = fork());
    
    # this is true in the parent process
    # communication from remote to client
    if ($childpid) {
        while(defined($line = <$remote>)) {
            $line =~ s/apache/IIS/gi;
            print $client $line;
        }
        # when connection closes term-inate child
        kill("TERM",$childpid);
    }
    # this else is true only in the child process
    # communication from client to remote
    else {
        while(defined($line = <$client>)) {
            print $remote $line;
        }
    }

    close $remote;
    close $client;
}

close $listen;

