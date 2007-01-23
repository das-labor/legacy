library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

library UNISIM;
use UNISIM.VComponents.all;

entity DataPath is
	port (
		clk_2x   : in    std_logic;
		clk_2x90 : in    std_logic;
		clk_fb90 : in    std_logic;
		reset    : in    std_logic;
		cal_xing : in    std_logic_vector(5 downto 0);
		--
		rtrig    : in    std_logic;
		wtrig    : in    std_logic;
		rdone    : out   std_logic;
		wdone    : out   std_logic;
		rdata    : out   std_logic_vector(63 downto 0);
		wdata    : in    std_logic_vector(63 downto 0);
		dmask    : in    std_logic_vector( 7 downto 0);
		-- DDR Connection
		ddr_dqs  : inout std_logic_vector( 1 downto 0);
		ddr_dq   : inout std_logic_vector(15 downto 0);
		ddr_dm   : out   std_logic_vector( 1 downto 0);
		-- Debug 
		led      : out   std_logic_vector( 7 downto 0);
		sw       : in    std_logic_vector(3 downto 0) );

end DataPath;

architecture RTL of DataPath is

component FDDRRSE
   -- synthesis translate_off
   generic (INIT : bit := '1');
   -- synthesis translate_on
   port (Q  : out STD_ULOGIC;
         C0 : in  STD_ULOGIC;
         C1 : in  STD_ULOGIC;
         CE : in  STD_ULOGIC;
         D0 : in  STD_ULOGIC;
         D1 : in  STD_ULOGIC;
         R  : in  STD_ULOGIC;
         S  : in  STD_ULOGIC);
end component; 

signal clk_2x180 : std_logic;
signal clk_2x270 : std_logic;
signal clk_fb270 : std_logic;

signal rstate    : std_logic_vector(5 downto 0);
signal wstate    : std_logic_vector(3 downto 0);

signal rtrig90   : std_logic;
signal rtrig270  : std_logic;
signal rdone90   : std_logic;
signal rdone90_2 : std_logic;
signal rdone270  : std_logic;
signal rdone270_2: std_logic;

signal dqs1    : std_logic;
signal tri     : std_logic;
signal dqs_out : std_logic_vector( 1 downto 0);
signal dq_out  : std_logic_vector(15 downto 0);
signal dm_out  : std_logic_vector( 1 downto 0);

signal wdata90   : std_logic_vector(15 downto 0);
signal wdata270  : std_logic_vector(15 downto 0);

signal dmask90   : std_logic_vector( 1 downto 0);
signal dmask270  : std_logic_vector( 1 downto 0);

signal rdata90   : std_logic_vector(63 downto 0);
signal rdata270  : std_logic_vector(63 downto 0);

signal dq90   : std_logic_vector(15 downto 0);
signal dq270  : std_logic_vector(15 downto 0);

begin

------------------------------------------------------
-- Generate Clocks via local clock inversion
clk_2x180 <= not clk_2x;
clk_2x270 <= not clk_2x90;
clk_fb270 <= not clk_fb90;

------------------------------------------------------
-- Write Path 
wproc: process(reset, clk_2x)
begin
	if reset='1' then
		wstate <= (others => '0');
	elsif clk_2x'event and clk_2x='1' then
		wstate(3 downto 1) <= wstate(2 downto 0);
		wstate(0) <= wtrig;
	end if;
end process;
wdone <= wstate(2) or wstate(3);


wdataproc: process(clk_2x270)
begin
	if clk_2x270'event and clk_2x270='1' then
		if wtrig='1' then
			wdata90  <= wdata(15 downto  0);
			wdata270 <= wdata(31 downto 16);
			dmask90  <= not dmask( 1 downto 0);
			dmask270 <= not dmask( 3 downto 2);			
		else
			wdata90  <= wdata(47 downto 32);
			wdata270 <= wdata(63 downto 48);
			dmask90  <= not dmask( 5 downto 4);
			dmask270 <= not dmask( 7 downto 6);			
		end if;
	end if;			
end process;

-- tristate off when writing
tri  <= not (wstate(0) or wstate(1) or wstate(2));
dqs1 <= wstate(0) or wstate(1);

-- drive tri-state buffers
ddr_dqs <= (others => 'Z') when tri='1' else
           dqs_out;

ddr_dq  <= (others => 'Z') when tri='1' else
			  dq_out;

ddr_dm  <= (others => 'Z') when tri='1' else
			  dm_out;

-- generate dqs_out
dqs: for i in 1 downto 0 generate
begin
	dqsff: FDDRRSE
		port map (
			Q   => dqs_out(i),
			C0  => clk_2x,
			C1  => clk_2x180,
			D0  => '0',
			D1  => dqs1,
			CE  => '1',
			R   => tri,
			S   => '0' );
end generate;

