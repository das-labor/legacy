--
-- Copyright (C) 1988-2004 Altera Corporation
--
-- Any megafunction design, and related net list (encrypted or decrypted),
-- support information, device programming or simulation file, and any
-- other associated documentation or information provided by Altera or a
-- partner under Altera's Megafunction Partnership Program may be used only
-- to program PLD devices (but not masked PLD devices) from Altera.  Any
-- other use of such megafunction design, net list, support information,
-- device programming or simulation file, or any other related
-- documentation or information is prohibited for any other purpose,
-- including, but not limited to modification, reverse engineering, de-
-- compiling, or use with any other silicon devices, unless such use is
-- explicitly licensed under a separate agreement with Altera or a
-- megafunction partner.  Title to the intellectual property, including
-- patents, copyrights, trademarks, trade secrets, or maskworks, embodied
-- in any such megafunction design, net list, support information, device
-- programming or simulation file, or any other related documentation or
-- information provided by Altera or a megafunction partner, remains with
-- Altera, the megafunction partner, or their respective licensors.  No
-- other licenses, including any licenses needed under any third party's
-- intellectual property, are provided herein.
----------------------------------------------------------------------------
-- ALtera Megafunction Component Declaration File
----------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.all;

package altera_mf_components is
-- pragma translate_off

component altsyncram
	generic (
		address_aclr_a	:	string := "UNUSED";
		address_aclr_b	:	string := "NONE";
		address_reg_b	:	string := "CLOCK1";
		byte_size	:	natural := 8;
		byteena_aclr_a	:	string := "UNUSED";
		byteena_aclr_b	:	string := "NONE";
		byteena_reg_b	:	string := "CLOCK1";
		clock_enable_input_a	:	string := "NORMAL";
		clock_enable_input_b	:	string := "NORMAL";
		clock_enable_output_a	:	string := "NORMAL";
		clock_enable_output_b	:	string := "NORMAL";
		intended_device_family	:	string := "unused";
		implement_in_les	:	string := "OFF";
		indata_aclr_a	:	string := "UNUSED";
		indata_aclr_b	:	string := "NONE";
		indata_reg_b	:	string := "CLOCK1";
		init_file	:	string := "UNUSED";
		init_file_layout	:	string := "PORT_A";
		maximum_depth	:	natural := 0;
		numwords_a	:	natural := 0;
		numwords_b	:	natural := 0;
		operation_mode	:	string := "BIDIR_DUAL_PORT";
		outdata_aclr_a	:	string := "NONE";
		outdata_aclr_b	:	string := "NONE";
		outdata_reg_a	:	string := "UNREGISTERED";
		outdata_reg_b	:	string := "UNREGISTERED";
		power_up_uninitialized	:	string := "FALSE";
		ram_block_type	:	string := "AUTO";
		rdcontrol_aclr_b	:	string := "NONE";
		rdcontrol_reg_b	:	string := "CLOCK1";
		read_during_write_mode_mixed_ports	:	string := "DONT_CARE";
		width_a	:	natural;
		width_b	:	natural := 1;
		width_byteena_a	:	natural := 1;
		width_byteena_b	:	natural := 1;
		widthad_a	:	natural;
		widthad_b	:	natural := 1;
		wrcontrol_aclr_a	:	string := "UNUSED";
		wrcontrol_aclr_b	:	string := "NONE";
		wrcontrol_wraddress_reg_b	:	string := "CLOCK1";
		lpm_hint	:	string := "UNUSED";
		lpm_type	:	string := "altsyncram"
	);
	port(
		aclr0	:	in std_logic := '0';
		aclr1	:	in std_logic := '0';
		address_a	:	in std_logic_vector(widthad_a-1 downto 0);
		address_b	:	in std_logic_vector(widthad_b-1 downto 0) := (others => '1');
		addressstall_a	:	in std_logic := '0';
		addressstall_b	:	in std_logic := '0';
		byteena_a	:	in std_logic_vector(width_byteena_a-1 downto 0) := (others => '1');
		byteena_b	:	in std_logic_vector(width_byteena_b-1 downto 0) := (others => '1');
		clock0	:	in std_logic := '1';
		clock1	:	in std_logic := '1';
		clocken0	:	in std_logic := '1';
		clocken1	:	in std_logic := '1';
		data_a	:	in std_logic_vector(width_a-1 downto 0) := (others => '1');
		data_b	:	in std_logic_vector(width_b-1 downto 0) := (others => '1');
		q_a	:	out std_logic_vector(width_a-1 downto 0);
		q_b	:	out std_logic_vector(width_b-1 downto 0);
		rden_b	:	in std_logic := '1';
		wren_a	:	in std_logic := '0';
		wren_b	:	in std_logic := '0'
	);
end component;


