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
# Security Warning
# ----------------
# Due to an eval of user-defined perl code (regular expressions) the user can
# execute system() calls. Do not setuid this script and do not screen it away
# for everyone to attach.
#
# Requirements
# ------------
# CONFIG_IP_NF_QUEUE
# iptables
# IPTables::IPv4::IPQueue
# NetPacket::IP
# Net::RawSock http://www.hsc.fr/ressources/outils/rawsock/index.html.enk
#
# Contributors
# ------------
# Felix Groebert <redlips@baraddur.de>
#
# Usage
# -----
# iptables -A INPUT -p tcp --dport 1234 -j QUEUE
# sudo perl redlips.pl
# nc -l -v -v -p 1234
# nc localhost 1234
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
# optimieren von NetPacket-> aufrufe
# term::console
# nfnetlink_queue
# log to file
# unified output sub
# better dump_ascii escape non printable perl like \x0f
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
use constant TIMEOUT => 1_000_000 * 2;    # 2 seconds

#
# config
#

# general options
my %o = ( debug_level => 9 );

# colors
my %c = (
  normal      => "\x1b\x5b" . "m",
  debug       => "\x1b\x5b" . "31m",
  src_ip      => "\x1b\x5b" . "35m",
  src_port    => "\x1b\x5b" . "36m",
  dest_ip     => "\x1b\x5b" . "35m",
  dest_port   => "\x1b\x5b" . "36m",
  mark        => "\x1b\x5b" . "37m:",
  unimportant => "\x1b\x5b" . "37m"
);

# rules
#
#
my @r;
radd("tcp any:any <> any:any s/felix/xilef/i");
radd("tcp any:any <> any:any s/felix/xilef/i");
radd("tcp any:any > any:any s/foobar/barfoo/i");
radd("tcp any:any <> any:any s/asdf/fdaslef/i");
radd("tcp any:any <> any:any s/asdf(as/df/i");    # invalid
radd("udp any:any <> any:any s/AAAAA/BBBBBB/i");

#
# rule subs
#
sub radd {
  my $string = shift;
  my $hash   = {};

  if (
    $string =~ m/
        (tcp|udp)\s #proto
        (any|\d+\.\d+\.\d+\.\d+): # ip1
        (any|\d+)\s #port1
        (\>|\<\>)\s #direction
        (any|\d+\.\d+\.\d+\.\d+): # ip2
        (any|\d+)\s #port2
        (.*$) #regex
        /x
    )
  {
    if ( validateregex($7) ) {    # everything is valid
      push @r, [ $1, $2, $3, $4, $5, $6, $7 ];
      return 1;
    }
  }

  return 0;
}

#
# main loop
#
my $swap;
my $queue = new IPTables::IPv4::IPQueue(
  copy_mode  => IPQ_COPY_PACKET,
  copy_range => 2048
  )
  or die IPTables::IPv4::IPQueue->errstr;

