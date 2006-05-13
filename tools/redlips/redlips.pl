#!/usr/bin/perl

# redlips.pl - red live intrusion prevention system
# -------------------------------------------------
# redlips.pl is a frontend for viewing and modifying packets
# in real time. It suits best as a layer 7 filter in capture
# the flag style linux deathmatchs.

# License
# -------
# Copyright (C) 2006 Felix Groebert <redlips@baraddur.de>
# version 0.1
# Homepage:
# Download:
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
#
# Requirements
# ------------
# CONFIG_IP_NF_QUEUE
# iptables
# IPTables::IPv4::IPQueue
# NetPacket::IP
# Net::RawSock http://www.hsc.fr/ressources/outils/rawsock/index.html.enk
# Regexp::Parser
#
# Contributors
# ------------
# Felix Groebert <redlips@baraddur.de>
#
# Usage
# -----
#
# ChangeLog
# ---------
# 0.1
#   - initial release
#
#
# Todo
# ----
# gute abkuerzung fuer red
# copyright & license
# bessere doku
# weniger NetPacket-> aufrufe
# oo?
#
# -> milestone ncurses
# -> milestone regex-machine
# ---> release 0.1
#

#
# init
#
package redlips;
use strict;
$^W = 1;

use IPTables::IPv4::IPQueue qw(:constants);
use NetPacket::IP;
use NetPacket::ICMP;
use NetPacket::UDP;
use NetPacket::TCP;
use Net::RawSock;
use Regexp::Parser;
use constant TIMEOUT => 1_000_000 * 2;    # 2 seconds

#
# config
#

# general options
my %o = (
    debug_level => 9
);

# colors
my %c = (
    normal      => "\x1b\x5b" . "m",
    debug       => "\x1b\x5b" . "31m",
    ipsrc       => "\x1b\x5b" . "35m",
    tcpdst      => "\x1b\x5b" . "36m",
    mark        => "\x1b\x5b" . "37m:",
    unimportant => "\x1b\x5b" . "37m"
);

# rules
#
#
my @r = [];
radd("tcp any:any <> any:any s/felix/xilef/i");
radd("tcp any:any <> any:any s///i");
radd("tcp any:any <> any:any s/asdf(as/df/i");

#
# rule subs
#
sub radd
{
    my $rule = shift;
    if ( rverify($rule) ) {
        push @r, $rule;
        return 1;
    } else {
        return 0;
    }
}

sub rverify
{
    my $rule = shift;
    bug("verifying rule $rule",8);

    $rule =~ m/
        (tcp|udp|icmp)\s #proto
        (any|[0-9\*]\.[0-9\*]\.[0-9\*]\.[0-9\*]): # ip1
        (any|[0-9]{})\s #port1
        (\>|\<|\<\>)\s #direction
        (any|[0-9\*]\.[0-9\*]\.[0-9\*]\.[0-9\*]): # ip2
        (any|[0-9])\s #port2
        (.*$) #regex
        /x;

    bug("proto:$1 src:$2 port:$3 direction:$4 dst:$5 port:$6 regex:'$7'",8);
    if(validateregex($7)) {
      bug("sucess",8);
      return 1;
    } else {
      bug("err",8);
      return 0;
    }
}

# testing eval of regex XXX
exit;

#
# main loop
#
my $swap;
my $queue = new IPTables::IPv4::IPQueue(
    copy_mode  => IPQ_COPY_PACKET,
    copy_range => 2048
  )
  or die IPTables::IPv4::IPQueue->errstr;

while (1) {
    my $msg = $queue->get_message(TIMEOUT);
    if ( !defined $msg ) {
        next if IPTables::IPv4::IPQueue->errstr eq 'Timeout';
        die IPTables::IPv4::IPQueue->errstr;
    }

    if ( $msg->data_len() ) {    # skip empty packets
        phandle($msg);
    }
}

#
# helper subs
#
sub validateregex {
  my $test = shift;
  my ($code, $simple, $quanti, $regexp);

  $code = qr/(?: (?:
      [^{}]                  # not a curly brackets
  |   \{ (??{ $code }) \}    # balanced curly brackets
  )* )/x;

  $simple = qr/(?:
      \\ [pP] \{ \w+ \}                                       # \p{Prop}
  |   \\.                                                     # escape
  |   \[ (?: \\. | [^\]] )+ \]                                # [range]
  |   \( \? \# [^)]+ \)                                       # (?#text)
  |   \( \? [imsx]* -? [imsx]* (?: : (??{ $regexp }) )? \)    # (?imsx-imsx)
  |   \( \? (?: [:=!>] | <[=!] ) (??{ $regexp }) \)           # (?=pattern)
  |   \( \? \?? \{ (??{ $code }) \} \)                        # (?{ code })
  |   \( \? \( (?: \d+ | \? [=!<] (??{ $regexp }) | \? \{ (??{ $code }) \} ) \)
          (??{ $regexp }) (?: \| (??{ $regexp }) )? \)        # (?(1)y|n)
  |   \( (??{ $regexp }) \)                                   # parenthesis
  |   [^\\|()\[\]*+?{}]                                       # single char
  )/x;

  $quanti = qr/(?:
      (?: $simple (?: [+*?] \?? | \{ \d+ (?: , \d* )? \} \?? )?
  )* )/x;

  $regexp = qr/(?:
      $quanti (?: \| $quanti )*
  )/x;

  if($test =~ /^$regexp$/) {
    return 1;
  } else {
    return 0;
  }
}

