VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "spartan3"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL i2
        SIGNAL o2
        SIGNAL i3
        SIGNAL o3
        SIGNAL i4
        SIGNAL o4
        SIGNAL i5
        SIGNAL o5
        SIGNAL i1
        SIGNAL o1
        SIGNAL clk
        PORT Input i2
        PORT Output o2
        PORT Input i3
        PORT Output o3
        PORT Input i4
        PORT Output o4
        PORT Input i5
        PORT Output o5
        PORT Input i1
        PORT Output o1
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
        BEGIN BLOCK XLXI_1 ofd
            PIN C clk
            PIN D i2
            PIN Q o2
        END BLOCK
        BEGIN BLOCK XLXI_2 ofd
            PIN C clk
            PIN D i3
            PIN Q o3
        END BLOCK
        BEGIN BLOCK XLXI_3 ofd
            PIN C clk
            PIN D i4
            PIN Q o4
        END BLOCK
        BEGIN BLOCK XLXI_4 ofd
            PIN C clk
            PIN D i5
            PIN Q o5
        END BLOCK
        BEGIN BLOCK XLXI_5 ofd
            PIN C clk
            PIN D i1
            PIN Q o1
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        INSTANCE XLXI_1 1680 1120 R0
        BEGIN BRANCH i2
            WIRE 1648 864 1680 864
        END BRANCH
        BEGIN BRANCH o2
            WIRE 2064 864 2096 864
        END BRANCH
        INSTANCE XLXI_2 1696 1504 R0
        BEGIN BRANCH i3
            WIRE 1664 1248 1696 1248
        END BRANCH
        BEGIN BRANCH o3
            WIRE 2080 1248 2112 1248
        END BRANCH
        INSTANCE XLXI_3 1712 1888 R0
        BEGIN BRANCH i4
            WIRE 1680 1632 1712 1632
        END BRANCH
        BEGIN BRANCH o4
            WIRE 2096 1632 2128 1632
        END BRANCH
        INSTANCE XLXI_4 1728 2272 R0
        BEGIN BRANCH i5
            WIRE 1696 2016 1728 2016
        END BRANCH
        BEGIN BRANCH o5
            WIRE 2112 2016 2144 2016
        END BRANCH
        INSTANCE XLXI_5 1664 736 R0
        BEGIN BRANCH i1
            WIRE 1632 480 1664 480
        END BRANCH
        BEGIN BRANCH o1
            WIRE 2048 480 2080 480
        END BRANCH
        BEGIN BRANCH clk
            WIRE 1488 608 1568 608
            WIRE 1568 608 1664 608
            WIRE 1568 608 1568 992
            WIRE 1568 992 1680 992
            WIRE 1568 992 1568 1376
            WIRE 1568 1376 1696 1376
            WIRE 1568 1376 1568 1760
            WIRE 1568 1760 1712 1760
            WIRE 1568 1760 1568 2144
            WIRE 1568 2144 1728 2144
        END BRANCH
        IOMARKER 1648 864 i2 R180 28
        IOMARKER 2096 864 o2 R0 28
        IOMARKER 1664 1248 i3 R180 28
        IOMARKER 2112 1248 o3 R0 28
        IOMARKER 1680 1632 i4 R180 28
        IOMARKER 2128 1632 o4 R0 28
        IOMARKER 1696 2016 i5 R180 28
        IOMARKER 2144 2016 o5 R0 28
        IOMARKER 1632 480 i1 R180 28
        IOMARKER 2080 480 o1 R0 28
        IOMARKER 1488 608 clk R180 28
    END SHEET
END SCHEMATIC
