------------------------------------------------------------------------------
--  This file is a part of the GRLIB VHDL IP LIBRARY
--  Copyright (C) 2003, Gaisler Research
--
--  This program is free software; you can redistribute it and/or modify
--  it under the terms of the GNU General Public License as published by
--  the Free Software Foundation; either version 2 of the License, or
--  (at your option) any later version.
--
--  This program is distributed in the hope that it will be useful,
--  but WITHOUT ANY WARRANTY; without even the implied warranty of
--  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
--  GNU General Public License for more details.
--
--  You should have received a copy of the GNU General Public License
--  along with this program; if not, write to the Free Software
--  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA 
-----------------------------------------------------------------------------
-- Entity: 	greth
-- File:	greth.vhd
-- Author:	Marko Isomaki 
-- Description:	Ethernet Media Access Controller with Ethernet Debug
--              Communication Link
------------------------------------------------------------------------------
library ieee;
library grlib;
library gaisler; 
use ieee.std_logic_1164.all;
use grlib.stdlib.all;
use grlib.amba.all;
use grlib.devices.all;
library techmap;
use techmap.gencomp.all;
use gaisler.net.all;
use gaisler.ethernet_mac.all;
use gaisler.misc.all;

entity greth is
  generic(
    hindex         : integer := 0;
    pindex         : integer := 0;
    paddr          : integer := 0;
    pmask          : integer := 16#FFF#;
    pirq           : integer := 0;
    memtech        : integer := 0;
    ifg_gap        : integer := 24; 
    attempt_limit  : integer := 16;
    backoff_limit  : integer := 10;
    slot_time      : integer := 128;
    mdcscaler      : integer range 0 to 255 := 25; 
    enable_mdio    : integer range 0 to 1 := 0;
    fifosize       : integer range 4 to 64 := 8;
    nsync          : integer range 1 to 2 := 2;
    edcl           : integer range 0 to 1 := 0;
    edclbufsz      : integer range 1 to 64 := 1;
    macaddrh       : integer := 16#00005E#;
    macaddrl       : integer := 16#000000#;
    ipaddrh        : integer := 16#c0a8#;
    ipaddrl        : integer := 16#0035#;
    phyrstadr      : integer range 0 to 31 := 0;
    rmii           : integer range 0 to 1  := 0); 
  port(
    rst            : in  std_ulogic;
    clk            : in  std_ulogic;
    ahbmi          : in  ahb_mst_in_type;
    ahbmo          : out ahb_mst_out_type;
    apbi           : in  apb_slv_in_type;
    apbo           : out apb_slv_out_type;
    ethi           : in  eth_in_type;
    etho           : out eth_out_type
  );
end entity;
  
