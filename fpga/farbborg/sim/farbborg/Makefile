CVER=cver
GTKWAVE=gtkwave
IVERILOG=iverilog
VVP=vvp
	
sim: wb_farbborg_tb.view

VINCDIR=					\
	-I../../rtl/wb_farbborg			\
	-I../../rtl/wb_conbus			\
	-I../../rtl/wb_lm32

DUALPORTRAM1_TB_SRC=				\
	dualportram1_tb.v			\
	../../rtl/wb_farbborg/dualportram.v
	
DUALPORTRAM2_TB_SRC=				\
	dualportram2_tb.v			\
	../../rtl/wb_farbborg/dualportram.v

PWM_TB_SRC=				\
	pwm_tb.v			\
	../../rtl/wb_farbborg/pwm.v   \
	../../rtl/wb_farbborg/dualportram.v

WB_FARBBORG_TB_SRC=				\
	wb_farbborg_tb.v			\
	../../rtl/wb_farbborg/pwm.v   \
	../../rtl/wb_farbborg/wb_farbborg.v   \
	../../rtl/wb_farbborg/dualportram.v
	
SYSTEM_TB_SRC=					\
	system_tb.v				\
	../../rtl/system.v				\
	../../rtl/wb_lm32/lm32_cpu.v		\
	../../rtl/wb_lm32/lm32_instruction_unit.v	\
	../../rtl/wb_lm32/lm32_decoder.v		\
	../../rtl/wb_lm32/lm32_load_store_unit.v	\
	../../rtl/wb_lm32/lm32_adder.v		\
	../../rtl/wb_lm32/lm32_addsub.v		\
	../../rtl/wb_lm32/lm32_logic_op.v		\
	../../rtl/wb_lm32/lm32_shifter.v		\
	../../rtl/wb_lm32/lm32_multiplier.v	\
	../../rtl/wb_lm32/lm32_mc_arithmetic.v	\
	../../rtl/wb_lm32/lm32_interrupt.v		\
	../../rtl/wb_lm32/lm32_icache.v		\
	../../rtl/wb_bram/wb_bram.v		\
	../../rtl/uart_core/intface.v		\
	../../rtl/uart_core/modem.v		\
	../../rtl/uart_core/rxcver.v		\
	../../rtl/uart_core/txmitt.v		\
	../../rtl/uart_core/uart_core.v		\
	../../rtl/wb_conbus/wb_conbus_top.v	\
	../../rtl/wb_conbus/wb_conbus_arb.v

####################################################################
# cver simulation
#cver:
#	$(CVER) +incdir+../../rtl/wb_conbus+../../rtl/wb_lm32 -f system_tb.list

####################################################################
# Ikarus verilog simulation

system_tb.vvp: $(SYSTEM_TB_SRC)
	rm -f $@.list
	for i in `echo $^`; do \
	    echo $$i >> $@.list; \
	done
	$(IVERILOG) -o $@ $(VINCDIR) -c $@.list -s $(@:.vvp=)

dualportram1_tb.vvp: $(DUALPORTRAM1_TB_SRC)
	rm -f $@.list
	for i in `echo $^`; do \
	    echo $$i >> $@.list; \
	done
	$(IVERILOG) -o $@ $(VINCDIR) -c $@.list -s $(@:.vvp=)

dualportram2_tb.vvp: $(DUALPORTRAM2_TB_SRC)
	rm -f $@.list
	for i in `echo $^`; do \
	    echo $$i >> $@.list; \
	done
	$(IVERILOG) -o $@ $(VINCDIR) -c $@.list -s $(@:.vvp=)

pwm_tb.vvp: $(PWM_TB_SRC)
	rm -f $@.list
	for i in `echo $^`; do \
	    echo $$i >> $@.list; \
	done
	$(IVERILOG) -o $@ $(VINCDIR) -c $@.list -s $(@:.vvp=)

wb_farbborg_tb.vvp: $(WB_FARBBORG_TB_SRC)
	rm -f $@.list
	for i in `echo $^`; do \
	    echo $$i >> $@.list; \
	done
	$(IVERILOG) -o $@ $(VINCDIR) -c $@.list -s $(@:.vvp=)

####################################################################
# final targets

%.vcd: %.vvp
	$(VVP) $<

%.view: %.vcd
	$(GTKWAVE) $< $<.save

clean:
	rm -f *.vcd verilog.log *.vvp *.vvp.list

