Version 4
SymbolType BLOCK
TEXT 32 32 LEFT 4 fifo
RECTANGLE Normal 32 32 544 672
LINE Wide 0 80 32 80
PIN 0 80 LEFT 36
PINATTR PinName din[33:0]
PINATTR Polarity IN
LINE Normal 0 144 32 144
PIN 0 144 LEFT 36
PINATTR PinName wr_en
PINATTR Polarity IN
LINE Normal 0 240 32 240
PIN 0 240 LEFT 36
PINATTR PinName rd_en
PINATTR Polarity IN
LINE Normal 0 368 32 368
PIN 0 368 LEFT 36
PINATTR PinName clk
PINATTR Polarity IN
LINE Wide 576 80 544 80
PIN 576 80 RIGHT 36
PINATTR PinName dout[33:0]
PINATTR Polarity OUT
LINE Normal 576 208 544 208
PIN 576 208 RIGHT 36
PINATTR PinName full
PINATTR Polarity OUT
LINE Normal 576 272 544 272
PIN 576 272 RIGHT 36
PINATTR PinName prog_full
PINATTR Polarity OUT
LINE Normal 576 432 544 432
PIN 576 432 RIGHT 36
PINATTR PinName empty
PINATTR Polarity OUT
LINE Normal 576 496 544 496
PIN 576 496 RIGHT 36
PINATTR PinName prog_empty
PINATTR Polarity OUT