architecture rtl of greth is
  --host constants
  constant fabits          : integer := log2(fifosize);
  constant burstlength     : integer := fifosize / 2;
  constant burstbits       : integer := log2(burstlength);
  constant ctrlopcode      : std_logic_vector(15 downto 0) := X"8808"; 
  constant broadcast       : std_logic_vector(47 downto 0) := X"FFFFFFFFFFFF";
  constant maxsizetx       : integer := 1514;
  constant index           : integer := log2(edclbufsz);
  constant receiveOK       : std_logic_vector(3 downto 0) := "0000";
  constant frameCheckError : std_logic_vector(3 downto 0) := "0100";
  constant alignmentError  : std_logic_vector(3 downto 0) := "0001";
  constant frameTooLong    : std_logic_vector(3 downto 0) := "0010";
  constant overrun         : std_logic_vector(3 downto 0) := "1000";
  
  constant REVISION : amba_version_type := 0;

  constant pconfig : apb_config_type := (
  0 => ahb_device_reg ( VENDOR_GAISLER, GAISLER_ETHMAC, 0, REVISION, pirq),
  1 => apb_iobar(paddr, pmask));

  function getfifosize(edcl, fifosize, ebufsize : in integer) return integer is
  begin
    if (edcl = 1) then
      return ebufsize;
    else
      return fifosize;
    end if;
  end function;

  function mirror(din : in std_logic_vector(3 downto 0))
                        return std_logic_vector is
    variable do : std_logic_vector(3 downto 0);
  begin
    do(3) := din(0); do(2) := din(1);
    do(1) := din(2); do(0) := din(3);
    return do;
  end function; 

  function calccrc(d   : in std_logic_vector(3 downto 0);
                   crc : in std_logic_vector(31 downto 0))
                         return std_logic_vector is
    variable ncrc : std_logic_vector(31 downto 0);
    variable tc   : std_logic_vector(3 downto 0);
  begin
    tc(0) := d(0) xor crc(31); tc(1) := d(1) xor crc(30);
    tc(2) := d(2) xor crc(29); tc(3) := d(3) xor crc(28);
    ncrc(31) := crc(27);
    ncrc(30) := crc(26);
    ncrc(29) := tc(0) xor crc(25);
    ncrc(28) := tc(1) xor crc(24);
    ncrc(27) := tc(2) xor crc(23);
    ncrc(26) := tc(0) xor tc(3) xor crc(22);
    ncrc(25) := tc(0) xor tc(1) xor crc(21);
    ncrc(24) := tc(1) xor tc(2) xor crc(20);
    ncrc(23) := tc(2) xor tc(3) xor crc(19);
    ncrc(22) := tc(3) xor crc(18);
    ncrc(21) := crc(17);
    ncrc(20) := crc(16);
    ncrc(19) := tc(0) xor crc(15);
    ncrc(18) := tc(1) xor crc(14);
    ncrc(17) := tc(2) xor crc(13);
    ncrc(16) := tc(3) xor crc(12);
    ncrc(15) := tc(0) xor crc(11);
    ncrc(14) := tc(0) xor tc(1) xor crc(10);
    ncrc(13) := tc(0) xor tc(1) xor tc(2) xor crc(9);
    ncrc(12) := tc(1) xor tc(2) xor tc(3) xor crc(8);
    ncrc(11) := tc(0) xor tc(2) xor tc(3) xor crc(7);
    ncrc(10) := tc(0) xor tc(1) xor tc(3) xor crc(6);
    ncrc(9)  := tc(1) xor tc(2) xor crc(5);
    ncrc(8)  := tc(0) xor tc(2) xor tc(3) xor crc(4);
    ncrc(7)  := tc(0) xor tc(1) xor tc(3) xor crc(3);
    ncrc(6)  := tc(1) xor tc(2) xor crc(2);
    ncrc(5)  := tc(0) xor tc(2) xor tc(3) xor crc(1);
    ncrc(4)  := tc(0) xor tc(1) xor tc(3) xor crc(0);
    ncrc(3)  := tc(0) xor tc(1) xor tc(2);
    ncrc(2)  := tc(1) xor tc(2) xor tc(3);
    ncrc(1)  := tc(2) xor tc(3);
    ncrc(0)  := tc(3);
    return ncrc;
  end function;

  --16-bit one's complement adder
  function crcadder(d1   : in std_logic_vector(15 downto 0);
                    d2   : in std_logic_vector(17 downto 0))
                         return std_logic_vector is
    variable vd1  : std_logic_vector(17 downto 0);
    variable vd2  : std_logic_vector(17 downto 0);
    variable sum  : std_logic_vector(17 downto 0);
  begin
    vd1 := "00" & d1; vd2 := d2;
    sum := vd1 + vd2;
    return sum;
  end function;

  function init_ifg(
    ifg_gap : in integer;
    rmii    : in integer)
              return integer is
  begin
    if rmii = 0 then
      return log2(ifg_gap);
    else
      return log2(ifg_gap*20);
    end if;
  end function;

  --mdio constants
  constant divisor : std_logic_vector(7 downto 0) :=
    conv_std_logic_vector(mdcscaler, 8);

  --transmitter constants
  constant ifg_bits : integer := init_ifg(ifg_gap, rmii);
  constant ifg_p1 : std_logic_vector(ifg_bits-1 downto 0) :=
    conv_std_logic_vector((ifg_gap)/3, ifg_bits);
  constant ifg_p2 : std_logic_vector(ifg_bits-1 downto 0) :=
    conv_std_logic_vector((ifg_gap*2)/3, ifg_bits);
  constant ifg_p1_r100 : std_logic_vector(ifg_bits-1 downto 0) :=
    conv_std_logic_vector((ifg_gap*2)/3, ifg_bits);
  constant ifg_p2_r100 : std_logic_vector(ifg_bits-1 downto 0) :=
    conv_std_logic_vector(rmii*(ifg_gap*4)/3, ifg_bits);
  constant ifg_p1_r10 : std_logic_vector(ifg_bits-1 downto 0) :=
    conv_std_logic_vector(rmii*(ifg_gap*20)/3, ifg_bits);
  constant ifg_p2_r10 : std_logic_vector(ifg_bits-1 downto 0) :=
    conv_std_logic_vector(rmii*(ifg_gap*40)/3, ifg_bits); 

  function ifg_sel(
    rmii  : in integer;
    p1    : in integer;
    speed : in std_ulogic)
            return std_logic_vector is
  begin
    if p1 = 1 then
      if rmii = 0 then
        return ifg_p1;
      else
        if speed = '1' then
          return ifg_p1_r100;
        else
          return ifg_p1_r10;
        end if;
      end if;
    else
      if rmii = 0 then
        return ifg_p2;
      else
        if speed = '1' then
          return ifg_p2_r100;
        else
          return ifg_p2_r10;
        end if;
      end if;
    end if;
  end function;
  
  constant maxattempts : std_logic_vector(4 downto 0) :=
    conv_std_logic_vector(attempt_limit, 5);


  --receiver constants
  constant maxsizerx : integer := 1500;
  constant maxsize   : integer := 1518;
  constant minsize   : integer := 64;

  --edcl constants
  type szvct is array (0 to 6) of integer;
  constant ebuf : szvct := (64, 128, 128, 256, 256, 256, 256);
  constant blbits : szvct := (6, 7, 7, 8, 8, 8, 8);
  constant winsz : szvct := (4, 4, 8, 8, 16, 32, 64);
  constant macaddr : std_logic_vector(47 downto 0) :=
    conv_std_logic_vector(macaddrh, 24) & conv_std_logic_vector(macaddrl, 24);
  constant bpbits : integer := blbits(log2(edclbufsz));
  constant wsz : integer := winsz(log2(edclbufsz));
  constant bselbits : integer := log2(wsz);
  constant eabits: integer := log2(edclbufsz) + 8;
  constant ebufmax : std_logic_vector(bpbits-1 downto 0) := (others => '1');
  constant bufsize : std_logic_vector(2 downto 0) :=
                       conv_std_logic_vector(log2(edclbufsz), 3);
  constant ebufsize : integer := ebuf(log2(edclbufsz));
  constant txfifosize : integer := getfifosize(edcl, fifosize, ebufsize);
  constant txfabits : integer := log2(txfifosize);
  
  type edclrstate_type is (idle, wrda, wrdsa, wrsa, wrtype, ip, ipdata,
                           oplength, arp, iplength, ipcrc, arpop, udp, spill);

  type duplexstate_type is (start, checkrst, readrstbit, checkrstbit, done);
    
  --host types
  type txd_state_type is (idle, read_desc, check_desc, req, fill_fifo,
                          check_result, write_result, readhdr, start, wrbus,
                          etdone, getlen, ahberror);
  type rxd_state_type is (idle, read_desc, check_desc, read_req, read_fifo,
                          discard, write_status, write_status2);

  --mdio types
  type mdio_state_type is (idle, preamble, startst, op, op2, phyadr, regadr,
                           ta, ta2, ta3, data, dataend);

  type ctrl_reg_type is record
    txen	: std_ulogic;
    rxen        : std_ulogic;
    tx_irqen    : std_ulogic;
    rx_irqen    : std_ulogic; 
    full_duplex : std_ulogic; 
    prom        : std_ulogic;
    reset       : std_ulogic;
    speed       : std_ulogic;
  end record;

  type status_reg_type is record
    tx_int      : std_ulogic;
    rx_int      : std_ulogic;
    rx_err      : std_ulogic;
    tx_err      : std_ulogic;
    txahberr    : std_ulogic;
    rxahberr    : std_ulogic;
    toosmall    : std_ulogic;
    invaddr     : std_ulogic;
  end record;

  type mdio_ctrl_reg_type is record
    phyadr   : std_logic_vector(4 downto 0);
    regadr   : std_logic_vector(4 downto 0);
    write    : std_ulogic;
    read     : std_ulogic;
    data     : std_logic_vector(15 downto 0);
    busy     : std_ulogic;
    linkfail : std_ulogic;
  end record;

  subtype mac_addr_reg_type is std_logic_vector(47 downto 0); 

  type fifo_access_in_type is record
    renable   : std_ulogic;    
    raddress  : std_logic_vector(fabits-1 downto 0);
    write     : std_ulogic;
    waddress  : std_logic_vector(fabits-1 downto 0);
    datain    : std_logic_vector(31 downto 0);
  end record;

  type fifo_access_out_type is record
    data      : std_logic_vector(31 downto 0);
  end record;

  type tx_fifo_access_in_type is record
    renable   : std_ulogic;    
    raddress  : std_logic_vector(txfabits-1 downto 0);
    write     : std_ulogic;
    waddress  : std_logic_vector(txfabits-1 downto 0);
    datain    : std_logic_vector(31 downto 0);
  end record;

  type tx_fifo_access_out_type is record
    data      : std_logic_vector(31 downto 0);
  end record;

  type edcl_ram_in_type is record
    renable   : std_ulogic;    
    raddress  : std_logic_vector(eabits-1 downto 0);
    writem    : std_ulogic;
    writel    : std_ulogic;
    waddressm : std_logic_vector(eabits-1 downto 0);
    waddressl : std_logic_vector(eabits-1 downto 0);
    datain    : std_logic_vector(31 downto 0);
  end record;

  type edcl_ram_out_type is record
    data      : std_logic_vector(31 downto 0);
  end record;

  type reg_type is record
    --user registers
    ctrl        : ctrl_reg_type;
    status      : status_reg_type;
    mdio_ctrl   : mdio_ctrl_reg_type;
    mac_addr    : mac_addr_reg_type;
    txdesc      : std_logic_vector(31 downto 10);
    rxdesc      : std_logic_vector(31 downto 10);
    edclip      : std_logic_vector(31 downto 0);
    
    --master tx interface
    txdsel          : std_logic_vector(9 downto 3);
    tmsto           : eth_tx_ahb_in_type;
    txdstate        : txd_state_type;
    txwrap          : std_ulogic;
    txden           : std_ulogic;
    txirq           : std_ulogic;
    txaddr          : std_logic_vector(31 downto 2);
    txlength        : std_logic_vector(10 downto 0);
    txburstcnt      : std_logic_vector(burstbits downto 0);
    tfwpnt          : std_logic_vector(txfabits-1 downto 0);
    tfrpnt          : std_logic_vector(txfabits-1 downto 0);
    tfcnt           : std_logic_vector(txfabits downto 0); 
    txcnt           : std_logic_vector(10 downto 0);
    txstart         : std_ulogic;
    txirqgen        : std_ulogic;
    txstatus        : std_logic_vector(1 downto 0);
    txvalid         : std_ulogic; 
    txdata          : std_logic_vector(31 downto 0);
    txprevaddr      : std_logic_vector(31 downto 0);
    writeok         : std_ulogic;
    txread          : std_logic_vector(nsync-1 downto 0);
    txrestart       : std_logic_vector(nsync downto 0);
    txdone          : std_logic_vector(nsync downto 0);
    txstart_sync    : std_ulogic;
    txreadack       : std_ulogic;
    txdataav        : std_ulogic;
          
    --master rx interface 
    rxdsel          : std_logic_vector(9 downto 3);
    rmsto           : eth_rx_ahb_in_type;
    rxdstate        : rxd_state_type;
    rxstatus        : std_logic_vector(3 downto 0);
    rxaddr          : std_logic_vector(31 downto 2);
    rxlength        : std_logic_vector(10 downto 0);
    rxbytecount     : std_logic_vector(10 downto 0);
    rxwrap          : std_ulogic;
    rxirq           : std_ulogic;
    rfwpnt          : std_logic_vector(fabits-1 downto 0);
    rfrpnt          : std_logic_vector(fabits-1 downto 0);
    rfcnt           : std_logic_vector(fabits downto 0);
    rxcnt           : std_logic_vector(10 downto 0);
    rxdoneold       : std_ulogic;
    rxdoneack       : std_ulogic; 
    rxdone          : std_logic_vector(nsync-1 downto 0);
    rxstart         : std_logic_vector(nsync downto 0);
    rxwrite         : std_logic_vector(nsync-1 downto 0);
    rxwriteack      : std_ulogic; 
    rxburstcnt      : std_logic_vector(burstbits downto 0);
    addrnok         : std_ulogic;
    ctrlpkt         : std_ulogic;
    check           : std_ulogic;
    checkdata       : std_logic_vector(31 downto 0);
    usesizefield    : std_ulogic;
    rxden           : std_ulogic;
    gotframe        : std_ulogic;
    bcast           : std_ulogic; 
    
    --mdio
    mdccnt          : std_logic_vector(7 downto 0);
    mdioclk         : std_ulogic;
    mdioclkold      : std_ulogic;
    mdio_state      : mdio_state_type;
    mdioo           : std_ulogic;
    mdioi           : std_ulogic;
    mdioen          : std_ulogic;
    cnt             : std_logic_vector(4 downto 0);
    error           : std_ulogic;
    done            : std_ulogic;
    start           : std_ulogic;

    --edcl
    edclrstate      : edclrstate_type;
    duplexstate     : duplexstate_type;
    edclactive      : std_ulogic;
    nak             : std_ulogic;
    ewr             : std_ulogic;
    write           : std_logic_vector(wsz-1 downto 0);
    seq             : std_logic_vector(13 downto 0);
    abufs           : std_logic_vector(bselbits downto 0);
    tpnt            : std_logic_vector(bselbits-1 downto 0);
    rpnt            : std_logic_vector(bselbits-1 downto 0);
    tcnt            : std_logic_vector(bpbits-1 downto 0);
    rcntm           : std_logic_vector(bpbits-1 downto 0);
    rcntl           : std_logic_vector(bpbits-1 downto 0);
    ipcrc           : std_logic_vector(17 downto 0);
    applength       : std_logic_vector(15 downto 0);
    oplen           : std_logic_vector(9 downto 0);
    udpsrc          : std_logic_vector(15 downto 0);
    ecnt            : std_logic_vector(3 downto 0);
    tarp            : std_ulogic;
    tnak            : std_ulogic;
    tedcl           : std_ulogic;
    tedclstarted    : std_ulogic;
    edclbcast       : std_ulogic;
  end record;

  --transmitter types
  type tx_state_type is (idle, preamble, sfd, data1, data2, pad1, pad2, fcs,
    fcs2, finish, calc_backoff, wait_backoff, send_jam, send_jam2,
    check_attempts);
  type def_state_type is (monitor, def_on, ifg1, ifg2, frame_waitingst);

  type tx_reg_type is record
    --deference process
    def_state        : def_state_type; 
    ifg_cycls        : std_logic_vector(ifg_bits-1 downto 0);
    deferring        : std_ulogic; 
    was_transmitting : std_ulogic;
    
    --tx process
    main_state   : tx_state_type;
    transmitting : std_ulogic;
    tx_en        : std_ulogic;
    txd          : std_logic_vector(3 downto 0);
    cnt          : std_logic_vector(3 downto 0);
    icnt         : std_logic_vector(1 downto 0);
    crc          : std_logic_vector(31 downto 0);
    crc_en       : std_ulogic; 
    byte_count   : std_logic_vector(10 downto 0);
    slot_count   : std_logic_vector(6 downto 0);
    random       : std_logic_vector(9 downto 0);
    delay_val    : std_logic_vector(9 downto 0);
    retry_cnt    : std_logic_vector(4 downto 0);
    status       : std_logic_vector(1 downto 0); 
    data         : std_logic_vector(31 downto 0);
    
    --synchronization
    read         : std_ulogic;
    done         : std_ulogic;
    restart      : std_ulogic;
    start        : std_logic_vector(nsync downto 0);
    read_ack     : std_logic_vector(nsync-1 downto 0);
    crs          : std_logic_vector(1 downto 0);
    col          : std_logic_vector(1 downto 0);
    fullduplex   : std_logic_vector(1 downto 0);
    
    --rmii
    crs_act      : std_ulogic;
    crs_prev     : std_ulogic;
    speed        : std_logic_vector(1 downto 0);
    rcnt         : std_logic_vector(3 downto 0);
    switch       : std_ulogic;
    txd_msb      : std_logic_vector(1 downto 0);
    zero         : std_ulogic;
    rmii_crc_en  : std_ulogic;
  end record;

  --receiver types
  type rx_state_type is (idle, wait_sfd, data1, data2, errorst, report_status,
                         wait_report, check_crc, discard_packet);
  
  type rx_reg_type is record
    er              : std_ulogic;
    en              : std_ulogic;
    rxd             : std_logic_vector(3 downto 0);                 
    crc             : std_logic_vector(31 downto 0);
    sync_start      : std_ulogic;
    gotframe        : std_ulogic;
    start           : std_ulogic;
    write           : std_ulogic; 
    done            : std_ulogic; 
    odd_nibble      : std_ulogic;
    byte_count      : std_logic_vector(10 downto 0);
    data            : std_logic_vector(31 downto 0);
    dataout         : std_logic_vector(31 downto 0);
    rx_state        : rx_state_type;
    status          : std_logic_vector(3 downto 0);
    write_ack       : std_logic_vector(nsync-1 downto 0);
    done_ack        : std_logic_vector(nsync downto 0);
    rxen            : std_logic_vector(1 downto 0);

    --rmii
    enold           : std_ulogic;
    act             : std_ulogic;
    dv              : std_ulogic;
    cnt             : std_logic_vector(3 downto 0);
    rxd2            : std_logic_vector(1 downto 0);
    speed           : std_logic_vector(1 downto 0);
    zero            : std_ulogic;
  end record;

  --host signals
  signal irst             : std_ulogic;
  signal rxenable         : std_ulogic;
  signal vcc              : std_ulogic;
  
  signal rxfi             : fifo_access_in_type;
  signal txfi             : tx_fifo_access_in_type; 
  signal rxfo             : fifo_access_out_type;
  signal txfo             : tx_fifo_access_out_type;
  signal eri              : edcl_ram_in_type;
  signal ero              : edcl_ram_out_type;

  signal tmsto            : eth_tx_ahb_in_type;
  signal tmsti            : eth_tx_ahb_out_type;

  signal rmsto            : eth_rx_ahb_in_type;
  signal rmsti            : eth_rx_ahb_out_type;


  --DEBUG!!!!!
--   signal dv_d               : std_ulogic;
--   signal rdv_d              : std_ulogic;
--   signal rxd_d              : std_logic_vector(3 downto 0);
--   signal act_d              : std_ulogic;

  signal r, rin           : reg_type;

  --transmitter signals
  signal tr, trin         : tx_reg_type;
  signal txclk            : std_ulogic;
  signal txrst            : std_ulogic;
  
  --receiver signals
  signal rr, rrin         : rx_reg_type;
  signal rxclk            : std_ulogic;
  signal rxrst            : std_ulogic;
  
begin
    --debug
--   etho.tarp       <= r.tarp;
--   etho.txrestart  <= r.txrestart(0);
--   etho.txstart    <= r.txstart_sync;
--   etho.txdone     <= r.txdone(0);
--   etho.rxstart    <= rr.start;
--   etho.rxgotframe <= r.gotframe;
--   etho.rxdone     <= r.rxdoneold;
--   etho.edclact    <= r.edclactive;
--   etho.dv         <= dv_d;
--   etho.rdv        <= rdv_d;
--   etho.rxd_d      <= rxd_d;
--   etho.act        <= act_d;
--   etho.seq        <= r.seq;


--  etho.abufs      <= r.abufs;
--   etho.tfcnt      <= r.tfcnt;
--   etho.rfcnt      <= r.rfcnt;
--  etho.tpnt       <= r.tpnt;  
--  etho.rpnt       <= r.rpnt;  

