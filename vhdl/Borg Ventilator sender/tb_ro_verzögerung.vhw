--------------------------------------------------------------------------------
-- Copyright (c) 1995-2007 Xilinx, Inc.
-- All Right Reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 9.2.04i
--  \   \         Application : ISE
--  /   /         Filename : tb_ro_verzögerung.vhw
-- /___/   /\     Timestamp : Sat Apr 18 23:27:28 2009
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: 
--Design Name: tb_ro_verzögerung
--Device: Xilinx
--

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
USE IEEE.STD_LOGIC_TEXTIO.ALL;
USE STD.TEXTIO.ALL;

ENTITY tb_ro_verzögerung IS
END tb_ro_verzögerung;

ARCHITECTURE testbench_arch OF tb_ro_verzögerung IS
    COMPONENT roh_verzoegerung
        PORT (
            clk200 : In std_logic;
            a : In std_logic;
            q : Out std_logic
        );
    END COMPONENT;

    SIGNAL clk200 : std_logic := '0';
    SIGNAL a : std_logic := '0';
    SIGNAL q : std_logic := '0';

    constant PERIOD : time := 10 ns;
    constant DUTY_CYCLE : real := 0.5;
    constant OFFSET : time := 5 ns;

    BEGIN
        UUT : roh_verzoegerung
        PORT MAP (
            clk200 => clk200,
            a => a,
            q => q
        );

        PROCESS    -- clock process for clk200
        BEGIN
            WAIT for OFFSET;
            CLOCK_LOOP : LOOP
                clk200 <= '0';
                WAIT FOR (PERIOD - (PERIOD * DUTY_CYCLE));
                clk200 <= '1';
                WAIT FOR (PERIOD * DUTY_CYCLE);
            END LOOP CLOCK_LOOP;
        END PROCESS;

        PROCESS
            BEGIN
                -- -------------  Current Time:  129ns
                WAIT FOR 129 ns;
                a <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  179ns
                WAIT FOR 50 ns;
                a <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  279ns
                WAIT FOR 100 ns;
                a <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  399ns
                WAIT FOR 120 ns;
                a <= '0';
                -- -------------------------------------
                WAIT FOR 611 ns;

            END PROCESS;

    END testbench_arch;

