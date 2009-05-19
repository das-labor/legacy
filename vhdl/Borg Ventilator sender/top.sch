VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "spartan3"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL clk
        SIGNAL XLXN_19
        SIGNAL wr_str
        SIGNAL out_p(7:0)
        SIGNAL tmp1
        SIGNAL irq
        SIGNAL i1(7:0)
        SIGNAL i2(7:0)
        SIGNAL i3(7:0)
        SIGNAL schaltin(7:0)
        SIGNAL tast(3:0)
        SIGNAL p_id(7:0)
        SIGNAL led(7:0)
        SIGNAL display(11:0)
        SIGNAL XLXN_87
        SIGNAL XLXN_92
        SIGNAL XLXN_120
        SIGNAL data_roh
        SIGNAL XLXN_64
        SIGNAL data_out
        SIGNAL f_cnt(7:0)
        SIGNAL pattern_o(7:0)
        SIGNAL switch_o(2:0)
        SIGNAL XLXN_155(7:0)
        SIGNAL XLXN_156(7:0)
        SIGNAL XLXN_157(7:0)
        SIGNAL irq_ack
        SIGNAL XLXN_161(7:0)
        SIGNAL XLXN_162(7:0)
        SIGNAL XLXN_163(7:0)
        SIGNAL XLXN_164(7:0)
        SIGNAL XLXN_165(7:0)
        SIGNAL XLXN_166(7:0)
        SIGNAL trig
        SIGNAL XLXN_172
        SIGNAL XLXN_173
        SIGNAL XLXN_175(7:0)
        PORT Input clk
        PORT Input irq
        PORT Input i1(7:0)
        PORT Input i2(7:0)
        PORT Input i3(7:0)
        PORT Input schaltin(7:0)
        PORT Input tast(3:0)
        PORT Output led(7:0)
        PORT Output display(11:0)
        PORT Output data_roh
        PORT Output data_out
        PORT Output f_cnt(7:0)
        PORT Output pattern_o(7:0)
        PORT Output switch_o(2:0)
        PORT Output irq_ack
        PORT Output trig
        BEGIN BLOCKDEF display2
            TIMESTAMP 2009 5 19 10 30 2
            LINE N 64 -160 0 -160 
            RECTANGLE N 64 -192 288 4 
            RECTANGLE N 0 -44 64 -20 
            LINE N 64 -32 0 -32 
            RECTANGLE N 288 -124 352 -100 
            LINE N 288 -112 352 -112 
            RECTANGLE N 0 -76 64 -52 
            LINE N 64 -64 0 -64 
            LINE N 64 -96 0 -96 
        END BLOCKDEF
        BEGIN BLOCKDEF f_null
            TIMESTAMP 2009 5 19 10 30 2
            RECTANGLE N 40 -64 96 0 
            LINE N 96 -32 160 -32 
        END BLOCKDEF
        BEGIN BLOCKDEF takt
            TIMESTAMP 2009 5 19 10 30 2
            LINE N 64 -96 0 -96 
            LINE N 272 -64 336 -64 
            LINE N 64 -32 0 -32 
            LINE N 272 -32 336 -32 
            RECTANGLE N 64 -128 272 0 
            LINE N 272 -96 336 -96 
        END BLOCKDEF
        BEGIN BLOCKDEF Picoblaze
            TIMESTAMP 2009 5 19 10 30 2
            LINE N 64 -416 0 -416 
            LINE N 320 -416 384 -416 
            RECTANGLE N 320 -396 384 -372 
            LINE N 320 -384 384 -384 
            RECTANGLE N 320 -364 384 -340 
            LINE N 320 -352 384 -352 
            RECTANGLE N 320 -284 384 -260 
            LINE N 320 -272 384 -272 
            RECTANGLE N 320 -204 384 -180 
            LINE N 320 -192 384 -192 
            RECTANGLE N 320 -172 384 -148 
            LINE N 320 -160 384 -160 
            RECTANGLE N 320 -140 384 -116 
            LINE N 320 -128 384 -128 
            RECTANGLE N 320 -108 384 -84 
            LINE N 320 -96 384 -96 
            LINE N 64 -32 0 -32 
            RECTANGLE N 0 -204 64 -180 
            LINE N 64 -192 0 -192 
            RECTANGLE N 0 -172 64 -148 
            LINE N 64 -160 0 -160 
            RECTANGLE N 0 -140 64 -116 
            LINE N 64 -128 0 -128 
            RECTANGLE N 0 -108 64 -84 
            LINE N 64 -96 0 -96 
            RECTANGLE N 0 -284 64 -260 
            LINE N 64 -272 0 -272 
            RECTANGLE N 0 -316 64 -292 
            LINE N 64 -304 0 -304 
            RECTANGLE N 64 -448 320 168 
            RECTANGLE N 320 -76 384 -52 
            LINE N 320 -64 384 -64 
            RECTANGLE N 320 -44 384 -20 
            LINE N 320 -32 384 -32 
            RECTANGLE N 320 -12 384 12 
            LINE N 320 0 384 0 
            RECTANGLE N 320 20 384 44 
            LINE N 320 32 384 32 
            RECTANGLE N 320 52 384 76 
            LINE N 320 64 384 64 
            RECTANGLE N 320 84 384 108 
            LINE N 320 96 384 96 
            LINE N 320 144 384 144 
        END BLOCKDEF
        BEGIN BLOCKDEF Daten_schieber
            TIMESTAMP 2009 5 19 10 30 2
            RECTANGLE N 0 20 64 44 
            LINE N 64 32 0 32 
            RECTANGLE N 0 52 64 76 
            LINE N 64 64 0 64 
            RECTANGLE N 320 20 384 44 
            LINE N 320 32 384 32 
            LINE N 64 -256 0 -256 
            LINE N 320 -256 384 -256 
            LINE N 64 -224 0 -224 
            RECTANGLE N 320 -220 384 -196 
            LINE N 320 -208 384 -208 
            LINE N 64 -176 0 -176 
            LINE N 64 -144 0 -144 
            LINE N 64 -112 0 -112 
            LINE N 64 -80 0 -80 
            RECTANGLE N 320 -172 384 -148 
            LINE N 320 -160 384 -160 
            RECTANGLE N 320 -108 384 -84 
            LINE N 320 -96 384 -96 
            BEGIN LINE N 64 -40 320 -40 
                LINECOLOR 0 0 255
            END LINE
            RECTANGLE N 64 -288 320 92 
            RECTANGLE N 0 -12 64 12 
            LINE N 64 0 0 0 
        END BLOCKDEF
        BEGIN BLOCKDEF clk20
            TIMESTAMP 2009 5 19 10 30 2
            RECTANGLE N 64 -128 320 0 
            LINE N 64 -96 0 -96 
            LINE N 320 -96 384 -96 
            LINE N 320 -32 384 -32 
        END BLOCKDEF
        BEGIN BLOCKDEF testdaten
            TIMESTAMP 2009 5 19 10 30 2
            RECTANGLE N 64 -448 288 -124 
            LINE N 64 -416 0 -416 
            RECTANGLE N 0 -396 64 -372 
            LINE N 64 -384 0 -384 
            RECTANGLE N 0 -348 64 -324 
            LINE N 64 -336 0 -336 
            RECTANGLE N 0 -300 64 -276 
            LINE N 64 -288 0 -288 
            RECTANGLE N 0 -252 64 -228 
            LINE N 64 -240 0 -240 
            RECTANGLE N 0 -204 64 -180 
            LINE N 64 -192 0 -192 
            RECTANGLE N 0 -156 64 -132 
            LINE N 64 -144 0 -144 
            LINE N 288 -416 352 -416 
            LINE N 288 -384 352 -384 
            LINE N 288 -352 352 -352 
            LINE N 288 -320 352 -320 
            LINE N 288 -256 352 -256 
        END BLOCKDEF
        BEGIN BLOCK XLXI_6 takt
            PIN RST_IN XLXN_19
            PIN CLKIN_IN clk
            PIN CLKIN_IBUFG_OUT tmp1
            PIN CLK0_OUT XLXN_92
            PIN CLKFX_OUT XLXN_87
        END BLOCK
        BEGIN BLOCK XLXI_8 Picoblaze
            PIN clk XLXN_92
            PIN irq irq
            PIN i0(7:0) XLXN_175(7:0)
            PIN i1(7:0) i1(7:0)
            PIN i2(7:0) i2(7:0)
            PIN i3(7:0) i3(7:0)
            PIN schalter(7:0) schaltin(7:0)
            PIN taster(3:0) tast(3:0)
            PIN wr_str wr_str
            PIN p_id(7:0) p_id(7:0)
            PIN out_p(7:0) out_p(7:0)
            PIN q0(7:0) XLXN_155(7:0)
            PIN q1(7:0) XLXN_156(7:0)
            PIN q2(7:0) XLXN_157(7:0)
            PIN led(7:0) led(7:0)
            PIN q3(7:0) XLXN_161(7:0)
            PIN q4(7:0) XLXN_162(7:0)
            PIN q5(7:0) XLXN_163(7:0)
            PIN q6(7:0) XLXN_164(7:0)
            PIN q7(7:0) XLXN_165(7:0)
            PIN q8(7:0) XLXN_166(7:0)
            PIN q9(7:0)
            PIN irq_ack irq_ack
        END BLOCK
        BEGIN BLOCK XLXI_4 f_null
            PIN fo XLXN_19
        END BLOCK
        BEGIN BLOCK XLXI_1 display2
            PIN clk XLXN_92
            PIN out_p(7:0) out_p(7:0)
            PIN Display(11:0) display(11:0)
            PIN p_id(7:0) p_id(7:0)
            PIN wr_str wr_str
        END BLOCK
        BEGIN BLOCK XLXI_17 clk20
            PIN CLKIN_IN tmp1
            PIN CLKFX_OUT XLXN_120
            PIN CLK0_OUT
        END BLOCK
        BEGIN BLOCK XLXI_18 testdaten
            PIN clk20
            PIN pattern(7:0) XLXN_161(7:0)
            PIN mux(7:0) XLXN_162(7:0)
            PIN I1(7:0) XLXN_163(7:0)
            PIN I2(7:0) XLXN_164(7:0)
            PIN I3(7:0) XLXN_165(7:0)
            PIN i4(7:0) XLXN_166(7:0)
            PIN q1 data_roh
            PIN q2 XLXN_173
            PIN q3 XLXN_172
            PIN q4 XLXN_64
            PIN trig trig
        END BLOCK
        BEGIN BLOCK XLXI_13 Daten_schieber
            PIN clk20 XLXN_120
            PIN clk200 XLXN_87
            PIN data1_in data_roh
            PIN data2_in XLXN_173
            PIN data3_in XLXN_172
            PIN data4_in XLXN_64
            PIN inb(7:0) XLXN_156(7:0)
            PIN addrb(7:0) XLXN_157(7:0)
            PIN data_out1 data_out
            PIN switch_o(2:0) switch_o(2:0)
            PIN pattern_o(7:0) pattern_o(7:0)
            PIN cnt(7:0) f_cnt(7:0)
            PIN outb(7:0) XLXN_175(7:0)
            PIN wenb(7:0) XLXN_155(7:0)
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        BEGIN BRANCH clk
            WIRE 528 800 704 800
        END BRANCH
        IOMARKER 528 800 clk R180 28
        BEGIN BRANCH XLXN_19
            WIRE 672 736 704 736
        END BRANCH
        BEGIN INSTANCE XLXI_4 512 768 R0
        END INSTANCE
        BEGIN INSTANCE XLXI_8 448 1888 R0
        END INSTANCE
        BEGIN BRANCH tmp1
            WIRE 1040 768 1248 768
        END BRANCH
        BEGIN BRANCH irq
            WIRE 416 1856 448 1856
        END BRANCH
        IOMARKER 416 1856 irq R180 28
        BEGIN BRANCH i1(7:0)
            WIRE 416 1728 448 1728
        END BRANCH
        IOMARKER 416 1728 i1(7:0) R180 28
        BEGIN BRANCH i2(7:0)
            WIRE 416 1760 448 1760
        END BRANCH
        IOMARKER 416 1760 i2(7:0) R180 28
        BEGIN BRANCH i3(7:0)
            WIRE 416 1792 448 1792
        END BRANCH
        IOMARKER 416 1792 i3(7:0) R180 28
        BEGIN BRANCH schaltin(7:0)
            WIRE 416 1616 448 1616
        END BRANCH
        IOMARKER 416 1616 schaltin(7:0) R180 28
        BEGIN BRANCH tast(3:0)
            WIRE 416 1584 448 1584
        END BRANCH
        IOMARKER 416 1584 tast(3:0) R180 28
        BEGIN BRANCH led(7:0)
            WIRE 832 1616 864 1616
        END BRANCH
        IOMARKER 864 1616 led(7:0) R0 28
        BEGIN BRANCH wr_str
            WIRE 832 1472 896 1472
        END BRANCH
        BEGIN BRANCH p_id(7:0)
            WIRE 832 1504 896 1504
        END BRANCH
        BEGIN BRANCH out_p(7:0)
            WIRE 832 1536 896 1536
        END BRANCH
        BEGIN INSTANCE XLXI_6 704 832 R0
        END INSTANCE
        BEGIN INSTANCE XLXI_17 1248 864 R0
        END INSTANCE
        BEGIN INSTANCE XLXI_1 896 1568 R0
        END INSTANCE
        BEGIN BRANCH XLXN_92
            WIRE 416 1296 416 1472
            WIRE 416 1472 448 1472
            WIRE 416 1296 816 1296
            WIRE 816 1296 1040 1296
            WIRE 816 1296 816 1408
            WIRE 816 1408 896 1408
            WIRE 1040 800 1040 1296
        END BRANCH
        BEGIN BRANCH display(11:0)
            WIRE 1248 1456 1312 1456
        END BRANCH
        IOMARKER 1312 1456 display(11:0) R0 28
        BEGIN BRANCH data_roh
            WIRE 1808 1840 1920 1840
            WIRE 1920 1152 1920 1840
            WIRE 1920 1152 2224 1152
            WIRE 2224 1152 2256 1152
            WIRE 2224 896 2224 1152
            WIRE 2224 896 2320 896
        END BRANCH
        BEGIN BRANCH XLXN_64
            WIRE 1808 1936 1968 1936
            WIRE 1968 1248 1968 1936
            WIRE 1968 1248 2256 1248
        END BRANCH
        BEGIN BRANCH data_out
            WIRE 2640 1072 2672 1072
        END BRANCH
        BEGIN BRANCH f_cnt(7:0)
            WIRE 2640 1232 2672 1232
        END BRANCH
        BEGIN BRANCH pattern_o(7:0)
            WIRE 2640 1120 2672 1120
        END BRANCH
        BEGIN BRANCH switch_o(2:0)
            WIRE 2640 1168 2672 1168
        END BRANCH
        BEGIN INSTANCE XLXI_13 2256 1328 R0
        END INSTANCE
        IOMARKER 2672 1072 data_out R0 28
        IOMARKER 2320 896 data_roh R0 28
        IOMARKER 2672 1232 f_cnt(7:0) R0 28
        IOMARKER 2672 1120 pattern_o(7:0) R0 28
        IOMARKER 2672 1168 switch_o(2:0) R0 28
        BEGIN BRANCH XLXN_87
            WIRE 1040 736 1056 736
            WIRE 1056 736 1056 1104
            WIRE 1056 1104 2256 1104
        END BRANCH
        BEGIN BRANCH XLXN_120
            WIRE 1632 768 1648 768
            WIRE 1648 768 1648 1072
            WIRE 1648 1072 2256 1072
        END BRANCH
        BEGIN BRANCH XLXN_155(7:0)
            WIRE 832 1696 1552 1696
            WIRE 1552 1328 1552 1696
            WIRE 1552 1328 2256 1328
        END BRANCH
        BEGIN BRANCH XLXN_156(7:0)
            WIRE 832 1728 1584 1728
            WIRE 1584 1360 1584 1728
            WIRE 1584 1360 2256 1360
        END BRANCH
        BEGIN BRANCH XLXN_157(7:0)
            WIRE 832 1760 1616 1760
            WIRE 1616 1392 1616 1760
            WIRE 1616 1392 2256 1392
        END BRANCH
        BEGIN BRANCH irq_ack
            WIRE 832 2032 864 2032
        END BRANCH
        IOMARKER 864 2032 irq_ack R0 28
        BEGIN INSTANCE XLXI_18 1456 2256 R0
        END INSTANCE
        BEGIN BRANCH XLXN_161(7:0)
            WIRE 832 1792 1136 1792
            WIRE 1136 1792 1136 1872
            WIRE 1136 1872 1456 1872
        END BRANCH
        BEGIN BRANCH XLXN_162(7:0)
            WIRE 832 1824 1120 1824
            WIRE 1120 1824 1120 1920
            WIRE 1120 1920 1456 1920
        END BRANCH
        BEGIN BRANCH XLXN_163(7:0)
            WIRE 832 1856 1104 1856
            WIRE 1104 1856 1104 1968
            WIRE 1104 1968 1456 1968
        END BRANCH
        BEGIN BRANCH XLXN_164(7:0)
            WIRE 832 1888 1088 1888
            WIRE 1088 1888 1088 2016
            WIRE 1088 2016 1456 2016
        END BRANCH
        BEGIN BRANCH XLXN_165(7:0)
            WIRE 832 1920 1072 1920
            WIRE 1072 1920 1072 2064
            WIRE 1072 2064 1456 2064
        END BRANCH
        BEGIN BRANCH XLXN_166(7:0)
            WIRE 832 1952 1056 1952
            WIRE 1056 1952 1056 2112
            WIRE 1056 2112 1456 2112
        END BRANCH
        BEGIN BRANCH trig
            WIRE 1808 2000 1840 2000
        END BRANCH
        IOMARKER 1840 2000 trig R0 28
        BEGIN BRANCH XLXN_172
            WIRE 1808 1904 1952 1904
            WIRE 1952 1216 1952 1904
            WIRE 1952 1216 2096 1216
            WIRE 2096 1216 2256 1216
        END BRANCH
        BEGIN BRANCH XLXN_173
            WIRE 1808 1872 1936 1872
            WIRE 1936 1184 1936 1872
            WIRE 1936 1184 2096 1184
            WIRE 2096 1184 2256 1184
        END BRANCH
        BEGIN BRANCH XLXN_175(7:0)
            WIRE 272 1696 448 1696
            WIRE 272 1696 272 2224
            WIRE 272 2224 1120 2224
            WIRE 1120 2224 2720 2224
            WIRE 2640 1360 2720 1360
            WIRE 2720 1360 2720 2224
        END BRANCH
    END SHEET
END SCHEMATIC
