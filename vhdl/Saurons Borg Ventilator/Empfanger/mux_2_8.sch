VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "spartan3"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL a(7:0)
        SIGNAL a(0)
        SIGNAL a(1)
        SIGNAL a(2)
        SIGNAL a(3)
        SIGNAL a(4)
        SIGNAL a(5)
        SIGNAL a(6)
        SIGNAL a(7)
        SIGNAL b(7:0)
        SIGNAL b(0)
        SIGNAL b(1)
        SIGNAL b(2)
        SIGNAL b(3)
        SIGNAL b(4)
        SIGNAL b(5)
        SIGNAL b(6)
        SIGNAL b(7)
        SIGNAL mux
        SIGNAL q(7:0)
        SIGNAL q(0)
        SIGNAL q(1)
        SIGNAL q(2)
        SIGNAL q(3)
        SIGNAL q(4)
        SIGNAL q(5)
        SIGNAL q(6)
        SIGNAL q(7)
        PORT Input a(7:0)
        PORT Input b(7:0)
        PORT Input mux
        PORT Output q(7:0)
        BEGIN BLOCKDEF m2_1
            TIMESTAMP 2000 1 1 10 10 10
            LINE N 96 -64 96 -192 
            LINE N 256 -96 96 -64 
            LINE N 256 -160 256 -96 
            LINE N 96 -192 256 -160 
            LINE N 176 -32 96 -32 
            LINE N 176 -80 176 -32 
            LINE N 0 -32 96 -32 
            LINE N 320 -128 256 -128 
            LINE N 0 -96 96 -96 
            LINE N 0 -160 96 -160 
        END BLOCKDEF
        BEGIN BLOCK XLXI_38 m2_1
            PIN D0 a(0)
            PIN D1 b(0)
            PIN S0 mux
            PIN O q(0)
        END BLOCK
        BEGIN BLOCK XLXI_39 m2_1
            PIN D0 a(1)
            PIN D1 b(1)
            PIN S0 mux
            PIN O q(1)
        END BLOCK
        BEGIN BLOCK XLXI_40 m2_1
            PIN D0 a(2)
            PIN D1 b(2)
            PIN S0 mux
            PIN O q(2)
        END BLOCK
        BEGIN BLOCK XLXI_41 m2_1
            PIN D0 a(3)
            PIN D1 b(3)
            PIN S0 mux
            PIN O q(3)
        END BLOCK
        BEGIN BLOCK XLXI_42 m2_1
            PIN D0 a(4)
            PIN D1 b(4)
            PIN S0 mux
            PIN O q(4)
        END BLOCK
        BEGIN BLOCK XLXI_43 m2_1
            PIN D0 a(5)
            PIN D1 b(5)
            PIN S0 mux
            PIN O q(5)
        END BLOCK
        BEGIN BLOCK XLXI_45 m2_1
            PIN D0 a(6)
            PIN D1 b(6)
            PIN S0 mux
            PIN O q(6)
        END BLOCK
        BEGIN BLOCK XLXI_44 m2_1
            PIN D0 a(7)
            PIN D1 b(7)
            PIN S0 mux
            PIN O q(7)
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        INSTANCE XLXI_38 1456 256 R0
        INSTANCE XLXI_39 1456 416 R0
        INSTANCE XLXI_40 1456 576 R0
        INSTANCE XLXI_41 1456 736 R0
        INSTANCE XLXI_42 1456 896 R0
        INSTANCE XLXI_43 1456 1056 R0
        INSTANCE XLXI_45 1456 1216 R0
        INSTANCE XLXI_44 1456 1376 R0
        BEGIN BRANCH a(7:0)
            WIRE 464 32 480 32
            WIRE 480 32 480 96
            WIRE 480 96 480 256
            WIRE 480 256 480 416
            WIRE 480 416 480 576
            WIRE 480 576 480 736
            WIRE 480 736 480 896
            WIRE 480 896 480 1056
            WIRE 480 1056 480 1216
            WIRE 480 1216 480 1264
        END BRANCH
        IOMARKER 464 32 a(7:0) R180 28
        BUSTAP 480 96 576 96
        BEGIN BRANCH a(0)
            WIRE 576 96 832 96
            WIRE 832 96 1456 96
            BEGIN DISPLAY 832 96 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 480 256 576 256
        BEGIN BRANCH a(1)
            WIRE 576 256 816 256
            WIRE 816 256 1456 256
            BEGIN DISPLAY 816 256 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 480 416 576 416
        BEGIN BRANCH a(2)
            WIRE 576 416 816 416
            WIRE 816 416 1456 416
            BEGIN DISPLAY 816 416 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 480 576 576 576
        BEGIN BRANCH a(3)
            WIRE 576 576 816 576
            WIRE 816 576 1456 576
            BEGIN DISPLAY 816 576 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 480 736 576 736
        BEGIN BRANCH a(4)
            WIRE 576 736 816 736
            WIRE 816 736 1456 736
            BEGIN DISPLAY 816 736 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 480 896 576 896
        BEGIN BRANCH a(5)
            WIRE 576 896 816 896
            WIRE 816 896 1456 896
            BEGIN DISPLAY 816 896 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 480 1056 576 1056
        BEGIN BRANCH a(6)
            WIRE 576 1056 816 1056
            WIRE 816 1056 1456 1056
            BEGIN DISPLAY 816 1056 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 480 1216 576 1216
        BEGIN BRANCH a(7)
            WIRE 576 1216 832 1216
            WIRE 832 1216 1456 1216
            BEGIN DISPLAY 832 1216 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH b(7:0)
            WIRE 720 16 880 16
            WIRE 880 16 880 160
            WIRE 880 160 880 320
            WIRE 880 320 880 480
            WIRE 880 480 880 640
            WIRE 880 640 880 800
            WIRE 880 800 880 960
            WIRE 880 960 880 1120
            WIRE 880 1120 880 1280
            WIRE 880 1280 880 1376
        END BRANCH
        IOMARKER 720 16 b(7:0) R180 28
        BUSTAP 880 160 976 160
        BEGIN BRANCH b(0)
            WIRE 976 160 1024 160
            WIRE 1024 160 1456 160
            BEGIN DISPLAY 1024 160 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 880 320 976 320
        BEGIN BRANCH b(1)
            WIRE 976 320 1024 320
            WIRE 1024 320 1456 320
            BEGIN DISPLAY 1024 320 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 880 480 976 480
        BEGIN BRANCH b(2)
            WIRE 976 480 1024 480
            WIRE 1024 480 1456 480
            BEGIN DISPLAY 1024 480 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 880 640 976 640
        BEGIN BRANCH b(3)
            WIRE 976 640 1024 640
            WIRE 1024 640 1456 640
            BEGIN DISPLAY 1024 640 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 880 800 976 800
        BEGIN BRANCH b(4)
            WIRE 976 800 1008 800
            WIRE 1008 800 1456 800
            BEGIN DISPLAY 1008 800 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 880 960 976 960
        BEGIN BRANCH b(5)
            WIRE 976 960 1008 960
            WIRE 1008 960 1456 960
            BEGIN DISPLAY 1008 960 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 880 1120 976 1120
        BEGIN BRANCH b(6)
            WIRE 976 1120 1008 1120
            WIRE 1008 1120 1456 1120
            BEGIN DISPLAY 1008 1120 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 880 1280 976 1280
        BEGIN BRANCH b(7)
            WIRE 976 1280 1008 1280
            WIRE 1008 1280 1456 1280
            BEGIN DISPLAY 1008 1280 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BEGIN BRANCH mux
            WIRE 1264 224 1328 224
            WIRE 1328 224 1456 224
            WIRE 1328 224 1328 384
            WIRE 1328 384 1456 384
            WIRE 1328 384 1328 544
            WIRE 1328 544 1456 544
            WIRE 1328 544 1328 704
            WIRE 1328 704 1456 704
            WIRE 1328 704 1328 864
            WIRE 1328 864 1456 864
            WIRE 1328 864 1328 1024
            WIRE 1328 1024 1456 1024
            WIRE 1328 1024 1328 1184
            WIRE 1328 1184 1456 1184
            WIRE 1328 1184 1328 1344
            WIRE 1328 1344 1456 1344
        END BRANCH
        IOMARKER 1264 224 mux R180 28
        BEGIN BRANCH q(7:0)
            WIRE 2416 32 2528 32
            WIRE 2416 32 2416 128
            WIRE 2416 128 2416 288
            WIRE 2416 288 2416 448
            WIRE 2416 448 2416 608
            WIRE 2416 608 2416 768
            WIRE 2416 768 2416 928
            WIRE 2416 928 2416 1088
            WIRE 2416 1088 2416 1248
            WIRE 2416 1248 2416 1312
        END BRANCH
        IOMARKER 2528 32 q(7:0) R0 28
        BUSTAP 2416 128 2320 128
        BEGIN BRANCH q(0)
            WIRE 1776 128 2064 128
            WIRE 2064 128 2320 128
            BEGIN DISPLAY 2064 128 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 2416 288 2320 288
        BEGIN BRANCH q(1)
            WIRE 1776 288 2032 288
            WIRE 2032 288 2320 288
            BEGIN DISPLAY 2032 288 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 2416 448 2320 448
        BEGIN BRANCH q(2)
            WIRE 1776 448 2032 448
            WIRE 2032 448 2320 448
            BEGIN DISPLAY 2032 448 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 2416 608 2320 608
        BEGIN BRANCH q(3)
            WIRE 1776 608 2048 608
            WIRE 2048 608 2320 608
            BEGIN DISPLAY 2048 608 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 2416 768 2320 768
        BEGIN BRANCH q(4)
            WIRE 1776 768 2032 768
            WIRE 2032 768 2320 768
            BEGIN DISPLAY 2032 768 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 2416 928 2320 928
        BEGIN BRANCH q(5)
            WIRE 1776 928 2032 928
            WIRE 2032 928 2320 928
            BEGIN DISPLAY 2032 928 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 2416 1088 2320 1088
        BEGIN BRANCH q(6)
            WIRE 1776 1088 2032 1088
            WIRE 2032 1088 2320 1088
            BEGIN DISPLAY 2032 1088 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
        BUSTAP 2416 1248 2320 1248
        BEGIN BRANCH q(7)
            WIRE 1776 1248 2032 1248
            WIRE 2032 1248 2320 1248
            BEGIN DISPLAY 2032 1248 ATTR Name
                ALIGNMENT SOFT-BCENTER
            END DISPLAY
        END BRANCH
    END SHEET
END SCHEMATIC
