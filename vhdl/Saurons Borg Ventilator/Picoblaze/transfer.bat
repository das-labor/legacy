mem2hex picoblaze.mem picoblaze.hex
hex2svf picoblaze.hex picoblaze.svf
svf2xsvf -d -i picoblaze.svf -o picoblaze.xsvf
copy picoblaze.xsvf d:\borg_sender\sender
c:\Xilinx92i\bin\nt\impact.exe -batch impact_batch.txt

