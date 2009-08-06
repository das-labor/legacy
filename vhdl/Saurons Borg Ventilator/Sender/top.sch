VERSION 6
BEGIN SCHEMATIC
    BEGIN ATTR DeviceFamilyName "spartan3"
        DELETE all:0
        EDITNAME all:0
        EDITTRAIT all:0
    END ATTR
    BEGIN NETLIST
        SIGNAL clk50
        SIGNAL clk20
        SIGNAL clk
        SIGNAL out0
        SIGNAL gnd0
        SIGNAL clk100
        SIGNAL sram_read(15:0)
        SIGNAL sram_pos(7:0)
        SIGNAL winkel_ram(9:0)
        SIGNAL sram_oe
        SIGNAL sram_we
        SIGNAL sram_adr(17:0)
        SIGNAL sram_1_io(15:0)
        SIGNAL sram_1_ce
        SIGNAL sram_1_ub
        SIGNAL sram_1_lb
        SIGNAL sram_2_ce
        SIGNAL sram_2_io(15:0)
        SIGNAL sram_2_ub
        SIGNAL sram_2_lb
        SIGNAL winkel(9:0)
        SIGNAL b10code_k4
        SIGNAL b10code_k3
        SIGNAL b10code_k2
        SIGNAL b10code_k1
        PORT Output clk50
        PORT Output clk20
        PORT Input clk
        PORT Output out0
        PORT Output gnd0
        PORT Output clk100
        PORT Output sram_read(15:0)
        PORT Output sram_pos(7:0)
        PORT Output winkel_ram(9:0)
        PORT Output sram_oe
        PORT Output sram_we
        PORT Output sram_adr(17:0)
        PORT BiDirectional sram_1_io(15:0)
        PORT Output sram_1_ce
        PORT Output sram_1_ub
        PORT Output sram_1_lb
        PORT Output sram_2_ce
        PORT BiDirectional sram_2_io(15:0)
        PORT Output sram_2_ub
        PORT Output sram_2_lb
        PORT Input winkel(9:0)
        PORT Output b10code_k4
        PORT Output b10code_k3
        PORT Output b10code_k2
        PORT Output b10code_k1
        BEGIN BLOCKDEF takt2
            TIMESTAMP 2009 8 6 20 50 17
            LINE N 240 -192 304 -192 
            LINE N 64 -288 0 -288 
            LINE N 240 -288 304 -288 
            LINE N 240 -256 304 -256 
            LINE N 240 -224 304 -224 
            RECTANGLE N 64 -320 240 -164 
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
        BEGIN BLOCKDEF ram_control
            TIMESTAMP 2009 8 6 10 21 13
            LINE N 448 -864 512 -864 
            LINE N 448 -672 512 -672 
            LINE N 448 -544 512 -544 
            LINE N 448 -832 512 -832 
            LINE N 448 -640 512 -640 
            LINE N 448 -704 512 -704 
            LINE N 448 -512 512 -512 
            LINE N 448 -480 512 -480 
            RECTANGLE N 448 -812 512 -788 
            LINE N 448 -800 512 -800 
            RECTANGLE N 448 -748 512 -724 
            LINE N 448 -736 512 -736 
            RECTANGLE N 448 -588 512 -564 
            LINE N 448 -576 512 -576 
            RECTANGLE N 448 -428 512 -404 
            LINE N 448 -416 512 -416 
            RECTANGLE N 112 -896 448 -244 
            RECTANGLE N 448 -396 512 -372 
            LINE N 448 -384 512 -384 
            LINE N 448 -320 512 -320 
            RECTANGLE N 448 -300 512 -276 
            LINE N 448 -288 512 -288 
            LINE N 112 -864 48 -864 
            LINE N 112 -832 48 -832 
            RECTANGLE N 48 -748 112 -724 
            LINE N 112 -736 48 -736 
            LINE N 112 -704 48 -704 
            RECTANGLE N 48 -780 112 -756 
            LINE N 112 -768 48 -768 
            LINE N 112 -656 48 -656 
            RECTANGLE N 48 -636 112 -612 
            LINE N 112 -624 48 -624 
            RECTANGLE N 48 -604 112 -580 
            LINE N 112 -592 48 -592 
            RECTANGLE N 48 -540 112 -516 
            LINE N 112 -528 48 -528 
        END BLOCKDEF
        BEGIN BLOCKDEF packet_buffer
            TIMESTAMP 2009 8 6 10 21 14
            RECTANGLE N 420 404 484 428 
            LINE N 420 416 484 416 
            RECTANGLE N 420 436 484 460 
            LINE N 420 448 484 448 
            RECTANGLE N 420 468 484 492 
            LINE N 420 480 484 480 
            RECTANGLE N 420 500 484 524 
            LINE N 420 512 484 512 
            RECTANGLE N 420 308 484 332 
            LINE N 420 320 484 320 
            RECTANGLE N 420 340 484 364 
            LINE N 420 352 484 352 
            RECTANGLE N 420 372 484 396 
            LINE N 420 384 484 384 
            LINE N 64 -352 0 -352 
            LINE N 64 -320 0 -320 
            RECTANGLE N 0 -284 64 -260 
            LINE N 64 -272 0 -272 
            RECTANGLE N 0 -236 64 -212 
            LINE N 64 -224 0 -224 
            RECTANGLE N 0 -188 64 -164 
            LINE N 64 -176 0 -176 
            RECTANGLE N 416 -348 480 -324 
            LINE N 416 -336 480 -336 
            LINE N 416 -224 480 -224 
            LINE N 416 -192 480 -192 
            LINE N 416 -160 480 -160 
            LINE N 416 -128 480 -128 
            LINE N 416 -96 480 -96 
            LINE N 416 -64 480 -64 
            LINE N 416 -32 480 -32 
            LINE N 416 0 480 0 
            RECTANGLE N 416 20 480 44 
            LINE N 416 32 480 32 
            RECTANGLE N 416 52 480 76 
            LINE N 416 64 480 64 
            RECTANGLE N 416 84 480 108 
            LINE N 416 96 480 96 
            RECTANGLE N 416 116 480 140 
            LINE N 416 128 480 128 
            RECTANGLE N 416 148 480 172 
            LINE N 416 160 480 160 
            RECTANGLE N 416 180 480 204 
            LINE N 416 192 480 192 
            RECTANGLE N 416 212 480 236 
            LINE N 416 224 480 224 
            RECTANGLE N 416 244 480 268 
            LINE N 416 256 480 256 
            RECTANGLE N 416 276 480 300 
            LINE N 416 288 480 288 
            RECTANGLE N 416 -300 480 -276 
            LINE N 416 -288 480 -288 
            RECTANGLE N 416 -268 480 -244 
            LINE N 416 -256 480 -256 
            RECTANGLE N 64 -384 420 544 
        END BLOCKDEF
        BEGIN BLOCK XLXI_31 takt2
            PIN clk clk
            PIN clk_20 clk20
            PIN clk50 clk50
            PIN g90_c10 out0
            PIN clk100 clk100
        END BLOCK
        BEGIN BLOCK XLXI_41 inv
            PIN I out0
            PIN O gnd0
        END BLOCK
        BEGIN BLOCK XLXI_42 ram_control
            PIN sram_1_io(15:0) sram_1_io(15:0)
            PIN sram_2_io(15:0) sram_2_io(15:0)
            PIN sram_oe sram_oe
            PIN sram_we sram_we
            PIN sram_1_ce sram_1_ce
            PIN sram_1_ub sram_1_ub
            PIN sram_1_lb sram_1_lb
            PIN sram_2_ce sram_2_ce
            PIN sram_2_ub sram_2_ub
            PIN sram_2_lb sram_2_lb
            PIN sram_adr(17:0) sram_adr(17:0)
            PIN sram_read(15:0) sram_read(15:0)
            PIN sram_pos(7:0) sram_pos(7:0)
            PIN write_lesen_diag
            PIN winkel_diag(9:0) winkel_ram(9:0)
            PIN clk50 clk50
            PIN clk100 clk100
            PIN ad_dat(15:0)
            PIN ad_wr
            PIN ad_adr(17:0)
            PIN wr_str
            PIN p_id(7:0)
            PIN out_p(7:0)
            PIN winkel(9:0) winkel(9:0)
        END BLOCK
        BEGIN BLOCK XLXI_44 packet_buffer
            PIN clk20 clk20
            PIN clk50 clk50
            PIN sram_read(15:0) sram_read(15:0)
            PIN sram_pos(7:0) sram_pos(7:0)
            PIN winkel(9:0) winkel_ram(9:0)
            PIN b10code_k1 b10code_k1
            PIN b10code_k2 b10code_k2
            PIN b10code_k3 b10code_k3
            PIN b10code_k4 b10code_k4
            PIN rdy_k1_diag
            PIN rdy_k2_diag
            PIN rdy_k3_diag
            PIN rdy_k4_diag
            PIN freeze_k1_diag(4:0)
            PIN freeze_k2_diag(4:0)
            PIN freeze_k3_diag(4:0)
            PIN freeze_k4_diag(4:0)
            PIN winkel_diag(9:0)
            PIN addrb_diag(12:0)
            PIN doutb_diag(15:0)
            PIN b8_code_k1_diag(7:0)
            PIN b8_code_k2_diag(7:0)
            PIN b8_code_k3_diag(7:0)
            PIN b8_code_k4_diag(7:0)
            PIN counter_k1_diag(9:0)
            PIN counter_k2_diag(9:0)
            PIN counter_k3_diag(9:0)
            PIN counter_k4_diag(9:0)
            PIN doutb_k1_diag(15:0)
            PIN doutb_k2_diag(15:0)
            PIN doutb_k3_diag(15:0)
            PIN doutb_k4_diag(15:0)
        END BLOCK
    END NETLIST
    BEGIN SHEET 1 3520 2720
        BEGIN INSTANCE XLXI_31 304 784 R0
        END INSTANCE
        BEGIN BRANCH clk
            WIRE 128 496 304 496
        END BRANCH
        IOMARKER 128 496 clk R180 28
        BEGIN BRANCH out0
            WIRE 608 560 704 560
            WIRE 704 560 704 608
            WIRE 704 608 720 608
            WIRE 704 560 976 560
        END BRANCH
        BEGIN BRANCH gnd0
            WIRE 944 608 976 608
        END BRANCH
        BEGIN BRANCH clk100
            WIRE 608 592 624 592
            WIRE 624 592 624 1104
            WIRE 624 1104 1712 1104
            WIRE 624 432 768 432
            WIRE 624 432 624 592
            WIRE 1712 240 2016 240
            WIRE 1712 240 1712 1104
        END BRANCH
        IOMARKER 768 496 clk20 R0 28
        IOMARKER 768 464 clk50 R0 28
        IOMARKER 768 432 clk100 R0 28
        INSTANCE XLXI_41 720 640 R0
        IOMARKER 976 608 gnd0 R0 28
        IOMARKER 976 560 out0 R0 28
        BEGIN BRANCH sram_oe
            WIRE 2480 208 2512 208
        END BRANCH
        BEGIN BRANCH sram_we
            WIRE 2480 240 2512 240
        END BRANCH
        BEGIN BRANCH sram_adr(17:0)
            WIRE 2480 272 2512 272
        END BRANCH
        BEGIN BRANCH sram_1_io(15:0)
            WIRE 2480 336 2512 336
        END BRANCH
        BEGIN BRANCH sram_1_ce
            WIRE 2480 368 2512 368
        END BRANCH
        BEGIN BRANCH sram_1_ub
            WIRE 2480 400 2512 400
        END BRANCH
        BEGIN BRANCH sram_1_lb
            WIRE 2480 432 2512 432
        END BRANCH
        BEGIN BRANCH sram_2_ce
            WIRE 2480 528 2512 528
        END BRANCH
        BEGIN BRANCH sram_2_io(15:0)
            WIRE 2480 496 2512 496
        END BRANCH
        BEGIN BRANCH sram_2_ub
            WIRE 2480 560 2512 560
        END BRANCH
        BEGIN BRANCH sram_2_lb
            WIRE 2480 592 2512 592
        END BRANCH
        BEGIN BRANCH winkel(9:0)
            WIRE 1984 544 2016 544
        END BRANCH
        BEGIN INSTANCE XLXI_42 1968 1072 R0
        END INSTANCE
        IOMARKER 2512 208 sram_oe R0 28
        IOMARKER 2512 240 sram_we R0 28
        IOMARKER 2512 272 sram_adr(17:0) R0 28
        IOMARKER 2512 336 sram_1_io(15:0) R0 28
        IOMARKER 2512 400 sram_1_ub R0 28
        IOMARKER 2512 432 sram_1_lb R0 28
        IOMARKER 2512 368 sram_1_ce R0 28
        IOMARKER 2512 528 sram_2_ce R0 28
        IOMARKER 2512 496 sram_2_io(15:0) R0 28
        IOMARKER 2512 560 sram_2_ub R0 28
        IOMARKER 2512 592 sram_2_lb R0 28
        IOMARKER 1984 544 winkel(9:0) R180 28
        BEGIN BRANCH clk50
            WIRE 608 528 656 528
            WIRE 656 528 656 1072
            WIRE 656 1072 1680 1072
            WIRE 1680 1072 2032 1072
            WIRE 656 464 768 464
            WIRE 656 464 656 528
            WIRE 1680 208 2016 208
            WIRE 1680 208 1680 1072
        END BRANCH
        BEGIN BRANCH clk20
            WIRE 608 496 688 496
            WIRE 688 496 768 496
            WIRE 688 496 688 1040
            WIRE 688 1040 2032 1040
        END BRANCH
        BEGIN BRANCH b10code_k4
            WIRE 2512 1264 2544 1264
        END BRANCH
        BEGIN BRANCH b10code_k3
            WIRE 2512 1232 2544 1232
        END BRANCH
        BEGIN BRANCH b10code_k2
            WIRE 2512 1200 2544 1200
        END BRANCH
        BEGIN BRANCH b10code_k1
            WIRE 2512 1168 2544 1168
        END BRANCH
        BEGIN INSTANCE XLXI_44 2032 1392 R0
        END INSTANCE
        IOMARKER 2544 1264 b10code_k4 R0 28
        IOMARKER 2544 1232 b10code_k3 R0 28
        IOMARKER 2544 1200 b10code_k2 R0 28
        IOMARKER 2544 1168 b10code_k1 R0 28
        BEGIN BRANCH sram_pos(7:0)
            WIRE 1904 912 1904 1168
            WIRE 1904 1168 2032 1168
            WIRE 1904 912 2608 912
            WIRE 2480 688 2608 688
            WIRE 2608 688 2608 912
            WIRE 2608 688 2704 688
        END BRANCH
        BEGIN BRANCH sram_read(15:0)
            WIRE 1920 928 1920 1120
            WIRE 1920 1120 2032 1120
            WIRE 1920 928 2624 928
            WIRE 2480 656 2624 656
            WIRE 2624 656 2624 928
            WIRE 2624 656 2704 656
        END BRANCH
        BEGIN BRANCH winkel_ram(9:0)
            WIRE 1888 896 1888 1216
            WIRE 1888 1216 2032 1216
            WIRE 1888 896 2592 896
            WIRE 2480 784 2592 784
            WIRE 2592 784 2592 896
            WIRE 2592 784 2688 784
        END BRANCH
        IOMARKER 2704 656 sram_read(15:0) R0 28
        IOMARKER 2704 688 sram_pos(7:0) R0 28
        IOMARKER 2688 784 winkel_ram(9:0) R0 28
    END SHEET
END SCHEMATIC
