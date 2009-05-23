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
        SIGNAL XLXN_185
        SIGNAL clk
        SIGNAL XLXN_213
        SIGNAL out1
        SIGNAL out2
        SIGNAL out3
        SIGNAL out4
        SIGNAL trig
        SIGNAL gnd1
        SIGNAL gnd2
        SIGNAL gnd3
        SIGNAL gnd4
        SIGNAL display(11:0)
        SIGNAL out0
        SIGNAL gnd0
        PORT Input irq
        PORT Input schaltin(7:0)
        PORT Input tast(3:0)
        PORT Output led(7:0)
        PORT Output irq_ack
        PORT Input clk
        PORT Output out1
        PORT Output out2
        PORT Output out3
        PORT Output out4
        PORT Output trig
        PORT Output gnd1
        PORT Output gnd2
        PORT Output gnd3
        PORT Output gnd4
        PORT Output display(11:0)
        PORT Output out0
        PORT Output gnd0
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
            TIMESTAMP 2009 5 23 0 34 5
            RECTANGLE N 64 -320 240 -196 
            LINE N 64 -288 0 -288 
            LINE N 240 -288 304 -288 
            LINE N 240 -256 304 -256 
            LINE N 240 -224 304 -224 
        END BLOCKDEF
        BEGIN BLOCKDEF io_out
            TIMESTAMP 2009 5 23 0 0 27
            RECTANGLE N 64 -176 144 12 
            LINE N 64 -160 0 -160 
            LINE N 64 -128 0 -128 
            LINE N 144 -128 208 -128 
            LINE N 64 -96 0 -96 
            LINE N 144 -96 208 -96 
            LINE N 64 -64 0 -64 
            LINE N 144 -64 208 -64 
            LINE N 64 0 0 0 
            LINE N 144 0 208 0 
            LINE N 64 -32 0 -32 
            LINE N 144 -32 208 -32 
        END BLOCKDEF
        BEGIN BLOCK XLXI_8 Picoblaze
            PIN clk XLXN_92
            PIN irq irq
            PIN i0(7:0)
            PIN i1(7:0)
            PIN i2(7:0)
            PIN i3(7:0)
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
            PIN trig XLXN_213
        END BLOCK
        BEGIN BLOCK XLXI_31 takt2
            PIN clk clk
            PIN clk_20 clk_20
            PIN clk50 XLXN_92
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
        BEGIN BLOCK XLXI_1 display2
            PIN wr_str wr_str
            PIN clk XLXN_92
            PIN Display(11:0) display(11:0)
            PIN out_p(7:0) out_p(7:0)
            PIN p_id(7:0) p_id(7:0)
        END BLOCK
        BEGIN BLOCK XLXI_33 io_out
            PIN i2 XLXN_173
            PIN i3 XLXN_172
            PIN i4 XLXN_64
            PIN i5 XLXN_213
            PIN i1 XLXN_185
            PIN clk clk_20
            PIN o2 out2
            PIN o3 out3
            PIN o4 out4
            PIN o5 trig
            PIN o1 out1
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
            WIRE 832 1472 848 1472
            WIRE 848 1472 928 1472
        END BRANCH
        BEGIN BRANCH XLXN_64
            WIRE 1584 1872 1632 1872
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
        BEGIN BRANCH XLXN_92
            WIRE 448 1296 448 1472
            WIRE 448 1296 816 1296
            WIRE 816 1296 864 1296
            WIRE 864 1296 864 1456
            WIRE 864 1456 928 1456
            WIRE 608 528 816 528
            WIRE 816 528 816 1296
        END BRANCH
        BEGIN INSTANCE XLXI_22 1264 2064 R0
        END INSTANCE
        BEGIN BRANCH XLXN_172
            WIRE 1584 1840 1632 1840
        END BRANCH
        BEGIN BRANCH XLXN_173
            WIRE 1584 1808 1632 1808
        END BRANCH
        BEGIN BRANCH XLXN_185
            WIRE 1584 1776 1632 1776
        END BRANCH
        BEGIN INSTANCE XLXI_31 304 784 R0
        END INSTANCE
        BEGIN BRANCH clk
            WIRE 128 496 304 496
        END BRANCH
        IOMARKER 128 496 clk R180 28
        BEGIN BRANCH clk_20
            WIRE 608 496 1520 496
            WIRE 1520 496 1520 1728
            WIRE 1520 1728 1600 1728
            WIRE 1600 1728 1600 1744
            WIRE 1600 1744 1632 1744
            WIRE 1200 1728 1200 1760
            WIRE 1200 1760 1264 1760
            WIRE 1200 1728 1520 1728
        END BRANCH
        BEGIN BRANCH p_id(7:0)
            WIRE 832 1504 848 1504
            WIRE 848 1504 928 1504
        END BRANCH
        BEGIN BRANCH out_p(7:0)
            WIRE 832 1536 848 1536
            WIRE 848 1536 928 1536
        END BRANCH
        BEGIN BRANCH XLXN_213
            WIRE 1584 1904 1584 1936
            WIRE 1584 1904 1632 1904
        END BRANCH
        BEGIN BRANCH out1
            WIRE 1840 1776 1872 1776
        END BRANCH
        BEGIN BRANCH out2
            WIRE 1840 1808 1872 1808
        END BRANCH
        BEGIN BRANCH out3
            WIRE 1840 1840 1872 1840
        END BRANCH
        BEGIN BRANCH out4
            WIRE 1840 1872 1872 1872
        END BRANCH
        BEGIN BRANCH trig
            WIRE 1840 1904 1872 1904
        END BRANCH
        IOMARKER 1872 1776 out1 R0 28
        IOMARKER 1872 1808 out2 R0 28
        IOMARKER 1872 1840 out3 R0 28
        IOMARKER 1872 1872 out4 R0 28
        IOMARKER 1872 1904 trig R0 28
        BEGIN INSTANCE XLXI_23 1680 2000 R0
        END INSTANCE
        BEGIN BRANCH gnd1
            WIRE 1840 1968 1872 1968
        END BRANCH
        BEGIN INSTANCE XLXI_26 1680 2032 R0
        END INSTANCE
        BEGIN BRANCH gnd2
            WIRE 1840 2000 1872 2000
        END BRANCH
        BEGIN INSTANCE XLXI_27 1680 2064 R0
        END INSTANCE
        BEGIN BRANCH gnd3
            WIRE 1840 2032 1872 2032
        END BRANCH
        BEGIN INSTANCE XLXI_28 1680 2096 R0
        END INSTANCE
        BEGIN BRANCH gnd4
            WIRE 1840 2064 1872 2064
        END BRANCH
        IOMARKER 1872 1968 gnd1 R0 28
        IOMARKER 1872 2000 gnd2 R0 28
        IOMARKER 1872 2032 gnd3 R0 28
        IOMARKER 1872 2064 gnd4 R0 28
        BEGIN INSTANCE XLXI_1 928 1568 R0
        END INSTANCE
        BEGIN BRANCH display(11:0)
            WIRE 1248 1456 1296 1456
        END BRANCH
        IOMARKER 1296 1456 display(11:0) R0 28
        BEGIN INSTANCE XLXI_33 1632 1904 R0
        END INSTANCE
        BEGIN BRANCH out0
            WIRE 608 560 656 560
        END BRANCH
        IOMARKER 656 560 out0 R0 28
        BEGIN INSTANCE XLXI_19 448 672 R0
        END INSTANCE
        BEGIN BRANCH gnd0
            WIRE 608 640 656 640
        END BRANCH
        IOMARKER 656 640 gnd0 R0 28
    END SHEET
END SCHEMATIC
