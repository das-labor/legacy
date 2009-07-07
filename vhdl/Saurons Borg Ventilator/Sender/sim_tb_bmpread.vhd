-------------------------------------------------------------------------------
-- Title      : Testbench BMP Read
-- Project    : 
-------------------------------------------------------------------------------
-- File       : sim_tb_bmpread.vhd
-- Author     : Kest
-- Company    : 
-- Created    : 2006-12-05
-- Last update: 2007-12-26
-- Platform   : ModelSIM
-- Standard   : VHDL'93
-------------------------------------------------------------------------------
-- Description: 
-------------------------------------------------------------------------------
-- Copyright (c) 2006 
-------------------------------------------------------------------------------
-- Revisions  :
-- Date        Version  Author          Description
-- 2006-12-05  1.0      kest            Created
-------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.all;

use work.sim_bmppack.all;

entity sim_tb_bmpread is
end sim_tb_bmpread;

-------------------------------------------------------------------------------
architecture testbench of sim_tb_bmpread is

  
  signal clk                     : std_logic                     := '0';
  signal data ,data2                   : std_logic_vector(23 downto 0) := x"ffffff";
  signal ImageWidth, ImageHeight : integer                       := 0;
  signal xd,yd : integer;
  
begin

  process(clk) is
  begin
    clk <= not clk after 5 ns;
  end process;


  -----------------------------------------------------------------------------
  --
  -----------------------------------------------------------------------------
  process is

  begin
    ReadFile("lena.bmp");


    GetWidth(ImageWidth);
    GetHeigth(ImageHeight);

    wait until clk'event and clk = '1';

    for y in 0 to ImageHeight-1 loop
      for x in 0 to ImageWidth-1 loop
        xd <= x; yd <= y;
        GetPixel(x, y, data);
        wait until clk = '1' and clk'event;
		  data2 <= data ( 23 downto 21)&"00000" & data (15 downto 12)&"0000" & data (7 downto 5)&"00000" ;
        SetPixel(x, y, data2);
        wait until clk = '1' and clk'event;

      end loop;
    end loop;

    data <= (others => '1');
    
     
    wait until clk'event and clk = '1';

    report "Jetzt ist das Bild fertig...";

    WriteFile("lena_copy.bmp");

    wait until clk'event and clk = '1';

    wait;
  end process;
  
  
end testbench;

