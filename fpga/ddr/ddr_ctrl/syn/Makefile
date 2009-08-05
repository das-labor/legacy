XSTCLEAN=system.bgn system.drc system.mrp system.pcf 
XSTCLEAN+=system.bld *.lso *.ncd *.ngd *.ngm *.srp
XSTCLEAN+=system.bit system_signalbrowser.* system-routed_pad.tx
XSTCLEAN+=system.map system_summary.xml timing.twr
XSTCLEAN+=system-routed* system_usage* system.ngc param.opt netlist.lst
XSTCLEAN+=xst



syn: system.bit system-routed.twr

#############################################################################
# Xilinx Synthesis using XST
system.ngc:
	xst -ifn system.xst

system.ngd: system.ngc
	ngdbuild -uc system.ucf system.ngc

system.ncd: system.ngd
	map system.ngd

system-routed.ncd: system.ncd
	par -ol high -w system.ncd system-routed.ncd

system.bit: system-routed.ncd
	bitgen -w system-routed.ncd system.bit

system-routed.xdl: system-routed.ncd
	xdl -ncd2xdl system-routed.ncd system-routed.xdl

system-routed.twr: system-routed.ncd
	trce -v 10 system-routed.ncd system.pcf

timing: system-routed.twr

usage: system-routed.xdl
	xdlanalyze.pl system-routed.xdl 1


clean: 
	rm -Rf $(XSTCLEAN)