--   debug : process(r, rr, tr) is
--   begin
--     case tr.main_state is
--       when idle =>
--         etho.trmain <= "0000";
--       when preamble =>
--         etho.trmain <= "0001";
--       when sfd =>
--         etho.trmain <= "0010";
--       when data1 =>
--         etho.trmain <= "0011";
--       when data2 =>
--         etho.trmain <= "0100";
--       when pad1 =>
--         etho.trmain <= "0101";
--       when pad2 =>
--         etho.trmain <= "0110";
--       when fcs =>
--         etho.trmain <= "0111";
--       when finish =>
--         etho.trmain <= "1000";
--       when calc_backoff =>
--         etho.trmain <= "1001";
--       when wait_backoff =>
--         etho.trmain <= "1010";
--       when send_jam =>
--         etho.trmain <= "1011";
--       when send_jam2 =>
--         etho.trmain <= "1100";
--       when check_attempts =>
--         etho.trmain <= "1101";
--       when others =>
--         etho.trmain <= "1110";
--     end case;
    
--     case tr.def_state is
--       when monitor =>
--         etho.trdef <= "000";
--       when def_on =>
--         etho.trdef <= "001";
--       when ifg1 =>
--         etho.trdef <= "010";
--       when ifg2 =>
--         etho.trdef <= "011";
--       when frame_waitingst =>
--         etho.trdef <= "100";
--       when others =>
--         etho.trdef <= "101";
--     end case;

--     case rr.rx_state is
--       when idle =>
--         etho.rrmain <= "0000";
--       when wait_sfd =>
--         etho.rrmain <= "0001";
--       when data1 =>
--         etho.rrmain <= "0010";
--       when data2 =>
--         etho.rrmain <= "0011";
--       when errorst =>
--         etho.rrmain <= "0100";
--       when report_status =>
--         etho.rrmain <= "0101";
--       when wait_report =>
--         etho.rrmain <= "0110";
--       when check_crc =>
--         etho.rrmain <= "0111";
--       when discard_packet =>
--         etho.rrmain <= "1000";
--       when others =>
--         etho.rrmain <= "1001";
--     end case;

--     case r.txdstate is
--       when idle =>
--         etho.txdma <= "0000";
--       when read_desc =>
--         etho.txdma <= "0001";
--       when check_desc =>
--         etho.txdma <= "0010";
--       when req =>
--         etho.txdma <= "0011";
--       when fill_fifo =>
--         etho.txdma <= "0100";
--       when check_result =>
--         etho.txdma <= "0101";
--       when write_result =>
--         etho.txdma <= "0110";
--       when readhdr =>
--         etho.txdma <= "0111";
--       when start =>
--         etho.txdma <= "1000";
--       when wrbus =>
--         etho.txdma <= "1001";
--       when etdone =>
--         etho.txdma <= "1010";
--       when getlen =>
--         etho.txdma <= "1011";
--       when others =>
--         etho.txdma <= "1100";
--     end case;

--     case r.rxdstate is
--       when idle =>
--         etho.rxdma <= "0000";
--       when read_desc =>
--         etho.rxdma <= "0001";
--       when check_desc =>
--         etho.rxdma <= "0010";
--       when read_req =>
--         etho.rxdma <= "0011";
--       when read_fifo =>
--         etho.rxdma <= "0100";
--       when discard =>
--         etho.rxdma <= "0101";
--       when write_status =>
--         etho.rxdma <= "0110";
--       when write_status2 =>
--         etho.rxdma <= "0111";
--       when others =>
--         etho.rxdma <= "1000";
--     end case;

--     case r.edclrstate is
--       when idle =>
--         etho.tedcl <= "0000";
--       when wrda =>
--         etho.tedcl <= "0001";
--       when wrdsa =>
--         etho.tedcl <= "0010";
--       when wrsa =>
--         etho.tedcl <= "0011";
--       when wrtype =>
--         etho.tedcl <= "0100";
--       when ip =>
--         etho.tedcl <= "0101";
--       when ipdata =>
--         etho.tedcl <= "0110";
--       when oplength =>
--         etho.tedcl <= "0111";
--       when arp =>
--         etho.tedcl <= "1000";
--       when iplength =>
--         etho.tedcl <= "1001";
--       when ipcrc =>
--         etho.tedcl <= "1010";
--       when arpop =>
--         etho.tedcl <= "1011";
--       when udp =>
--         etho.tedcl <= "1100";
--       when spill =>
--         etho.tedcl <= "1101";
--       when others =>
--         etho.tedcl <= "1110";
--     end case;
--     end process;
  
  --reset generators for transmitter and receiver
  vcc <= '1';
  irst <= rst and not r.ctrl.reset;
  tx_rst : rstgen
  port map(irst, txclk, vcc, txrst, open);

  rx_rst : rstgen
  port map(irst, rxclk, vcc, rxrst, open);

  --clock assignments, needed because synopsys DC does
  --not allow record elements in rising_edge statement
  rmii0 : if rmii = 0 generate 
    rxclk <= ethi.rx_clk;
    txclk <= ethi.tx_clk;
  end generate;

  rmii1 : if rmii = 1 generate
    rxclk <= ethi.rmii_clk;
    txclk <= ethi.rmii_clk;
  end generate;
  
  comb : process(rst, irst, ethi, r, apbi, rmsti, tmsti, txfo, rxfo, rr, tr, ero) is
    variable v             : reg_type;
    variable vpirq         : std_logic_vector(NAHBIRQ-1 downto 0);
    variable prdata        : std_logic_vector(31 downto 0);
    variable txvalid       : std_ulogic;
    variable vtxfi         : tx_fifo_access_in_type; 
    variable vrxfi         : fifo_access_in_type;
    variable lengthav      : std_ulogic;
    variable txdone        : std_ulogic;
    variable txread        : std_ulogic;
    variable txrestart     : std_ulogic;
    variable rxstart       : std_ulogic;
    variable rxdone        : std_ulogic;
    variable rxwrite       : std_ulogic;
    variable ovrunstop     : std_ulogic;
    --mdio
    variable mdioindex     : integer range 0 to 31;
    variable mclk          : std_ulogic;
    --edcl
    variable veri          : edcl_ram_in_type;
    variable swap          : std_ulogic;
    variable setmz         : std_ulogic;
    variable ipcrctmp      : std_logic_vector(15 downto 0);
    variable ipcrctmp2     : std_logic_vector(17 downto 0);
    variable vrxenable     : std_ulogic;
    variable crctmp        : std_ulogic;
    variable vecnt         : integer;
  begin 
    v := r; prdata := (others => '0'); vpirq := (others => '0');
    v.check := '0'; lengthav := r.rxdoneold or r.usesizefield;
    ovrunstop := '0';
    
    vtxfi.datain := tmsti.data; 
    vtxfi.raddress := r.tfrpnt; vtxfi.write := '0';
    vtxfi.waddress := r.tfwpnt; vtxfi.renable := '1';

    vrxfi.datain := rr.dataout; 
    vrxfi.write := '0'; vrxfi.waddress := r.rfwpnt;
    vrxfi.renable := '1'; vrxenable := r.ctrl.rxen;

    --synchronization
    v.txdone(0)     := tr.done;
    v.txread(0)     := tr.read;
    v.txrestart(0)  := tr.restart;
    v.rxstart(0)    := rr.sync_start;
    v.rxdone(0)     := rr.done;
    v.rxwrite(0)    := rr.write;
        
    if nsync = 2 then
      v.txdone(1)     := r.txdone(0);
      v.txread(1)     := r.txread(0);
      v.txrestart(1)  := r.txrestart(0);
      v.rxstart(1)    := r.rxstart(0);
      v.rxdone(1)     := r.rxdone(0);
      v.rxwrite(1)    := r.rxwrite(0);
    end if;

    txdone     := r.txdone(nsync)     xor r.txdone(nsync-1);
    txread     := r.txreadack         xor r.txread(nsync-1);
    txrestart  := r.txrestart(nsync)  xor r.txrestart(nsync-1);
    rxstart    := r.rxstart(nsync)    xor r.rxstart(nsync-1);
    rxdone     := r.rxdoneack         xor r.rxdone(nsync-1);
    rxwrite    := r.rxwriteack        xor r.rxwrite(nsync-1);
   
    if txdone = '1' then
      v.txstatus := tr.status;
    end if;
    
