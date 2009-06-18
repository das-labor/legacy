VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "spartan3"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL winkel(9:0)
        SIGNAL sin(8:0)
        SIGNAL cos(8:0)
        SIGNAL XLXN_1(17:0)
        SIGNAL XLXN_2(17:0)
        SIGNAL XLXN_3(35:0)
        PORT Input winkel(9:0)
        PORT Output sin(8:0)
        PORT Output cos(8:0)
        PORT Input XLXN_1(17:0)
        PORT Input XLXN_2(17:0)
        PORT Output XLXN_3(35:0)
        BEGIN BLOCKDEF sinus
            TIMESTAMP 2009 6 7 20 24 4
            RECTANGLE N 32 0 256 208 
            BEGIN LINE W 0 48 32 48 
            END LINE
            BEGIN LINE W 256 48 288 48 
            END LINE
            BEGIN LINE W 256 80 288 80 
            END LINE
        END BLOCKDEF
        BEGIN BLOCKDEF mult18x18
            TIMESTAMP 2000 1 1 10 10 10
            RECTANGLE N 64 -224 416 -32 
            RECTANGLE N 416 -140 480 -116 
            RECTANGLE N 0 -172 64 -148 
            RECTANGLE N 0 -108 64 -84 
            LINE N 480 -128 416 -128 
            LINE N 0 -160 64 -160 
            LINE N 0 -96 64 -96 
        END BLOCKDEF
        BEGIN BLOCK XLXI_1 sinus
            PIN THETA(9:0) winkel(9:0)
            PIN SINE(8:0) sin(8:0)
            PIN COSINE(8:0) cos(8:0)
        END BLOCK
        BEGIN BLOCK XLXI_3 mult18x18
            PIN A(17:0) XLXN_1(17:0)
            PIN B(17:0) XLXN_2(17:0)
            PIN P(35:0) XLXN_3(35:0)
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        BEGIN BRANCH winkel(9:0)
            WIRE 1328 1216 1360 1216
        END BRANCH
        IOMARKER 1328 1216 winkel(9:0) R180 28
        BEGIN BRANCH sin(8:0)
            WIRE 1648 1216 1680 1216
        END BRANCH
        IOMARKER 1680 1216 sin(8:0) R0 28
        BEGIN BRANCH cos(8:0)
            WIRE 1648 1248 1680 1248
        END BRANCH
        IOMARKER 1680 1248 cos(8:0) R0 28
        BEGIN INSTANCE XLXI_1 1360 1168 R0
        END INSTANCE
        INSTANCE XLXI_3 1232 1056 R0
        BEGIN BRANCH XLXN_1(17:0)
            WIRE 1200 896 1232 896
        END BRANCH
        IOMARKER 1200 896 XLXN_1(17:0) R180 28
        BEGIN BRANCH XLXN_2(17:0)
            WIRE 1200 960 1232 960
        END BRANCH
        IOMARKER 1200 960 XLXN_2(17:0) R180 28
        BEGIN BRANCH XLXN_3(35:0)
            WIRE 1712 928 1744 928
        END BRANCH
        IOMARKER 1744 928 XLXN_3(35:0) R0 28
    END SHEET
END SCHEMATIC
