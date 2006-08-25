TARGET=Main
UCFFILE=Main.ucf
PART=xc3s500e-fg320-4
ROUTEEFFORT=std
BITGEN=default.ut


all: $(TARGET).bit

$(TARGET).ngc: *.vhd
	xst -ifn $(TARGET).xst

$(TARGET).ngd: $(TARGET).ngc
	ngdbuild $(TARGET).ngc -aul -uc $(UCFFILE) -p $(PART) -sd .

$(TARGET).ncd: $(TARGET).ngd
	map -pr b -p $(PART) $(TARGET)

placeandroute: $(TARGET).ncd
	par  -ol $(ROUTEEFFORT) -w $(TARGET) $(TARGET).ncd

$(TARGET).bit: $(TARGET).ncd placeandroute
	bitgen $(TARGET) -l -m -w -d -f $(BITGEN)

clean:
	rm -f *.lso netlist.lst $(TARGET)_usage.xml
	rm -f $(TARGET).bld $(TARGET).lso $(TARGET).bgn $(TARGET).bit
	rm -f $(TARGET).bld $(TARGET).ll $(TARGET).mrp $(TARGET).msd
	rm -f $(TARGET).msk $(TARGET).ncd $(TARGET).ngc $(TARGET).ngd
	rm -f $(TARGET).ngm $(TARGET).pad $(TARGET).par $(TARGET).pcf
	rm -f $(TARGET).rba $(TARGET).rbb $(TARGET).rbd $(TARGET).rbt
	rm -f $(TARGET).srp $(TARGET).unroutes $(TARGET).rbd $(TARGET).xpi
	rm -f $(TARGET)_pad.txt $(TARGET)_pad.csv $(TARGET).rbd $(TARGET).xpi

