VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "spartan3"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL XLXN_1(7:0)
        SIGNAL XLXN_2(7:0)
        SIGNAL XLXN_3(7:0)
        SIGNAL XLXN_4(7:0)
        SIGNAL clk20
        SIGNAL XLXN_14
        SIGNAL XLXN_15
        SIGNAL XLXN_16
        SIGNAL XLXN_17
        SIGNAL XLXN_18
        SIGNAL XLXN_19
        SIGNAL XLXN_20
        SIGNAL XLXN_21
        SIGNAL q1
        SIGNAL q2
        SIGNAL q3
        SIGNAL q4
        SIGNAL pattern(7:0)
        SIGNAL XLXN_29
        SIGNAL XLXN_30
        SIGNAL XLXN_31
        SIGNAL XLXN_32
        SIGNAL mux(7:0)
        SIGNAL trig
        SIGNAL I1(7:0)
        SIGNAL I2(7:0)
        SIGNAL I3(7:0)
        SIGNAL i4(7:0)
        SIGNAL XLXN_55
        SIGNAL XLXN_56
        SIGNAL XLXN_57
        SIGNAL XLXN_58
        PORT Input clk20
        PORT Output q1
        PORT Output q2
        PORT Output q3
        PORT Output q4
        PORT Input pattern(7:0)
        PORT Input mux(7:0)
        PORT Output trig
        PORT Input I1(7:0)
        PORT Input I2(7:0)
        PORT Input I3(7:0)
        PORT Input i4(7:0)
        BEGIN BLOCKDEF tgb
            TIMESTAMP 2009 5 19 10 30 2
            LINE N 64 -288 0 -288 
            RECTANGLE N 64 -320 272 28 
            RECTANGLE N 0 -252 64 -228 
            LINE N 64 -240 0 -240 
            RECTANGLE N 0 -220 64 -196 
            LINE N 64 -208 0 -208 
            RECTANGLE N 0 -188 64 -164 
            LINE N 64 -176 0 -176 
            RECTANGLE N 0 -156 64 -132 
            LINE N 64 -144 0 -144 
            LINE N 64 -96 0 -96 
            LINE N 64 -64 0 -64 
            LINE N 64 -32 0 -32 
            LINE N 64 0 0 0 
            RECTANGLE N 272 -300 336 -276 
            LINE N 272 -288 336 -288 
            RECTANGLE N 272 -220 336 -196 
            LINE N 272 -208 336 -208 
            RECTANGLE N 272 -140 336 -116 
            LINE N 272 -128 336 -128 
            RECTANGLE N 272 -60 336 -36 
            LINE N 272 -48 336 -48 
        END BLOCKDEF
        BEGIN BLOCKDEF packet_write
            TIMESTAMP 2009 5 19 10 30 2
            RECTANGLE N 64 -176 272 -48 
            LINE N 64 -144 0 -144 
            RECTANGLE N 0 -124 64 -100 
            LINE N 64 -112 0 -112 
            LINE N 272 -144 336 -144 
            LINE N 272 -112 336 -112 
            RECTANGLE N 272 -92 336 -68 
            LINE N 272 -80 336 -80 
        END BLOCKDEF
        BEGIN BLOCKDEF testpattern
            TIMESTAMP 2009 5 19 10 30 2
            LINE N 64 -224 0 -224 
            RECTANGLE N 64 -256 256 -20 
            RECTANGLE N 0 -188 64 -164 
            LINE N 64 -176 0 -176 
            LINE N 256 -240 320 -240 
            LINE N 256 -208 320 -208 
            LINE N 256 -176 320 -176 
            LINE N 256 -144 320 -144 
            LINE N 256 -80 320 -80 
            RECTANGLE N 256 -60 320 -36 
            LINE N 256 -48 320 -48 
        END BLOCKDEF
        BEGIN BLOCKDEF m2_1
            TIMESTAMP 2000 1 1 10 10 10
            LINE N 96 -64 96 -192 
            LINE N 256 -96 96 -64 
            LINE N 256 -160 256 -96 
            LINE N 96 -192 256 -160 
            LINE N 176 -32 96 -32 
            LINE N 176 -80 176 -32 
            LINE N 0 -32 96 -32 
            LINE N 320 -128 256 -128 
            LINE N 0 -96 96 -96 
            LINE N 0 -160 96 -160 
        END BLOCKDEF
        BEGIN BLOCKDEF b84
            TIMESTAMP 2009 5 19 10 30 2
            RECTANGLE N 64 -256 208 -124 
            LINE N 208 -240 272 -240 
            RECTANGLE N 0 -252 64 -228 
            LINE N 64 -240 0 -240 
            LINE N 208 -208 272 -208 
            LINE N 208 -176 272 -176 
            LINE N 208 -144 272 -144 
        END BLOCKDEF
        BEGIN BLOCK XLXI_1 tgb
            PIN clk clk20
            PIN a(7:0) I1(7:0)
            PIN b(7:0) I2(7:0)
            PIN c(7:0) I3(7:0)
            PIN d(7:0) i4(7:0)
            PIN en1 XLXN_55
            PIN en2 XLXN_56
            PIN en3 XLXN_57
            PIN en4 XLXN_58
            PIN q1(7:0) XLXN_1(7:0)
            PIN q2(7:0) XLXN_2(7:0)
            PIN q3(7:0) XLXN_3(7:0)
            PIN q4(7:0) XLXN_4(7:0)
        END BLOCK
        BEGIN BLOCK XLXI_2 packet_write
            PIN clk clk20
            PIN b8_code(7:0) XLXN_1(7:0)
            PIN q1 XLXN_14
            PIN rdy XLXN_55
            PIN counter(9:0)
        END BLOCK
        BEGIN BLOCK XLXI_3 packet_write
            PIN clk clk20
            PIN b8_code(7:0) XLXN_2(7:0)
            PIN q1 XLXN_15
            PIN rdy XLXN_56
            PIN counter(9:0)
        END BLOCK
        BEGIN BLOCK XLXI_4 packet_write
            PIN clk clk20
            PIN b8_code(7:0) XLXN_3(7:0)
            PIN q1 XLXN_16
            PIN rdy XLXN_57
            PIN counter(9:0)
        END BLOCK
        BEGIN BLOCK XLXI_5 packet_write
            PIN clk clk20
            PIN b8_code(7:0) XLXN_4(7:0)
            PIN q1 XLXN_17
            PIN rdy XLXN_58
            PIN counter(9:0)
        END BLOCK
        BEGIN BLOCK XLXI_11 testpattern
            PIN clk20 clk20
            PIN Pattern(7:0) pattern(7:0)
            PIN q1 XLXN_18
            PIN q2 XLXN_19
            PIN q3 XLXN_20
            PIN q4 XLXN_21
            PIN trig trig
            PIN cnt(1:0)
        END BLOCK
        BEGIN BLOCK XLXI_12 m2_1
            PIN D0 XLXN_14
            PIN D1 XLXN_18
            PIN S0 XLXN_29
            PIN O q1
        END BLOCK
        BEGIN BLOCK XLXI_13 m2_1
            PIN D0 XLXN_15
            PIN D1 XLXN_19
            PIN S0 XLXN_30
            PIN O q2
        END BLOCK
        BEGIN BLOCK XLXI_14 m2_1
            PIN D0 XLXN_16
            PIN D1 XLXN_20
            PIN S0 XLXN_31
            PIN O q3
        END BLOCK
        BEGIN BLOCK XLXI_15 m2_1
            PIN D0 XLXN_17
            PIN D1 XLXN_21
            PIN S0 XLXN_32
            PIN O q4
        END BLOCK
        BEGIN BLOCK XLXI_16 b84
            PIN q1 XLXN_29
            PIN a(7:0) mux(7:0)
            PIN q2 XLXN_30
            PIN q3 XLXN_31
            PIN q4 XLXN_32
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        BEGIN INSTANCE XLXI_2 1632 720 R0
        END INSTANCE
        BEGIN INSTANCE XLXI_3 1632 944 R0
        END INSTANCE
        BEGIN INSTANCE XLXI_4 1632 1152 R0
        END INSTANCE
        BEGIN INSTANCE XLXI_5 1632 1392 R0
        END INSTANCE
        BEGIN BRANCH XLXN_1(7:0)
            WIRE 1408 800 1520 800
            WIRE 1520 608 1520 800
            WIRE 1520 608 1632 608
        END BRANCH
        BEGIN BRANCH XLXN_2(7:0)
            WIRE 1408 880 1520 880
            WIRE 1520 832 1520 880
            WIRE 1520 832 1632 832
        END BRANCH
        BEGIN BRANCH XLXN_3(7:0)
            WIRE 1408 960 1520 960
            WIRE 1520 960 1520 1040
            WIRE 1520 1040 1632 1040
        END BRANCH
        BEGIN BRANCH XLXN_4(7:0)
            WIRE 1408 1040 1504 1040
            WIRE 1504 1040 1504 1280
            WIRE 1504 1280 1632 1280
        END BRANCH
        BEGIN INSTANCE XLXI_11 1584 1888 R0
        END INSTANCE
        INSTANCE XLXI_14 2592 1696 R0
        INSTANCE XLXI_15 2592 1936 R0
        INSTANCE XLXI_12 2592 1136 R0
        INSTANCE XLXI_13 2608 1424 R0
        BEGIN BRANCH XLXN_14
            WIRE 1968 576 2432 576
            WIRE 2432 576 2432 976
            WIRE 2432 976 2592 976
        END BRANCH
        BEGIN BRANCH XLXN_15
            WIRE 1968 800 2400 800
            WIRE 2400 800 2400 1264
            WIRE 2400 1264 2608 1264
        END BRANCH
        BEGIN BRANCH XLXN_16
            WIRE 1968 1008 2368 1008
            WIRE 2368 1008 2368 1536
            WIRE 2368 1536 2592 1536
        END BRANCH
        BEGIN BRANCH XLXN_17
            WIRE 1968 1248 2336 1248
            WIRE 2336 1248 2336 1776
            WIRE 2336 1776 2592 1776
        END BRANCH
        BEGIN BRANCH XLXN_18
            WIRE 1904 1648 2160 1648
            WIRE 2160 1040 2160 1648
            WIRE 2160 1040 2592 1040
        END BRANCH
        BEGIN BRANCH XLXN_19
            WIRE 1904 1680 2192 1680
            WIRE 2192 1328 2192 1680
            WIRE 2192 1328 2608 1328
        END BRANCH
        BEGIN BRANCH XLXN_20
            WIRE 1904 1712 2224 1712
            WIRE 2224 1600 2224 1712
            WIRE 2224 1600 2592 1600
        END BRANCH
        BEGIN BRANCH XLXN_21
            WIRE 1904 1744 2240 1744
            WIRE 2240 1744 2240 1840
            WIRE 2240 1840 2592 1840
        END BRANCH
        BEGIN BRANCH q1
            WIRE 2912 1008 2944 1008
        END BRANCH
        IOMARKER 2944 1008 q1 R0 28
        BEGIN BRANCH q2
            WIRE 2928 1296 2960 1296
        END BRANCH
        IOMARKER 2960 1296 q2 R0 28
        BEGIN BRANCH q3
            WIRE 2912 1568 2944 1568
        END BRANCH
        IOMARKER 2944 1568 q3 R0 28
        BEGIN BRANCH q4
            WIRE 2912 1808 2944 1808
        END BRANCH
        IOMARKER 2944 1808 q4 R0 28
        BEGIN BRANCH pattern(7:0)
            WIRE 1552 1712 1584 1712
        END BRANCH
        IOMARKER 1552 1712 pattern(7:0) R180 28
        IOMARKER 416 1104 clk20 R180 28
        BEGIN BRANCH XLXN_31
            WIRE 2320 2096 2576 2096
            WIRE 2576 1664 2592 1664
            WIRE 2576 1664 2576 2096
        END BRANCH
        BEGIN BRANCH XLXN_32
            WIRE 2320 2128 2592 2128
            WIRE 2592 1904 2592 2128
        END BRANCH
        BEGIN INSTANCE XLXI_16 2048 2272 R0
        END INSTANCE
        BEGIN BRANCH XLXN_29
            WIRE 2320 2032 2544 2032
            WIRE 2544 1104 2592 1104
            WIRE 2544 1104 2544 2032
        END BRANCH
        BEGIN BRANCH XLXN_30
            WIRE 2320 2064 2560 2064
            WIRE 2560 1392 2608 1392
            WIRE 2560 1392 2560 2064
        END BRANCH
        BEGIN BRANCH mux(7:0)
            WIRE 2016 2032 2048 2032
        END BRANCH
        IOMARKER 2016 2032 mux(7:0) R180 28
        BEGIN BRANCH trig
            WIRE 1904 1808 1936 1808
        END BRANCH
        IOMARKER 1936 1808 trig R0 28
        BEGIN BRANCH clk20
            WIRE 416 1104 544 1104
            WIRE 544 1104 544 1664
            WIRE 544 1664 1584 1664
            WIRE 544 576 544 1104
            WIRE 544 576 848 576
            WIRE 848 576 1584 576
            WIRE 1584 576 1632 576
            WIRE 1584 576 1584 800
            WIRE 1584 800 1632 800
            WIRE 1584 800 1584 1008
            WIRE 1584 1008 1632 1008
            WIRE 1584 1008 1584 1248
            WIRE 1584 1248 1632 1248
            WIRE 848 576 848 800
            WIRE 848 800 1072 800
        END BRANCH
        BEGIN INSTANCE XLXI_1 1072 1088 R0
        END INSTANCE
        BEGIN BRANCH I1(7:0)
            WIRE 1040 848 1072 848
        END BRANCH
        IOMARKER 1040 848 I1(7:0) R180 28
        BEGIN BRANCH I2(7:0)
            WIRE 1040 880 1072 880
        END BRANCH
        IOMARKER 1040 880 I2(7:0) R180 28
        BEGIN BRANCH I3(7:0)
            WIRE 1040 912 1072 912
        END BRANCH
        IOMARKER 1040 912 I3(7:0) R180 28
        BEGIN BRANCH i4(7:0)
            WIRE 1040 944 1072 944
        END BRANCH
        IOMARKER 1040 944 i4(7:0) R180 28
        BEGIN BRANCH XLXN_55
            WIRE 992 992 1072 992
            WIRE 992 992 992 1424
            WIRE 992 1424 2064 1424
            WIRE 1968 608 2064 608
            WIRE 2064 608 2064 1424
        END BRANCH
        BEGIN BRANCH XLXN_56
            WIRE 1008 1024 1072 1024
            WIRE 1008 1024 1008 1408
            WIRE 1008 1408 2048 1408
            WIRE 1968 832 2048 832
            WIRE 2048 832 2048 1408
        END BRANCH
        BEGIN BRANCH XLXN_57
            WIRE 1024 1056 1072 1056
            WIRE 1024 1056 1024 1392
            WIRE 1024 1392 2032 1392
            WIRE 1968 1040 2032 1040
            WIRE 2032 1040 2032 1392
        END BRANCH
        BEGIN BRANCH XLXN_58
            WIRE 1040 1088 1072 1088
            WIRE 1040 1088 1040 1376
            WIRE 1040 1376 2016 1376
            WIRE 1968 1280 2016 1280
            WIRE 2016 1280 2016 1376
        END BRANCH
    END SHEET
    BEGIN SHEET 2 3520 2720
    END SHEET
END SCHEMATIC
