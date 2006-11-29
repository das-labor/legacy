library ieee;
use ieee.std_logic_1164.all;
library grlib;
use grlib.amba.all;

package dac is

  component dac_ahb
    generic (
      length : integer;
      hindex : integer;
      haddr  : integer;
      hmask  : integer;
      tech   : integer;
      kbytes : integer);
    port (
      rst     : in  std_ulogic;
      clk     : in  std_ulogic;
      ahbsi   : in  ahb_slv_in_type;
      ahbso   : out ahb_slv_out_type;
      dac_out : out std_ulogic);
  end component;

  component sigdelt
    generic (
      c_dacin_length : positive);
    port (
      reset   : in  std_logic;
      clock   : in  std_logic;
      dac_in  : in  std_logic_vector(c_dacin_length-1 downto 0);
      dac_out : out std_logic);
  end component;

end dac;
