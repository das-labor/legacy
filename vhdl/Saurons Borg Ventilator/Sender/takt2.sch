VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "spartan3"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL clk_20
        SIGNAL clk50
        SIGNAL tmp1
        SIGNAL clk
        SIGNAL XLXN_93
        SIGNAL XLXN_97
        SIGNAL g90_c10
        SIGNAL XLXN_116
        SIGNAL clk100
        PORT Output clk_20
        PORT Output clk50
        PORT Input clk
        PORT Output g90_c10
        PORT Output clk100
        BEGIN BLOCKDEF takt
            TIMESTAMP 2009 6 19 22 27 30
            LINE N 272 0 336 0 
            LINE N 64 -96 0 -96 
            LINE N 272 -64 336 -64 
            LINE N 64 -32 0 -32 
            LINE N 272 -32 336 -32 
            RECTANGLE N 64 -128 272 32 
        END BLOCKDEF
        BEGIN BLOCKDEF f_null
            TIMESTAMP 2009 5 24 21 36 47
            RECTANGLE N 60 -44 96 -20 
            LINE N 96 -32 160 -32 
        END BLOCKDEF
        BEGIN BLOCKDEF clk20
            TIMESTAMP 2009 5 24 21 36 47
            LINE N 320 32 384 32 
            LINE N 64 -96 0 -96 
            LINE N 320 -96 384 -96 
            LINE N 320 -32 384 -32 
            RECTANGLE N 64 -128 320 64 
        END BLOCKDEF
        BEGIN BLOCKDEF com_takt
            TIMESTAMP 2009 5 24 23 31 51
            LINE N 448 -144 512 -144 
            LINE N 64 -288 0 -288 
            LINE N 64 -240 0 -240 
            LINE N 448 -288 512 -288 
            RECTANGLE N 64 -320 448 -120 
            LINE N 448 -240 512 -240 
            LINE N 448 -192 512 -192 
        END BLOCKDEF
        BEGIN BLOCKDEF ftc
            TIMESTAMP 2000 1 1 10 10 10
            RECTANGLE N 64 -320 320 -64 
            LINE N 192 -32 64 -32 
            LINE N 192 -64 192 -32 
            LINE N 80 -128 64 -144 
            LINE N 64 -112 80 -128 
            LINE N 384 -256 320 -256 
            LINE N 0 -256 64 -256 
            LINE N 0 -32 64 -32 
            LINE N 0 -128 64 -128 
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
        BEGIN BLOCK XLXI_6 takt
            PIN RST_IN XLXN_93
            PIN CLKIN_IN clk
            PIN CLKIN_IBUFG_OUT tmp1
            PIN CLK0_OUT clk50
            PIN CLK2X_OUT clk100
        END BLOCK
        BEGIN BLOCK XLXI_4 f_null
            PIN fo XLXN_93
        END BLOCK
        BEGIN BLOCK XLXI_17 clk20
            PIN CLKIN_IN tmp1
            PIN CLKFX_OUT clk_20
            PIN CLK0_OUT
            PIN CLK180_OUT
        END BLOCK
        BEGIN BLOCK XLXI_18 com_takt
            PIN CLKIN_IN clk_20
            PIN RST_IN XLXN_93
            PIN CLK0_OUT
            PIN CLK270_OUT
            PIN CLK90_OUT
            PIN CLK180_OUT XLXN_116
        END BLOCK
        BEGIN BLOCK XLXI_19 ftc
            PIN C XLXN_116
            PIN CLR
            PIN T XLXN_97
            PIN Q g90_c10
        END BLOCK
        BEGIN BLOCK XLXI_20 inv
            PIN I XLXN_93
            PIN O XLXN_97
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        BEGIN BRANCH clk50
            WIRE 1152 656 1168 656
            WIRE 1168 656 1360 656
        END BRANCH
        BEGIN BRANCH tmp1
            WIRE 688 752 688 880
            WIRE 688 880 752 880
            WIRE 688 752 1232 752
            WIRE 1152 624 1232 624
            WIRE 1232 624 1232 752
        END BRANCH
        BEGIN BRANCH clk
            WIRE 640 656 816 656
        END BRANCH
        BEGIN INSTANCE XLXI_6 816 688 R0
        END INSTANCE
        BEGIN INSTANCE XLXI_17 752 976 R0
        END INSTANCE
        IOMARKER 640 656 clk R180 28
        IOMARKER 1456 816 clk_20 R0 28
        BEGIN BRANCH clk_20
            WIRE 784 1088 784 1280
            WIRE 784 1280 800 1280
            WIRE 784 1088 1312 1088
            WIRE 1136 880 1312 880
            WIRE 1312 880 1408 880
            WIRE 1312 880 1312 1088
            WIRE 1408 816 1456 816
            WIRE 1408 816 1408 880
        END BRANCH
        BEGIN INSTANCE XLXI_18 800 1520 R0
        END INSTANCE
        BEGIN INSTANCE XLXI_4 80 624 R0
        END INSTANCE
        BEGIN BRANCH XLXN_93
            WIRE 240 592 368 592
            WIRE 368 592 816 592
            WIRE 368 592 368 1232
            WIRE 368 1232 800 1232
            WIRE 368 1232 368 1456
            WIRE 368 1456 912 1456
        END BRANCH
        INSTANCE XLXI_19 1776 1584 R0
        INSTANCE XLXI_20 912 1488 R0
        BEGIN BRANCH XLXN_97
            WIRE 1136 1456 1200 1456
            WIRE 1200 1456 1200 1520
            WIRE 1200 1520 1728 1520
            WIRE 1728 1328 1776 1328
            WIRE 1728 1328 1728 1520
        END BRANCH
        BEGIN BRANCH g90_c10
            WIRE 2160 1328 2416 1328
        END BRANCH
        IOMARKER 2416 1328 g90_c10 R0 28
        BEGIN BRANCH XLXN_116
            WIRE 1312 1328 1536 1328
            WIRE 1536 1328 1536 1456
            WIRE 1536 1456 1776 1456
        END BRANCH
        IOMARKER 1360 656 clk50 R0 28
        BEGIN BRANCH clk100
            WIRE 1152 688 1168 688
            WIRE 1168 688 1360 688
        END BRANCH
        IOMARKER 1360 688 clk100 R0 28
    END SHEET
END SCHEMATIC
