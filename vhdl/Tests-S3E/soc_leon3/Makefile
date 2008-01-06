GHDL=ghdl
GHDLOPTS=--workdir=ghdlwork -Pghdlwork
GTKWAVE=/usr/local/bin/gtkwave

SYNCLEAN=System.bgn System.drc System.mrp System.ngd System.pcf 
SYNCLEAN+=System.bld System.lso System.ncd System.ngm System.srp
SYNCLEAN+=System.bit System_signalbrowser.* System-routed_pad.tx
SYNCLEAN+=System-routed* System_usage* System.ngc param.opt netlist.lst

VHDLSRC=components.vhd config.vhd
VHDLSRC+=Sim/wb_ddr.vhd Sim/ClkGen.vhd 
VHDLSRC+=wb_leon/ahb2wb.vhd wb_leon/wb_leon.vhd wb_leon/irqctrl.vhd
VHDLSRC+=wb32_wb64/wb32_wb64.vhd
VHDLSRC+=wb_gpio/wb_gpio.vhd
VHDLSRC+=wb_timer/wb_timer.vhd
VHDLSRC+=wb_uart/uart_rx.vhd wb_uart/uart_tx.vhd
VHDLSRC+=wb_uart/uart.vhd wb_uart/wb_uart.vhd
VHDLSRC+=wb_scope/wb_scope.vhd wb_scope/bram_dp.vhd wb_scope/mux32.vhd
VHDLSRC+=wb_bram.vhd
VHDLSRC+=System.vhd TestWBLeon.vhd



all: grlib TestWBLeon


#############################################################################
# TestWBLeon 
TestWBLeon: import
	$(GHDL) -m $(GHDLOPTS)  TestWBLeon

TestWBLeon-run:  
	$(GHDL) -r TestWBLeon --vcd=TestWBLeon.vcd --stop-time=50ms

TestWBLeon-view: 
	$(GTKWAVE) TestWBLeon.vcd TestWBLeon.sav


#############################################################################
# Synthesis
System.ngc:
	xst -ifn System.xst

System.ngd: System.ngc
	ngdbuild -uc System.ucf System.ngc

System.ncd: System.ngd
	map System.ngd

System-routed.ncd: System.ncd
	par -ol high -w System.ncd System-routed.ncd

System.bit: System-routed.ncd
	bitgen -w System-routed.ncd System.bit

Syn: System.ngc System.bit

SynClean: 
	rm -f $(SYNCLEAN)



ReSyn: SynClean Syn


#############################################################################
# general targets
import:
	$(GHDL) -i $(GHDLOPTS) $(VHDLSRC)
	$(GHDL) -a $(GHDLOPTS) $(VHDLSRC)

grlib:
	sh make-grlib.sh

clean: SynClean
#	$(GHDL) --remove $(GHDLOPTS)

realclean: clean
	rm -Rf ghdlwork/*

