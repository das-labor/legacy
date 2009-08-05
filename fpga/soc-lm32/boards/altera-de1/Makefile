PROJECT = system

SYN_SRC =	                              \

SIM_SRC =	                              \
	system_tb.v                           \
	../../sim/sram/sram16.v

SRC =	                                  \
	system.v                              \
	../../rtl/lac/lac.v                   \
	../../rtl/lac/uart.v                  \
	../../rtl/lac/dp_ram.v                \
	../../rtl/lm32/lm32_cpu.v             \
	../../rtl/lm32/lm32_instruction_unit.v	\
	../../rtl/lm32/lm32_decoder.v         \
	../../rtl/lm32/lm32_simtrace.v        \
	../../rtl/lm32/lm32_load_store_unit.v \
	../../rtl/lm32/lm32_adder.v           \
	../../rtl/lm32/lm32_addsub.v          \
	../../rtl/lm32/lm32_logic_op.v        \
	../../rtl/lm32/lm32_shifter.v         \
	../../rtl/lm32/lm32_multiplier.v      \
	../../rtl/lm32/lm32_mc_arithmetic.v   \
	../../rtl/lm32/lm32_interrupt.v       \
	../../rtl/lm32/lm32_icache.v          \
	../../rtl/lm32/lm32_dcache.v          \
	../../rtl/lm32/lm32_ram.v             \
	../../rtl/wb_bram/wb_bram.v           \
	../../rtl/wb_uart/wb_uart.v           \
	../../rtl/wb_timer/wb_timer.v         \
	../../rtl/wb_gpio/wb_gpio.v           \
	../../rtl/wb_conbus/wb_conbus_top.v   \
	../../rtl/wb_conbus/wb_conbus_arb.v   \
	../../rtl/wb_farbborg/wb_farbborg.v   \
	../../rtl/wb_farbborg/dualportram.v   \
	../../rtl/wb_farbborg/pwm.v           \
	../../rtl/wb_sram/wb_sram16.v


#############################################################################
# Main targets
sim: system_tb.vcd
view: system_tb.view

#############################################################################
# Syntheis constants
SYNCLEAN     = *.rpt system.qsf *.chg smart.log *.htm *.eqn *.pin *.sof *.pof db
SYNCLEAN_ALL = *.ssf *.csf *.esf *.fsf *.psf *.quartus *.qws

MAP_ARGS = --family="Cyclone II"
FIT_ARGS = --part=EP2C20F484C7
ASM_ARGS =
TAN_ARGS =

#############################################################################
# Simulation constants
SIMCLEAN=system_tb.vvp system_tb.vcd verilog.log system_tb.vvp.list

CVER=cver
GTKWAVE=gtkwave
IVERILOG=iverilog
VVP=vvp
	
#############################################################################
# Ikarus verilog simulation
system_tb.vvp: $(SRC) $(SIM_SRC)
	rm -f $@.list
	@for i in `echo $^`; do \
	    echo $$i >> $@.list; \
	done
	$(IVERILOG) -o $@ $(VINCDIR) -c $@.list -s $(@:.vvp=)

%.vcd: %.vvp
	$(VVP) $<



################################################################
# Synthesis
#ASSIGNMENT_FILES = system.quartus system.psf system.csf
ASSIGNMENT_FILES = system.psf system.csf

system.qsf: $(SRC)
	@rm -f $@
	@cp -f preferences.qsf $@ 
	@for i in `echo $^`; do \
		echo "set_global_assignment -name VERILOG_FILE $$i" >> $@; \
	done

#system.prj: $(SRC) 
#	rm -f system.prj
#	@for i in `echo $^`; do \
#	    echo "set_global_assignment -name VERILOG_FILE $$i" >> system.prj; \
#	done


################################################################
# Main Targets
# 
# syn: build everything
# clean: remove output files and database
# clean_all: removes settings files as well as clean.
################################################################
syn: system.qsf smart.log $(PROJECT).asm.rpt $(PROJECT).tan.rpt

map: smart.log $(PROJECT).map.rpt
fit: smart.log $(PROJECT).fit.rpt
asm: smart.log $(PROJECT).asm.rpt
tan: smart.log $(PROJECT).tan.rpt
smart: smart.log

################################################################
# Target implementations
################################################################
STAMP = echo done >

$(PROJECT).map.rpt: map.chg $(SRC)
	quartus_map $(MAP_ARGS) $(PROJECT)
	$(STAMP) fit.chg

$(PROJECT).fit.rpt: fit.chg $(PROJECT).map.rpt
	quartus_fit $(FIT_ARGS) $(PROJECT)
	$(STAMP) asm.chg
	$(STAMP) tan.chg

$(PROJECT).asm.rpt: asm.chg $(PROJECT).fit.rpt
	quartus_asm $(ASM_ARGS) $(PROJECT)

$(PROJECT).tan.rpt: tan.chg $(PROJECT).fit.rpt
	quartus_tan $(TAN_ARGS) $(PROJECT)

smart.log: $(ASSIGNMENT_FILES)
	quartus_sh --determine_smart_action $(PROJECT) > smart.log

map.chg:
	$(STAMP)   map.chg

fit.chg:
	$(STAMP)   fit.chg

tan.chg:
	$(STAMP)   tan.chg

asm.chg:
	$(STAMP)   asm.chg

####################################################################
# final targets

%.view: %.vcd
	$(GTKWAVE) $< $<.save

clean:
	rm -Rf $(SYNCLEAN) $(SIMCLEAN)


