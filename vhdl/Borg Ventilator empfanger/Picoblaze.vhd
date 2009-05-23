------------------------------------------------------------------------------------
------------------------------------------------------------------------------------
-- Hackerspace:		Das Labor    www.das-labor.org                               --
-- Hacker: 				Sauron                                                       --
--                                                                                --
-- Datum:    			03.04.2009 									                         --
-- Änderung:         10.04.2009                                                   --
-- Projekt: 			Der Borg Ventilator													    --
-- Modul Name:     	picoblaze_e.vhd                                              --
-- Beschreibung: 		Ich enthalte eine picoblaze cpu, verbunden mit einem rom,    --
--                   ein paar freie ports und interface                           --
--							                                 									 --
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


entity Picoblaze_e is

 Port (
	 clk           : in  STD_LOGIC;
	 in0           : in  STD_LOGIC;
	 in1           : in  STD_LOGIC;
	 in2           : in  STD_LOGIC;
	 in3           : in  STD_LOGIC;
	 in4           : in  STD_LOGIC;
	 in5           : in  STD_LOGIC;
	 in6           : in  STD_LOGIC;
	 in7           : in  STD_LOGIC;
	 in8           : in  STD_LOGIC;
	 in9           : in  STD_LOGIC;
	 in10          : in  STD_LOGIC;
	 in11          : in  STD_LOGIC;
	 in12          : in  STD_LOGIC;
	 in13          : in  STD_LOGIC;
	 in14          : in  STD_LOGIC;
	 in15          : in  STD_LOGIC;
	 dcm_sts       : in  STD_LOGIC_VECTOR (7 downto 0);
	 led           : out STD_LOGIC;
	 com_res       : out STD_LOGIC;
	 wr_str        : out STD_LOGIC;
	 p_id,out_p    : out STD_LOGIC_VECTOR (7 downto 0);

--	 diag_q0,diag_q1,diag_imp0,diag_imp1         : out STD_LOGIC_VECTOR (7 downto 0);
	 
	 irq           : in  STD_LOGIC;
	 irq_ack       : out STD_LOGIC;
    q2,q3         : out STD_LOGIC_VECTOR (7 downto 0);	-- Verbindung zum BRAM
    en_ram        : out std_logic);
	
end Picoblaze_e;


architecture Behavioral of Picoblaze_e is

signal address       : std_logic_vector(9 downto 0);
signal instruction   : std_logic_vector(17 downto 0);
signal port_id       : std_logic_vector(7 downto 0);
signal out_port      : std_logic_vector(7 downto 0);
signal in_port       : std_logic_vector(7 downto 0);
signal in_port_i     : std_logic_vector(7 downto 0);
signal q0_old,q0     : std_logic_vector(7 downto 0):=x"00";
signal q1_old,q1     : std_logic_vector(7 downto 0):=x"00";
signal q4,q5         : std_logic_vector(7 downto 0);
signal i0,i1,i5      : std_logic_vector(7 downto 0):=x"ff";
signal imp0,imp1     : std_logic_vector(7 downto 0):=x"00";
signal en0,en1       : std_logic;
signal en2,en3,en5   : std_logic;
signal en4,en128     : std_logic;
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
-- Die folgende Zeile muss auskommentiert sein, wenn das ROM keinen JTAG Anschluss hat
--			 	 proc_reset : out std_logic;      
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
-- Die folgende Zeile muss auskommentiert sein, wenn das ROM keinen JTAG Anschluss hat
--				 proc_reset => reset,
                    clk => clk);
						  
						  reset <= '0';
-----------------------------------------------------------------------------------
                 --SIGNALE DURCHREICHEN 
                  p_id <= port_id;
                 out_p <= out_port;						  
                wr_str <= write_strobe; 
					 

------------Ausgangs-PORTS----------------------------------------------------------
--                                               enable für port xx

--  process (clk) begin
--    if RISING_EDGE(clk) then
--      if port_id = 40 then
--	     en40 <= '1';
--      else                        Dies ist die Ausgeschriebene version, von dem,
--        en40 <= '0';
--      end if;                            was Komprimiert hier drunter steht
--    end if;
--  end process;  

