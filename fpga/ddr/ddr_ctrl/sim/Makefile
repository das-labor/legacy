
sim: iverilog

####################################################################
# ikarus verilog simulation
system_tb.vvp: 
	iverilog -o system_tb.vvp -I../sim -I../rtl  -c system_tb.list

iverilog: system_tb.vvp
	vvp system_tb.vvp

####################################################################
# other targets

view: system_tb.vcd
	gtkwave system_tb.vcd system_tb.save

clean:
	rm -f *.vcd verilog.log system_tb.vvp
	