-------------------------------------------------------------------------------
-- HOST INTERFACE -------------------------------------------------------------
-------------------------------------------------------------------------------
   --SLAVE INTERFACE

   --write
   if (apbi.psel(pindex) and apbi.penable and apbi.pwrite) = '1' then
     case apbi.paddr(5 downto 2) is
     when "0000" => --ctrl reg
       if rmii = 1 then
       v.ctrl.speed       := apbi.pwdata(7);  
       end if;
       v.ctrl.reset       := apbi.pwdata(6);
       v.ctrl.prom        := apbi.pwdata(5); 
       v.ctrl.full_duplex := apbi.pwdata(4);
       v.ctrl.rx_irqen    := apbi.pwdata(3);
       v.ctrl.tx_irqen    := apbi.pwdata(2);
       v.ctrl.rxen        := apbi.pwdata(1);
       v.ctrl.txen        := apbi.pwdata(0);
     when "0001" => --status/int source reg
       if apbi.pwdata(7) = '1' then v.status.invaddr  := '0'; end if;
       if apbi.pwdata(6) = '1' then v.status.toosmall := '0'; end if;
       if apbi.pwdata(5) = '1' then v.status.txahberr := '0'; end if;
       if apbi.pwdata(4) = '1' then v.status.rxahberr := '0';  end if;
       if apbi.pwdata(3) = '1' then v.status.tx_int := '0'; end if;
       if apbi.pwdata(2) = '1' then v.status.rx_int := '0'; end if;
       if apbi.pwdata(1) = '1' then v.status.tx_err := '0'; end if;
       if apbi.pwdata(0) = '1' then v.status.rx_err := '0'; end if;
     when "0010" => --mac addr msb/mdio address
       v.mac_addr(47 downto 32) := apbi.pwdata(15 downto 0);
     when "0011" => --mac addr lsb
       v.mac_addr(31 downto 0)  := apbi.pwdata(31 downto 0);
     when "0100" => --mdio ctrl/status
       if enable_mdio = 1 then 
         v.mdio_ctrl.data   := apbi.pwdata(31 downto 16);
         v.mdio_ctrl.phyadr := apbi.pwdata(15 downto 11);
         v.mdio_ctrl.regadr := apbi.pwdata(10 downto 6);
         if r.mdio_ctrl.busy = '0' then
           v.mdio_ctrl.read   := apbi.pwdata(1);
           v.mdio_ctrl.write  := apbi.pwdata(0);
           v.mdio_ctrl.busy := apbi.pwdata(1) or apbi.pwdata(0);
         end if;
       end if;
     when "0101" => --tx descriptor 
       v.txdesc := apbi.pwdata(31 downto 10);
       v.txdsel := apbi.pwdata(9 downto 3);
     when "0110" => --rx descriptor
       v.rxdesc := apbi.pwdata(31 downto 10);
       v.rxdsel := apbi.pwdata(9 downto 3);
     when "0111" => --edcl ip
       if (edcl = 1) then
       v.edclip := apbi.pwdata;
       end if;
     when others => null; 
     end case; 
   end if;

   --read
   case apbi.paddr(5 downto 2) is
   when "0000" => --ctrl reg
     if (edcl = 1) then
       prdata(31) := '1';
       prdata(30 downto 28) := bufsize;
     end if;
     if rmii = 1 then
     prdata(7) := r.ctrl.speed;
     end if;
     prdata(6) := r.ctrl.reset;
     prdata(5) := r.ctrl.prom;
     prdata(4) := r.ctrl.full_duplex;
     prdata(3) := r.ctrl.rx_irqen;
     prdata(2) := r.ctrl.tx_irqen;
     prdata(1) := r.ctrl.rxen;
     prdata(0) := r.ctrl.txen; 
   when "0001" => --status/int source reg
     prdata(5) := r.status.invaddr;
     prdata(4) := r.status.toosmall;
     prdata(5) := r.status.txahberr;
     prdata(4) := r.status.rxahberr;
     prdata(3) := r.status.tx_int;
     prdata(2) := r.status.rx_int;
     prdata(1) := r.status.tx_err;
     prdata(0) := r.status.rx_err; 
   when "0010" => --mac addr msb/mdio address
     prdata(15 downto 0) := r.mac_addr(47 downto 32);
   when "0011" => --mac addr lsb
     prdata := r.mac_addr(31 downto 0); 
   when "0100" => --mdio ctrl/status
     prdata(31 downto 16) := r.mdio_ctrl.data;
     prdata(15 downto 11) := r.mdio_ctrl.phyadr;
     prdata(10 downto 6) :=  r.mdio_ctrl.regadr;  
     prdata(3) := r.mdio_ctrl.busy;
     prdata(2) := r.mdio_ctrl.linkfail;
     prdata(1) := r.mdio_ctrl.read;
     prdata(0) := r.mdio_ctrl.write; 
   when "0101" => --tx descriptor 
     prdata(31 downto 10) := r.txdesc;
     prdata(9 downto 3)   := r.txdsel;
   when "0110" => --rx descriptor
     prdata(31 downto 10) := r.rxdesc;
     prdata(9 downto 3)   := r.rxdsel;
   when "0111" => --edcl ip
     if (edcl = 1) then
     prdata := r.edclip;
     end if;
   when others => null; 
   end case;   
      
   --MASTER INTERFACE

   --tx dma fsm
   case r.txdstate is
   when idle =>
     v.txcnt := (others => '0');
     if (edcl = 1) then
       v.tedcl := '0'; v.tedclstarted := '0';
     end if;
     if (edcl = 1) and (conv_integer(r.abufs) /= 0)  then
       v.txdstate := getlen;
       v.tcnt := conv_std_logic_vector(10, bpbits);
     elsif r.ctrl.txen = '1' then
       v.txdstate := read_desc; v.tmsto.write := '0';  
       v.tmsto.addr := r.txdesc & r.txdsel & "000"; v.tmsto.req := '1';
     end if;
     if r.txirqgen = '1' then
       vpirq(pirq) := '1'; v.txirqgen := '0';
     end if;
     if txrestart = '1' then
       v.txrestart(nsync) := r.txrestart(nsync-1);
       v.tfcnt := (others => '0'); v.tfrpnt := (others => '0');
       v.tfwpnt := (others => '0');
     end if;
   when read_desc =>
     v.tmsto.write := '0'; v.txstatus := (others => '0'); 
     v.tfwpnt := (others => '0'); v.tfrpnt := (others => '0');
     v.tfcnt := (others => '0');
     if tmsti.grant = '1' then
       v.tmsto.addr := r.tmsto.addr + 4;
     end if;
     if tmsti.ready = '1' then
       v.txcnt := r.txcnt + 1; v.tmsto.req := '0';
       case r.txcnt(1 downto 0) is
         when "00" =>
           v.txlength  := tmsti.data(10 downto 0);
           v.txden     := tmsti.data(11);
           v.txwrap    := tmsti.data(12);
           v.txirq     := tmsti.data(13);
           v.ctrl.txen := tmsti.data(11);
         when "01" =>
           v.txaddr    := tmsti.data(31 downto 2);
           v.txdstate  := check_desc;
         when others => null;
       end case; 
     end if;
   when check_desc =>
     v.txstart := '0'; 
     v.txburstcnt := (others => '0'); 
     if r.txden = '1' then
       if (conv_integer(r.txlength) > maxsizetx) or
                  (conv_integer(r.txlength) = 0) then
         v.txdstate := write_result; v.tmsto.req := '1';
         v.tmsto.write := '1'; v.tmsto.addr := r.txdesc & r.txdsel & "000";
         v.tmsto.data := (others => '0');
       else
         v.txdstate := req;
         v.tmsto.addr := r.txaddr & "00"; v.txcnt(10 downto 0) := r.txlength;
       end if;
     else
       v.txdstate := idle;
     end if;
   when req =>
     if txrestart = '1' then
       v.txdstate := idle; v.txstart := '0'; 
       if (edcl = 1) and (r.tedcl = '1') then
         v.txdstate := idle; 
       end if;
     elsif txdone = '1' then
       v.txdstate := check_result;
       v.tfcnt := (others => '0'); v.tfrpnt := (others => '0');
       v.tfwpnt := (others => '0');
       if (edcl = 1) and (r.tedcl = '1') then
         v.txdstate := etdone;
       end if;
     elsif conv_integer(r.txcnt) = 0 then
       v.txdstate := check_result;
       if (edcl = 1) and (r.tedcl = '1') then
         v.txdstate := etdone; v.txstart_sync := not r.txstart_sync;
       end if;
     elsif (r.tfcnt(txfabits downto txfabits-1) = "00") or (r.tedcl = '1') then
       v.tmsto.req := '1'; v.txdstate := fill_fifo;
     end if;
     v.txburstcnt := (others => '0');
   when fill_fifo =>
     if tmsti.grant = '1' then
       v.tmsto.addr := r.tmsto.addr + 4;
       if ((conv_integer(r.txcnt) <= 8) and (tmsti.ready = '1')) or
          ((conv_integer(r.txcnt) <= 4) and (tmsti.ready = '0')) then
         v.tmsto.req := '0'; 
       end if;
       if (conv_integer(r.txburstcnt) = burstlength-1) then
         v.tmsto.req := '0';
       else
         v.txburstcnt := r.txburstcnt + 1;
       end if;
     end if;
     if (tmsti.ready = '1') or ((edcl = 1) and (r.tedcl and tmsti.error) = '1') then
       v.tfwpnt := r.tfwpnt + 1; v.tfcnt := r.tfcnt + 1; vtxfi.write := '1';
       if r.tmsto.req = '0' then
         v.txdstate := req;
         if (r.txstart = '0') and not ((edcl = 1) and (r.tedcl = '1')) then
           v.txstart := '1'; v.txstart_sync := not r.txstart_sync; 
         end if;
       end if;
       if conv_integer(r.txcnt) > 3 then
         v.txcnt := r.txcnt - 4;
       else
         v.txcnt := (others => '0');
       end if;
     end if;
   when check_result =>
     if txdone = '1' then
       v.txdstate := write_result; v.tmsto.req := '1'; v.txstart := '0';
       v.tmsto.write := '1'; v.tmsto.addr := r.txdesc & r.txdsel & "000";
       v.tmsto.data(31 downto 16) := (others => '0');
       v.tmsto.data(15 downto 14) := v.txstatus;
       v.tmsto.data(13 downto 0)  := (others => '0');
       v.txdone(nsync) := r.txdone(nsync-1);
     elsif txrestart = '1' then
       v.txdstate := idle; v.txstart := '0'; 
     end if;
   when write_result =>
     if tmsti.grant = '1' then
       v.tmsto.req := '0'; v.tmsto.addr := r.tmsto.addr + 4;
     end if;
     if tmsti.ready = '1' then
       v.txdstate := idle; 
       v.txirqgen := r.ctrl.tx_irqen and r.txirq; 
       if r.txwrap = '0' then v.txdsel := r.txdsel + 1;
       else v.txdsel := (others => '0'); end if;
       if conv_integer(r.txstatus) = 0 then v.status.tx_int := '1';
       else v.status.tx_err := '1'; end if;
     end if;
   when ahberror =>
     v.tfcnt := (others => '0'); v.tfwpnt := (others => '0');
     v.tfrpnt := (others => '0');
     if not ((edcl = 1) and (r.tedcl = '1')) then
       v.status.txahberr := '1'; v.ctrl.txen := '0';
       if r.txstart = '1' then
         if txdone = '1' then
           v.txdstate := idle; v.txdone(nsync) := r.txdone(nsync-1);
         end if;
       else
         v.txdstate := idle;
       end if;
     else
       if r.tedclstarted = '1' then
         if txdone = '1' then
           v.txdstate := idle; v.txdone(nsync) := r.txdone(nsync-1);
           v.abufs := r.abufs - 1; v.tpnt := r.tpnt + 1;
         end if;
       else
         v.txdstate := idle; 
         v.abufs := r.abufs - 1; v.tpnt := r.tpnt + 1;
       end if;
     end if;
   when others =>
     null;
   end case;

   --tx fifo read
   v.txdataav := '0';
   if conv_integer(r.tfcnt) /= 0 then
     v.txdataav := '1';
   end if;
   if txread = '1' then
     v.txreadack := not r.txreadack;
     if r.txdataav = '1' then
       if conv_integer(r.tfcnt) < 2 then
         v.txdataav := '0';
       end if;
       v.txvalid := '1';
       v.tfcnt := v.tfcnt - 1; v.tfrpnt := r.tfrpnt + 1;
     else
       v.txvalid := '0';
     end if;
     v.txdata := txfo.data;
   end if;

   --rx dma fsm
   case r.rxdstate is
   when idle =>
     v.rmsto.req := '0'; v.rmsto.write := '0'; v.addrnok := '0'; 
     v.rxcnt := (others => '0'); v.rxdoneold := '0';
     v.ctrlpkt := '0'; v.bcast := '0';
     if r.ctrl.rxen = '1' then
       v.rxdstate := read_desc; v.rmsto.req := '1';
       v.rmsto.addr := r.rxdesc & r.rxdsel & "000"; 
     elsif rxstart = '1' then
       v.rxstart(nsync) := r.rxstart(nsync-1);
       v.rxdstate := discard;
     end if;
   when read_desc =>
     v.rxstatus := (others => '0');
     if rmsti.grant = '1' then
       v.rmsto.addr := r.rmsto.addr + 4;
     end if;
     if rmsti.ready = '1' then
       v.rxcnt := r.rxcnt + 1; v.rmsto.req := '0';
       case r.rxcnt(1 downto 0) is
         when "00" =>
           v.ctrl.rxen := rmsti.data(11);
           v.rxden     := rmsti.data(11);
           v.rxwrap    := rmsti.data(12);
           v.rxirq     := rmsti.data(13);
         when "01" =>
           v.rxaddr    := rmsti.data(31 downto 2);
           v.rxdstate  := check_desc;
         when others =>
           null;
       end case; 
     end if;
     if rmsti.error = '1' then
       v.rmsto.req := '0'; v.rxdstate := idle;
       v.status.rxahberr := '1'; v.ctrl.rxen := '0';
     end if;
   when check_desc =>
     v.rxcnt := (others => '0'); v.usesizefield := '0'; v.rmsto.write := '1'; 
     if r.rxden = '1' then
       if rxstart = '1' then
         v.rxdstate := read_req; v.rxstart(nsync) := r.rxstart(nsync-1);
       end if; 
     else
       v.rxdstate := idle;
     end if;
     v.rmsto.addr := r.rxaddr & "00";
   when read_req =>
     if (rxdone and rr.status(3)) = '1' then
       v.rxdstate := write_status; ovrunstop := '1';
       v.rfcnt := (others => '0'); v.rfwpnt := (others => '0');
       v.rfrpnt := (others => '0'); v.writeok := '1';
     elsif (r.addrnok or r.ctrlpkt) = '1' then
       v.rxdstate := discard; v.status.invaddr := '1';
     elsif ((r.rxdoneold = '1') and r.rxcnt >= r.rxlength) then
       if r.gotframe = '1' then
         v.rxdstate := write_status;
       else
         v.rxdstate := discard; v.status.toosmall := '1';
       end if;
     elsif (r.rfcnt(fabits) or r.rfcnt(fabits-1) or r.rxdoneold) = '1' then
       v.rmsto.req := '1'; v.rxdstate := read_fifo;
       v.rfrpnt := r.rfrpnt + 1; v.rfcnt := r.rfcnt - 1;
     end if;
     v.rxburstcnt := (others => '0'); v.rmsto.data := rxfo.data; 
   when read_fifo =>
     if rmsti.grant = '1' then
       v.rmsto.addr := r.rmsto.addr + 4;
       if (lengthav = '1') then
         if ((conv_integer(r.rxcnt) >=
              (conv_integer(r.rxlength) - 8)) and (rmsti.ready = '1')) or
         ((conv_integer(r.rxcnt) >=
           (conv_integer(r.rxlength) - 4)) and (rmsti.ready = '0')) then
           v.rmsto.req := '0'; 
         end if;
       end if;
       if (conv_integer(r.rxburstcnt) = burstlength-1) then
         v.rmsto.req := '0';
       else
         v.rxburstcnt := r.rxburstcnt + 1;
       end if;
     end if;
     if rmsti.ready = '1' then
       v.rmsto.data := rxfo.data; 
       v.rxcnt := r.rxcnt + 4;  
       if r.rmsto.req = '0' then
         v.rxdstate := read_req;
       else
         v.rfcnt := r.rfcnt - 1; v.rfrpnt := r.rfrpnt + 1;
       end if;
       v.check := '1'; v.checkdata := r.rmsto.data; 
     end if;
     if rmsti.error = '1' then
       v.rmsto.req := '0'; v.rxdstate := discard;
       v.rxcnt := r.rxcnt + 4;
       v.status.rxahberr := '1'; v.ctrl.rxen := '0';
     end if;
   when write_status =>
     v.rmsto.req := '1'; v.rmsto.addr := r.rxdesc & r.rxdsel & "000";
     v.rxdstate := write_status2;
     v.rmsto.data := X"000" & "00" & r.rxstatus & "000" & r.rxlength;  
   when write_status2 =>
     if rmsti.grant = '1' then
       v.rmsto.req := '0'; v.rmsto.addr := r.rmsto.addr + 4;
     end if;
     if rmsti.ready = '1' then
       if r.rxstatus(3) = '0' then
         v.rxdstate := discard;
       else
         v.rxdstate := idle;
       end if;
       if (r.ctrl.rx_irqen and r.rxirq) = '1' then
         vpirq(pirq) := '1';
       end if;
       if conv_integer(r.rxstatus) = 0 then v.status.rx_int := '1';
       else v.status.rx_err := '1'; end if;
       if r.rxwrap = '1' then
         v.rxdsel := (others => '0');
       else
         v.rxdsel := r.rxdsel + 1;
       end if;
     end if;
     if rmsti.error = '1' then
       v.rmsto.req := '0'; v.rxdstate := idle;
       v.status.rxahberr := '1'; v.ctrl.rxen := '0';
     end if;
   when discard =>
     if (r.rxdoneold = '0') or ((r.rxdoneold = '1') and
        (conv_integer(r.rxcnt) < conv_integer(r.rxbytecount))) then
       if conv_integer(r.rfcnt) /= 0 then
         v.rfrpnt := r.rfrpnt + 1; v.rfcnt := r.rfcnt - 1;
         v.rxcnt := r.rxcnt + 4;
       end if;
     elsif (r.rxdoneold = '1') then
       v.rxdstate := idle; v.ctrlpkt := '0';
     end if;
   when others =>
     null;
   end case;
   
   --rx address/type check
   if r.check = '1' and r.rxcnt(10 downto 5) = "000000" then 
     case r.rxcnt(4 downto 2) is
     when "001" =>
       if r.checkdata /= broadcast(47 downto 16) and
          r.checkdata /= r.mac_addr(47 downto 16) and
          (not r.ctrl.prom) = '1'then
         v.addrnok := '1';
       elsif r.checkdata = broadcast(47 downto 16) then
         v.bcast := '1';
       end if;
     when "010" =>
       if r.checkdata(31 downto 16) /= broadcast(15 downto 0) and
          r.checkdata(31 downto 16) /= r.mac_addr(15 downto 0) and
          (not r.ctrl.prom) = '1' then
         v.addrnok := '1';
       elsif (r.bcast = '0') and
             (r.checkdata(31 downto 16) = broadcast(15 downto 0)) then
         v.addrnok := '1';
       end if;
     when "011" =>
       null;
     when "100" =>
       if r.checkdata(31 downto 16) = ctrlopcode then v.ctrlpkt := '1'; end if;
       if conv_integer(r.checkdata(31 downto 16)) <= maxsizerx then
         v.usesizefield := '1'; v.rxlength := r.checkdata(26 downto 16) + 14;
       end if;
     when others =>
       null;
     end case; 
   end if;
    
   --rx packet overrun
   if (rxdone and not rxstart) = '1' then
     v.gotframe := rr.gotframe; v.rxbytecount := rr.byte_count;
     v.rxstatus := rr.status;
     if r.usesizefield = '0' then
       v.rxlength := rr.byte_count;
     end if;
     v.rxdoneold := '1';
     if ((not rr.status(3)) or (rr.status(3) and ovrunstop)) = '1' then
       v.rxdoneack := not r.rxdoneack; 
     end if;
   end if; 
     
   --rx fifo write
   if rxwrite = '1' then
     v.rxwriteack := not r.rxwriteack;
     if (not r.rfcnt(fabits)) = '1' then 
       v.rfwpnt := r.rfwpnt + 1; v.rfcnt := v.rfcnt + 1; v.writeok := '1'; 
       vrxfi.write := '1'; 
     else
       v.writeok := '0'; 
     end if;
   end if;  

   --must be placed here because it uses variable  
   vrxfi.raddress := v.rfrpnt;

