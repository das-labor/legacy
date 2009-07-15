-------------------------------------------------------------------------------
-- Title      : BMP Package
-- Project    : 
-------------------------------------------------------------------------------
-- File       : sim_bmppack.vhd
-- Author     : Kest
-- Company    : 
-- Created    : 2006-12-05
-- Last update: 2007-10-29
-- Platform   : ModelSIM 6.0
-- Standard   : VHDL'93
-------------------------------------------------------------------------------
-- Description: 
-------------------------------------------------------------------------------
-- Copyright (c) 2006 by Kest
-------------------------------------------------------------------------------
-- Revisions  :
-- Date        Version  Author          Description
-- 2006-12-05  1.0      kest            Created
-------------------------------------------------------------------------------


library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;


-------------------------------------------------------------------------------
package sim_bmppack is

  -- maximale Größe des Speichers
  constant cMAX_X         : integer := 1300;
  constant cMAX_Y         : integer := 1300;
  constant cBytesPerPixel : integer := 3;

  constant cMaxMemSize : integer := cMAX_X * cMAX_Y * cBytesPerPixel;

  subtype file_element is std_logic_vector(7 downto 0);
  type    mem_array is array(cMaxMemSize downto 0) of file_element;
  type    header_array is array(53 downto 0) of file_element;


  procedure ReadFile(FileName  : in string);
  procedure WriteFile(FileName : in string);

  procedure ReadByteFromMemory (adr : in integer; variable data : out std_logic_vector(7 downto 0));
  procedure WriteByteToMemory (adr  : in integer; variable data : in std_logic_vector(7 downto 0));

  function GetWidth(header        : in  header_array) return integer;
  procedure GetWidth(signal width : out integer);

  function GetHeigth(header         : in  header_array) return integer;
  procedure GetHeigth(signal height : out integer);

  procedure GetPixel (x : in integer; y : in integer; signal data : out std_logic_vector(23 downto 0));
  procedure SetPixel (x : in integer; y : in integer; signal data : in std_logic_vector(23 downto 0));
  
end package sim_bmppack;


-------------------------------------------------------------------------------
-- Package body 
-------------------------------------------------------------------------------
package body sim_bmppack is


  -----------------------------------------------------------------------------
  -- 
  -----------------------------------------------------------------------------

  shared variable memory_in  : mem_array;
  shared variable memory_out : mem_array;

  shared variable header : header_array;

  shared variable pImageSize   : integer;
  shared variable pImageWidth  : integer;
  shared variable pImageHeight : integer;



  -----------------------------------------------------------------------------
  -- This code reads a raw binary file one byte at a time.
  -----------------------------------------------------------------------------
  procedure ReadFile(FileName : in string) is

    variable next_vector : bit_vector (0 downto 0);
    variable actual_len  : natural;
    variable index       : integer := 0;
    type     bit_vector_file is file of bit_vector;
    file read_file       : bit_vector_file open read_mode is FileName;

  begin
    report "                                        Read File" ;
    

    index := 0;

    ---------------------------------------------------------------------------
    -- Header einlesen
    ---------------------------------------------------------------------------

--    report "Read Header";
    for i in 0 to 53 loop
      read(read_file, next_vector, actual_len);
      if actual_len > next_vector'length then
        report "vector too long";
      else
        header(index) := conv_std_logic_vector(bit'pos(next_vector(0)), 8);
        index         := index + 1;
      end if;
    end loop;

    pImageWidth  := GetWidth(header);
    pImageHeight := GetHeigth(header);
    pImageSize   := pImageWidth * pImageHeight;

--    report "Read Image";
    index := 0;
    while not endfile(read_file) loop
      read(read_file, next_vector, actual_len);
      if actual_len > next_vector'length then
        report "vector too long";
      else
        memory_in(index)  := conv_std_logic_vector(bit'pos(next_vector(0)), 8);
        memory_out(index) := x"45";
        index             := index + 1;
      end if;
    end loop;

--    report "Okay";
  end ReadFile;

  -----------------------------------------------------------------------------
  -- Read one byte from Memory
  -----------------------------------------------------------------------------
  procedure ReadByteFromMemory (adr : in integer; variable data : out std_logic_vector(7 downto 0)) is
  begin
    data := memory_in(adr);
  end ReadByteFromMemory;


  -----------------------------------------------------------------------------
  -- Pixel Operationen
  -----------------------------------------------------------------------------

  procedure GetPixel (x : in integer; y : in integer; signal data : out std_logic_vector(23 downto 0)) is
  begin
    if x >= 0 and x < cMAX_X and y >= 0 and y < cMAX_Y then
      data(23 downto 16) <= memory_in(x*3 + 3*y*GetWidth(header));
      data(15 downto 8)  <= memory_in(x*3+1 + 3*y*GetWidth(header));
      data(7 downto 0)   <= memory_in(x*3+2 + 3*y*GetWidth(header));
    end if;
  end GetPixel;

  procedure SetPixel (x : in integer; y : in integer; signal data : in std_logic_vector(23 downto 0)) is
  begin
    if x >= 0 and x < cMAX_X and y >= 0 and y < cMAX_Y then
      memory_out(x*3+y*(GetWidth(header)*3))   := data(23 downto 16);
      memory_out(x*3+1+y*(GetWidth(header)*3)) := data(15 downto 8);
      memory_out(x*3+2+y*(GetWidth(header)*3)) := data(7 downto 0);
    end if;
  end SetPixel;



  -----------------------------------------------------------------------------
  -- Write one byte to Memory
  -----------------------------------------------------------------------------
  procedure WriteByteToMemory (adr : in integer; variable data : in std_logic_vector(7 downto 0)) is
  begin
    memory_out(adr) := data;
  end WriteByteToMemory;

  -- Get Width of Image
  function GetWidth(header : in header_array) return integer is
  begin
    return conv_integer(header(21) & header(20) & header(19) & header(18));
  end function GetWidth;

  procedure GetWidth(signal width : out integer) is
  begin
    width <= pImageWidth;
  end GetWidth;

  -- Get Height of Image
  function GetHeigth(header : in header_array) return integer is
  begin
    return conv_integer(header(25) & header(24) & header(23) & header(22));
  end function GetHeigth;

  procedure GetHeigth(signal height : out integer) is
  begin
    height <= pImageHeight;
  end GetHeigth;

  -----------------------------------------------------------------------------
  -- This code write a raw binary file one byte at a time.
  -----------------------------------------------------------------------------
  procedure WriteFile(FileName : in string) is

    variable next_vector : character;
    variable index       : integer := 0;
    type     char_file is file of character;
    file write_file      : char_file open write_mode is FileName;

  begin
    report "                                        Write File...";
    report FileName;

--    report "write Header";
    index := 0;
    for i in 0 to 53 loop
      next_vector := character'val(conv_integer(header(index)));
      write(write_file, next_vector);
      index       := index + 1;
    end loop;


--    report "write Image";
    index := 0;
    while index < pImageSize*3 loop
      next_vector := character'val(conv_integer(memory_out(index)));
      write(write_file, next_vector);
      index       := index + 1;
    end loop;

--    report "Okay";

  end WriteFile;

  
end sim_bmppack;
