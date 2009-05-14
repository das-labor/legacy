VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "spartan3"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL XLXN_4(7:0)
        SIGNAL XLXN_6(7:0)
        SIGNAL XLXN_11
        SIGNAL XLXN_13
        SIGNAL XLXN_16(7:0)
        SIGNAL XLXN_18(7:0)
        SIGNAL XLXN_20(7:0)
        SIGNAL XLXN_25
        SIGNAL XLXN_30
        SIGNAL XLXN_32
        SIGNAL XLXN_34
        SIGNAL XLXN_37
        SIGNAL XLXN_48
        SIGNAL XLXN_50
        SIGNAL XLXN_1010
        SIGNAL XLXN_1011
        SIGNAL XLXN_1012
        SIGNAL XLXN_1013(7:0)
        PORT Input XLXN_1010
        PORT Input XLXN_1011
        PORT Output XLXN_1012
        PORT Output XLXN_1013(7:0)
        BEGIN BLOCKDEF Picoblaze_e
            TIMESTAMP 2009 5 7 4 8 41
            LINE N 64 -272 0 -272 
            LINE N 320 -272 384 -272 
            LINE N 64 -240 0 -240 
            LINE N 320 -240 384 -240 
            RECTANGLE N 320 -156 384 -132 
            LINE N 320 -144 384 -144 
            RECTANGLE N 320 -188 384 -164 
            LINE N 320 -176 384 -176 
            RECTANGLE N 68 -300 320 344 
            LINE N 64 -176 0 -176 
            LINE N 64 -80 0 -80 
            LINE N 64 -48 0 -48 
            LINE N 64 -16 0 -16 
            LINE N 64 16 0 16 
            LINE N 64 48 0 48 
            LINE N 64 80 0 80 
            LINE N 64 112 0 112 
            LINE N 64 144 0 144 
            LINE N 64 176 0 176 
            LINE N 64 208 0 208 
            LINE N 64 240 0 240 
            LINE N 64 272 0 272 
            LINE N 64 304 0 304 
            LINE N 64 -112 0 -112 
            LINE N 64 -144 0 -144 
            RECTANGLE N 320 260 384 284 
            LINE N 320 272 384 272 
            RECTANGLE N 320 212 384 236 
            LINE N 320 224 384 224 
            LINE N 320 320 384 320 
        END BLOCKDEF
        BEGIN BLOCK XLXI_317 Picoblaze_e
            PIN clk
            PIN irq
            PIN wr_str
            PIN irq_ack XLXN_1012
            PIN p_id(7:0) XLXN_1013(7:0)
            PIN out_p(7:0)
            PIN in0
            PIN in3
            PIN in4
            PIN in5 XLXN_1010
            PIN in6
            PIN in7 XLXN_1011
            PIN in8
            PIN in9
            PIN in10
            PIN in11
            PIN in12
            PIN in13
            PIN in14
            PIN in15
            PIN in2
            PIN in1
            PIN q2(7:0)
            PIN q3(7:0)
            PIN en_ram
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        BEGIN INSTANCE XLXI_317 1648 1296 R0
        END INSTANCE
        BEGIN BRANCH XLXN_1010
            WIRE 1616 1280 1648 1280
        END BRANCH
        IOMARKER 1616 1280 XLXN_1010 R180 28
        BEGIN BRANCH XLXN_1011
            WIRE 1616 1344 1648 1344
        END BRANCH
        IOMARKER 1616 1344 XLXN_1011 R180 28
        BEGIN BRANCH XLXN_1012
            WIRE 2032 1056 2064 1056
        END BRANCH
        IOMARKER 2064 1056 XLXN_1012 R0 28
        BEGIN BRANCH XLXN_1013(7:0)
            WIRE 2032 1120 2064 1120
        END BRANCH
        IOMARKER 2064 1120 XLXN_1013(7:0) R0 28
    END SHEET
END SCHEMATIC
