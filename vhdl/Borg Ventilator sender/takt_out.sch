VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "spartan3"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL i1
        SIGNAL o1a
        SIGNAL o1b
        SIGNAL XLXN_3
        SIGNAL clk
        PORT Input i1
        PORT Output o1a
        PORT Output o1b
        PORT Input clk
        BEGIN BLOCKDEF ofd
            TIMESTAMP 2000 1 1 10 10 10
            RECTANGLE N 64 -320 320 -64 
            LINE N 84 -128 64 -140 
            LINE N 64 -116 84 -128 
            LINE N 384 -256 320 -256 
            LINE N 0 -256 64 -256 
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
        BEGIN BLOCK XLXI_5 ofd
            PIN C clk
            PIN D i1
            PIN Q o1a
        END BLOCK
        BEGIN BLOCK XLXI_6 ofd
            PIN C clk
            PIN D XLXN_3
            PIN Q o1b
        END BLOCK
        BEGIN BLOCK XLXI_7 inv
            PIN I i1
            PIN O XLXN_3
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        INSTANCE XLXI_5 1184 688 R0
        BEGIN BRANCH i1
            WIRE 752 432 976 432
            WIRE 976 432 1184 432
            WIRE 976 304 976 432
            WIRE 976 304 1744 304
            WIRE 1744 304 1744 448
            WIRE 1744 448 1856 448
        END BRANCH
        BEGIN BRANCH o1a
            WIRE 1568 432 1600 432
        END BRANCH
        INSTANCE XLXI_6 2144 704 R0
        BEGIN BRANCH o1b
            WIRE 2528 448 2560 448
        END BRANCH
        INSTANCE XLXI_7 1856 480 R0
        BEGIN BRANCH XLXN_3
            WIRE 2080 448 2144 448
        END BRANCH
        IOMARKER 1600 432 o1a R0 28
        IOMARKER 1008 560 clk R180 28
        IOMARKER 752 432 i1 R180 28
        IOMARKER 2560 448 o1b R0 28
        BEGIN BRANCH clk
            WIRE 1008 560 1088 560
            WIRE 1088 560 1184 560
            WIRE 1088 560 1088 656
            WIRE 1088 656 1616 656
            WIRE 1616 576 1616 656
            WIRE 1616 576 2080 576
            WIRE 2080 576 2144 576
        END BRANCH
    END SHEET
END SCHEMATIC
