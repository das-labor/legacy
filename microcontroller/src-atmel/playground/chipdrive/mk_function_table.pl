#!/usr/bin/perl

$prefix = "fkt__";

my $output = "";


my $main_commands = "";
my $main_commands_num = 0;

foreach (@ARGV){
	my $filename = $_;
	$filename =~ /^(.*)\.[^\.]*$/;
	
	$output = "#include \"".$filename."\"\n".$output;
	my $section_name = $1;
	my $c_prefix = $prefix.$section_name."__";

	open (FILE, "<", $filename) ||die "File $_ not found";

	my $commands = "";
	my $command_num = 0;
	
	while (<FILE>){
		my $line = $_;
		if ($line =~ /^\s*([^\(\)]+[\s\*]+)([^\(\)\s]+)\s*\(([^\(\)]*)\)\s*;/ ){
			my $argnames = "";
			my $argstruct = "";
			my $function = "";
			my $argnum = 0;
			my $ret = $1; my $name = $2; my $args = $3;
			#print "ret: $1\nnam: $2\narg: $3\n\n";
			while( $args =~ /^\s*([^,]+),?(.*)/ ) {
				$argnum++;
				$args = $2;
				$1 =~ /^(.*[\s\*]+)(\S+)\s*$/;
				my $argname = $2;
				
				my $argtype = "ptr";
				$argnames .= "const static char ".$c_prefix.$name."_".$argname."[] PROGMEM = \"$2\";\n";
				if ($1 =~ /^\s*char\s*\*/){
					$argtype = "str";
				}elsif (!($1 =~ /\*/ )){
					if($1 =~ /(unsigned\s*char)|uint8_t|u08/){
						$argtype = "uc";
					}elsif($1 =~ /(unsigned\s*int)|uint16_t|u16/){
						$argtype = "ui";
					}elsif($1 =~ /char|int8_t|u08/){
						$argtype = "c";
					}elsif($1 =~ /int|int16_t|u16/){
						$argtype = "i";
					}elsif($1 =~ /void/){
						$argtype = "vd";
					}
				}
				$argstruct.="{".$argtype.",".$c_prefix.$name."_".$argname."},";
			};
			my $argtype = "ptr";
				if ($ret =~ /^\s*char\s*\*/){
					$argtype = "str";
				}elsif (!($ret =~ /\*/ )){
					if($ret =~ /unsigned\s*char/){
						$argtype = "uc";
					}elsif($ret =~ /unsigned\s*int/){
						$argtype = "ui";
					}elsif($ret =~ /char/){
						$argtype = "c";
					}elsif($ret =~ /int/){
						$argtype = "i";
					}elsif($ret =~ /void/){
						$argtype = "vd";
					}
			}
			$function = "c_function ".$c_prefix.$name."__cfkt PROGMEM={\n";
			$function .= "\t(void(*)(void))&".$name.", {".$argtype.", 0},".$argnum.",{".$argstruct."}\n};\n";
			my $command_name = "const static char ".$c_prefix.$name."__name[] PROGMEM = \"".$name."\";\n";
			
			my $command = "const static command ".$c_prefix.$name."__command PROGMEM={\n";
			$command .= "\tfunction, ".$c_prefix.$name."__name, (void *)&".$c_prefix.$name."__cfkt\n};\n";
			
			$commands .= "\t&".$c_prefix.$name."__command,\n";
			$command_num++;
			
			$output .= "$argnames \n";
			$output .= "$function \n";
			$output .= "$command_name \n";
			$output .= "$command \n";
			
		};
	};
	my $command_table = "const static command_table ".$c_prefix."table PROGMEM={\n";
	$command_table .= "\t".$command_num.", {".$commands."}\n};\n";
	
	my $table_command_name = "const static char ".$c_prefix."__table__name[] PROGMEM = \"".$section_name."\";\n";

	my $table_command = "const static command ".$c_prefix."__table__command PROGMEM={\n";
	$table_command .= "\ttable, ".$c_prefix."__table__name, (void *)&".$c_prefix."table\n};\n";
	
	$main_commands .= "\t&".$c_prefix."__table__command,\n";
	$main_commands_num++;
	
	$output .= "$command_table \n";
	$output .= "$table_command_name \n";
	$output .= "$table_command \n";
}

my $main_table = "const command_table main_table PROGMEM={\n";
$main_table .= "\t".$main_commands_num." ,{".$main_commands."\t}\n};";

$output .= "$main_table \n";

$output = "#include <avr/pgmspace.h>\n".$output;
$output = "#include \"console.h\"\n".$output;

print $output;
