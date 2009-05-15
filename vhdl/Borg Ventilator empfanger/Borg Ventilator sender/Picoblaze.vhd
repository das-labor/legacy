------------------------------------------------------------------------------------
------------------------------------------------------------------------------------
-- Hackerspace:		Das Labor    www.das-labor.org                               --
-- Hacker: 				Sauron                                                       --
--                                                                                --
-- Datum:    			03.04.2009 									                         --
-- �nderung:         10.04.2009                                                   --
-- Projekt: 			Der Borg Ventilator													    --
-- Modul Name:     	picoblaze.vhd                                                --
-- Beschreibung: 		Ich enthalte eine picoblaze cpu, verbunden mit einem rom,    --
--                   ein paar freie ports und interface zu 7seg display, leds,    --
--							Tastern und schaltern            									 --
--																											 --
-- Dependencies: 		KCPSM3, pb_rom       													 --
--																											 --
-- Version:          V9.4.1																		 --
-- 																										 --
-- Additional Comments: Wiederstand ist Zwecklos											 --
--																											 --
------------------------------------------------------------------------------------
------------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;


entity Picoblaze is

 Port (
	 clk           : in  STD_LOGIC;
	 wr_str        : out STD_LOGIC;
	 p_id          : out STD_LOGIC_VECTOR (7 downto 0);
	 out_p         : out STD_LOGIC_VECTOR (7 downto 0);
	 irq           : in  STD_LOGIC;
	 irq_ack       : out STD_LOGIC;
    i0            : in  STD_LOGIC_VECTOR (7 downto 0);
    i1            : in  STD_LOGIC_VECTOR (7 downto 0);
    i2            : in  STD_LOGIC_VECTOR (7 downto 0);
    i3            : in  STD_LOGIC_VECTOR (7 downto 0);
    q0            : out STD_LOGIC_VECTOR (7 downto 0);
    q1            : out STD_LOGIC_VECTOR (7 downto 0);
    q2            : out STD_LOGIC_VECTOR (7 downto 0);
	 schalter      : in  STD_LOGIC_VECTOR (7 downto 0);
	 taster        : in  STD_LOGIC_VECTOR (3 downto 0);
	 led           : out STD_LOGIC_VECTOR (7 downto 0);
    q3,q4,q5,q6,q7,q8,q9: out STD_LOGIC_VECTOR (7 downto 0));
	
end Picoblaze;


architecture Behavioral of Picoblaze is

signal address       : std_logic_vector(9 downto 0);
signal instruction   : std_logic_vector(17 downto 0);
signal port_id       : std_logic_vector(7 downto 0);
signal out_port      : std_logic_vector(7 downto 0);
signal in_port       : std_logic_vector(7 downto 0);
signal in_port_i     : std_logic_vector(7 downto 0);
signal en10,en11,en12: std_logic;
signal en13,en40     : std_logic;
signal en14,en15,en16: std_logic;
signal en17,en18,en19: std_logic;
signal read_strobe   : std_logic;
signal write_strobe  : std_logic;
signal reset         : std_logic;



--------------------KOMPONENTEN-BEKANNTMACHEN---------------------------------------
component kcpsm3
	port (
	             address : out std_logic_vector(9 downto 0);
            instruction : in  std_logic_vector(17 downto 0);
                port_id : out std_logic_vector(7 downto 0);
           write_strobe : out std_logic;
               out_port : out std_logic_vector(7 downto 0);
            read_strobe : out std_logic;
                in_port : in  std_logic_vector(7 downto 0);
              interrupt : in  std_logic;
          interrupt_ack : out std_logic;
                  reset : in  std_logic;
                    clk : in  std_logic);
end component;

component pb_rom
	port (       address : in  std_logic_vector(9 downto 0);
            instruction : out std_logic_vector(17 downto 0);
			 	 proc_reset : out std_logic;
                    clk : in  std_logic);
end component;
------------------------------------------------------------------------------------
begin

u0 :kcpsm3
port map (      address => address,
            instruction => instruction,
                port_id => port_id, 
           write_strobe => write_strobe,
               out_port => out_port,
            read_strobe => read_strobe,
                in_port => in_port,
              interrupt => irq,
          interrupt_ack => irq_ack,
                  reset => reset,
                    clk => clk);


u1 :pb_rom
port map (      address => address,
            instruction => instruction,
				 proc_reset => reset,
                    clk => clk);
-----------------------------------------------------------------------------------
                 --SIGNALE DURCHREICHEN 
                  p_id <= port_id;
                 out_p <= out_port;						  
                wr_str <= write_strobe; 
					 

------------Ausgangs-PORTS----------------------------------------------------------
--                                               enable f�r port xx

--  process (clk) begin
--    if RISING_EDGE(clk) then
--      if port_id = 40 then
--	     en40 <= '1';
--      else                        Dies ist die Ausgeschriebene version, von dem,
--        en40 <= '0';
--      end if;                            was Komprimiert hier drunter steht
--    end if;
--  end process;  

