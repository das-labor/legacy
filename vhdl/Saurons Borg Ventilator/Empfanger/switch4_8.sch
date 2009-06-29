VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "spartan3"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL XLXN_3
        SIGNAL XLXN_4
        SIGNAL XLXN_6
        SIGNAL XLXN_8
        SIGNAL XLXN_9(7:0)
        SIGNAL XLXN_10(7:0)
        SIGNAL q(7:0)
        SIGNAL taste
        SIGNAL clk
        SIGNAL a(7:0)
        SIGNAL b(7:0)
        SIGNAL c(7:0)
        SIGNAL d(7:0)
        PORT Output q(7:0)
        PORT Input taste
        PORT Input clk
        PORT Input a(7:0)
        PORT Input b(7:0)
        PORT Input c(7:0)
        PORT Input d(7:0)
        BEGIN BLOCKDEF pushbutton_tog
            TIMESTAMP 2009 6 29 11 32 51
            RECTANGLE N 64 -128 320 0 
            LINE N 64 -96 0 -96 
            LINE N 64 -32 0 -32 
            LINE N 320 -96 384 -96 
            LINE N 320 -32 384 -32 
        END BLOCKDEF
        BEGIN BLOCKDEF mux_2_8
            TIMESTAMP 2009 6 29 11 32 51
            RECTANGLE N 64 -192 320 0 
            RECTANGLE N 0 -172 64 -148 
            LINE N 64 -160 0 -160 
            RECTANGLE N 0 -108 64 -84 
            LINE N 64 -96 0 -96 
            LINE N 64 -32 0 -32 
            RECTANGLE N 320 -172 384 -148 
            LINE N 320 -160 384 -160 
        END BLOCKDEF
        BEGIN BLOCKDEF f_eins
            TIMESTAMP 2009 6 29 11 32 51
            RECTANGLE N 192 -64 320 0 
            LINE N 320 -32 384 -32 
        END BLOCKDEF
        BEGIN BLOCKDEF cb2ce
            TIMESTAMP 2000 1 1 10 10 10
            RECTANGLE N 64 -384 320 -64 
            LINE N 384 -128 320 -128 
            LINE N 0 -32 64 -32 
            LINE N 0 -128 64 -128 
            LINE N 80 -128 64 -144 
            LINE N 64 -112 80 -128 
            LINE N 192 -32 64 -32 
            LINE N 192 -64 192 -32 
            LINE N 0 -192 64 -192 
            LINE N 384 -192 320 -192 
            LINE N 384 -256 320 -256 
            LINE N 384 -320 320 -320 
        END BLOCKDEF
        BEGIN BLOCK XLXI_231 mux_2_8
            PIN a(7:0) d(7:0)
            PIN b(7:0) a(7:0)
            PIN mux XLXN_6
            PIN q(7:0) XLXN_9(7:0)
        END BLOCK
        BEGIN BLOCK XLXI_242 mux_2_8
            PIN a(7:0) b(7:0)
            PIN b(7:0) c(7:0)
            PIN mux XLXN_6
            PIN q(7:0) XLXN_10(7:0)
        END BLOCK
        BEGIN BLOCK XLXI_243 mux_2_8
            PIN a(7:0) XLXN_9(7:0)
            PIN b(7:0) XLXN_10(7:0)
            PIN mux XLXN_8
            PIN q(7:0) q(7:0)
        END BLOCK
        BEGIN BLOCK XLXI_245 f_eins
            PIN f1 XLXN_3
        END BLOCK
        BEGIN BLOCK XLXI_246 cb2ce
            PIN C XLXN_4
            PIN CE XLXN_3
            PIN CLR
            PIN CEO
            PIN Q0 XLXN_6
            PIN Q1 XLXN_8
            PIN TC
        END BLOCK
        BEGIN BLOCK XLXI_240 pushbutton_tog
            PIN clk clk
            PIN tast taste
            PIN tog
            PIN imp XLXN_4
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        BEGIN INSTANCE XLXI_231 1264 1120 R0
        END INSTANCE
        BEGIN INSTANCE XLXI_242 1280 1552 R0
        END INSTANCE
        BEGIN INSTANCE XLXI_243 1904 1312 R0
        END INSTANCE
        BEGIN BRANCH XLXN_3
            WIRE 1760 576 1792 576
        END BRANCH
        BEGIN INSTANCE XLXI_245 1376 608 R0
        END INSTANCE
        BEGIN BRANCH XLXN_4
            WIRE 1408 640 1792 640
        END BRANCH
        INSTANCE XLXI_246 1792 768 R0
        BEGIN BRANCH XLXN_6
            WIRE 1104 816 1104 1088
            WIRE 1104 1088 1264 1088
            WIRE 1104 1088 1104 1520
            WIRE 1104 1520 1280 1520
            WIRE 1104 816 2256 816
            WIRE 2176 448 2256 448
            WIRE 2256 448 2256 816
        END BRANCH
        BEGIN BRANCH XLXN_8
            WIRE 1840 1040 2272 1040
            WIRE 1840 1040 1840 1280
            WIRE 1840 1280 1904 1280
            WIRE 2176 512 2272 512
            WIRE 2272 512 2272 1040
        END BRANCH
        BEGIN BRANCH XLXN_9(7:0)
            WIRE 1648 960 1664 960
            WIRE 1664 960 1776 960
            WIRE 1776 960 1776 1152
            WIRE 1776 1152 1904 1152
        END BRANCH
        BEGIN BRANCH XLXN_10(7:0)
            WIRE 1664 1392 1680 1392
            WIRE 1680 1392 1776 1392
            WIRE 1776 1216 1776 1392
            WIRE 1776 1216 1904 1216
        END BRANCH
        BEGIN BRANCH q(7:0)
            WIRE 2288 1152 2304 1152
            WIRE 2304 1152 2320 1152
        END BRANCH
        IOMARKER 2320 1152 q(7:0) R0 28
        BEGIN INSTANCE XLXI_240 1024 672 R0
        END INSTANCE
        BEGIN BRANCH taste
            WIRE 944 640 1008 640
            WIRE 1008 640 1024 640
        END BRANCH
        BEGIN BRANCH clk
            WIRE 960 576 1024 576
        END BRANCH
        IOMARKER 944 640 taste R180 28
        IOMARKER 960 576 clk R180 28
        BEGIN BRANCH a(7:0)
            WIRE 1232 1024 1264 1024
        END BRANCH
        IOMARKER 1232 1024 a(7:0) R180 28
        BEGIN BRANCH b(7:0)
            WIRE 1248 1392 1280 1392
        END BRANCH
        IOMARKER 1248 1392 b(7:0) R180 28
        BEGIN BRANCH c(7:0)
            WIRE 1248 1456 1280 1456
        END BRANCH
        IOMARKER 1248 1456 c(7:0) R180 28
        BEGIN BRANCH d(7:0)
            WIRE 1232 960 1264 960
        END BRANCH
        IOMARKER 1232 960 d(7:0) R180 28
    END SHEET
END SCHEMATIC
