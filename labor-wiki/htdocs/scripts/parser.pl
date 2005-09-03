#!/usr/bin/perl -w
# Wiki Termine 2 rss 
# Joern Bratzke <joern@bratzke.info>
# $Id$

while (<STDIN>){
push @lines,$_;
}
foreach (@lines){
next unless $_ =~ /^     \* (\d\d\.\d\d\.\d\d) .+\[(\d*)\]/;
push @linklist,$2;
push @datumlist,$1;
push @eventlist,$_;
}
foreach (@lines){
	$line = $_;
	next unless $_ =~ /^[ ]{2,3}(\d+)\./;
	foreach (@linklist){
		if ($_ == $1){
			push @reallinks,$line;
		}
	}
}
$i=0;
foreach(@eventlist){
$key={"datum"=> $datumlist[$i],"event" => $eventlist[$i], "Link" => $reallinks[$i],};
push @events,$key;
$i++;
}
print '<start>';
foreach (@events){ 
	%hash = %{$_};
	print '<event><date>';
	print $hash{"datum"};
	print '</date>';
	print '<desc>';
	print $hash{"event"};
	print '</desc>';
	print '<link>';
	print $hash{"Link"};
	print '</link>';
	print '</event>';
	print '</start>'

}
