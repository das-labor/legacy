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
        SIGNAL schaltin(7:0)
        SIGNAL tast(3:0)
        SIGNAL p_id(7:0)
        SIGNAL led(7:0)
        SIGNAL clk50
        SIGNAL clk_20
        SIGNAL XLXN_64
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
        SIGNAL gnd3
        SIGNAL gnd2
        SIGNAL gnd1
        SIGNAL out0
        SIGNAL gnd0
        SIGNAL clk100
        SIGNAL gnd4
        SIGNAL sram_oe
        SIGNAL sram_we
        SIGNAL sram_adr(17:0)
        SIGNAL sram_1_io(15:0)
        SIGNAL sram_1_ce
        SIGNAL sram_1_ub
        SIGNAL sram_1_lb
        SIGNAL sram_2_ce
        SIGNAL sram_2_io(15:0)
        SIGNAL sram_2_ub
        SIGNAL sram_2_lb
        SIGNAL display(11:0)
        SIGNAL winkel(9:0)
        SIGNAL sram_read(15:0)
        SIGNAL sram_pos(7:0)
        SIGNAL winkel_diag(9:0)
        PORT Input schaltin(7:0)
        PORT Input tast(3:0)
        PORT Output led(7:0)
        PORT Output clk50
        PORT Input clk
        PORT Output out1
        PORT Output out2
        PORT Output out3
        PORT Output out4
        PORT Output trig
        PORT Output gnd3
        PORT Output gnd2
        PORT Output gnd1
        PORT Output out0
        PORT Output gnd0
        PORT Output clk100
        PORT Output gnd4
        PORT Output sram_oe
        PORT Output sram_we
        PORT Output sram_adr(17:0)
        PORT BiDirectional sram_1_io(15:0)
        PORT Output sram_1_ce
        PORT Output sram_1_ub
        PORT Output sram_1_lb
        PORT Output sram_2_ce
        PORT BiDirectional sram_2_io(15:0)
        PORT Output sram_2_ub
        PORT Output sram_2_lb
        PORT Output display(11:0)
        PORT Input winkel(9:0)
        PORT Output sram_read(15:0)
        PORT Output sram_pos(7:0)
        PORT Output winkel_diag(9:0)
        BEGIN BLOCKDEF display2
            TIMESTAMP 2009 7 16 13 56 18
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
        BEGIN BLOCKDEF Picoblaze
            TIMESTAMP 2009 7 16 13 56 17
            RECTANGLE N 320 196 384 220 
            LINE N 320 208 384 208 
            RECTANGLE N 320 228 384 252 
            LINE N 320 240 384 240 
            RECTANGLE N 320 260 384 284 
            LINE N 320 272 384 272 
            RECTANGLE N 320 292 384 316 
            LINE N 320 304 384 304 
            LINE N 64 -416 0 -416 
            LINE N 320 -416 384 -416 
            RECTANGLE N 320 -396 384 -372 
            LINE N 320 -384 384 -384 
            RECTANGLE N 320 -364 384 -340 
            LINE N 320 -352 384 -352 
            RECTANGLE N 320 -284 384 -260 
            LINE N 320 -272 384 -272 
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
            RECTANGLE N 320 -108 384 -84 
            LINE N 320 -96 384 -96 
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
            LINE N 316 144 380 144 
            RECTANGLE N 64 -448 320 364 
            LINE N 320 336 384 336 
        END BLOCKDEF
        BEGIN BLOCKDEF tgb
            TIMESTAMP 2009 7 16 13 56 18
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
            TIMESTAMP 2009 7 16 13 56 18
            LINE N 240 -192 304 -192 
            LINE N 64 -288 0 -288 
            LINE N 240 -288 304 -288 
            LINE N 240 -256 304 -256 
            LINE N 240 -224 304 -224 
            RECTANGLE N 64 -320 240 -164 
        END BLOCKDEF
        BEGIN BLOCKDEF io_out
            TIMESTAMP 2009 7 16 13 56 18
            LINE N 64 -160 0 -160 
            LINE N 64 -128 0 -128 
            LINE N 64 -96 0 -96 
            LINE N 64 -64 0 -64 
            LINE N 64 -32 0 -32 
            LINE N 144 -128 208 -128 
            LINE N 144 -96 208 -96 
            LINE N 144 -64 208 -64 
            LINE N 144 -32 208 -32 
            LINE N 64 0 0 0 
            LINE N 144 0 208 0 
            RECTANGLE N 64 -176 144 144 
            LINE N 144 32 208 32 
            LINE N 144 64 208 64 
            LINE N 144 96 208 96 
            LINE N 144 128 208 128 
        END BLOCKDEF
        BEGIN BLOCKDEF inv
            TIMESTAMP 2000 1 1 10 10 10
            LINE N 0 -32 64 -32 
            LINE N 224 -32 160 -32 
            LINE N 64 -64 128 -32 
            LINE N 128 -32 64 0 
            LINE N 64 0 64 -64 
            CIRCLE N 128 -48 160 -16 
        END BLOCKDEF
        BEGIN BLOCKDEF ram_control
            TIMESTAMP 2009 7 18 10 41 53
            LINE N 448 -864 512 -864 
            LINE N 448 -672 512 -672 
            LINE N 448 -544 512 -544 
            LINE N 448 -832 512 -832 
            LINE N 448 -640 512 -640 
            LINE N 448 -704 512 -704 
            LINE N 448 -512 512 -512 
            LINE N 448 -480 512 -480 
            RECTANGLE N 448 -812 512 -788 
            LINE N 448 -800 512 -800 
            RECTANGLE N 448 -748 512 -724 
            LINE N 448 -736 512 -736 
            RECTANGLE N 448 -588 512 -564 
            LINE N 448 -576 512 -576 
            RECTANGLE N 448 -428 512 -404 
            LINE N 448 -416 512 -416 
            RECTANGLE N 112 -896 448 -244 
            RECTANGLE N 448 -396 512 -372 
            LINE N 448 -384 512 -384 
            LINE N 448 -320 512 -320 
            RECTANGLE N 448 -300 512 -276 
            LINE N 448 -288 512 -288 
            LINE N 112 -864 48 -864 
            LINE N 112 -832 48 -832 
            RECTANGLE N 48 -748 112 -724 
            LINE N 112 -736 48 -736 
            LINE N 112 -704 48 -704 
            RECTANGLE N 48 -780 112 -756 
            LINE N 112 -768 48 -768 
            LINE N 112 -656 48 -656 
            RECTANGLE N 48 -636 112 -612 
            LINE N 112 -624 48 -624 
            RECTANGLE N 48 -604 112 -580 
            LINE N 112 -592 48 -592 
            RECTANGLE N 48 -540 112 -516 
            LINE N 112 -528 48 -528 
        END BLOCKDEF
        BEGIN BLOCK XLXI_8 Picoblaze
            PIN clk clk50
            PIN irq
            PIN i0(7:0)
            PIN i1(7:0)
            PIN i2(7:0)
            PIN i3(7:0)
            PIN schalter(7:0) schaltin(7:0)
            PIN taster(3:0) tast(3:0)
            PIN wr_str wr_str
            PIN irq_ack
            PIN p_id(7:0) p_id(7:0)
            PIN out_p(7:0) out_p(7:0)
            PIN led(7:0) led(7:0)
            PIN q13(7:0) XLXN_161(7:0)
            PIN q14(7:0) XLXN_162(7:0)
            PIN q15(7:0) XLXN_163(7:0)
            PIN q16(7:0) XLXN_164(7:0)
            PIN q17(7:0) XLXN_165(7:0)
            PIN q18(7:0) XLXN_166(7:0)
            PIN q0(7:0)
            PIN q1(7:0)
            PIN q2(7:0)
            PIN q3(7:0)
            PIN q4
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
            PIN clk50 clk50
            PIN g90_c10 out0
            PIN clk100 clk100
        END BLOCK
        BEGIN BLOCK XLXI_33 io_out
            PIN i2 XLXN_173
            PIN i4 XLXN_64
            PIN i1 XLXN_185
            PIN clk clk_20
            PIN i3 XLXN_172
            PIN o1a out1
            PIN o2a out2
            PIN o3a out3
            PIN o4a out4
            PIN trig XLXN_213
            PIN trib_o trig
            PIN o1b gnd1
            PIN o2b gnd2
            PIN o3b gnd3
            PIN o4b gnd4
        END BLOCK
        BEGIN BLOCK XLXI_41 inv
            PIN I out0
            PIN O gnd0
        END BLOCK
        BEGIN BLOCK XLXI_42 ram_control
            PIN sram_1_io(15:0) sram_1_io(15:0)
            PIN sram_2_io(15:0) sram_2_io(15:0)
            PIN sram_oe sram_oe
            PIN sram_we sram_we
            PIN sram_1_ce sram_1_ce
            PIN sram_1_ub sram_1_ub
            PIN sram_1_lb sram_1_lb
            PIN sram_2_ce sram_2_ce
            PIN sram_2_ub sram_2_ub
            PIN sram_2_lb sram_2_lb
            PIN sram_adr(17:0) sram_adr(17:0)
            PIN sram_read(15:0) sram_read(15:0)
            PIN sram_pos(7:0) sram_pos(7:0)
            PIN write_lesen_diag
            PIN winkel_diag(9:0) winkel_diag(9:0)
            PIN clk50 clk50
            PIN clk100 clk100
            PIN ad_dat(15:0)
            PIN ad_wr
            PIN ad_adr(17:0)
            PIN wr_str wr_str
            PIN p_id(7:0) p_id(7:0)
            PIN out_p(7:0) out_p(7:0)
            PIN winkel(9:0) winkel(9:0)
        END BLOCK
        BEGIN BLOCK XLXI_1 display2
            PIN wr_str wr_str
            PIN clk clk50
            PIN Display(11:0) display(11:0)
            PIN out_p(7:0) out_p(7:0)
            PIN p_id(7:0) p_id(7:0)
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        BEGIN INSTANCE XLXI_8 448 1888 R0
        END INSTANCE
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
            WIRE 864 1472 1616 1472
            WIRE 864 1312 928 1312
            WIRE 864 1312 864 1472
            WIRE 1616 1168 1616 1472
            WIRE 1616 1168 2400 1168
        END BRANCH
        BEGIN BRANCH XLXN_64
            WIRE 1584 1872 1632 1872
        END BRANCH
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
            WIRE 848 1504 880 1504
            WIRE 880 1504 1632 1504
            WIRE 880 1344 880 1504
            WIRE 880 1344 928 1344
            WIRE 1632 1200 1632 1504
            WIRE 1632 1200 2400 1200
        END BRANCH
        BEGIN BRANCH out_p(7:0)
            WIRE 832 1536 848 1536
            WIRE 848 1536 896 1536
            WIRE 896 1536 1648 1536
            WIRE 896 1376 896 1536
            WIRE 896 1376 928 1376
            WIRE 1648 1232 1648 1536
            WIRE 1648 1232 2400 1232
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
        BEGIN INSTANCE XLXI_33 1632 1904 R0
        END INSTANCE
        BEGIN BRANCH gnd3
            WIRE 1840 2000 1872 2000
        END BRANCH
        IOMARKER 1872 2000 gnd3 R0 28
        BEGIN BRANCH gnd2
            WIRE 1840 1968 1872 1968
        END BRANCH
        BEGIN BRANCH gnd1
            WIRE 1840 1936 1872 1936
        END BRANCH
        IOMARKER 1872 1936 gnd1 R0 28
        IOMARKER 1872 1968 gnd2 R0 28
        INSTANCE XLXI_41 1088 720 R0
        BEGIN BRANCH out0
            WIRE 608 560 976 560
            WIRE 976 560 976 688
            WIRE 976 688 1088 688
            WIRE 976 560 1344 560
        END BRANCH
        BEGIN BRANCH gnd0
            WIRE 1312 688 1344 688
        END BRANCH
        IOMARKER 1344 688 gnd0 R0 28
        IOMARKER 1344 560 out0 R0 28
        BEGIN BRANCH gnd4
            WIRE 1840 2032 1856 2032
        END BRANCH
        BEGIN BRANCH clk50
            WIRE 448 1296 448 1472
            WIRE 448 1296 816 1296
            WIRE 816 1296 928 1296
            WIRE 608 528 816 528
            WIRE 816 528 816 960
            WIRE 816 960 816 1296
            WIRE 816 960 2400 960
            WIRE 816 416 896 416
            WIRE 816 416 816 528
        END BRANCH
        BEGIN BRANCH clk100
            WIRE 608 592 624 592
            WIRE 624 592 624 992
            WIRE 624 992 2400 992
            WIRE 624 368 896 368
            WIRE 624 368 624 592
        END BRANCH
        BEGIN INSTANCE XLXI_42 2352 1824 R0
        END INSTANCE
        BEGIN BRANCH sram_oe
            WIRE 2864 960 2896 960
        END BRANCH
        BEGIN BRANCH sram_we
            WIRE 2864 992 2896 992
        END BRANCH
        IOMARKER 2896 960 sram_oe R0 28
        IOMARKER 2896 992 sram_we R0 28
        BEGIN BRANCH sram_adr(17:0)
            WIRE 2864 1024 2896 1024
        END BRANCH
        IOMARKER 2896 1024 sram_adr(17:0) R0 28
        BEGIN BRANCH sram_1_io(15:0)
            WIRE 2864 1088 2896 1088
        END BRANCH
        IOMARKER 2896 1088 sram_1_io(15:0) R0 28
        BEGIN BRANCH sram_1_ce
            WIRE 2864 1120 2896 1120
        END BRANCH
        BEGIN BRANCH sram_1_ub
            WIRE 2864 1152 2896 1152
        END BRANCH
        BEGIN BRANCH sram_1_lb
            WIRE 2864 1184 2896 1184
        END BRANCH
        IOMARKER 2896 1152 sram_1_ub R0 28
        IOMARKER 2896 1184 sram_1_lb R0 28
        IOMARKER 2896 1120 sram_1_ce R0 28
        BEGIN BRANCH sram_2_ce
            WIRE 2864 1280 2896 1280
        END BRANCH
        IOMARKER 2896 1280 sram_2_ce R0 28
        BEGIN BRANCH sram_2_io(15:0)
            WIRE 2864 1248 2896 1248
        END BRANCH
        IOMARKER 2896 1248 sram_2_io(15:0) R0 28
        BEGIN BRANCH sram_2_ub
            WIRE 2864 1312 2896 1312
        END BRANCH
        IOMARKER 2896 1312 sram_2_ub R0 28
        BEGIN BRANCH sram_2_lb
            WIRE 2864 1344 2896 1344
        END BRANCH
        IOMARKER 2896 1344 sram_2_lb R0 28
        IOMARKER 1856 2032 gnd4 R0 28
        IOMARKER 896 416 clk50 R0 28
        IOMARKER 896 368 clk100 R0 28
        BEGIN INSTANCE XLXI_1 928 1408 R0
        END INSTANCE
        BEGIN BRANCH display(11:0)
            WIRE 1248 1296 1296 1296
        END BRANCH
        IOMARKER 1296 1296 display(11:0) R0 28
        BEGIN BRANCH winkel(9:0)
            WIRE 2368 1296 2400 1296
        END BRANCH
        IOMARKER 2368 1296 winkel(9:0) R180 28
        BEGIN BRANCH sram_read(15:0)
            WIRE 2864 1408 2896 1408
        END BRANCH
        IOMARKER 2896 1408 sram_read(15:0) R0 28
        BEGIN BRANCH sram_pos(7:0)
            WIRE 2864 1440 2896 1440
        END BRANCH
        IOMARKER 2896 1440 sram_pos(7:0) R0 28
        BEGIN BRANCH winkel_diag(9:0)
            WIRE 2864 1536 2896 1536
        END BRANCH
        IOMARKER 2896 1536 winkel_diag(9:0) R0 28
    END SHEET
END SCHEMATIC
