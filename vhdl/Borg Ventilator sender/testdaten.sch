VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "spartan3"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL I1(7:0)
        SIGNAL I2(7:0)
        SIGNAL I3(7:0)
        SIGNAL i4(7:0)
        SIGNAL pattern(7:0)
        SIGNAL mux(7:0)
        SIGNAL trig
        SIGNAL q1
        SIGNAL q2
        SIGNAL q3
        SIGNAL q4
        SIGNAL clk20
        PORT Input I1(7:0)
        PORT Input I2(7:0)
        PORT Input I3(7:0)
        PORT Input i4(7:0)
        PORT Input pattern(7:0)
        PORT Input mux(7:0)
        PORT Output trig
        PORT Output q1
        PORT Output q2
        PORT Output q3
        PORT Output q4
        PORT Input clk20
        BEGIN BLOCKDEF tgb
            TIMESTAMP 2009 5 19 22 59 21
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
        BEGIN BLOCK XLXI_1 tgb
            PIN clk20
            PIN q1
            PIN a(7:0) I2(7:0)
            PIN b(7:0) I3(7:0)
            PIN c(7:0) i4(7:0)
            PIN d(7:0)
            PIN mux(7:0) I1(7:0)
            PIN pattern(7:0)
            PIN q2
            PIN q3
            PIN q4
            PIN trig
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        BEGIN INSTANCE XLXI_1 1072 1088 R0
        END INSTANCE
        BEGIN BRANCH I1(7:0)
            WIRE 1040 848 1072 848
        END BRANCH
        IOMARKER 1040 848 I1(7:0) R180 28
        BEGIN BRANCH I2(7:0)
            WIRE 1040 880 1072 880
        END BRANCH
        IOMARKER 1040 880 I2(7:0) R180 28
        BEGIN BRANCH I3(7:0)
            WIRE 1040 912 1072 912
        END BRANCH
        IOMARKER 1040 912 I3(7:0) R180 28
        BEGIN BRANCH i4(7:0)
            WIRE 1040 944 1072 944
        END BRANCH
        IOMARKER 1040 944 i4(7:0) R180 28
        BEGIN BRANCH pattern(7:0)
            WIRE 1040 1040 1072 1040
        END BRANCH
        IOMARKER 1040 1040 pattern(7:0) R180 28
        BEGIN BRANCH mux(7:0)
            WIRE 1040 1088 1072 1088
        END BRANCH
        IOMARKER 1040 1088 mux(7:0) R180 28
        BEGIN BRANCH trig
            WIRE 1408 992 1440 992
        END BRANCH
        IOMARKER 1440 992 trig R0 28
        BEGIN BRANCH q1
            WIRE 1408 800 1440 800
        END BRANCH
        IOMARKER 1440 800 q1 R0 28
        BEGIN BRANCH q2
            WIRE 1408 848 1440 848
        END BRANCH
        IOMARKER 1440 848 q2 R0 28
        BEGIN BRANCH q3
            WIRE 1408 896 1440 896
        END BRANCH
        IOMARKER 1440 896 q3 R0 28
        BEGIN BRANCH q4
            WIRE 1408 944 1440 944
        END BRANCH
        IOMARKER 1440 944 q4 R0 28
        BEGIN BRANCH clk20
            WIRE 1040 992 1072 992
        END BRANCH
        IOMARKER 1040 992 clk20 R180 28
    END SHEET
    BEGIN SHEET 2 3520 2720
    END SHEET
END SCHEMATIC