component altpll
generic (   
        intended_device_family     : string := "Stratix" ;
        operation_mode             : string := "NORMAL" ;
        pll_type                   : string := "AUTO" ;
        qualify_conf_done          : string := "OFF" ;
        compensate_clock           : string := "CLK0" ;
        scan_chain                 : string := "LONG";
        primary_clock              : string := "inclk0" ;
        inclk0_input_frequency     : positive;   -- required parameter
        inclk1_input_frequency     : natural := 0;
        gate_lock_signal           : string := "NO";
        gate_lock_counter          : integer := 0;
        lock_high                  : natural := 1;
        lock_low                   : natural := 5;
        valid_lock_multiplier      : natural := 1;
        invalid_lock_multiplier    : natural := 5;
        switch_over_type           : string := "AUTO";
        switch_over_on_lossclk     : string := "OFF" ;
        switch_over_on_gated_lock  : string := "OFF" ;
        enable_switch_over_counter : string := "OFF";
        switch_over_counter        : natural := 0;
        feedback_source            : string := "EXTCLK0" ;
        bandwidth                  : natural := 0;
        bandwidth_type             : string := "UNUSED";
        spread_frequency           : natural := 0;
        down_spread                : string := "0.0";
        -- simulation-only parameters 
        simulation_type            : string := "functional";
        source_is_pll              : string := "off";
        skip_vco                    : string := "off";

        -- internal clock specifications
        clk5_multiply_by           : positive := 1;
        clk4_multiply_by           : positive := 1;
        clk3_multiply_by           : positive := 1;
        clk2_multiply_by           : positive := 1;
        clk1_multiply_by           : positive := 1;
        clk0_multiply_by           : positive := 1;
        clk5_divide_by             : positive := 1;
        clk4_divide_by             : positive := 1;
        clk3_divide_by             : positive := 1;
        clk2_divide_by             : positive := 1;
        clk1_divide_by             : positive := 1;
        clk0_divide_by             : positive := 1;
        clk5_phase_shift           : string := "0";
        clk4_phase_shift           : string := "0";
        clk3_phase_shift           : string := "0";
        clk2_phase_shift           : string := "0";
        clk1_phase_shift           : string := "0";
        clk0_phase_shift           : string := "0";
        clk5_time_delay            : string := "0";
        clk4_time_delay            : string := "0";
        clk3_time_delay            : string := "0";
        clk2_time_delay            : string := "0";
        clk1_time_delay            : string := "0";
        clk0_time_delay            : string := "0";
        clk5_duty_cycle            : natural := 50;
        clk4_duty_cycle            : natural := 50;
        clk3_duty_cycle            : natural := 50;
        clk2_duty_cycle            : natural := 50;
        clk1_duty_cycle            : natural := 50;
        clk0_duty_cycle            : natural := 50;
        -- external clock specifications
        extclk3_multiply_by        : positive := 1;
        extclk2_multiply_by        : positive := 1;
        extclk1_multiply_by        : positive := 1;
        extclk0_multiply_by        : positive := 1;
        extclk3_divide_by          : positive := 1;
        extclk2_divide_by          : positive := 1;
        extclk1_divide_by          : positive := 1;
        extclk0_divide_by          : positive := 1;
        extclk3_phase_shift        : string := "0";
        extclk2_phase_shift        : string := "0";
        extclk1_phase_shift        : string := "0";
        extclk0_phase_shift        : string := "0";
        extclk3_time_delay         : string := "0";
        extclk2_time_delay         : string := "0";
        extclk1_time_delay         : string := "0";
        extclk0_time_delay         : string := "0";
        extclk3_duty_cycle         : natural := 50;
        extclk2_duty_cycle         : natural := 50;
        extclk1_duty_cycle         : natural := 50;
        extclk0_duty_cycle         : natural := 50;
        vco_multiply_by            : integer := 1;
        vco_divide_by              : integer := 1;
        sclkout0_phase_shift       : string := "0";
        sclkout1_phase_shift       : string := "0";

        -- advanced user parameters
        vco_min                    : natural := 0;
        vco_max                    : natural := 0;
        vco_center                 : natural := 0;
        pfd_min                    : natural := 0;
        pfd_max                    : natural := 0;
        m_initial                  : natural := 1;
        m                          : natural := 0; -- m must default to 0 to force altpll to calculate the internal parameters for itself
        n                          : natural := 1;
        m2                         : natural := 1;
        n2                         : natural := 1;
        ss                         : natural := 0;
        c0_high                    : natural := 1;
        c1_high                    : natural := 1;
        c2_high                    : natural := 1;
        c3_high                    : natural := 1;
        c4_high                    : natural := 1;
        c5_high                    : natural := 1;
        l0_high                    : natural := 1;
        l1_high                    : natural := 1;
        g0_high                    : natural := 1;
        g1_high                    : natural := 1;
        g2_high                    : natural := 1;
        g3_high                    : natural := 1;
        e0_high                    : natural := 1;
        e1_high                    : natural := 1;
        e2_high                    : natural := 1;
        e3_high                    : natural := 1;
        c0_low                     : natural := 1;
        c1_low                     : natural := 1;
        c2_low                     : natural := 1;
        c3_low                     : natural := 1;
        c4_low                     : natural := 1;
        c5_low                     : natural := 1;
        l0_low                     : natural := 1;
        l1_low                     : natural := 1;
        g0_low                     : natural := 1;
        g1_low                     : natural := 1;
        g2_low                     : natural := 1;
        g3_low                     : natural := 1;
        e0_low                     : natural := 1;
        e1_low                     : natural := 1;
        e2_low                     : natural := 1;
        e3_low                     : natural := 1;
        c0_initial                 : natural := 1;
        c1_initial                 : natural := 1;
        c2_initial                 : natural := 1;
        c3_initial                 : natural := 1;
        c4_initial                 : natural := 1;
        c5_initial                 : natural := 1;
        l0_initial                 : natural := 1;
        l1_initial                 : natural := 1;
        g0_initial                 : natural := 1;
        g1_initial                 : natural := 1;
        g2_initial                 : natural := 1;
        g3_initial                 : natural := 1;
        e0_initial                 : natural := 1;
        e1_initial                 : natural := 1;
        e2_initial                 : natural := 1;
        e3_initial                 : natural := 1;
        c0_mode                    : string := "bypass" ;
        c1_mode                    : string := "bypass" ;
        c2_mode                    : string := "bypass" ;
        c3_mode                    : string := "bypass" ;
        c4_mode                    : string := "bypass" ;
        c5_mode                    : string := "bypass" ;
        l0_mode                    : string := "bypass" ;
        l1_mode                    : string := "bypass" ;
        g0_mode                    : string := "bypass" ;
        g1_mode                    : string := "bypass" ;
        g2_mode                    : string := "bypass" ;
        g3_mode                    : string := "bypass" ;
        e0_mode                    : string := "bypass" ;
        e1_mode                    : string := "bypass" ;
        e2_mode                    : string := "bypass" ;
        e3_mode                    : string := "bypass" ;
        c0_ph                      : natural := 0;
        c1_ph                      : natural := 0;
        c2_ph                      : natural := 0;
        c3_ph                      : natural := 0;
        c4_ph                      : natural := 0;
        c5_ph                      : natural := 0;
        l0_ph                      : natural := 0;
        l1_ph                      : natural := 0;
        g0_ph                      : natural := 0;
        g1_ph                      : natural := 0;
        g2_ph                      : natural := 0;
        g3_ph                      : natural := 0;
        e0_ph                      : natural := 0;
        e1_ph                      : natural := 0;
        e2_ph                      : natural := 0;
        e3_ph                      : natural := 0;
        m_ph                       : natural := 0;
        l0_time_delay              : natural := 0;
        l1_time_delay              : natural := 0;
        g0_time_delay              : natural := 0;
        g1_time_delay              : natural := 0;
        g2_time_delay              : natural := 0;
        g3_time_delay              : natural := 0;
        e0_time_delay              : natural := 0;
        e1_time_delay              : natural := 0;
        e2_time_delay              : natural := 0;
        e3_time_delay              : natural := 0;
        m_time_delay               : natural := 0;
        n_time_delay               : natural := 0;
        c1_use_casc_in             : string := "off";
        c2_use_casc_in             : string := "off";
        c3_use_casc_in             : string := "off";
        c4_use_casc_in             : string := "off";
        c5_use_casc_in             : string := "off";
        extclk3_counter            : string := "e3" ;
        extclk2_counter            : string := "e2" ;
        extclk1_counter            : string := "e1" ;
        extclk0_counter            : string := "e0" ;
        clk5_counter               : string := "l1" ;
        clk4_counter               : string := "l0" ;
        clk3_counter               : string := "g3" ;
        clk2_counter               : string := "g2" ;
        clk1_counter               : string := "g1" ;
        clk0_counter               : string := "g0" ;
        enable0_counter            : string := "l0";
        enable1_counter            : string := "l0";
        charge_pump_current        : natural := 2;
        loop_filter_r              : string := "1.0";
        loop_filter_c              : natural := 5;
        vco_post_scale             : natural := 0;
        lpm_type                   : string := "altpll"
);
port (
        inclk       : in std_logic_vector(1 downto 0) := (others => '0');
        fbin        : in std_logic := '1';
        pllena      : in std_logic := '1';
        clkswitch   : in std_logic := '0';
        areset      : in std_logic := '0';
        pfdena      : in std_logic := '1';
        clkena      : in std_logic_vector(5 downto 0) := (others => '1');
        extclkena   : in std_logic_vector(3 downto 0) := (others => '1');
        scanclk     : in std_logic := '0';
        scanaclr    : in std_logic := '0';
        scanread    : in std_logic := '0';
        scanwrite   : in std_logic := '0';
        scandata    : in std_logic := '0';

        clk         : out std_logic_vector(5 downto 0);
        extclk      : out std_logic_vector(3 downto 0);
        clkbad      : out std_logic_vector(1 downto 0);
        enable0     : out std_logic;
        enable1     : out std_logic;
        activeclock : out std_logic;
        clkloss     : out std_logic;
        locked      : out std_logic;
        scandataout : out std_logic;
        scandone    : out std_logic;
        sclkout0     : out std_logic;
        sclkout1     : out std_logic
);
end component;

-- pragma translate_on

component alt_dummy
port (
        inclk       : in std_logic_vector(1 downto 0);
        sclkout1     : out std_logic
     );
end component;

end;


