VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "spartan3"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL i2
        SIGNAL o2a
        SIGNAL o3a
        SIGNAL i4
        SIGNAL o4a
        SIGNAL trig
        SIGNAL trib_o
        SIGNAL i1
        SIGNAL o1a
        SIGNAL clk
        SIGNAL o1b
        SIGNAL XLXN_3
        SIGNAL o2b
        SIGNAL XLXN_6
        SIGNAL o3b
        SIGNAL XLXN_9
        SIGNAL o4b
        SIGNAL XLXN_11
        SIGNAL i3
        PORT Input i2
        PORT Output o2a
        PORT Output o3a
        PORT Input i4
        PORT Output o4a
        PORT Input trig
        PORT Output trib_o
        PORT Input i1
        PORT Output o1a
        PORT Input clk
        PORT Output o1b
        PORT Output o2b
        PORT Output o3b
        PORT Output o4b
        PORT Input i3
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
        BEGIN BLOCK XLXI_1 ofd
            PIN C clk
            PIN D i2
            PIN Q o2a
        END BLOCK
        BEGIN BLOCK XLXI_2 ofd
            PIN C clk
            PIN D i3
            PIN Q o3a
        END BLOCK
        BEGIN BLOCK XLXI_3 ofd
            PIN C clk
            PIN D i4
            PIN Q o4a
        END BLOCK
        BEGIN BLOCK XLXI_4 ofd
            PIN C clk
            PIN D trig
            PIN Q trib_o
        END BLOCK
        BEGIN BLOCK XLXI_5 ofd
            PIN C clk
            PIN D i1
            PIN Q o1a
        END BLOCK
        BEGIN BLOCK XLXI_7 inv
            PIN I i1
            PIN O XLXN_3
        END BLOCK
        BEGIN BLOCK XLXI_6 ofd
            PIN C clk
            PIN D XLXN_3
            PIN Q o1b
        END BLOCK
        BEGIN BLOCK XLXI_9 ofd
            PIN C clk
            PIN D XLXN_6
            PIN Q o2b
        END BLOCK
        BEGIN BLOCK XLXI_10 inv
            PIN I i2
            PIN O XLXN_6
        END BLOCK
        BEGIN BLOCK XLXI_11 ofd
            PIN C clk
            PIN D XLXN_9
            PIN Q o3b
        END BLOCK
        BEGIN BLOCK XLXI_12 inv
            PIN I i3
            PIN O XLXN_9
        END BLOCK
        BEGIN BLOCK XLXI_13 ofd
            PIN C clk
            PIN D XLXN_11
            PIN Q o4b
        END BLOCK
        BEGIN BLOCK XLXI_14 inv
            PIN I i4
            PIN O XLXN_11
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        INSTANCE XLXI_1 1680 1120 R0
        BEGIN BRANCH i2
            WIRE 1392 864 1600 864
            WIRE 1600 864 1680 864
            WIRE 1600 736 1600 864
            WIRE 1600 736 2192 736
            WIRE 2192 736 2192 944
            WIRE 2192 944 2352 944
        END BRANCH
        BEGIN BRANCH o2a
            WIRE 2064 864 2096 864
        END BRANCH
        INSTANCE XLXI_2 1696 1504 R0
        BEGIN BRANCH o3a
            WIRE 2080 1248 2112 1248
        END BRANCH
        INSTANCE XLXI_3 1712 1888 R0
        BEGIN BRANCH i4
            WIRE 1392 1632 1632 1632
            WIRE 1632 1632 1632 1888
            WIRE 1632 1888 2256 1888
            WIRE 1632 1632 1712 1632
            WIRE 2256 1616 2256 1888
            WIRE 2256 1616 2384 1616
        END BRANCH
        BEGIN BRANCH o4a
            WIRE 2096 1632 2128 1632
        END BRANCH
        INSTANCE XLXI_4 1728 2272 R0
        BEGIN BRANCH trib_o
            WIRE 2112 2016 2144 2016
        END BRANCH
        INSTANCE XLXI_5 1664 736 R0
        BEGIN BRANCH i1
            WIRE 1232 480 1456 480
            WIRE 1456 480 1664 480
            WIRE 1456 352 1456 480
            WIRE 1456 352 2224 352
            WIRE 2224 352 2224 496
            WIRE 2224 496 2336 496
        END BRANCH
        BEGIN BRANCH o1a
            WIRE 2048 480 2080 480
        END BRANCH
        IOMARKER 2096 864 o2a R0 28
        IOMARKER 2112 1248 o3a R0 28
        IOMARKER 2128 1632 o4a R0 28
        IOMARKER 2144 2016 trib_o R0 28
        IOMARKER 2080 480 o1a R0 28
        IOMARKER 1488 608 clk R180 28
        IOMARKER 1232 480 i1 R180 28
        INSTANCE XLXI_6 2624 752 R0
        BEGIN BRANCH o1b
            WIRE 3008 496 3040 496
        END BRANCH
        IOMARKER 3040 496 o1b R0 28
        INSTANCE XLXI_7 2336 528 R0
        BEGIN BRANCH XLXN_3
            WIRE 2560 496 2624 496
        END BRANCH
        INSTANCE XLXI_9 2640 1200 R0
        BEGIN BRANCH o2b
            WIRE 3024 944 3056 944
        END BRANCH
        INSTANCE XLXI_10 2352 976 R0
        BEGIN BRANCH XLXN_6
            WIRE 2576 944 2640 944
        END BRANCH
        IOMARKER 3056 944 o2b R0 28
        IOMARKER 1392 864 i2 R180 28
        INSTANCE XLXI_11 2656 1552 R0
        BEGIN BRANCH o3b
            WIRE 3040 1296 3072 1296
        END BRANCH
        INSTANCE XLXI_12 2368 1328 R0
        BEGIN BRANCH XLXN_9
            WIRE 2592 1296 2656 1296
        END BRANCH
        IOMARKER 3072 1296 o3b R0 28
        INSTANCE XLXI_13 2672 1872 R0
        BEGIN BRANCH o4b
            WIRE 3056 1616 3088 1616
        END BRANCH
        INSTANCE XLXI_14 2384 1648 R0
        BEGIN BRANCH XLXN_11
            WIRE 2608 1616 2672 1616
        END BRANCH
        IOMARKER 3088 1616 o4b R0 28
        BEGIN BRANCH i3
            WIRE 1392 1248 1616 1248
            WIRE 1616 1248 1696 1248
            WIRE 1616 1104 1616 1248
            WIRE 1616 1104 2224 1104
            WIRE 2224 1104 2224 1296
            WIRE 2224 1296 2368 1296
        END BRANCH
        IOMARKER 1392 1248 i3 R180 28
        IOMARKER 1392 1632 i4 R180 28
        IOMARKER 1424 2016 trig R180 28
        BEGIN BRANCH clk
            WIRE 1488 608 1568 608
            WIRE 1568 608 1664 608
            WIRE 1568 608 1568 704
            WIRE 1568 704 1568 992
            WIRE 1568 992 1680 992
            WIRE 1568 992 1568 1376
            WIRE 1568 1376 1696 1376
            WIRE 1568 1376 1568 1760
            WIRE 1568 1760 1712 1760
            WIRE 1568 1760 1568 2144
            WIRE 1568 2144 1728 2144
            WIRE 1568 704 2096 704
            WIRE 2096 624 2096 704
            WIRE 2096 624 2560 624
            WIRE 2560 624 2624 624
            WIRE 2560 624 2560 848
            WIRE 2560 848 2624 848
            WIRE 2624 848 2624 1072
            WIRE 2624 1072 2640 1072
            WIRE 2624 1072 2624 1424
            WIRE 2624 1424 2640 1424
            WIRE 2640 1424 2656 1424
            WIRE 2640 1424 2640 1744
            WIRE 2640 1744 2672 1744
        END BRANCH
        BEGIN BRANCH trig
            WIRE 1424 2016 1728 2016
        END BRANCH
    END SHEET
END SCHEMATIC
