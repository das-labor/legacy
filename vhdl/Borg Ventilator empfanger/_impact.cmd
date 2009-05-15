setMode -bs
setMode -bs
setCable -port auto
Identify 
identifyMPM 
assignFile -p 2 -file "D:/Borg Ventilator empfanger/videodaten.bit"
Program -p 2 -defaultVersion 0 
Program -p 2 -defaultVersion 0 
Program -p 2 -defaultVersion 0 
setCable -port auto
setCable -port auto
setCable -port auto
Program -p 2 -defaultVersion 0 
Program -p 2 -defaultVersion 0 
Program -p 2 -defaultVersion 0 
setMode -pff
setMode -pff
addConfigDevice  -name "Untitled" -path "D:\Borg Ventilator empfanger\"
setSubmode -pffserial
setAttribute -configdevice -attr multibootBpiType -value ""
addDesign -version 0 -name ""
setMode -pff
addDeviceChain -index 0
setMode -pff
addDeviceChain -index 0
setAttribute -configdevice -attr compressed -value "FALSE"
setAttribute -configdevice -attr compressed -value "FALSE"
setAttribute -configdevice -attr autoSize -value "FALSE"
setAttribute -configdevice -attr fileFormat -value "mcs"
setAttribute -configdevice -attr fillValue -value "FF"
setAttribute -configdevice -attr swapBit -value "FALSE"
setAttribute -configdevice -attr dir -value "UP"
setAttribute -configdevice -attr multiboot -value "FALSE"
setAttribute -configdevice -attr multiboot -value "FALSE"
setAttribute -configdevice -attr spiSelected -value "FALSE"
setAttribute -configdevice -attr spiSelected -value "FALSE"
addPromDevice -p 1 -size 0 -name xcf02s
setMode -pff
setMode -pff
deletePromDevice -position 1
setCurrentDesign -version 0
setCurrentDeviceChain -index 0
setCurrentDeviceChain -index 0
deleteDesign -version 0
setCurrentDesign -version -1
setMode -pff
addConfigDevice -size 512 -name "Untitled" -path "D:\Borg Ventilator empfanger\/"
setSubmode -pffserial
setAttribute -configdevice -attr dir -value "UP"
setAttribute -configdevice -attr flashDataWidth -value "8"
addPromDevice -p 1 -size 0 -name xcf02s
setMode -pff
setSubmode -pffserial
setAttribute -configdevice -attr dir -value "UP"
addDesign -version 0 -name "0000"
setMode -pff
addDeviceChain -index 0
setCurrentDeviceChain -index 0
setCurrentDeviceChain -index 0
setAttribute -design -attr name -value "0"
addDevice -p 1 -file "D:/Borg Ventilator empfanger/videodaten.bit"
setMode -pff
setSubmode -pffserial
setAttribute -configdevice -attr fillValue -value "FF"
setAttribute -configdevice -attr fileFormat -value "mcs"
setAttribute -configdevice -attr dir -value "UP"
setAttribute -configdevice -attr path -value "D:\Borg Ventilator empfanger\/"
setAttribute -configdevice -attr name -value "Untitled"
setCurrentDesign -version 0
generate
setMode -bs
setMode -bs
assignFile -p 1 -file "D:/Borg Ventilator empfanger/Untitled.mcs"
setAttribute -position 1 -attr packageName -value "(null)"
Program -p 1 -e -v -defaultVersion 0 
Program -p 2 -defaultVersion 0 
setMode -pff
setMode -pff
setMode -pff
setSubmode -pffserial
setAttribute -configdevice -attr fillValue -value "FF"
setAttribute -configdevice -attr fileFormat -value "mcs"
setAttribute -configdevice -attr dir -value "UP"
setAttribute -configdevice -attr path -value "D:\Borg Ventilator empfanger\/"
setAttribute -configdevice -attr name -value "Untitled"
generate
setCurrentDesign -version 0
setMode -bs
setMode -bs
Program -p 1 -e -v -defaultVersion 0 
saveProjectFile -file "D:/Borg Ventilator empfanger/Borg Ventilator empfanger.ipf"
setMode -bs
deleteDevice -position 1
deleteDevice -position 1
setMode -ss
setMode -sm
setMode -hw140
setMode -spi
setMode -acecf
setMode -acempm
setMode -pff
deletePromDevice -position 1
setCurrentDesign -version 0
setCurrentDeviceChain -index 0
deleteDevice -position 1
setCurrentDeviceChain -index 0
deleteDesign -version 0
setCurrentDesign -version -1
setMode -pff
setMode -pff
setMode -bs
setMode -ss
setMode -sm
setMode -hw140
setMode -spi
setMode -acecf
setMode -acempm
setMode -pff
setMode -bs
setMode -pff
setMode -bs
