# Synplicity, Inc. constraint file
# C:\ispTOOLS6_0\micosystem\components\lm32_top\rtl\verilog\system.sdc
# Written on Thu Apr 06 13:56:58 2006
# by Synplify for Lattice, Synplify for Lattice 8.5D Scope Editor

#
# Clocks
#
define_clock            -name {clk_i}  -freq 25.000 -clockgroup default_clkgroup_0

#
# Clock to Clock
#

#
# Inputs/Outputs
#

#
# Registers
#

#
# Multicycle Path
#

#
# False Path
#

#
# Path Delay
#

#
# Attributes
#
define_attribute          {clk_i} loc {a10}
define_attribute          {reset_n} loc {h6}
define_attribute          {uartSIN} loc {m2}
define_attribute          {uartSOUT} loc {l1}
define_attribute          {sramsram_csn} loc {y4}
define_attribute          {sramsram_oen} loc {aa6}
define_attribute          {sramsram_wen} loc {ab6}
define_attribute          {sramsram_be[3:0]} loc {aa3,ab3,aa4,ab4}
define_attribute          {sramsram_data[31:0]} loc {v7,w7,y7,aa7,ab7,v8,w8,y8,aa8,ab8,v9,w9,y9,aa9,ab9,v10,w10,y10,aa10,w11,y11,aa11,y13,aa13,w14,y14,aa14,ab14,u15,v15,w15,y15}
define_attribute          {sramsram_addr[22:0]} loc {u16,v16,w16,y16,aa16,ab16,u17,v17,w17,y17,aa17,ab17,w18,y18,aa18,ab18,y19,aa19,ab19,aa20,ab20,y20,w20}
define_attribute          {gpio_LCDPIO_OUT[15:0]} loc {h5,t13,t17,u13,v13,p5,p3,p4,n5,n4,n3,n2,n1,m5,m4,m3}
define_attribute          {gpio_LEDPIO_OUT[9:0]} loc {u11,v5,u6,u4,g5,g4,f5,f4,e5,e4}
define_attribute          {gpio_KeypadPIO_IN[3:0]} loc {R1,T3,T2,T1}

#
# I/O standards
#

#
# Other Constraints
#
