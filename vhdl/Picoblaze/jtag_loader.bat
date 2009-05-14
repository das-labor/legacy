if exist .\hex2svf.cnf goto one
  echo Need to set up jtag chain first
  hex2svfsetup.exe
  echo jtag chain set up
  pause
:one
hex2svf %1.hex %1.svf
svf2xsvf -d -i %1.svf -o %1.xsvf
playxsvf %1.xsvf