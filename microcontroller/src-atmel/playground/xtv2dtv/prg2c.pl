#!/usr/bin/perl

print "#include <avr/pgmspace.h>\n\n";
print "uint8_t programm[] PROGMEM = {\n\t";

$b = 0;
getc(STDIN);
getc(STDIN);

while(($a = getc(STDIN))ne"" ){
	$b++;	
	printf("%#x,", vec($a,0,8) );
	if ($b == 16){
		$b=0;
		print "\n\t";
	}
}
print "};\n\n";

