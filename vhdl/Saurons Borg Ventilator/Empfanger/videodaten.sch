VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "spartan3"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL slow_clk
        SIGNAL data1(7:0)
        SIGNAL XLXN_821
        SIGNAL XLXN_824
        SIGNAL XLXN_825
        SIGNAL XLXN_828
        SIGNAL XLXN_832
        SIGNAL XLXN_833
        SIGNAL data2(7:0)
        SIGNAL data3(7:0)
        SIGNAL data4(7:0)
        SIGNAL XLXN_896
        SIGNAL kanal1
        SIGNAL XLXN_950
        SIGNAL XLXN_981(7:0)
        SIGNAL clk200
        SIGNAL XLXN_989
        SIGNAL kanal0
        SIGNAL kanal2
        SIGNAL kanal3
        SIGNAL in1a
        SIGNAL in1b
        SIGNAL in4a
        SIGNAL in4b
        SIGNAL in3b
        SIGNAL in3a
        SIGNAL in2a
        SIGNAL in2b
        SIGNAL clk60
        SIGNAL XLXN_1010(7:0)
        SIGNAL p_adr(7:0)
        SIGNAL p_data(7:0)
        SIGNAL XLXN_1021
        SIGNAL XLXN_1022
        SIGNAL XLXN_1023(7:0)
        SIGNAL XLXN_1024(7:0)
        SIGNAL XLXN_1025
        SIGNAL XLXN_1026
        SIGNAL led
        SIGNAL tp3
        SIGNAL tp1
        SIGNAL XLXN_1034(7:0)
        SIGNAL XLXN_1044
        SIGNAL clk
        SIGNAL XLXN_985
        SIGNAL XLXN_1027
        SIGNAL XLXN_1069
        SIGNAL in0b
        SIGNAL in0a
        SIGNAL tp2
        SIGNAL XLXN_1032
        SIGNAL XLXN_1033(7:0)
        SIGNAL XLXN_1050
        SIGNAL XLXN_1079
        SIGNAL XLXN_1080
        SIGNAL XLXN_1081
        SIGNAL out1
        PORT Output XLXN_981(7:0)
        PORT Input XLXN_989
        PORT Input in1a
        PORT Input in1b
        PORT Input in4a
        PORT Input in4b
        PORT Input in3b
        PORT Input in3a
        PORT Input in2a
        PORT Input in2b
        PORT Output XLXN_1010(7:0)
        PORT Output p_adr(7:0)
        PORT Output p_data(7:0)
        PORT Output XLXN_1023(7:0)
        PORT Output XLXN_1024(7:0)
        PORT Output XLXN_1025
        PORT Output XLXN_1026
        PORT Output led
        PORT Output tp3
        PORT Output tp1
        PORT Input clk
        PORT Input in0b
        PORT Input in0a
        PORT Output tp2
        PORT Output out1
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
        BEGIN BLOCKDEF f_null
            TIMESTAMP 2009 6 29 11 32 51
            RECTANGLE N 280 -48 320 -16 
            LINE N 320 -32 384 -32 
        END BLOCKDEF
        BEGIN BLOCKDEF obuf
            TIMESTAMP 2000 1 1 10 10 10
            LINE N 64 0 64 -64 
            LINE N 128 -32 64 0 
            LINE N 64 -64 128 -32 
            LINE N 0 -32 64 -32 
            LINE N 224 -32 128 -32 
        END BLOCKDEF
        BEGIN BLOCKDEF takt
            TIMESTAMP 2009 6 29 11 32 51
            LINE N 64 -160 0 -160 
            LINE N 400 -160 464 -160 
            RECTANGLE N 64 -192 400 -44 
            LINE N 400 -128 464 -128 
            LINE N 400 -96 464 -96 
            LINE N 64 -96 0 -96 
        END BLOCKDEF
        BEGIN BLOCKDEF bram2p
            TIMESTAMP 2009 6 29 11 40 45
            RECTANGLE N 32 0 256 496 
            BEGIN LINE W 0 48 32 48 
            END LINE
            BEGIN LINE W 0 80 32 80 
            END LINE
            LINE N 0 112 32 112 
            LINE N 0 240 32 240 
            BEGIN LINE W 0 272 32 272 
            END LINE
            LINE N 0 464 32 464 
            BEGIN LINE W 256 272 288 272 
            END LINE
        END BLOCKDEF
        BEGIN BLOCKDEF f_eins
            TIMESTAMP 2009 6 29 11 32 51
            RECTANGLE N 192 -64 320 0 
            LINE N 320 -32 384 -32 
        END BLOCKDEF
        BEGIN BLOCKDEF switch4_8
            TIMESTAMP 2009 6 29 11 32 51
            RECTANGLE N 64 -300 320 0 
            LINE N 64 -16 0 -16 
            RECTANGLE N 0 -124 64 -100 
            LINE N 64 -112 0 -112 
            LINE N 64 -48 0 -48 
            RECTANGLE N 0 -172 64 -148 
            LINE N 64 -160 0 -160 
            RECTANGLE N 0 -220 64 -196 
            LINE N 64 -208 0 -208 
            RECTANGLE N 0 -268 64 -244 
            LINE N 64 -256 0 -256 
            RECTANGLE N 320 -268 384 -244 
            LINE N 320 -256 384 -256 
        END BLOCKDEF
        BEGIN BLOCKDEF com_clock
            TIMESTAMP 2009 6 29 11 32 51
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
        BEGIN BLOCKDEF clk_200
            TIMESTAMP 2009 6 29 11 32 51
            RECTANGLE N 64 -192 368 -80 
            LINE N 64 -160 0 -160 
            LINE N 368 -160 432 -160 
            LINE N 368 -128 432 -128 
            LINE N 368 -96 432 -96 
            LINE N 64 -128 0 -128 
        END BLOCKDEF
        BEGIN BLOCKDEF packet_read
            TIMESTAMP 2009 6 29 11 32 51
            RECTANGLE N 64 -320 288 -80 
            LINE N 64 -288 0 -288 
            LINE N 64 -192 0 -192 
            LINE N 288 -160 352 -160 
            RECTANGLE N 288 -300 352 -276 
            LINE N 288 -288 352 -288 
            LINE N 288 -256 352 -256 
            RECTANGLE N 288 -236 352 -212 
            LINE N 288 -224 352 -224 
            LINE N 288 -128 352 -128 
            LINE N 288 -96 352 -96 
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
            TIMESTAMP 2009 6 29 11 32 51
            LINE N 400 32 464 32 
            LINE N 64 -160 0 -160 
            LINE N 64 -32 0 -32 
            LINE N 400 -160 464 -160 
            RECTANGLE N 400 -108 464 -84 
            LINE N 400 -96 464 -96 
            RECTANGLE N 64 -192 400 192 
            LINE N 400 80 464 80 
            LINE N 400 128 464 128 
            LINE N 400 -16 464 -16 
        END BLOCKDEF
        BEGIN BLOCKDEF Picoblaze_e
            TIMESTAMP 2009 6 29 11 32 51
            RECTANGLE N 4 372 68 396 
            LINE N 68 384 4 384 
            LINE N 320 352 384 352 
            LINE N 64 -272 0 -272 
            LINE N 320 -272 384 -272 
            LINE N 64 -240 0 -240 
            LINE N 320 -240 384 -240 
            RECTANGLE N 320 -156 384 -132 
            LINE N 320 -144 384 -144 
            RECTANGLE N 320 -188 384 -164 
            LINE N 320 -176 384 -176 
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
            LINE N 320 -64 384 -64 
            RECTANGLE N 68 -300 320 440 
        END BLOCKDEF
        BEGIN BLOCKDEF com_20_status
            TIMESTAMP 2009 6 29 11 32 51
            RECTANGLE N 64 -80 176 20 
            LINE N 64 -64 0 -64 
            RECTANGLE N 176 -76 240 -52 
            LINE N 176 -64 240 -64 
            RECTANGLE N 0 -12 64 12 
            LINE N 64 0 0 0 
        END BLOCKDEF
        BEGIN BLOCKDEF packet_write
            TIMESTAMP 2009 6 29 11 32 51
            RECTANGLE N 64 -152 288 -36 
            RECTANGLE N 0 -108 64 -84 
            LINE N 64 -96 0 -96 
            LINE N 288 -96 352 -96 
            RECTANGLE N 288 -76 352 -52 
            LINE N 288 -64 352 -64 
            LINE N 64 -128 0 -128 
            LINE N 288 -128 352 -128 
        END BLOCKDEF
        BEGIN BLOCK XLXI_239 f_eins
            PIN f1 XLXN_1021
        END BLOCK
        BEGIN BLOCK XLXI_238 bram2p
            PIN addra(7:0) p_adr(7:0)
            PIN dina(7:0) p_data(7:0)
            PIN wea XLXN_1021
            PIN clka XLXN_1079
            PIN addrb(7:0) data4(7:0)
            PIN clkb XLXN_1079
            PIN doutb(7:0) XLXN_1010(7:0)
        END BLOCK
        BEGIN BLOCK XLXI_249 switch4_8
            PIN clk clk60
            PIN d(7:0) data4(7:0)
            PIN taste XLXN_989
            PIN c(7:0) data3(7:0)
            PIN b(7:0) data2(7:0)
            PIN a(7:0) data1(7:0)
            PIN q(7:0) XLXN_981(7:0)
        END BLOCK
        BEGIN BLOCK XLXI_270 com_clock
            PIN diag_cf2(6:0)
            PIN diag_cf1(6:0)
            PIN diag_res2
            PIN diag_res1
            PIN diag_ff2
            PIN diag_ff1
            PIN com_clk XLXN_1069
            PIN com_clk_2x XLXN_1044
            PIN MHZ_200 clk200
        END BLOCK
        BEGIN BLOCK XLXI_282 packet_read
            PIN data_in kanal0
            PIN clk slow_clk
            PIN rdy
            PIN crc_ok XLXN_821
            PIN crc_er XLXN_896
            PIN data_out(7:0) data1(7:0)
            PIN counter(9:0)
            PIN synchron
        END BLOCK
        BEGIN BLOCK XLXI_283 packet_read
            PIN data_in kanal1
            PIN clk slow_clk
            PIN rdy
            PIN crc_ok XLXN_824
            PIN crc_er XLXN_825
            PIN data_out(7:0) data2(7:0)
            PIN counter(9:0)
            PIN synchron
        END BLOCK
        BEGIN BLOCK XLXI_284 packet_read
            PIN data_in kanal2
            PIN clk slow_clk
            PIN rdy
            PIN crc_ok XLXN_828
            PIN crc_er XLXN_950
            PIN data_out(7:0) data3(7:0)
            PIN counter(9:0)
            PIN synchron
        END BLOCK
        BEGIN BLOCK XLXI_286 packet_read
            PIN data_in kanal3
            PIN clk slow_clk
            PIN rdy
            PIN crc_ok XLXN_832
            PIN crc_er XLXN_833
            PIN data_out(7:0) data4(7:0)
            PIN counter(9:0)
            PIN synchron
        END BLOCK
        BEGIN BLOCK XLXI_172 ibufds
            PIN I in1a
            PIN IB in1b
            PIN O kanal1
        END BLOCK
        BEGIN BLOCK XLXI_215 ibufds
            PIN I in4a
            PIN IB in4b
            PIN O kanal2
        END BLOCK
        BEGIN BLOCK XLXI_263 ibufds
            PIN I in3a
            PIN IB in3b
            PIN O kanal0
        END BLOCK
        BEGIN BLOCK XLXI_213 ibufds
            PIN I in2a
            PIN IB in2b
            PIN O kanal3
        END BLOCK
        BEGIN BLOCK XLXI_317 Picoblaze_e
            PIN clk clk60
            PIN in0 XLXN_821
            PIN in1 XLXN_896
            PIN in2
            PIN in3
            PIN in4 XLXN_824
            PIN in5 XLXN_825
            PIN in6
            PIN in7
            PIN in8 XLXN_828
            PIN in9 XLXN_950
            PIN in10
            PIN in11
            PIN in12 XLXN_832
            PIN in13 XLXN_833
            PIN in14
            PIN in15
            PIN irq
            PIN dcm_sts(7:0) XLXN_1034(7:0)
            PIN led led
            PIN com_res XLXN_1022
            PIN wr_str XLXN_1026
            PIN irq_ack XLXN_1025
            PIN en_ram XLXN_1079
            PIN p_id(7:0) XLXN_1024(7:0)
            PIN out_p(7:0) XLXN_1023(7:0)
            PIN q2(7:0) p_data(7:0)
            PIN q3(7:0) p_adr(7:0)
        END BLOCK
        BEGIN BLOCK XLXI_312 obuf
            PIN I slow_clk
            PIN O tp3
        END BLOCK
        BEGIN BLOCK XLXI_276 obuf
            PIN I kanal2
            PIN O tp1
        END BLOCK
        BEGIN BLOCK XLXI_319 packet_write
            PIN clk XLXN_1081
            PIN b8_code(7:0) data4(7:0)
            PIN q1 out1
            PIN rdy
            PIN counter(9:0)
        END BLOCK
        BEGIN BLOCK XLXI_203 takt
            PIN RST_IN XLXN_1027
            PIN CLKIN_IN clk
            PIN LOCKED_OUT
            PIN CLKIN_IBUFG_OUT XLXN_985
            PIN CLK0_OUT clk60
        END BLOCK
        BEGIN BLOCK XLXI_277 clk_200
            PIN CLKIN_IN XLXN_985
            PIN LOCKED_OUT
            PIN CLKFX_OUT clk200
            PIN CLK0_OUT
            PIN RST_IN XLXN_1027
        END BLOCK
        BEGIN BLOCK XLXI_208 f_null
            PIN fo XLXN_1027
        END BLOCK
        BEGIN BLOCK XLXI_214 ibufds
            PIN I in0a
            PIN IB in0b
            PIN O XLXN_1069
        END BLOCK
        BEGIN BLOCK XLXI_316 obuf
            PIN I XLXN_1069
            PIN O tp2
        END BLOCK
        BEGIN BLOCK XLXI_313 com_20
            PIN CLKIN_IN XLXN_1044
            PIN RST_IN XLXN_1079
            PIN STATUS_OUT(7:0) XLXN_1033(7:0)
            PIN LOCKED_OUT XLXN_1032
            PIN CLK90_OUT XLXN_1080
            PIN CLK180_OUT
            PIN CLK270_OUT
            PIN CLK0_OUT XLXN_1050
        END BLOCK
        BEGIN BLOCK XLXI_287 bufg
            PIN I XLXN_1050
            PIN O slow_clk
        END BLOCK
        BEGIN BLOCK XLXI_318 com_20_status
            PIN a XLXN_1032
            PIN q(7:0) XLXN_1034(7:0)
            PIN b(7:0) XLXN_1033(7:0)
        END BLOCK
        BEGIN BLOCK XLXI_336 bufg
            PIN I XLXN_1080
            PIN O XLXN_1081
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        BEGIN BRANCH data1(7:0)
            WIRE 2112 96 2592 96
            WIRE 2592 96 2592 416
            WIRE 2592 416 3008 416
        END BRANCH
        BEGIN BRANCH XLXN_821
            WIRE 2112 224 2416 224
            WIRE 2416 224 2416 1088
            WIRE 2416 1088 2848 1088
        END BRANCH
        BEGIN BRANCH XLXN_824
            WIRE 2096 816 2352 816
            WIRE 2352 816 2352 1216
            WIRE 2352 1216 2848 1216
        END BRANCH
        BEGIN BRANCH XLXN_825
            WIRE 2096 848 2336 848
            WIRE 2336 848 2336 1248
            WIRE 2336 1248 2848 1248
        END BRANCH
        BEGIN BRANCH XLXN_828
            WIRE 2096 1408 2224 1408
            WIRE 2224 1344 2224 1408
            WIRE 2224 1344 2848 1344
        END BRANCH
        BEGIN BRANCH XLXN_832
            WIRE 2096 2000 2336 2000
            WIRE 2336 1472 2848 1472
            WIRE 2336 1472 2336 2000
        END BRANCH
        BEGIN BRANCH XLXN_833
            WIRE 2096 2032 2352 2032
            WIRE 2352 1504 2848 1504
            WIRE 2352 1504 2352 2032
        END BRANCH
        BEGIN BRANCH data2(7:0)
            WIRE 2096 688 2512 688
            WIRE 2512 464 3008 464
            WIRE 2512 464 2512 688
        END BRANCH
        BEGIN BRANCH data3(7:0)
            WIRE 2096 1280 2544 1280
            WIRE 2544 512 3008 512
            WIRE 2544 512 2544 1280
        END BRANCH
        BEGIN INSTANCE XLXI_249 3008 672 R0
        END INSTANCE
        BEGIN BRANCH XLXN_896
            WIRE 2112 256 2384 256
            WIRE 2384 256 2384 1120
            WIRE 2384 1120 2848 1120
        END BRANCH
        BEGIN BRANCH XLXN_950
            WIRE 2096 1440 2240 1440
            WIRE 2240 1376 2240 1440
            WIRE 2240 1376 2848 1376
        END BRANCH
        BEGIN BRANCH XLXN_981(7:0)
            WIRE 3104 144 3168 144
            WIRE 3104 144 3104 224
            WIRE 3104 224 3408 224
            WIRE 3408 224 3408 416
            WIRE 3392 416 3408 416
        END BRANCH
        BEGIN BRANCH clk200
            WIRE 176 624 880 624
            WIRE 176 624 176 768
            WIRE 176 768 256 768
            WIRE 736 416 880 416
            WIRE 880 416 880 624
        END BRANCH
        BEGIN INSTANCE XLXI_282 1760 384 R0
        END INSTANCE
        BEGIN INSTANCE XLXI_283 1744 976 R0
        END INSTANCE
        BEGIN INSTANCE XLXI_284 1744 1568 R0
        END INSTANCE
        BEGIN INSTANCE XLXI_286 1744 2160 R0
        END INSTANCE
        BEGIN BRANCH XLXN_989
            WIRE 2976 624 3008 624
        END BRANCH
        IOMARKER 2976 624 XLXN_989 R180 28
        IOMARKER 3168 144 XLXN_981(7:0) R0 28
        BEGIN BRANCH kanal2
            WIRE 1584 1568 1584 1664
            WIRE 1584 1664 1648 1664
            WIRE 1584 1568 1696 1568
            WIRE 1680 1376 1696 1376
            WIRE 1696 1376 1744 1376
            WIRE 1696 1376 1696 1568
        END BRANCH
        BEGIN BRANCH in1a
            WIRE 1392 768 1424 768
        END BRANCH
        BEGIN BRANCH in1b
            WIRE 1392 800 1424 800
        END BRANCH
        INSTANCE XLXI_172 1424 816 R0
        IOMARKER 1392 768 in1a R180 28
        IOMARKER 1392 800 in1b R180 28
        BEGIN BRANCH in4a
            WIRE 1392 1360 1456 1360
        END BRANCH
        BEGIN BRANCH in4b
            WIRE 1392 1392 1456 1392
        END BRANCH
        INSTANCE XLXI_215 1456 1408 R0
        IOMARKER 1392 1360 in4a R180 28
        IOMARKER 1392 1392 in4b R180 28
        BEGIN BRANCH in3b
            WIRE 1344 208 1408 208
        END BRANCH
        BEGIN BRANCH in3a
            WIRE 1344 176 1408 176
        END BRANCH
        INSTANCE XLXI_263 1408 224 R0
        IOMARKER 1344 176 in3a R180 28
        IOMARKER 1344 208 in3b R180 28
        BEGIN BRANCH in2a
            WIRE 1392 1952 1408 1952
        END BRANCH
        BEGIN BRANCH in2b
            WIRE 1392 1984 1408 1984
        END BRANCH
        INSTANCE XLXI_213 1408 2000 R0
        IOMARKER 1392 1952 in2a R180 28
        IOMARKER 1392 1984 in2b R180 28
        BEGIN BRANCH clk60
            WIRE 768 176 832 176
            WIRE 832 48 832 176
            WIRE 832 48 2480 48
            WIRE 2480 48 2480 656
            WIRE 2480 656 3008 656
            WIRE 2480 656 2480 992
            WIRE 2480 992 2848 992
        END BRANCH
        BEGIN INSTANCE XLXI_317 2848 1264 R0
        END INSTANCE
        BEGIN BRANCH XLXN_1010(7:0)
            WIRE 3232 2064 3264 2064
        END BRANCH
        IOMARKER 3264 2064 XLXN_1010(7:0) R0 28
        BEGIN BRANCH p_adr(7:0)
            WIRE 2880 1728 2880 1840
            WIRE 2880 1840 2944 1840
            WIRE 2880 1728 3280 1728
            WIRE 3232 1488 3280 1488
            WIRE 3280 1488 3344 1488
            WIRE 3280 1488 3280 1728
        END BRANCH
        BEGIN BRANCH p_data(7:0)
            WIRE 2928 1744 2928 1872
            WIRE 2928 1872 2944 1872
            WIRE 2928 1744 3296 1744
            WIRE 3232 1536 3296 1536
            WIRE 3296 1536 3344 1536
            WIRE 3296 1536 3296 1744
        END BRANCH
        BEGIN BRANCH data4(7:0)
            WIRE 352 1888 432 1888
            WIRE 352 1888 352 2416
            WIRE 352 2416 2592 2416
            WIRE 2096 1872 2592 1872
            WIRE 2592 1872 2592 2064
            WIRE 2592 2064 2944 2064
            WIRE 2592 2064 2592 2416
            WIRE 2592 560 3008 560
            WIRE 2592 560 2592 1872
        END BRANCH
        BEGIN INSTANCE XLXI_238 2944 1792 R0
        END INSTANCE
        IOMARKER 3344 1536 p_data(7:0) R0 28
        IOMARKER 3344 1488 p_adr(7:0) R0 28
        BEGIN INSTANCE XLXI_239 2448 1936 R0
        END INSTANCE
        BEGIN BRANCH XLXN_1021
            WIRE 2832 1904 2944 1904
        END BRANCH
        BEGIN BRANCH XLXN_1023(7:0)
            WIRE 3232 1120 3264 1120
        END BRANCH
        IOMARKER 3264 1120 XLXN_1023(7:0) R0 28
        BEGIN BRANCH XLXN_1024(7:0)
            WIRE 3232 1088 3264 1088
        END BRANCH
        IOMARKER 3264 1088 XLXN_1024(7:0) R0 28
        BEGIN BRANCH XLXN_1025
            WIRE 3232 1024 3264 1024
        END BRANCH
        IOMARKER 3264 1024 XLXN_1025 R0 28
        BEGIN BRANCH XLXN_1026
            WIRE 3232 992 3264 992
        END BRANCH
        IOMARKER 3264 992 XLXN_1026 R0 28
        BEGIN BRANCH led
            WIRE 3232 1200 3264 1200
        END BRANCH
        IOMARKER 3264 1200 led R0 28
        BEGIN BRANCH slow_clk
            WIRE 1120 1328 1216 1328
            WIRE 1216 1328 1216 1872
            WIRE 1216 1872 1744 1872
            WIRE 1216 1872 1216 2336
            WIRE 1216 2336 1840 2336
            WIRE 1216 96 1760 96
            WIRE 1216 96 1216 688
            WIRE 1216 688 1744 688
            WIRE 1216 688 1216 1280
            WIRE 1216 1280 1744 1280
            WIRE 1216 1280 1216 1328
        END BRANCH
        BEGIN BRANCH tp3
            WIRE 2064 2336 2160 2336
        END BRANCH
        INSTANCE XLXI_312 1840 2368 R0
        IOMARKER 2160 2336 tp3 R0 28
        BEGIN BRANCH kanal0
            WIRE 1632 192 1760 192
        END BRANCH
        BEGIN BRANCH kanal1
            WIRE 1648 784 1744 784
        END BRANCH
        BEGIN BRANCH tp1
            WIRE 1872 1664 1968 1664
        END BRANCH
        INSTANCE XLXI_276 1648 1696 R0
        IOMARKER 1968 1664 tp1 R0 28
        BEGIN BRANCH XLXN_1034(7:0)
            WIRE 848 1184 2832 1184
            WIRE 2832 1184 2832 1648
            WIRE 2832 1648 2848 1648
        END BRANCH
        BEGIN BRANCH XLXN_1022
            WIRE 3232 1616 3248 1616
        END BRANCH
        BEGIN BRANCH kanal3
            WIRE 1632 1968 1744 1968
        END BRANCH
        BEGIN INSTANCE XLXI_203 304 272 R0
        END INSTANCE
        BEGIN BRANCH clk
            WIRE 272 176 304 176
        END BRANCH
        BEGIN INSTANCE XLXI_277 304 544 R0
        END INSTANCE
        BEGIN BRANCH XLXN_985
            WIRE 256 272 256 384
            WIRE 256 384 304 384
            WIRE 256 272 848 272
            WIRE 768 144 848 144
            WIRE 848 144 848 272
        END BRANCH
        BEGIN BRANCH XLXN_1027
            WIRE 128 112 160 112
            WIRE 160 112 304 112
            WIRE 160 112 160 416
            WIRE 160 416 304 416
        END BRANCH
        BEGIN INSTANCE XLXI_208 -256 144 R0
        END INSTANCE
        IOMARKER 272 176 clk R180 28
        BEGIN INSTANCE XLXI_270 256 992 R0
        END INSTANCE
        BEGIN BRANCH XLXN_1069
            WIRE 240 656 464 656
            WIRE 240 656 240 736
            WIRE 240 736 256 736
            WIRE 416 544 464 544
            WIRE 464 544 464 656
            WIRE 464 544 496 544
        END BRANCH
        BEGIN BRANCH in0b
            WIRE 112 560 192 560
        END BRANCH
        BEGIN BRANCH in0a
            WIRE 112 528 192 528
        END BRANCH
        INSTANCE XLXI_214 192 576 R0
        IOMARKER 112 560 in0b R180 28
        IOMARKER 112 528 in0a R180 28
        BEGIN BRANCH tp2
            WIRE 720 544 736 544
        END BRANCH
        INSTANCE XLXI_316 496 576 R0
        IOMARKER 736 544 tp2 R0 28
        BEGIN INSTANCE XLXI_313 96 1344 R0
        END INSTANCE
        BEGIN INSTANCE XLXI_318 608 1248 R0
        END INSTANCE
        BEGIN BRANCH XLXN_1032
            WIRE 560 1184 608 1184
        END BRANCH
        BEGIN BRANCH XLXN_1033(7:0)
            WIRE 560 1248 608 1248
        END BRANCH
        BEGIN BRANCH XLXN_1050
            WIRE 560 1328 896 1328
        END BRANCH
        BEGIN BRANCH XLXN_1044
            WIRE 32 1072 32 1312
            WIRE 32 1312 96 1312
            WIRE 32 1072 752 1072
            WIRE 640 736 752 736
            WIRE 752 736 752 1072
        END BRANCH
        BEGIN BRANCH XLXN_1079
            WIRE 80 1184 96 1184
            WIRE 80 1184 80 1712
            WIRE 80 1712 3312 1712
            WIRE 2880 2032 2944 2032
            WIRE 2880 2032 2880 2256
            WIRE 2880 2256 2880 2352
            WIRE 2880 2352 3248 2352
            WIRE 2880 2256 2944 2256
            WIRE 3232 1584 3248 1584
            WIRE 3248 1584 3248 1616
            WIRE 3248 1616 3248 2352
            WIRE 3248 1616 3312 1616
            WIRE 3312 1616 3312 1712
        END BRANCH
        INSTANCE XLXI_336 640 1408 R0
        BEGIN BRANCH XLXN_1080
            WIRE 560 1376 640 1376
        END BRANCH
        BEGIN INSTANCE XLXI_319 432 1984 R0
        END INSTANCE
        BEGIN BRANCH XLXN_1081
            WIRE 368 1632 880 1632
            WIRE 368 1632 368 1856
            WIRE 368 1856 432 1856
            WIRE 864 1376 880 1376
            WIRE 880 1376 880 1632
        END BRANCH
        INSTANCE XLXI_287 896 1360 R0
        BEGIN BRANCH out1
            WIRE 784 1856 800 1856
            WIRE 800 1856 848 1856
        END BRANCH
        IOMARKER 848 1856 out1 R0 28
    END SHEET
END SCHEMATIC