-------------------------------------------------------------------------------
-- MDIO INTERFACE -------------------------------------------------------------
-------------------------------------------------------------------------------
   --mdio commands
   if enable_mdio = 1 then
     mclk := r.mdioclk and not r.mdioclkold;
     v.mdioclkold := r.mdioclk;
     if r.mdccnt = "00000000" then
       v.mdccnt := divisor;
       v.mdioclk := not r.mdioclk;
     else
       v.mdccnt := r.mdccnt - 1;
     end if;
     mdioindex := conv_integer(r.cnt); v.mdioi := ethi.mdio_i;
     if mclk = '1' then
       case r.mdio_state is
         when idle =>
           v.error := '0'; v.cnt := (others => '0');
           if r.mdio_ctrl.busy = '1' then
             if r.mdio_ctrl.read = '1' then
               v.mdio_ctrl.write := '0'; v.mdio_ctrl.linkfail := '0';
             end if;
             v.mdio_state := preamble; v.mdioen := '0'; v.mdioo := '1';
           end if; 
         when preamble =>
           v.cnt := r.cnt + 1; 
           if r.cnt = "11111" then
             v.mdioo := '0'; v.mdio_state := startst; 
           end if; 
         when startst =>
           v.mdioo := '1'; v.mdio_state := op; v.cnt := (others => '0');
         when op =>
           v.mdio_state := op2;    
           if r.mdio_ctrl.read = '1' then v.mdioo := '1';
           else v.mdioo := '0'; end if;
         when op2 =>
           v.mdioo := not r.mdioo; v.mdio_state := phyadr;
           v.cnt := (others => '0');
         when phyadr =>
           v.cnt := r.cnt + 1;
           case mdioindex is
           when 0 => v.mdioo := r.mdio_ctrl.phyadr(4);
           when 1 => v.mdioo := r.mdio_ctrl.phyadr(3);
           when 2 => v.mdioo := r.mdio_ctrl.phyadr(2);
           when 3 => v.mdioo := r.mdio_ctrl.phyadr(1);
           when 4 => v.mdioo := r.mdio_ctrl.phyadr(0);
                     v.mdio_state := regadr; v.cnt := (others => '0');
           when others => null;
           end case;
         when regadr =>
           v.cnt := r.cnt + 1;
           case mdioindex is
           when 0 => v.mdioo := r.mdio_ctrl.regadr(4);
           when 1 => v.mdioo := r.mdio_ctrl.regadr(3);
           when 2 => v.mdioo := r.mdio_ctrl.regadr(2);
           when 3 => v.mdioo := r.mdio_ctrl.regadr(1);
           when 4 => v.mdioo := r.mdio_ctrl.regadr(0);
                     v.mdio_state := ta; v.cnt := (others => '0');
           when others => null;
           end case;
         when ta =>
           v.mdio_state := ta2;
           if r.mdio_ctrl.read = '1' then v.mdioen := '1';
           else v.mdioo := '1'; end if;
         when ta2 =>
           v.cnt := "01111"; v.mdio_state := ta3; 
           if r.mdio_ctrl.write = '1' then v.mdioo := '0'; v.mdio_state := data; end if;
         when ta3 =>
           v.mdio_state := data;
           if r.mdioi /= '0' then
             v.mdio_ctrl.linkfail := '1';
           end if;
         when data =>
           v.cnt := r.cnt - 1;
           if r.mdio_ctrl.read = '1' then
             v.mdio_ctrl.data(mdioindex) := r.mdioi; 
           else
             v.mdioo := r.mdio_ctrl.data(mdioindex);
           end if;
           if r.cnt = "00000" then
             v.mdio_state := dataend;
           end if;
         when dataend =>
           v.mdio_ctrl.busy := '0'; v.mdio_ctrl.read := '0';
           v.mdio_ctrl.write := '0'; v.mdioen := '1';
           v.mdio_state := idle;
         when others =>
           null;
       end case;
     end if;
   end if;

