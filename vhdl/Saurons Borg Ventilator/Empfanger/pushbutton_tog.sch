VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "spartan3"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL clk
        SIGNAL tog
        SIGNAL XLXN_21
        SIGNAL imp
        SIGNAL tast
        SIGNAL XLXN_25
        SIGNAL XLXN_26
        SIGNAL XLXN_27
        PORT Input clk
        PORT Output tog
        PORT Output imp
        PORT Input tast
        BEGIN BLOCKDEF cb16ce
            TIMESTAMP 2000 1 1 10 10 10
            LINE N 384 -192 320 -192 
            RECTANGLE N 320 -268 384 -244 
            LINE N 384 -256 320 -256 
            LINE N 0 -192 64 -192 
            LINE N 192 -32 64 -32 
            LINE N 192 -64 192 -32 
            LINE N 80 -128 64 -144 
            LINE N 64 -112 80 -128 
            LINE N 0 -128 64 -128 
            LINE N 0 -32 64 -32 
            LINE N 384 -128 320 -128 
            RECTANGLE N 64 -320 320 -64 
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
        BEGIN BLOCKDEF f_null
            TIMESTAMP 2009 6 29 11 32 51
            RECTANGLE N 280 -48 320 -16 
            LINE N 320 -32 384 -32 
        END BLOCKDEF
        BEGIN BLOCKDEF fjkc
            TIMESTAMP 2000 1 1 10 10 10
            LINE N 0 -128 64 -128 
            LINE N 0 -32 64 -32 
            LINE N 0 -320 64 -320 
            LINE N 384 -256 320 -256 
            LINE N 0 -256 64 -256 
            LINE N 192 -32 64 -32 
            LINE N 192 -64 192 -32 
            LINE N 80 -128 64 -144 
            LINE N 64 -112 80 -128 
            RECTANGLE N 64 -384 320 -64 
        END BLOCKDEF
        BEGIN BLOCKDEF f_eins
            TIMESTAMP 2009 6 29 11 32 51
            RECTANGLE N 192 -64 320 0 
            LINE N 320 -32 384 -32 
        END BLOCKDEF
        BEGIN BLOCK XLXI_1 cb16ce
            PIN C clk
            PIN CE tast
            PIN CLR XLXN_26
            PIN CEO
            PIN Q(15:0)
            PIN TC XLXN_27
        END BLOCK
        BEGIN BLOCK XLXI_2 inv
            PIN I tast
            PIN O XLXN_26
        END BLOCK
        BEGIN BLOCK XLXI_4 f_null
            PIN fo XLXN_25
        END BLOCK
        BEGIN BLOCK XLXI_5 fjkc
            PIN C clk
            PIN CLR XLXN_25
            PIN J XLXN_27
            PIN K XLXN_26
            PIN Q imp
        END BLOCK
        BEGIN BLOCK XLXI_6 ftc
            PIN C imp
            PIN CLR XLXN_25
            PIN T XLXN_21
            PIN Q tog
        END BLOCK
        BEGIN BLOCK XLXI_7 f_eins
            PIN f1 XLXN_21
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        INSTANCE XLXI_1 1040 1072 R0
        IOMARKER 576 880 tast R180 28
        BEGIN BRANCH clk
            WIRE 576 944 688 944
            WIRE 688 944 1040 944
            WIRE 688 944 688 1184
            WIRE 688 1184 1792 1184
        END BRANCH
        IOMARKER 576 944 clk R180 28
        BEGIN BRANCH tog
            WIRE 2720 1056 2736 1056
            WIRE 2736 1056 3040 1056
        END BRANCH
        INSTANCE XLXI_2 752 1072 R0
        BEGIN INSTANCE XLXI_4 1072 1392 R0
        END INSTANCE
        IOMARKER 3040 1056 tog R0 28
        INSTANCE XLXI_6 2336 1312 R0
        BEGIN INSTANCE XLXI_7 1792 736 R0
        END INSTANCE
        BEGIN BRANCH XLXN_21
            WIRE 2176 704 2256 704
            WIRE 2256 704 2256 1056
            WIRE 2256 1056 2336 1056
        END BRANCH
        BEGIN BRANCH imp
            WIRE 2176 1056 2208 1056
            WIRE 2208 1056 2208 1184
            WIRE 2208 1184 2336 1184
            WIRE 2208 1184 2208 1792
            WIRE 2208 1792 2944 1792
        END BRANCH
        IOMARKER 2944 1792 imp R0 28
        BEGIN BRANCH tast
            WIRE 576 880 736 880
            WIRE 736 880 736 1040
            WIRE 736 1040 752 1040
            WIRE 736 880 1040 880
        END BRANCH
        BEGIN BRANCH XLXN_25
            WIRE 1456 1360 1792 1360
            WIRE 1792 1360 2336 1360
            WIRE 1792 1280 1792 1360
            WIRE 2336 1280 2336 1360
        END BRANCH
        BEGIN BRANCH XLXN_26
            WIRE 976 1040 1024 1040
            WIRE 1024 1040 1040 1040
            WIRE 1024 1040 1024 1056
            WIRE 1024 1056 1504 1056
            WIRE 1504 1056 1792 1056
        END BRANCH
        INSTANCE XLXI_5 1792 1312 R0
        BEGIN BRANCH XLXN_27
            WIRE 1424 944 1600 944
            WIRE 1600 944 1600 992
            WIRE 1600 992 1792 992
        END BRANCH
    END SHEET
END SCHEMATIC
