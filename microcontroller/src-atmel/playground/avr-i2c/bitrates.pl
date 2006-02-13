#!/usr/bin/perl
#
#					CPU freq
# SCL freq = ------------------------
#			  16 + 2(TWBR) * 4^(TWPS)
#
#

# a^n = e^(n * ln (a))



my $CPU_freq = 16000000; #16MHz
my $TWPS = 0;
my $TWBR = 0;

print "/* I2C Bus-speed parameters for AVR */";

for ($TWBR=0; $TWBR<=255; ++$TWBR){
#	printf "SCL = %f : TWBR=%u TWPS=%u\n",
#		$CPU_freq / (16+2*$TWBR*(4**$TWPS)) ,
#		$TWBR, $TWPS;
	printf "#define I2C_SPEED_%u\t%u,%u\t/* more exactly %f Hz*/\n",
		int ($CPU_freq / (16+2*$TWBR*(4**$TWPS))) ,
		$TWBR, $TWPS,
		$CPU_freq / (16+2*$TWBR*(4**$TWPS));

}

for ($TWPS=1; $TWPS<=3; ++$TWPS){
	for ($TWBR=0x40; $TWBR<=255; ++$TWBR){
#		printf "SCL = %f : TWBR=%u TWPS=%u\n",
#			$CPU_freq / (16+2*$TWBR*(4**$TWPS)) ,
#			$TWBR, $TWPS;
	printf "#define I2C_SPEED_%u\t%u,%u\t/* more exactly %f Hz*/\n",
		int ($CPU_freq / (16+2*$TWBR*(4**$TWPS))) ,
		$TWBR, $TWPS,
		$CPU_freq / (16+2*$TWBR*(4**$TWPS));
	}

}