-------------------------------------------------------------------------------
-- EDCL -----------------------------------------------------------------------
-------------------------------------------------------------------------------
   if (edcl = 1) then
     veri.renable := '1'; veri.writem := '0'; veri.writel := '0';
     veri.waddressm := r.rpnt & r.rcntm; veri.waddressl := r.rpnt & r.rcntl;
     swap := '0'; vrxenable := '1'; vecnt := conv_integer(r.ecnt); setmz := '0';
     veri.datain := rr.dataout;

     if rxwrite = '1' then
       v.rxwriteack := not r.rxwriteack;
     end if;

     --edcl receiver 
     case r.edclrstate is
       when idle =>
         v.edclbcast := '0';
         if rxstart = '1' then
           v.edclrstate := wrda; v.edclactive := '0';
           v.rcntm := conv_std_logic_vector(2, bpbits);
           v.rcntl := conv_std_logic_vector(1, bpbits);
         end if;
       when wrda =>
         if rxwrite = '1' then
           v.edclrstate := wrdsa;
           veri.writem := '1'; veri.writel := '1';
           swap := '1';
           v.rcntm := r.rcntm - 2; v.rcntl := r.rcntl + 1;
           if (macaddr(47 downto 16) /= rr.dataout) and
                        (X"FFFFFFFF" /= rr.dataout) then
             v.edclrstate := spill;
           elsif (X"FFFFFFFF" = rr.dataout) then
              v.edclbcast := '1'; 
           end if;
           if conv_integer(r.abufs) = wsz then
             v.edclrstate := spill;
           end if;
         end if;
         if (rxdone and not rxstart) = '1' then
           v.edclrstate := idle;
         end if;
       when wrdsa =>
         if rxwrite = '1' then
           v.edclrstate := wrsa; swap := '1';
           veri.writem := '1'; veri.writel := '1';
           v.rcntm := r.rcntm + 1; v.rcntl := r.rcntl - 2;
           if (macaddr(15 downto 0) /= rr.dataout(31 downto 16)) and
                           (X"FFFF" /= rr.dataout(31 downto 16)) then
             v.edclrstate := spill; 
           elsif (X"FFFF" = rr.dataout(31 downto 16)) then
             v.edclbcast := r.edclbcast; 
           end if;
         end if;
         if (rxdone and not rxstart) = '1' then
           v.edclrstate := idle;
         end if;
       when wrsa =>
         if rxwrite = '1' then
           veri.writem := '1'; veri.writel := '1';
           v.edclrstate := wrtype; swap := '1';
           v.rcntm := r.rcntm + 2; v.rcntl := r.rcntl + 3;
         end if;
         if (rxdone and not rxstart) = '1' then
           v.edclrstate := idle;
         end if;
       when wrtype =>
         if rxwrite = '1' then
           veri.writem := '1'; veri.writel := '1';
           v.rcntm := r.rcntm + 1; v.rcntl := r.rcntl + 1;
           if X"0800" = rr.dataout(31 downto 16) and (r.edclbcast = '0') then
             v.edclrstate := ip;
           elsif X"0806" = rr.dataout(31 downto 16) and (r.edclbcast = '1') then
             v.edclrstate := arp;
           else
             v.edclrstate := spill;
           end if;
         end if;
         v.ecnt := (others => '0'); v.ipcrc := (others => '0');
         if (rxdone and not rxstart) = '1' then
           v.edclrstate := idle;
         end if;
       when ip =>
         if rxwrite = '1' then
           v.ecnt := r.ecnt + 1;
           veri.writem := '1'; veri.writel := '1';
           case vecnt is
             when 0 =>
               v.ipcrc :=
               crcadder(not rr.dataout(31 downto 16), r.ipcrc);
               v.rcntm := r.rcntm + 1; v.rcntl := r.rcntl + 1;
             when 1 =>
               v.rcntm := r.rcntm + 1; v.rcntl := r.rcntl + 2;
             when 2 =>
               v.ipcrc :=
               crcadder(not rr.dataout(31 downto 16), r.ipcrc);
               v.rcntm := r.rcntm + 2; v.rcntl := r.rcntl - 1;
             when 3 =>
               v.rcntm := r.rcntm - 1; v.rcntl := r.rcntl + 2;
             when 4 =>
               v.udpsrc := rr.dataout(15 downto 0);
               v.rcntm := r.rcntm + 2; v.rcntl := r.rcntl + 1; 
             when 5 =>
               setmz := '1';
               v.rcntm := r.rcntm + 1; v.rcntl := r.rcntl + 1; 
             when 6 =>
               v.rcntm := r.rcntm + 1; v.rcntl := r.rcntl + 1; 
             when 7 =>
               v.rcntm := r.rcntm + 1; v.rcntl := r.rcntl + 1;
               if (rr.dataout(31 downto 18) = r.seq) then
                 v.seq := r.seq + 1; v.nak := '0'; 
               else
                 v.nak := '1'; 
                 veri.datain(31 downto 18) := r.seq;
               end if;
               veri.datain(17) := v.nak; v.ewr := rr.dataout(17);
               if (rr.dataout(17) or v.nak) = '1' then
                 veri.datain(16 downto 7) := (others => '0');
               end if;
               v.oplen := rr.dataout(16 downto 7);
               v.applength := "000000" & veri.datain(16 downto 7);
               v.ipcrc :=
               crcadder(v.applength + 38, r.ipcrc);
               v.write(conv_integer(r.rpnt)) := rr.dataout(17);
             when 8 =>
               ipcrctmp := (others => '0');
               ipcrctmp(1 downto 0) := r.ipcrc(17 downto 16);
               ipcrctmp2 := "00" & r.ipcrc(15 downto 0);
               v.ipcrc :=
               crcadder(ipcrctmp, ipcrctmp2);
               v.rcntm := r.rcntm + 1; v.rcntl := r.rcntl + 1;
               v.edclrstate := ipdata;
             when others =>
               null;
           end case;
         end if;
         if (rxdone and not rxstart) = '1' then
           v.edclrstate := idle;
         end if;
       when ipdata =>
         if (rxwrite and r.ewr and not r.nak) = '1' and
                               (r.rcntm /= ebufmax) then
           veri.writem := '1'; veri.writel := '1';
           v.rcntm := r.rcntm + 1; v.rcntl := r.rcntl + 1;
         end if;
         if rxdone = '1' then
           v.edclrstate := ipcrc; v.rcntm := conv_std_logic_vector(6, bpbits);
           ipcrctmp := (others => '0');
           ipcrctmp(1 downto 0) := r.ipcrc(17 downto 16);
           ipcrctmp2 := "00" & r.ipcrc(15 downto 0);
           v.ipcrc := crcadder(ipcrctmp, ipcrctmp2);
           if conv_integer(v.rxstatus) /= 0 then
             v.edclrstate := idle;
           end if;
         end if;
       when ipcrc =>
         veri.writem := '1'; veri.datain(31 downto 16) := not r.ipcrc(15 downto 0);
         v.edclrstate := udp; v.rcntm := conv_std_logic_vector(9, bpbits);
         v.rcntl := conv_std_logic_vector(9, bpbits);
       when udp =>
         veri.writem := '1'; veri.writel := '1';
         v.edclrstate := iplength;
         veri.datain(31 downto 16) := r.udpsrc;
         veri.datain(15 downto 0) := r.applength + 18;
         v.rcntm := conv_std_logic_vector(4, bpbits);
       when iplength =>
         veri.writem := '1';
         veri.datain(31 downto 16) := r.applength + 38;
         v.edclrstate := oplength;
         v.rcntm := conv_std_logic_vector(10, bpbits);
         v.rcntl := conv_std_logic_vector(10, bpbits);
       when oplength =>
         if rxstart = '0' then
           v.abufs := r.abufs + 1; v.rpnt := r.rpnt + 1;
           veri.writel := '1'; veri.writem := '1';
         end if;
         v.edclrstate := idle;
         veri.datain(31 downto 0) := (others => '0');
         veri.datain(15 downto 0) := "00000" & r.nak & r.oplen;
       when arp =>
         if rxwrite = '1' then
           v.ecnt := r.ecnt + 1;
           veri.writem := '1'; veri.writel := '1';
           case vecnt is
             when 0 =>
               v.rcntm := r.rcntm + 4; 
             when 1 =>
               swap := '1'; veri.writel := '0'; 
               v.rcntm := r.rcntm + 1; v.rcntl := r.rcntl + 4;
             when 2 =>
               swap := '1';
               v.rcntm := r.rcntm + 1; v.rcntl := r.rcntl + 1;
             when 3 =>
               swap := '1';
               v.rcntm := r.rcntm - 4; v.rcntl := r.rcntl - 4;
             when 4 =>
               veri.datain := macaddr(31 downto 16) & macaddr(47 downto 32);
               v.rcntm := r.rcntm + 1; v.rcntl := r.rcntl + 1; 
             when 5 =>
               v.rcntl := r.rcntl + 1;
               veri.datain(31 downto 16) := rr.dataout(15 downto 0);
               veri.datain(15 downto 0) := macaddr(15 downto 0);
               if rr.dataout(15 downto 0) /= r.edclip(31 downto 16) then
                 v.edclrstate := spill;
               end if;
             when 6 =>
               swap := '1'; veri.writem := '0'; 
               v.rcntm := conv_std_logic_vector(5, bpbits);
               v.rcntl := conv_std_logic_vector(1, bpbits);
               if rr.dataout(31 downto 16) /= r.edclip(15 downto 0) then
                 v.edclrstate := spill;
               else
                 v.edclactive := '1'; v.rxdstate := discard;
                 v.rmsto.req := '0';
               end if;
             when 7 =>
               veri.writem := '0';
               veri.datain(15 downto 0) := macaddr(47 downto 32);
               v.rcntl := r.rcntl + 1;
               v.rcntm := conv_std_logic_vector(2, bpbits);
             when 8 =>
               v.edclrstate := arpop;
               veri.datain := macaddr(31 downto 0);
               v.rcntm := conv_std_logic_vector(5, bpbits);
             when others =>
               null;
           end case;
         end if;
         if (rxdone and not rxstart) = '1' then
           v.edclrstate := idle;
         end if;
       when arpop =>
         veri.writem := '1'; veri.datain(31 downto 16) := X"0002";
         if (rxdone and not rxstart) = '1' then
           v.edclrstate := idle;
           if conv_integer(v.rxstatus) = 0 and (rr.gotframe = '1') then
             v.abufs := r.abufs + 1; v.rpnt := r.rpnt + 1;
           end if;
         end if;
       when spill =>
         if (rxdone and not rxstart) = '1' then
           v.edclrstate := idle;
         end if;
     end case;
          
     --edcl transmitter
     case r.txdstate is
       when getlen =>
         v.tcnt := r.tcnt + 1;
         if conv_integer(r.tcnt) = 10 then
           v.txlength := '0' & ero.data(9 downto 0);
           v.tnak := ero.data(10);
           v.txcnt := v.txlength;
           if (r.write(conv_integer(r.tpnt)) or v.tnak) = '1' then
             v.txlength := (others => '0');
           end if;
         end if;
         if conv_integer(r.tcnt) = 11 then
           v.txdstate := readhdr;
           v.tcnt := (others => '0');
         end if;
       when readhdr =>
         v.tcnt := r.tcnt + 1; vtxfi.write := '1';
         v.tfwpnt := r.tfwpnt + 1; v.tfcnt := v.tfcnt + 1; 
         vtxfi.datain := ero.data;
         if conv_integer(r.tcnt) = 12 then
           v.txaddr := ero.data(31 downto 2);
         end if;
         if conv_integer(r.tcnt) = 3 then
           if ero.data(31 downto 16) = X"0806" then
             v.tarp := '1'; v.txlength := conv_std_logic_vector(42, 11);
           else
             v.tarp := '0'; v.txlength := r.txlength + 52;
           end if;
         end if;
         if r.tarp = '0' then
           if conv_integer(r.tcnt) = 12 then
             v.txdstate := start;
           end if;
         else
           if conv_integer(r.tcnt) = 10 then
             v.txdstate := start;
           end if;
         end if;
         if (txrestart or txdone) = '1' then
           v.txdstate := etdone;
         end if;
       when start =>
         v.tmsto.addr := r.txaddr & "00"; 
         v.tmsto.write := r.write(conv_integer(r.tpnt));
         if (conv_integer(r.txcnt) = 0) or (r.tarp or r.tnak) = '1' then
           v.tmsto.req := '0'; v.txdstate := etdone;
           v.txstart_sync := not r.txstart_sync;
         elsif r.write(conv_integer(r.tpnt)) = '0' then
           v.txdstate := req; v.tedcl := '1';
         else
           v.txstart_sync := not r.txstart_sync;
           v.txdstate := wrbus; v.tmsto.req := '1'; v.tedcl := '1';
           v.tmsto.data := ero.data; v.tcnt := r.tcnt + 1;
         end if;
         if (txrestart or txdone) = '1' then
           v.txdstate := etdone;
         end if;
       when wrbus =>
         if tmsti.grant = '1' then
           v.tmsto.addr := r.tmsto.addr + 4;
           if ((conv_integer(r.txcnt) <= 4) and (tmsti.ready = '0')) or
              ((conv_integer(r.txcnt) <= 8) and (tmsti.ready = '1')) then
             v.tmsto.req := '0';
           end if;
         end if;
         if (tmsti.ready or tmsti.error) = '1' then
           v.tmsto.data := ero.data; v.tcnt := r.tcnt + 1;
           v.txcnt := r.txcnt - 4;
           if r.tmsto.req = '0' then
             v.txdstate := etdone;
           end if;
         end if;
         if tmsti.retry = '1' then
           v.tmsto.addr := r.tmsto.addr - 4; v.tmsto.req := '1';
         end if;
         --if (txrestart or txdone) = '1' then
          -- v.txdstate := etdone; v.tmsto.req := '0';
         --end if;
       when etdone =>
         if txdone = '1' then
           v.txdstate := idle; v.txdone(nsync) := r.txdone(nsync-1);
           v.abufs := v.abufs - 1; v.tpnt := r.tpnt + 1;
           v.tfcnt := (others => '0'); v.tfrpnt := (others => '0');
           v.tfwpnt := (others => '0');
         elsif txrestart = '1' then
           v.txdstate := idle;
         end if;
       when others =>
         null;
     end case;

     if swap = '1' then
       veri.datain(31 downto 16) := rr.dataout(15 downto 0); 
       veri.datain(15 downto 0)  := rr.dataout(31 downto 16);
     end if;
     if setmz = '1' then
       veri.datain(31 downto 16) := (others => '0');
     end if;
     veri.raddress := r.tpnt & v.tcnt;
   end if;

   --edcl duplex mode read
   if (rmii = 1) or (edcl = 1) then
     case r.duplexstate is
       when start =>
         v.mdio_ctrl.read := '1'; v.mdio_ctrl.busy := '1';
         v.duplexstate := checkrst;
       when checkrst =>
         if r.mdio_ctrl.busy = '0' then
           if r.mdio_ctrl.linkfail = '1' then
             v.duplexstate := start;
           else
             v.duplexstate := readrstbit;
           end if;
         end if;
       when readrstbit =>
         v.mdio_ctrl.read := '1'; v.mdio_ctrl.busy := '1';
         v.duplexstate := checkrstbit;
       when checkrstbit =>
         if r.mdio_ctrl.busy = '0' then
           if r.mdio_ctrl.linkfail = '1' then
             v.duplexstate := done;
           else
             if r.mdio_ctrl.data(15) = '1' then
               v.duplexstate := readrstbit;
             else
               v.ctrl.full_duplex := r.mdio_ctrl.data(8);
               v.ctrl.speed := r.mdio_ctrl.data(13);
               v.duplexstate := done;
             end if;
           end if;
         end if;
       when done =>
         null;
     end case;
   end if;

   --transmitter retry
   if tmsti.retry = '1' then
     v.tmsto.req := '1'; v.tmsto.addr := r.tmsto.addr - 4;
     v.txburstcnt := r.txburstcnt - 1;
   end if;

   --transmitter AHB error
   if tmsti.error = '1' and (not ((edcl = 1) and (r.tedcl = '1'))) then
     v.tmsto.req := '0'; v.txdstate := ahberror;
   end if;
    
   --receiver retry
   if rmsti.retry = '1' then
     v.rmsto.req := '1'; v.rmsto.addr := r.rmsto.addr - 4;
     v.rxburstcnt := r.rxburstcnt - 1;
   end if;

