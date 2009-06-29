VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "spartan3"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL read_in(7:0)
        SIGNAL clk20
        SIGNAL XLXN_12
        SIGNAL read_rdy
        SIGNAL read_crc_ok
        SIGNAL read_crc_er
        SIGNAL read_out(7:0)
        SIGNAL counter(9:0)
        SIGNAL read_synchron
        SIGNAL read_sync_ok
        SIGNAL read_sync_err
        SIGNAL diag_crccount(7:0)
        SIGNAL XLXN_14
        SIGNAL clk200
        SIGNAL clk
        SIGNAL schieb(2:0)
        SIGNAL q1
        SIGNAL q2
        SIGNAL q3
        SIGNAL pattern_in(7:0)
        SIGNAL tp_cnt(1:0)
        SIGNAL daten_out
        SIGNAL ds_cnt(7:0)
        SIGNAL pattern_out(7:0)
        SIGNAL q4
        SIGNAL trig
        SIGNAL XLXN_58
        SIGNAL XLXN_59(7:0)
        PORT Output read_in(7:0)
        PORT Output read_rdy
        PORT Output read_crc_ok
        PORT Output read_crc_er
        PORT Output read_out(7:0)
        PORT Output counter(9:0)
        PORT Output read_synchron
        PORT Output read_sync_ok
        PORT Output read_sync_err
        PORT Output diag_crccount(7:0)
        PORT Input clk
        PORT Input schieb(2:0)
        PORT Output q1
        PORT Output q2
        PORT Output q3
        PORT Input pattern_in(7:0)
        PORT Output tp_cnt(1:0)
        PORT Output daten_out
        PORT Output ds_cnt(7:0)
        PORT Output pattern_out(7:0)
        PORT Output q4
        PORT Output trig
        BEGIN BLOCKDEF cb8ce
            TIMESTAMP 2000 1 1 10 10 10
            LINE N 384 -128 320 -128 
            RECTANGLE N 320 -268 384 -244 
            LINE N 384 -256 320 -256 
            LINE N 0 -192 64 -192 
            LINE N 192 -32 64 -32 
            LINE N 192 -64 192 -32 
            LINE N 80 -128 64 -144 
            LINE N 64 -112 80 -128 
            LINE N 0 -128 64 -128 
            LINE N 0 -32 64 -32 
            LINE N 384 -192 320 -192 
            RECTANGLE N 64 -320 320 -64 
        END BLOCKDEF
        BEGIN BLOCKDEF packet_read
            TIMESTAMP 2009 3 16 8 11 36
            RECTANGLE N 320 68 384 92 
            LINE N 320 80 384 80 
            LINE N 64 -352 0 -352 
            LINE N 64 -192 0 -192 
            RECTANGLE N 320 -364 384 -340 
            LINE N 320 -352 384 -352 
            RECTANGLE N 320 -300 384 -276 
            LINE N 320 -288 384 -288 
            LINE N 320 -224 384 -224 
            LINE N 320 -176 384 -176 
            LINE N 320 -128 384 -128 
            LINE N 320 -80 384 -80 
            LINE N 320 -32 384 -32 
            LINE N 320 16 384 16 
            RECTANGLE N 64 -384 320 112 
        END BLOCKDEF
        BEGIN BLOCKDEF packet_write
            TIMESTAMP 2009 4 10 22 15 3
            RECTANGLE N 64 -176 336 -4 
            LINE N 64 -144 0 -144 
            RECTANGLE N 0 -108 64 -84 
            LINE N 64 -96 0 -96 
            LINE N 336 -144 400 -144 
            LINE N 336 -96 400 -96 
            RECTANGLE N 336 -60 400 -36 
            LINE N 336 -48 400 -48 
        END BLOCKDEF
        BEGIN BLOCKDEF Daten_schieber
            TIMESTAMP 2009 3 30 18 56 50
            RECTANGLE N 320 -124 384 -100 
            LINE N 320 -112 384 -112 
            LINE N 64 -480 0 -480 
            LINE N 64 -432 0 -432 
            LINE N 64 -368 0 -368 
            LINE N 64 -320 0 -320 
            LINE N 64 -272 0 -272 
            LINE N 64 -224 0 -224 
            LINE N 320 -480 384 -480 
            RECTANGLE N 320 -252 384 -228 
            LINE N 320 -240 384 -240 
            RECTANGLE N 64 -512 320 -76 
            RECTANGLE N 0 -172 64 -148 
            LINE N 64 -160 0 -160 
        END BLOCKDEF
        BEGIN BLOCKDEF DCM_200
            TIMESTAMP 2009 3 23 9 12 47
            RECTANGLE N 64 -192 400 0 
            LINE N 64 -160 0 -160 
            LINE N 64 -32 0 -32 
            LINE N 400 -160 464 -160 
            LINE N 400 -32 464 -32 
        END BLOCKDEF
        BEGIN BLOCKDEF f_null
            TIMESTAMP 2009 2 20 3 27 36
            RECTANGLE N 224 -64 320 0 
            LINE N 320 -32 384 -32 
        END BLOCKDEF
        BEGIN BLOCKDEF bufg
            TIMESTAMP 2000 1 1 10 10 10
            LINE N 64 -64 64 0 
            LINE N 128 -32 64 -64 
            LINE N 64 0 128 -32 
            LINE N 224 -32 128 -32 
            LINE N 0 -32 64 -32 
        END BLOCKDEF
        BEGIN BLOCKDEF testpattern
            TIMESTAMP 2009 3 31 12 45 2
            LINE N 256 -48 320 -48 
            LINE N 64 -288 0 -288 
            RECTANGLE N 0 -252 64 -228 
            LINE N 64 -240 0 -240 
            LINE N 256 -288 320 -288 
            LINE N 256 -240 320 -240 
            LINE N 256 -192 320 -192 
            LINE N 256 -144 320 -144 
            RECTANGLE N 256 -108 320 -84 
            LINE N 256 -96 320 -96 
            RECTANGLE N 64 -320 256 -20 
        END BLOCKDEF
        BEGIN BLOCK XLXI_5 cb8ce
            PIN C clk20
            PIN CE XLXN_12
            PIN CLR
            PIN CEO
            PIN Q(7:0) read_in(7:0)
            PIN TC
        END BLOCK
        BEGIN BLOCK XLXI_9 packet_write
            PIN clk clk20
            PIN b8_code(7:0) read_in(7:0)
            PIN q1 XLXN_58
            PIN rdy XLXN_12
            PIN counter(9:0)
        END BLOCK
        BEGIN BLOCK XLXI_8 packet_read
            PIN data_in XLXN_58
            PIN clk clk20
            PIN rdy read_rdy
            PIN synchron read_synchron
            PIN crc_ok read_crc_ok
            PIN crc_er read_crc_er
            PIN sync_ok read_sync_ok
            PIN sync_er read_sync_err
            PIN data_out(7:0) read_out(7:0)
            PIN counter(9:0) counter(9:0)
            PIN diag_crccount(7:0) diag_crccount(7:0)
        END BLOCK
        BEGIN BLOCK XLXI_11 DCM_200
            PIN CLKIN_IN clk20
            PIN RST_IN XLXN_14
            PIN CLKFX_OUT clk200
            PIN CLK0_OUT
        END BLOCK
        BEGIN BLOCK XLXI_12 f_null
            PIN fo XLXN_14
        END BLOCK
        BEGIN BLOCK XLXI_14 bufg
            PIN I clk
            PIN O clk20
        END BLOCK
        BEGIN BLOCK XLXI_15 testpattern
            PIN clk20 clk20
            PIN Pattern(7:0) pattern_in(7:0)
            PIN q1 q1
            PIN q2 q2
            PIN q3 q3
            PIN q4 q4
            PIN cnt(1:0) tp_cnt(1:0)
            PIN trig trig
        END BLOCK
        BEGIN BLOCK XLXI_10 Daten_schieber
            PIN clk20 clk20
            PIN clk200 clk200
            PIN data1_in q1
            PIN data2_in q2
            PIN data3_in q3
            PIN data4_in q4
            PIN data_out1 daten_out
            PIN cnt(7:0) ds_cnt(7:0)
            PIN pattern_o(7:0) pattern_out(7:0)
            PIN switch(2:0) schieb(2:0)
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        INSTANCE XLXI_5 80 992 R0
        IOMARKER 624 1376 read_in(7:0) R0 28
        BEGIN BRANCH XLXN_12
            WIRE 32 512 32 800
            WIRE 32 800 80 800
            WIRE 32 512 1072 512
            WIRE 1072 512 1072 1696
            WIRE 928 1696 1072 1696
        END BRANCH
        BEGIN BRANCH read_in(7:0)
            WIRE 464 736 528 736
            WIRE 528 736 528 1376
            WIRE 528 1376 624 1376
            WIRE 464 1376 528 1376
            WIRE 464 1376 464 1664
            WIRE 464 1664 464 1696
            WIRE 464 1696 528 1696
        END BRANCH
        BEGIN BRANCH read_rdy
            WIRE 2192 992 2224 992
        END BRANCH
        BEGIN BRANCH read_crc_ok
            WIRE 2192 1088 2224 1088
        END BRANCH
        BEGIN BRANCH read_crc_er
            WIRE 2192 1136 2224 1136
        END BRANCH
        BEGIN BRANCH read_out(7:0)
            WIRE 2192 864 2224 864
        END BRANCH
        BEGIN BRANCH counter(9:0)
            WIRE 2192 928 2224 928
        END BRANCH
        BEGIN BRANCH read_synchron
            WIRE 2192 1040 2224 1040
        END BRANCH
        BEGIN BRANCH read_sync_ok
            WIRE 2192 1184 2224 1184
        END BRANCH
        BEGIN BRANCH read_sync_err
            WIRE 2192 1232 2224 1232
        END BRANCH
        BEGIN INSTANCE XLXI_8 1808 1216 R0
        END INSTANCE
        IOMARKER 2224 992 read_rdy R0 28
        IOMARKER 2224 1088 read_crc_ok R0 28
        IOMARKER 2224 1136 read_crc_er R0 28
        IOMARKER 2224 864 read_out(7:0) R0 28
        IOMARKER 2224 928 counter(9:0) R0 28
        IOMARKER 2224 1040 read_synchron R0 28
        IOMARKER 2224 1184 read_sync_ok R0 28
        IOMARKER 2224 1232 read_sync_err R0 28
        BEGIN BRANCH diag_crccount(7:0)
            WIRE 2192 1296 2224 1296
        END BRANCH
        IOMARKER 2224 1296 diag_crccount(7:0) R0 28
        BEGIN INSTANCE XLXI_11 1232 400 R0
        END INSTANCE
        BEGIN BRANCH XLXN_14
            WIRE 1200 240 1232 240
        END BRANCH
        BEGIN INSTANCE XLXI_12 816 272 R0
        END INSTANCE
        INSTANCE XLXI_14 336 400 R0
        BEGIN BRANCH clk
            WIRE 304 368 336 368
        END BRANCH
        IOMARKER 304 368 clk R180 28
        BEGIN BRANCH q1
            WIRE 1696 1584 1808 1584
            WIRE 1808 1584 2304 1584
            WIRE 1808 1584 1808 2016
            WIRE 1808 2016 2064 2016
        END BRANCH
        BEGIN BRANCH q2
            WIRE 1696 1632 1872 1632
            WIRE 1872 1632 2304 1632
            WIRE 1872 1632 1872 1952
            WIRE 1872 1952 2064 1952
        END BRANCH
        BEGIN BRANCH q3
            WIRE 1696 1680 1952 1680
            WIRE 1952 1680 2304 1680
            WIRE 1952 1680 1952 1888
            WIRE 1952 1888 2064 1888
        END BRANCH
        BEGIN INSTANCE XLXI_15 1376 1872 R0
        END INSTANCE
        BEGIN BRANCH pattern_in(7:0)
            WIRE 1344 1632 1376 1632
        END BRANCH
        IOMARKER 1344 1632 pattern_in(7:0) R180 28
        BEGIN BRANCH clk20
            WIRE 64 864 80 864
            WIRE 64 864 64 1152
            WIRE 64 1152 288 1152
            WIRE 288 1152 1120 1152
            WIRE 288 1152 288 1648
            WIRE 288 1648 528 1648
            WIRE 560 368 1120 368
            WIRE 1120 368 1232 368
            WIRE 1120 368 1120 864
            WIRE 1120 864 1120 1152
            WIRE 1120 864 1680 864
            WIRE 1680 864 1808 864
            WIRE 1680 864 1680 1312
            WIRE 1680 1312 1680 1472
            WIRE 1680 1472 2304 1472
            WIRE 1200 1312 1680 1312
            WIRE 1200 1312 1200 1584
            WIRE 1200 1584 1376 1584
        END BRANCH
        BEGIN BRANCH tp_cnt(1:0)
            WIRE 1424 1952 1424 2032
            WIRE 1424 2032 1488 2032
            WIRE 1424 1952 1776 1952
            WIRE 1696 1776 1776 1776
            WIRE 1776 1776 1776 1952
        END BRANCH
        BEGIN BRANCH daten_out
            WIRE 2688 1472 2720 1472
        END BRANCH
        BEGIN BRANCH ds_cnt(7:0)
            WIRE 2688 1712 2720 1712
        END BRANCH
        BEGIN BRANCH pattern_out(7:0)
            WIRE 2688 1840 2720 1840
        END BRANCH
        BEGIN INSTANCE XLXI_10 2304 1952 R0
        END INSTANCE
        IOMARKER 2720 1472 daten_out R0 28
        IOMARKER 2720 1712 ds_cnt(7:0) R0 28
        IOMARKER 2720 1840 pattern_out(7:0) R0 28
        BEGIN BRANCH clk200
            WIRE 1696 240 1744 240
            WIRE 1744 240 1744 1520
            WIRE 1744 1520 2304 1520
        END BRANCH
        BEGIN BRANCH schieb(2:0)
            WIRE 2288 1792 2304 1792
            WIRE 2288 1792 2288 1952
            WIRE 2288 1952 2512 1952
            WIRE 2512 1952 2512 2096
            WIRE 2448 2096 2512 2096
        END BRANCH
        IOMARKER 2448 2096 schieb(2:0) R180 28
        IOMARKER 1488 2032 tp_cnt(1:0) R0 28
        BEGIN BRANCH q4
            WIRE 1696 1728 2032 1728
            WIRE 2032 1728 2304 1728
            WIRE 2032 1728 2032 1808
            WIRE 2032 1808 2064 1808
        END BRANCH
        IOMARKER 2064 1808 q4 R0 28
        IOMARKER 2064 1888 q3 R0 28
        IOMARKER 2064 1952 q2 R0 28
        IOMARKER 2064 2016 q1 R0 28
        BEGIN BRANCH trig
            WIRE 1520 1872 1520 1904
            WIRE 1520 1904 1600 1904
            WIRE 1520 1872 1712 1872
            WIRE 1696 1824 1712 1824
            WIRE 1712 1824 1712 1872
        END BRANCH
        IOMARKER 1600 1904 trig R0 28
        BEGIN BRANCH XLXN_58
            WIRE 928 1648 1024 1648
            WIRE 1024 1024 1808 1024
            WIRE 1024 1024 1024 1648
        END BRANCH
        BEGIN INSTANCE XLXI_9 528 1792 R0
        END INSTANCE
    END SHEET
END SCHEMATIC
