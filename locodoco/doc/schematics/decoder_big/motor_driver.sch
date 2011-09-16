EESchema Schematic File Version 2  date Tue 06 Sep 2011 12:41:05 PM CEST
LIBS:power
LIBS:device
LIBS:transistors
LIBS:conn
LIBS:linear
LIBS:regul
LIBS:74xx
LIBS:cmos4000
LIBS:adc-dac
LIBS:memory
LIBS:xilinx
LIBS:special
LIBS:microcontrollers
LIBS:dsp
LIBS:microchip
LIBS:analog_switches
LIBS:motorola
LIBS:texas
LIBS:intel
LIBS:audio
LIBS:interface
LIBS:digital-audio
LIBS:philips
LIBS:display
LIBS:cypress
LIBS:siliconi
LIBS:opto
LIBS:atmel
LIBS:contrib
LIBS:valves
LIBS:decoder_v2-cache
EELAYER 25  0
EELAYER END
$Descr A4 11700 8267
encoding utf-8
Sheet 2 2
Title ""
Date "6 sep 2011"
Rev ""
Comp ""
Comment1 ""
Comment2 ""
Comment3 ""
Comment4 ""
$EndDescr
$Comp
L MOSFET_N QM3
U 1 1 4D8BA4BE
P 4000 2600
F 0 "QM3" H 4010 2770 60  0000 R CNN
F 1 "MOSFET_N" H 4010 2450 60  0000 R CNN
	1    4000 2600
	-1   0    0    -1  
$EndComp
Wire Wire Line
	3600 1950 3900 1950
Connection ~ 3400 2900
Wire Wire Line
	3900 2800 3900 2900
Wire Wire Line
	3900 2900 3000 2900
Connection ~ 3900 2200
Wire Wire Line
	3900 1950 3900 2400
Connection ~ 3000 2200
Wire Wire Line
	3300 2200 3000 2200
Wire Wire Line
	3000 2400 3000 1950
Wire Wire Line
	3900 2200 3500 2200
Wire Wire Line
	3000 2900 3000 2800
Wire Wire Line
	3400 2900 3400 2950
Wire Wire Line
	3000 1950 3200 1950
$Comp
L VCC #PWR?
U 1 1 4D8C7140
P 5400 1450
F 0 "#PWR?" H 5400 1550 30  0001 C CNN
F 1 "VCC" H 5400 1550 30  0000 C CNN
	1    5400 1450
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR?
U 1 1 4D8C713A
P 2450 900
F 0 "#PWR?" H 2450 1000 30  0001 C CNN
F 1 "VCC" H 2450 1000 30  0000 C CNN
	1    2450 900 
	1    0    0    -1  
$EndComp
$Comp
L VCC #PWR?
U 1 1 4D8C711F
P 3400 1050
F 0 "#PWR?" H 3400 1150 30  0001 C CNN
F 1 "VCC" H 3400 1150 30  0000 C CNN
	1    3400 1050
	1    0    0    -1  
$EndComp
$Comp
L C CM0
U 1 1 4D8C7108
P 3400 1950
F 0 "CM0" H 3450 2050 50  0000 L CNN
F 1 "C" H 3450 1850 50  0000 L CNN
	1    3400 1950
	0    1    1    0   
$EndComp
$Comp
L GND #PWR?
U 1 1 4D8C70E8
P 3400 2950
F 0 "#PWR?" H 3400 2950 30  0001 C CNN
F 1 "GND" H 3400 2880 30  0001 C CNN
	1    3400 2950
	1    0    0    -1  
$EndComp
Text HLabel 900  2950 0    60   Input ~ 0
MLN
Text HLabel 950  1500 0    60   Input ~ 0
MLP
Text HLabel 7450 1650 2    60   Input ~ 0
MRP
Text HLabel 6800 3850 2    60   Input ~ 0
MRN
$Comp
L R RM3
U 1 1 4D8C702A
P 7850 2550
F 0 "RM3" V 7930 2550 50  0000 C CNN
F 1 "R" V 7850 2550 50  0000 C CNN
	1    7850 2550
	1    0    0    -1  
$EndComp
$Comp
L R RM1
U 1 1 4D8C7018
P 6200 1250
F 0 "RM1" V 6280 1250 50  0000 C CNN
F 1 "R" V 6200 1250 50  0000 C CNN
	1    6200 1250
	1    0    0    -1  
$EndComp
$Comp
L MOSFET_P QM1
U 1 1 4D8BA4D9
P 4000 1750
F 0 "QM1" H 4000 1940 60  0000 R CNN
F 1 "MOSFET_P" H 4000 1570 60  0000 R CNN
	1    4000 1750
	-1   0    0    1   
$EndComp
$Comp
L CONN_2 PM0
U 1 1 4D8BA4B0
P 3400 2550
F 0 "PM0" V 3350 2550 40  0000 C CNN
F 1 "CONN_2" V 3450 2550 40  0000 C CNN
	1    3400 2550
	0    1    1    0   
$EndComp
$Comp
L R RM0
U 1 1 4D8BA404
P 1350 1100
F 0 "RM0" V 1430 1100 50  0000 C CNN
F 1 "R" V 1350 1100 50  0000 C CNN
	1    1350 1100
	1    0    0    -1  
$EndComp
$Comp
L R RM2
U 1 1 4D8BA400
P 2050 3800
F 0 "RM2" V 2130 3800 50  0000 C CNN
F 1 "R" V 2050 3800 50  0000 C CNN
	1    2050 3800
	1    0    0    -1  
$EndComp
$Comp
L MOSFET_N QM2
U 1 1 4D8BA3FD
P 2900 2600
F 0 "QM2" H 2910 2770 60  0000 R CNN
F 1 "MOSFET_N" H 2910 2450 60  0000 R CNN
	1    2900 2600
	1    0    0    -1  
$EndComp
$Comp
L MOSFET_P QM0
U 1 1 4D8BA3FA
P 2900 1750
F 0 "QM0" H 2900 1940 60  0000 R CNN
F 1 "MOSFET_P" H 2900 1570 60  0000 R CNN
	1    2900 1750
	1    0    0    1   
$EndComp
$EndSCHEMATC