------------------------------------------------------------------------------
-- RESET ----------------------------------------------------------------------
-------------------------------------------------------------------------------
    if irst = '0' then
      v.txdstate := idle; v.rxdstate := idle; v.rfrpnt := (others => '0');
      v.tmsto.req := '0'; v.tmsto.req := '0'; v.rfwpnt := (others => '0'); 
      v.rfcnt := (others => '0');  v.mdio_ctrl.read := '0';
      v.mdio_ctrl.write := '0'; v.ctrl.txen := '0';
      v.mdio_ctrl.busy := '0'; v.txirqgen := '0'; v.ctrl.rxen := '0';
      v.txdsel := (others => '0'); v.txstart_sync := '0';
      v.txread := (others => '0'); v.txrestart := (others => '0');
      v.txdone := (others => '0'); v.txreadack := '0'; 
      v.rxdsel := (others => '0'); v.rxdone := (others => '0');
      v.rxdoneold := '0'; v.rxdoneack := '0'; v.rxwriteack := '0';
      v.rxstart := (others => '0'); v.rxwrite := (others => '0');
      v.mdio_ctrl.linkfail := '0'; v.ctrl.reset := '0';
      v.status.invaddr := '0'; v.status.toosmall := '0';
      if (enable_mdio = 1) then
        v.mdccnt := (others => '0'); v.mdioclk := '0';
        v.mdio_state := idle; v.mdioen := '1'; v.done := '0';
      end if;
      if (edcl = 1) then
        v.tpnt := (others => '0'); v.rpnt := (others => '0');
        v.tcnt := (others => '0'); v.edclactive := '0';
        v.tarp := '0'; v.abufs := (others => '0');
        v.edclrstate := idle;
      end if;
      if (rmii = 1) then
        v.ctrl.speed := '1'; 
      end if;
    end if;

    --some parts of edcl are only affected by hw reset
    if rst = '0' then
      v.edclip := conv_std_logic_vector(ipaddrh, 16) &
                  conv_std_logic_vector(ipaddrl, 16);
      v.duplexstate := start;
      v.mdio_ctrl.phyadr := conv_std_logic_vector(phyrstadr, 5);
      v.seq := (others => '0');
    end if;
-------------------------------------------------------------------------------
-- SIGNAL ASSIGNMENTS ---------------------------------------------------------
-------------------------------------------------------------------------------
    rin           <= v;
    apbo.prdata   <= prdata;                          	
    apbo.pirq     <= vpirq;
    tmsto         <= r.tmsto;
    rmsto         <= r.rmsto;
    rxenable      <= vrxenable;
    etho.mdc      <= r.mdioclk;
    etho.mdio_o   <= r.mdioo; 
    etho.mdio_oe  <= r.mdioen; 

    eri           <= veri;   
    txfi          <= vtxfi;
    rxfi          <= vrxfi;
  end process;

  apbo.pindex    <= pindex;
  apbo.pconfig   <= pconfig; 	

  regs : process(clk) is
  begin
    if rising_edge(clk) then r <= rin; end if;
  end process;

-------------------------------------------------------------------------------
-- TRANSMITTER-----------------------------------------------------------------
-------------------------------------------------------------------------------
  tx : process(txrst, r, tr, ethi) is
    variable collision     : std_ulogic; 
    variable frame_waiting : std_ulogic;
    variable index         : integer range 0 to 7;
    variable start         : std_ulogic;
    variable read_ack      : std_ulogic;
    variable v             : tx_reg_type;

    variable crs           : std_ulogic;
    variable col           : std_ulogic;
    variable tx_done       : std_ulogic;
  begin
    v := tr; frame_waiting := '0'; tx_done := '0'; v.rmii_crc_en := '0';
    
    --synchronization
    v.col(1) := tr.col(0); v.col(0) := ethi.rx_col;
    v.crs(1) := tr.crs(0); v.crs(0) := ethi.rx_crs;
    v.fullduplex(0) := r.ctrl.full_duplex;
    v.fullduplex(1) := tr.fullduplex(0);

    v.start(0)       := r.txstart_sync;
    v.read_ack(0)    := r.txreadack;
     
    if nsync = 2 then
      v.start(1)       := tr.start(0);
      v.read_ack(1)    := tr.read_ack(0);
    end if;

    start       := tr.start(nsync) xor tr.start(nsync-1);
    read_ack    := not (tr.read    xor tr.read_ack(nsync-1));

    --crc generation
    if (tr.crc_en = '1') and ((rmii = 0) or (tr.rmii_crc_en = '1')) then
      v.crc := calccrc(tr.txd, tr.crc);
    end if;

    --rmii
    if rmii = 0 then
      col := tr.col(1); crs := tr.crs(1);
      tx_done := '1';
    else
      v.crs_prev := tr.crs(1);
      if (tr.crs(0) and not tr.crs_act) = '1' then
        v.crs_act := '1';
      end if;
      if (tr.crs(1) or tr.crs(0)) = '0' then
        v.crs_act := '0';
      end if;
      crs := tr.crs(1) and not ((not tr.crs_prev) and tr.crs_act);
      col := crs and tr.tx_en;

      v.speed(1) := tr.speed(0); v.speed(0) := r.ctrl.speed;

      if tr.tx_en = '1' then
        v.rcnt := tr.rcnt - 1;
        if tr.speed(1) = '1' then
          v.switch := not tr.switch;
          if tr.switch = '1' then
            tx_done := '1'; v.rmii_crc_en := '1';
          end if;
          if tr.switch = '0' then
            v.txd(1 downto 0) := tr.txd_msb;
          end if;  
        else
          v.zero := '0';
          if tr.rcnt = "0001" then
            v.zero := '1'; 
          end if;
          if tr.zero = '1' then
            v.switch := not tr.switch;
            v.rcnt := "1001";
            if tr.switch = '0' then
              v.txd(1 downto 0) := tr.txd_msb;
            end if;
          end if;
          if (tr.switch and tr.zero) = '1' then
            tx_done := '1'; v.rmii_crc_en := '1';
          end if;
        end if;
      end if;
    end if;

    collision := col and not tr.fullduplex(1); 
       
    --main fsm
    case tr.main_state is
    when idle =>
      v.transmitting := '0'; 
      if rmii = 1 then
        v.rcnt := "1001"; v.switch := '0';
      end if;
      if (start and not tr.deferring) = '1' then
        v.main_state := preamble; v.transmitting := '1'; v.tx_en := '1';
	v.byte_count := (others => '1'); v.status := (others => '0');
	v.read := not tr.read; v.start(nsync) := tr.start(nsync-1);
      elsif start = '1' then
	frame_waiting := '1'; 
      end if;
      v.txd := "0101"; v.cnt := "1110";
    when preamble =>
      if tx_done = '1' then
        v.cnt := tr.cnt - 1; 
        if tr.cnt = "0000" then
          v.txd := "1101"; v.main_state := sfd; 
        end if;
        if collision = '1' then v.main_state := send_jam; end if;
      end if;
    when sfd =>
      if tx_done = '1' then
        v.main_state := data1; v.icnt := (others => '0'); v.crc_en := '1';
        v.crc := (others => '1'); v.byte_count := (others => '0');
        v.txd := r.txdata(27 downto 24);
        if (read_ack and r.txvalid) = '0' then
          v.status(0) := '1'; v.main_state := finish; v.tx_en := '0';  
        else
          v.data := r.txdata; v.read := not tr.read;
        end if;
        if collision = '1' then v.main_state := send_jam; end if;
      end if;
    when data1 =>
      index := conv_integer(tr.icnt);
      if tx_done = '1' then
        v.byte_count := tr.byte_count + 1;
        v.main_state := data2; v.icnt := tr.icnt + 1;
        case index is
        when 0 => v.txd := tr.data(31 downto 28);
        when 1 => v.txd := tr.data(23 downto 20);
        when 2 => v.txd := tr.data(15 downto 12);
        when 3 => v.txd := tr.data(7 downto 4);
        when others => null;
        end case;
        if v.byte_count = r.txlength then
          v.tx_en := '1';
          if conv_integer(v.byte_count) >= 60 then
            v.main_state := fcs; v.cnt := (others => '0'); 
          else
            v.main_state := pad1; 
          end if;
        elsif index = 3 then
          if (read_ack and r.txvalid) = '0' then
            v.status(0) := '1'; v.main_state := finish; v.tx_en := '0';  
          else
            v.data := r.txdata; v.read := not tr.read;
          end if;
        end if;
        if collision = '1' then v.main_state := send_jam; end if;
      end if;
    when data2 =>
      index := conv_integer(tr.icnt); 
      if tx_done = '1' then
        v.main_state := data1;
        case index is
        when 0 => v.txd := tr.data(27 downto 24);
        when 1 => v.txd := tr.data(19 downto 16);
        when 2 => v.txd := tr.data(11 downto 8);
        when 3 => v.txd := tr.data(3 downto 0);
        when others => null;
        end case;
        if collision = '1' then v.main_state := send_jam; end if;
      end if;
    when pad1 =>
      if tx_done = '1' then
        v.main_state := pad2; 
        if collision = '1' then v.main_state := send_jam; end if;
      end if;
    when pad2 =>
      if tx_done = '1' then
        v.byte_count := tr.byte_count + 1; 
        if conv_integer(v.byte_count) = 60 then
          v.main_state := fcs; v.cnt := (others => '0');
        else
          v.main_state := pad1;
        end if; 
        if collision = '1' then v.main_state := send_jam; end if;
      end if;
    when fcs =>
      if tx_done = '1' then
        v.cnt := tr.cnt + 1; v.crc_en := '0'; index := conv_integer(tr.cnt);
        case index is
        when 0 => v.txd := mirror(not v.crc(31 downto 28));
        when 1 => v.txd := mirror(not tr.crc(27 downto 24));
        when 2 => v.txd := mirror(not tr.crc(23 downto 20));
        when 3 => v.txd := mirror(not tr.crc(19 downto 16));
        when 4 => v.txd := mirror(not tr.crc(15 downto 12));
        when 5 => v.txd := mirror(not tr.crc(11 downto 8));
        when 6 => v.txd := mirror(not tr.crc(7 downto 4));
        when 7 => v.txd := mirror(not tr.crc(3 downto 0));
                  v.main_state := fcs2;
        when others => null;
        end case;
      end if;
    when fcs2 =>
      if tx_done = '1' then
        v.main_state := finish; v.tx_en := '0';
      end if;
    when finish =>
      v.tx_en := '0'; v.transmitting := '0'; v.main_state := idle;
      v.retry_cnt := (others => '0'); v.done := not tr.done;
    when send_jam =>
      if tx_done = '1' then
        v.cnt := "0110"; v.main_state := send_jam2; v.crc_en := '0';
      end if;  
    when send_jam2 =>
      if tx_done = '1' then
        v.cnt := tr.cnt - 1;
        if tr.cnt = "0000" then
          v.main_state := check_attempts; v.retry_cnt := tr.retry_cnt + 1;
          v.tx_en := '0';
        end if;
      end if;
    when check_attempts =>
      v.transmitting := '0';
      if tr.retry_cnt = maxattempts then
        v.main_state := finish; v.status(1) := '1';  
      else
        v.main_state := calc_backoff; v.restart := not tr.restart;
      end if;
      v.tx_en := '0';
    when calc_backoff =>
      v.delay_val := (others => '0');
      for i in 1 to backoff_limit-1 loop
	if i < conv_integer(tr.retry_cnt)+1 then
	  v.delay_val(i) := tr.random(i);
	end if; 
      end loop;
      v.main_state := wait_backoff; v.slot_count := (others => '1'); 
    when wait_backoff =>
      if conv_integer(tr.delay_val) = 0 then
	v.main_state := idle; 
      end if;
      v.slot_count := tr.slot_count - 1;
      if conv_integer(tr.slot_count) = 0 then
	v.slot_count := (others => '1'); v.delay_val := tr.delay_val - 1; 
      end if;
    when others =>
      v.main_state := idle;
    end case;
    
    --random values; 
    v.random := tr.random(8 downto 0) & (not (tr.random(2) xor tr.random(9)));
   
    --deference
    case tr.def_state is
    when monitor =>
      v.was_transmitting := '0'; 
      if ( (crs and not tr.fullduplex(1)) or
	   (tr.transmitting and tr.fullduplex(1)) ) = '1' then
	v.deferring := '1'; v.def_state := def_on;
	v.was_transmitting := tr.transmitting; 
      end if;
    when def_on =>
      v.was_transmitting := tr.was_transmitting or tr.transmitting; 
      if tr.fullduplex(1) = '1' then
	if tr.transmitting = '0' then v.def_state := ifg1; end if;
        v.ifg_cycls := ifg_sel(rmii, 1, tr.speed(1));
      else
	if (tr.transmitting or crs) = '0' then
	  v.def_state := ifg1; v.ifg_cycls := ifg_sel(rmii, 1, tr.speed(1));
	end if; 
      end if; 
    when ifg1 =>
      v.ifg_cycls := tr.ifg_cycls - 1;
      if tr.ifg_cycls = zero32(ifg_bits-1 downto 0) then
        v.def_state := ifg2;
        v.ifg_cycls := ifg_sel(rmii, 0, tr.speed(1));
      elsif (crs and not tr.fullduplex(1)) = '1' then
        v.ifg_cycls := ifg_sel(rmii, 1, tr.speed(1));
      end if; 
    when ifg2 =>
      v.ifg_cycls := tr.ifg_cycls - 1;
      if tr.ifg_cycls = zero32(ifg_bits-1 downto 0) then
	v.deferring := '0'; 
	if (tr.fullduplex(1) or not frame_waiting) = '1' then
	  v.def_state := monitor;
        elsif frame_waiting = '1' then
	  v.def_state := frame_waitingst;
	end if; 
      end if;
    when frame_waitingst =>
       if frame_waiting = '0' then v.def_state := monitor; end if; 
    when others => v.def_state := monitor; 
    end case;

    if rmii = 1 then
      v.txd_msb := v.txd(3 downto 2);
    end if;
        
    if txrst = '0' then
      v.main_state := idle; v.random := (others => '0');
      v.def_state := monitor; v.deferring := '0'; v.tx_en := '0'; 
      v.done := '0'; v.restart := '0'; v.read := '0';
      v.start := (others => '0'); v.read_ack := (others => '0');
      if rmii = 1 then
        v.crs_act := '0'; 
      end if;
    end if;

    trin                     <= v;
    etho.tx_er               <= '0';
    etho.tx_en               <= tr.tx_en;
    etho.txd(3 downto 0)     <= tr.txd; 
  end process;

  txregs : process(txclk) is
  begin
    if rising_edge(txclk) then tr <= trin; end if;
  end process;
