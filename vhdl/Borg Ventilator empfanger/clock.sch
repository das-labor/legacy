VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "spartan3"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL clk
        SIGNAL XLXN_983
        SIGNAL in0b
        SIGNAL in0a
        SIGNAL XLXN_976
        SIGNAL clk200
        SIGNAL XLXN_985
        SIGNAL XLXN_1009
        SIGNAL com20
        SIGNAL clk_20
        SIGNAL clk_60
        PORT Input clk
        PORT Input in0b
        PORT Input in0a
        PORT Output clk_20
        PORT Output clk_60
        BEGIN BLOCKDEF takt
            TIMESTAMP 2009 5 14 21 51 8
            LINE N 64 -160 0 -160 
            LINE N 400 -160 464 -160 
            RECTANGLE N 64 -192 400 -44 
            LINE N 400 -128 464 -128 
            LINE N 400 -96 464 -96 
            LINE N 64 -96 0 -96 
        END BLOCKDEF
        BEGIN BLOCKDEF clk_200
            TIMESTAMP 2009 5 14 21 51 8
            RECTANGLE N 64 -192 368 -80 
            LINE N 64 -160 0 -160 
            LINE N 368 -160 432 -160 
            LINE N 368 -128 432 -128 
            LINE N 368 -96 432 -96 
            LINE N 64 -128 0 -128 
        END BLOCKDEF
        BEGIN BLOCKDEF f_null
            TIMESTAMP 2009 5 14 21 51 8
            RECTANGLE N 280 -48 320 -16 
            LINE N 320 -32 384 -32 
        END BLOCKDEF
        BEGIN BLOCKDEF ibufds
            TIMESTAMP 2000 1 1 10 10 10
            LINE N 0 -48 64 -48 
            LINE N 64 -64 128 -32 
            LINE N 128 -32 64 0 
            LINE N 64 0 64 -64 
            LINE N 224 -32 128 -32 
            CIRCLE N 48 -24 64 -8 
            LINE N 0 -16 48 -16 
        END BLOCKDEF
        BEGIN BLOCKDEF com_clock
            TIMESTAMP 2009 5 14 21 51 8
            RECTANGLE N 64 -276 320 -12 
            RECTANGLE N 320 -44 384 -20 
            LINE N 320 -32 384 -32 
            RECTANGLE N 320 -76 384 -52 
            LINE N 320 -64 384 -64 
            LINE N 320 -96 384 -96 
            LINE N 320 -128 384 -128 
            LINE N 320 -160 384 -160 
            LINE N 320 -192 384 -192 
            LINE N 64 -256 0 -256 
            LINE N 320 -256 384 -256 
            LINE N 64 -224 0 -224 
        END BLOCKDEF
        BEGIN BLOCKDEF bufg
            TIMESTAMP 2000 1 1 10 10 10
            LINE N 64 -64 64 0 
            LINE N 128 -32 64 -64 
            LINE N 64 0 128 -32 
            LINE N 224 -32 128 -32 
            LINE N 0 -32 64 -32 
        END BLOCKDEF
        BEGIN BLOCKDEF com_20
            TIMESTAMP 2009 5 14 21 51 8
            RECTANGLE N 64 -192 400 0 
            LINE N 64 -160 0 -160 
            LINE N 64 -32 0 -32 
            LINE N 400 -160 464 -160 
            LINE N 400 -32 464 -32 
        END BLOCKDEF
        BEGIN BLOCK XLXI_203 takt
            PIN RST_IN XLXN_983
            PIN CLKIN_IN clk
            PIN LOCKED_OUT
            PIN CLKIN_IBUFG_OUT XLXN_985
            PIN CLK0_OUT clk_60
        END BLOCK
        BEGIN BLOCK XLXI_277 clk_200
            PIN CLKIN_IN XLXN_985
            PIN LOCKED_OUT
            PIN CLKFX_OUT clk200
            PIN CLK0_OUT
            PIN RST_IN XLXN_983
        END BLOCK
        BEGIN BLOCK XLXI_208 f_null
            PIN fo XLXN_983
        END BLOCK
        BEGIN BLOCK XLXI_214 ibufds
            PIN I in0a
            PIN IB in0b
            PIN O XLXN_976
        END BLOCK
        BEGIN BLOCK XLXI_270 com_clock
            PIN diag_cf2(6:0)
            PIN diag_cf1(6:0)
            PIN diag_res2
            PIN diag_res1
            PIN diag_ff2
            PIN diag_ff1
            PIN com_clk XLXN_976
            PIN com_clk_2x XLXN_1009
            PIN MHZ_200 clk200
        END BLOCK
        BEGIN BLOCK XLXI_313 com_20
            PIN CLKIN_IN XLXN_1009
            PIN RST_IN XLXN_983
            PIN LOCKED_OUT
            PIN CLK0_OUT com20
        END BLOCK
        BEGIN BLOCK XLXI_287 bufg
            PIN I com20
            PIN O clk_20
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        BEGIN INSTANCE XLXI_203 1520 640 R0
        END INSTANCE
        BEGIN BRANCH clk
            WIRE 1488 544 1520 544
        END BRANCH
        BEGIN INSTANCE XLXI_277 1520 912 R0
        END INSTANCE
        BEGIN BRANCH XLXN_983
            WIRE 944 480 1104 480
            WIRE 1104 480 1104 784
            WIRE 1104 784 1520 784
            WIRE 1104 480 1520 480
            WIRE 1056 784 1104 784
            WIRE 1056 784 1056 1760
            WIRE 1056 1760 1488 1760
        END BRANCH
        INSTANCE XLXI_214 1264 1152 R0
        BEGIN BRANCH in0b
            WIRE 1200 1136 1264 1136
        END BRANCH
        BEGIN BRANCH in0a
            WIRE 1200 1104 1264 1104
        END BRANCH
        BEGIN BRANCH XLXN_976
            WIRE 1488 1120 1568 1120
        END BRANCH
        BEGIN INSTANCE XLXI_270 1568 1376 R0
        END INSTANCE
        BEGIN BRANCH clk200
            WIRE 1552 944 1552 1152
            WIRE 1552 1152 1568 1152
            WIRE 1552 944 2032 944
            WIRE 1952 784 2032 784
            WIRE 2032 784 2032 944
        END BRANCH
        BEGIN BRANCH XLXN_985
            WIRE 1472 640 1472 752
            WIRE 1472 752 1520 752
            WIRE 1472 640 2064 640
            WIRE 1984 512 2064 512
            WIRE 2064 512 2064 640
        END BRANCH
        IOMARKER 1488 544 clk R180 28
        IOMARKER 1200 1136 in0b R180 28
        IOMARKER 1200 1104 in0a R180 28
        BEGIN INSTANCE XLXI_208 560 512 R0
        END INSTANCE
        BEGIN INSTANCE XLXI_313 1488 1920 R0
        END INSTANCE
        BEGIN BRANCH com20
            WIRE 1952 1888 2176 1888
        END BRANCH
        INSTANCE XLXI_287 2176 1920 R0
        BEGIN BRANCH clk_20
            WIRE 2400 1888 2432 1888
        END BRANCH
        IOMARKER 2432 1888 clk_20 R0 28
        BEGIN BRANCH XLXN_1009
            WIRE 1408 1888 1488 1888
            WIRE 1408 1888 1408 2000
            WIRE 1408 2000 2064 2000
            WIRE 1952 1120 2064 1120
            WIRE 2064 1120 2064 2000
        END BRANCH
        BEGIN BRANCH clk_60
            WIRE 1984 544 2240 544
        END BRANCH
        IOMARKER 2240 544 clk_60 R0 28
    END SHEET
END SCHEMATIC