bug( "starting packet while loop", 7 );

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

  # http://www.perlmonks.org/?node_id=359606
  my $test = shift;
  my ( $code, $simple, $quanti, $regexp );

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

  if ( $test =~ /^$regexp$/ ) {
    return 1;
  }
  else {
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

# print the ip, tp and data
sub dump_itd {
  my ( $msg, $ip, $tp, $data ) = @_;
  my $string;

  $string = "[udp] " if ( $ip->{proto} == 58 );
  $string = "[tcp] " if ( $ip->{proto} == 6 );

  $string .=
      $c{src_ip}
    . $ip->{src_ip}
    . $c{mark}
    . $c{src_port}
    . $tp->{src_port}
    . $c{normal} . " > "
    . $c{dest_ip}
    . $ip->{dest_ip}
    . $c{mark}
    . $c{dest_port}
    . $tp->{dest_port} . " ";

  print $string . dump_ascii( $tp->{data} ) . $c{normal} . "\n";

  if ($data) {
    print "[modified] " . $string . dump_ascii($data) . $c{normal} . "\n";
  }
}

#
# packet handling
#
sub pdrop {
  my $msg = shift;
  bug( "dropped " . $msg->packet_id, 5 );
  $queue->set_verdict( $msg->packet_id, NF_DROP );
}

sub pinject {
  my $pkt = shift;
  bug( "injected packet", 5 );
  Net::RawSock::write_ip($pkt);
}

sub paccept {
  my $msg = shift;
  bug( "acceptd " . $msg->packet_id, 5 );
  $queue->set_verdict( $msg->packet_id, NF_ACCEPT );
}

sub preassemble {
  my ( $msg, $ip, $tp, $data ) = @_;
  my $raw;

#  bless $ip, NetPacket::IP;
#  bless $tp, NetPacket::TCP if ( $ip->{proto} == 6 );
#  bless $tp, NetPacket::UDP if ( $ip->{proto} == 58 );

  $tp->{data} = $data;
  $ip->{data} = $tp->encode($ip);
  $raw        = $ip->encode;

  dump_itd( $msg, $ip, $tp, $data );

  $queue->set_verdict( $msg->packet_id, NF_ACCEPT, length($raw), $raw );
}

sub phandle {
  my $msg = shift;
  my ( $rule, $data );
  my $ip = NetPacket::IP->decode( $msg->payload() );
  my $tp;

  bug(
    "recieved "
      . $msg->data_len
      . " byte packet "
      . $msg->packet_id
      . " on device "
      . $msg->indev_name . "/"
      . $msg->outdev_name
      . " with hook "
      . $msg->hook . " and ip layer: "
      . $ip->{src_ip} . " > "
      . $ip->{dest_ip},
    7
  );

  if ( $ip->{proto} == 58 ) {    # udp
    my $tp = NetPacket::UDP->decode( $ip->{data} );
    if ( $tp->{data} ) {
      $data = $tp->{data};
      dump_itd( $msg, $ip, $tp );
    }
    else {
      bug( "skipping empty packet", 7 );
      paccept($msg);             # skipt empty packets
      return;
    }
  }
  elsif ( $ip->{proto} == 6 ) {    # tcp
    my $tp = NetPacket::TCP->decode( $ip->{data} );
    if ( $tp->{data} ) {
      $data = $tp->{data};
      dump_itd( $msg, $ip, $tp );
    }
    else {
      bug( "skipping empty packet", 7 );
      paccept($msg);               #skip empty packets (syn)
      return;
    }
  }

  # walk through rules
  for $rule ( 0 .. $#r ) {
    bug(
      "rule: "
        . $r[$rule][0] . " "
        . $r[$rule][1] . ":"
        . $r[$rule][2] . " "
        . $r[$rule][3] . " "
        . $r[$rule][4] . ":"
        . $r[$rule][5] . " "
        . $r[$rule][6],
      7
    );

    if ( ( $r[$rule][0] eq "tcp" and $ip->{proto} == 58 )
      or ( $r[$rule][0] eq "udp" and $ip->{proto} == 6 ) )
    {
      bug( "skipping rule: wrong proto", 7 );
      next;
    }

    unless (
      (
        $r[$rule][3] eq "<>"
        and (
          (
                ( $r[$rule][1] eq $ip->{src_ip}  or $r[$rule][1] eq "any" )
            and ( $r[$rule][4] eq $ip->{dest_ip} or $r[$rule][4] eq "any" )
          )
          or (  ( $r[$rule][4] eq $ip->{src_ip} or $r[$rule][4] eq "any" )
            and ( $r[$rule][1] eq $ip->{dest_ip} or $r[$rule][1] eq "any" ) )
        )
      )
      or (
        $r[$rule][3] eq ">"
        and ( ( $r[$rule][1] eq $ip->{src_ip} or $r[$rule][1] eq "any" )
          and ( $r[$rule][4] eq $ip->{dest_ip} or $r[$rule][4] eq "any" ) )
      )
      )
    {
      bug( "skipping rule: wrong network layer", 7 );
      next;
    }

    unless (
      (
        $r[$rule][3] eq "<>"
        and (
          (
                ( $r[$rule][2] eq $tp->{src_port}  or $r[$rule][2] eq "any" )
            and ( $r[$rule][5] eq $tp->{dest_port} or $r[$rule][5] eq "any" )
          )
          or (  ( $r[$rule][5] eq $tp->{src_port} or $r[$rule][5] eq "any" )
            and ( $r[$rule][2] eq $tp->{dest_port} or $r[$rule][2] eq "any" ) )
        )
      )
      or (
        $r[$rule][3] eq ">"
        and ( ( $r[$rule][2] eq $tp->{src_port} or $r[$rule][2] eq "any" )
          and ( $r[$rule][5] eq $tp->{dest_port} or $r[$rule][5] eq "any" ) )
      )
      )
    {
      bug( "skipping rule: wrong transport layer", 7 );
      next;
    }

    # network options match, now apply the regex
    eval "\$data =~ $r[$rule][6]";

    if ( length $@ ) {
      bug(
        "skipping rule: error in eval of rule number " . $rule
          . " regex "
          . $r[$rule][6] . ": $@",
        2
      );
      next;
    }

    # reassemble and re-inject the packet
    bug( "rule matches ... reassemble", 7 );
    preassemble( $msg, $ip, $tp, $data );

  }

  # accept non-matching packages
  paccept($msg);
}

#eof
