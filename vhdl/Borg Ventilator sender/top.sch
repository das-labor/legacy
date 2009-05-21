VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "spartan3"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL wr_str
        SIGNAL out_p(7:0)
        SIGNAL irq
        SIGNAL schaltin(7:0)
        SIGNAL tast(3:0)
        SIGNAL p_id(7:0)
        SIGNAL led(7:0)
        SIGNAL clk200
        SIGNAL XLXN_92
        SIGNAL clk_20
        SIGNAL XLXN_64
        SIGNAL irq_ack
        SIGNAL XLXN_161(7:0)
        SIGNAL XLXN_162(7:0)
        SIGNAL XLXN_163(7:0)
        SIGNAL XLXN_164(7:0)
        SIGNAL XLXN_165(7:0)
        SIGNAL XLXN_166(7:0)
        SIGNAL XLXN_172
        SIGNAL XLXN_173
        SIGNAL XLXN_175(7:0)
        SIGNAL XLXN_182
        SIGNAL XLXN_185
        SIGNAL XLXN_198(7:0)
        SIGNAL XLXN_205(7:0)
        SIGNAL XLXN_206(7:0)
        SIGNAL trig
        SIGNAL out1
        SIGNAL out2
        SIGNAL out3
        SIGNAL out4
        SIGNAL data_roh
        SIGNAL display(11:0)
        SIGNAL clk
        SIGNAL gnd1
        SIGNAL gnd2
        SIGNAL gnd3
        SIGNAL gnd4
        SIGNAL gnd0
        SIGNAL out0
        PORT Input irq
        PORT Input schaltin(7:0)
        PORT Input tast(3:0)
        PORT Output led(7:0)
        PORT Output clk200
        PORT Output clk_20
        PORT Output irq_ack
        PORT Output trig
        PORT Output out1
        PORT Output out2
        PORT Output out3
        PORT Output out4
        PORT Output data_roh
        PORT Output display(11:0)
        PORT Input clk
        PORT Output gnd1
        PORT Output gnd2
        PORT Output gnd3
        PORT Output gnd4
        PORT Output gnd0
        PORT Output out0
        BEGIN BLOCKDEF display2
            TIMESTAMP 2009 5 20 23 44 40
            RECTANGLE N 64 -132 256 -16 
            LINE N 64 -96 0 -96 
            LINE N 64 -112 0 -112 
            RECTANGLE N 256 -124 320 -100 
            LINE N 256 -112 320 -112 
            RECTANGLE N 0 -44 64 -20 
            RECTANGLE N 0 -76 64 -52 
            LINE N 64 -64 0 -64 
            LINE N 64 -32 0 -32 
        END BLOCKDEF
        BEGIN BLOCKDEF f_null
            TIMESTAMP 2009 5 20 23 44 40
            RECTANGLE N 60 -44 96 -20 
            LINE N 96 -32 160 -32 
        END BLOCKDEF
        BEGIN BLOCKDEF Picoblaze
            TIMESTAMP 2009 5 20 23 44 40
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
            TIMESTAMP 2009 5 20 23 44 39
            LINE N 64 -256 0 -256 
            LINE N 64 -224 0 -224 
            LINE N 64 -176 0 -176 
            LINE N 64 -144 0 -144 
            LINE N 64 -112 0 -112 
            LINE N 64 -80 0 -80 
            BEGIN LINE N 64 -40 320 -40 
                LINECOLOR 0 0 255
            END LINE
            RECTANGLE N 320 -28 384 -4 
            RECTANGLE N 320 4 384 28 
            LINE N 320 16 384 16 
            RECTANGLE N 320 36 384 60 
            LINE N 320 48 384 48 
            RECTANGLE N 320 68 384 92 
            LINE N 320 80 384 80 
            LINE N 320 -16 384 -16 
            LINE N 64 0 0 0 
            RECTANGLE N 0 20 64 44 
            LINE N 64 32 0 32 
            RECTANGLE N 0 52 64 76 
            LINE N 64 64 0 64 
            LINE N 320 -176 384 -176 
            LINE N 320 -144 384 -144 
            LINE N 320 -112 384 -112 
            LINE N 320 -80 384 -80 
            RECTANGLE N 64 -288 320 112 
        END BLOCKDEF
        BEGIN BLOCKDEF roh_verzoegerung
            TIMESTAMP 2009 5 20 23 44 39
            RECTANGLE N 64 -84 160 -16 
            LINE N 64 -32 0 -32 
            LINE N 64 -64 0 -64 
            LINE N 160 -32 224 -32 
        END BLOCKDEF
        BEGIN BLOCKDEF tgb
            TIMESTAMP 2009 5 20 23 44 40
            RECTANGLE N 64 -320 256 -88 
            LINE N 64 -304 0 -304 
            LINE N 256 -288 320 -288 
            RECTANGLE N 0 -220 64 -196 
            LINE N 64 -208 0 -208 
            RECTANGLE N 0 -188 64 -164 
            LINE N 64 -176 0 -176 
            RECTANGLE N 0 -156 64 -132 
            LINE N 64 -144 0 -144 
            RECTANGLE N 0 -124 64 -100 
            LINE N 64 -112 0 -112 
            RECTANGLE N 0 -252 64 -228 
            LINE N 64 -240 0 -240 
            RECTANGLE N 0 -284 64 -260 
            LINE N 64 -272 0 -272 
            LINE N 256 -256 320 -256 
            LINE N 256 -224 320 -224 
            LINE N 256 -192 320 -192 
            LINE N 256 -128 320 -128 
        END BLOCKDEF
        BEGIN BLOCKDEF takt2
            TIMESTAMP 2009 5 20 23 44 39
            RECTANGLE N 64 -320 240 -96 
            LINE N 64 -288 0 -288 
            LINE N 240 -288 304 -288 
            LINE N 240 -256 304 -256 
            LINE N 240 -224 304 -224 
            LINE N 240 -128 304 -128 
        END BLOCKDEF
        BEGIN BLOCK XLXI_8 Picoblaze
            PIN clk XLXN_92
            PIN irq irq
            PIN i0(7:0) XLXN_175(7:0)
            PIN i1(7:0) XLXN_198(7:0)
            PIN i2(7:0) XLXN_205(7:0)
            PIN i3(7:0) XLXN_206(7:0)
            PIN schalter(7:0) schaltin(7:0)
            PIN taster(3:0) tast(3:0)
            PIN wr_str wr_str
            PIN p_id(7:0) p_id(7:0)
            PIN out_p(7:0) out_p(7:0)
            PIN q0(7:0)
            PIN q1(7:0)
            PIN q2(7:0)
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
        BEGIN BLOCK XLXI_1 display2
            PIN wr_str wr_str
            PIN clk XLXN_92
            PIN Display(11:0) display(11:0)
            PIN out_p(7:0) out_p(7:0)
            PIN p_id(7:0) p_id(7:0)
        END BLOCK
        BEGIN BLOCK XLXI_22 tgb
            PIN clk20 clk_20
            PIN q1 XLXN_185
            PIN a(7:0) XLXN_163(7:0)
            PIN b(7:0) XLXN_164(7:0)
            PIN c(7:0) XLXN_165(7:0)
            PIN d(7:0) XLXN_166(7:0)
            PIN mux(7:0) XLXN_162(7:0)
            PIN pattern(7:0) XLXN_161(7:0)
            PIN q2 XLXN_173
            PIN q3 XLXN_172
            PIN q4 XLXN_64
            PIN trig XLXN_182
        END BLOCK
        BEGIN BLOCK XLXI_21 roh_verzoegerung
            PIN a XLXN_182
            PIN clk200 clk200
            PIN q trig
        END BLOCK
        BEGIN BLOCK XLXI_13 Daten_schieber
            PIN clk20 clk_20
            PIN clk200 clk200
            PIN data1_in XLXN_185
            PIN data2_in XLXN_173
            PIN data3_in XLXN_172
            PIN data4_in XLXN_64
            PIN wr_str wr_str
            PIN p_id(7:0) p_id(7:0)
            PIN out_p(7:0) out_p(7:0)
            PIN data_out1 out1
            PIN data_out2 out2
            PIN data_out3 out3
            PIN data_out4 out4
            PIN outb1(7:0) XLXN_175(7:0)
            PIN outb2(7:0) XLXN_198(7:0)
            PIN outb3(7:0) XLXN_205(7:0)
            PIN outb4(7:0) XLXN_206(7:0)
        END BLOCK
        BEGIN BLOCK XLXI_20 roh_verzoegerung
            PIN a XLXN_185
            PIN clk200 clk200
            PIN q data_roh
        END BLOCK
        BEGIN BLOCK XLXI_31 takt2
            PIN clk clk
            PIN clk_20 clk_20
            PIN clk50 XLXN_92
            PIN clk200 clk200
            PIN g90_c10 out0
        END BLOCK
        BEGIN BLOCK XLXI_23 f_null
            PIN fo gnd1
        END BLOCK
        BEGIN BLOCK XLXI_26 f_null
            PIN fo gnd2
        END BLOCK
        BEGIN BLOCK XLXI_27 f_null
            PIN fo gnd3
        END BLOCK
        BEGIN BLOCK XLXI_28 f_null
            PIN fo gnd4
        END BLOCK
        BEGIN BLOCK XLXI_19 f_null
            PIN fo gnd0
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        BEGIN INSTANCE XLXI_8 448 1888 R0
        END INSTANCE
        BEGIN BRANCH irq
            WIRE 416 1856 448 1856
        END BRANCH
        IOMARKER 416 1856 irq R180 28
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
            WIRE 832 1472 864 1472
            WIRE 864 1472 1824 1472
            WIRE 1824 1472 1824 1952
            WIRE 1824 1952 2288 1952
            WIRE 864 1312 928 1312
            WIRE 864 1312 864 1472
        END BRANCH
        BEGIN BRANCH p_id(7:0)
            WIRE 832 1504 880 1504
            WIRE 880 1504 1808 1504
            WIRE 1808 1504 1808 1984
            WIRE 1808 1984 2288 1984
            WIRE 880 1344 928 1344
            WIRE 880 1344 880 1504
        END BRANCH
        BEGIN BRANCH out_p(7:0)
            WIRE 832 1536 896 1536
            WIRE 896 1536 1792 1536
            WIRE 1792 1536 1792 2016
            WIRE 1792 2016 2288 2016
            WIRE 896 1376 896 1536
            WIRE 896 1376 928 1376
        END BRANCH
        BEGIN BRANCH XLXN_64
            WIRE 1584 1872 2288 1872
        END BRANCH
        BEGIN BRANCH irq_ack
            WIRE 832 2032 864 2032
        END BRANCH
        IOMARKER 864 2032 irq_ack R0 28
        BEGIN BRANCH XLXN_161(7:0)
            WIRE 832 1792 1264 1792
        END BRANCH
        BEGIN BRANCH XLXN_162(7:0)
            WIRE 832 1824 1264 1824
        END BRANCH
        BEGIN BRANCH XLXN_163(7:0)
            WIRE 832 1856 1264 1856
        END BRANCH
        BEGIN BRANCH XLXN_164(7:0)
            WIRE 832 1888 1264 1888
        END BRANCH
        BEGIN BRANCH XLXN_165(7:0)
            WIRE 832 1920 1264 1920
        END BRANCH
        BEGIN BRANCH XLXN_166(7:0)
            WIRE 832 1952 1264 1952
        END BRANCH
        BEGIN BRANCH XLXN_175(7:0)
            WIRE 288 1696 448 1696
            WIRE 288 1696 288 2304
            WIRE 288 2304 2768 2304
            WIRE 2672 1936 2768 1936
            WIRE 2768 1936 2768 2304
        END BRANCH
        BEGIN BRANCH XLXN_92
            WIRE 448 1296 448 1472
            WIRE 448 1296 816 1296
            WIRE 816 1296 928 1296
            WIRE 608 528 816 528
            WIRE 816 528 816 1296
        END BRANCH
        BEGIN INSTANCE XLXI_1 928 1408 R0
        END INSTANCE
        BEGIN BRANCH XLXN_198(7:0)
            WIRE 304 1728 448 1728
            WIRE 304 1728 304 2288
            WIRE 304 2288 2752 2288
            WIRE 2672 1968 2752 1968
            WIRE 2752 1968 2752 2288
        END BRANCH
        BEGIN BRANCH XLXN_205(7:0)
            WIRE 320 1760 448 1760
            WIRE 320 1760 320 2272
            WIRE 320 2272 2736 2272
            WIRE 2672 2000 2736 2000
            WIRE 2736 2000 2736 2272
        END BRANCH
        BEGIN BRANCH XLXN_206(7:0)
            WIRE 336 1792 336 2256
            WIRE 336 2256 2720 2256
            WIRE 336 1792 448 1792
            WIRE 2672 2032 2720 2032
            WIRE 2720 2032 2720 2256
        END BRANCH
        BEGIN INSTANCE XLXI_22 1264 2064 R0
        END INSTANCE
        BEGIN BRANCH XLXN_172
            WIRE 1584 1840 2288 1840
        END BRANCH
        BEGIN INSTANCE XLXI_21 1504 2240 R0
        END INSTANCE
        BEGIN BRANCH trig
            WIRE 1728 2208 1760 2208
        END BRANCH
        IOMARKER 1760 2208 trig R0 28
        BEGIN BRANCH XLXN_182
            WIRE 1440 2096 1440 2208
            WIRE 1440 2208 1504 2208
            WIRE 1440 2096 1584 2096
            WIRE 1584 1936 1584 2096
        END BRANCH
        BEGIN BRANCH XLXN_173
            WIRE 1584 1808 2288 1808
        END BRANCH
        BEGIN INSTANCE XLXI_13 2288 1952 R0
        END INSTANCE
        BEGIN BRANCH out1
            WIRE 2672 1776 2704 1776
        END BRANCH
        BEGIN BRANCH out2
            WIRE 2672 1808 2704 1808
        END BRANCH
        BEGIN BRANCH out3
            WIRE 2672 1840 2704 1840
        END BRANCH
        IOMARKER 2704 1776 out1 R0 28
        IOMARKER 2704 1808 out2 R0 28
        IOMARKER 2704 1840 out3 R0 28
        BEGIN INSTANCE XLXI_20 1984 2240 R0
        END INSTANCE
        BEGIN BRANCH data_roh
            WIRE 2208 2208 2224 2208
        END BRANCH
        IOMARKER 2224 2208 data_roh R0 28
        BEGIN BRANCH XLXN_185
            WIRE 1584 1776 1920 1776
            WIRE 1920 1776 2288 1776
            WIRE 1920 1776 1920 2208
            WIRE 1920 2208 1984 2208
        END BRANCH
        BEGIN BRANCH display(11:0)
            WIRE 1248 1296 1296 1296
        END BRANCH
        IOMARKER 1296 1296 display(11:0) R0 28
        BEGIN INSTANCE XLXI_31 304 784 R0
        END INSTANCE
        BEGIN BRANCH clk
            WIRE 128 496 304 496
        END BRANCH
        IOMARKER 128 496 clk R180 28
        BEGIN BRANCH clk200
            WIRE 608 560 1936 560
            WIRE 1936 560 1936 1728
            WIRE 1936 1728 2288 1728
            WIRE 1936 1728 1936 2112
            WIRE 1936 2112 1936 2176
            WIRE 1936 2176 1984 2176
            WIRE 1936 560 2032 560
            WIRE 1456 2112 1456 2176
            WIRE 1456 2176 1504 2176
            WIRE 1456 2112 1936 2112
        END BRANCH
        IOMARKER 2032 560 clk200 R0 28
        BEGIN BRANCH out4
            WIRE 2672 1872 2704 1872
        END BRANCH
        IOMARKER 2704 1872 out4 R0 28
        BEGIN INSTANCE XLXI_23 2816 1808 R0
        END INSTANCE
        BEGIN BRANCH gnd1
            WIRE 2976 1776 3008 1776
        END BRANCH
        BEGIN INSTANCE XLXI_26 2816 1840 R0
        END INSTANCE
        BEGIN BRANCH gnd2
            WIRE 2976 1808 3008 1808
        END BRANCH
        BEGIN INSTANCE XLXI_27 2816 1872 R0
        END INSTANCE
        BEGIN BRANCH gnd3
            WIRE 2976 1840 3008 1840
        END BRANCH
        BEGIN INSTANCE XLXI_28 2816 1904 R0
        END INSTANCE
        BEGIN BRANCH gnd4
            WIRE 2976 1872 3008 1872
        END BRANCH
        IOMARKER 3008 1776 gnd1 R0 28
        IOMARKER 3008 1808 gnd2 R0 28
        IOMARKER 3008 1840 gnd3 R0 28
        IOMARKER 3008 1872 gnd4 R0 28
        BEGIN INSTANCE XLXI_19 448 752 R0
        END INSTANCE
        BEGIN BRANCH gnd0
            WIRE 608 720 640 720
        END BRANCH
        IOMARKER 640 720 gnd0 R0 28
        IOMARKER 2032 496 clk_20 R0 28
        BEGIN BRANCH clk_20
            WIRE 608 496 1920 496
            WIRE 1920 496 2032 496
            WIRE 1920 496 1920 1696
            WIRE 1920 1696 2288 1696
            WIRE 1200 1696 1200 1760
            WIRE 1200 1760 1264 1760
            WIRE 1200 1696 1920 1696
        END BRANCH
        BEGIN BRANCH out0
            WIRE 608 656 944 656
        END BRANCH
        IOMARKER 944 656 out0 R0 28
    END SHEET
END SCHEMATIC