process (clk) begin if RISING_EDGE(clk) then if port_id = x"02"  then en2  <= '1';
else en2 <= '0';end if;end if;end process;  
process (clk) begin if RISING_EDGE(clk) then if port_id = x"03"  then en3  <= '1';
else en3 <= '0';end if;end if;end process;  
process (clk) begin if RISING_EDGE(clk) then if port_id = x"04"  then en4  <= '1';
else en4 <= '0';end if;end if;end process;  
process (clk) begin if RISING_EDGE(clk) then if port_id = x"05"  then en5  <= '1';
else en5 <= '0';end if;end if;end process;  
process (clk) begin if RISING_EDGE(clk) then if port_id = x"80" then en128 <= '1';
else en128 <= '0';end if;end if;end process;  


--                        Port xx ausgeben und in FF speichern

--  process (clk) begin
--    if RISING_EDGE(clk) then                      Dies ist die Ausgeschriebene 
--      if en40 = '0' and write_strobe = '0' then   version, von dem, was 
--	     led(0) <= out_port(0);                      Komprimiert hier drunter steht
--      end if;
--    end if;
--  end process;  


process (clk) begin if RISING_EDGE(clk) then if en2 = '1' and write_strobe = '1' 
then q2 <= out_port;end if;end if;end process;  
process (clk) begin if RISING_EDGE(clk) then if en3 = '1' and write_strobe = '1' 
then q3 <= out_port;end if;end if;end process;  
process (clk) begin if RISING_EDGE(clk) then if en4 = '1' and write_strobe = '1' 
then q4 <= out_port;end if;end if;end process;  
process (clk) begin if RISING_EDGE(clk) then if en5 = '1' and write_strobe = '1' 
then q5 <= out_port;end if;end if;end process;  

en_ram <= en128;
led    <= not q4(0);
com_res<= q5(0);

----------IMPULS-PORTS---------------------------------------
--ports zusammenfassen zu Bytes
imp0<=in7  & in6  & in5  & in4  & in3  & in2  & in1 & in0;
imp1<=in15 & in14 & in13 & in12 & in11 & in10 & in9 & in8;


-- Enable für port 0 und 1
process (clk) begin if RISING_EDGE(clk) then if port_id = 0 then en0  <= '1';
else en0  <= '0';end if;end if;end process;  
process (clk) begin if RISING_EDGE(clk) then if port_id = 1 then en1  <= '1';
else en1  <= '0';end if;end if;end process;  


-- Ausgangsport in q0 speichern und vorhergehenden wert zwischenspeichern für Flankenerkennung
process (clk) begin 
  if RISING_EDGE(clk) then 
    if en0 = '1' and write_strobe = '1' then 
      q0_old <= q0; 
		q0     <= out_port;
	 end if;
  end if;
end process; 

-- und nochmal für q1
process (clk) begin 
  if RISING_EDGE(clk) then 
    if en1 = '1' and write_strobe = '1' then 
      q1_old <= q1; q1 <= out_port;
    end if;
  end if;
end process;  

process (clk) begin
  for x in 0 to 7 loop	  -- die einzelnen Bits des Bytes durchgehen
    if rising_edge (clk) then
	   if q0_old(x) = '0' and q0(x) = '1' 	-- wenn steigende Flanke auf q0
	   then i0(x) <= '0'; 			-- dann lösche i0
      elsif imp0(x) = '1' then      -- wenn eingangang auf 1
      i0(x) <= imp0(x);	 			-- dann Setze i0 
  	   end if;
    end if;
  end loop;
end process;  
	 
process (clk) begin
  for x in 0 to 7 loop			
    if rising_edge (clk) then
   	if q1_old(x) = '0' and q1(x) = '1' 
	   then i1(x) <= '0'; 
	   elsif imp1(x) = '1' then
      i1(x) <= imp1(x);	 
	   end if;
    end if;
  end loop;
end process;  


-------------EINGANGS_PORTS--------------------------------------------------------


process (port_id,i0,i1,i5)
begin
   case port_id is
      when x"00" => in_port_i <= i0;                   -- port  0
      when x"01" => in_port_i <= i1;                   -- port  1
		
		when x"05" => in_port_i <= dcm_sts;              -- port  5 dcm_status
		
		
      when others => in_port_i <= (others =>'-');
   end case;
end process;

process (clk) begin                                  --in_port pipeline 
  if rising_edge (clk) and read_strobe = '0' then
    in_port <= in_port_i;
  end if;
end process;
 
 
--diag_q0 <= q0;
--diag_q1 <= q1;
--diag_imp0 <= imp0;
--diag_imp1 <= imp1;


end Behavioral;