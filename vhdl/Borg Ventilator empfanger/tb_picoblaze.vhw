--------------------------------------------------------------------------------
-- Copyright (c) 1995-2007 Xilinx, Inc.
-- All Right Reserved.
--------------------------------------------------------------------------------
--   ____  ____ 
--  /   /\/   / 
-- /___/  \  /    Vendor: Xilinx 
-- \   \   \/     Version : 9.2.04i
--  \   \         Application : ISE
--  /   /         Filename : tb_picoblaze.vhw
-- /___/   /\     Timestamp : Wed May 13 04:26:09 2009
-- \   \  /  \ 
--  \___\/\___\ 
--
--Command: 
--Design Name: tb_picoblaze
--Device: Xilinx
--

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_ARITH.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
USE IEEE.STD_LOGIC_TEXTIO.ALL;
USE STD.TEXTIO.ALL;

ENTITY tb_picoblaze IS
END tb_picoblaze;

ARCHITECTURE testbench_arch OF tb_picoblaze IS
    FILE RESULTS: TEXT OPEN WRITE_MODE IS "results.txt";

    COMPONENT Picoblaze_e
        PORT (
            clk : In std_logic;
            in0 : In std_logic;
            in1 : In std_logic;
            in2 : In std_logic;
            in3 : In std_logic;
            in4 : In std_logic;
            in5 : In std_logic;
            in6 : In std_logic;
            in7 : In std_logic;
            in8 : In std_logic;
            in9 : In std_logic;
            in10 : In std_logic;
            in11 : In std_logic;
            in12 : In std_logic;
            in13 : In std_logic;
            in14 : In std_logic;
            in15 : In std_logic;
            wr_str : Out std_logic;
            p_id : Out std_logic_vector (7 DownTo 0);
            out_p : Out std_logic_vector (7 DownTo 0);
            diag_q0 : Out std_logic_vector (7 DownTo 0);
            diag_q1 : Out std_logic_vector (7 DownTo 0);
            diag_imp0 : Out std_logic_vector (7 DownTo 0);
            diag_imp1 : Out std_logic_vector (7 DownTo 0);
            irq : In std_logic;
            irq_ack : Out std_logic;
            q2 : Out std_logic_vector (7 DownTo 0);
            q3 : Out std_logic_vector (7 DownTo 0);
            en_ram : Out std_logic
        );
    END COMPONENT;

    SIGNAL clk : std_logic := '0';
    SIGNAL in0 : std_logic := '0';
    SIGNAL in1 : std_logic := '0';
    SIGNAL in2 : std_logic := '0';
    SIGNAL in3 : std_logic := '0';
    SIGNAL in4 : std_logic := '0';
    SIGNAL in5 : std_logic := '0';
    SIGNAL in6 : std_logic := '0';
    SIGNAL in7 : std_logic := '0';
    SIGNAL in8 : std_logic := '0';
    SIGNAL in9 : std_logic := '0';
    SIGNAL in10 : std_logic := '0';
    SIGNAL in11 : std_logic := '0';
    SIGNAL in12 : std_logic := '0';
    SIGNAL in13 : std_logic := '0';
    SIGNAL in14 : std_logic := '0';
    SIGNAL in15 : std_logic := '0';
    SIGNAL wr_str : std_logic := '0';
    SIGNAL p_id : std_logic_vector (7 DownTo 0) := "00000000";
    SIGNAL out_p : std_logic_vector (7 DownTo 0) := "00000000";
    SIGNAL diag_q0 : std_logic_vector (7 DownTo 0) := "00000000";
    SIGNAL diag_q1 : std_logic_vector (7 DownTo 0) := "00000000";
    SIGNAL diag_imp0 : std_logic_vector (7 DownTo 0) := "00000000";
    SIGNAL diag_imp1 : std_logic_vector (7 DownTo 0) := "00000000";
    SIGNAL irq : std_logic := '0';
    SIGNAL irq_ack : std_logic := '0';
    SIGNAL q2 : std_logic_vector (7 DownTo 0) := "00000000";
    SIGNAL q3 : std_logic_vector (7 DownTo 0) := "00000000";
    SIGNAL en_ram : std_logic := '0';

    constant PERIOD : time := 20 ns;
    constant DUTY_CYCLE : real := 0.5;
    constant OFFSET : time := 10 ns;

    BEGIN
        UUT : Picoblaze_e
        PORT MAP (
            clk => clk,
            in0 => in0,
            in1 => in1,
            in2 => in2,
            in3 => in3,
            in4 => in4,
            in5 => in5,
            in6 => in6,
            in7 => in7,
            in8 => in8,
            in9 => in9,
            in10 => in10,
            in11 => in11,
            in12 => in12,
            in13 => in13,
            in14 => in14,
            in15 => in15,
            wr_str => wr_str,
            p_id => p_id,
            out_p => out_p,
            diag_q0 => diag_q0,
            diag_q1 => diag_q1,
            diag_imp0 => diag_imp0,
            diag_imp1 => diag_imp1,
            irq => irq,
            irq_ack => irq_ack,
            q2 => q2,
            q3 => q3,
            en_ram => en_ram
        );

        PROCESS    -- clock process for clk
        BEGIN
            WAIT for OFFSET;
            CLOCK_LOOP : LOOP
                clk <= '0';
                WAIT FOR (PERIOD - (PERIOD * DUTY_CYCLE));
                clk <= '1';
                WAIT FOR (PERIOD * DUTY_CYCLE);
            END LOOP CLOCK_LOOP;
        END PROCESS;

        PROCESS
            BEGIN
                -- -------------  Current Time:  639ns
                WAIT FOR 639 ns;
                in0 <= '1';
                in1 <= '1';
                in10 <= '1';
                in11 <= '1';
                in12 <= '1';
                in13 <= '1';
                in14 <= '1';
                in15 <= '1';
                in2 <= '1';
                in3 <= '1';
                in4 <= '1';
                in5 <= '1';
                in6 <= '1';
                in7 <= '1';
                in8 <= '1';
                in9 <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  659ns
                WAIT FOR 20 ns;
                in0 <= '0';
                in1 <= '0';
                in10 <= '0';
                in11 <= '0';
                in12 <= '0';
                in13 <= '0';
                in14 <= '0';
                in15 <= '0';
                in2 <= '0';
                in3 <= '0';
                in4 <= '0';
                in5 <= '0';
                in6 <= '0';
                in7 <= '0';
                in8 <= '0';
                in9 <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  59839ns
                WAIT FOR 59180 ns;
                in6 <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  59859ns
                WAIT FOR 20 ns;
                in6 <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  96059ns
                WAIT FOR 36200 ns;
                in6 <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  96119ns
                WAIT FOR 60 ns;
                in6 <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  237839ns
                WAIT FOR 141720 ns;
                in6 <= '1';
                -- -------------------------------------
                -- -------------  Current Time:  237899ns
                WAIT FOR 60 ns;
                in6 <= '0';
                -- -------------------------------------
                -- -------------  Current Time:  309319ns
                WAIT FOR 71420 ns;
                in6 <= '1';
                -- -------------------------------------
                WAIT FOR 190701 ns;

            END PROCESS;

    END testbench_arch;