sub bug {
    my $info  = shift;
    my $level = shift;
    if ( $info and $level le $o{debug_level} ) {
        print $c{debug};
        print "[debug] $info";
        print $c{normal} . "\n";
    }
}

#
# dumper subs
#

# take a binary string and dump it in ascii or ^s
sub dump_ascii {
    my $input = shift;
    my ( $i, $char, $return );
    $return = " ";

    for ( $i = 0 ; $i < length $input ; $i++ ) {
        $char = substr( $input, $i, 1 );
        if ( ord $char > 31 ) {
            $return = $return . $c{normal} . $char;
        }
        elsif ( ord $char == 9 ) {
            $return = $return . $c{unimportant} . "\\t" . $c{normal};
        }
        elsif ( ord $char == 10 ) {
            $return = $return . $c{unimportant} . "\\n" . $c{normal};
        }
        else {
            $return = $return . $c{unimportant} . "^" . $c{normal};
        }
    }
    return $return;
}

sub dump_tcp_nice {
    my ( $payload, $ip, $tcp );
    $payload = shift;

    $ip  = NetPacket::IP->decode($payload);
    $tcp = NetPacket::TCP->decode( $ip->{data} );

    if ( $tcp->{data} ) {    # skip empty tcp packets
        print "[tcp] "
          . $c{ipsrc}
          . $ip->{src_ip}
          . $c{mark}
          . $c{tcpdst}
          . $tcp->{dest_port}
          . $c{mark}
          . dump_ascii( $tcp->{data} )
          . $c{normal} . "\n";
    }
}

sub dump_packet {
    my ( $payload, $ip );
    $payload = shift;

    $ip = NetPacket::IP->decode($payload);

    print <<EOT;
[IP Header]
Version           : $ip->{ver}
Header Length     : $ip->{hlen}
Flags             : $ip->{flags}
Frag. Offset      : $ip->{foffset}
TOS               : $ip->{tos}
Length            : $ip->{len}
ID                : $ip->{id}
TTL               : $ip->{ttl}
Protocol          : $ip->{proto}
Checksum          : $ip->{cksum}
Source IP         : $ip->{src_ip}
Destination IP    : $ip->{dest_ip}
Options           : $ip->{options}

EOT
}

sub dump_meta {
    my $msg = shift;

    print <<EOT;
[Metadata]
Packet ID         : @{[ $msg->packet_id() ]}
Mark              : @{[ $msg->mark() ]}
Timestamp (sec)   : @{[ $msg->timestamp_sec() ]}
Timestamp (usec)  : @{[ $msg->timestamp_usec() ]}
Hook              : @{[ $msg->hook() ]}
In Device         : @{[ $msg->indev_name() ]}
Out Device        : @{[ $msg->outdev_name() ]}
HW Protocol       : @{[ $msg->hw_protocol() ]}
HW Type           : @{[ $msg->hw_type() ]}
HW Address Length : @{[ $msg->hw_addrlen() ]}
HW Address        : @{[ unpack('H*', $msg->hw_addr()) ]}
Data Length       : @{[ $msg->data_len() ]}
EOT
}

#
# packet handling
#
sub pdrop
{
    my $msg = shift;
    bug( "dropped " . $msg->packet_id, 5 );
    $queue->set_verdict( $msg->packet_id, NF_DROP );
}

sub paccept
{
    my $msg = shift;
    bug( "acceptd " . $msg->packet_id, 5 );
    $queue->set_verdict( $msg->packet_id, NF_ACCEPT );
}

sub pinject
{
    my $pkt = shift;
    bug( "injected packet",5);
    Net::RawSock::write_ip($pkt);
}

sub phandle
{
    my $msg = shift;
    bug( "got ".$msg->data_len." byte packet ".$msg->packet_id." on device ".$msg->indev_name."/".$msg->outdev_name." with hook ". $msg->hook, 5 );

    my $ip = NetPacket::IP->decode( $msg->payload() );

    if ( $ip->{proto} == 1 ) { # icmp packets
        my $icmp = NetPacket::ICMP->decode( $ip->{data} );

        if ( $icmp->{type} == 8 ) {    # icmp echo request
            pdrop($msg);

            #
            # sample icmp ping reply faker/injector
            #

            # change packet
            $icmp->{type}  = 0;
            $icmp->{cksum} = 0;

            $ip->{data}    = $icmp->encode;
            $swap          = $ip->{dest_ip};
            $ip->{dest_ip} = $ip->{src_ip};
            $ip->{src_ip}  = $swap;

            bug( "injected fake echo reply", 5 );
            pinject( $ip->encode );

            return;
        }
    }
    elsif ( $ip->{proto} == 58 ) {    # udp
    }
    elsif ( $ip->{proto} == 6 ) {     # tcp
        my $tcp = NetPacket::TCP->decode( $ip->{data} );

        dump_tcp_nice( $msg->payload() );

        if( $tcp->{data} ) {
            bug( "testing packet for modification", 5);
            $tcp->{data} =~ s/felix/xilef/g;
            if(length $&) { # XXX is this the correct var?
                bug( "modifying packet", 5);
                $ip->{data} = $tcp->encode($ip); # build tcp packet
                pdrop($msg);
                pinject($ip->encode); # build ip packet
            }

            paccept($msg);

            return;
        }

    }

    # accept any other packages
    paccept($msg);
}

#eof