process (clk) begin if RISING_EDGE(clk) then if port_id = 10 then en10 <= '1';
else en10 <= '0';end if;end if;end process;  
process (clk) begin if RISING_EDGE(clk) then if port_id = 11 then en11 <= '1';
else en11 <= '0';end if;end if;end process;  
process (clk) begin if RISING_EDGE(clk) then if port_id = 12 then en12 <= '1';
else en12 <= '0';end if;end if;end process;  
process (clk) begin if RISING_EDGE(clk) then if port_id = 13 then en13 <= '1';
else en13 <= '0';end if;end if;end process;  
process (clk) begin if RISING_EDGE(clk) then if port_id = 14 then en14 <= '1';
else en14 <= '0';end if;end if;end process;  
process (clk) begin if RISING_EDGE(clk) then if port_id = 15 then en15 <= '1';
else en15 <= '0';end if;end if;end process;  
process (clk) begin if RISING_EDGE(clk) then if port_id = 16 then en16 <= '1';
else en16 <= '0';end if;end if;end process;  
process (clk) begin if RISING_EDGE(clk) then if port_id = 17 then en17 <= '1';
else en17 <= '0';end if;end if;end process;  
process (clk) begin if RISING_EDGE(clk) then if port_id = 18 then en18 <= '1';
else en18 <= '0';end if;end if;end process;  
process (clk) begin if RISING_EDGE(clk) then if port_id = 19 then en19 <= '1';
else en19 <= '0';end if;end if;end process;  


process (clk) begin if RISING_EDGE(clk) then if port_id = 40 then en40 <= '1';
else en40 <= '0';end if;end if;end process;  


--                        Port xx ausgeben und in FF speichern

--  process (clk) begin
--    if RISING_EDGE(clk) then                      Dies ist die Ausgeschriebene 
--      if en40 = '0' and write_strobe = '0' then   version, von dem, was 
--	     led(0) <= out_port(0);                      Komprimiert hier drunter steht
--      end if;
--    end if;
--  end process;  

process (clk) begin if RISING_EDGE(clk) then if en10 = '1' and write_strobe = '1' 
then q0 <= out_port;end if;end if;end process;  
process (clk) begin if RISING_EDGE(clk) then if en11 = '1' and write_strobe = '1' 
then q1 <= out_port;end if;end if;end process;  
process (clk) begin if RISING_EDGE(clk) then if en12 = '1' and write_strobe = '1' 
then q2 <= out_port;end if;end if;end process;  
process (clk) begin if RISING_EDGE(clk) then if en13 = '1' and write_strobe = '1' 
then q3 <= out_port;end if;end if;end process;  
process (clk) begin if RISING_EDGE(clk) then if en14 = '1' and write_strobe = '1' 
then q4 <= out_port;end if;end if;end process;  
process (clk) begin if RISING_EDGE(clk) then if en15 = '1' and write_strobe = '1' 
then q5 <= out_port;end if;end if;end process;  
process (clk) begin if RISING_EDGE(clk) then if en16 = '1' and write_strobe = '1' 
then q6 <= out_port;end if;end if;end process;  
process (clk) begin if RISING_EDGE(clk) then if en17 = '1' and write_strobe = '1' 
then q7 <= out_port;end if;end if;end process;  
process (clk) begin if RISING_EDGE(clk) then if en18 = '1' and write_strobe = '1' 
then q8 <= out_port;end if;end if;end process;  
process (clk) begin if RISING_EDGE(clk) then if en19 = '1' and write_strobe = '1' 
then q9 <= out_port;end if;end if;end process;  

process (clk) begin if RISING_EDGE(clk) then if en40 = '1' and write_strobe = '1' 
then led<= out_port;end if;end if;end process;  


----------IMPULS-PORTS--------------(not-implementet-jet)---------------------------
--process (clk) begin if rising_edge (clk) then 
--  if port_id = 10 and write_strobe = '1' then  
--    imp_res <= out_port; end if;end if; end process;
--
--process (clk)
--begin
--if rising_edge (clk) then
--  if imp0 = '1' then
--    imp0i <= '1';
--  elsif (imp0_syn = '0' and imp0_last = '1') then --fallende Flanke auf Imp
--    imp0i <= '0';
--  end if;
--end if;
--end process;
--
--process(CLK)
-- begin
--   if rising_edge(CLK) then
--     imp0_syn  <= imp_res(0);
--     imp0_last <= imp0_syn;
--    end if;
--end process;
-------------EINGANGS_PORTS--------------------------------------------------------


process (port_id,i0,i1,i2,i3,schalter,taster)
begin
   case port_id is
      when "00000000" => in_port_i <= i0;                     -- port  0
      when "00000001" => in_port_i <= i1;                     -- port  1
      when "00000010" => in_port_i <= i2;                     -- port  2
      when "00000011" => in_port_i <= i3;                     -- port  3
		when "00110010" => in_port_i <= schalter;               -- port 50
		when "00110011" => in_port_i <= "0000" & 
		         taster(3) & taster(2) & taster(1) & taster(0); -- port 51
      when others => in_port_i <= "00000000";
   end case;
end process;

process (clk) begin                                  --in_port pipeline 
  if rising_edge (clk) and read_strobe = '0' then
    in_port <= in_port_i;
  end if;
end process;
 



end Behavioral;