-- generate dq_out
dq: for i in 15 downto 0 generate
begin
	dqff: FDDRRSE
		port map (
			Q   => dq_out(i),
			C0  => clk_2x90,
			C1  => clk_2x270,
			D0  => wdata90(i),
			D1  => wdata270(i),
			CE  => '1',
			R   => reset,
			S   => '0' );
end generate;

-- generate dm_out
dm: for i in 1 downto 0 generate
begin
	dqsff: FDDRRSE
		port map (
			Q   => dm_out(i),
			C0  => clk_2x,
			C1  => clk_2x180,
			D0  => dmask90(i),
			D1  => dmask270(i),
			CE  => '1',
			R   => reset,
			S   => '0' );
end generate;

------------------------------------------------------
-- Read Path
rproc: process(reset, clk_2x) is
begin
	if clk_2x'event and clk_2x='1' then
		if cal_xing(0)='1' or cal_xing(2)='1' then
			rdone <= rdone90 or rdone90_2;
			if rdone90='1' then 
				rdata <= rdata90;			
			end if;
		end if;
		if cal_xing(1)='1' or cal_xing(3)='1' then
			rdone <= rdone270 or rdone270_2;
			if rdone270='1' then 			
				rdata <= rdata270;			
			end if;				
		end if;
	end if;
end process;


rdata270proc: process(reset, clk_fb270) is
variable match : std_logic;
begin
	if reset='1' then
		rstate <= (others => '0');		
	elsif clk_fb270'event and clk_fb270='1' then
		rstate(5 downto 1) <= rstate(4 downto 0);
		rstate(0) <= '0';
		
		-- Handle Clock boundary		
		rtrig270 <= rtrig;
		rdone270_2 <= rdone270; 
		rdone270   <= rstate(3);	
		
		if cal_xing(0)='1' then
			rstate(1) <= rtrig;
		end if;
		if cal_xing(2)='1' then
			rstate(0) <= rtrig;
		end if;
		if cal_xing(1)='1' then
			rstate(1) <= rtrig90;
		end if;
		if cal_xing(3)='1' then
			rstate(0) <= rtrig90;
		end if;
		

		-- Data In FF	
		if rstate(2)='1' then
			rdata270(15 downto  0) <= rdata90(15 downto 0);		
			rdata270(31 downto 16) <= ddr_dq;
		end if;
		if rstate(3)='1' then
			rdata270(47 downto 32) <= rdata90(47 downto 32);		
			rdata270(63 downto 48) <= ddr_dq;
		end if;
		
	
		--DEBUG
		dq270 <= ddr_dq;
		match := '0';
		if sw(3)='1' and dq270="0000000000000011" then
			match := '1';
		end if;
		if sw(2)='1' and dq270="0000000000000111" then
			match := '1';
		end if;
		if sw(1)='1' and dq270="0000000000001111" then
			match := '1';
		end if;
		if sw(0)='1' and dq270="0000000000011111" then
			match := '1';
		end if;

--		if rstate(2)='1' then
--			led(6) <= match;
--		end if;
--		if rstate(3)='1' then
--			led(4) <= match;
--		end if;
--		if rstate(4)='1' then
--			led(2) <= match;
--		end if;
--		if rstate(5)='1' then
--			led(0) <= match;
--		end if;
	end if;
end process;

rdata90proc: process(reset, clk_fb90) is
variable match : std_logic;
begin
	if clk_fb90'event and clk_fb90='1' then

		-- Handle Clock boundary
		rtrig90 <= rtrig;
		rdone90_2 <= rdone90;
		rdone90   <= rstate(4);		
		
		if rstate(2)='1' then
			rdata90(15 downto  0) <= ddr_dq;
		end if;
		if rstate(3)='1' then
			rdata90(31 downto 16) <= rdata270(31 downto 16);
			rdata90(47 downto 32) <= ddr_dq;			
		end if;
		if rstate(4)='1' then
			rdata90(63 downto 48) <= rdata270(63 downto 48);
		end if;

		
		--DEBUG
		dq90 <= ddr_dq;
		match := '0';
		if sw(3)='1' and dq90="0000000000000011" then
			match := '1';
		end if;
		if sw(2)='1' and dq90="0000000000000111" then
			match := '1';
		end if;
		if sw(1)='1' and dq90="0000000000001111" then
			match := '1';
		end if;
		if sw(0)='1' and dq90="0000000000011111" then
			match := '1';
		end if;

--		if rstate(2)='1' then
--			led(7) <= match;
--		end if;
--		if rstate(3)='1' then
--			led(5) <= match;
--		end if;
--		if rstate(4)='1' then
--			led(3) <= match;
--		end if;
--		if rstate(5)='1' then
--			led(1) <= match;
--		end if;
	end if;
end process;


end RTL;

