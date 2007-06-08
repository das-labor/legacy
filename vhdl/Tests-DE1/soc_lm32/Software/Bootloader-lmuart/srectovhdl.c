#include <stdio.h>
#include <string.h>

#define BUF_SIZE 90

static unsigned char hex2Nibble(char hex) {
  unsigned char byteOut  = 0, value;
  switch (hex & 0x70) {
    case '0':
       value = hex & 0x0f;
       if (value < 10) {
          byteOut = value;
       }
       break;
    case 'A' & 0xf0:
    case 'a' & 0xf0:
       value = hex & 0x0f;
       if (value < 7 && value) {
          byteOut = 9 + value;
       }
       break;
  }
  return byteOut;
}

void header() {
	printf(
	"--DO NOT EDIT - This is a generated - edit Software/Boootloader/srectovhdl.c \n" 
	"-- Wishbone Block Ram -------------------------------------------------------\n" 
	"library ieee;\n" 
	"use ieee.std_logic_1164.ALL;\n" 
	"use ieee.numeric_std.all;\n" 
	"\n" 
	"entity wb_bram is\n" 
	"   port (\n" 
	"      clk      : in  std_logic;\n" 
	"      reset    : in  std_logic;\n" 
	"      -- Wishbone bus\n" 
	"      wb_adr_i : in  std_logic_vector(31 downto 0);\n" 
	"      wb_dat_i : in  std_logic_vector(31 downto 0);\n" 
	"      wb_dat_o : out std_logic_vector(31 downto 0);\n" 
	"      wb_sel_i : in  std_logic_vector( 3 downto 0);\n" 
	"      wb_cyc_i : in  std_logic;\n" 
	"      wb_stb_i : in  std_logic;\n" 
	"      wb_ack_o : out std_logic;\n" 
	"      wb_we_i  : in  std_logic );\n" 
	"end wb_bram;\n" 
	"\n" 
	"-----------------------------------------------------------------------------\n" 
	"-- Implementation -----------------------------------------------------------\n" 
	"architecture rtl of wb_bram is\n" 
	"\n" 
	"constant mem_size : natural := (8*512) - 1;\n" 
	"type mem_type is array(0 to mem_size) of std_logic_vector(31 downto 0);\n" 
	"\n" 
	"-----------------------------------------------------------------------------\n" 
	"-- Pre-Initialized-Data -----------------------------------------------------\n" 
	"signal mem : mem_type := (");
} 

void footer() {
printf(
	"   others => x\"00000000\" \n"
	");\n"
	"\n"        
	"signal ack : std_logic;\n"
	"\n"
	"begin\n"
	"\n"
	"wb_ack_o <= wb_stb_i and ack;\n"
	"\n"
	"memproc: process (clk) is\n"
	"variable a : integer;\n"
	"begin\n"
	"        if clk'event and clk='1' then\n"
	"        if wb_stb_i='1' and wb_cyc_i='1' then\n"
	"                a := to_integer(unsigned(wb_adr_i(12 downto 2)));\n"
	"        \n"
	"                if wb_we_i='1' then \n"
	"                        mem(a) <= wb_dat_i;\n"
	"                end if;\n"
	"\n"
	"                wb_dat_o <= mem(a);\n"
	"                ack <= '1' and not ack after 2 ns;\n"
	"        else\n"
	"                ack <= '0';\n"
	"        end if;\n"
	"        end if;\n"
	"end process;\n"
	"\n"
	"end rtl;\n");
}

int main(int argc, char** argv) {
	FILE *srec;
	char buf[BUF_SIZE];
	char help[9];
	int count, i, j = 0;
	help[8] = 0;
	
	if (argc < 1)
		return 1;
	if ((srec = fopen(argv[1], "r")) == 0)
		return 1;
		
	header();	
	
	while  (fgets(buf, BUF_SIZE, srec)) {
		if (buf[1] == '1') {
			count  = hex2Nibble(buf[2]) << 4; 
			count += hex2Nibble(buf[3]);
			for (i = 0; i < count-3; i+=4) {
				if (!j--) {
					printf("\n\t");
					j = 3;
				}
				strncpy(help, &buf[8+i*2], 8);
				printf("x\"%s\", ", help);
			}
		}
	}
	
	footer();
	
	fclose(srec);
	return 0;
}