-------------------------------------------------------------------------------
-- RECEIVER -------------------------------------------------------------------
-------------------------------------------------------------------------------
  rx : process(rxrst, r, rr, ethi, rxenable) is
    variable v         : rx_reg_type;
    variable index     : integer range 0 to 3;
    variable crc_en    : std_ulogic;
    variable write_req : std_ulogic;
    variable write_ack : std_ulogic;
    variable done_ack  : std_ulogic;

    variable er        : std_ulogic;
    variable dv        : std_ulogic;
    variable act       : std_ulogic;
    variable rxd       : std_logic_vector(3 downto 0);
  begin
    v := rr; v.rxd := ethi.rxd(3 downto 0);
    if rmii = 0 then
      v.en := ethi.rx_dv;
    else
      v.en := ethi.rx_crs;
    end if;
    v.er := ethi.rx_er; write_req := '0'; crc_en := '0';
    index := conv_integer(rr.byte_count(1 downto 0));
    
    --synchronization
    v.rxen(1) := rr.rxen(0); v.rxen(0) := rxenable;
   
    v.write_ack(0) := r.rxwriteack;
    v.done_ack(0)  := r.rxdoneack; 
    
    if nsync = 2 then
      v.write_ack(1) := rr.write_ack(0);
      v.done_ack(1)  := rr.done_ack(0);
    end if;

    write_ack := not (rr.write xor rr.write_ack(nsync-1));
    done_ack  := not (rr.done xor rr.done_ack(nsync-1));

    --rmii/mii
    if rmii = 0 then
      er := rr.er; dv := rr.en; act := rr.en; rxd := rr.rxd;
    else
      --sync 
      v.speed(1) := rr.speed(0); v.speed(0) := r.ctrl.speed;
      
      rxd := rr.rxd(1 downto 0) & rr.rxd2;

      if rr.cnt = "0000" then
        v.cnt := "1001";
      else
        v.cnt := rr.cnt - 1;
      end if;

      if v.cnt = "0000" then
        v.zero := '1';
      else
        v.zero := '0';
      end if;
    
      act := rr.act; er := '0';
      
      if rr.speed(1) = '0' then
        if rr.zero = '1' then
          v.enold := rr.en; 
          dv := rr.en and rr.dv;
          v.dv := rr.act and not rr.dv;
          if rr.dv = '0' then
            v.rxd2 := rr.rxd(1 downto 0);
          end if;
          if (rr.enold or rr.en) = '0' then
            v.act := '0';
          end if;
        else
          dv := '0';
        end if;
      else
        v.enold := rr.en; 
        dv := rr.en and rr.dv;
        v.dv := rr.act and not rr.dv;
        v.rxd2 := rr.rxd(1 downto 0);
        if (rr.enold or rr.en) = '0' then
          v.act := '0';
        end if;
      end if;
    end if;

    if (rr.en and not rr.act) = '1' then
      if (rxd = "0101") and (rr.speed(1) or
         (not rr.speed(1) and rr.zero)) = '1' then
        v.act := '1'; v.dv := '0';
      end if;
    end if;
    
    --fsm
    case rr.rx_state is
    when idle =>
      v.gotframe := '0'; v.status := (others => '0');
      v.byte_count := (others => '0'); v.odd_nibble := '0';
      if (dv and rr.rxen(1)) = '1' then
        v.rx_state := wait_sfd;  
      elsif dv = '1' then v.rx_state := discard_packet; end if;
    when discard_packet =>
      if act = '0' then v.rx_state := idle; end if; 
    when wait_sfd =>
      if act = '0' then v.rx_state := idle; 
      elsif (rxd = "1101") and (dv = '1') then
        v.rx_state := data1; v.sync_start := not rr.sync_start;
      end if;
      v.start := '0'; v.crc := (others => '1');
      if er = '1' then v.status(2) := '1'; end if;
    when data1 =>
      if (act and dv) = '1' then 
        crc_en := '1';
        v.odd_nibble := not rr.odd_nibble; v.rx_state := data2;
        case index is
        when 0 => v.data(27 downto 24) := rxd;
        when 1 => v.data(19 downto 16) := rxd;
        when 2 => v.data(11 downto 8)  := rxd;
        when 3 => v.data(3 downto 0)   := rxd; 
        end case;
      elsif act = '0' then
        v.rx_state := check_crc;
      end if;
      if (rr.byte_count(1 downto 0) = "00" and (rr.start and act and dv) = '1') then
        write_req := '1';
      end if;
      if er = '1' then v.status(2) := '1'; end if;
      if conv_integer(rr.byte_count) > maxsize then
        v.rx_state := errorst; v.status(1) := '1';
        v.byte_count := rr.byte_count - 4;
      end if;
    when data2 =>
      if (act and dv) = '1' then
        crc_en := '1';
        v.odd_nibble := not rr.odd_nibble; v.rx_state := data1;
        v.byte_count := rr.byte_count + 1; v.start := '1';
        case index is
        when 0 => v.data(31 downto 28) := rxd;
        when 1 => v.data(23 downto 20) := rxd;
        when 2 => v.data(15 downto 12) := rxd;
        when 3 => v.data(7 downto 4)   := rxd;
        end case;
      elsif act = '0' then
        v.rx_state := check_crc;
      end if;
      if er = '1' then v.status(2) := '1'; end if;
    when check_crc =>
      if rr.crc /= X"C704DD7B" then
        if rr.odd_nibble = '1' then v.status(0) := '1';
        else v.status(2) := '1'; end if;
      end if;
      if write_ack = '1' then
        v.rx_state := report_status; v.byte_count := rr.byte_count - 4;
        if conv_integer(rr.byte_count) < minsize then
          v.rx_state := wait_report; v.done := not rr.done;
        end if;
      end if; 
    when errorst =>
      if act = '0' then
        v.rx_state := wait_report; v.done := not rr.done;
        v.gotframe := '1';
      end if;
    when report_status =>
      v.done := not rr.done; v.rx_state := wait_report;
      v.gotframe := '1';
    when wait_report =>
      if done_ack = '1' then
        if act = '1' then
          v.rx_state := discard_packet;
        else
          v.rx_state := idle;
        end if;
      end if;    
    when others => null;
    end case;

    --write to fifo
    if write_req = '1' then
      if (rr.status(3) or not write_ack) = '1' then
        v.status(3) := '1';
      else
        v.dataout := rr.data; v.write := not rr.write; 
      end if;
    end if;

    if r.rxwriteack = '1' then 
      if r.writeok = '0' then v.status(3) := '1'; end if;
    end if;

    --crc generation
     if crc_en = '1' then
       v.crc := calccrc(rxd, rr.crc); 
     end if;
    
    if rxrst = '0' then
      v.rx_state  := idle; v.write := '0'; v.done := '0'; v.sync_start := '0';
      v.done_ack  := (others => '0'); 
      v.gotframe  := '0'; v.write_ack := (others => '0');
      if rmii = 1 then
        v.dv := '0'; v.cnt := (others => '0'); v.zero := '0';
      end if;
    end if;

    rrin  <= v;
  end process;

  rxregs : process(rxclk) is
  begin
    if rising_edge(rxclk) then rr <= rrin; end if;
  end process;
  
-------------------------------------------------------------------------------
-- AHB MST INTERFACE ----------------------------------------------------------
-------------------------------------------------------------------------------
  ahb0 : eth_ahb_mst generic map (hindex => hindex, revision => revision,
    irq => pirq) 
    port map(rst, clk, ahbmi, ahbmo, tmsto, tmsti, rmsto, rmsti);
-------------------------------------------------------------------------------
-- FIFOS ----------------------------------------------------------------------
-------------------------------------------------------------------------------
  tx_fifo0 : syncram_2p generic map(tech => memtech, abits => txfabits,
    dbits => 32, sepclk => 0)
    port map(clk, txfi.renable, txfi.raddress, txfo.data, clk,
    txfi.write, txfi.waddress, txfi.datain);

  rx_fifo0 : syncram_2p generic map(tech => memtech, abits => fabits,
    dbits => 32, sepclk => 0)
    port map(clk, rxfi.renable, rxfi.raddress, rxfo.data, clk,
    rxfi.write, rxfi.waddress, rxfi.datain);

-------------------------------------------------------------------------------
-- EDCL buffer ram ------------------------------------------------------------
-------------------------------------------------------------------------------
  edclram : if (edcl = 1) generate
    rloopm : for i in 0 to 1 generate
      r0 : syncram_2p generic map (memtech, eabits, 8) port map (
	clk, eri.renable, eri.raddress, ero.data(i*8+23 downto i*8+16), clk,
        eri.writem, eri.waddressm, eri.datain(i*8+23 downto i*8+16)); 
    end generate;
    rloopl : for i in 0 to 1 generate
      r0 : syncram_2p generic map (memtech, eabits, 8) port map (
	clk, eri.renable, eri.raddress, ero.data(i*8+7 downto i*8), clk,
        eri.writel, eri.waddressl, eri.datain(i*8+7 downto i*8)); 
    end generate;
  end generate;
  
-- pragma translate_off
  bootmsg : report_version 
  generic map (
    "greth" & tost(hindex) & ": 10/100 Mbit Ethernet MAC rev " & tost(REVISION)
    & tost(hindex) & ", EDCL " & tost(edcl) & ", buffer " & 
		tost(edclbufsz*edcl) & " kbyte " & tost(txfifosize) & " txfifo" 
  );
-- pragma translate_on

end architecture;
