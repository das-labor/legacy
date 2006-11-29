library ieee;
use ieee.std_logic_1164.all;


package miscellaneous is

  component ClockGenerator
    port (
      Clk     : in  std_ulogic;
      Reset   : in  std_ulogic;
      oMCLK   : out std_ulogic;
      oBCLK   : out std_ulogic;
      oSCLK   : out std_ulogic;
      oLRCOUT : out std_ulogic);
  end component;

end miscellaneous;
